// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_RESPONSEPARSER
#define QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_RESPONSEPARSER

#include <map>

#include "src/components/data/parser/state.h"
#include "src/components/data/parser/states/rfc5234.h"
#include "src/components/data/parser/tokens.h"
#include "src/components/data/parser/parser.h"
#include "src/components/data/parser/states/oneormore.h"
#include "src/components/data/parser/nodes/char.h"

namespace qor { namespace components { namespace protocols { namespace echo {

    //Client side parser for interpretting response
    //Belongs to the EchoResponseFilter
    enum class echoResponseToken : uint64_t
    {
        responseChar = static_cast<uint64_t>(qor::components::parser::eToken::Max) + 1ull,
        response,
    };

    static const std::map<const uint64_t, const std::string> echoResponseTokenNames = {{
        {static_cast< const uint64_t>(echoResponseToken::responseChar), "responseChar"},
        {static_cast< const uint64_t>(echoResponseToken::response), "response"},
    }};

    class responseChar : public qor::components::parser::OneOfARange
    {
    public: responseChar(qor::components::parser::Parser* parser) :
                qor::components::parser::OneOfARange(parser,
                0x00,
                0xFF,
                static_cast<uint64_t>(echoResponseToken::responseChar))
            { }

        virtual ~responseChar() = default;
        virtual void Emit()
        {
            char charValue = (char)m_result.first;
            GetParser()->PushNode(
                qor::new_ref<qor::components::parser::CharNode>(
                    charValue,static_cast<uint64_t>(echoResponseToken::responseChar)
                ).template AsRef<qor::components::parser::Node>()
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

    class qor_pp_module_interface(QOR_ECHO) response : public qor::components::parser::OneOrMore
    {
    public: 
        response(qor::components::parser::Parser* parser);
        virtual ~response();
        virtual void Prepare();
        virtual void Emit();
        virtual void Fail();
    };

}}}}//qor::components::protocols::echo

#endif//QOR_PP_H_COMPONENTS_PROTOCOLS_ECHO_RESPONSEPARSER