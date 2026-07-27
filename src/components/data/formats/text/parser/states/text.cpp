// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "text.h"
#include "bom.h"
#include "deferredchar.h"
#include "../nodes/textnode.h"

namespace qor { namespace data { namespace parser { namespace text {

    //text = [bom] *( determined_char )
    text::text(Parser* parser) :
        Sequence(parser,
            new_ref<Optional>(parser,
                new_ref<bom>(parser)
            ),
            new_ref<ZeroOrMore>(parser, new_ref<deferredchar>(parser))
            ,
            static_cast<uint64_t>(textToken::text))
    { }

    text::~text() = default;
    
    void text::Prepare()
    {
        log::debug("Looking for text...");
        GetParser()->PushNode(new_ref<TextNode>());
    }

    void text::Emit()
    {
        log::debug("Emitting text");
    }

    void text::Fail()
    {
        log::debug("...Didn't find text.");
        ref_of<Node>::type node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }

}}}}//qor::data::parser::json