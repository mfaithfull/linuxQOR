// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>

#include "workflow.h"
#include "src/qor/flyers/error/error.h"

namespace qor{ 
    
    namespace workflow{

        State::State(Workflow* workflow) : m_Workflow(workflow)
        {
            Enter = std::bind(&Workflow::Enter, workflow);
            Suspend = std::bind(&Workflow::Suspend, workflow);
            Resume = std::bind(&Workflow::Resume, workflow);
            Leave = std::bind(&Workflow::Leave, workflow);
        }

        State::~State() = default;
    }//qor::workflow

    Workflow::Workflow(){ }
    Workflow::~Workflow() = default;

    Workflow::Workflow(const Workflow& src) : Workflow()
    {
        *this = src;
    }

    Workflow& Workflow::operator = (const Workflow& src)
    {
        m_complete = src.m_complete;
        m_StateStack = src.m_StateStack;
        m_initialState = src.m_initialState;
        return *this;
    }

    int Workflow::Run()
    {   
        m_complete = false;
        if(m_StateStack.empty())
        {
            serious("No initial state set for workflow.");
            return -1;
        }
        try{   
            while(!IsComplete())
            {
                CurrentState()->Enter();
            }
            while(!m_StateStack.empty())
            {
                PopState();
            }
        }
        catch(const Serious& error)
        {
            std::cerr << "Workflow failed due to: " << error.what().Content() << '\n';
        }
        catch(const std::exception& e)
        {
            std::cerr << "Workflow failed due to: " << e.what() << '\n';
        }
        catch(...)
        {
            std::cerr << "Workflow failed due to unhandled exception.\n";
        }
        return m_result;
    }

    void Workflow::Enter()
    {
        m_complete = true;
    }
    
    void Workflow::Suspend(){}
    
    void Workflow::Resume(){}

    void Workflow::Leave(){}

    ref_of<workflow::State>::type Workflow::CurrentState()
    {
        if( !m_StateStack.empty() )
        {
            return m_StateStack.top().Clone();
        }
        return nullptr;
    }

    ref_of<workflow::State>::type Workflow::GetInitialState() const
    {
        return m_initialState;
    }

    void Workflow::SetInitialState(ref_of<workflow::State>::type initialState)//weak pointer
    {
        if(initialState && m_StateStack.empty())
        {
            m_initialState = initialState;
            m_StateStack.push(initialState.Clone());
            m_complete = false;
        }
    }

    void Workflow::SetState(ref_of<workflow::State>::type newState)
    {
        if(newState)
        {
            if(!m_StateStack.empty())
            {
                ref_of<workflow::State>::type currentState = CurrentState();
                if(currentState)
                {
                    currentState->Leave();
                }
                m_StateStack.pop();
            }
            m_StateStack.push(newState.Clone());
        }
    }

    void Workflow::PushState(ref_of<workflow::State>::type newState)
    {
        if(newState)
        {
            if(!m_StateStack.empty())
            {
                ref_of<workflow::State>::type currentState = CurrentState();
                if(currentState)
                {
                    currentState->Suspend();
                }
            }
            m_StateStack.push(newState.Clone());
        }
    }

    void Workflow::PopState()
    {
        if(!m_StateStack.empty())
        {
    		ref_of<workflow::State>::type currentState = CurrentState();
            if(currentState)
            {
    	    	currentState->Leave();
            }
            m_StateStack.pop();		
            if(!m_StateStack.empty())
            {
                ref_of<workflow::State>::type newCurrentState = CurrentState();
                if(newCurrentState)
                {
        	        newCurrentState->Resume();
                }
            }
            else
            {
                SetComplete();
            }
        }
    }

    bool Workflow::IsComplete() const
    {
        return m_complete;
    }

    void Workflow::SetComplete()
    {
        m_complete = true;
    }

    void Workflow::SetComplete(int result)
    {
        SetResult(result);
        m_complete = true;
    }

    void Workflow::SetResult(int result)
    {
        m_result = result;
    }

}//qor
