// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//An Application with command line options using the QOR SDK
//For a simpler example of the flow of a QOR application see the outline project

#include "sdk/app.h"
#include "optionsapp.h"                                         //Include the declaration of our custom application type

using namespace qor;

qor_pp_implement_module(OptionsApp::Name)

int main(const int argc, const char** argv, char** env)
{    
    return AppBuilder().Build<OptionsApp>(                      //We parameterise the Build function with our custom application type
        OptionsApp::Name,
        [argc,argv,env](ref_of<OptionsApp>::type app)           //Pass a configuration function for the application to the builder
        {                                                       //capturing the arguments to main so we can pass them to the OptionGetter
                                                                //Parse the options from the command line and pass them to the OptionsApp
            app::OptionGetter options(argc, argv, app(qor_shared));
        }
    )->Run(
        []()->int
        {
            std::cout << "Hello world." << std::endl;
            return EXIT_SUCCESS;
        }
    );
}