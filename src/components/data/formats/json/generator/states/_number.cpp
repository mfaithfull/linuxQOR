// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cstddef>
#include <cmath>
#include <charconv>
#include <system_error>
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"
#include "src/components/io/generator/result.h"
#include "_number.h"

namespace qor { namespace data { namespace generator { namespace json {

    _number::_number(qor::components::generator::Generator* generator, long long value) :
        qor::components::generator::GeneratorState(generator),
        m_text{},
        m_length(0),
        m_position(0),
        m_valid(true)
    {
        // Format the value into m_text before the workflow runs, so that Enter
        // only has to walk over the characters it has already produced.
        unsigned long long magnitude(0);

        if(value < 0)
        {
            m_text[m_length++] = static_cast<byte>('-');
            magnitude = static_cast<unsigned long long>(-(value + 1)) + 1ULL;
        }
        else
        {
            magnitude = static_cast<unsigned long long>(value);
        }

        // The digits arrive least significant first, so they are collected
        // temporarily and then written out in reverse order.
        byte digits[20] = {};
        size_t digitCount(0);

        do
        {
            digits[digitCount++] = static_cast<byte>(static_cast<char>('0' + (magnitude % 10ULL)));
            magnitude /= 10ULL;
        } while(magnitude != 0);

        while(digitCount > 0)
        {
            m_text[m_length++] = digits[--digitCount];
        }

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

    //Read a double from the number model and render it as a JSON number. The
    //standard shortest round trip conversion may choose an exponent, so the
    //number grammar includes the optional exponent part. Non finite values are
    //rejected because the JSON grammar has no token for them.
    _number::_number(qor::components::generator::Generator* generator, qor::data::model::json::Number* number) :
        qor::components::generator::GeneratorState(generator),
        m_text{},
        m_length(0),
        m_position(0),
        m_valid(true)
    {
        double value = number->GetValue();

        if(!std::isfinite(value))
        {
            m_valid = false;
        }
        else
        {
            char text[64] = {};

            auto [ptr, errorCode] = std::to_chars(text, text + sizeof(text), value);

            if(errorCode == std::errc())
            {
                m_length = static_cast<size_t>(ptr - text);

                for(size_t index = 0; index < m_length; ++index)
                {
                    m_text[index] = static_cast<byte>(text[index]);
                }
            }
            else
            {
                m_valid = false;
            }
        }

        Enter = [this]()->void
        {
            if(!m_valid)
            {
                m_result.code = qor::components::generator::Result::FAILURE;
                Workflow()->PopState();
            }
            else if(m_position < m_length)
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

    _number::~_number() noexcept = default;

}}}}//qor::data::generator::json
