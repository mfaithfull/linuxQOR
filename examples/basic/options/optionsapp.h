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
    virtual qor::app::Option* ProvideLongOptions();                              //Implement to tell the parser what long options we support
    virtual void ReceiveOptionSwitch(char c){}                              //Implement to receive switch options
    virtual void ReceiveOptionParameter(char c, const char* value);         //Implement to receive options parameters
    virtual void ReceiveLongOption(const char* option, const char* value);  //Implement to receive long options
    virtual void ReceiveNonOption(const char* parameter){}                  //Implement to receive non option command line strings

    //Interfaces to provide properties, determined from the options to the rest of the program
    std::string GetFileName();

private:

    std::string m_filename;

};

//Declare that the OptionsApp to be a Singleton, access protected with a RecursiveMutex
qor_pp_declare_singleton(OptionsApp);

#endif//QOR_PP_H_EXAMPLES_OPTIONS_APP