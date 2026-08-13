// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_STATES_ANYONEOF
#define QOR_PP_H_DATA_PARSER_STATES_ANYONEOF

#include "../state.h"

namespace qor { namespace data { namespace parser {

    //Must match either head or tail to succeed
    class qor_pp_module_interface(QOR_PARSER) AnyOneOf : public ParserState
    {
    public:

        AnyOneOf(Parser * parser, ref_of<ParserState>::type head, ref_of<ParserState>::type tail, uint64_t token = static_cast<uint64_t>(eToken::Lexical));
        virtual ~AnyOneOf();

    private:

        unsigned int m_internalState;
        ref_of<ParserState>::type m_head;
        ref_of<ParserState>::type m_tail;
    };

    template< class head_t, class tail_t >
    class AnyOneOf_t : public ParserState
    {
    public:

        AnyOneOf_t(Parser* parser, head_t* head, tail_t* tail, uint64_t token = static_cast<uint64_t>(eToken::Lexical)) : ParserState(parser, token),
        m_internalState(0), m_head(head), m_tail(tail)
        {
            m_internalState = 0;
            Enter = [this]()
                {
                    Prepare();
                    if(m_internalState == 0)
                    {
                        Workflow()->PushStep(m_head);
                    }
                    else if(m_internalState == 1)
                    {
                        Workflow()->PushStep(m_tail);
                    }
                };

            Resume = [this]()
                {
                    switch (m_internalState)
                    {
                    case 0:
                        if (m_head->m_result.code == Result::SUCCESS)
                        {
                            m_result.code = Result::SUCCESS;
                            m_result.first = m_head->m_result.first;
                            m_result.length = m_head->m_result.length;
                            m_result.token = m_token;
                            m_result.m_position = m_head->m_result.m_position;
                            Workflow()->PopStep();
                        }
                        else if (m_head->m_result.code == Result::MORE_DATA)
                        {
                            Fail();
                            return;                        
                        }
                        else
                        {
                            m_internalState = 1;
                            Workflow()->PushStep(m_tail);
                        }
                        break;
                    case 1:
                        if (m_tail->m_result.code == Result::SUCCESS)
                        {
                            m_result.code = Result::SUCCESS;
                            m_result.first = m_tail->m_result.first;
                            m_result.length = m_tail->m_result.length;
                            m_result.token = m_token;
                            m_result.m_position = m_tail->m_result.m_position;
                            m_internalState = 0;
                            Workflow()->PopStep();
                        }
                        else if (m_head->m_result.code == Result::MORE_DATA)
                        {
                            Fail();
                            return;                        
                        }
                        else
                        {
                            m_internalState = 0;
                            m_result.m_position = m_head->m_result.m_position;
                            m_result.code = Result::FAILURE;
                            Workflow()->PopStep();
                        }
                        break;
                    }
                };
        }

        virtual ~AnyOneOf_t() = default;

    private:

        unsigned int m_internalState;
        head_t* m_head;
        tail_t* m_tail;

    };
}}}//qor::data::parser

namespace qor{
    qor_pp_declare_source_of(data::parser::AnyOneOf, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::AnyOneOf>::type, memory::FastSource)
}

#endif//QOR_PP_H_DATA_PARSER_STATES_ANYONEOF
