// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_MODEL_TRUE
#define QOR_PP_H_PROTOCOLS_YAML_MODEL_TRUE

#include "valuemodel.h"

namespace qor { namespace protocols { namespace model { namespace yaml {

    class _True : public ValueModel
    {
    public:

        _True(){};
        virtual ~_True() = default;

        virtual ValueType GetType() override
        {
            return ValueType::_true;
        }
    };

}}}}//qor::protocols::model::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_MODEL_TRUE