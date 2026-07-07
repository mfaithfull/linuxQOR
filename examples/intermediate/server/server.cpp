// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "server.h"

using namespace qor;

SingletonRedirector<Application, Server> Server::m_sRedirect;


const char* Server::ProvideShortOptionString()
{
    return "f";
}

app::Option* Server::ProvideLongOptions()
{
    static app::Option longOptions[] =
    {
    //   NAME       ARGUMENT				                    FLAG	SHORTNAME
        {"file",    app::Option::required_argument,       nullptr, 'f'},
        {nullptr,   0,						                    nullptr, 0}
    };
    return longOptions;
}

void Server::ReceiveOptionParameter(char c, const char* value)
{
    switch (c)
    {
    case 'f':
        m_filename = value;
        break;
    }
}

void Server::ReceiveLongOption(const char* option, const char* value)
{
    if (strncmp(option, "file", 4) == 0)
    {
        m_filename = value;
    }
}

std::string Server::GetFileName()
{
    return m_filename;
}
