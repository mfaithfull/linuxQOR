// XVideo test program by Jan Wedekind (jan at wedesoft.de).
//
// Based on
// * http://bellet.info/XVideo/testxv.c
// * http://svn.mplayerhq.hu/mplayer/trunk/libvo/vo_xv.c?view=markup
// * http://svn.mplayerhq.hu/mplayer/trunk/libvo/x11_common.c?view=markup
//

#include "src/configuration/configuration.h"

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include "sdk/using_framework.h"
#include "src/platform/os/linux/x/xlib/xlib.h"
#include "src/platform/os/linux/x/xlib/display.h"
#include "src/platform/os/linux/x/xlib/screen.h"
#include "src/platform/os/linux/x/xlib/window.h"
#include "src/platform/os/linux/x/xlib/font.h"
#include "src/platform/os/linux/x/xlib/colourmap.h"
#include "src/platform/os/linux/x/xlib/event.h"
#include "src/platform/os/linux/x/xvideo/video.h"
#include "src/platform/os/linux/x/xvideo/adaptorinfo.h"

using namespace std;
using namespace qor::platform::nslinux::x;

#define GUID_YUV12_PLANAR 0x32315659
#define GUID_UYVY_PLANAR 0x59565955

#define USE_BGFILL

int waitForNotify( void*, Event* e, char* arg )
{
    return ( e->type == MapNotifyType ) && ( e->xmap.window == *((unsigned long*)arg) );
}

static unsigned long xv_intern_atom_if_exists( Video& videoSupport, Display* display, unsigned long port, char const *atom_name )
{
    int i;
    unsigned long xv_atom = 0;

    std::vector<Attribute> attributes = videoSupport.QueryPortAttributes(port);
    for( auto i = attributes.begin(); i < attributes.end(); ++i )
    {
        if ( strcmp(i->name.c_str(), atom_name ) == 0 )
        {
            xv_atom = display->CreateAtom(atom_name);
            break; // found what we want, break out
        }
    }
    return xv_atom;
}

const char* appName = "TestXV";

qor_pp_implement_module(appName)
qor_pp_module_requires(ICurrentThread)
qor_pp_module_requires(XClient)

