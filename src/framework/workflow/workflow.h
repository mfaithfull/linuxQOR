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

#ifndef QOR_PP_H_FRAMEWORK_WORKFLOW
#define QOR_PP_H_FRAMEWORK_WORKFLOW

#include <stack>
#include "src/qor/delegate/delegate.h"

namespace qor{ namespace workflow{

    enum Transition
    {
        Enter,
        Suspend,
        Resume,
        Leave
    };

    typedef Delegate<void(Transition)> state_t;

    class qor_pp_module_interface(QOR_WORKFLOW) Workflow
    {
    public:

        Workflow();
        virtual ~Workflow() = default;
        Workflow(const Workflow& src);
        Workflow& operator = (const Workflow& src);
        void Start();
        bool IsComplete() const;
    protected:
        virtual void InitialStateHandler(Transition t);
        void DefaultHandle(Transition t);
        state_t CurrentState();
        void SetState(state_t state);
        void PushState(state_t state);
        void PopState();        
        state_t GetInitialState();
        bool m_complete;
        std::stack< state_t > m_StateStack;
    };

}}//qor::workflow

#endif//QOR_PP_H_FRAMEWORK_WORKFLOW
