// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "request_linetoken.h"
#include "src/qor/flyers/log/debug.h"

namespace qor { namespace components { namespace protocols { namespace http {

    request_line::request_line(parser::Parser* parser) :
                parser::Sequence( parser,
                    new_ref<method>(parser),
                    new_ref<parser::Sequence>( parser,
                        new_ref<parser::SP>(parser),
                        new_ref<parser::Sequence>( parser,
                            new_ref<target>(parser),
                            new_ref<parser::Sequence>( parser,
                                new_ref<parser::SP>(parser),
                                new_ref<parser::Sequence>(parser,
                                    new_ref<protocol>(parser),
                                    new_ref<parser::CRLF>(parser)
                                )
                            )
                        )
                    ),
                    static_cast<uint64_t>(httpRequestToken::request_line)
                )
    { }

    request_line::~request_line() = default;

    void request_line::Prepare()
    {
        log::debug("Expecting a request line.");
        GetParser()->PushNode(new_ref<RequestLineNode>());
    }

    void request_line::Emit()
    {
        log::debug("Emitting a request line.");
        std::string method;
        std::string protocolName;
        std::string protocolVersion;
        Target target;
        auto node = GetParser()->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();
            auto f = httpRequestTokenNames.find((httpRequestToken)token);
            std::string tokenName;
            if(f != httpRequestTokenNames.end())
            {
                tokenName = f->second;
            }

            if(token == static_cast<uint64_t>(httpRequestToken::method))
            {
                auto methodNode = node.AsRef<MethodNode>();
                if(methodNode.IsNotNull())
                {
                    method = methodNode->GetObject()->m_methodName;
                }
            }
            else if(token == static_cast<uint64_t>(httpRequestToken::target))
            {
                auto targetNode = node.AsRef<TargetNode>();
                if(targetNode.IsNotNull())
                {
                    target = targetNode->GetObject()()();
                }
            }
            else if(token == static_cast<uint64_t>(httpRequestToken::protocol))
            {
                auto protocolNode = node.AsRef<ProtocolNode>();
                if(protocolNode.IsNotNull())
                {
                    protocolName = protocolNode->GetObject()->m_protocolName;
                    protocolVersion = protocolNode->GetObject()->m_protocolVersion;
                }
            }
            else
            {
                continuable("Unexpected token.");
            }
            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            auto requestLineNode = node.AsRef<RequestLineNode>();
            if(requestLineNode.IsNotNull())
            {
                requestLineNode->GetObject()->m_method = method;
                requestLineNode->GetObject()->m_protocolVersion = protocolVersion;
                requestLineNode->GetObject()->m_target = target;
            }
            GetParser()->PushNode(node);
        }
    }

    void request_line::Fail()
    {
        auto node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }

}}}}//qor::components::protocols::http
