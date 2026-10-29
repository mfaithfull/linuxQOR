// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "gdiobject.h"
#include "src/platform/os/windows/common/stringconv.h"
#include "src/platform/os/windows/api_layer/user/user32.h"
#include "src/platform/os/windows/api_layer/gdi/gdi32.h"

using namespace qor::win::api;

namespace qor{ namespace platform { namespace win{


    GDIObject::GDIObject(GDIObjectType objType) : m_handle(0), m_objType(objType), m_owner(true)
    {
    }

    GDIObject::GDIObject(const PrimitiveHandle& h, GDIObjectType objType, bool takeOwnership = true) :
        m_handle(h.Use()), m_objType(objType), m_owner(takeOwnership)
    {
        m_handle.DontClose();
    }

    GDIObject::~GDIObject()
    {
        if(m_owner && !m_handle.IsInvalid())
        {
            GDI32::DeleteObject(m_handle.Use());
            m_handle.Drop();
        }
    }

    const Handle& GDIObject::GetHandle() const
    {
        return m_handle;
    }

}}}//qor::platform::win
