// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_SERVER
#define QOR_PP_H_EXAMPLES_SERVER

#include "src/framework/app/application/application.h"

#include <string>

class Server : public qor::Application
{
    qor_pp_declare_app_class(Server)

public:

    constexpr static const char* Name = "Server Example";    

    Server() = default;
    virtual ~Server() = default;
};

namespace qor{
    qor_pp_declare_instancer_of(Server, SingletonInstancer);
    qor_pp_declare_sync_of(Server,RecursiveMutex);
}

#endif//QOR_PP_H_EXAMPLES_OPTIONS_APP