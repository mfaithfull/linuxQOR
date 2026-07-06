// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_WORKFLOW
#define QOR_PP_H_FRAMEWORK_WORKFLOW

#include <stack>
#include <functional>

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "iworkflow.h"
#include "src/framework/event/delegate/delegate.h"

namespace qor{ 

    class qor_pp_module_interface_gcc(QOR_WORKFLOW) Workflow;

    namespace workflow{

        class qor_pp_module_interface(QOR_WORKFLOW) State
        {
        public:

            typedef ref_of<State>::type ref;

            std::function<void(void)> Enter;
            std::function<void(void)> Suspend;
            std::function<void(void)> Resume;
            std::function<void(void)> Leave;

            State(Workflow* workflow);
            State& operator = (const State&) = delete;
            State(const State&) = delete;
            virtual ~State();

        protected:

            Workflow* m_Workflow;
        };
    }//qor::workflow

    class qor_pp_module_interface_gcc(QOR_WORKFLOW) Workflow : public workflow::IWorkflow
    {
    public:

        qor_pp_module_interface(QOR_WORKFLOW) Workflow();
        qor_pp_module_interface(QOR_WORKFLOW) virtual ~Workflow();
        qor_pp_module_interface(QOR_WORKFLOW) Workflow(const Workflow& src);
        qor_pp_module_interface(QOR_WORKFLOW) Workflow& operator = (const Workflow& src);
        qor_pp_module_interface(QOR_WORKFLOW) virtual int Run();

        template< typename workflow_config_func>
        int Run(workflow_config_func&& configure)
        {
            configure(*this);
            return Run();
        }

        qor_pp_module_interface(QOR_WORKFLOW) virtual bool IsComplete() const;
        qor_pp_module_interface(QOR_WORKFLOW) void SetInitialState(ref_of<workflow::State>::type state);
        qor_pp_module_interface(QOR_WORKFLOW) void SetState(ref_of<workflow::State>::type state);
        qor_pp_module_interface(QOR_WORKFLOW) void PushState(ref_of<workflow::State>::type state);
        qor_pp_module_interface(QOR_WORKFLOW) void PopState();        
        qor_pp_module_interface(QOR_WORKFLOW) void SetComplete();
        qor_pp_module_interface(QOR_WORKFLOW) void SetComplete(int result);
        qor_pp_module_interface(QOR_WORKFLOW) void SetResult(int result);
        
        qor_pp_module_interface(QOR_WORKFLOW) virtual void Enter();
        qor_pp_module_interface(QOR_WORKFLOW) virtual void Suspend();
        qor_pp_module_interface(QOR_WORKFLOW) virtual void Resume();
        qor_pp_module_interface(QOR_WORKFLOW) virtual void Leave();

        qor_pp_module_interface(QOR_WORKFLOW) ref_of<workflow::State>::type GetInitialState() const;
        qor_pp_module_interface(QOR_WORKFLOW) ref_of<workflow::State>::type CurrentState();

    protected:
    
        int m_result{0};
        bool m_complete{false};
        ref_of<workflow::State>::type m_initialState;
        std::stack< ref_of<workflow::State>::type > m_StateStack;

    };

}//qor

#endif//QOR_PP_H_FRAMEWORK_WORKFLOW
