// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cstddef>
#include <string>
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"
#include "src/components/io/generator/result.h"
#include "src/components/data/formats/json/model/member.h"
#include "_member.h"
#include "_string.h"
#include "_colon.h"
#include "_value.h"

namespace qor { namespace data { namespace generator { namespace json {

    _member::_member(qor::components::generator::Generator* generator, qor::data::model::json::Member* member) :
        qor::components::generator::GeneratorState(generator),
        m_member(member),
        m_name(member->GetName()),
        m_position(0)
    {
        Enter = [this]()->void
        {
            switch(m_position)
            {
                case 0:
                {
                    //Write the member name as a quoted, escaped string.
                    Workflow()->PushState(new_ref<qor::data::generator::json::_string>(GetGenerator(), m_name.c_str(), m_name.size()));
                    break;
                }
                case 1:
                {
                    //Write the colon that separates the name from the value.
                    Workflow()->PushState(new_ref<qor::data::generator::json::_colon>(GetGenerator()));
                    break;
                }
                case 2:
                {
                    //Write the member value through the shared value state.
                    Workflow()->PushState(new_ref<qor::data::generator::json::_value>(GetGenerator(), &m_member->GetValue()));
                    break;
                }
                default:
                {
                    m_result.code = qor::components::generator::Result::SUCCESS;
                    Workflow()->PopState();
                    break;
                }
            }
        };

        Resume = [this]()->void
        {
            ++m_position;
        };
    }

    _member::~_member() noexcept = default;

}}}}//qor::data::generator::json
