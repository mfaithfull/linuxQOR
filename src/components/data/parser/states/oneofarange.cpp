// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/platform/compiler/compiler.h"
#include "oneofarange.h"
#include "../context.h"
#include "../parser.h"
#include "src/qor/flyers/log/debug.h"

namespace qor { namespace data { namespace parser {

    //Octets in a contiguous numerical range. Used for things like 0-9 digits and a-z letters
    OneOfAByteRange::OneOfAByteRange(Parser* parser, byte firstOctet, byte lastOctet, uint64_t token) : ParserState(parser, token),
        m_first(firstOctet), m_last(lastOctet)
    {
        m_context = dynamic_cast<ByteContext*>(GetParser()->GetContext());

        Enter = [this]()
            {
                Prepare();
                byte* data = nullptr;
                if (m_context->GetItem(data))
                {
                    m_result.m_position = m_context->GetPosition();
                    if ((*data >= m_first && *data <= m_last))
                    {                        
                        m_result.first = *data;                        
                        m_context->ConsumeItem();
                        m_result.token = m_token;
                        ++m_result.length;
                        m_result.code = Result::SUCCESS;
                    }
                    else
                    {      
                        m_context->RejectItem();                                         
                        m_result.code = Result::FAILURE;
                        m_result.length = 0;
                    }
                    Workflow()->PopStep();
                }
                else
                {
                    log::debug("Ran out of data looking for a range '{0:x02}'-'{1:x02}'", (char)m_first, (char)m_last);
                    m_result.code = Result::MORE_DATA;//Need more data
                    Fail();
                }
            };
    }

    OneOfAByteRange::~OneOfAByteRange() = default;

}}}//qor::data::parser
