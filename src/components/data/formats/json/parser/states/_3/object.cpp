// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "object.h"
#include "../_1/begin_object.h"
#include "../_2/end_object.h"
#include "../_3/member.h"
#include "../_4/value_separator.h"
#include "../../nodes/object.h"
#include "../../nodes/member.h"

namespace qor { namespace data { namespace parser { namespace json {

    //object = begin-object [ member *( value-separator member ) ] end-object
    object::object(Parser* parser) :
        Sequence(parser,
            new_ref<begin_object>(parser),
            new_ref<Sequence>(parser,
                new_ref<Optional>(parser,
                    new_ref<Sequence>(parser,
                        new_ref<member>(parser),
                        new_ref<ZeroOrMore>(parser,
                            new_ref<Sequence>(parser,
                                new_ref<value_separator>(parser),
                                new_ref<member>(parser)
                            )
                        )
                    )
                ),
                new_ref<end_object>(parser)
            ),
        static_cast<uint64_t>(jsonToken::object)){ }

    object::~object() = default;
    
    void object::Prepare()
    {
        log::debug("Looking for an Object...");
        GetParser()->PushNode(new_ref<ObjectNode>());
    }

    void object::Emit()
    {
        log::debug("Emitting an Object");
        std::vector<ref_of<MemberNode>::type> members;
        auto node = GetParser()->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();
            auto f = jsonTokenNames.find((jsonToken)token);
            std::string tokenName;
            if(f != jsonTokenNames.end())
            {
                tokenName = f->second;
            }

            if(token == static_cast<uint64_t>(jsonToken::member))
            {
                auto memberNode = node.AsRef<MemberNode>();
                members.push_back(memberNode);
            }
            else
            {                
                continuable("Unexpected {0}", tokenName);                
            }
            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            for(auto it = members.rbegin(); it != members.rend(); ++it)
            {
                node.AsRef<ObjectNode>()->GetObject()->m_members.push_back((*it)->GetObject());
            }
            GetParser()->PushNode(node);
        }
    }

    void object::Fail()
    {
        log::debug("...Didn't find an Object.");
        ref_of<Node>::type node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }

}}}}//qor::data::parser::json