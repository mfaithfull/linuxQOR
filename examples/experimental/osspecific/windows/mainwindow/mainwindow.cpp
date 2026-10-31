// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/tdd/assert/assert.h"
#include "src/platform/os/windows/common/structures.h"
#include "src/platform/os/windows/app/exebootstrap/winqorexeboot.h"
#include "src/platform/os/windows/ui/gui/factories/windowfactory.h"
#include "src/platform/os/windows/ui/gui/view/handlers/messagehandler.h"
#include "src/platform/os/windows/ui/gui/view/handlers/toplevel.h"
#include "src/platform/os/windows/ui/gui/painting/themedata.h"
#include "src/platform/os/windows/ui/gui/painting/paintsession.h"
#include "src/platform/os/windows/ui/gui/view/messages.h"

using namespace qor;
using namespace qor::win;
using namespace qor::platform::win;
using namespace qor::platform::win::gui::view;
using namespace std;

class CustomNonClientHandler : public NonClientHandler
{
public:

    virtual bool OnNCActivate(Window& window, bool active, bool repaint, void* prevWindow)
    {
        return false;
    }
};

class CustomNonClientRenderingHandler : public NonClientRenderingHandler
{
public:

    void GetRectCenter(Rect* prc, Point *ppt)
    {
        ppt->m_x = prc->m_left + (prc->m_right - prc->m_left)/2;
        ppt->m_y = prc->m_top + (prc->m_bottom - prc->m_top)/2;
    }

    void CustomClientAreaAdjustment(Rect* proposedClientArea)
    {
        proposedClientArea->m_left += 0;   // Left frame width
        proposedClientArea->m_right -= 0;  // Right frame width
        proposedClientArea->m_top += 0;    // Title bar height//themeData->GetSysSize(SMCYSIZE)
        proposedClientArea->m_bottom -= 0; // Bottom frame width
    }

    virtual long long OnNCCalcSize(Window& window, bool indicateValidClientArea, Rect* proposedWindowRect, NCCalcSizeParams* ncCalcSizeParams)
    {
        if(indicateValidClientArea)
        {
            Rect* prcClientNew = &ncCalcSizeParams->rgrc[0];
            CustomClientAreaAdjustment(prcClientNew);
            Rect* prcValidDst  = &ncCalcSizeParams->rgrc[1];
            Rect* prcValidSrc  = &ncCalcSizeParams->rgrc[2];
            // Compute the old and new center points
            Point ptOldCenter, ptNewCenter;
            GetRectCenter(prcValidSrc, &ptOldCenter);
            GetRectCenter(prcClientNew, &ptNewCenter);
            //  Tell USER how the old and new client rectangles match up
            *prcValidDst = *prcClientNew; // use entire client area
            prcValidDst->m_left += ptNewCenter.m_x - ptOldCenter.m_x;
            prcValidDst->m_top += ptNewCenter.m_y - ptOldCenter.m_y;
            return 0x0400;
        }
        CustomClientAreaAdjustment(proposedWindowRect);
        return 0;
    }

    virtual bool OnNCPaint(Window& window, unsigned long long wParam)
    {
        WindowController* controller = window.GetController();
        if(controller)
        {
            ref_of<DeviceContext>::type dc;
            if(wParam != 1)
            {
                ref_of<Region>::type updateRegion = new_ref<Region>(PrimitiveHandle((void*)(wParam)), false);
                dc = controller->GetADC(updateRegion, DCXWindow|DCXIntersectRegion);
            }
            else
            {
                dc = controller->GetDC();
            }
            if(dc)
            {
                //Do nothing as we don't really have a non client area anymore
            }
        }
        return true;
    }

    virtual long long OnNCHitTest(Window& window, int x, int y, unsigned long long wParam, long long lParam)
    {
        //x & y are in screen coordinates
        //Interrogate layout to determine the Regions for the various elements
        //Each layout element should have a hit test value and we return the one of the first element hit
        WindowController* controller = window.GetController();
        if(controller)
        {
            Rect r;
            controller->GetRect(r);//get screen coordinate rectangle of window
            if(!r.IsPointInRect(Point(x,y)))
            {
                return HTNowhere;
            }
            if( y < r.m_top + 31)
            {   // in caption area
                if( y < r.m_top + 5)
                {
                    return HTTop;
                }
                if( x < r.m_left + 5 )
                {
                    return HTTopLeft;
                }
                else if( x > (r.m_right - 5))
                {
                    return HTTopRight;
                }
            }
            else if( y > (r.m_bottom - 5))
            {
                return HTBottom;
            }
            else
            {
                if( x < r.m_left + 5 )
                {
                    return HTLeft;
                }
                else if( x > (r.m_right - 5))
                {
                    return HTRight;
                }
            }
        }
        return HTCaption;
    }
};

