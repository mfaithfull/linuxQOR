// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "enhmetadc.h"
#include "src/platform/os/windows/common/stringconv.h"
#include "src/platform/os/windows/api_layer/user/user32.h"

using namespace qor::win::api;

namespace qor{ namespace platform { namespace win{

    EnhancedMetaDeviceContext::EnhancedMetaDeviceContext() : GDIObject(OEnhMetaDC)
    {
    }

    EnhancedMetaDeviceContext::EnhancedMetaDeviceContext(const PrimitiveHandle& h, bool takeOwnership) : GDIObject(h, OEnhMetaDC, takeOwnership)
    {
    }

    EnhancedMetaDeviceContext::~EnhancedMetaDeviceContext()
    {
    }

}}}//qor::platform::win
