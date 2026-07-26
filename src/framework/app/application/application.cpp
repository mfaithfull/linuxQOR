// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "application.h"
#include "src/framework/app/role/role.h"
#include "src/framework/app/argparse/argumentparser.h"

namespace qor{

    const app::OptConfig Application::defaultOptConfig{
        .allowAbbreviation = true,
        .addHelp = true,
        .ignoreUnknownArgs = true,
        .prefixChars = '-'
    };

    const std::vector<app::NamedArgSpec> Application::defaultNamedArgs = {
        /*
        {
            "option",
            "",
            1,
            ArgType::String,
            false,
            "This should be an optional -option option."
        }
        */
    };
    
    const std::vector<app::PositionalArgSpec> Application::defaultPositionalArgs = {
        /*
        {
            
            "first",
            1,
            app::ArgType::String,
            true,
            "Whatever is passed first ends up here"
        }
        */
    };

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

    Application& Application::SetWorkflow(ref_of<workflow::IWorkflow>::type workflow)
    {
        m_Workflow = workflow;
        return *this;
    }

    ref_of<workflow::IWorkflow>::type Application::GetWorkflow() const
    {
        return m_Workflow;
    }

    const std::string Application::Name()
    {
        return m_Name;
    }

    void Application::SetName(const std::string& name)
    {
        m_Name = name;
    }

    int Application::RunWorkflow(ref_of<workflow::IWorkflow>::type workflow)
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

    const std::string Application::Description()
    {
        return std::string("QOR Application");
    }

    const std::string Application::UsageEpilogue()
    {
        return std::string("Built with the QOR framwork.");
    }

    const std::string Application::OverrideUsage()
    {
        return std::string();
    }

    const app::OptConfig Application::Config()
    {
        return defaultOptConfig;
    }

    const std::vector<app::NamedArgSpec> Application::NamedArguments()
    {
        return defaultNamedArgs;
    }

    const std::vector<app::PositionalArgSpec> Application::PositionalArguments()
    {
        return defaultPositionalArgs;
    }

    void Application::ParseArgs(const int argc, const char** argv)
    {
        auto parser = app::ArgumentParser(*this);
        auto args = parser.ParseArgs(argc,argv);
        if(!args.IsArgValid())
        {            
            std::cout << parser.GetUsage() << std::endl;
            std::cout << args.GetErrorString() << std::endl;
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
