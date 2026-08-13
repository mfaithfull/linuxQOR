// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DATA_PARSER_STATES_SEQUENCE
#define QOR_PP_H_DATA_PARSER_STATES_SEQUENCE

#include "../state.h"

namespace qor { namespace data { namespace parser {

    //Matches head followed by tail
    class qor_pp_module_interface(QOR_PARSER) Sequence : public ParserState
    {
    public:

        Sequence(Parser * parser, ref_of<ParserState>::type head, ref_of<ParserState>::type tail, uint64_t token = static_cast<uint64_t>(eToken::Lexical));
        virtual ~Sequence();

        virtual void Prepare();

    private:

        unsigned int m_internalState{0};
        ref_of<ParserState>::type m_head;
        ref_of<ParserState>::type m_tail;
    };

    //Matches head followed by tail

    template< class head_t, class tail_t >
    class Sequence_t : public ParserState
    {
    public:

        Sequence_t(Parser* parser, head_t* head, tail_t* tail, uint64_t token = static_cast<uint64_t>(eToken::Lexical)) : ParserState(parser, token),
            m_head(head), m_tail(tail)
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
                case 0://head
                    if(m_head->m_result.code == Result::FAILURE)
                    {
                        m_result.code = m_head->m_result.code;
                        m_result.length = 0;
                        m_result.m_position = m_head->m_result.m_position;
                        Workflow()->PopStep();
                    }
                    else if(m_head->m_result.code == Result::SUCCESS)
                    {
                        m_result.first = m_head->m_result.first;
                        m_result.length = m_head->m_result.length;
                        m_result.m_position = m_head->m_result.m_position;
                        m_internalState = 1;
                        Workflow()->PushStep(m_tail);
                    }
                    else
                    {
                        Fail();
                        return;
                    }
                    break;
                case 1://tail
                    if (m_tail->m_result.code == Result::SUCCESS)
                    {
                        m_result.code = Result::SUCCESS;
                        m_result.length = m_head->m_result.length + m_tail->m_result.length;
                        m_result.token = m_token;
                        m_internalState = 0;
                        Workflow()->PopStep();
                    }
                    else if (m_tail->m_result.code == Result::FAILURE)
                    {
                        m_result.code = Result::FAILURE;
                        m_result.length = 0;
                        m_internalState = 0;
                        Workflow()->PopStep();
                    }
                    else
                    {
                        if (GetParser()->IsFinal())//Try to succeed
                        {
                            m_result.code = Result::SUCCESS;
                            m_result.length = m_head->m_result.length + m_tail->m_result.length;
                            m_result.token = m_token;
                            m_internalState = 0;
                            Workflow()->PopStep();
                        }
                    }
                    break;
                }
            };
        }

        virtual ~Sequence_t() = default;

    private:

        unsigned int m_internalState{0};
        head_t* m_head;
        tail_t* m_tail;
    };

}}}//qor::data::parser

namespace qor{
    qor_pp_declare_source_of(data::parser::Sequence, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<data::parser::Sequence>::type, memory::FastSource)
}

#endif//QOR_PP_H_DATA_PARSER_STATES_SEQUENCE
