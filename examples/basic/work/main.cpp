// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//An Application with a Workflow

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/framework/app/application/builder.h"
#include "src/framework/app/role/role.h"
#include "customworkflow.h"             //Workflows are always overrides of the base Workflow class.

using namespace qor;
constexpr const char* appName = "Work";
qor_pp_implement_module(appName)

int main()
{    
    return AppBuilder().
        Build(appName)(qor_unlocked).
        SetRole<app::Role>().
        RunWorkflow<CustomWorkflow>();  //The CustomWorkflow becomes the runnable component for our Application
}
