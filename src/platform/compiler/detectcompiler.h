// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

/*  Compiler detection based on code from Boost
    Including this file will define two macros
    qor_pp_compiler as an arbitrary number representing the commpiler
    qor_pp_compiler_root_folder as a name that can be used to form part of a path*/

#ifndef QOR_PP_H_DETECTCOMPILER
#define QOR_PP_H_DETECTCOMPILER

#include "compilers.h"

#if defined __clang__ && !defined(__CUDACC__) && !defined(__ibmxl__)
#   define qor_pp_compiler qor_pp_compiler_clang
#   define qor_pp_compiler_root_folder clang
#elif defined __GNUC__ && !defined(__ibmxl__)
#   define qor_pp_compiler qor_pp_compiler_gcc
#   define qor_pp_compiler_root_folder gcc
//Note: Add new compiler support here
#elif defined _MSC_VER
#   define qor_pp_compiler qor_pp_compiler_msvc
#   define qor_pp_compiler_root_folder msvc
#elif defined __EDG__
#   define qor_pp_compiler qor_pp_compiler_edg
#   define qor_pp_compiler_root_folder edg
#endif

// Attempt to generate an error if we don't recognise the compiler:
#	ifndef qor_pp_compiler
#		error "Unknown compiler - Please add support for your compiler at the 'NOTE: Add new compiler support here' marked points "
#	endif

#endif//QOR_PP_H_DETECTCOMPILER
