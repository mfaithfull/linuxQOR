// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_GUI_BRUSH
#define QOR_PP_H_WINDOWS_GUI_BRUSH

#include <string>

#include "src/platform/os/windows/common/handles/handle.h"
#include "../view/drawing/point.h"
#include "gdiobject.h"

//All types on this interface must be portable
namespace qor{ namespace platform { namespace win{

    struct LogBrush
    {
        unsigned int        m_style;
        unsigned long       m_colour;
        unsigned long long  m_hatch;
    };

    class qor_pp_module_interface(QOR_WINGUI) Brush : public GDIObject
    {
    public:

        Brush();
        Brush(int);
        Brush(const PrimitiveHandle& h, bool takeOwnership);
        virtual ~Brush();

        static Brush CreateIndirect(const LogBrush& logBrush);
        static Brush CreateDIBPattern(void* hglbDIBPacked, unsigned int fuColorSpec);
        static Brush CreateHatch(int fnStyle, unsigned long clrref);
        static Brush CreatePattern(Handle& bitmap);
        static Brush CreateSolid(unsigned long crColor);
        static Brush BlackBrush();
        static Brush DarkGrayBrush();
        static Brush DCBrush();
        static Brush GrayBrush();
        static Brush HollowBrush();
        static Brush LightGrayBrush();
        static Brush NullBrush();
        static Brush WhiteBrush();

        bool GetOrg(const Handle& deviceContext, Point& pt);
        bool SetOrg(const Handle& deviceContext, int x, int y, Point& pt);
    };

    class qor_pp_module_interface(QOR_WINGUI) SysBrush : public Brush
    {
    public:

        SysBrush(int nIndex);
        virtual ~SysBrush();
    };

}}}//qor::platform::win

#endif//QOR_PP_H_WINDOWS_GUI_BRUSH
