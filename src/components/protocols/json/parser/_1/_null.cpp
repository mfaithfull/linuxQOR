// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/debug.h"
#include "_null.h"
#include "../nodes/_null.h"

namespace qor { namespace components { namespace parser { namespace json {

    _null::_null(Parser* parser) :
        Sequence(parser,
            new_ref<Specific>(parser, (byte)0x6e),
            new_ref<Sequence>(parser,
                new_ref<Specific>(parser, (byte)0x75),
                new_ref<Sequence>(parser,
                    new_ref<Specific>(parser, (byte)0x6c),
                    new_ref<Specific>(parser, (byte)0x6c)
                )
            ),
        static_cast<uint64_t>(jsonToken::_null))
    { }

    _null::~_null() = default;

    void _null::Prepare()
    {
        //log::debug("Looking for a null...");
    }

    void _null::Emit()
    {        
        log::debug("Emitting a null.");
        GetParser()->PushNode(new_ref<_NullNode>());
    }

    void _null::Fail()
    {
        //log::debug("...Didn't find a null.");
    }

}}}}//qor::components::parser::json
