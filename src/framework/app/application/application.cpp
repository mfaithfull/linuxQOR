// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "application.h"
#include "src/framework/app/role/role.h"

namespace qor{

    Application::Application(){ }
    Application::~Application(){ }

    Application& Application::SetRole(ref_of<app::IRole>::type role)
    {
        m_Role = role;
        return *this;
    }

    Application& Application::SetRole()
    {
        return SetRole<app::Role>();
    }

    ref_of<app::IRole>::type Application::GetRole() const
    {
        return m_Role;
    }

    Application& Application::SetWorkflow( ref_of<workflow::IWorkflow>::type workflow)
    {
        m_Workflow = workflow;
        return *this;
    }

    ref_of<workflow::IWorkflow>::type Application::GetWorkflow() const
    {
        return m_Workflow;        
    }
    
    std::string& Application::Name()
    {
        return m_Name;
    }

    void Application::SetName(const std::string& name) 
    { 
        m_Name = name; 
    }

    int Application::RunWorkflow( ref_of<workflow::IWorkflow>::type workflow )
    {
        SetWorkflow(workflow);
        return RunWorkflowInternal();
    }

    int Application::RunWorkflowInternal()
    {
        int result = -1;
        if(m_Role.IsNotNull())
        {
            m_Role->Setup();
        }
        
        if(m_Workflow.IsNotNull())
        {
            result = m_Workflow->Run();
            m_Workflow.Dispose();
        }

        if(m_Role.IsNotNull())
        {
            m_Role->Shutdown();
        }
        return result;
    }

    const char* Application::ProvideShortOptionString()
    {
        return "?";
    }

    app::Option* Application::ProvideLongOptions()
    {
        static app::Option longOptions[] =
        {
            //NAME      ARGUMENT				           FLAG	        SHORTNAME
            {"usage",   app::Option::optional_argument,    nullptr,     'u'},
            {nullptr,   0,						           nullptr,     0}
        };
        return longOptions;
    }

    void Application::ReceiveOptionSwitch(char c)
    {
    }

    void Application::ReceiveOptionParameter(char c, const char*)
    {
    }

    void Application::ReceiveLongOption(const char* option, const char* value)
    {
    }

    void Application::ReceiveNonOption(const char* parameter)
    {
        if(m_Path.empty())
        {
            m_Path = parameter;
        }
    }

    int Application::Run(ref_of<app::IRunable>::type runable)
    {
        int result = -1;
        if(m_Role.IsNotNull())
        {
            m_Role->Setup();
        }
        
        if(runable.IsNotNull())
        {
            result = runable->Run();
        }

        if(m_Role.IsNotNull())
        {
            m_Role->Shutdown();
        }
        return result;
    }

}//qor
