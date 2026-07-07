// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_SERVER
#define QOR_PP_H_EXAMPLES_SERVER

#include "src/framework/app/application/application.h"

#include <string>

class Server : public qor::Application
{
private:

    static qor::SingletonRedirector<qor::Application, Server> m_sRedirect;

public:

    constexpr static const char* Name = "Options App";

    Server() = default;
    virtual ~Server() = default;

    //IOptionable interface
    virtual const char* ProvideShortOptionString();                         //Implement to tell the parser what short options we support
    virtual qor::app::Option* ProvideLongOptions();                        //Implement to tell the parser what long options we support
    virtual void ReceiveOptionSwitch(char c){}                              //Implement to receive switch options
    virtual void ReceiveOptionParameter(char c, const char* value);         //Implement to receive options parameters
    virtual void ReceiveLongOption(const char* option, const char* value);  //Implement to receive long options
    virtual void ReceiveNonOption(const char* parameter){}                  //Implement to receive non option command line strings

    //Interfaces to provide properties, determined from the options to the rest of the program
    std::string GetFileName();

private:

    std::string m_filename;

};

namespace qor{qor_pp_declare_instancer_of(Server, SingletonInstancer);}

#endif//QOR_PP_H_EXAMPLES_OPTIONS_APP