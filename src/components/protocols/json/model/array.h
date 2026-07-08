// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_JSON_MODEL_ARRAY
#define QOR_PP_H_COMPONENTS_JSON_MODEL_ARRAY

#include <vector>
#include "value.h"

namespace qor { namespace components{ namespace model{ namespace json {

    class Array
    {
    public:

        Array(){}
        virtual ~Array() = default;        

        std::vector<ref_of<Value>::type> m_values;

    };

}}}}//qor::components::model::json

#endif//QOR_PP_H_COMPONENTS_JSON_MODEL_ARRAY
