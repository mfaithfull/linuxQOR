// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "utf16lechar.h"
#include "utf16lowbyte.h"
#include "utf16highbyte.h"
#include "../../nodes/utf16charnode.h"
#include "../../nodes/textnode.h"
#include "src/qor/essentials/text/codepage/codepages/utf16.h"

namespace qor { namespace data { namespace parser { namespace text {

    utf16LEchar::utf16LEchar(Parser* parser) : Sequence(parser, 
        new_ref<UTF16LowByte>(parser), 
        new_ref<UTF16HighByte>(parser),
        static_cast<uint64_t>(textToken::utf16LEchar))
    { }

    utf16LEchar::~utf16LEchar() = default;
    
    void utf16LEchar::Prepare()
    {
        log::debug("Looking for a UTF-16LE char");    
        GetParser()->PushNode(new_ref<UTF16CharNode>());    
    }

    void utf16LEchar::Emit()
    {
        log::debug("Emitting a UTF-16LE char");
        auto node = GetParser()->PopNode();
        if(node.IsNotNull())        
        {
            auto utf16char = node.AsRef<UTF16CharNode>();
            if(utf16char.IsNotNull())
            {
                UTF16CodePage utf16codePage;
                UTF8CodePage UTF8CodePage;
                unsigned short s = *(utf16char->GetObject());
                const char16_t* u16p = reinterpret_cast<char16_t*>(&s);
                auto textNode = GetParser()->PopNode();
                size_t available = 1;
                CodePoint cp = utf16codePage.Decode(u16p, available);
                char8_t tmpBuffer[6];
                available = 6;
                char8_t* tmp = &tmpBuffer[0];
                UTF8CodePage.Encode(cp, tmp, available);
                for(int length = 0; length < (6 - available); ++length)
                {
                    textNode.AsRef<TextNode>()->GetObject()->push_back(tmpBuffer[length]);
                }
                GetParser()->PushNode(textNode);
            }
        }
    }

    void utf16LEchar::Fail()
    {
        log::debug("...Didn't find UTF-16LE char.");
        auto node = GetParser()->PopNode();
        if(node.IsNotNull())
        {
            auto utf16char = node.AsRef<UTF16CharNode>();
            if(utf16char.IsNull())
            {
                GetParser()->PushNode(node);
            }
        }
    }

}}}}//qor::data::parser::json