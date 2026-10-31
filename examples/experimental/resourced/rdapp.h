// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_RESOURCED_APP
#define QOR_PP_H_EXAMPLES_RESOURCED_APP

#include <string>

#include "src/qor/memory/instance/singleton.h"
#include "src/framework/app/application/application.h"
#include "src/framework/parallel/sync/recursive_mutex.h"
#include "src/framework/app/optionparser/ioptionable.h"

class ResourcedApp : public qor::Application
{
    qor_pp_declare_app_class(ResourcedApp)

public:

    constexpr static const char* Name = "Resourced App";

    ResourcedApp() = default;
    virtual ~ResourcedApp() = default;

};

qor_pp_declare_singleton(ResourcedApp);

#endif//QOR_PP_H_EXAMPLES_RESOURCED_APP