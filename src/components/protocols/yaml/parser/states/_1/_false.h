// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_FALSE
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_FALSE

#include "../../tokens.h"

namespace qor { namespace protocols { namespace yaml {

    //false = %x66.61.6c.73.65  ; false
    class _false final : public Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > > >
    {
    public:
        _false(Parser* parser);
        virtual ~_false() noexcept;
        virtual void Emit();

    private:
        Specific<uint32_t> m_f;
        Specific<uint32_t> m_a;
        Specific<uint32_t> m_l;
        Specific<uint32_t> m_s;
        Specific<uint32_t> m_e;
        Sequence_t< Specific<uint32_t>, Specific<uint32_t> > m_se;
        Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > m_lse;
        Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Sequence_t< Specific<uint32_t>, Specific<uint32_t> > > > m_alse;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_FALSE