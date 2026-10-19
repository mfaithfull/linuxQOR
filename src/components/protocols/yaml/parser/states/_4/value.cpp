// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>
#include <iostream>
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "value.h"
#include "../_1/_false.h"
#include "../_1/_null.h"
#include "../_1/_true.h"
#include "../_3/mapping.h"
#include "../_1/sequence.h"
#include "string.h"
#include "../../nodes/value.h"
#include "../../nodes/_false.h"
#include "../../nodes/_true.h"
#include "../../nodes/_null.h"
#include "../../nodes/mapping.h"
#include "../../nodes/sequence.h"
#include "../../nodes/string.h"

namespace qor { namespace protocols { namespace yaml {

    //value = false / null / true / mapping / sequence / string
    value::value(Parser* parser) :
        AnyOneOf(parser,
            new_ref<_false>(parser),
            new_ref<AnyOneOf>(parser,
                new_ref<_null>(parser),
                new_ref<AnyOneOf>(parser,
                    new_ref<_true>(parser),
                    new_ref<AnyOneOf>(parser,
                        new_ref<deferred<mapping>>(parser, static_cast<uint64_t>(yamlToken::mapping)),
                        new_ref<AnyOneOf>(parser,
                            new_ref<deferred<sequence>>(parser, static_cast<uint64_t>(yamlToken::sequence)),
                            new_ref<string>(parser)
                        )
                    )
                )
            ),
        static_cast<uint64_t>(yamlToken::value)){ }

    value::~value() noexcept = default;

    void value::Prepare()
    {
        log::debug("Looking for a Value...");
        GetParser()->PushNode(new_ref<ValueNode>().AsRef<Node>());
    }

    void value::Emit()
    {
        log::debug("Emiting a Value:");
        model::yaml::ValueType vt = model::yaml::ValueType::_null;
        ref_of<Node>::type valueNode;
        auto node = GetParser()->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();

            switch((yamlToken)token)
            {
            case yamlToken::_false:
                vt = model::yaml::ValueType::_false;
                valueNode = node;
                break;
            case yamlToken::_true:
                vt = model::yaml::ValueType::_true;
                valueNode = node;
                break;
            case yamlToken::_null:
                vt = model::yaml::ValueType::_null;
                valueNode = node;
                break;
            case yamlToken::mapping:
                vt = model::yaml::ValueType::mapping;
                valueNode = node;
                break;
            case yamlToken::sequence:
                vt = model::yaml::ValueType::sequence;
                valueNode = node;
                break;
            case yamlToken::string:
                vt = model::yaml::ValueType::string;
                valueNode = node;
                break;
            default:
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
            if( valueNode.IsNotNull())
            {
                auto thisNode = node.AsRef<ValueNode>();
                switch(vt)
                {
                    case model::yaml::ValueType::_false:
                    {
                        auto model = valueNode.AsRef< NodeAdapter< model::yaml::_False > >()->GetObject();
                        thisNode->GetObject()->SetValue(polymorphic<model::yaml::ValueModel>(*model));
                    }
                    break;
                    case model::yaml::ValueType::_true:
                    {
                        auto model = valueNode.AsRef< NodeAdapter< model::yaml::_True > >()->GetObject();
                        thisNode->GetObject()->SetValue(polymorphic<model::yaml::ValueModel>(*model));
                    }
                    break;
                    case model::yaml::ValueType::_null:
                    {
                        auto model = valueNode.AsRef< NodeAdapter< model::yaml::_Null > >()->GetObject();
                        thisNode->GetObject()->SetValue(polymorphic<model::yaml::ValueModel>(*model));
                    }
                    break;
                    case model::yaml::ValueType::mapping:
                    {
                        auto model = valueNode.AsRef< NodeAdapter< model::yaml::Mapping > >()->GetObject();
                        thisNode->GetObject()->SetValue(polymorphic<model::yaml::ValueModel>(*model));
                    }
                    break;
                    case model::yaml::ValueType::sequence:
                    {
                        auto model = valueNode.AsRef< NodeAdapter< model::yaml::Sequence > >()->GetObject();
                        thisNode->GetObject()->SetValue(polymorphic<model::yaml::ValueModel>(*model));
                    }
                    break;
                    case model::yaml::ValueType::string:
                    {
                        auto model = valueNode.AsRef< NodeAdapter< model::yaml::String > >()->GetObject();
                        thisNode->GetObject()->SetValue(polymorphic<model::yaml::ValueModel>(*model));
                    }
                    break;
                    default:
                    {
                        continuable("YAML Value must be false, true, null, mapping, sequence or string.");
                    }
                }
            }
            GetParser()->PushNode(node);
        }
    }

    void value::Fail()
    {
        //log::debug("...Didn't find a Value.");
        uint64_t topToken = m_parser->TopNode()->GetToken();
        if(topToken == m_token)
        {
            m_parser->PopNode();
        }
    }

}}}//qor::protocols::yaml
