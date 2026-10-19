// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <stack>
#include <iostream>

#include "requestgenerator.h"
#include "state.h"
#include "src/qor/flyers/error/error.h"

namespace qor { namespace components { namespace protocols { namespace http {

    HTTPRequestGenerator::HTTPRequestGenerator(ref_of<Context>::type context) : Workflow(), m_context(context)
    {
        SetInitialState(new_ref<RequestGenInitial>(this));
    }

    int HTTPRequestGenerator::Run()
    {
        m_complete = false;
        if(m_StateStack.empty())
        {
            serious("No initial state set for HTTPRequestGenerator.");
            return -1;
        }
        try
        {
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
                note("Generator completed successfuly within available buffer.");
            }
            if(!m_context->HasSpace())
            {
                note("Filled output buffer. Run generator again after flushing.");
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
            std::cerr << "HTTPRequestGenerator failed due to unhandled exception.\n";
        }

        std::string stackNote = std::format("Stack on exit has {0} states.", m_StateStack.size());
        note(stackNote);
        return m_result;
    }

}}}}//qor::components::protocols::http
