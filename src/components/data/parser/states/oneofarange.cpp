// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/platform/compiler/compiler.h"
#include "oneofarange.h"
#include "../context.h"
#include "../parser.h"
#include "src/qor/flyers/log/debug.h"

namespace qor { namespace components { namespace parser {

    //Octets in a contiguous numerical range. Used for things like 0-9 digits and a-z letters
    OneOfARange::OneOfARange(Parser* parser, byte firstOctet, byte lastOctet, uint64_t token) : ParserState(parser, token),
        m_first(firstOctet), m_last(lastOctet)
    {
        Enter = [this]()
            {
                Prepare();

                byte* data = nullptr;
                if (GetContext()->GetOctet(data))
                {
                    if ((*data >= m_first && *data <= m_last))
                    {
                        //log::debug("Matched '{0:x02}' in range '{1:x02}'-'{2:x02}'", (char)*data, (char)m_first, (char)m_last);
                        m_result.first = *data;
                        m_result.m_position = GetContext()->GetPosition();
                        GetContext()->ConsumeOctet();
                        m_result.token = m_token;
                        ++m_result.length;
                        m_result.code = Result::SUCCESS;
                    }
                    else
                    {
                        //log::debug("Did not match '{0:x02}' in range '{1:x02}'-'{2:x02}'", (char)*data, (char)m_first, (char)m_last);
                        m_result.m_position = GetContext()->GetPosition();
                        m_result.code = Result::FAILURE;
                        m_result.length = 0;
                    }
                    Workflow()->PopState();
                }
                else
                {
                    log::debug("Ran out of data looking for a range '{0:x02}'-'{1:x02}'", (char)m_first, (char)m_last);
                    m_result.code = Result::MORE_DATA;//Need more data
                    Fail();
                }
            };
    }

    OneOfARange::~OneOfARange() = default;

}}}//qor::components::parser
