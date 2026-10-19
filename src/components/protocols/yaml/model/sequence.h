// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_MODEL_SEQUENCE
#define QOR_PP_H_PROTOCOLS_YAML_MODEL_SEQUENCE

#include <vector>
#include "value.h"

namespace qor { namespace protocols { namespace model { namespace yaml {

    class Sequence : public ValueModel
    {
    public:

        Sequence(){}
        virtual ~Sequence() = default;

        virtual ValueType GetType() override
        {
            return ValueType::sequence;
        }

        std::vector<ref_of<Value>::type> m_values;

    };

}}}}//qor::protocols::model::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_MODEL_SEQUENCE