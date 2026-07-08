// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_DETECTOS
#define QOR_PP_H_DETECTOS

#include "systems.h"

#ifndef qor_pp_os_target

#   ifdef __linux__
#       define qor_pp_os_target qor_pp_os_linux
#   elif (defined _WIN32 || defined _WIN64)
#       define qor_pp_os_target qor_pp_os_windows
#   elif (defined __APPLE__)
#       define qor_pp_os_target qor_pp_os_mac
#   elif (defined __ANDROID__)
#       error Target operating system Android detected but not supported
#   elif (defined __FreeBSD__)
#       error Target operating system Free BSD detected but not supported
#   else
#       error Target operating system could not be detected from host. Please define qor_pp_os_target in your build system
//NOTE: Add additional OS detection here - see:https://sourceforge.net/p/predef/wiki/OperatingSystems/
#   endif

#endif//qor_pp_os_target

#if (qor_pp_os_target == qor_pp_os_linux)
#   define qor_pp_os_root_folder linux
//qor_pp_compiler_message( "Build targetting Linux Operating Systems." )
#elif (qor_pp_os_target == qor_pp_os_windows)
#   define qor_pp_os_root_folder windows
#   ifdef _WIN64
#       define qor_pp_os_64bit
qor_pp_compiler_message( "Build targetting 64 bit Microsoft Windows Operating Systems." )
#       else
qor_pp_compiler_message( "Build targetting 32 bit Microsoft Windows Operating Systems." )
#   endif
#elif (qor_pp_os_target == qor_pp_os_mac)
#   define qor_pp_os_root_folder osx
qor_pp_compiler_message( "Build targetting Apple OSX." )
#endif

#endif//QOR_PP_H_OS
