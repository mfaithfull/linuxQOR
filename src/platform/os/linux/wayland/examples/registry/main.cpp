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
#include "src/platform/os/linux/wayland/client/seat.h"
#include "src/platform/os/linux/wayland/client/keyboard.h"
#include "src/platform/os/linux/wayland/client/listeners/bufferlistener.h"
#include "src/platform/os/linux/wayland/client/listeners/keyboardlistener.h"
#include "src/platform/os/linux/wayland/client/controllers/keyboardcontroller.h"
#include "src/platform/os/linux/wayland/xdgshell/xdgwmbase.h"
#include "src/platform/os/linux/wayland/xdgshell/xdgsurface.h"
#include "src/platform/os/linux/wayland/xdgshell/xdgtoplevel.h"
#include "src/platform/os/linux/wayland/xdgshell/listeners/xdgtoplevellistener.h"
#include "src/platform/os/linux/wayland/xdgshell/listeners/xdgsurfacelistener.h"
#include "src/platform/os/linux/wayland/xdgshell/listeners/xdgwmbaselistener.h"
#include "src/platform/os/linux/wayland/xdgshell/xdgsession.h"

using namespace qor::platform::nslinux;

const char* appName = "registry";

qor_pp_implement_module(appName)
qor_pp_module_requires(ICurrentThread)
qor_pp_module_requires(WaylandClient)

class customSession : public wl::XDGSession
{
public:
    
    customSession(ref_of<wl::Display>::type display) : wl::XDGSession(display)
    {
        m_end = false;
        pixels = nullptr;
        SetWidth(640);
        SetHeight(480);

        auto seats = GetSeats();
        if(seats.size() > 0)
        {
            for(auto seatindex : seats)
            {
                auto seat = GetSeat(seatindex);
                if(seat.IsNotNull())
                {
                    m_keyboard = seat->GetKeyboard();
                    m_keyboard->AddListener(keyboard_listener, m_keyboard.operator->());
                    break;
                }
            }
        }
    }

    virtual ~customSession()
    {
    }

    void DrawFrame()
    {
        int stride = m_width * 4;
        int size = stride * m_height;  // bytes

        // open an anonymous file and write some zero bytes to it
        int fd = syscall(SYS_memfd_create, "buffer", 0);
        ftruncate(fd, size);

        // map it to the memory
        pixels = reinterpret_cast<uint8_t*>(mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

        // turn it into a shared memory pool
        auto shmPool = m_Shm->CreatePool(fd, size);

        // allocate the buffer in that pool
        buffer = shmPool->CreateBuffer(0, m_width, m_height, stride, wl::WL_SHM_FORMAT_XRGB8888);

        // Draw background
        uint32_t *pixel_ptr = (uint32_t *) pixels;
        size_t total_pixels = m_width * m_height;

        for (size_t i = 0; i < total_pixels; i++)
        {
            pixel_ptr[i] = 0xCC0A0B0C;
        }
        
        //Attach a listener to the Buffer to free it when the compositor is done with it.
        buffer->AddListener(buffer_listener, NULL);
            
        //Get the pixels out of the door by:        
        m_xdgSurface->BaseSurface()->Attach(buffer, 0, 0);          //attaching them to the surface
        m_xdgSurface->BaseSurface()->Damage( 0, 0, m_width, m_height);  //marking the whole surface as needing redrawing
        m_xdgSurface->BaseSurface()->Commit();                      //commiting the surface

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

        if (m_width != new_width || m_height != new_height)
        {
            //Free the mapped memory before DrawFrame reallocates it at the new size.
            munmap(pixels, m_width * m_height * 4);
            m_width = new_width;
            m_height = new_height;

            DrawFrame();
        }
    }

    void End()
    {
        m_end = true;
    }

    int Run()
    {
        while(m_Display->Dispatch() && !m_end)
        {        
        }
        return 0;
    }

    bool m_end;
    uint8_t* pixels;
    wl::BufferListener buffer_listener;
    wl::KeyboardListener keyboard_listener;
    ref_of<wl::Buffer>::type buffer;   
    ref_of<wl::Keyboard>::type m_keyboard; 
};

class customKeyboardController : public wl::KeyboardController
{
    public:

    customKeyboardController(customSession* session, ref_of<wl::Keyboard>::type keyboard) : wl::KeyboardController(keyboard), m_session(session)
    {}

    virtual ~customKeyboardController()
    {}

    virtual void OnKey(wl::Keyboard*, uint32_t serial, uint32_t time, uint32_t key, uint32_t state)
    {
        if(key == 1 && state == 1)
        {
            m_session->End();
        }
    }

    protected:

    customSession* m_session;    
};

int main(int argc, char **argv) 
{    
    auto registryApp = AppBuilder().Build(appName);

    registryApp->SetRole<Role>(
        [](ref_of<IRole>::type role)
        {
            role->AddFeature<WaylandClient>();
        }
    );

    return registryApp(qor_shared).Run
    (
        make_runable(

            []()->int
            {
                {//Inner scope ensures the wayland session is cleaned up neatly before the application quits

                    //Get a reference to the Wayland Client feature
                    auto waylandClient = AppBuilder().TheApplication(qor_shared)->GetRole(qor_shared)->GetFeature<WaylandClient>();
                    auto display = waylandClient(qor_shared).GetDisplay("");//Get the default local display
                    auto session = new_ref<customSession>(display);//Make a custom Session to own this thread's wayland related state

                    {   //Inner scope ensures all our local session dependent objects go away before the session cleans up

                        auto topLevelWindow = session->GetXDGTopLevelWindow();
                        topLevelWindow->SetTitle("Wayland XDG Example");//Set window type properties on the TopLevel Adaptor

                        customKeyboardController keycon(session, session->m_keyboard);
                        session->Run();
                    }            

                }   
                printf("disconnected from display\n");

                return EXIT_SUCCESS;
            }
        )
    );
}