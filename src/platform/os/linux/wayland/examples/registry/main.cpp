#include "src/configuration/configuration.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdk/using_framework.h"
#include "src/platform/os/linux/wayland/client/client.h"
#include "src/platform/os/linux/wayland/client/display.h"
#include "src/platform/os/linux/wayland/client/registry.h"

#include <wayland-client.h>

//struct wl_display* display = NULL;
struct wl_compositor* compositor = NULL;

static void global_registry_handler(void* data, struct wl_registry* registry, uint32_t id, const char* interface, uint32_t version)
{
    printf("Got a registry event for %s id %d\n", interface, id);
    if (strcmp(interface, "wl_compositor") == 0)
    {
        compositor = reinterpret_cast<wl_compositor*>(
            wl_registry_bind(registry, 
                id, 
                &wl_compositor_interface, 
                1
            )
        );
    }
}

static void global_registry_remover(void *data, struct wl_registry *registry, uint32_t id)
{
    printf("Got a registry losing event for %d\n", id);
}

static const struct wl_registry_listener registry_listener = 
{
    global_registry_handler,
    global_registry_remover
};

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
                auto display = waylandClient->GetDisplay("");                

                if (display.IsNull()) 
                {
                    fprintf(stderr, "Can't connect to display\n");
                    exit(1);
                }
                printf("connected to display\n");

                auto registry = display->GetRegistry();
                registry.AddListener(registry_listener, nullptr);
                                
                display->Dispatch();
                display->Roundtrip();

                if (compositor == NULL) 
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