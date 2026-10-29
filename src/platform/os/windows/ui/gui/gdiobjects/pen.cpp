// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "pen.h"
#include "src/platform/os/windows/common/stringconv.h"
#include "src/platform/os/windows/api_layer/gdi/gdi32.h"

using namespace qor::win::api;

namespace qor{ namespace platform { namespace win{

    Pen::Pen() : GDIObject(OPen)
    {

    }

    Pen::Pen(const PrimitiveHandle& h, bool takeOwnerShip) : GDIObject(h, OPen, takeOwnerShip)
    {

    }

    Pen::~Pen()
    {

    }

    Pen Pen::Create(int style, int width, unsigned long crColour)
    {
        PrimitiveHandle ph(GDI32::CreatePen(style, width, crColour));
        Pen p(ph, true);
        return p;
    }

    Pen Pen::Create(const LogPen* lgpn)
    {
        PrimitiveHandle ph(GDI32::CreatePenIndirect(reinterpret_cast<const LOGPEN*>(lgpn)));
        Pen p(ph, true);
        return p;
    }

    Pen Pen::Create(unsigned long penStyle, unsigned long width, const LogBrush* lplb, unsigned long styleCount, const unsigned long* style)
    {
        PrimitiveHandle ph(GDI32::ExtCreatePen(penStyle, width, reinterpret_cast<const LOGBRUSH*>(lplb), styleCount, style));
        Pen p(ph, true);
        return p;
    }

    Pen Pen::WhitePen()
    {
        PrimitiveHandle ph(GDI32::GetStockObject(WHITE_PEN));
        Pen p(ph, false);
        return p;
    }

    Pen Pen::BlackPen()
    {
        PrimitiveHandle ph(GDI32::GetStockObject(BLACK_PEN));
        Pen p(ph, false);
        return p;
    }

    Pen Pen::NullPen()
    {
        PrimitiveHandle ph(GDI32::GetStockObject(NULL_PEN));
        Pen p(ph, false);
        return p;
    }

    Pen Pen::DCColourPen()
    {
        PrimitiveHandle ph(GDI32::GetStockObject(DC_PEN));
        Pen p(ph, false);
        return p;
    }

}}}//qor::platform::win
