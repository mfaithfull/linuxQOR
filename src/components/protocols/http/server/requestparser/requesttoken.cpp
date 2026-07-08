// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "nodes/requestnode.h"
#include "requesttoken.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"

using namespace qor;
using namespace qor::components::parser;

namespace qor { namespace components { namespace protocols { namespace http {

    request::request(qor::components::parser::Parser* parser) :
            qor::components::parser::Sequence( parser,
                new_ref<request_line>(parser),
                new_ref<qor::components::parser::Sequence>( parser,
                    new_ref<qor::components::parser::ZeroOrMore>( parser,
                        new_ref<field_line>(parser)
                    ),
                    new_ref<qor::components::parser::CRLF>(parser)
                ),
                static_cast<uint64_t>(httpRequestToken::request))
    { }

    request::~request() = default;

    void request::Prepare()
    {
        log::debug("Expecting an HTTP request.");
        GetParser()->PushNode(new_ref<RequestNode>().template AsRef<Node>());
    }

    void request::Emit()
    {
        log::debug("Emitting an HTTP request.");
        std::string method;
        std::string protocolVersion;
        Target uri;
        Params params;
        Headers m_headers;

        auto node = GetParser()->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();

            if(token == static_cast<uint64_t>(httpRequestToken::request_line))
            {
                auto reqlineNode = node.AsRef<RequestLineNode>();
                if(reqlineNode.IsNotNull())
                {
                    method = reqlineNode->GetObject()->m_method;
                    protocolVersion = reqlineNode->GetObject()->m_protocolVersion;
                    uri = reqlineNode->GetObject()->m_target;
                }
            }
            else if(token == static_cast<uint64_t>(httpRequestToken::field_line))
            {
                auto fieldlineNode = node.AsRef<FieldLineNode>();
                if(fieldlineNode.IsNotNull())
                {
                    auto key = fieldlineNode->GetObject()->m_fieldName;
                    for(std::string fieldValue : fieldlineNode->GetObject()->m_fieldValues)
                    {
                        m_headers.emplace(key, fieldValue);
                    }
                }
            }
            else if(token == static_cast<uint64_t>(parser::eToken::CarriageReturnLineFeed))
            {
                log::debug("Reached the end of the header.");
            }
            else
            {
                continuable("Request parsing failed. Unexpected token.");
            }

            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            auto requestNode = node.AsRef<RequestNode>();
            if(requestNode.IsNotNull())
            {
                requestNode->GetObject()->SetMethod(method);
                requestNode->GetObject()->SetVersion(1,1);//TODO:
                requestNode->GetObject()->SetPath(uri.GetPath());
                requestNode->GetObject()->SetParams(params);
                requestNode->GetObject()->SetHeaders(m_headers);
            }
            GetParser()->PushNode(node);
            log::debug("Parsed an HTTP request.");
        }
    }

    void request::Fail()
    {
        auto node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }

}}}}//qor::components::protocols::http
