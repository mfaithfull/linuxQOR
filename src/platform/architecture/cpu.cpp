// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "cpu.h"

namespace qor{ 
    
    qor_pp_module_interface(QOR_ARCH) ref_of<arch::CPU>::type TheCPU()
    {
        return new_ref<arch::CPU>();        
    }

    namespace arch{

    CPU::CPU()
    { }    

    Endian CPU::ByteOrder()
    {
        return endian;
    }

}}//qor::arch
