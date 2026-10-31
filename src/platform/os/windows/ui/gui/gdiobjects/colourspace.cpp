// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "colourspace.h"
#include "src/platform/os/windows/common/stringconv.h"
#include "src/platform/os/windows/api_layer/gdi/gdi32.h"

using namespace qor::win::api;

namespace qor{ namespace platform { namespace win{

    ColourSpace::ColourSpace() : GDIObject(OColourSpace)
    {
    }

    ColourSpace::ColourSpace(const PrimitiveHandle& h, bool takeOwnership) : GDIObject(h, OColourSpace, takeOwnership)
    {
    }

    ColourSpace::~ColourSpace()
    {
        if(m_handle.NeedsClose())
        {
            Delete();
            m_handle.Drop();
        }
    }
    ColourSpace ColourSpace::Create(LogColourSpace* lpLogColorSpace)
    {
        ColourSpace cs(GDI32::CreateColorSpaceT(reinterpret_cast<LPLOGCOLORSPACE>(lpLogColorSpace)), true);
        return cs;
    }

    bool ColourSpace::Delete()
    {
        return GDI32::DeleteColorSpace((HCOLORSPACE)(m_handle.Use())) ? true : false;
    }

    ColourSpace ColourSpace::Get(const Handle& deviceContext)
    {
        ColourSpace cs(GDI32::GetColorSpace((HDC)(deviceContext.Use())), false);
        return cs;
    }

    bool ColourSpace::GetLogicalColorSpace(LogColourSpace* lpBuffer, unsigned long nSize)
    {
        return GDI32::GetLogColorSpaceT((HCOLORSPACE)(m_handle.Use()), reinterpret_cast<LPLOGCOLORSPACE>(lpBuffer), nSize) ? true : false;
    }

    Handle ColourSpace::Set(const Handle& deviceContext)
    {
        PrimitiveHandle ph(GDI32::SetColorSpace((HDC)(deviceContext.Use()), (HCOLORSPACE)(m_handle.Use())));
        Handle h(ph.Use());
        h.DontClose();
        return h;
    }

}}}//qor::platform::win