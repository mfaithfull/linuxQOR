// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "cursor.h"
#include "src/platform/os/windows/common/stringconv.h"
#include "src/platform/os/windows/api_layer/user/user32.h"
#include "../view/drawing/point.h"

using namespace qor::win::api;

namespace qor{ namespace platform { namespace win{

    Cursor::Cursor(const PrimitiveHandle& h, bool own) : m_handle(h.Use()), m_owning(own)
    {
    }

    Cursor::Cursor(const Cursor& src) : m_handle(src.m_handle), m_owning(false)
    {
    }

    const PrimitiveHandle& Cursor::GetHandle() const
    {
        return m_handle;
    }

    Cursor::Cursor(void* hInst, int xHotSpot, int yHotSpot, int nWidth, int nHeight, const void* pvANDPlane, const void* pvXORPlane)
    {
        m_handle = User32::CreateCursor(
            reinterpret_cast<HINSTANCE>(hInst), xHotSpot, yHotSpot, nWidth, nHeight, pvANDPlane, pvXORPlane);
        m_owning = true;
    }

    Cursor::Cursor(void* hInstance, const std::string& cursorName)
    {
        m_handle = User32::LoadCursorT(
            reinterpret_cast<HINSTANCE>(hInstance),
            to_tstring(cursorName.c_str()).c_str());
        m_owning = false;
    }

    Cursor::Cursor(const std::string& fileName)
    {
        m_handle = User32::LoadCursorFromFileT(to_tstring(fileName.c_str()).c_str());
        m_owning = true;
    }

    Cursor::Cursor(const wchar_t* id)
    {
        m_handle = User32::LoadCursorT( reinterpret_cast<HINSTANCE>(nullptr), id);
        m_owning = false;
    }

    //U20 - this destructor destroys on a flag the ctor guessed, and leaves the handle for ~PrimitiveHandle to complain about.
    //(Note landed by the winqor_api_user review; its entry is src/platform/os/windows/api_layer/user/registers/u20.md,
    //opened 30 Oct 2026 (session 9, cursor.cpp) and closed 11 Nov 2026 by decision D47. The cites below are that
    //entry's, carried as written - nothing in this file was re-audited for them.)
    //m_owning is whatever the caller asserted - cursor.h:29's own = true - and a handle carries no ownership of its own;
    //see the ownership paragraph at cursor.h. Two consequences of that, both measured:
    //Clone() (:64-69) feeds PrimitiveHandle(User32::CopyCursorT(...)) straight into Cursor(handle, true). CopyCursorT
    //binds the SDK's CopyIcon (there is no CopyCursor export - winuser.h:10681 is a cast macro), and Library::Call
    //returns nullptr when the call yields nothing, so a failed copy still runs DestroyCursor(nullptr) - there is no
    //null or invalid-handle guard on this path.
    //m_handle is never cleared, and PrimitiveHandle has no release or reset API (common/handles/handle.h:9-47), so the
    //member's own destructor (common/handles/handle.cpp:27-33) sees a non-null m_h and raises
    //warning("PrimitiveHandle destructor called with non-null handle. This may indicate a resource leak.") for an
    //owning object whose handle was already destroyed and for a non-owning object whose handle was never its own to
    //destroy alike. This is the one U20 finding that runs today: windowfactory.cpp:90
    //(windowClass->SetCursor(Cursor(IDC_ARROW))) builds a non-owning temporary - IDC_ARROW reaches the
    //(const wchar_t* id) ctor at :49-53, so no DestroyCursor runs - and raises that warning once per AddWindowClass call.
    //For the audit, the wrappers this class feeds already make the distinction the callers miss: CreateCursor,
    //LoadCursorFromFileT and CopyCursorT return handles the caller must DestroyCursor, while LoadCursorT and GetCursor
    //return shared system handles that must not be destroyed (src/platform/os/windows/api_layer/user/cursor.cpp:35-41).
    //Recorded, not fixed: m_owning is this class's ownership contract and the class is winqor_gui published surface -
    //changing who destroys a handle is a behaviour change in another library, which the review records rather than
    //performs. That is the rule U20 states, and U20 closes on it at D47 with these notes in place.


