// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "optionsapp.h"

/*Redirects the Appliation singleton factory to use the OptionsApp factory
This ensures all requests to get an Application instance will really get
the single OptionsApp instance
This makes it safe for QOR code to use AppBuilder().TheApplication() to access
the OptionsApp instance without needing to know about OptionsApp iff
it is derived from Application*/
qor_pp_redirect_app_class(OptionsApp)

const char* OptionsApp::ProvideShortOptionString()
{
    return "f";
}

app::Option* OptionsApp::ProvideLongOptions()
{
    static app::Option longOptions[] =
    {
    //   NAME       ARGUMENT				                    FLAG	SHORTNAME
        {"file",    app::Option::required_argument,             nullptr, 'f'},
        {nullptr,   0,						                    nullptr, 0}
    };
    return longOptions;
}

void OptionsApp::ReceiveOptionParameter(char c, const char* value)
{
    switch (c)
    {
    case 'f':
        m_filename = value;
        break;
    }
}

void OptionsApp::ReceiveLongOption(const char* option, const char* value)
{
    if (strncmp(option, "file", 4) == 0)
    {
        m_filename = value;
    }
}

std::string OptionsApp::GetFileName()
{
    return m_filename;
}
