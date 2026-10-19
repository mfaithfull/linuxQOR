// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_MODEL_MAPPING
#define QOR_PP_H_PROTOCOLS_YAML_MODEL_MAPPING

#include "pair.h"
#include "valuemodel.h"

namespace qor { namespace protocols { namespace model { namespace yaml {

    class Mapping : public ValueModel
    {
    public:

        Mapping(){ }
        virtual ~Mapping() = default;

        virtual ValueType GetType() override
        {
            return ValueType::mapping;
        }

        std::vector<ref_of<Pair>::type> m_pairs;
    };

}}}}//qor::protocols::model::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_MODEL_MAPPING