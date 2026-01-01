#include "src/configuration/configuration.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdk/using_framework.h"
#include "src/platform/os/linux/wayland/client/client.h"
#include "src/platform/os/linux/wayland/client/display.h"
#include "src/platform/os/linux/wayland/client/registry.h"
#include "src/platform/os/linux/wayland/client/session.h"
#include "src/platform/os/linux/wayland/client/compositor.h"

const char* appName = "registry";

qor_pp_implement_module(appName)
qor_pp_module_requires(ICurrentThread)
qor_pp_module_requires(WaylandClient)

int main(int argc, char **argv) 
{    
    auto registryApp = AppBuilder().Build(appName);

    registryApp->SetRole<Role>(
        [](ref_of<IRole>::type role)
        {
            qor_pp_fcontext;
            
            role->AddFeature<qor::platform::nslinux::WaylandClient>();
        }
    );

    return registryApp(qor_shared).Run
    (
        make_runable(

            []()->int
            {
                auto waylandClient = AppBuilder().TheApplication(qor_shared)->GetRole(qor_shared)->GetFeature<qor::platform::nslinux::WaylandClient>();
                auto display = waylandClient(qor_shared).GetDisplay("");
                
                if (display.IsNull()) 
                {
                    fprintf(stderr, "Can't connect to display\n");
                    exit(1);
                }
                printf("connected to display\n");

                auto session = new_ref<qor::platform::nslinux::wl::Session>();
                auto registry = display->GetRegistry();
                registry->AddDefaultListener(session);
                display->Dispatch();
                display->Roundtrip();

                if (session->GetCompositor().IsNull()) 
                {
                    fprintf(stderr, "Can't find compositor\n");
                    exit(1);
                } 
                else 
                {
                    fprintf(stderr, "Found compositor\n");
                }

                display.Dispose();
                printf("disconnected from display\n");

                return EXIT_SUCCESS;
            }
        )
    );
}