// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "optionsapp.h"
#include "src/framework/event/delegate/bindmember.h"

/*Redirects the Appliation singleton factory to use the OptionsApp factory
This ensures all requests to get an Application instance will really get
the single OptionsApp instance
This makes it safe for exisitng QOR code to use AppBuilder().TheApplication() 
to access the OptionsApp instance without needing to know about the
OptionsApp type*/
qor_pp_redirect_app_class(OptionsApp)

using namespace qor;
using namespace qor::app;

const std::string OptionsApp::Description()
{
    return std::string("Command line options sample");
}

const std::vector<NamedArgSpec> OptionsApp::NamedArguments()
{    
    return std::vector<NamedArgSpec>{
        {
            "",                             //short name (will be generated from long name)
            "file",                         //long name
            1,                              //number of argument values (exact number or ArgCountZeroOrMore or ArgCountOneOrMore)
            ArgType::String,                //argument type (must match binding type exactly, String, Int, LongLong, souble, Bool)
            true,                           //parameter required or not
            "Please provide a file name.",  //description
            &m_filename                     //binding
        },
        {
            "o",
            "",
            1,
            ArgType::LongLong,
            false,
            "Optional order number.",
            &m_order
        }
    };
}

std::string OptionsApp::GetFileName()
{
    return m_filename;
}

long long OptionsApp::GetOrder()
{
    return m_order;
}
