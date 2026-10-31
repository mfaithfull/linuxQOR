// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_WINDOWS_GUI_GDIOBJECT
#define QOR_PP_H_WINDOWS_GUI_GDIOBJECT

#include "src/platform/os/windows/common/handles/handle.h"

//All types on this interface must be portable
namespace qor{ namespace platform { namespace win{

    enum GDIObjectType
    {
        OBitmap,
        OBrush,
        OColourSpace,
        ODeviceContext,
        OEnhMetaDC,
        OEnhMetaFile,
        OExtPen,
        OFont,
        OMemDC,
        OMetaFile,
        OMetaDC,
        OPalette,
        OPen,
        ORegion,
        OUnknown
    };

    class qor_pp_module_interface(QOR_WINGUI) GDIObject
    {
    public:

        GDIObject(GDIObjectType objType);
        GDIObject(const PrimitiveHandle& h, GDIObjectType objType, bool takeOwnership);
        virtual ~GDIObject();

        const Handle& GetHandle() const;

    protected:

        Handle m_handle;
        GDIObjectType m_objType;
        bool m_owner;
    };

}}}//qor::platform::win

#endif//QOR_PP_H_WINDOWS_GUI_GDIOBJECT