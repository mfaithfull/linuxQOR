// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_EXAMPLES_ROLLER_APP
#define QOR_PP_H_EXAMPLES_ROLLER_APP

#include <string>

#include "src/framework/app/application/application.h"

/*To customise the Application class we derive from it.
this is optional. The built in Application class is
usually sufficient*/
class App : public qor::Application
{
private:

    qor_pp_declare_app_class(App)

public:

    constexpr static const char* Name = "Role based App";

    App() = default;
    virtual ~App() = default;

    void CustomConfigure()
    {
    }

};

namespace qor
{
    /*Declare that the App uses a Singleton factory.
    This is done by overloading the type trait template
    instancer_of<T> such that instancer_of<App>::type
    is SingletonInstancer. This must be done in the qor namespace
    because that's where the type trait instancer_of lives */
    qor_pp_declare_instancer_of(App, SingletonInstancer);
    qor_pp_declare_sync_of(App, RecursiveMutex);
}

#endif//QOR_PP_H_EXAMPLES_ROLLER_APP