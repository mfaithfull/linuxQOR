// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//An Application with command line options using the QOR SDK
//For a simpler example of the flow of a QOR application see the outline project

#include "sdk/app.h"
#include "optionsapp.h"                                         //Include the declaration of our custom application type
#include "src/framework/app/role/getfeature.h"

using namespace qor;
using namespace qor::app;

qor_pp_implement_module(OptionsApp::Name)

int main(const int argc, const char** argv, char** env)
{    
    return AppBuilder().Build<OptionsApp>(                      //We parameterise the Build function with our custom application type
        OptionsApp::Name,
        [argc,argv](ref_of<OptionsApp>::type app)               //Pass a configuration function for the application to the builder
        {                                                       //capturing the arguments to main so we can pass them to the argument parser
            app(qor_shared).ParseArgs(argc, argv);              //Parse the argument from the command line and pass them to the OptionsApp
        }
    )(qor_unlocked).Run(                                        //Unlock the OptionsApp and Run a simple lambda
        []()->int
        {            
            auto app = GetApplication<OptionsApp>();
            std::cout << "File name: " << app(qor_shared).GetFileName() << std::endl;
            long long orderNumber = app(qor_shared).GetOrder();
            if( orderNumber != 0)
            {
                std::cout << "Optional Order Number: " << orderNumber << std::endl;
            }
            return EXIT_SUCCESS;
        }
    );
}
