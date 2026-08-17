// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/flyers/log/debug.h"
#include "array.h"
#include "begin_array.h"
#include "../_4/value.h"
#include "../_4/value_separator.h"
#include "../_2/end_array.h"
#include "../../nodes/array.h"
#include "../../nodes/value.h"
#include "../../helpers.h"

namespace qor { namespace data { namespace parser { namespace json {

    //array = begin-array [ value *( value-separator value ) ] end-array
    array::array(Parser* parser) :
        Sequence(parser,
            new_ref<begin_array>(parser),
            new_ref<Sequence>(parser,
                new_ref<Optional>(parser,
                    new_ref<Sequence>(parser,
                        new_ref<value>(parser),
                        new_ref<ZeroOrMore>(parser,
                            new_ref<Sequence>(parser,
                                new_ref<value_separator>(parser),
                                new_ref<value>(parser)
                            )
                        )
                    )
                ),
                new_ref<end_array>(parser)
            ),static_cast<uint64_t>(jsonToken::array)),
            m_parser(parser)
    { }

    array::~array() = default;

    void array::Prepare()
    {
        log::debug("Looking for an Array...");
        m_parser->PushNode(new_ref<ArrayNode>().AsRef<Node>());
    }

    void array::Emit()
    {        
        log::debug("Emitting an Array:");
        std::vector<ref_of<ValueNode>::type> values;
        auto node = m_parser->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();

            if(token == static_cast<uint64_t>(jsonToken::value))
            {
                auto valueNode = node.AsRef<ValueNode>();
                values.push_back(valueNode);
            }
            else
            {
                std::string tokenName = GetTokenName(token);
                continuable("Unexpected: {0}", tokenName);
            }
            node = m_parser->PopNode();
        }

        if(node.IsNotNull())
        {
            for(auto it = values.rbegin(); it != values.rend(); ++it)
            {
                node.AsRef<ArrayNode>()->GetObject()->m_values.push_back((*it)->GetObject());
            }
            m_parser->PushNode(node);
        }
    }

    void array::Fail()
    {
        //log::debug("...Didn't find an Array.");
        uint64_t topToken = m_parser->TopNode()->GetToken();        
        if(topToken == m_token)
        {
            m_parser->PopNode();
        }
    }
}}}}//qor::data::parser::json
