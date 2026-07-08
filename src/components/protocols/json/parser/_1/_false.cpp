// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/debug.h"
#include "_false.h"
#include "../nodes/_false.h"

namespace qor { namespace components { namespace parser { namespace json {

    _false::_false(Parser* parser) : Sequence(parser,
            new_ref<Specific>(parser, (byte)0x66),
            new_ref<Sequence>(parser,
                new_ref<Specific>(parser, (byte)0x61),
                new_ref<Sequence>(parser,
                    new_ref<Specific>(parser, (byte)0x6c),
                    new_ref<Sequence>(parser,
                        new_ref<Specific>(parser, (byte)0x73),
                        new_ref<Specific>(parser, (byte)0x65)
                    )
                )
            ),
        static_cast<uint64_t>(jsonToken::_false))
    { }

    _false::~_false() = default;

    void _false::Prepare()
    {
        //log::debug("Looking for a false...");
    }

    void _false::Emit()
    {        
        log::debug("Emitting a false.");
        GetParser()->PushNode(new_ref<_FalseNode>());
    }

    void _false::Fail()
    {
        //log::debug("...Didn't find a false.");
    }

}}}}//qor::components::parser::json
