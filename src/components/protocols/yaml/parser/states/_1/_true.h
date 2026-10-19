// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_TRUE
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_TRUE

#include "../../tokens.h"

namespace qor { namespace protocols { namespace yaml {

    //true = %x74.72.75.65  ; true
    class _true final : public Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > >
    {
    public:
        _true(Parser* parser);
        virtual ~_true() noexcept;
        virtual void Emit();

    private:
        Specific<uint32_t> m_t;
        Specific<uint32_t> m_r;
        Specific<uint32_t> m_u;
        Specific<uint32_t> m_e;
        Sequence_t< Specific<uint32_t>, Specific<uint32_t> > m_ue;
        Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > m_rue;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_TRUE