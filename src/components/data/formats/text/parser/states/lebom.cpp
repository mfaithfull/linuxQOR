// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "lebom.h"
#include "utf32/utf32leindicator.h"
#include "../nodes/bomnode.h"

namespace qor { namespace data { namespace parser { namespace text {

    LEbom::LEbom(Parser* parser) : Sequence(parser, 
        new_ref<Sequence>(parser,
            new_ref<Specific>(parser, 0xFF),
            new_ref<Specific>(parser, 0xFE)
        ), 
        new_ref<Optional>(parser, new_ref<utf32LEindicator>(parser)),
        static_cast<uint64_t>(textToken::bom))
    { }

    LEbom::~LEbom() = default;
    
    void LEbom::Prepare()
    {
        log::debug("Looking for Little Endian BOM...");
    }

    void LEbom::Emit()
    {
        log::debug("Emitting a Little Endian BOM");
        auto node = GetParser()->TopNode();
        if(node.IsNotNull() && node->GetToken() == static_cast<uint64_t>(textToken::bom))
        {
            auto bomNode = node.AsRef<BOMNode>();
            if(bomNode.IsNotNull())
            {
                if(bomNode->GetIndicator() == BOMNode::Size::ThrirtyTwo)
                {
                    log::debug("BOM is UTF-32LE");
                    bomNode->SetBOMType(textToken::utf32LEBOM);
                }
                else
                {
                    log::debug("BOM is UTF-16LE");
                    bomNode->SetBOMType(textToken::utf16LEBOM);
                }
            }
        }
    }

    void LEbom::Fail()
    {
        log::debug("...Didn't find Little Endian BOM.");
    }

}}}}//qor::data::parser::json