// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_MODEL_PAIR
#define QOR_PP_H_PROTOCOLS_YAML_MODEL_PAIR

#include "value.h"

namespace qor { namespace protocols { namespace model { namespace yaml {

    class Pair
    {
    public:

        Pair(){ }
        virtual ~Pair() = default;

        Value& GetValue() {return m_value;}
        const Value& GetValue() const {return m_value;}
        std::string GetKey() {return m_key;};
        void SetKey(const std::string& key) { m_key = key;}

    private:

        std::string m_key;
        Value m_value;
    };

}}}}//qor::protocols::model::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_MODEL_PAIR