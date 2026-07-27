// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "utf32bebom.h"
#include "../../nodes/bomnode.h"

namespace qor { namespace data { namespace parser { namespace text {

    utf32BEbom::utf32BEbom(Parser* parser) :
        Sequence(parser, 
            new_ref<Sequence>(parser, 
                new_ref<Specific>(parser, 0x00),
                new_ref<Specific>(parser, 0x00)
            ), 
            new_ref<Sequence>(parser, 
                new_ref<Specific>(parser, 0xFE),
                new_ref<Specific>(parser, 0xFF)
            ), 
            static_cast<uint64_t>(textToken::utf32BEBOM))
    { }

    utf32BEbom::~utf32BEbom() = default;
    
    void utf32BEbom::Prepare()
    {
        log::debug("Looking for UTF-32BE BOM");
    }

    void utf32BEbom::Emit()
    {
        log::debug("Emitting a UTF-32BE BOM");
        ref_of<Node>::type node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() == static_cast<uint64_t>(textToken::bom))
        {
            node.AsRef<BOMNode>()->SetBOMType(textToken::utf32BEBOM);
        }
        if(node.IsNotNull())
        {
            GetParser()->PushNode(node);
        }
    }

    void utf32BEbom::Fail()
    {
        log::debug("...Didn't find UTF-32BE BOM.");
    }

}}}}//qor::data::parser::json