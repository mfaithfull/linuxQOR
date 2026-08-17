// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_CLANG_VERSION
#define QOR_PP_H_CLANG_VERSION

#include "clang.h"

#   ifdef qor_pp_compiler_reportconfig
#	    define qor_pp_compiler_message(_X) __pragma( message( _X ) )
#   else
#	    define qor_pp_compiler_message(_X)
#   endif

#	define qor_pp_compiler_version_folder recent
#	define qor_pp_compiler_name "recent clang"
#	define qor_pp_compiler_folder qor_pp_cat(qor_pp_compiler_root_folder,/recent)
#	define qor_pp_compiler_header recentclang.h
    qor_pp_compiler_message( qor_pp_compiler_name ": " qor_pp_stringize(qor_pp_compiler_version) )

#endif//QOR_PP_H_CLANG_VERSION

