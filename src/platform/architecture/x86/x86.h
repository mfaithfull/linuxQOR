// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ARCHITECTURE_X86
#define QOR_PP_H_ARCHITECTURE_X86

namespace qor{ namespace arch {

    constexpr Endian endian = little;

#if(qor_pp_arch_is_64bit)
    constexpr unsigned wordBytes = 8;
#else
    constexpr unsigned wordBytes = 4;
#endif

}}//qor::arch

#endif//QOR_PP_H_ARCHITECTURE_X86