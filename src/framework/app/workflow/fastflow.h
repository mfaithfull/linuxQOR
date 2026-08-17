// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_FASTFLOW
#define QOR_PP_H_FRAMEWORK_FASTFLOW

#include <stack>
#include <functional>

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/memory/sources/fastsource.h"
#include "iworkflow.h"
#include "src/qor/function/tef.h"
#include "tefallocator.h"

namespace qor{ 

    class qor_pp_module_interface_gcc(QOR_WORKFLOW) Fastflow;

    namespace fastflow{

        class qor_pp_module_interface(QOR_WORKFLOW) Step
        {
        public:

            typedef ref_of<Step>::type ref;

            tef::Function<void(void)> Enter;            
            tef::Function<void(void)> Resume;
            tef::Function<void(void)> Leave;

            Step(Fastflow* fastflow);
            Step& operator = (const Step&) = delete;
            Step(const Step&) = delete;
            virtual ~Step();

        protected:

            Fastflow* m_Fastflow;
        };
    }//qor::fastflow

    qor_pp_declare_source_of(fastflow::Step, memory::FastSource)
    qor_pp_declare_source_of(typename ref_of<fastflow::Step>::type, memory::FastSource)

    class qor_pp_module_interface_gcc(QOR_WORKFLOW) Fastflow : public workflow::IWorkflow
    {
    public:

        qor_pp_module_interface(QOR_WORKFLOW) Fastflow();
        qor_pp_module_interface(QOR_WORKFLOW) virtual ~Fastflow();
        qor_pp_module_interface(QOR_WORKFLOW) Fastflow(const Fastflow& src);
        qor_pp_module_interface(QOR_WORKFLOW) Fastflow& operator = (const Fastflow& src);
        qor_pp_module_interface(QOR_WORKFLOW) virtual int Run();

        template< typename workflow_config_func>
        int Run(workflow_config_func&& configure)
        {
            configure(*this);
            return Run();
        }

        qor_pp_module_interface(QOR_WORKFLOW) virtual bool IsComplete() const;
        qor_pp_module_interface(QOR_WORKFLOW) void SetInitialStep(ref_of<fastflow::Step>::type step);
        qor_pp_module_interface(QOR_WORKFLOW) void SetStep(fastflow::Step* step);
        qor_pp_module_interface(QOR_WORKFLOW) void PushStep(fastflow::Step* step);
        qor_pp_module_interface(QOR_WORKFLOW) void PopStep();
        qor_pp_module_interface(QOR_WORKFLOW) void SetComplete();
        qor_pp_module_interface(QOR_WORKFLOW) void SetComplete(int result);
        qor_pp_module_interface(QOR_WORKFLOW) void SetResult(int result);
        
        qor_pp_module_interface(QOR_WORKFLOW) virtual void Enter();
        qor_pp_module_interface(QOR_WORKFLOW) virtual void Resume();
        qor_pp_module_interface(QOR_WORKFLOW) virtual void Leave();

        qor_pp_module_interface(QOR_WORKFLOW) fastflow::Step* GetInitialStep() const;
        qor_pp_module_interface(QOR_WORKFLOW) fastflow::Step* CurrentStep();

    protected:
    
        int m_result{0};
        bool m_complete{false};
        ref_of<fastflow::Step>::type m_initialStep;        
        std::stack< fastflow::Step*, std::vector< fastflow::Step* > > m_StepStack;
        TEFAllocator m_tefAllocator;
    };

}//qor

#endif//QOR_PP_H_FRAMEWORK_FASTFLOW
