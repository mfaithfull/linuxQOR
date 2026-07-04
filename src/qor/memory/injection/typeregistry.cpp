// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "typeregistry.h"

namespace {
    static qor::TypeRegistry _theTypeRegistry;
}

namespace qor{

    qor_pp_module_interface(QOR_INJECTION) TypeRegistry* TheTypeRegistry()
    {
        return &(_theTypeRegistry);
    }

}//qor
