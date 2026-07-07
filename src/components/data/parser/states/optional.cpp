// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/platform/compiler/compiler.h"
#include "optional.h"
#include "../context.h"
#include "../parser.h"

namespace qor { namespace components { namespace parser {

    //Matches the head state if it's present. If it's not that's fine, we just move on.
    Optional::Optional(Parser* parser, ref_of<ParserState>::type head, uint64_t token) : ParserState(parser, token),
        m_head(head), m_first(true)
    {
        Enter = [this]()
            {
                Prepare();
                Workflow()->PushState(m_head.AsRef<workflow::State>());
            };

        Resume = [this]()
            {
                m_result.length = 0;
                if (m_head->m_result.code == Result::SUCCESS && m_head->m_result.length > 0)
                {
                    m_result.first = m_head->m_result.first;
                    m_result.length += m_head->m_result.length;
                    m_result.token = m_head->m_result.token;
                }
                m_result.m_position = m_head->m_result.m_position;
                m_result.code = m_head->m_result.code;
                Workflow()->PopState();
            };

        Leave = [this]()
            {
                if (m_result.code == Result::FAILURE)
                {
                    Fail();
                }
                else if (m_result.code == Result::SUCCESS && m_result.length > 0 && m_result.token != static_cast<uint64_t>(eToken::Lexical))
                {
                    if (m_token == static_cast<uint64_t>(eToken::Lexical) && m_result.token != static_cast<uint64_t>(eToken::Lexical))
                    {
                        m_token = m_result.token;
                    }
                    Emit();
                }
                m_result.code = Result::SUCCESS;
            };

    }

    Optional::~Optional() = default;
    
}}}//qor::components::parser
