// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_MODEL_FALSE
#define QOR_PP_H_PROTOCOLS_YAML_MODEL_FALSE

#include "valuemodel.h"

namespace qor { namespace protocols { namespace model { namespace yaml {

    class _False : public ValueModel
    {
    public:

        _False(){};
        virtual ~_False() = default;

        bool GetValue() { return false; }

        virtual ValueType GetType() override
        {
            return ValueType::_false;
        }
    };

}}}}//qor::protocols::model::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_MODEL_FALSE