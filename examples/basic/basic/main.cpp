// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0
//A minimal QOR application

#include "sdk/app.h"

using namespace qor;                                //Use the qor namespace

constexpr const char* appName = "BasicApp";         //Give the application a name
qor_pp_implement_module(appName)                    //Every QOR application must also be a Module

int main()                                          //Standard C++ entry point
{
    return AppBuilder().
        Build(appName)(qor_shared).Run(             //Use the AppBuilder factory to make an Application and then Run it
        []()->int                                   //We provide a lambda to be run in this case
        {
            std::cout << "Hello from a basic QOR application." << std::endl;
            return EXIT_SUCCESS;                    //Standard success return code (0) is returned from the program
        });
}