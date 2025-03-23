// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef QOR_PP_H_DETECTARCHITECTURE
#define QOR_PP_H_DETECTARCHITECTURE

#include "architectures.h"

#ifndef qor_pp_arch_target
//TODO: Actual architecture detection
#endif//qor_pp_arch_target

#if (qor_pp_arch_target == qor_pp_arch_anyX86)
#   define qor_pp_arch_root_folder x86
qor_pp_compiler_message( "Build targetting x86 hardware." )
#elif (qor_pp_arch_target == qor_pp_arch_anyARM)
#   define qor_pp_arch_root_folder arm
qor_pp_compiler_message( "Build targetting ARM hardware." )
#elif (qor_pp_arch_target == qor_pp_os_mac)
#   define qor_pp_arch_root_folder riscv
qor_pp_compiler_message( "Build targetting RISC-V hardware." )
#else
#   error Target architecture not defined or detected. Define qor_pp_arch_target in your build settings
#endif

#endif//QOR_PP_H_DETECTARCHITECTURE
