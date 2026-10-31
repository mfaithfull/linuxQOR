// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_GUI_ENHMETAFILE
#define QOR_PP_H_WINDOWS_GUI_ENHMETAFILE

#include "src/platform/os/windows/common/handles/handle.h"
#include "gdiobject.h"

//All types on this interface must be portable
namespace qor{ namespace platform { namespace win{

    class EnhancedMetaFile : public GDIObject
    {
    public:

        explicit EnhancedMetaFile(const PrimitiveHandle& h, bool own = true);

    protected:

        PrimitiveHandle m_handle;

    };

}}}//qor::platform::win

#endif//QOR_PP_H_WINDOWS_GUI_ENHMETAFILE