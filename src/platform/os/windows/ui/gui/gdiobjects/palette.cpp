// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "palette.h"
#include "src/platform/os/windows/common/stringconv.h"
#include "src/platform/os/windows/api_layer/gdi/gdi32.h"

using namespace qor::win::api;

namespace qor{ namespace platform { namespace win{

    Palette::Palette() : GDIObject(OPalette)
    {
    }

    Palette::Palette(const PrimitiveHandle& h, bool takeOwnerShip) : GDIObject(h, OPalette, takeOwnerShip)
    {
    }

    Palette::~Palette()
    {
    }

    const Handle& Palette::GetHandle() const
    {
        return m_handle;
    }

    bool Palette::Animate(unsigned int iStartIndex, unsigned int cEntries, const PaletteEntry* ppe)
    {
        return GDI32::AnimatePalette((HPALETTE)(m_handle.Use()), iStartIndex, cEntries, reinterpret_cast< const PALETTEENTRY*>(ppe)) ? true : false;
    }

    Palette Palette::CreateHalftone(const Handle& deviceContext)
    {
        Palette p(GDI32::CreateHalftonePalette((HDC)(deviceContext.Use())), true);
        return p;
    }

    Palette Palette::Create(const LogPalette* lplgpl)
    {
        Palette p(GDI32::CreatePalette(reinterpret_cast<const LOGPALETTE*>(lplgpl)), true);
        return p;
    }

    Palette Palette::Default()
    {
        PrimitiveHandle h(GDI32::GetStockObject(DEFAULT_PALETTE));
        Palette p(h, false);
        return p;
    }

    unsigned int Palette::GetNearestIndex(unsigned long crColor)
    {
        return GDI32::GetNearestPaletteIndex((HPALETTE)(m_handle.Use()), crColor);
    }

    unsigned int Palette::GetEntries(unsigned int iStartIndex, unsigned int nEntries, PaletteEntry* lppe)
    {
        return GDI32::GetPaletteEntries((HPALETTE)(m_handle.Use()), iStartIndex, nEntries, reinterpret_cast<LPPALETTEENTRY>(lppe));
    }

    bool Palette::Resize(unsigned int nEntries)
    {
        return GDI32::ResizePalette((HPALETTE)(m_handle.Use()), nEntries) ? true : false;
    }
/*
    Handle Palette::Select(Handle& hdc, bool bForceBackground)
    {
        PrimitiveHandle ph (
                GDI32::SelectPalette(
                    (HDC)(hdc.Use()), (HPALETTE)(m_handle.Use()), bForceBackground ? 1: 0) );
        Handle h(ph.Use());
        h.DontClose();
        return h;
    }
*/
    unsigned int Palette::SetEntries(unsigned int iStart, unsigned int cEntries, const PaletteEntry* lppe)
    {
        return GDI32::SetPaletteEntries((HPALETTE)(m_handle.Use()), iStart, cEntries, reinterpret_cast<const PALETTEENTRY*>(lppe));
    }

}}}//qor::platform::win