// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "tokentoken.h"
#include "nodes/tokennode.h"
#include "src/qor/flyers/log/debug.h"

namespace qor { namespace components { namespace protocols { namespace http {

    void token::Prepare()
    {
        log::debug("Looking for a basic token.");
        GetParser()->PushNode(new_ref<TokenNode>());
    }

    void token::Emit()
    {
        log::debug("Emitting a basic token.");
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

            log::debug(tokenName);
            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            GetParser()->PushNode(node);
        }
    }

    void token::Fail()
    {
        ref_of<parser::Node>::type node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }


    method::method(parser::Parser* parser) : parser::OneOrMore(parser, new_ref<tchar>(parser),
                static_cast<uint64_t>(httpRequestToken::method))
    { }

    method::~method() = default;

    void method::Prepare()
    {
        log::debug("Expecting a method.");
        GetParser()->PushNode(new_ref<MethodNode>());
    }

    void method::Emit()
    {
        log::debug("Emitting a method.");
        auto node = GetParser()->PopNode();
        std::string methodName;
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();
            auto f = httpRequestTokenNames.find((httpRequestToken)token);
            std::string tokenName;
            if(f != httpRequestTokenNames.end())
            {
                tokenName = f->second;
            }

            //log::debug(tokenName);

            if(token == static_cast<uint64_t>(httpRequestToken::tchar))
            {
                auto tcharNode = node.AsRef<TCharNode>();
                char c = tcharNode->GetObject()->m_char;
                char U = toupper(c);
                methodName = std::string(&U,1) + methodName;
            }
            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            auto methodNode = node.AsRef<MethodNode>();
            if(methodNode.IsNotNull())
            {
                methodNode->GetObject()->m_methodName = methodName;
                //std::cout << "Method: " << methodName << std::endl;
            }
            GetParser()->PushNode(node);
        }
    }

    void method::Fail()
    {
        ref_of<parser::Node>::type node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }

    void field_name::Prepare()
    {
        log::debug("Expecting a field name.");
        GetParser()->PushNode(new_ref<FieldNameNode>());
    }

    void field_name::Emit()
    {
        log::debug("Emitting a field name.");
        auto node = GetParser()->PopNode();
        std::vector<char> fieldNameChars;
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();
            auto f = httpRequestTokenNames.find((httpRequestToken)token);
            std::string tokenName;
            if(f != httpRequestTokenNames.end())
            {
                tokenName = f->second;
            }

            if(token == static_cast<uint64_t>(httpRequestToken::tchar))
            {
                auto tcharNode = node.AsRef<TCharNode>();
                char c = tcharNode->GetObject()->m_char;
                fieldNameChars.push_back(c);
            }
            else
            {
                char c = (char)m_result.first;
                fieldNameChars.push_back(c);
            }
            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            auto fieldnameNode = node.AsRef<FieldNameNode>();
            if(fieldnameNode.IsNotNull())
            {
                std::string fieldName(fieldNameChars.rbegin(), fieldNameChars.rend());
                fieldnameNode->GetObject()->m_fieldName = fieldName;
                //std::cout << "Field Name: " << fieldName << std::endl;
            }
            GetParser()->PushNode(node);
        }
    }

    void field_name::Fail()
    {
        ref_of<parser::Node>::type node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }

    protocol_version::protocol_version(parser::Parser* parser) : parser::OneOrMore(parser,
        new_ref<tchar>(parser),
        static_cast<uint64_t>(httpRequestToken::protocol_version))
    { }

    protocol_version::~protocol_version() = default;

    void protocol_version::Prepare()
    {
        log::debug("Expecting a protocol version.");
        GetParser()->PushNode(new_ref<ProtocolVersionNode>());
    }

    void protocol_version::Emit()
    {
        log::debug("Emitting a protocol version.");
        auto node = GetParser()->PopNode();
        std::string version;
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();
            auto f = httpRequestTokenNames.find((httpRequestToken)token);
            std::string tokenName;
            if(f != httpRequestTokenNames.end())
            {
                tokenName = f->second;
            }

            log::debug(tokenName);

            if(token == static_cast<uint64_t>(httpRequestToken::tchar))
            {
                auto tcharNode = node.AsRef<TCharNode>();
                char c = tcharNode->GetObject()->m_char;
                version = std::string(&c,1) + version;
            }
            else
            {
                char c = '?';
                version = std::string(&c,1) + version;
            }
            node = GetParser()->PopNode();
        }

        if(node.IsNotNull())
        {
            auto versionNode = node.AsRef<ProtocolVersionNode>();
            if(versionNode.IsNotNull())
            {
                versionNode->GetObject()->m_version = version;
                //std::cout << "Protocol Version: " << version << std::endl;
            }
            GetParser()->PushNode(node);
        }
    }

    void protocol_version::Fail()
    {
        ref_of<parser::Node>::type node = GetParser()->PopNode();
        if(node.IsNotNull() && node->GetToken() != m_token)
        {
            GetParser()->PushNode(node);
        }
    }
}}}}//qor::components::protocols::http