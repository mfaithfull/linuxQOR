// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "utf8bom.h"
#include "../../nodes/bomnode.h"

namespace qor { namespace data { namespace parser { namespace text {

    utf8bom::utf8bom(Parser* parser) :
        Sequence(parser, new_ref<Specific>(parser, 0xEF),
            new_ref<Sequence>(parser, new_ref<Specific>(parser, 0xBB), new_ref<Specific>(parser, 0xBF)), 
            static_cast<uint64_t>(textToken::utf8BOM))
    { }

    utf8bom::~utf8bom() = default;
    
    void utf8bom::Prepare()
    {
        log::debug("Looking for UTF-8 BOM");        
    }

    void utf8bom::Emit()
    {
        log::debug("Emitting a UTF-8 BOM");
        ref_of<Node>::type node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() == static_cast<uint64_t>(textToken::bom))
        {
            node.AsRef<BOMNode>()->SetBOMType(textToken::utf8BOM);
        }
        if(node.IsNotNull())
        {
            GetParser()->PushNode(node);
        }
    }

    void utf8bom::Fail()
    {
        log::debug("...Didn't find UTF-8 BOM.");
    }

}}}}//qor::data::parser::json