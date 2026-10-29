// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_GUI_METADEVICECONTEXT
#define QOR_PP_H_WINDOWS_GUI_METADEVICECONTEXT

#include "src/platform/os/windows/common/handles/handle.h"
#include "../view/drawing/rect.h"
#include "brush.h"
#include "../view/drawing/size.h"
#include "palette.h"
#include "gdiobject.h"

//All types on this interface must be portable
namespace qor{ namespace platform { namespace win{

    class MetaDeviceContext : public GDIObject
    {
    public:
        MetaDeviceContext();
        MetaDeviceContext(const PrimitiveHandle& h, bool takeOwnership);
        virtual ~MetaDeviceContext();
    };

}}}//qor::platform::win

#endif//QOR_PP_H_WINDOWS_GUI_METADEVICECONTEXT
