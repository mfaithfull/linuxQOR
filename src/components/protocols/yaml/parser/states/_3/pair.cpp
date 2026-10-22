// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include <iostream>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "pair.h"
#include "../../nodes/pair.h"
#include "../../nodes/value.h"
#include "../../nodes/string.h"

namespace qor { namespace protocols { namespace yaml {

    //pair = string name-separator value
    pair::pair(Parser* parser) :
        Sequence_t< string, Sequence_t< name_separator, value > >(parser, &m_str, &m_tail, static_cast<uint64_t>(yamlToken::pair)),
            m_v(parser),
            m_ns(parser),
            m_str(parser),
            m_tail(parser, &m_ns, &m_v)
    { }

    pair::~pair() noexcept = default;

    void pair::Prepare()
    {
        //log::debug("Looking for a Pair...");
        GetParser()->PushNode(new_ref<PairNode>());
    }

    void pair::Emit()
    {
        log::debug("Emitting a Pair.");
        std::string key;
        ref_of<ValueNode>::type valueNode;
        auto node = GetParser()->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();

            if(token == static_cast<uint64_t>(yamlToken::value))
            {
                auto value = node.AsRef<ValueNode>();
                valueNode = value;
            }
            else if(token == static_cast<uint64_t>(yamlToken::string))
            {
                auto stringNode = node.AsRef<StringNode>();
                key = stringNode->GetObject()->GetValue();
            }
            else
            {
                auto f = yamlTokenNames.find((yamlToken)token);
                std::string tokenName;
                if(f != yamlTokenNames.end())
                {
                    tokenName = f->second;
                }
                continuable("Unexpected: {0}", tokenName);
            }
            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            auto pairNode = node.AsRef<PairNode>();
            pairNode->GetObject()->SetKey(key);
            pairNode->GetObject()->GetValue() = valueNode->GetObject();
            GetParser()->PushNode(node);
        }
    }

    void pair::Fail()
    {
        //log::debug("...Didn't find a Pair.");
        uint64_t topToken = m_parser->TopNode()->GetToken();
        if(topToken == m_token)
        {
            m_parser->PopNode();
        }
    }

}}}//qor::protocols::yaml
