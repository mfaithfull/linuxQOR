// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>

#include "src/platform/compiler/compiler.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "parser.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/debug.h"
#include "src/qor/flyers/log/imperative.h"
#include "state.h"

namespace qor { namespace data {

    Parser::Parser() : Fastflow(), m_final(false){ }

    Parser::Parser(ref_of<class parser::Context>::type context) : Fastflow(), m_context(context){ }

    Parser::~Parser() = default;

    parser::Context* Parser::GetContext() const
    {
        return m_context;
    }

    void Parser::SetContext(ref_of<class parser::Context>::type context)
    {
        m_context = context;
    }

    void Parser::PushNode(ref_of<parser::Node>::type node)
    {
        if(node.IsNotNull())
        {
            m_nodes.push(node);
        }
    }

    ref_of<parser::Node>::type& Parser::TopNode()
    {
        return m_nodes.top();
    }
    
    ref_of<parser::Node>::type Parser::PopNode()
    {
        ref_of<parser::Node>::type result;
        if(!m_nodes.empty())
        {
            result = m_nodes.top();
            m_nodes.pop();
        }

        if(m_nodes.empty())
        {
            Diagnostic();
        }
        return result;
    }

    //Call once all input data has been converted
    //Drains any remaining stack of AST Nodes into the
    //associated object model.
    void Parser::Drain()
    {
        qor_pp_ofcontext;
        while(!IsComplete() && m_StepStack.size() > 0)
        {
            while(!IsComplete() && m_context->HasUnparsedData())
            {
                CurrentStep()->Enter();
            }
            PopStep();
            if(IsComplete())
            {
                log::debug("Parse complete.");
                break;
            }
        }
    }

    //Parses up to the end of the available data
    //Intended to be called repeatedly as more data is available
    //until all the data is processed into AST Nodes
    void Parser::InnerParse()
    {
        qor_pp_ofcontext;

        while(!IsComplete() && m_context->HasUnparsedData())
        {
            CurrentStep()->Enter();
        }
        if(IsComplete())
        {
            log::debug("Initial parse complete.");
        }
        if(!m_context->HasUnparsedData())
        {
            log::debug("Parse ran out of data.");
        }
    }

    int Parser::SafeParse()
    {
        qor_pp_ofcontext;

        if(m_inError)
        {
            continuable("Parse error. Parser cannot continue.");
            return -1;
        }
        try
        {
            log::debug("Stack on entry has {0} states, {1} nodes", m_StepStack.size(), m_nodes.size());
            m_final ? Drain() : InnerParse();
        }
        catch(const Serious& error)
        {
            log::imperative("Parser failed: {0}", error.what().Content());
            m_inError = true;
        }
        catch(const std::exception& e)
        {
            log::imperative("Parser failed: {0}", e.what());
            m_inError = true;
        }
        catch(...)
        {
            log::imperative("Parser failed due to unhandled exception.");
            m_inError = true;
        }

        log::debug("Stack on exit has {0} states, {1} nodes", m_StepStack.size(), m_nodes.size());

        if(!m_final && m_StepStack.size() == 0 )
        {
            log::debug("Parse finished early.");

            if(m_nodes.size() == 0)
            {
                log::debug("Nothing found.");
            }

        }

#ifndef NDEBUG
        std::stack< ref_of<fastflow::Step>::type, std::vector< ref_of<fastflow::Step>::type > > copy = m_StepStack; // Copy the stack
        while (!copy.empty())
        {
            uint64_t token = copy.top().AsRef<parser::ParserState>()->GetToken();
            std::cout << "token: " << token << "\n";
            copy.pop();
        }
#endif

        return m_result;
    }

    int Parser::FinalParse()
    {
        qor_pp_ofcontext;

        if(m_final)
        {
            return m_result;
        }
        m_final = true;
        log::debug("Entering final parse.");
        m_complete = false;
        if(m_StepStack.empty())
        {
            log::debug("Final parse not required.");
            log::debug("Stack has {0} states, {1} nodes", m_StepStack.size(), m_nodes.size());
            return m_result;
        }
        auto result = SafeParse();
        log::debug("Final parse complete.");
        log::debug("Stack has {0} states, {1} nodes", m_StepStack.size(), m_nodes.size());
        return result;
    }

    int Parser::Parse()
    {
        qor_pp_ofcontext;

        log::debug("Partial parse starting.");
        m_final = false;
        m_complete = false;
        if(m_StepStack.empty())
        {
            serious("No initial state set for Parser.");
            return -1;
        }
        int result = SafeParse();
        log::debug("Partial parse complete.");
        return result;
    }

    bool Parser::IsFinal()
    {
        return m_final;
    }

    bool Parser::IsInError()
    {
        return m_inError;
    }

    void Parser::Diagnostic()
    {
        qor_pp_ofcontext;

        std::string finalParse = m_final ? "Yes" : "No";
        std::string inError = m_inError ? "Yes" : "No";

        log::debug("Parser diagnostics: In final parse = {0}, In Error = {1}, Node stack size = {2}, State stack size = {3}", finalParse, inError, m_nodes.size(), m_StepStack.size());

        if(m_nodes.size() > 0)
        {
            auto node = m_nodes.top();
            node->Diagnostic();
        }
    }

}}//qor::data
