// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/platform/compiler/compiler.h"
#include "zeroormore.h"
#include "../context.h"
#include "../parser.h"

namespace qor { namespace data { namespace parser {

    //Matches Zero or more sequential instances of the head state. None is fine. There's no limit except memory
    ZeroOrMore::ZeroOrMore(Parser* parser, ref_of<ParserState>::type head, uint64_t token) : ParserState(parser, token),
        m_head(head), m_first(true)
    {
        m_first = true;
        m_result.length = 0;
        Enter = [this]()
            {
                Prepare();
                Workflow()->PushState(m_head);
            };

        Resume = [this]()
            {                
                m_result.code = m_head->m_result.code;                
                if (m_head->m_result.code == Result::SUCCESS && GetParser()->GetContext()->HasUnparsedData())
                {
                    if (m_first)
                    {
                        m_result.first = m_head->m_result.first;
                        m_result.m_position = m_head->m_result.m_position;
                        m_first = false;
                    }
                    m_result.length += m_head->m_result.length;
                    m_head->Reset();
                    Workflow()->PushState(m_head);
                }
                else
                {
                    m_first = true;
                    Fail();
                    if(GetParser()->GetContext()->HasUnparsedData())
                    {
                        Workflow()->PopState();
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
                else if (m_result.code == Result::SUCCESS)
                {                    
                    //Emit();
                }
                m_result.code = Result::SUCCESS;
            };

    }

    ZeroOrMore::~ZeroOrMore() = default;
    
}}}//qor::data::parser
