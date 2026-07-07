// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/platform/compiler/compiler.h"
#include "sequence.h"
#include "../context.h"
#include "../parser.h"

namespace qor { namespace components { namespace parser {

    //Matches a sequence beggining with head and followed by tail, which may itself be a sequence
    Sequence::Sequence(Parser* parser, ref_of<ParserState>::type head, ref_of<ParserState>::type tail, uint64_t token) : ParserState(parser, token),
        m_internalState(0), m_head(head), m_tail(tail)
    {
        Enter = [this]()
            {
                Prepare();
                m_internalState = 0;
                Workflow()->PushState(m_head);
            };

        Resume = [this]()
            {
                switch (m_internalState)
                {
                case 0://head
                    if (m_head->m_result.code == Result::FAILURE)
                    {
                        m_result.code = m_head->m_result.code;
                        m_result.length = 0;
                        m_result.m_position = m_head->m_result.m_position;
                        Workflow()->PopState();
                    }
                    else if (m_head->m_result.code == Result::SUCCESS)
                    {
                        m_result.first = m_head->m_result.first;
                        m_result.length = m_head->m_result.length;
                        m_result.m_position = m_head->m_result.m_position;
                        m_internalState = 1;
                        Workflow()->PushState(m_tail);
                    }
                    break;
                case 1://tail
                    if (m_tail->m_result.code == Result::SUCCESS)
                    {
                        m_result.code = Result::SUCCESS;
                        m_result.length = m_head->m_result.length + m_tail->m_result.length;
                        m_result.token = m_token;
                        Workflow()->PopState();
                    }
                    else if (m_tail->m_result.code == Result::FAILURE)
                    {
                        m_result.code = Result::FAILURE;
                        m_result.length = 0;
                        Workflow()->PopState();
                    }
                    else
                    {
                        if (GetParser()->IsFinal())//Try to succeed
                        {
                            m_result.code = Result::SUCCESS;
                            m_result.length = m_head->m_result.length + m_tail->m_result.length;
                            m_result.token = m_token;
                            Workflow()->PopState();
                        }
                    }
                    break;
                }
            };
    }

    Sequence::~Sequence() = default;
    
}}}//qor::components::parser
