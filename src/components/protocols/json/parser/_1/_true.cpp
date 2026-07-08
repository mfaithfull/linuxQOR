// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/debug.h"
#include "_true.h"
#include "../nodes/_true.h"

namespace qor { namespace components { namespace parser { namespace json {

    _true::_true(Parser* parser) :
        Sequence(parser,
            new_ref<Specific>(parser, (byte)0x74),
            new_ref<Sequence>(parser,
                new_ref<Specific>(parser, (byte)0x72),
                new_ref<Sequence>(parser,
                    new_ref<Specific>(parser, (byte)0x75),
                    new_ref<Specific>(parser, (byte)0x65)
                )
            ),
        static_cast<uint64_t>(jsonToken::_true))
    { }

    _true::~_true() = default;

    void _true::Prepare()
    {
        //log::debug("Looking for a true...");
    }

    void _true::Emit()
    {
        log::debug("Emitting a true.");
        GetParser()->PushNode(new_ref<_TrueNode>());
    }

    void _true::Fail()
    {
        //log::debug("...Didn't find a true.");
    }

}}}}//qor::components::parser::json
