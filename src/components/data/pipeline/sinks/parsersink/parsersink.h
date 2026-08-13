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

    class qor_pp_module_interface_gcc(QOR_PARSERSINK) BaseParserSink : public pipeline::Sink
    {
    public:

        qor_pp_module_interface(QOR_PARSERSINK) BaseParserSink();
        qor_pp_module_interface(QOR_PARSERSINK) virtual ~BaseParserSink();
        qor_pp_module_interface(QOR_PARSERSINK) qor::data::Parser& Parser();
        qor_pp_module_interface(QOR_PARSERSINK) void SetEOD();

    protected:
        
        qor_pp_module_interface(QOR_PARSERSINK) bool Push(size_t& unitsWritten, size_t unitsToWrite);
        qor_pp_module_interface(QOR_PARSERSINK) virtual size_t Parse();

        ref_of<data::AbstractDataContext>::type m_sourceContext;
        data::Parser m_parser;      
        bool m_EOD{false};
    };

    template<class TObjectState>
    class ParserSink : public BaseParserSink
    {
    public:

        ParserSink() : BaseParserSink(), m_objectState(new_ref<TObjectState>(&m_parser))
        {
            m_parser.SetInitialStep(m_objectState);
        }

        ParserSink(const pipeline::Buffer& buffer) : ParserSink()
        {
            SetBuffer(buffer);
            m_sourceContext = new_ref<data::parser::CodePointContext>(&buffer);
            m_parser.SetContext(m_sourceContext);
        }

        virtual ~ParserSink() = default;

        void Reset()
        {
            qor_pp_ofcontext;
            if(!m_EOD)
            {
                m_sourceContext->Reset();
                m_objectState = new_ref<TObjectState>(&m_parser);
                m_parser.SetInitialStep(m_objectState);
            }
            else
            {
                log::Debug("EOD");
            }
        }

    protected:

        ref_of<TObjectState>::type m_objectState;

        virtual size_t Parse()
        {
            qor_pp_ofcontext;
            m_parser.Parse();
            log::debug("Parser consumed {0} bytes.", m_sourceContext->GetPosition());
            return m_sourceContext->GetPosition();
        }
    };

}}}//qor::pipeline::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SINKS_PARSER
