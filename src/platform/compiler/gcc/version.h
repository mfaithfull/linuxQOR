// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_GCC_VERSION
#define QOR_PP_H_GCC_VERSION

#include "gcc.h"

#define qor_pp_gcc_version              (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#define qor_pp_compiler_patch_version   qor_pp_cat(_,__GNUC_PATCHLEVEL__)
#define qor_pp_compiler_minor_version   qor_pp_cat(__GNUC_MINOR__,qor_pp_compiler_patch_version)
#define qor_pp_compiler_version         qor_pp_cat(gcc,qor_pp_cat(__GNUC__,qor_pp_cat(_, qor_pp_compiler_minor_version)))
#define qor_pp_compiler_version_folder  qor_pp_compiler_version
#define qor_pp_compiler_name            "Gnu C++ " qor_pp_stringize(__GNUC__) "." qor_pp_stringize(__GNUC_MINOR__) "." qor_pp_stringize(__GNUC_PATCHLEVEL__)
#define qor_pp_compiler_folder          qor_pp_cat(qor_pp_compiler_root_folder,/qor_pp_compiler_version_folder)
#define qor_pp_compiler_header          qor_pp_compiler_version.h

#endif//QOR_PP_H_GCC_VERSION