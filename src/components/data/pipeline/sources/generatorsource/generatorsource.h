// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_GENERATOR
#define QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_GENERATOR

#include <string>
#include <utility>
#include "src/framework/data/pipeline/iosource.h"
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"
#include "src/components/io/generator/context.h"

namespace qor{ namespace pipeline{ namespace components{ 

    class qor_pp_module_interface(QOR_GENERATORSOURCE) BaseGeneratorSource : public pipeline::Source
    {

    public:

        BaseGeneratorSource();
        virtual ~BaseGeneratorSource();

        qor::components::generator::Generator& Generator()
        {
            return m_generator;
        }

    protected:
        
        bool Pull(size_t& unitsRead, size_t unitsToRead);        
        //virtual size_t ReadBytes(byte* space, size_t bytesToRead) = 0;

        qor::ref_of<qor::components::generator::Context>::type m_context;
        qor::components::generator::Generator m_generator;
    };

    template<class TObjectState>
    class GeneratorSource : public BaseGeneratorSource
    {
    public:

        GeneratorSource() : BaseGeneratorSource(), m_objectState(new_ref<TObjectState>(&m_generator))
        {
            m_generator.SetInitialState(m_objectState);
        }        

        //Pipeline driven construction. Mirrors ParserSink(const pipeline::Buffer&).
        //The source owns the output Context that the generator writes into and
        //reserves the space for each pull. Any extra arguments are the model data
        //the initial state was built from, so that a model driven state such as
        //json::_value(Generator*, Value*) can be created for the model.
        template<typename... TArgs>
        GeneratorSource(const pipeline::Buffer& buffer, TArgs&&... args) : BaseGeneratorSource(), m_objectState(new_ref<TObjectState>(&m_generator, std::forward<TArgs>(args)...))
        {
            SetBuffer(buffer);
            m_context = new_ref<qor::components::generator::Context>();
            m_generator.SetContext(m_context);
            m_generator.SetInitialState(m_objectState);
        }

        virtual ~GeneratorSource() = default;

    protected:

        ref_of<TObjectState>::type m_objectState;

        virtual size_t Generate(byte* space, size_t bytesToParse)
        {
            m_context->SetSpace(space, bytesToParse);                        
            m_generator.Generate();
            return m_context->GetPosition();
        }

        //The pipeline::Source hook. BaseGeneratorSource::Pull reserves space in
        //the pipeline buffer and asks us to fill it. We generate into that space
        //and report the number of bytes produced. Once the generator has run the
        //state stack down to nothing we report no data, which ends the pipeline.
        //Generation that runs out of space mid state leaves the state stack intact,
        //so the next pull resumes the suspended state at its saved position.
        virtual size_t ReadBytes(byte* space, size_t bytesToRead)
        {
            if(m_context.IsNull() || m_generator.IsComplete())
            {
                return 0;
            }
            return Generate(space, bytesToRead);
        }
    };

}}}//qor::pipeline::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_GENERATOR
