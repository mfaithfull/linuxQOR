// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "dynamiclibrary.h"

namespace qor{

    DynamicLibrary::DynamicLibrary(){ }

    DynamicLibrary::DynamicLibrary(const std::string& /*libName*/){ }

    DynamicLibrary::~DynamicLibrary(){ }
    
    DynamicLibrary::DynProc DynamicLibrary::GetProcAddress(const std::string& /*procName*/)
    {
        DynamicLibrary::DynProc result = nullptr;
        return result;
    }
    
}//qor
