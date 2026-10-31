// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "bitmap.h"
#include "src/platform/os/windows/common/stringconv.h"
#include "devicecontext.h"
#include "src/platform/os/windows/api_layer/user/user32.h"
#include "src/platform/os/windows/api_layer/gdi/gdi32.h"

using namespace qor::win::api;

namespace qor{ namespace platform { namespace win{

    Bitmap::Bitmap() : GDIObject(OBitmap)
    {
    }

    Bitmap::Bitmap(const PrimitiveHandle& h, bool takeOwnership) : GDIObject(h, OBitmap, takeOwnership)
    {
    }

    Bitmap::~Bitmap()
    {
    }

    DIB::DIB() : Bitmap()
    {
        //GDI32::CreateDIBitmap(hdc, bitmapinfoheader, initFlags, init, bitmapinfo, usage);
    }

    DIB::DIB(const PrimitiveHandle& h, bool takeOwnership) : Bitmap(h, takeOwnership)
    {
    }

    DIB::~DIB()
    {
    }

    DIBSection::DIBSection(ref_of<DeviceContext>::type dc, const BitmapInfo* bmpi, unsigned int usage, void** bits, void* section, unsigned long offset) : Bitmap()
    {
        m_handle = GDI32::CreateDIBSection((HDC)(dc->GetHandle().Use()), reinterpret_cast<CONST::BITMAPINFO*>(bmpi), usage, bits, (HANDLE)(section), offset);
        m_owner = true;
    }

    DIBSection::DIBSection(const PrimitiveHandle& h, bool takeOwnership) : Bitmap(h, takeOwnership)
    {
    }

    DIBSection::~DIBSection()
    {
    }

}}}//qor::platform::win
