// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "escape.h"
#include "../../nodes/escape.h"
#include "../../nodes/character.h"

namespace qor { namespace data { namespace parser { namespace json {

    escape::escape(Parser* parser) : Specific<uint32_t>(parser, 0x5C, static_cast<uint64_t>(jsonToken::escape)){ }
    escape::~escape() noexcept = default;

    void escape::Emit(){ }

}}}}//qor::data::parser::json
