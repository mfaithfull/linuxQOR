// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cstddef>
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"
#include "src/components/io/generator/result.h"
#include "_true.h"

namespace qor { namespace data { namespace generator { namespace json {

    _true::_true(qor::components::generator::Generator* generator) :
        qor::components::generator::GeneratorState(generator),
        m_text{ 0x74, 0x72, 0x75, 0x65 },
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

    _true::~_true() noexcept = default;

}}}}//qor::data::generator::json
