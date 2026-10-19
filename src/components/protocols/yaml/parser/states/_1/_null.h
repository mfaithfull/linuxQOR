// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_NULL
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_NULL

#include "../../tokens.h"

namespace qor { namespace protocols { namespace yaml {

    //null = %x6e.75.6c.6c  ; null
    class _null final : public Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > >
    {
    public:
        _null(Parser* parser);
        virtual ~_null() noexcept;
        virtual void Emit();

    private:
        Specific<uint32_t> m_n;
        Specific<uint32_t> m_u;
        Specific<uint32_t> m_l1;
        Specific<uint32_t> m_l2;
        Sequence_t< Specific<uint32_t>, Specific<uint32_t> > m_ll;
        Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > m_ull;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_NULL