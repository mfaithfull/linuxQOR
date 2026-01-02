#include "src/configuration/configuration.h"

#include <sys/mman.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdk/using_framework.h"
#include "src/platform/os/linux/wayland/client/client.h"
#include "src/platform/os/linux/wayland/client/display.h"
#include "src/platform/os/linux/wayland/client/registry.h"
#include "src/platform/os/linux/wayland/client/session.h"
#include "src/platform/os/linux/wayland/client/compositor.h"
#include "src/platform/os/linux/wayland/client/shell.h"
#include "src/platform/os/linux/wayland/client/shellsurface.h"
#include "src/platform/os/linux/wayland/client/shm.h"
#include "src/platform/os/linux/wayland/client/shmpool.h"
#include "src/platform/os/linux/wayland/client/surface.h"
#include "src/platform/os/linux/wayland/client/buffer.h"
#include "src/platform/os/linux/wayland/client/listeners/bufferlistener.h"
#include "src/platform/os/linux/wayland/xdgshell/xdgwmbase.h"
#include "src/platform/os/linux/wayland/xdgshell/xdgsurface.h"
#include "src/platform/os/linux/wayland/xdgshell/xdgtoplevel.h"
#include "src/platform/os/linux/wayland/xdgshell/listeners/xdgtoplevellistener.h"
#include "src/platform/os/linux/wayland/xdgshell/listeners/xdgsurfacelistener.h"
#include "src/platform/os/linux/wayland/xdgshell/listeners/xdgwmbaselistener.h"


const char* appName = "registry";

qor_pp_implement_module(appName)
qor_pp_module_requires(ICurrentThread)
qor_pp_module_requires(WaylandClient)

class customSession : public qor::platform::nslinux::wl::Session
{
public:
    
    customSession()
    {
        pixels = nullptr;
        width = 640;
        height = 480;
    }

    virtual ~customSession()
    {
    }

    void DrawFrame()
    {
        int stride = width * 4;
        int size = stride * height;  // bytes

        // open an anonymous file and write some zero bytes to it
        int fd = syscall(SYS_memfd_create, "buffer", 0);
        ftruncate(fd, size);

        // map it to the memory
        pixels = reinterpret_cast<uint8_t*>(mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

        // turn it into a shared memory pool
        auto shmPool = shm->CreatePool(fd, size);

        // allocate the buffer in that pool
        buffer = shmPool->CreateBuffer(0, width, height, stride, qor::platform::nslinux::wl::WL_SHM_FORMAT_XRGB8888);

        // Draw background
        uint32_t *pixel_ptr = (uint32_t *) pixels;
        size_t total_pixels = width * height;

        for (size_t i = 0; i < total_pixels; i++)
        {
            pixel_ptr[i] = 0xCC0A0000;
        }
        
        //Attach a listener to the Buffer to free it when the compositor is done with it.
        buffer->AddListener(buffer_listener, NULL);
            
        //Get the pixels out of the door by:        
        surface->Attach(buffer, 0, 0);          //attaching them to the surface
        surface->Damage( 0, 0, width, height);  //marking the whole surface as needing redrawing
        surface->Commit();                      //commiting the surface

        shmPool.Dispose();                      //clean up the pool.
        close(fd);                              //clean up the file handle.
    }

    virtual void OnXDGSurfaceConfigured()
    {
        if(!pixels)
        {
            DrawFrame();
        }
    }

    virtual void OnXDGTopLevelConfigured(int32_t new_width, int32_t new_height, struct wl_array* states)
    {
        if (new_width == 0 || new_height == 0)
        {
            return;
        }

        if (width != new_width || height != new_height)
        {
            //Free the mapped memory before DrawFrame reallocates it at the new size.
            munmap(pixels, width * height * 4);
            width = new_width;
            height = new_height;

            DrawFrame();
        }
    }

    int width;
    int height;
    uint8_t* pixels;
    qor::platform::nslinux::wl::BufferListener buffer_listener;
    qor::ref_of<qor::platform::nslinux::wl::Buffer>::type buffer;
    qor::ref_of<qor::platform::nslinux::wl::Surface>::type surface;
    qor::ref_of<qor::platform::nslinux::wl::SharedMemory>::type shm;
};

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
                //Get a reference to the Wayland Client feature
                auto waylandClient = AppBuilder().TheApplication(qor_shared)->GetRole(qor_shared)->GetFeature<qor::platform::nslinux::WaylandClient>();
                //Get the default local display
                auto display = waylandClient(qor_shared).GetDisplay("");
                //Make a custom Session to own this thread's wayland related state
                auto session = new_ref<customSession>();
                //Get the global registry object
                auto registry = display->GetRegistry();
                //Attach the registry to our session with a default listener so it tells the Session about the available protocols
                registry->AddDefaultListener(session);
                //Make the internals do their thing until all pending events have been processed
                display->Dispatch();
                display->Roundtrip();

                {
                    //The session will now know about the compositor so extact it
                    auto compositor = session->GetCompositor();
                    //The default session caches info for globals it doesn't know about.
                    //Get the cached info for the XDG Shell Extension
                    auto xdgWmBaseInfo = session->GetGlobal("xdg_wm_base");                    
                    //Make a Surface
                    session->surface = compositor->CreateSurface();
                    //Get the global shared memory manager from the Session
                    session->shm = session->GetShm();
                    //Use the XDG Shell Extension info returned earlier to bind the extension object to it's registry entry.
                    auto xdgWmBase = new_ref<qor::platform::nslinux::wl::XDGWMBase>(registry, xdgWmBaseInfo->Name, xdgWmBaseInfo->Version);
                    //Add a listener to handle incoming from the XDG Shell Extension
                    qor::platform::nslinux::wl::XDGWMBaseListener xdgWmBaseListener;
                    xdgWmBase->AddListener(xdgWmBaseListener, nullptr);
                    //Get an XDG Surface adapter for the ordinary surface we already created
                    auto xdgSurface = xdgWmBase->GetXDGSurface(session->surface(qor_shared));
                    //Add a listener to handle events for the XDGSurface 
                    qor::platform::nslinux::wl::XDGSurfaceListener xdgSurfaceListener;
                    xdgSurface->AddListener(xdgSurfaceListener, xdgSurface.operator->());
                    //Attach the XDGSurface to the session. It will then forward events to the Session
                    xdgSurface->SetSession(session);
                    //Get a TopLevel Adaptor for the XDGSurface to make it an 'ordinary' window (not a popup)
                    auto xdgTopLevel = xdgSurface->GetToplevel();
                    //Set window type properties on the TopLevel Adaptor
                    xdgTopLevel->SetTitle("Wayland XDG Example");
                    //Commit the Surface to trigger configuration events from the server
                    session->surface->Commit();
                    //Process those events
                    display->Dispatch();
                    display->Roundtrip();

                    //At this point the window is rendering and ready for interaction.
                    while(display->Dispatch())
                    {
                        //Event loop
                    }
                }            

                registry.Dispose();
                session.Dispose();
                display.Dispose();
                printf("disconnected from display\n");

                return EXIT_SUCCESS;
            }
        )
    );
}