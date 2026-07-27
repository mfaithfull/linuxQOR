// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "bom.h"
#include "utf8/utf8bom.h"
#include "utf16/utf16bebom.h"
#include "utf32/utf32bebom.h"
#include "lebom.h"
#include "../nodes/bomnode.h"

namespace qor { namespace data { namespace parser { namespace text {

    bom::bom(Parser* parser) : AnyOneOfSet(parser, new std::vector<ref_of<parser::ParserState>::type>({{
                        new_ref<utf8bom>(parser),
                        new_ref<utf16BEbom>(parser),
                        new_ref<utf32BEbom>(parser),
                        new_ref<LEbom>(parser),
                        }}),
            static_cast<uint64_t>(textToken::bom))
    { }

    bom::~bom() = default;
    
    void bom::Prepare()
    {
        log::debug("Looking for BOM...");
        GetParser()->PushNode(new_ref<BOMNode>());
    }

    void bom::Emit()
    {
        log::debug("Emitting a BOM");
        GetParser()->Diagnostic();
    }

    void bom::Fail()
    {
        log::debug("...Didn't find BOM.");
        ref_of<Node>::type node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }

}}}}//qor::data::parser::json