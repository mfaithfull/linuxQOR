// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "utf16lowbyte.h"
#include "../../nodes/utf16charnode.h"

namespace qor { namespace data { namespace parser { namespace text {

    UTF16LowByte::UTF16LowByte(Parser* parser) : OneOfARange(parser, 0x00, 0xFF, static_cast<uint64_t>(textToken::utf16lowbyte))
    { }

    UTF16LowByte::~UTF16LowByte() = default;
    
    void UTF16LowByte::Prepare()
    {
        log::debug("Looking for a UTF-16 low byte.");        
    }

    void UTF16LowByte::Emit()
    {
        log::debug("Emitting a UTF-16 low byte.");        
        ref_of<Node>::type node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() == static_cast<uint64_t>(textToken::utf16char))
        {
            node.AsRef<UTF16CharNode>()->SetLowByte(m_result.first);
        }
        if(node.IsNotNull())
        {
            GetParser()->PushNode(node);
        }
    }

    void UTF16LowByte::Fail()
    {
        log::debug("...Didn't find a UTF-16 low byte.");
    }

}}}}//qor::data::parser::json