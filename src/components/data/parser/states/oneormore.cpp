// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/platform/compiler/compiler.h"
#include "oneormore.h"
#include "../context.h"
#include "../parser.h"

namespace qor { namespace components { namespace parser {

    //Matches one or more of the head state. Must be at least one, can be any number. First non match breaks the sequence
    OneOrMore::OneOrMore(Parser* parser, ref_of<ParserState>::type head, uint64_t token) : ParserState(parser,token),
        m_head(head), m_first(true)
    {
        Enter = [this]()
        {
            Prepare();
            m_first = true;
            m_result.length = 0;
            Workflow()->PushState(m_head.AsRef<workflow::State>());
        };

        Resume = [this]()
        {            
            if(m_head->m_result.code == Result::SUCCESS)
            {
                if(m_first)
                {
                    m_result.first = m_head->m_result.first;
                    m_result.m_position = m_head->m_result.m_position;
                    m_first = false;
                }
                m_result.code = Result::SUCCESS;
                m_result.token = m_token;
                m_result.length += m_head->m_result.length;
                m_head->Reset();
                Workflow()->PushState(m_head.AsRef<workflow::State>());
            }
            else if(m_head->m_result.code == Result::FAILURE)
            {
                if(m_first)
                {
                    m_result.code = Result::FAILURE;
                    m_result.m_position = m_head->m_result.m_position;
                }
                Workflow()->PopState();
            }
            else
            {
                Workflow()->PopState();
            }
        };
    }

    OneOrMore::~OneOrMore() = default;
    
}}}//qor::components::parser
