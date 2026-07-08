// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/log/debug.h"
#include "src/components/data/parser/nodes/digit.h"
#include "digit1_9.h"
#include "../nodes/digit1_9.h"

namespace qor { namespace components { namespace parser { namespace json {

    digit1_9::digit1_9(Parser* parser) : OneOfARange(parser, (byte)0x31, (byte)0x39, static_cast<uint64_t>(jsonToken::digit1_9)){ }

    digit1_9::~digit1_9() = default;

    void digit1_9::Emit()
    {        
        int digitVal = m_result.first - '0';
        log::debug("Emitting a digit: {0}", digitVal);
        GetParser()->PushNode(new_ref<Digit1_9Node>((char)digitVal));
    }

}}}}//qor::components::parser::json