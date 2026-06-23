// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_OPTIONS_APP
#define QOR_PP_H_EXAMPLES_OPTIONS_APP

#include <string>

#include "sdk/using_framework.h"
#include "sdk/components/framework.h"

/*To customise the Application class we derive from it.
We implement the IOptionable interface to receive options
on our derived class*/
class OptionsApp : public Application
{
private:

    /*Because Application is a singleton we must redirect the Application
    factory to use the OptionsApp Factory at static initialisation time*/
    qor_pp_declare_app_class(OptionsApp)

public:

    constexpr static const char* Name = "Options App";

    OptionsApp() = default;
    virtual ~OptionsApp() = default;

    //IOptionable interface
    virtual const char* ProvideShortOptionString();                         //Implement to tell the parser what short options we support
    virtual app::Option* ProvideLongOptions();                              //Implement to tell the parser what long options we support
    virtual void ReceiveOptionSwitch(char c);                               //Implement to receive switch options
    virtual void ReceiveOptionParameter(char c, const char* value);         //Implement to receive options parameters
    virtual void ReceiveLongOption(const char* option, const char* value);  //Implement to receive long options
    virtual void ReceiveNonOption(const char* parameter);                   //Implement to receive non option command line strings

    //Interfaces to provide properties, determined from the options to the rest of the program
    std::string GetFileName();

private:

    std::string m_filename;

};

namespace qor{

    /*Declare that the OptionsApp uses a Singleton factory.
    This is done by overloading the type trait template
    instancer_of<T> such that instancer_of<OptionsApp>::type
    is SingletonInstancer. This must be done in the qor namespace
    because that's where the type trait instancer_of lives */
    qor_pp_declare_instancer_of(OptionsApp, SingletonInstancer);
    qor_pp_declare_sync_of(OptionsApp, RecursiveMutex);
}

#endif//QOR_PP_H_EXAMPLES_OPTIONS_APP