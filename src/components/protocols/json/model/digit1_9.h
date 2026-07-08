// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_MODEL_DIGIT1_9
#define QOR_PP_H_COMPONENTS_JSON_MODEL_DIGIT1_9

#include <string>

namespace qor { namespace components{ namespace model { namespace json {

    class Digit1_9
    {
    public:

        Digit1_9() : m_value(0){ }
        Digit1_9(char c) : m_value(c){ }
        virtual ~Digit1_9() = default;
        char GetValue() {return m_value;}
        void SetValue(char c) { m_value = c; }
    private:
        char m_value;
    };

}}}}//qor::components::model::json

#endif//QOR_PP_H_COMPONENTS_JSON_MODEL_DIGIT1_9
