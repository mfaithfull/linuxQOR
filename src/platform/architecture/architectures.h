// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ARCHITECTURES
#define QOR_PP_H_ARCHITECTURES

#define qor_pp_arch_anyX86      0x01
#define qor_pp_arch_anyARM      0x02
#define qor_pp_arch_anyRISCV    0x04
//NOTE: Add new architecture support here

namespace qor{ namespace arch{

    enum Endian
    {
        host = 1,       //whatever the host is, i.e. don't mess with it
        network = 2,    //Network regardless of host
        big = 2,        //Big, for encoding, whatever the host is. This is actually the same as Network in practice but is used for target cpu
        little = 3,     //Little, for encoding, whatever the host is.
    };
    //Note: this might get more complex with ARM as some of those seem to be able to do both or to switch
}}//qor::arch

#endif//QOR_PP_H_ARCHITECTURES
