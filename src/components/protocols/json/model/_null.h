// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_MODEL_NULL
#define QOR_PP_H_COMPONENTS_JSON_MODEL_NULL

#include "valuemodel.h"

namespace qor { namespace components{ namespace model{ namespace json {

    class _Null : public ValueModel
    {
    public:

        _Null(){}
        virtual ~_Null() = default;

        virtual ValueType GetType() override
        {
            return ValueType::_null;
        }

    };

}}}}//qor::components::model::json

#endif//QOR_PP_H_COMPONENTS_JSON_MODEL_NULL
