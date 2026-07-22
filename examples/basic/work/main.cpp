// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//An Application with a Workflow
#include "sdk/app.h"
#include "customworkflow.h"             //Workflows are always overrides of the base Workflow class.

using namespace qor;
qor_pp_implement_module("Work")

int main()
{    
    qor_pp_run_workflow(CustomWorkflow);  //The CustomWorkflow becomes the runnable component for our Application
}
