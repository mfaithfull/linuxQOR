// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_GUI_ENHMETADEVICECONTEXT
#define QOR_PP_H_WINDOWS_GUI_ENHMETADEVICECONTEXT

#include "src/platform/os/windows/common/handles/handle.h"
#include "../view/drawing/rect.h"
#include "brush.h"
#include "../view/drawing/size.h"
#include "palette.h"
#include "gdiobject.h"

//All types on this interface must be portable
namespace qor{ namespace platform { namespace win{

    class EnhancedMetaDeviceContext : public GDIObject
    {
    public:
        EnhancedMetaDeviceContext();
        EnhancedMetaDeviceContext(const PrimitiveHandle& h, bool takeOwnership);
        virtual ~EnhancedMetaDeviceContext();
    };

}}}//qor::platform::win

#endif//QOR_PP_H_WINDOWS_GUI_ENHMETADEVICECONTEXT
