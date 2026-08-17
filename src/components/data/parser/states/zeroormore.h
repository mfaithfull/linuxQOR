// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_STATES_ZEROORMORE
#define QOR_PP_H_DATA_PARSER_STATES_ZEROORMORE

#include "../state.h"

namespace qor { namespace data { namespace parser {

    //Matches zero or more of the head state
    class qor_pp_module_interface(QOR_PARSER) ZeroOrMore : public ParserState
    {
    public:

        ZeroOrMore(Parser* parser, ref_of<ParserState>::type head, uint64_t token = static_cast<uint64_t>(eToken::Lexical));
        virtual ~ZeroOrMore();

    private:

        Fastflow* m_flow;
        data::AbstractDataContext* m_context;
        ref_of<ParserState>::type m_head;
        bool m_first;
    };

    template< class head_t >
    class ZeroOrMore_t : public ParserState
    {
    public:

        ZeroOrMore_t(Parser* parser, head_t* head, uint64_t token = static_cast<uint64_t>(eToken::Lexical)) : ParserState(parser, token), 
            m_head(head)
        {
            m_first = true;
            m_result.length = 0;
            m_flow = Workflow();            
            Enter = [this]()
                {
                    Prepare();
                    m_flow->PushStep(m_head);
                };

            Resume = [this]()
                {               
                    m_context = GetParser()->GetContext(); 
                    m_result.code = m_head->m_result.code;                
                    if (m_head->m_result.code == Result::SUCCESS  && m_context->HasData())
                    {
                        if (m_first)
                        {
                            m_result.first = m_head->m_result.first;
                            m_result.m_position = m_head->m_result.m_position;
                            m_first = false;
                        }
                        m_result.length += m_head->m_result.length;
                        m_head->Reset();
                        m_flow->PushStep(m_head);
                    }
                    else
                    {
                        m_first = true;
                        if(m_context->HasData() || GetParser()->IsFinal())
                        {
                            m_flow->PopStep();
                        }
                        return;                    
                    }
                };

            Leave = [this]()
                {
                    m_result.token = m_token;
                    if (m_result.code == Result::FAILURE)
                    {
                        Fail();
                    }
                    m_result.code = Result::SUCCESS;
                };

        }

        virtual ~ZeroOrMore_t() = default;

    private:

        Fastflow* m_flow;
        data::AbstractDataContext* m_context;
        head_t* m_head;
        bool m_first;
    };

}}}//qor::data::parser

namespace qor{
    qor_pp_declare_source_of(data::parser::ZeroOrMore, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::ZeroOrMore>::type, memory::FastSource)
}

#endif//QOR_PP_H_DATA_PARSER_STATES_ZEROORMORE
