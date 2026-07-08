// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <stack>
#include <iostream>

#include "responsegenerator.h"
#include "state.h"

namespace qor { namespace components { namespace protocols { namespace http { namespace response {
    
    Generator::Generator(ref_of<parser::Context>::type context) : Workflow(), m_context(context)
    {
        SetInitialState(new_ref<GenerateResponse>(this));
    }

    int Generator::Run()
    {   
        m_complete = false;
        if(m_response.IsNull())
        {
            serious("No response object set on HTTP response generator.");
            return -1;
        }
        if(m_StateStack.empty())
        {
            serious("No initial state set for Generator.");
            return -1;
        }
        try{   
            while(!IsComplete() && m_context->HasSpace())
            {
                auto ref = CurrentState().operator->();
                if(ref)
                {
                    ref->Enter();                
                }
            }
            if(IsComplete())
            {
                note("HTTP response generation completed in the available buffer space.");
            }
            else if(!m_context->HasSpace())
            {
                note("HTTP response generation ran out of buffer space. Continue after sending partial reponse.");
            }
            else
            {
                warning("HTTP response generator quit without reaching completion. Check output.");
            }
        }
        catch(const Serious& error)
        {
            std::cerr << error.what().Content() << '\n';
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        catch(...)
        {
            std::cerr << "HTTP Response Generator failed due to unhandled exception.\n";
        }

        std::string noteString = std::format("Stack on exit has {0} states", m_StateStack.size());
        note(noteString);
        return m_result;
    }

}}}}}//qor::components::protocols::http::response

