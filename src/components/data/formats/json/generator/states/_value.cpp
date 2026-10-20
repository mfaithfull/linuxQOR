// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cstddef>
#include <cmath>
#include "src/components/io/generator/generator.h"
#include "src/components/io/generator/state.h"
#include "src/components/io/generator/result.h"
#include "src/components/data/formats/json/model/value.h"
#include "src/components/data/formats/json/model/string.h"
#include "src/components/data/formats/json/model/array.h"
#include "src/components/data/formats/json/model/object.h"
#include "src/components/data/formats/json/model/number.h"
#include "_value.h"
#include "_null.h"
#include "_true.h"
#include "_false.h"
#include "_string.h"
#include "_array.h"
#include "_number.h"
#include "_object.h"

namespace qor { namespace data { namespace generator { namespace json {

    _value::_value(qor::components::generator::Generator* generator, qor::data::model::json::Value* value) :
        qor::components::generator::GeneratorState(generator),
        m_value(value),
        m_position(0)
    {
        Enter = [this]()->void
        {
            if(m_position == 0)
            {
                switch(m_value->GetType())
                {
                    case qor::data::model::json::ValueType::_null:
                    {
                        Workflow()->PushState(new_ref<qor::data::generator::json::_null>(GetGenerator()));
                        break;
                    }
                    case qor::data::model::json::ValueType::_true:
                    {
                        Workflow()->PushState(new_ref<qor::data::generator::json::_true>(GetGenerator()));
                        break;
                    }
                    case qor::data::model::json::ValueType::_false:
                    {
                        Workflow()->PushState(new_ref<qor::data::generator::json::_false>(GetGenerator()));
                        break;
                    }
                    case qor::data::model::json::ValueType::string:
                    {
                        //Reach the concrete String model through the polymorphic
                        //value handle and hand its decoded text to the string
                        //state, which adds the quotes and the escaping.
                        auto stringValue = dynamic_cast<qor::data::model::json::String*>(m_value->GetValue().operator->());
                        if(stringValue != nullptr)
                        {
                            std::string text(stringValue->GetValue());
                            Workflow()->PushState(new_ref<qor::data::generator::json::_string>(GetGenerator(), text.c_str(), text.size()));
                        }
                        else
                        {
                            m_result.code = qor::components::generator::Result::FAILURE;
                            Workflow()->PopState();
                        }
                        break;
                    }
                    case qor::data::model::json::ValueType::array:
                    {
                        //Reach the concrete Array model and let the array state
                        //iterate its element values.
                        auto arrayModel = dynamic_cast<qor::data::model::json::Array*>(m_value->GetValue().operator->());
                        if(arrayModel != nullptr)
                        {
                            Workflow()->PushState(new_ref<qor::data::generator::json::_array>(GetGenerator(), arrayModel));
                        }
                        else
                        {
                            m_result.code = qor::components::generator::Result::FAILURE;
                            Workflow()->PopState();
                        }
                        break;
                    }
                    case qor::data::model::json::ValueType::object:
                    {
                        //Reach the concrete Object model and let the object state
                        //iterate its members.
                        auto objectModel = dynamic_cast<qor::data::model::json::Object*>(m_value->GetValue().operator->());
                        if(objectModel != nullptr)
                        {
                            Workflow()->PushState(new_ref<qor::data::generator::json::_object>(GetGenerator(), objectModel));
                        }
                        else
                        {
                            m_result.code = qor::components::generator::Result::FAILURE;
                            Workflow()->PopState();
                        }
                        break;
                    }
                    case qor::data::model::json::ValueType::number:
                    {
                        //Reach the concrete Number model and let the number state
                        //render its double value as a JSON number. Non finite
                        //values cannot be represented by the JSON number grammar,
                        //so the value state rejects them before entering number.
                        auto numberModel = dynamic_cast<qor::data::model::json::Number*>(m_value->GetValue().operator->());
                        if(numberModel != nullptr && std::isfinite(numberModel->GetValue()))
                        {
                            Workflow()->PushState(new_ref<qor::data::generator::json::_number>(GetGenerator(), numberModel));
                        }
                        else
                        {
                            m_result.code = qor::components::generator::Result::FAILURE;
                            Workflow()->PopState();
                        }
                        break;
                    }
                    default:
                    {
                        // An unrecognised value type cannot be generated.
                        m_result.code = qor::components::generator::Result::FAILURE;
                        Workflow()->PopState();
                        break;
                    }
                }
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

    _value::~_value() noexcept = default;

}}}}//qor::data::generator::json
