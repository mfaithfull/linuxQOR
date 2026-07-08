// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/debug.h"
#include "character.h"
#include "../nodes/character.h"

namespace qor { namespace components { namespace parser { namespace json {

    character::character(Parser* parser) : AnyOneOf(parser,  new_ref<unescaped>(parser), 
            new_ref<escapeSequence>(parser),
            static_cast<uint64_t>(jsonToken::character)
        )
    { }

    character::~character() = default;

    void character::Emit()
    {
        char charVal = (char)m_result.first;
        log::debug("output char: {0}", charVal);
        GetParser()->PushNode(new_ref<CharacterNode>(charVal));     
    }

}}}}//qor::components::parser::json
