// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_MODEL_STRING
#define QOR_PP_H_COMPONENTS_JSON_MODEL_STRING

#include <string>
#include "valuemodel.h"

namespace qor { namespace components{ namespace model{ namespace json{

    class String : public ValueModel
    {
    public:

        String(){ }
        virtual ~String() = default;

        std::string GetValue() { return m_value; }
        void SetValue(const std::string& stringValue ) {m_value = stringValue;};

        virtual ValueType GetType() override
        {
            return ValueType::string;
        }

    private:
        std::string m_value;
    };

}}}}//qor::components::model::json

#endif//QOR_PP_H_COMPONENTS_JSON_MODEL_STRING
