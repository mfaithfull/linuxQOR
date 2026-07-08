// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_MODEL_CHARACTER
#define QOR_PP_H_COMPONENTS_JSON_MODEL_CHARACTER

#include <string>

namespace qor { namespace components{ namespace model { namespace json {

    class Character
    {
    public:

        Character() : m_value(0){ }
        Character(unsigned int c) : m_value(c){ }
        virtual ~Character() = default;
        unsigned int GetValue() {return m_value;}
        void SetValue(unsigned int c) { m_value = c; }
    private:
        unsigned int m_value;
    };

}}}}//qor::components::model::json

#endif//QOR_PP_H_COMPONENTS_JSON_MODEL_CHARACTER
