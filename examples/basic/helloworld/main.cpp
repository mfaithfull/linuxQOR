// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0
//A minimal QOR application

#include "sdk/app.h"                                //Include the things we need to make a QOR Application

using namespace qor;                                //Use the qor namespace

qor_pp_implement_module("Hello World App")          //Every QOR Application must also be a QOR Module

int main()                                          //Standard C++ entry point
{
    qor_pp_run(                                     //Run a base Application
    []()->int                                       //This is what gets Run. A simple lambda here but it could be other things.
    {
        std::cout << "Hello from a simple QOR application." << std::endl;
        return EXIT_SUCCESS;                        //Standard success return code (0) is returned from the program
    });
}
