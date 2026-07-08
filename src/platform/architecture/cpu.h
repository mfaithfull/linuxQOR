// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ARCHITECTURE_CPU
#define QOR_PP_H_ARCHITECTURE_CPU

#include "detectarchitecture.h"

#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/essentials/objectcontext/anyobject.h"
#include "src/qor/memory/reference/newref.h"

/*The Machine class represents the core CPU hardware at runtime, it's capabilities services, 
features, configuration and settings.
*/

namespace qor{ namespace arch{

    class qor_pp_module_interface(QOR_ARCH) CPU
    {
    public:

        CPU();     
        ~CPU() = default;

        Endian ByteOrder();
    };

    }//qor::arch

    qor_pp_declare_instancer_of(arch::CPU, SingletonInstancer);
    qor_pp_declare_sync_of(arch::CPU, RecursiveMutex);

    qor_pp_module_interface(QOR_ARCH) ref_of<arch::CPU>::type TheCPU();
}//qor

#endif//QOR_PP_H_ARCHITECTURE_CPU