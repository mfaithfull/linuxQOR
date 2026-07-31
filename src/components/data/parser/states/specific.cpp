// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "specific.h"
#include "../context.h"
#include "../parser.h"
#include "src/qor/flyers/log/debug.h"

namespace qor {	namespace data { namespace parser {

    //Matches one specific octet value e.g. '?'
    Specific::Specific(Parser* parser, byte matchingOctet, uint64_t token) : ParserState(parser, token),
        m_matchingOctet(matchingOctet)
    {
        Enter = [this]()
            {
                Prepare();

                byte* data = nullptr;
                if (GetContext()->GetOctet(data))
                {
                    if (*data == m_matchingOctet)
                    {
                        m_result.first = *data;
                        m_result.m_position = GetContext()->GetPosition();
                        GetContext()->ConsumeOctet();
                        m_result.token = m_token;
                        m_result.length = 1;
                        m_result.code = Result::SUCCESS;
                        log::debug("Matched '{0}'", (char)m_matchingOctet);
                    }
                    else if (data)
                    {
                        log::debug("{0} != {1}", (char)*data, (char)m_matchingOctet);
                        m_result.code = Result::FAILURE;
                        m_result.m_position = GetContext()->GetPosition();
                        m_result.length = 0;
                    }
                    Workflow()->PopStep();
                }
                else
                {
                    Fail();
                    m_result.code = Result::MORE_DATA;//Need more data to continue
                }
            };
    }

    Specific::~Specific() = default;

    void Specific::Emit() { }

}}}//qor::data::parser
