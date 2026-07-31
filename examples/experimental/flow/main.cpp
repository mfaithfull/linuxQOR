// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//An Application with a Workflow
#include "sdk/app.h"
#include "sdk/memory.h"
#include "customfastflow.h"

using namespace qor;
qor_pp_implement_module("Flow")

qor_pp_module_requires(ICurrentThread);
qor_pp_module_requires(ThreadHeap);

int main()
{    
    qor_pp_run_workflow(CustomFastflow);  //The CustomFastflow becomes the runnable component for our Application
}