    Cursor::~Cursor()
    {
        if(m_owning)
        {
            User32::DestroyCursor(reinterpret_cast<HCURSOR>(m_handle.Use()));
            m_owning = false;
        }
    }

    Cursor Cursor::Clone()
    {
        Cursor c(PrimitiveHandle(User32::CopyCursorT(
            reinterpret_cast<HCURSOR>(m_handle.Use()))), true);
        return c;
    }

    bool Cursor::Clip(const Rect& rect)
    {
        RECT r = {rect.m_left, rect.m_top, rect.m_right, rect.m_bottom};
        return User32::ClipCursor(&r) ? true : false;
    }

    bool Cursor::GetClip(Rect& rect)
    {
        RECT r = {0};
        bool result = User32::GetClipCursor(&r) ? true : false;
        if(result)
        {
            rect.m_left = r.left;
            rect.m_top = r.top;
            rect.m_right = r.right;
            rect.m_bottom = r.bottom;
        }
        return result;
    }

    Cursor Cursor::GetCurrent()
    {
        PrimitiveHandle h(User32::GetCursor());
        Cursor c(h, false);
        return c;
    }

    bool Cursor::GetInfo(CursorInfo& ci)
    {
        ci.cbSize = sizeof(CursorInfo);
        return User32::GetCursorInfo(
            reinterpret_cast<PCURSORINFO>(&ci)) ? true : false;
    }

    bool Cursor::GetPosition(Point& point)
    {
        ::POINT p = {0};
        bool result = User32::GetCursorPos(&p) ? true : false;
        if(result)
        {
            point.m_x = p.x;
            point.m_y = p.y;
        }
        return result;
    }

    bool Cursor::GetPhysicalPosition(Point& point)
    {
        ::POINT p = {0};
        bool result = User32::GetPhysicalCursorPos(&p) ? true : false;
        if(result)
        {
            point.m_x = p.x;
            point.m_y = p.y;
        }
        return result;
    }

    bool Cursor::SetPosition(int x, int y)
    {
        return User32::SetCursorPos(x, y) ? true : false;
    }

    bool Cursor::SetPhysicalPosition(int x, int y)
    {
        return User32::SetPhysicalCursorPos(x, y) ? true : false;
    }

    //U20 - Show() reads ShowCursor's return as a flag, and it is not one.
    //(Note landed by the winqor_api_user review; its entry is src/platform/os/windows/api_layer/user/registers/u20.md,
    //opened 30 Oct 2026 (session 9, cursor.cpp) and closed 11 Nov 2026 by decision D47. The cites below are that
    //entry's, carried as written - nothing in this file was re-audited for them.)
    //ShowCursor returns the new signed display count, not a success flag - the api_layer's own banner at
    //src/platform/os/windows/api_layer/user/cursor.cpp:40-41 records it as "ShowCursor returns the new display count,
    //so 0 or negative is a normal hide, not a failure". The wrapper's int return is already right; the loss is here.
    //So the first Show(false) takes the count to -1 and reports failure for a hide that worked, and the next Show(true)
    //takes it back to 0 and reports success while changing nothing. There is no failure channel for a return check to
    //read: the call either moved the count or the caller is asking the wrong question.
    //Recorded, not fixed: making this bool mean "it worked" changes what every caller of Show() infers - a behaviour
    //change in winqor_gui, which the api_layer review records rather than performs (the rule U20 states, closed by D47).


    bool Cursor::Show(bool show)
    {
        return User32::ShowCursor(show ? 1 : 0) ? true : false;
    }

    void Cursor::SetAsCurrent()
    {
        User32::SetCursor(
            reinterpret_cast<HCURSOR>(m_handle.Use()));
    }

    bool Cursor::SetAsSystemCursor(unsigned long id)
    {
        return User32::SetSystemCursor(
            reinterpret_cast<HCURSOR>(m_handle.Use()), id) ? true : false;
    }

}}}//qor::platform::win
