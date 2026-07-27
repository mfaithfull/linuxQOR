// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "utf8char.h"
#include "multibyteutf8.h"
#include "../../nodes/textnode.h"

namespace qor { namespace data { namespace parser { namespace text {

    utf8char::utf8char(Parser* parser) : AnyOneOf(parser, 
        new_ref<OneOfARange>(parser, 0x00, 0x7F),
        new_ref<MultiByteUTF8Char>(parser),
        static_cast<uint64_t>(textToken::utf8char))
    { }

    utf8char::~utf8char() = default;
    
    void utf8char::Prepare()
    {
        log::debug("Looking for a UTF-8 char");        
    }

    void utf8char::Emit()
    {
        if(m_result.first >= 0x00 && m_result.first <= 0x7F)
        {
            log::debug("Emitting a UTF-8 char");
            ref_of<Node>::type node = GetParser()->PopNode();
            if(node.IsNotNull() && node->GetToken() == static_cast<uint64_t>(textToken::text))
            {
                node.AsRef<TextNode>()->GetObject()->push_back(static_cast<char>(m_result.first));
            }
            if(node.IsNotNull())
            {
                GetParser()->PushNode(node);
            }
        }   
    }

    void utf8char::Fail()
    {
        log::debug("...Didn't find UTF-8 char.");
        //detect invalid utf-8 and decide what to do with it.
        //probably just emit placeholder and move on.
    }

}}}}//qor::data::parser::json