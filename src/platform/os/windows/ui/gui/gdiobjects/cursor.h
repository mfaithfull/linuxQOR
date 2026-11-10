// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_GUI_CURSOR
#define QOR_PP_H_WINDOWS_GUI_CURSOR

#include <string>

#include "src/platform/os/windows/common/handles/handle.h"
#include "../view/drawing/rect.h"
#include "../view/drawing/point.h"

//All types on this interface must be portable
namespace qor{ namespace platform { namespace win{

    //Information about the global cursor.
    struct CursorInfo
    {
        unsigned long   cbSize;
        unsigned long   flags;
        void* hCursor;
        Point   ptScreenPos;
    };

    class qor_pp_module_interface(QOR_WINGUI) Cursor
    {
    public:

        //U20 - the ownership default on this ctor is a guess about a handle that carries no ownership.
        //(Note landed by the winqor_api_user review; its entry is src/platform/os/windows/api_layer/user/registers/u20.md,
        //opened 30 Oct 2026 (session 9, cursor.cpp) and closed 11 Nov 2026 by decision D47. The cites below are that
        //entry's, carried as written - nothing in this file was re-audited for them.)
        //A Windows handle is a value, not an owner. Who has to release it is a property of the call that produced it, and
        //the release verb is per family - none of these has a Close. HCURSOR releases as DestroyCursor
        //(src/platform/os/windows/api_layer/user/user32.h:391), HICON as DestroyIcon (:438), HWND as DestroyWindow
        //(:152) and not as any CloseWindow, HDC as ReleaseDC (:339), which requires the hWnd/hDC pair to match, HMENU
        //as DestroyMenu (:227), HACCEL as DestroyAcceleratorTable (:428). The GDI families name their own verb again -
        //gdi32.h DeleteColorSpace :107, DeleteDC :141, DeleteObject :144, DeleteEnhMetaFile :251 - and "Close" there is
        //a creator, not a release: CloseEnhMetaFile (gdi32.h:245) and CloseMetaFile (:246) make an
        //HENHMETAFILE/HMETAFILE out of an HDC, and DeleteEnhMetaFile/DeleteMetaFile (:251, :252) release what they made.
        //ShutdownBlockReasonDestroy (user32.h:671) destroys nothing at all. So a handle is in one of three states -
        //ours and it must be released, the OS's and it must never be, or ours but still live somewhere else and not
        //releasable yet - and the ctor cannot tell which, because a LoadCursorT result and a CreateCursor result arrive
        //as the same bare value. own = true asserts the first state for every caller: right for the CreateCursor and
        //LoadCursorFromFile ctors below, which set m_owning = true themselves, wrong for the two LoadCursorT ctors,
        //which set it false (cursor.cpp:35-41, :49-53), and wrong for GetCurrent(), which takes GetCursor's shared
        //handle and sets it false too (cursor.cpp:91-96). The handle ctor is the one that cannot be right for both of
        //its callers: Clone() feeds it a CopyCursorT result it owns, while a caller feeding it a GetCursor or
        //LoadCursorT result owns nothing.
        //Recorded, not fixed: the default sits on this class's published interface (qor_pp_module_interface(QOR_WINGUI)
        //at cursor.h:25), so moving it moves what every winqor_gui caller relies on - a behaviour change in another
        //library, which the api_layer review records rather than performs. That is the rule this item states; U20
        //closes on it at D47, with the notes left where the errors actually are. See the block at ~Cursor() in .cpp.


        explicit Cursor(const PrimitiveHandle& h, bool own = true);
        Cursor(const Cursor& src);
        Cursor(void* hInst, int xHotSpot, int yHotSpot, int nWidth, int nHeight, const void* pvANDPlane, const void* pvXORPlane);
        Cursor(void* hInstance, const std::string& cursorName);
        Cursor(const std::string& fileName);
        Cursor(const wchar_t* id);
        virtual ~Cursor();

        Cursor Clone();
        const PrimitiveHandle& GetHandle() const;

        bool Clip(const Rect& rect);
        bool GetClip(Rect& rect);
        static Cursor GetCurrent();
        bool GetInfo(CursorInfo& ci);
        bool GetPosition(Point& point);
        bool GetPhysicalPosition(Point& point);
        bool SetPosition(int x, int y);
        bool SetPhysicalPosition(int x, int y);
        static bool Show(bool show);
        void SetAsCurrent();
        bool SetAsSystemCursor(unsigned long id);

    protected:
        PrimitiveHandle m_handle;

    private:
        bool m_owning;

    };

}}}//qor::platform::win

#endif//QOR_PP_H_WINDOWS_GUI_CURSOR
