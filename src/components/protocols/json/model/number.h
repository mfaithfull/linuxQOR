// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_MODEL_NUMBER
#define QOR_PP_H_COMPONENTS_JSON_MODEL_NUMBER

#include "valuemodel.h"

namespace qor { namespace components{ namespace model{ namespace json {

    class Number : public ValueModel
    {
    public:

        Number(){ }
        virtual ~Number() = default;
        double GetValue() {return m_value;}
        void SetValue(const double& value){ m_value = value;}
        
        virtual ValueType GetType() override
        {
            return ValueType::number;
        }

    private:

        double m_value{ 0.0 };
    };

}}}}//qor::components::model::json

#endif//QOR_PP_H_COMPONENTS_JSON_MODEL_NUMBER
