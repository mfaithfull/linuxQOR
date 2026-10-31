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
