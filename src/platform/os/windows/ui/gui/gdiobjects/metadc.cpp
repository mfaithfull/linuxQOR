// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "metadc.h"
#include "src/platform/os/windows/common/stringconv.h"
#include "src/platform/os/windows/api_layer/user/user32.h"

using namespace qor::win::api;

namespace qor{ namespace platform { namespace win{

    MetaDeviceContext::MetaDeviceContext() : GDIObject(OMetaDC)
    {
    }

    MetaDeviceContext::MetaDeviceContext(const PrimitiveHandle& h, bool takeOwnership) : GDIObject(h, OMetaDC, takeOwnership)
    {
    }

    MetaDeviceContext::~MetaDeviceContext()
    {
    }

}}}//qor::platform::win
