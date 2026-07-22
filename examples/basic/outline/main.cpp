// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Outline application using the QOR SDK

#include "sdk/app.h"
#include "src/qor/module/moduleregistry.h"  //exceptional include not provided by the SDK, inernal header for module introspection only.
#include "src/framework/app/host/host.h"    //exceptional include not provided by the SDK, inernal header for module introspection only.

using namespace qor;
using namespace qor::app;

/*Our application is an executable module in the QOR ecosystem
The qor_pp_implement_module macro gives us a named, versioned module object
Every QOR executable must:
1. Link the qor_module static library to become a module
2. Link the qor_host module to become the host for other modules
3. provide an implementation of qor::Module& ThisModule(void) equivalent to this macro.
*/
qor_pp_implement_module("Outline")

void AccessModules();

int main()
{    
    /*The QOR Application is a context for running anything else that meets the requirements for a runable object*/
    qor_pp_run_role(Role)(
    []()->int
    {            
        AccessModules();
        return EXIT_SUCCESS;
    });
}

/*This (for science only) function can be used to access every loaded QOR module and it's constituent libraries.*/
void AccessModules()
{
    //Visit all the QOR modules loaded into this process
    TheHost()->Modules()->VisitModules(
        [](Module* module)
        {
            std::cout << "Module: " << module->Name() << " : " << module->Version() << std::endl;
            //Visit all the QOR static libraries linked into the module.
            module->VisitLibraries(
                [](Library* library)
                {
                    std::cout << "-Library: " << library->Name() << " : " << library->Version() << std::endl;
                }
            );
        }
    );
    //The last module on the list will be this executable itself
}
