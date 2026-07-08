// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DYNAMICLIBRARY
#define QOR_PP_H_FRAMEWORK_DYNAMICLIBRARY

#include <string>

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/externalfactory.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/app/role/ifeature.h"

namespace qor { 
  
    bool qor_pp_module_interface(QOR_DYNAMICLIBARY) ImplementsDynamicLibrary();

    class qor_pp_module_interface(QOR_DYNAMICLIBARY) DynamicLibrary
    {
    public:

        typedef int(*DynProc)(void);//Default procedure pointer type        

        DynamicLibrary();
        DynamicLibrary(const std::string& libName);
        virtual ~DynamicLibrary();

        virtual DynProc GetProcAddress(const std::string& procName);      
        
    };

    qor_pp_declare_factory_of(DynamicLibrary, ExternalFactory);
    constexpr GUID DynamicLibraryGUID = {0x106DCCF1, 0x6ECD, 0x4518, {0x91, 0x0F, 0x7D, 0xE3, 0xE6, 0xD5, 0xD3, 0x8C}};    
    qor_pp_declare_guid_of(DynamicLibrary,DynamicLibraryGUID);

}//qor

#endif//QOR_PP_H_FRAMEWORK_DYNAMICLIBRARY
