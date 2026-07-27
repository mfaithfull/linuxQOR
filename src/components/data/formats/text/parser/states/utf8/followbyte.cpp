// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "followbyte.h"
#include "../../nodes/textnode.h"

namespace qor { namespace data { namespace parser { namespace text {

    FollowByte::FollowByte(Parser* parser) : OneOfARange(parser, 0x80, 0xBF, static_cast<uint64_t>(textToken::utf8char))
    { }

    FollowByte::~FollowByte() = default;
    
    void FollowByte::Prepare()
    {
        log::debug("Looking for a continuation byte.");        
    }

    void FollowByte::Emit()
    {
        log::debug("Emitting a continuation byte.");
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

    void FollowByte::Fail()
    {
        log::debug("...Didn't find a continuation byte.");
    }

}}}}//qor::data::parser::json