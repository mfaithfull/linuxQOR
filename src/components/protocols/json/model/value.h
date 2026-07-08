// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_MODEL_VALUE
#define QOR_PP_H_COMPONENTS_JSON_MODEL_VALUE

#include "valuemodel.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"

namespace qor { namespace components{ namespace model{ namespace json {

    class Value
    {
    public:        

        Value(){ }
        virtual ~Value() = default;

        ref_of<ValueModel>::type GetValue()
        {
            return m_value;
        }

        void SetValue(ref_of<ValueModel>::type value)
        {
            m_value = value;
        }
        
        ValueType GetType() { return m_value->GetType(); };
        
    private:

        ref_of<ValueModel>::type m_value;
        
    };

}}}}//qor::components::model::json

#endif//QOR_PP_H_COMPONENTS_JSON_MODEL_VALUE