// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "brush.h"
#include "src/platform/os/windows/common/stringconv.h"
#include "src/platform/os/windows/api_layer/user/user32.h"
#include "src/platform/os/windows/api_layer/gdi/gdi32.h"

using namespace qor::win::api;

namespace qor{ namespace platform { namespace win{

    Brush::Brush() : GDIObject(OBrush)
    {
    }

    Brush::Brush(int colour) : GDIObject(colour, OBrush, true)
    {
    }

    Brush::Brush(const PrimitiveHandle& h, bool takeOwnership) : GDIObject(h, OBrush, takeOwnership)
    {
    }

    Brush::~Brush()
    {
    }

    Brush Brush::CreateIndirect(const LogBrush& logBrush)
    {
        Brush b(GDI32::CreateBrushIndirect(reinterpret_cast<const LOGBRUSH*>(&logBrush)), true);
        return b;
    }

    Brush Brush::CreateDIBPattern(void* hglbDIBPacked, unsigned int fuColorSpec)
    {
        Brush b(GDI32::CreateDIBPatternBrush(hglbDIBPacked, fuColorSpec), true);
        return b;
    }

    Brush Brush::CreateHatch(int fnStyle, unsigned long clrref)
    {
        Brush b(GDI32::CreateHatchBrush(fnStyle, clrref), true);
        return b;
    }

    Brush Brush::CreatePattern(Handle& bitmap)
    {
        Brush b(GDI32::CreatePatternBrush((HBITMAP)(bitmap.Use())), true);
        return b;
    }

    Brush Brush::CreateSolid(unsigned long crColor)
    {
        Brush b(GDI32::CreateSolidBrush(crColor), true);
        return b;
    }

    bool Brush::GetOrg(const Handle& deviceContext, Point& pt)
    {
        return GDI32::GetBrushOrgEx((HDC)(deviceContext.Use()), reinterpret_cast<LPPOINT>(&pt)) ? true : false;
    }

    bool Brush::SetOrg(const Handle& deviceContext, int x, int y, Point& pt)
    {
        return GDI32::SetBrushOrgEx((HDC)(deviceContext.Use()), x, y, reinterpret_cast<LPPOINT>(&pt)) ? true : false;
    }

    Brush Brush::BlackBrush()
    {
        return Brush(PrimitiveHandle(GDI32::GetStockObject(BLACK_BRUSH)), false);
    }

    Brush Brush::DarkGrayBrush()
    {
        return Brush(PrimitiveHandle(GDI32::GetStockObject(DKGRAY_BRUSH)), false);
    }

    Brush Brush::DCBrush()
    {
        return Brush(PrimitiveHandle(GDI32::GetStockObject(DC_BRUSH)), false);
    }

    Brush Brush::GrayBrush()
    {
        return Brush(PrimitiveHandle(GDI32::GetStockObject(GRAY_BRUSH)), false);
    }

    Brush Brush::HollowBrush()
    {
        return Brush(PrimitiveHandle(GDI32::GetStockObject(HOLLOW_BRUSH)), false);
    }

    Brush Brush::LightGrayBrush()
    {
        return Brush(PrimitiveHandle(GDI32::GetStockObject(LTGRAY_BRUSH)), false);
    }

    Brush Brush::NullBrush()
    {
        return Brush(PrimitiveHandle(GDI32::GetStockObject(NULL_BRUSH)), false);
    }

    Brush Brush::WhiteBrush()
    {
        return Brush(PrimitiveHandle(GDI32::GetStockObject(WHITE_BRUSH)), false);
    }

    SysBrush::SysBrush(int nIndex) : Brush()
    {
        m_handle = User32::GetSysColorBrush(nIndex);
        m_handle.DontClose();
        m_owner = false;
    }

    SysBrush::~SysBrush()
    {
    }


}}}//qor::platform::win