class CustomRenderingHandler : public RenderingHandler
{
public:

    virtual bool OnEraseBackground(Window& window, DeviceContext& dc)
    {
        return true;
    }

    virtual void OnSize(Window& window, unsigned long long wParam, long long lParam)
    {
        m_width = LoWord(lParam); m_height = HiWord(lParam);

        auto guiThread = new_ref<PerThread>();
        guiThread->ClearCurrentWGLContext();
        m_context.Dispose();
        CreateDIB(m_width, m_height);
        CreateRenderingContext();
        guiThread->SetCurrentWGLContext(m_dcDIB, m_context);
        InitScene();
        ResizeScene(m_width, m_height);
        RenderScene();
    }

    virtual bool OnPaint(Window& window)
    {
        WindowController* controller = window.GetController();
        if(controller)
        {
            PaintSession ps(&window);
            ref_of<DeviceContext>::type dc = ps.GetDC();
            Rect rc;
            RenderScene(); // OpenGL -> DIB
            Blit(dc()());  // DIB -> hDC
            controller->GetClientRect(rc);
            Size size;
            dc->GetTextExtentPoint32T(L"Hello, Windows!", 15, &size);
            dc->TextOutT((rc.Width()/2)-(size.m_x/2), (rc.Height()/2) - (size.m_y/2), L"Hello, Windows!", 15);
        }
        return true;
    }

private:

    int m_width{240};
    int m_height{240};
    ref_of<DeviceContext>::type m_dcDIB;
    int m_cxDIB{0};
    int m_cyDIB{0};
    BitmapInfoHeader m_BIH;
    ref_of<Bitmap>::type m_bmpDIB;
    void* m_bmp{nullptr};
    ref_of<WGLContext>::type m_context;

    bool InitScene()
    {
        auto guiThread = new_ref<PerThread>();
        auto& gl = guiThread->OGLSession();
        gl.Enable(OpenGLSession::ALPHA_TEST);
        gl.Enable(OpenGLSession::DEPTH_TEST);
        gl.Enable(OpenGLSession::COLOR_MATERIAL);
        gl.Enable(OpenGLSession::LIGHTING);
        gl.Enable(OpenGLSession::LIGHT0);
        gl.Enable(OpenGLSession::BLEND);
        gl.BlendFunc(OpenGLSession::SRC_ALPHA, OpenGLSession::ONE_MINUS_SRC_ALPHA);
        gl.ClearColor(0, 0, 0, 0);
        return 0;
    }

    void ResizeScene(int width,int height)
    {
        auto guiThread = new_ref<PerThread>();
        auto& gl = guiThread->OGLSession();
        gl.Viewport(0,0,width,height);
        gl.MatrixMode(OpenGLSession::PROJECTION);
        gl.LoadIdentity();
        gl.MatrixMode(OpenGLSession::MODELVIEW);
        gl.LoadIdentity();
    }

    void RenderScene()
    {
        auto guiThread = new_ref<PerThread>();
        auto& gl = guiThread->OGLSession();
        gl.Clear(OpenGLSession::COLOR_BUFFER_BIT | OpenGLSession::DEPTH_BUFFER_BIT );
        gl.PushMatrix();
        gl.Color3f(0, 1, 1);
        gl.Begin(OpenGLSession::TRIANGLES);                  // Drawing Using Triangles
            gl.Color3f(1.0f,0.0f,0.0f);                      // Set The Color To Red
            gl.Vertex3f( 0.0f, 1.0f, 0.0f);                  // Top
            gl.Color3f(0.0f,1.0f,0.0f);                      // Set The Color To Green
            gl.Vertex3f(-1.0f,-1.0f, 0.0f);                  // Bottom Left
            gl.Color3f(0.0f,0.0f,1.0f);                      // Set The Color To Blue
            gl.Vertex3f( 1.0f,-1.0f, 0.0f);                  // Bottom Right
        gl.End();
        gl.PopMatrix();
        gl.Flush();
    }

    // DIB -> hDC
    void Blit(DeviceContext& dest)
    {
        if(m_dcDIB.IsNotNull())
        {
            dest.BitBlt(0, 0, m_width, m_height, m_dcDIB()(), 0, 0, SrcCopy);
        }
    }

