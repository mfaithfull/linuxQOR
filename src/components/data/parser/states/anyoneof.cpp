// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "anyoneof.h"
#include "../context.h"
#include "../parser.h"
#include "src/qor/flyers/log/debug.h"

namespace qor { namespace components { namespace parser {

    //Matches any one of a set of child states, represented by a head state and a tail state which may also be compound states
    AnyOneOf::AnyOneOf(Parser* parser, ref_of<ParserState>::type head, ref_of<ParserState>::type tail, uint64_t token) : ParserState(parser, token),
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
                case 0:
                    if (m_head->m_result.code == Result::SUCCESS)
                    {
                        m_result.code = Result::SUCCESS;
                        m_result.first = m_head->m_result.first;
                        m_result.length = m_head->m_result.length;
                        m_result.token = m_token;
                        m_result.m_position = m_head->m_result.m_position;
                        Workflow()->PopState();
                    }
                    else if (m_head->m_result.code == Result::MORE_DATA)
                    {
                        Workflow()->PopState();
                    }
                    else
                    {
                        m_internalState = 1;
                        Workflow()->PushState(m_tail);
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
                        Workflow()->PopState();
                    }
                    else if (m_head->m_result.code == Result::MORE_DATA)
                    {
                        Workflow()->PopState();
                    }
                    else
                    {
                        m_result.m_position = m_head->m_result.m_position;
                        m_result.code = Result::FAILURE;
                        Workflow()->PopState();
                    }
                    break;
                }
            };
    }

    AnyOneOf::~AnyOneOf() = default;

}}}//qor::components::parser
