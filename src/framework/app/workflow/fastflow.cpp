// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>

#include "fastflow.h"
#include "src/qor/flyers/error/error.h"

namespace qor{ 
    
    namespace fastflow{

        Step::Step(Fastflow* fastflow) : m_Fastflow(fastflow)
        {
            //Enter = std::bind(&Fastflow::Enter, fastflow);
            //Resume = std::bind(&Fastflow::Resume, fastflow);
            //Leave = std::bind(&Fastflow::Leave, fastflow);
        }

        Step::~Step() = default;
    }//qor::fastflow

    Fastflow::Fastflow()
    {
        tef::SetCustomAllocator(
            [](size_t size, size_t alignment, void* context) 
            {
                return reinterpret_cast<TEFAllocator*>(context)->Allocate(size, alignment);
            }, &m_tefAllocator,

            [](void* address, size_t size, size_t alignment, void* context) 
            {
                return reinterpret_cast<TEFAllocator*>(context)->Deallocate(address, size, alignment);
            }, &m_tefAllocator
        );

        std::vector< fastflow::Step* > stepVector;
        stepVector.reserve(64);
        m_StepStack = std::stack< fastflow::Step*, std::vector< fastflow::Step* > >(std::move(stepVector));
    }

    Fastflow::~Fastflow() = default;

    Fastflow::Fastflow(const Fastflow& src) : Fastflow()
    {
        *this = src;
    }

    Fastflow& Fastflow::operator = (const Fastflow& src)
    {
        m_complete = src.m_complete;
        m_StepStack = src.m_StepStack;
        m_initialStep = src.m_initialStep;
        return *this;
    }

    int Fastflow::Run()
    {   
        m_complete = false;
        if(m_StepStack.empty())
        {
            serious("No initial step set for fastflow.");
            return -1;
        }
        try
        {   
            while(!IsComplete())
            {
                CurrentStep()->Enter();
            }
            while(!m_StepStack.empty())
            {
                PopStep();
            }
        }
        catch(const Serious& error)
        {
            std::cerr << "Fastflow failed due to: " << error.what().Content() << '\n';
        }
        catch(const std::exception& e)
        {
            std::cerr << "Fastflow failed due to: " << e.what() << '\n';
        }
        catch(...)
        {
            std::cerr << "Fastflow failed due to unhandled exception.\n";
        }
        return m_result;
    }

    void Fastflow::Enter()
    {
        m_complete = true;
    }
    
    void Fastflow::Resume(){}

    void Fastflow::Leave(){}

    fastflow::Step* Fastflow::CurrentStep()
    {
        if( !m_StepStack.empty() )
        {
            return m_StepStack.top();
        }
        return nullptr;
    }

    fastflow::Step* Fastflow::GetInitialStep() const
    {
        return m_initialStep;
    }

    void Fastflow::SetInitialStep(ref_of<fastflow::Step>::type initialStep)//weak pointer
    {
        if(initialStep && m_StepStack.empty())
        {
            m_initialStep = initialStep;
            m_StepStack.push(initialStep);
            m_complete = false;
        }
    }

    void Fastflow::SetStep(fastflow::Step* newState)
    {
        if(newState)
        {
            if(!m_StepStack.empty())
            {
                fastflow::Step* currentStep = CurrentStep();
                if(currentStep)
                {
                    currentStep->Leave();
                }
                m_StepStack.pop();
            }
            m_StepStack.push(newState);
        }
    }

    void Fastflow::PushStep(fastflow::Step* newStep)
    {
        if(newStep)
        {
            m_StepStack.push(newStep);
        }
    }

    void Fastflow::PopStep()
    {
        if(!m_StepStack.empty())
        {
    		fastflow::Step* currentStep = m_StepStack.top();
            if(currentStep)
            {
    	    	currentStep->Leave();
            }
            m_StepStack.pop();		
            if(!m_StepStack.empty())
            {
                fastflow::Step* newCurrentStep = m_StepStack.top();
                if(newCurrentStep)
                {
        	        newCurrentStep->Resume();
                }
            }
            else
            {
                SetComplete();
            }
        }
    }

    bool Fastflow::IsComplete() const
    {
        return m_complete;
    }

    void Fastflow::SetComplete()
    {
        m_complete = true;
    }

    void Fastflow::SetComplete(int result)
    {
        SetResult(result);
        m_complete = true;
    }

    void Fastflow::SetResult(int result)
    {
        m_result = result;
    }

}//qor