    void CreateDIB(int cx, int cy)
    {
        qor_pp_assert_that(cx > 0);
        qor_pp_assert_that(cy > 0);

        m_cxDIB = cx ;
        m_cyDIB = cy ;

        m_BIH.biWidth = cx;
        m_BIH.biHeight = cy;
        m_BIH.biPlanes = 1;
        m_BIH.biBitCount = 24;
        m_BIH.biCompression = BIRGB;

        if(m_dcDIB.IsNotNull())
        {
            m_dcDIB.Dispose();
        }

        m_dcDIB = DeviceContext::CreateForDIB();

        qor_pp_assert_that(m_dcDIB.IsNotNull());

        if(m_bmpDIB.IsNotNull())
        {
            m_bmpDIB.Dispose();
        }

        m_bmpDIB = new_ref<DIBSection>(m_dcDIB, (BitmapInfo*)(&m_BIH), DIBRGBCOLORS, &m_bmp, nullptr, 0);

        qor_pp_assert_that(m_bmpDIB.IsNotNull());
        qor_pp_assert_that(m_bmp != nullptr);

        if(m_bmpDIB.IsNotNull())
        {
            m_dcDIB->SelectObject(m_bmpDIB->GetHandle());
        }
    }

    bool CreateRenderingContext()
    {
        PixelFormatDescriptor pfd;
        unsigned long dwFlags = PFDSUPPORTOPENGL | PFDDRAWTOBITMAP;
        pfd.nVersion = 1;
        pfd.dwFlags = dwFlags;
        pfd.iPixelType = PFDTYPERGBA;
        pfd.cColorBits = 24;
        pfd.cDepthBits = 32;
        pfd.iLayerType = PFDMAINPLANE;

        int PixelFormat = m_dcDIB->ChoosePixelFormat(&pfd);
        if (PixelFormat == 0)
        {
            return false;
        }

        bool bResult = m_dcDIB->SetPixelFormat(PixelFormat, &pfd);
        if (bResult==false)
        {
            return false;
        }

        m_context = new_ref<WGLContext>(m_dcDIB);
        return m_context.IsNotNull();
    }

};

class CustomHandlerFactory : public HandlerFactory<TopLevelWindowHandler>
{
    virtual ref_of<qor::platform::win::gui::view::AbstractWindowHandler>::type Create()
    {
        auto styledHandler =new_ref<TopLevelWindowHandler>();
        auto ncrHandler    =new_ref<CustomNonClientRenderingHandler>();
                            styledHandler->m_ncRendering = ncrHandler;
        auto ncHandler   =  new_ref<CustomNonClientHandler>();
                            styledHandler->m_nonClient = ncHandler;
        auto rendering   =  new_ref<CustomRenderingHandler>();
                            styledHandler->m_rendering = rendering;
        return styledHandler;
    }
};

int main()
{
    WindowFactory factory(GetInstance());
    MessageHandler messageHandler;

    auto customClass =  factory.AddWindowClass(L"CustomWindow");
    ClassStyle          cs;
                        cs.SetHRedraw(true);
                        cs.SetVRedraw(true);
                        customClass->SetStyle(cs);
                        factory.RegisterClass(customClass, new_ref<CustomHandlerFactory>());


    auto controller =   factory.Create(L"MainWindow", L"Main Window Experiments");
                        controller->Show();
                        controller->Update();

    WindowStyle         style;
                        style.SetCaption(true);
                        style.SetSystemMenu(true);
                        style.SetSizingBorder(true);
                        style.SetMaximize(true);
                        style.SetMinimize(true);

    ExtendedWindowStyle exStyle;
                        exStyle.SetAcceptFiles(true);
                        exStyle.SetAppWindow(true);
                        exStyle.SetWindowEdge(true);
                        exStyle.SetClientEdge(true);
                        exStyle.SetComposited(true);
                        exStyle.SetControlParent(true);
                        exStyle.SetLayered(true);

    auto second =       factory.Create(customClass, L"Custom Top Level Window", style, exStyle);
                        second->SetTheme(L" ", L" ");
                        second->SetColourKey(0x0);
                        second->Show();
                        second->Update();


                        style.SetCaption(false);
                        style.SetSystemMenu(false);
                        style.SetSizingBorder(false);
                        exStyle.SetAppWindow(false);
                        exStyle.SetToolWindow(true);
                        exStyle.SetTopMost(true);

    auto tool =         factory.Create(L"MainWindow", L"ToolWindow", style, exStyle, 0, 0, 500, 100);
                        tool->Show();
                        tool->Update();

    return              messageHandler.MessageLoop();
}
