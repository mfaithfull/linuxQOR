// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_GUI_PEN
#define QOR_PP_H_WINDOWS_GUI_PEN

#include "src/platform/os/windows/common/handles/handle.h"
#include "../view/drawing/point.h"
#include "gdiobject.h"
#include "brush.h"

//All types on this interface must be portable
namespace qor{ namespace platform { namespace win{

    struct LogPen
    {
        unsigned int    lopnStyle;
        Point           lopnWidth;
        unsigned long   lopnColor;
    };

    class qor_pp_module_interface(QOR_WINGUI) Pen : public GDIObject
    {
        Pen();
        Pen(const PrimitiveHandle& h, bool takeOwnerShip);
        virtual ~Pen();

        static Pen Create(int style, int width, unsigned long crColour);
        static Pen Create(const LogPen* lgpn);
        static Pen Create(unsigned long penStyle, unsigned long width, const LogBrush* lplb, unsigned long styleCount, const unsigned long* style);
        static Pen WhitePen();
        static Pen BlackPen();
        static Pen NullPen();
        static Pen DCColourPen();
    };

}}}//qor::platform::win

#endif//QOR_PP_H_WINDOWS_GUI_PEN