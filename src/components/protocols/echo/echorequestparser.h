// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_REQUESTPARSER
#define QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_REQUESTPARSER

#include "src/components/data/parser/state.h"
#include "src/components/data/parser/states/rfc5234.h"
#include "src/components/data/parser/tokens.h"
#include "src/components/data/parser/parser.h"
#include "src/components/data/parser/states/oneormore.h"
#include "src/components/data/parser/nodes/char.h"

namespace qor { namespace components { namespace protocols { namespace echo {

    //Server side parser for interpretting request. Belongs to the EchoServiceFilter
    
    enum class echoRequestToken : uint64_t
    {
        requestChar = static_cast<uint64_t>(qor::components::parser::eToken::Max) + 1ull,
        request,
    };

    static const std::map<const uint64_t, const std::string> echoRequestTokenNames = {{
        {static_cast< const uint64_t>(echoRequestToken::requestChar), "requestChar"},
        {static_cast< const uint64_t>(echoRequestToken::request), "request"},
    }};

    class requestChar : public qor::components::parser::OneOfARange
    {
    public: requestChar(qor::components::parser::Parser* parser) :
                qor::components::parser::OneOfARange(parser,
                0x00,
                0xFF,
                static_cast<uint64_t>(echoRequestToken::requestChar))
            {}
        virtual ~requestChar() = default;
        virtual void Emit()
        {
            char charValue = (char)m_result.first;
            GetParser()->PushNode(
                qor::new_ref<qor::components::parser::CharNode>(
                    charValue,static_cast<uint64_t>(echoRequestToken::requestChar)
                )
            );
        }
        virtual void Fail()
        {
            if(m_result.code == qor::components::parser::Result::MORE_DATA)
            {
                //we're simply out of data. That's fine for an echo. Echo what we have
                m_result.code = qor::components::parser::Result::SUCCESS;                
                Workflow()->SetComplete();
            }            
            Workflow()->PopState();
        }
    };

    class request : public qor::components::parser::OneOrMore
    {
    public: request(qor::components::parser::Parser* parser) :
                qor::components::parser::OneOrMore( parser,
                    qor::new_ref<requestChar>(parser),
                    static_cast<uint64_t>(echoRequestToken::request))
            {}

        virtual ~request() = default;

        virtual void Prepare();
        virtual void Emit();
        virtual void Fail()
        {
            auto node = GetParser()->PopNode();
            if(node.IsNotNull() && node->GetToken() != m_token)
            {
                GetParser()->PushNode(node);
            }
        }
    };
    
}}}}//qor::components::protocols::echo

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_REQUESTPARSER