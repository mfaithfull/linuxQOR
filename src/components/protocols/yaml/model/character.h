// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_MODEL_CHARACTER
#define QOR_PP_H_PROTOCOLS_YAML_MODEL_CHARACTER

#include <cstdint>

namespace qor { namespace protocols { namespace model { namespace yaml {

    class Character
    {
    public:

        Character(uint32_t value = 0) : m_value(value) { }
        virtual ~Character() = default;

        void SetValue(uint32_t value) { m_value = value; }
        uint32_t GetValue() const { return m_value; }

    private:

        uint32_t m_value;
    };

}}}}//qor::protocols::model::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_MODEL_CHARACTER