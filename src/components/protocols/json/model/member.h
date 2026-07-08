// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_MODEL_MEMBER
#define QOR_PP_H_COMPONENTS_JSON_MODEL_MEMBER

#include "value.h"

namespace qor { namespace components{ namespace model{ namespace json {

    class Member
    {
    public:        

        Member(){ }
        virtual ~Member() = default;

        Value& GetValue() {return m_value;}
        const Value& GetValue() const {return m_value;}
        std::string GetName() {return m_name;};
        void SetName(const std::string& name) { m_name = name;}

    private:

        std::string m_name;
        class Value m_value;
    };

}}}}//qor::components::model::json

#endif//QOR_PP_H_COMPONENTS_JSON_MODEL_MEMBER
