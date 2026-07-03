// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "codepageregistry.h"
#include "codepageregentry.h"

namespace {
    static qor::CodePageRegistry _theCodePageRegistry;
}

namespace qor{
    CodePageRegistry* TheCodePageRegistry()
    {
        return &(_theCodePageRegistry);
    }
}//qor
