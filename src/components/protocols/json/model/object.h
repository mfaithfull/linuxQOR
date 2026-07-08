// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_MODEL_OBJECT
#define QOR_PP_H_COMPONENTS_JSON_MODEL_OBJECT

#include "member.h"
#include "valuemodel.h"

namespace qor { namespace components{ namespace model{ namespace json {

    class Object : public ValueModel
    {
    public:

        Object(){ }
        virtual ~Object() = default;

        virtual ValueType GetType() override
        {
            return ValueType::object;
        }
        
        std::vector<ref_of<Member>::type> m_members;
    };

}}}}//qor::components::model::json

#endif//QOR_PP_H_COMPONENTS_JSON_MODEL_OBJECT
