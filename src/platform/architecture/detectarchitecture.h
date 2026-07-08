// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DETECTARCHITECTURE
#define QOR_PP_H_DETECTARCHITECTURE

#include "architectures.h"

#undef qor_pp_arch_is_64bit
#ifndef qor_pp_arch_target
#   if defined(_M_IX86) || defined(__i386__) || defined(i386) || defined(_X86_) || defined(__THW_INTEL) ||  defined(__x86_64__) || defined(_M_X64)
#       define qor_pp_arch_target   qor_pp_arch_anyX86
#   elif defined(arm) || defined(__arm__) || defined(ARM) || defined(_ARM_) || defined(__aarch64__)
#       define qor_pp_arch_target   qor_pp_arch_anyARM
#   endif
#endif//qor_pp_arch_target

#if (qor_pp_arch_target == qor_pp_arch_anyX86)
#   define qor_pp_arch_root_folder x86
#   if (defined (__x86_64__) || defined (_M_X64))
#       define qor_pp_arch_is_64bit 1
//TODO: For reasons unknown the following line breaks GCC 9.4.0 blowing up the compile
//qor_pp_compiler_message( "Build targetting 64 bit x86 hardware." )
#   else
qor_pp_compiler_message( "Build targetting 32 bit x86 hardware." )
#       undef qor_pp_arch_is_64bit
#   endif
#elif (qor_pp_arch_target == qor_pp_arch_anyARM)
#   define qor_pp_arch_root_folder arm
qor_pp_compiler_message( "Build targetting ARM hardware." )
#elif (qor_pp_arch_target == qor_pp_arch_anyRISCV)
#   define qor_pp_arch_root_folder riscv
qor_pp_compiler_message( "Build targetting RISC-V hardware." )
#else
#   error Target architecture not defined or detected. Define qor_pp_arch_target in your build settings
#endif

#if(qor_pp_arch_target == qor_pp_arch_anyX86)
#   include "x86/x86.h"
#elif(qor_pp_arch_target == qor_pp_arch_anyARM)
#   include "arm/arm.h"
#elif (qor_pp_arch_target == qor_pp_arch_anyRISCV)
#   include "riscv/riscv.h"
#endif

#endif//QOR_PP_H_DETECTARCHITECTURE
