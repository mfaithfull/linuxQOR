// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cstddef>
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"
#include "src/components/io/generator/result.h"
#include "_false.h"

namespace qor { namespace data { namespace generator { namespace json {

    _false::_false(qor::components::generator::Generator* generator) :
        qor::components::generator::GeneratorState(generator),
        m_text{ 0x66, 0x61, 0x6c, 0x73, 0x65 },
        m_position(0)
    {
        Enter = [this]()->void
        {
            if(m_position < sizeof(m_text))
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

    _false::~_false() noexcept = default;

}}}}//qor::data::generator::json
