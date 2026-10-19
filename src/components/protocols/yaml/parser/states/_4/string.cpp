// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/debug.h"
#include "src/qor/essentials/text/codepage/codepages/utf8.h"
#include "string.h"
#include "../../nodes/string.h"
#include "../../nodes/character.h"
#include "../../helpers.h"

namespace qor { namespace protocols { namespace yaml {

    UTF8CodePage string::s_codePage;

    string::string(Parser* parser) :
        Sequence_t< quotation_mark, Sequence_t< ZeroOrMore_t< character >, quotation_mark > >(parser, &m_qopen, &m_tail, static_cast<uint64_t>(yamlToken::string)),
            m_qopen(parser),
            m_qclose(parser),
            m_char(parser),
            m_body(parser, &m_char),
            m_tail(parser, &m_body, &m_qclose)
    { }

    string::~string() noexcept = default;

    void string::Prepare()
    {
        StandardPrepare<StringNode>(*this);
    }

    void string::Emit()
    {
        std::string stringValue;
        auto node = GetParser()->PopNode();
        while(node.IsNotNull() && node->GetToken() != m_token)
        {
            uint64_t token = node->GetToken();
            if(token == static_cast<uint64_t>(yamlToken::character))
            {
                auto characterNode = node.AsRef<CharacterNode>();
                char32_t c = static_cast<char32_t>(characterNode->GetObject()->GetValue());
                char8_t space[6];
                char8_t* p = &space[0];
                size_t available = 1;
                if(s_codePage.Encode(c, p, available))
                {
                    stringValue = std::string((const char*)space, (p - &space[0])) + stringValue;
                }
            }
            else
            {
                std::string tokenName = GetTokenName(token);
                continuable("Unexpected: {0}", tokenName);
            }
            node = GetParser()->PopNode();
        }
        if(node.IsNotNull())
        {
            auto stringNode = node.AsRef<StringNode>();
            stringNode->GetObject()->SetValue(stringValue);
            GetParser()->PushNode(node);
        }
        else
        {
            StandardFail<StringNode>(*this);
        }
    }

    void string::Fail()
    {
        StandardFail<StringNode>(*this);
    }

}}}//qor::protocols::yaml