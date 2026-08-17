// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/debug.h"
#include "value.h"
#include "../_1/_false.h"
#include "../_1/_null.h"
#include "../_1/_true.h"
#include "../_3/number.h"
#include "../_3/object.h"
#include "../_4/string.h"
#include "../_1/array.h"
#include "../../nodes/value.h"
#include "../../nodes/string.h"
#include "../../nodes/_false.h"
#include "../../nodes/_true.h"
#include "../../nodes/_null.h"
#include "../../nodes/array.h"
#include "../../nodes/number.h"
#include "../../nodes/object.h"

namespace qor { namespace data { namespace parser { namespace json {

    //value = false / null / true / object / array / number / string
    value::value(Parser* parser) : 
        AnyOneOf(parser,
            new_ref<_false>(parser),
            new_ref<AnyOneOf>(parser,
                new_ref<_null>(parser),
                new_ref<AnyOneOf>(parser,
                    new_ref<_true>(parser),
                    new_ref<AnyOneOf>(parser,
                        new_ref<deferred<object>>(parser, static_cast<uint64_t>(jsonToken::object)),
                        new_ref<AnyOneOf>(parser,
                            new_ref<deferred<array>>(parser, static_cast<uint64_t>(jsonToken::array)),
                            new_ref<AnyOneOf>(parser,
                                new_ref<number>(parser),
                                new_ref<string>(parser)
                            )
                        )
                    )
                )
            ),
        static_cast<uint64_t>(jsonToken::value)){ }

    value::~value() = default;
    
    void value::Prepare()
    {
        log::debug("Looking for a Value...");
        GetParser()->PushNode(new_ref<ValueNode>().AsRef<Node>());
    }

    void value::Emit()
    {
        log::debug("Emiting a Value:");
        model::json::ValueType vt = model::json::ValueType::_null;
        ref_of<Node>::type valueNode;
        auto node = GetParser()->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();

            switch((jsonToken)token)
            {
            case jsonToken::_false:
                vt = model::json::ValueType::_false;
                valueNode = node;
                break;
            case jsonToken::_true:
                vt = model::json::ValueType::_true;
                valueNode = node;
                break;
            case jsonToken::_null:
                vt = model::json::ValueType::_null;
                valueNode = node;
                break;
            case jsonToken::object:
                vt = model::json::ValueType::object;
                valueNode = node;
                break;
            case jsonToken::array:
                vt = model::json::ValueType::array;
                valueNode = node;
                break;
            case jsonToken::number:
                vt = model::json::ValueType::number;
                valueNode = node;
                break;
            case jsonToken::string:
                vt = model::json::ValueType::string;
                valueNode = node;
                break;
            default:
                auto f = jsonTokenNames.find((jsonToken)token);
                std::string tokenName;
                if(f != jsonTokenNames.end())
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
                    case model::json::ValueType::_false:
                    {
                        auto model = valueNode.AsRef< NodeAdapter< model::json::_False > >()->GetObject();
                        thisNode->GetObject()->SetValue(model);
                    }
                    break;
                    case model::json::ValueType::_true:
                    {
                        auto model = valueNode.AsRef< NodeAdapter< model::json::_True > >()->GetObject();
                        thisNode->GetObject()->SetValue(model);
                    }
                    break;
                    case model::json::ValueType::_null:
                    {
                        auto model = valueNode.AsRef< NodeAdapter< model::json::_Null > >()->GetObject();
                        thisNode->GetObject()->SetValue(model);
                    }
                    break;
                    case model::json::ValueType::object:
                    {
                        auto model = valueNode.AsRef< NodeAdapter< model::json::Object > >()->GetObject();
                        thisNode->GetObject()->SetValue(model);
                    }
                    break;
                    case model::json::ValueType::array:
                    {
                        auto model = valueNode.AsRef< NodeAdapter< model::json::Array > >()->GetObject();
                        thisNode->GetObject()->SetValue(model);
                    }
                    break;
                    case model::json::ValueType::number:
                    {
                        auto model = valueNode.AsRef< NodeAdapter< model::json::Number > >()->GetObject();
                        thisNode->GetObject()->SetValue(model);
                    }
                    break;
                    case model::json::ValueType::string:
                    {
                        auto model = valueNode.AsRef< NodeAdapter< model::json::String > >()->GetObject();
                        thisNode->GetObject()->SetValue(model);
                    }
                    break;
                    default:
                    {
                        continuable("JSON Value must be false, true, null, object, array, number or string.");
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

}}}}//qor::data::parser::json
