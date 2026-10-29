// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_GUI_METAFILE
#define QOR_PP_H_WINDOWS_GUI_METAFILE

#include "src/platform/os/windows/common/handles/handle.h"
#include "gdiobject.h"

//All types on this interface must be portable
namespace qor{ namespace platform { namespace win{

    class MetaFile : public GDIObject
    {

    };

}}}//qor::platform::win

#endif//QOR_PP_H_WINDOWS_GUI_METAFILE