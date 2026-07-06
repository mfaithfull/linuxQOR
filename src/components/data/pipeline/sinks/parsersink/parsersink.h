// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SINKS_PARSER
#define QOR_PP_H_COMPONENTS_PIPELINE_SINKS_PARSER

#include <string>
#include "src/framework/data/pipeline/sink.h"
#include "src/components/data/parser/parser.h"
#include "src/components/data/parser/state.h"
#include "src/components/data/parser/context.h"
#include "src/qor/flyers/log/debug.h"
#include "src/qor/flyers/interception/functioncontext.h"

namespace qor{ namespace pipeline{ namespace components{ 

    class qor_pp_module_interface(QOR_PARSERSINK) BaseParserSink : public pipeline::Sink
    {
    public:

        BaseParserSink();
        virtual ~BaseParserSink();
        qor::components::parser::Parser& Parser();
        void SetEOD();

    protected:
        
        bool Push(size_t& unitsWritten, size_t unitsToWrite);
        virtual size_t Parse(byte* data, size_t bytesToParse);

        qor::ref_of<qor::components::parser::Context>::type m_context;
        qor::components::parser::Parser m_parser;      
        bool m_EOD{false};
    };

    template<class TObjectState>
    class ParserSink : public BaseParserSink
    {
    public:

        ParserSink() : BaseParserSink(), m_objectState(new_ref<TObjectState>(&m_parser))
        {
            m_parser.SetInitialState(m_objectState);
        }

        ParserSink(const pipeline::Buffer& buffer) : ParserSink()
        {
            SetBuffer(buffer);
        }

        virtual ~ParserSink() = default;

        void Reset()
        {
            qor_pp_ofcontext;
            if(!m_EOD)
            {
                log::Debug("Parser Sink Reset");
                m_context->Reset();
                m_objectState = new_ref<TObjectState>(&m_parser);
                m_parser.SetInitialState(m_objectState);
            }
            else
            {
                log::Debug("EOD");
            }
        }

    protected:

        ref_of<TObjectState>::type m_objectState;

        virtual size_t Parse(byte* data, size_t bytesToParse)
        {
            qor_pp_ofcontext;
            log::debug("Routing {0} bytes to Parser.", bytesToParse);
            m_context->SetData(data, bytesToParse);                        
            m_parser.Parse();
            log::debug("Parser consumed {0} bytes.", m_context->GetPosition());
            return m_context->GetPosition();
        }
    };

}}}//qor::pipeline::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SINKS_PARSER
