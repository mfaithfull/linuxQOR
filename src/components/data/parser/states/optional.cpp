// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/platform/compiler/compiler.h"
#include "src/qor/flyers/log/debug.h"
#include "optional.h"
#include "../context.h"
#include "../parser.h"

namespace qor { namespace data { namespace parser {

    //Matches the head state if it's present. If it's not that's fine, we just move on.
    Optional::Optional(Parser* parser, ref_of<ParserState>::type head, uint64_t token) : ParserState(parser, token),
        m_head(head), m_first(true)
    {
        Enter = [this]()
            {
                Prepare();
                log::debug("Looking for Optional item...");
                Workflow()->PushStep(m_head.AsRef<fastflow::Step>());
            };

        Resume = [this]()
            {                
                if(m_head->m_result.code == Result::MORE_DATA)
                {
                    log::debug("Ran out data looking for Optional item");
                    if(GetParser()->IsFinal())
                    {
                        m_result.code = Result::FAILURE;
                        Workflow()->PopStep();
                    }
                    return;
                }
                m_result.length = 0;
                if (m_head->m_result.code == Result::SUCCESS && m_head->m_result.length > 0)
                {
                    log::debug("Optional item found.");
                    m_result.first = m_head->m_result.first;
                    m_result.length += m_head->m_result.length;
                    m_result.token = m_head->m_result.token;
                }
                m_result.m_position = m_head->m_result.m_position;
                m_result.code = m_head->m_result.code;
                Workflow()->PopStep();
            };

        Leave = [this]()
            {
                if (m_result.code == Result::FAILURE)
                {
                    log::debug("Optional item not found.");
                    Fail();
                }
                else if (m_result.code == Result::SUCCESS && m_result.length > 0 && m_result.token != static_cast<uint64_t>(eToken::Lexical))
                {
                    if (m_token == static_cast<uint64_t>(eToken::Lexical) && m_result.token != static_cast<uint64_t>(eToken::Lexical))
                    {
                        m_token = m_result.token;
                    }                    
                }
                m_result.code = Result::SUCCESS;
            };

    }

    Optional::~Optional() = default;
    
}}}//qor::data::parser
