// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cstddef>
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"
#include "src/components/io/generator/result.h"
#include "_string.h"

namespace qor { namespace data { namespace generator { namespace json {

    _string::_string(qor::components::generator::Generator* generator, const char* text, size_t length) :
        qor::components::generator::GeneratorState(generator),
        m_text(),
        m_length(0),
        m_position(0)
    {
        // Build the escaped string up front so that Enter only has to walk over
        // the characters that have already been produced.
        auto hexDigit = [](byte nibble)->byte
        {
            return static_cast<byte>(nibble < 0xa
                ? static_cast<char>('0' + nibble)
                : static_cast<char>('a' + (nibble - 0xa)));
        };

        m_text.reserve(length + 2);
        m_text.push_back(static_cast<byte>('"'));

        for(size_t index(0); index < length; ++index)
        {
            const byte ch(static_cast<byte>(text[index]));

            switch(ch)
            {
                case static_cast<byte>('"'):
                {
                    m_text.push_back(static_cast<byte>('\\'));
                    m_text.push_back(ch);
                    break;
                }
                case static_cast<byte>('\\'):
                {
                    m_text.push_back(ch);
                    m_text.push_back(ch);
                    break;
                }
                case 0x8:
                {
                    m_text.push_back(static_cast<byte>('\\'));
                    m_text.push_back(static_cast<byte>('b'));
                    break;
                }
                case 0xc:
                {
                    m_text.push_back(static_cast<byte>('\\'));
                    m_text.push_back(static_cast<byte>('f'));
                    break;
                }
                case 0xa:
                {
                    m_text.push_back(static_cast<byte>('\\'));
                    m_text.push_back(static_cast<byte>('n'));
                    break;
                }
                case 0xd:
                {
                    m_text.push_back(static_cast<byte>('\\'));
                    m_text.push_back(static_cast<byte>('r'));
                    break;
                }
                case 0x9:
                {
                    m_text.push_back(static_cast<byte>('\\'));
                    m_text.push_back(static_cast<byte>('t'));
                    break;
                }
                default:
                {
                    if(ch < 0x20)
                    {
                        m_text.push_back(static_cast<byte>('\\'));
                        m_text.push_back(static_cast<byte>('u'));
                        m_text.push_back(static_cast<byte>('0'));
                        m_text.push_back(static_cast<byte>('0'));
                        m_text.push_back(hexDigit(ch >> 4));
                        m_text.push_back(hexDigit(ch & 0xf));
                    }
                    else
                    {
                        m_text.push_back(ch);
                    }
                    break;
                }
            }
        }

        m_text.push_back(static_cast<byte>('"'));
        m_length = m_text.size();

        Enter = [this]()->void
        {
            if(m_position < m_length)
            {
                Workflow()->PushState(new_ref<qor::components::generator::ASCIIChar>(GetGenerator(), m_text[m_position]));
            }
            else
            {
                m_result.code = qor::components::generator::Result::SUCCESS;
                Workflow()->PopState();
            }
        };

        Resume = [this]()->void
        {
            ++m_position;
        };
    }

    _string::~_string() noexcept = default;

}}}}//qor::data::generator::json
