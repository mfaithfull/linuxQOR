// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef QOR_PP_H_APPLICATION
#define QOR_PP_H_APPLICATION

#include <string>

#include "src/framework/role/irole.h"
#include "src/framework/workflow/iworkflow.h"
#include "src/qor/instance/singleton.h"
#include "src/system/system/system.h"

namespace qor{ namespace framework{

    class qor_pp_module_interface(QOR_APPLICATION) Application
    {
    public:

        Application() = default;
        virtual ~Application() = default;

        Application& SetRole( ref_of<IRole>::type role);

        template<class TRole>
        Application& SetRole()
        {
            return SetRole( new_ref<TRole>().template AsRef<IRole>() );
        }

        template<class TRole, typename TConfigureRole>
        Application& SetRole(TConfigureRole&& config_function)
        {
            auto role = new_ref<TRole>().template AsRef<IRole>();
            config_function(role);
            return SetRole(role);
        }

        ref_of<IRole>::type GetRole();
        Application& SetWorkflow( ref_of<workflow::IWorkflow>::type workflow);
        ref_of<workflow::IWorkflow>::type GetWorkflow();
        std::string& Name();

        template<class TSubsystem>
		Application& AddSubSystem()
		{
            TheSystem()->template AddSubsystem<TSubsystem>();
			return *this;
		}
        
        int RunWorkflow( ref_of<workflow::IWorkflow>::type workflow );

        template< class TWorkflow >
		int RunWorkflow()
		{
			auto workflow = new_ref<TWorkflow>();
			return RunWorkflow(workflow.template AsRef<workflow::IWorkflow>());
		}

        template< class TWorkflow, typename config_func >
		int RunWorkflow(config_func configureworkflowAction)
		{
			auto workflow = new_ref<TWorkflow>();
			configureworkflowAction(workflow.template AsRef<workflow::IWorkflow>());
			return RunWorkflow(workflow.template AsRef<workflow::IWorkflow>());
		}

    private:

        int RunWorkflowInternal();

        std::string m_Name;
        ref_of<IRole>::type m_Role;
        ref_of<workflow::IWorkflow>::type m_Workflow;

    };
}
    qor_pp_declare_instancer_of(framework::Application, SingletonInstancer);
}//qor::framework

#endif//QOR_PP_H_APPLICATION
