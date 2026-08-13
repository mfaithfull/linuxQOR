// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_STATE
#define QOR_PP_H_DATA_PARSER_STATE

#include <iostream>
#include "src/platform/compiler/compiler.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/app/workflow/fastflow.h"
#include "result.h"
#include "node.h"
#include "tokens.h"
#include "context.h"

namespace qor { namespace data { 

    class qor_pp_module_interface(QOR_PARSER) Parser;

    namespace parser {
        
    class qor_pp_module_interface(QOR_PARSER) ParserState : public fastflow::Step
    {
    public:

        ParserState(Parser* parser, uint64_t token = static_cast<uint64_t>(eToken::Lexical));
        virtual ~ParserState();
        virtual void Reset();
        uint64_t GetToken();
        Parser* GetParser();
        data::AbstractDataContext* GetContext();

        Result m_result;

    protected:

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();

        Fastflow* Workflow();
        uint64_t m_token{0};
    };

    template<typename item_t>
    class AcceptAll : public ParserState
    {
    public:
        //Will literally accept anything. Don't use this in practice as it will consume all the rest of any data. It's useful for testing
        AcceptAll(Parser* parser) : ParserState(parser)
        {
            Enter = [this]()
            {            
                Prepare();
                item_t* data = nullptr;
                m_result.code = Result::SUCCESS;
                if(GetContext()->GetItem(data))
                {
                    m_result.first = *data;
                    m_result.m_position = GetContext()->GetPosition();
                    GetContext()->ConsumeItem();
                    m_result.token = static_cast<uint64_t>(eToken::Octet);
                    ++m_result.length;
                }
                else
                {
                    return;                
                }
            };
        }
        virtual ~AcceptAll() = default;
    };

 


}}}//qor::data::parser

namespace qor{
    qor_pp_declare_source_of(data::parser::ParserState, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::ParserState>::type, memory::FastSource)

}

#endif//QOR_PP_H_DATA_PARSER_STATE