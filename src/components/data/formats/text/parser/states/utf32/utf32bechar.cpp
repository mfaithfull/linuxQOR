// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "utf32bechar.h"
#include "../../nodes/textnode.h"

namespace qor { namespace data { namespace parser { namespace text {

    utf32BEchar::utf32BEchar(Parser* parser) : Sequence(parser, 
        new_ref<Sequence>(parser, new_ref<Specific>(parser, 0x00),
            new_ref<Specific>(parser, 0xFE)
        ), 
        new_ref<Optional>(parser, new_ref<Specific>(parser, 0x01)),
        static_cast<uint64_t>(textToken::bom))
    { }

    utf32BEchar::~utf32BEchar() = default;
    
    void utf32BEchar::Prepare()
    {
        log::debug("Looking for a UTF-32BE char");        
    }

    void utf32BEchar::Emit()
    {
        log::debug("Emitting a UTF-32BE char");
    }

    void utf32BEchar::Fail()
    {
        log::debug("...Didn't find UTF-32BE char.");
    }

}}}}//qor::data::parser::json