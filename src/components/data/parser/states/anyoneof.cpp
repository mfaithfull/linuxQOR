// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "anyoneof.h"
#include "../context.h"
#include "../parser.h"
#include "src/qor/flyers/log/debug.h"

namespace qor { namespace data { namespace parser {

    //Matches any one of a set of child states, represented by a head state and a tail state which may also be compound states
    AnyOneOf::AnyOneOf(Parser* parser, ref_of<ParserState>::type head, ref_of<ParserState>::type tail, uint64_t token) : ParserState(parser, token),
    m_internalState(0), m_head(head), m_tail(tail)
    {
        m_internalState = 0;
        Enter = [this]()
            {
                Prepare();                
                if(m_internalState == 0)
                {
                    //log::debug("Looking for first of two options.");
                    Workflow()->PushStep(m_head.AsRef<fastflow::Step>());
                }
                else if(m_internalState == 1)
                {
                    //log::debug("Looking for second of two options.");
                    Workflow()->PushStep(m_tail.AsRef<fastflow::Step>());
                }
            };

        Resume = [this]()
            {
                switch (m_internalState)
                {
                case 0:
                    if (m_head->m_result.code == Result::SUCCESS)
                    {
                        //log::debug("Found first of two options.");
                        m_result.code = Result::SUCCESS;
                        m_result.first = m_head->m_result.first;
                        m_result.length = m_head->m_result.length;
                        m_result.token = m_token;
                        m_result.m_position = m_head->m_result.m_position;
                        Workflow()->PopStep();
                    }
                    else if (m_head->m_result.code == Result::MORE_DATA)
                    {
                        //log::debug("Ran out of data.");
                        Fail();
                        return;                        
                    }
                    else
                    {                        
                        m_internalState = 1;
                        Workflow()->PushStep(m_tail.AsRef<fastflow::Step>());
                    }
                    break;
                case 1:
                    if (m_tail->m_result.code == Result::SUCCESS)
                    {
                        //log::debug("Found second of two options.");
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
                        //log::debug("Ran out of data looking for second of two options.");
                        Fail();
                        return;                        
                    }
                    else
                    {
                        //log::debug("Failed to find either of two options.");
                        m_internalState = 0;
                        m_result.m_position = m_head->m_result.m_position;
                        m_result.code = Result::FAILURE;
                        Workflow()->PopStep();
                    }
                    break;
                }
            };
    }

    AnyOneOf::~AnyOneOf() = default;

}}}//qor::data::parser
