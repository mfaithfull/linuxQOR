// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_HTTPSERVER_APP
#define QOR_PP_H_EXAMPLES_HTTPSERVER_APP

#include <string>
#include "src/qor/flyers/error/defaulterrorhandler.h"
#include "src/qor/flyers/log/defaultloghandler.h"
#include "src/framework/app/application/builder.h"

class HTTPServerApp : public qor::Application
{
    qor_pp_declare_app_class(HTTPServerApp)                     //Required boiler plate for derived Application classes

public:

    constexpr static const char* Name = "QOR HTTP Server";      //Human readable application name
    constexpr static const char* logTag = "httpserver";         //Filesystem compatible name part for log files

    HTTPServerApp();
    virtual ~HTTPServerApp();

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

qor_pp_declare_singleton(HTTPServerApp);

#endif//QOR_PP_H_EXAMPLES_HTTPSERVER_APP