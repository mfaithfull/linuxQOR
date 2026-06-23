// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_WORK_CUSTOMWORKFLOW
#define QOR_PP_H_EXAMPLES_WORK_CUSTOMWORKFLOW

#include "src/framework/app/workflow/workflow.h"

/*A workflow is a stack of states forming a state machine
The state at the top of the stack is repeatedly Entered
until the stack is empty. States can pop themselves from
the stack or push other states onto it.
It's all a way of controlling the flow of a program
without having to write loops and function calls
explicitly. They are provided by the workflow automatically*/

class CustomWorkflow : public qor::workflow::Workflow
{
public:

    CustomWorkflow();
    virtual ~CustomWorkflow() = default;    

private:

    qor::workflow::State::ref beggining;
    qor::workflow::State::ref middle;
    qor::workflow::State::ref end;
};

#endif//QOR_PP_H_EXAMPLES_WORK_CUSTOMWORKFLOW