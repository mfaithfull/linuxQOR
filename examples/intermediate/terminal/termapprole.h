// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_TERMAPP_ROLE
#define QOR_PP_H_EXAMPLES_TERMAPP_ROLE

#include "src/qor/flyers/error/defaulterrorhandler.h"
#include "src/qor/flyers/log/debug.h"
#include "src/qor/flyers/log/defaultloghandler.h"
#include "src/platform/platform.h"
#include "src/framework/app/application/builder.h"
#include "src/framework/app/role/role.h"
#include "src/components/qor/logaggregator/logaggregator.h"
#include "src/framework/io/filesystem/filesystem.h"
#include "src/framework/ui/terminal/terminal.h"

class TermAppRole : public qor::app::Role
{
public:

    constexpr static const char* logTag = "termapp";

    TermAppRole();
    virtual ~TermAppRole();
    virtual void Setup();
    virtual void Shutdown();

};

#endif//QOR_PP_H_EXAMPLES_TERMAPP_ROLE