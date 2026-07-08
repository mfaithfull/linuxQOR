// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPILERPATHS
#define QOR_PP_H_COMPILERPATHS

#define qor_pp_compiler_version_header qor_pp_pathify(qor_pp_compiler_root_folder,version.h)

#ifdef qor_pp_compiler_version_header
#include qor_pp_compiler_version_header
#else
#	error "Could not find the root include path to support the detected compiler."
#endif

//Set up the compiler header paths according to the version detected
#define qor_pp_compiler_path qor_pp_pathify3(platform, compiler, qor_pp_compiler_root_folder)
#define qor_pp_compiler_include qor_pp_pathify3(qor_pp_compiler_root_folder, qor_pp_compiler_version_folder, qor_pp_compiler_header)
#define qor_pp_compiler_unfeature_include qor_pp_pathify(qor_pp_compiler_path, unfeature.h)
#define qor_pp_compiler_depcommon_include qor_pp_pathify(qor_pp_compiler_path, dependentcommon.h)
#define qor_pp_builtins_header qor_pp_pathify3(qor_pp_compiler_path, intrinsics, builtins.h)
#define qor_pp_unbuiltins_header qor_pp_pathify3(qor_pp_compiler_path, intrinsics, unbuiltins.h)
#define qor_pp_builtins_inc qor_pp_pathify3(qor_pp_compiler_path,intrinsics,builtins.inl)
#define qor_pp_intrinsics_header qor_pp_pathify3(qor_pp_compiler_path,intrinsics,intrinsics.h)
#define qor_pp_intrinsics_inc qor_pp_pathify3(qor_pp_compiler_path,intrinsics,intrinsics.inl)
#define qor_pp_compiler_buildopts qor_pp_pathify3(qor_pp_compiler_path, buildcommands, buildopts.h)

#endif//QOR_PP_H_COMPILERPATHS