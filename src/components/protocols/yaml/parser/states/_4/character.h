// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PROTOCOLS_YAML_PARSER_CHARACTER
#define QOR_PP_H_PROTOCOLS_YAML_PARSER_CHARACTER

#include "../../tokens.h"
#include "unescaped.h"
#include "../../nodes/character.h"
#include "src/qor/memory/reference/newref.h"

namespace qor { namespace protocols { namespace yaml {

    //character = unescaped (escaped deferred for now)
    class character final : public AnyOneOf_t< unescaped, unescaped >
    {
    public:
        character(Parser* parser) :
            AnyOneOf_t< unescaped, unescaped >(parser, &m_un1, &m_un2, static_cast<uint64_t>(yamlToken::character)),
            m_un1(parser),
            m_un2(parser)
        { }
        virtual ~character() noexcept = default;

        virtual void Emit()
        {
            GetParser()->PushNode(new_ref<CharacterNode>(m_result.first));
        }

    private:
        unescaped m_un1;
        unescaped m_un2;
    };

}}}//qor::protocols::yaml

#endif//QOR_PP_H_PROTOCOLS_YAML_PARSER_CHARACTER