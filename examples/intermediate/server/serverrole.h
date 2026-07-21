// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_SERVER_ROLE
#define QOR_PP_H_EXAMPLES_SERVER_ROLE

#include "src/qor/flyers/error/defaulterrorhandler.h"
#include "src/qor/flyers/log/debug.h"
#include "src/qor/flyers/log/defaultloghandler.h"
#include "src/platform/platform.h"
#include "src/framework/app/application/builder.h"
#include "src/framework/app/role/role.h"
#include "src/framework/io/async/service.h"
#include "src/components/qor/logaggregator/logaggregator.h"
#include "src/framework/io/filesystem/filesystem.h"

class ServerRole : public qor::app::Role
{
public:

    constexpr static const char* logTag = "server";

    ServerRole();
    virtual ~ServerRole();
    virtual void Setup();
    virtual void Shutdown();

};

#endif//QOR_PP_H_EXAMPLES_SERVER_ROLE