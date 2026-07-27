// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "multibyteutf8.h"
#include "followbyte.h"
#include "../../nodes/textnode.h"

namespace qor { namespace data { namespace parser { namespace text {

    MultiByteUTF8Char::MultiByteUTF8Char(Parser* parser) : ParserState(parser, static_cast<uint64_t>(textToken::utf8char))
    {
        Enter = [this]()
        {
            Prepare();

            byte* data = nullptr;
            if (GetContext()->GetOctet(data))
            {
                //work out how many more bytes to expect
                uint8_t decoded_length = codepoint_decoded_length[static_cast<char8_t>(*data)];
                if(decoded_length < 1 || decoded_length > 6)
                {
                    m_result.code = Result::FAILURE;
                }
                else
                {
                    m_result.first = *data;
                    m_result.m_position = GetContext()->GetPosition();
                    GetContext()->ConsumeOctet();
                    m_result.token = m_token;
                    ++m_result.length;
                    m_result.code = Result::SUCCESS;         
                    log::debug("Emitting a multibyte UTF-8 char");
                    ref_of<Node>::type node = GetParser()->PopNode();
                    if(node.IsNotNull() && node->GetToken() == static_cast<uint64_t>(textToken::text))
                    {
                        node.AsRef<TextNode>()->GetObject()->push_back(static_cast<char>(m_result.first));
                    }
                    if(node.IsNotNull())
                    {
                        GetParser()->PushNode(node);
                    }
                    for(uint8_t byteCount = 1; byteCount < decoded_length; byteCount++)
                    {
                        GetParser()->PushState(new_ref<FollowByte>(GetParser()));
                    }
                }
            }
            else
            {                
                m_result.code = Result::MORE_DATA;//Need more data
            }
        };
    }

    MultiByteUTF8Char::~MultiByteUTF8Char() = default;
    
    void MultiByteUTF8Char::Prepare()
    {
        log::debug("Looking for a multibyte UTF-8 char");        
    }

    void MultiByteUTF8Char::Emit()
    {        
    }

    void MultiByteUTF8Char::Fail()
    {
        log::debug("...Didn't find multibyte UTF-8 char.");
    }

}}}}//qor::data::parser::json