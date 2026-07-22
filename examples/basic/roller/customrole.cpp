// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "customrole.h"

using namespace qor;
using namespace qor::app;

void CustomRole::Setup()
{
    //Code here runs before any features are Setup. 
    //Warning: No services will be available

    IRole::template AddFeature<CustomFeature>(

        //These get customisation points just like builtin Featues
        [](ref_of<CustomFeature>::type customFeature)->void
        {
            customFeature->ConfigureMessage("Hello from a custom feature.");
        }

    );

    //Always call the base class to do the actual setup
    Role::Setup();

    //Code here runs after all featues are Setup. 
    //This is the first point where all services will be available
}

void CustomRole::Shutdown()
{
    //At this point all features are still available but should only be used to clean up

    // Always call the base class at the end to actually shutdown everything.
    Role::Shutdown();

    //Warning: Any code here runs after all features are shutdown. 
    //There is very little that is safe to do here unless it only uses the Standard Library and statically linkedQOR libraries
}