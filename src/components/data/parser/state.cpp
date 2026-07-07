// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>

#include "src/platform/compiler/compiler.h"
#include "state.h"
#include "context.h"
#include "parser.h"

namespace qor { namespace components { namespace parser {

    ParserState::ParserState(Parser* parser, uint64_t token) : workflow::State(parser)
    {
        m_result.code = Result::UNINITIALIZED;
        m_result.length = 0;
        m_token = token;

        Leave = [this]()
        {
            if(m_result.code == Result::FAILURE)
            {
                Fail();
            }
            else if(m_result.code == Result::SUCCESS && m_result.length > 0 && m_result.token > static_cast<uint64_t>(eToken::Lexical))
            {
                if(m_token == static_cast<uint64_t>(eToken::Lexical) && m_result.token > static_cast<uint64_t>(eToken::Lexical))
                {
                    m_token = m_result.token;
                }
                Emit();
            }
            else if(m_result.code == Result::MORE_DATA)
            {
                std::cout << "Ran out of data before we could decide. Reenter with more data to try again." << std::endl;
            }
            else if(GetParser()->IsFinal() && m_result.token > static_cast<uint64_t>(eToken::Lexical))
            {
                Emit();
            }
        };
    }

    ParserState::~ParserState() = default;

    void ParserState::Reset()
    {
        m_result.code = Result::UNINITIALIZED;
        m_result.first = 0;
        m_result.m_position = 0;
        m_result.length = 0;
    }

    uint64_t ParserState::GetToken()
    {
        return m_token;
    }

    Context* ParserState::GetContext()
    {
        return dynamic_cast<Parser*>(m_Workflow)->GetContext();
    }

    Workflow* ParserState::Workflow()
    {
        return dynamic_cast<class Workflow*>(m_Workflow);
    }

    Parser* ParserState::GetParser()
    {
        return dynamic_cast<Parser*>(m_Workflow);
    }
    
    void ParserState::Prepare()
    {        
        //Default leaf states don't require a prepare step
    }
    
    void ParserState::Emit()
    {
        //Default leaf states emit a leaf node which is just a token
        GetParser()->PushNode(new_ref<Node>(m_token));
    }

    void ParserState::Fail()
    {        
        //Must undo anything done in Prepare
        //Failures are not final, just a dead end in the parse. Another later branch/option may succeed.
    }
    
    //Will literally accept anything. Don't use this in practice as it will consume all the rest of any data. It's useful for testing
    AcceptAll::AcceptAll(Parser* parser) : ParserState(parser)
    {        
        Enter = [this]()
        {            
            Prepare();
            byte* data = nullptr;
            m_result.code = Result::SUCCESS;
            if(GetContext()->GetOctet(data))
            {
                m_result.first = *data;
                m_result.m_position = GetContext()->GetPosition();
                GetContext()->ConsumeOctet();
                m_result.token = static_cast<uint64_t>(eToken::Octet);
                ++m_result.length;
            }
            else
            {
                return;                
            }
        };
    }

    AcceptAll::~AcceptAll() = default;

}}}//qor::components::parser
