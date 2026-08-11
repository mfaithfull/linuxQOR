// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_APPLICATION
#define QOR_PP_H_APPLICATION

#include <string>

#include "src/framework/app/role/irole.h"
#include "src/framework/app/workflow/iworkflow.h"
#include "src/qor/module/module.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/instance/singleton.h"
#include "src/framework/parallel/sync/recursive_mutex.h"
#include "src/platform/platform.h"
#include "irunable.h"
#include "../argparse/iargumented.h"
//#include "../optionparser/ioptionable.h"

#define qor_pp_declare_app_class(_MYAPP)\
private: static qor::SingletonRedirector<qor::Application, _MYAPP> m_sRedirect;

#define qor_pp_redirect_app_class(_MYAPP)\
qor::SingletonRedirector<qor::Application, _MYAPP> _MYAPP::m_sRedirect;

namespace qor{

    class qor_pp_module_interface_gcc(QOR_APPLICATION) Application : public app::IArgumented
    {
    public:

        static const app::OptConfig defaultOptConfig;
        static const std::vector<app::NamedArgSpec> defaultNamedArgs;
        static const std::vector<app::PositionalArgSpec> defaultPositionalArgs;

        qor_pp_module_interface(QOR_APPLICATION) Application();
        qor_pp_module_interface(QOR_APPLICATION) virtual ~Application();

        qor_pp_module_interface(QOR_APPLICATION) Application& SetRole( ref_of<app::IRole>::type role);

        template<class TRole>
        Application& SetRole()
        {
            return SetRole( new_ref<TRole>().template AsRef<app::IRole>() );
        }

        template<class TRole, typename TConfigureRole>
        Application& SetRole(TConfigureRole&& config_function)
        {
            auto role = new_ref<TRole>().template AsRef<app::IRole>();
            config_function(role);
            return SetRole(role);
        }

        qor_pp_module_interface(QOR_APPLICATION) Application& SetRole();
        qor_pp_module_interface(QOR_APPLICATION) ref_of<app::IRole>::type GetRole() const;
        qor_pp_module_interface(QOR_APPLICATION) Application& SetWorkflow( ref_of<workflow::IWorkflow>::type workflow);
        qor_pp_module_interface(QOR_APPLICATION) ref_of<workflow::IWorkflow>::type GetWorkflow() const;
        qor_pp_module_interface(QOR_APPLICATION) const std::string Name() override;
        qor_pp_module_interface(QOR_APPLICATION) void SetName(const std::string& name);

        template<class TSubsystem>
		Application& AddSubSystem()
		{
            ThePlatform(qor_shared)->template AddSubsystem<TSubsystem>();
			return *this;
		}

        qor_pp_module_interface(QOR_APPLICATION) int RunWorkflow( ref_of<workflow::IWorkflow>::type workflow );

        template< class TWorkflow >
		int RunWorkflow()
		{
			auto workflow = new_ref<TWorkflow>();
			int result = RunWorkflow(workflow.template AsRef<workflow::IWorkflow>());
            return result;
		}

        template< class TWorkflow, typename config_func >
		int RunWorkflow(config_func configureworkflowAction)
		{
			auto workflow = new_ref<TWorkflow>();
			configureworkflowAction(workflow.template AsRef<workflow::IWorkflow>());
			return RunWorkflow(workflow.template AsRef<workflow::IWorkflow>());
		}

        template<typename Tlambda>
        int Run(Tlambda&& func)
        {
            return Run(new_ref<app::RunableFunc>(func).template AsRef<app::IRunable>());
        }

        qor_pp_module_interface(QOR_APPLICATION) int Run( ref_of<app::IRunable>::type runable );

        qor_pp_module_interface(QOR_APPLICATION) virtual const std::string Description() override;
        qor_pp_module_interface(QOR_APPLICATION) virtual const std::string UsageEpilogue() override;
        qor_pp_module_interface(QOR_APPLICATION) virtual const std::string OverrideUsage() override;
        qor_pp_module_interface(QOR_APPLICATION) virtual const app::OptConfig Config() override;
		qor_pp_module_interface(QOR_APPLICATION) virtual const std::vector<app::NamedArgSpec> NamedArguments() override;
		qor_pp_module_interface(QOR_APPLICATION) virtual const std::vector<app::PositionalArgSpec> PositionalArguments() override;

        qor_pp_module_interface(QOR_APPLICATION) void ParseArgs(const int argc, const char** argv);

    private:

        qor_pp_module_interface(QOR_APPLICATION) int RunWorkflowInternal();

        std::string m_Path;
        std::string m_Name;
        ref_of<app::IRole>::type m_Role;
        ref_of<workflow::IWorkflow>::type m_Workflow;

    };

    qor_pp_declare_instancer_of(Application, SingletonInstancer);
    qor_pp_declare_sync_of(Application, RecursiveMutex);
}//qor

#endif//QOR_PP_H_APPLICATION
