// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_GUI_MEMDEVICECONTEXT
#define QOR_PP_H_WINDOWS_GUI_MEMDEVICECONTEXT

#include "src/platform/os/windows/common/handles/handle.h"
#include "../view/drawing/rect.h"
#include "brush.h"
#include "../view/drawing/size.h"
#include "palette.h"
#include "gdiobject.h"

//All types on this interface must be portable
namespace qor{ namespace platform { namespace win{

    class MemDeviceContext : public GDIObject
    {
    public:
        MemDeviceContext();
        MemDeviceContext(const PrimitiveHandle& h, bool takeOwnership);
        virtual ~MemDeviceContext();
    };

}}}//qor::platform::win

#endif//QOR_PP_H_WINDOWS_GUI_MEMDEVICECONTEXT
