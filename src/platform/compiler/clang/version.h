// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_CLANG_VERSION
#define QOR_PP_H_CLANG_VERSION

#include "clang.h"

#define qor_pp_compiler_version _MSC_VER        

#if(qor_pp_compiler_version < 1931)
#error Unsupported compiler version. Please update your compiler.
#else
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

#endif

#if(qor_pp_compiler_version < 1944)
    qor_pp_compiler_message( "Compiler version is supprted." )
#else
    qor_pp_compiler_message( "Compiler version is untested. Proceed at your own risk" )
#endif

#endif//QOR_PP_H_CLANG_VERSION