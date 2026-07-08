// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "builder.h"

namespace qor{

    AppBuilder::AppBuilder() { }
    AppBuilder::~AppBuilder() noexcept { }

    ref_of<Application>::type AppBuilder::Build(const std::string& name)
    {
        auto application = new_ref<Application>();
        application->Name() = name;
        return application;
    }

    ref_of<Application>::type AppBuilder::TheApplication()
    {
        auto application = new_ref<Application>();
        return application;
    }

    void AppBuilder::AutoRedirect(ref_of<Application>::type application)
    {
        SingletonInstancer::AutoRedirect<Application>(application);
    }
    
}//qor
