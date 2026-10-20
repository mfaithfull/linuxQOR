// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cstddef>
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"
#include "src/components/io/generator/result.h"
#include "src/components/data/formats/json/model/array.h"
#include "src/components/data/formats/json/model/value.h"
#include "_array.h"
#include "_value.h"
#include "_comma.h"

namespace qor { namespace data { namespace generator { namespace json {

    //An empty array is just the two brackets.
    _array::_array(qor::components::generator::Generator* generator) :
        qor::components::generator::GeneratorState(generator),
        m_array(nullptr),
        m_count(0),
        m_position(0)
    {
        Enter = [this]()->void
        {
            if(m_position == 0)
            {
                Workflow()->PushState(new_ref<qor::components::generator::ASCIIChar>(GetGenerator(), static_cast<byte>('[')));
            }
            else if(m_position == 1)
            {
                Workflow()->PushState(new_ref<qor::components::generator::ASCIIChar>(GetGenerator(), static_cast<byte>(']')));
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

    //A non-empty array writes the opening bracket, then each element value
    //separated by a comma, then the closing bracket. The push sequence is:
    //   position 0            : opening bracket
    //   position 2k + 1       : element value k
    //   position 2k           : comma before element value k (k >= 1)
    //   position 2 * count    : closing bracket
    _array::_array(qor::components::generator::Generator* generator, qor::data::model::json::Array* array) :
        qor::components::generator::GeneratorState(generator),
        m_array(array),
        m_count(array->m_values.size()),
        m_position(0)
    {
        Enter = [this]()->void
        {
            if(m_position == 0)
            {
                Workflow()->PushState(new_ref<qor::components::generator::ASCIIChar>(GetGenerator(), static_cast<byte>('[')));
            }
            else if(m_position < (m_count * 2))
            {
                if((m_position & 1) != 0)
                {
                    //Write the element value through the shared value state. The
                    //value is owned by the reference stored in the model, which
                    //stays alive for as long as the array does.
                    qor::data::model::json::Value* element = m_array->m_values[m_position / 2];
                    Workflow()->PushState(new_ref<qor::data::generator::json::_value>(GetGenerator(), element));
                }
                else
                {
                    Workflow()->PushState(new_ref<qor::data::generator::json::_comma>(GetGenerator()));
                }
            }
            else if(m_position == (m_count * 2))
            {
                Workflow()->PushState(new_ref<qor::components::generator::ASCIIChar>(GetGenerator(), static_cast<byte>(']')));
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

    _array::~_array() noexcept = default;

}}}}//qor::data::generator::json
