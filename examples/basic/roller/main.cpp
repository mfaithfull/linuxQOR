// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

/*An Application with a Custom Role using the QOR SDK
QOR Applications each have a Role. The Role is a container of Features.
The Role manages the Setup and Shutdown of the Features*/

#include "sdk/app.h"
#include "customrole.h"                         //Include our custom Role class.
#include "src/framework/app/role/getfeature.h"  //must be included last to get a shortcut function to Features

using namespace qor;
using namespace qor::app;

qor_pp_implement_module("Role based App")

int main()
{
    qor_pp_run_role(CustomRole)(
    []()->int
    {
        GetFeature<CustomFeature>(qor_shared)->SayHello();
        return EXIT_SUCCESS;
    });
}
