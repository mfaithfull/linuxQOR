// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "memdevicecontext.h"
#include "src/platform/os/windows/common/stringconv.h"
#include "src/platform/os/windows/api_layer/user/user32.h"

using namespace qor::win::api;

namespace qor{ namespace platform { namespace win{

    MemDeviceContext::MemDeviceContext() : GDIObject(OMemDC)
    {
    }

    MemDeviceContext::MemDeviceContext(const PrimitiveHandle& h, bool takeOwnership) : GDIObject(h, OMemDC, takeOwnership)
    {
    }

    MemDeviceContext::~MemDeviceContext()
    {
    }

}}}//qor::platform::win
