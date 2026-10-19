// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_MODEL_STRING
#define QOR_PP_H_PROTOCOLS_YAML_MODEL_STRING

#include <string>
#include "valuemodel.h"

namespace qor { namespace protocols { namespace model { namespace yaml {

    class String : public ValueModel
    {
    public:

        String(){ m_value.reserve(32); }
        virtual ~String() = default;

        std::string GetValue() { return m_value; }
        void SetValue(const std::string& stringValue ) { m_value = stringValue; };
        void Append(const char* ptr, size_t count) { m_value.append(ptr, count); }

        virtual ValueType GetType() override
        {
            return ValueType::string;
        }

    private:
        std::string m_value;
    };

}}}}//qor::protocols::model::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_MODEL_STRING