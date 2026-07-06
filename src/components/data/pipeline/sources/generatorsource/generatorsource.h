// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_GENERATOR
#define QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_GENERATOR

#include <string>
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

        virtual ~GeneratorSource() = default;

    protected:

        ref_of<TObjectState>::type m_objectState;

        virtual size_t Generate(byte* space, size_t bytesToParse)
        {
            m_context->SetSpace(space, bytesToParse);                        
            m_generator.Generate();
            return m_context->GetPosition();
        }
    };

}}}//qor::pipeline::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_GENERATOR
