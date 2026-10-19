// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_UNESCAPED
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_UNESCAPED

#include "../../tokens.h"

namespace qor { namespace protocols { namespace yaml {

    //unescaped = (0x20-0x21 / 0x23-0x5B / 0x5D-0x10FFFF)  (excludes " and \)
    class unescaped final : public AnyOneOf_t< OneOfARange_t<uint32_t>, AnyOneOf_t< OneOfARange_t<uint32_t>, OneOfARange_t<uint32_t> > >
    {
    public:
        unescaped(Parser* parser) :
            AnyOneOf_t< OneOfARange_t<uint32_t>, AnyOneOf_t< OneOfARange_t<uint32_t>, OneOfARange_t<uint32_t> > >(parser, &m_a, &m_2, static_cast<uint64_t>(yamlToken::unescaped)),
            m_a(parser, 0x20, 0x21),
            m_b(parser, 0x23, 0x5B),
            m_c(parser, 0x5D, 0x10FFFF),
            m_2(parser, &m_b, &m_c)
        { }
        virtual ~unescaped() noexcept = default;
        void Emit(){ }

    private:
        OneOfARange_t<uint32_t> m_a;
        OneOfARange_t<uint32_t> m_b;
        OneOfARange_t<uint32_t> m_c;
        AnyOneOf_t< OneOfARange_t<uint32_t>, OneOfARange_t<uint32_t> > m_2;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_UNESCAPED