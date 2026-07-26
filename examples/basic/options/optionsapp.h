// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_OPTIONS_APP
#define QOR_PP_H_EXAMPLES_OPTIONS_APP

#include <string>
#include "sdk/app.h"

/*To customise the Application class we derive from it.
We implement the IOptionable interface to receive options
on our derived class*/
class OptionsApp : public qor::Application
{
    //We must redirect the Application factory to use the OptionsApp Factory
    qor_pp_declare_app_class(OptionsApp)

public:

    constexpr static const char* Name = "options";

    OptionsApp() = default;
    virtual ~OptionsApp() = default;

    virtual const std::string Description();
    virtual const std::vector<qor::app::NamedArgSpec> NamedArguments();    

    //Interfaces to provide properties, determined from the options to the rest of the program
    std::string GetFileName();
    long long GetOrder();

private:

    std::string m_filename;
    long long m_order{0};

};

//Declare the OptionsApp to be a Singleton, access protected with a RecursiveMutex
qor_pp_declare_singleton(OptionsApp);

#endif//QOR_PP_H_EXAMPLES_OPTIONS_APP