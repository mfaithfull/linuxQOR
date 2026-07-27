// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "utf32leindicator.h"
#include "../../nodes/bomnode.h"

namespace qor { namespace data { namespace parser { namespace text {

    utf32LEindicator::utf32LEindicator(Parser* parser) :
        Sequence(parser, new_ref<Specific>(parser, 0x00), new_ref<Specific>(parser, 0x00), 
            static_cast<uint64_t>(textToken::utf32LEBOM))
    { }

    utf32LEindicator::~utf32LEindicator() = default;
    
    void utf32LEindicator::Prepare()
    {
        log::debug("Looking for UTF-32 indicator.");
    }

    void utf32LEindicator::Emit()
    {
        log::debug("Emitting a UTF-32 indicator.");        
        ref_of<Node>::type node = GetParser()->PopNode();
        if(node.IsNotNull())
        {
            auto bomNode = node.AsRef<BOMNode>();
            if(bomNode.IsNotNull())
            {
                bomNode->SetIndicator(BOMNode::Size::ThrirtyTwo);
            }
            GetParser()->PushNode(node);
        }
    }

    void utf32LEindicator::Fail()
    {
        log::debug("...Didn't find UTF-32 indicator.");
    }

}}}}//qor::data::parser::json