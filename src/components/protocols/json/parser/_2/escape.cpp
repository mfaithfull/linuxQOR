// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "escape.h"
#include "../nodes/escape.h"
#include "../nodes/character.h"

namespace qor { namespace components { namespace parser { namespace json {

    escape::escape(Parser* parser) : Specific(parser, 0x5C, static_cast<uint64_t>(jsonToken::escape)){ }
    escape::~escape() = default;

    void escape::Prepare()
    {
        //log::debug("Looking for an Escape Character...");
    }

    void escape::Emit()
    {
        //log::debug("Emitting an Escape character: ");
    }

    void escape::Fail()
    {
        //log::debug("...Didn't find an Escape Character.");
    }

}}}}//qor::components::parser::json
