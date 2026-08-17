// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/debug.h"
#include "character.h"
#include "../../nodes/character.h"
#include "../../nodes/string.h"

namespace qor { namespace data { namespace parser { namespace json {

    UTF8CodePage character::s_codePage;

    character::character(Parser* parser) : AnyOneOf_t< unescaped, deferred<escapeSequence> >(parser, &m_un, &m_escaped,
            static_cast<uint64_t>(jsonToken::character)),
            m_un(parser),
            m_escaped(parser, static_cast<uint64_t>(jsonToken::escape_sequence))
    { }

    character::~character() = default;

    void character::Emit()
    {
        auto charVal = m_result.first;        
        log::debug("output char: {0}", charVal);
        auto node = GetParser()->TopNode();
        if(node.IsNotNull() && node->GetToken() == static_cast<uint64_t>(jsonToken::string))
        {
            auto stringNode = node.AsRef<StringNode>();
            char32_t c = static_cast<char32_t>(charVal);
            char8_t space[6];
            char8_t* p = &space[0];
            size_t available = 1;
            if(s_codePage.Encode(c, p, available))
            {
                stringNode->GetObject()->Append((const char*)space, (p - &space[0]));
            }
        }
        else
        {
            GetParser()->PushNode(new_ref<CharacterNode>(charVal));     
        }        
    }

}}}}//qor::data::parser::json
