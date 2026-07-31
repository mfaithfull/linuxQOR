// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/platform/compiler/compiler.h"
#include "oneormore.h"
#include "../context.h"
#include "../parser.h"

namespace qor { namespace data { namespace parser {

    //Matches one or more of the head state. Must be at least one, can be any number. First non match breaks the sequence
    OneOrMore::OneOrMore(Parser* parser, ref_of<ParserState>::type head, uint64_t token) : ParserState(parser,token),
        m_head(head), m_first(true)
    {
        m_first = true;
        m_result.length = 0;
        Enter = [this]()
        {
            Prepare();
            Workflow()->PushStep(m_head.AsRef<fastflow::Step>());
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
                Workflow()->PushStep(m_head.AsRef<fastflow::Step>());
            }
            else if(m_head->m_result.code == Result::FAILURE)
            {
                if(m_first)
                {
                    m_result.code = Result::FAILURE;
                    m_result.m_position = m_head->m_result.m_position;
                }
                m_first = true;                
                Workflow()->PopStep();
            }
            else
            {
                Fail();
                return;                
            }
        };
    }

    OneOrMore::~OneOrMore() = default;
    
}}}//qor::data::parser
