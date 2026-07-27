// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "utf32lechar.h"
#include "../../nodes/textnode.h"

namespace qor { namespace data { namespace parser { namespace text {

    utf32LEchar::utf32LEchar(Parser* parser) : Sequence(parser, 
        new_ref<Sequence>(parser, new_ref<Specific>(parser, 0x00),
            new_ref<Specific>(parser, 0xFE)
        ), 
        new_ref<Optional>(parser, new_ref<Specific>(parser, 0x01)),
        static_cast<uint64_t>(textToken::bom))
    { }

    utf32LEchar::~utf32LEchar() = default;
    
    void utf32LEchar::Prepare()
    {
        log::debug("Looking for a UTF-32LE char");        
    }

    void utf32LEchar::Emit()
    {
        log::debug("Emitting a UTF-32LE char");
    }

    void utf32LEchar::Fail()
    {
        log::debug("...Didn't find UTF-32LE char.");
    }

}}}}//qor::data::parser::json