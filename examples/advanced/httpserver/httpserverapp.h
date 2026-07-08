// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_HTTPSERVER_APP
#define QOR_PP_H_EXAMPLES_HTTPSERVER_APP

#include <string>
#include "src/qor/flyers/error/defaulterrorhandler.h"
#include "src/qor/flyers/log/defaultloghandler.h"
#include "src/framework/app/application/builder.h"
#include "src/framework/app/optionparser/getter.h"

class HTTPServerApp : public qor::Application
{
    qor_pp_declare_app_class(HTTPServerApp)                     //Required boiler plate for derived Application classes

public:

    constexpr static const char* Name = "QOR HTTP Server";      //Human readable application name
    constexpr static const char* logTag = "httpserver";         //Filesystem compatible name part for log files

    HTTPServerApp();
    virtual ~HTTPServerApp();

    //IOptionable interface for command line options
    virtual const char* ProvideShortOptionString();             //Implement to tell the parser what short options we support
    virtual qor::app::Option* ProvideLongOptions();             //Implement to tell the parser what long options we support
    virtual void ReceiveOptionSwitch(char /*c*/){}                  //Implement to receive switch options
    virtual void ReceiveOptionParameter(char c, const char* value);//Implement to receive options parameters
    virtual void ReceiveLongOption(const char* option, const char* value);  //Implement to receive long options
    virtual void ReceiveNonOption(const char* /*parameter*/){}      //Implement to receive non option command line strings

    //HTTPServerApp specific interface
    HTTPServerApp& UseAggregatedLogging();                      //Call to turn on Aggregated Logging. Requires Log aggregator component
    const qor::DefaultErrorHandler& GetErrorHandler() const;
    const qor::DefaultLogHandler& GetLogHandler() const;
    int Run();                                                  //App specific default Run function wraps running of workflow

private:

    void SetupAggregatedLogging() const;
    void ShutdownAggregatedLogging() const;

    bool m_useAggregatedLogging;
    qor::DefaultErrorHandler m_errorHandler;
    qor::DefaultLogHandler m_logHandler;

};

namespace qor{
    qor_pp_declare_instancer_of(HTTPServerApp, SingletonInstancer); //App classes should be Singletons
    qor_pp_declare_sync_of(HTTPServerApp, RecursiveMutex);          //Protected by a RecursiveMutex
}

#endif//QOR_PP_H_EXAMPLES_HTTPSERVER_APP