// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_MODEL_VALUE
#define QOR_PP_H_PROTOCOLS_YAML_MODEL_VALUE

#include "valuemodel.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/essentials/datastructures/polymorphic.h"

namespace qor { namespace protocols { namespace model { namespace yaml {

    class Value
    {
    public:

        Value(){ }
        virtual ~Value() = default;

        polymorphic<ValueModel>& GetValue()
        {
            return m_valueModel;
        }

        void SetValue(const polymorphic<ValueModel>& value)
        {
            m_valueModel = value;
        }

        ValueType GetType() { return m_valueModel->GetType(); };

    private:

        polymorphic<ValueModel> m_valueModel;
    };

}}}}//qor::protocols::model::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_MODEL_VALUE