int main( int argc, char *argv[] )
{
    auto testxvApp = AppBuilder().Build(appName);

    testxvApp->SetRole<Role>(
        [](ref_of<IRole>::type role)
        {
            qor_pp_fcontext;
            
            role->AddFeature<qor::platform::nslinux::XClient>();
        }
    );

    return testxvApp(qor_shared).Run
    (
        make_runable(

            []()->int
            {
                const int width = 1024;
                const int height = 768;
                auto xclient = AppBuilder().TheApplication(qor_shared)->GetRole(qor_shared)->GetFeature<qor::platform::nslinux::XClient>();
                auto defaultDisplay = xclient->GetDisplay(0);
                auto videoSupport = Video(defaultDisplay);
                unsigned int ver, rel, req, ev, err;
                videoSupport.QueryExtension(ver, rel, req, ev, err);
                
                AdaptorInfo adaptors = videoSupport.QueryAdaptors(defaultDisplay->DefaultRootWindow());

                unsigned long port = 0;
                for(int i = 0; i < adaptors.Size(); i++)
                {
                    if((adaptors.GetInfo(i)->type & ( XvInputMask | XvImageMask )) ==  ( XvInputMask | XvImageMask ) )
                    {
                        for ( int p = adaptors.GetInfo(i)->base_id;  p< adaptors.GetInfo(i)->base_id + adaptors.GetInfo(i)->num_ports; p++ )
                        {
                            if(!videoSupport.GrabPort(p, 0))
                            {
                                port = p;
                                break;
                            };
                        }
                        if ( port != 0 )
                        break;
                    }
                }

                std::cerr << "Xv port is " << port << endl;

                int colourkey = 0;
                unsigned long xvColorKey = xv_intern_atom_if_exists( videoSupport, defaultDisplay, port, "XV_COLORKEY");
                if ( xvColorKey != 0 ) 
                {
                    std::cerr << "Require drawing of colorkey" << endl;
                    if ( videoSupport.GetPortAttribute( port, xvColorKey, colourkey ) != 0 )
                    {
                        return 1;
                    }

                    unsigned long xvAutoPaint = xv_intern_atom_if_exists( videoSupport, defaultDisplay, port, "XV_AUTOPAINT_COLORKEY");
                    if ( xvAutoPaint != 0 ) 
                    {
                        std::cerr << "Enabling autopainting" << endl;
                        videoSupport.SetPortAttribute( port, xvAutoPaint, 1 );
                        xvColorKey = 0;
                    };                    
                }
                else 
                {
                    std::cerr << "No drawing of colourkey required" << endl;
                }

                std::vector<ImageFormat> fo = videoSupport.ListImageFormats(port);
                unsigned int format = 0;
                for( ImageFormat f : fo) 
                {
                    std::cerr << "found format " << (char *)&f.id << " GUID 0x" << setbase( 16 ) << f.id << setbase( 10 );

                    if ( f.id == GUID_UYVY_PLANAR )
                    {
                        format = f.id;
                        std::cerr << " (to be used)";
                    };

                    std::cerr << endl;
                }

                int depth;
                {
                    WindowAttributes attribs;
                    defaultDisplay->DefaultRootWindow().GetWindowAttributes(attribs);                    
                    depth = attribs.depth;
                    if (depth != 15 && depth != 16 && depth != 24 && depth != 32) depth = 24;
                }

                VisualInfo visualInfo;
                defaultDisplay->ScreenOfDisplay(defaultDisplay->DefaultScreen()).MatchVisualInfo(depth, TrueColor, visualInfo);                

                SetWindowAttributes swa;
                Colourmap colourMap(defaultDisplay, defaultDisplay->DefaultRootWindow().GetId(), (::Visual*)(visualInfo.visual), 0);
                swa.colormap = colourMap.GetId();
                swa.border_pixel = 0;
                swa.background_pixel = colourkey;
                swa.event_mask = ExposureMask | StructureNotifyMask | KeyPressMask;
                unsigned long mask = CWBorderPixel | CWColormap | CWEventMask;
                if ( xvColorKey != None )
                { 
                    mask |= CWBackPixel;
                }

                Window window(defaultDisplay, 
                    defaultDisplay->RootWindow(visualInfo.screen).GetId(), 
                    0, 
                    0, 
                    width, 
                    height, 
                    0, 
                    visualInfo.depth, 
                    InputOutput, 
                    (Visual*)(visualInfo.visual), 
                    mask, 
                    swa);

                Event event;
                window.Map();
                unsigned long windowId = window.GetId();

                defaultDisplay->IfEvent(event, waitForNotify, &windowId);

                GCValues gcv;
                auto gc = window.CreateGC(0, gcv);

                if(xvColorKey != None)
                {
                    gc.SetForeground(colourkey);
                    gc.FillRectangle(window.GetId(), 0, 0, width, height);
                    std::cerr << "Filled " << width << 'x' << height << "-rectangle with colour " << colourkey << '.' << std::endl;
                }

                auto image = videoSupport.CreateImage(port, format, nullptr, width, height);
                image->data = new char(image->data_size);
                std::cerr << "Filling image of size " << image->data_size << std::endl;

                for(int y = 0; y < height; y++)
                {
                    for(int x = 0; x < width; x++ )
                    {
                        ((unsigned short int *)image->data)[ y * width + x ] = y + x;
                    }
                }
               
                bool quit = false;
                int c = 0;
                do 
                {
                    Event event;                    
                    if(defaultDisplay->CheckMaskEvent(KeyPressMask | ExposureMask | StructureNotifyMask, event)) 
                    {
                        switch ( event.type ) 
                        {
                        case ConfigureNotifyType:
                        case ExposeType:
                            std::cerr << "Repainting" << std::endl;

                            gc.SetForeground(colourkey);
                            gc.FillRectangle(window.GetId(), 0, 0, width, height);
                            videoSupport.PutImage( port, window.GetId(), gc, image, 0, 0, width, height, 0, 0, width, height );
                            // Alternatively use XvShmPutImage.
                        case KeyPressType:
                            std::cerr << "Key was pressed" << std::endl;
                            if ( event.xkey.keycode == 0x09 )
                            {
                                quit = true;
                            }
                            break;
                        default:
                            break;
                        };
                    } 
                    else 
                    {
                        videoSupport.PutImage( port, window.GetId(), gc, image, 0, 0, width, height, 0, 0, width, height );
                    };
                    c++;

                } while ( !quit );

                std::cerr << "# frames = " << c << endl;
                delete( image->data );                
                xclient->Free( image );

                videoSupport.UngrabPort( port, 0 );
                return EXIT_SUCCESS;
            }
        )
    );
}