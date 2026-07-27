// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "utf16bebom.h"
#include "../../nodes/bomnode.h"

namespace qor { namespace data { namespace parser { namespace text {

    utf16BEbom::utf16BEbom(Parser* parser) :
        Sequence(parser, new_ref<Specific>(parser, 0xFE), new_ref<Specific>(parser, 0xFF), 
            static_cast<uint64_t>(textToken::utf16BEBOM))
    { }

    utf16BEbom::~utf16BEbom() = default;
    
    void utf16BEbom::Prepare()
    {
        log::debug("Looking for UTF-16BE BOM");
    }

    void utf16BEbom::Emit()
    {
        log::debug("Emitting a UTF-16BE BOM");
        ref_of<Node>::type node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() == static_cast<uint64_t>(textToken::bom))
        {
            node.AsRef<BOMNode>()->SetBOMType(textToken::utf16BEBOM);
        }
        if(node.IsNotNull())
        {
            GetParser()->PushNode(node);
        }
    }

    void utf16BEbom::Fail()
    {
        log::debug("...Didn't find UTF-16BE BOM.");
    }

}}}}//qor::data::parser::json