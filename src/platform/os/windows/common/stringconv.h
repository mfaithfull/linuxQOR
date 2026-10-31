// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_OS_WINDOWS_COMMON_STRINGCONV
#define QOR_PP_H_PLATFORM_OS_WINDOWS_COMMON_STRINGCONV

#include <string>

namespace qor { namespace platform { namespace win {

#if ( qor_pp_unicode )
	qor_pp_module_interface(QOR_WINCOMMON) std::wstring to_tstring(const char* data);
#else
    qor_pp_module_interface(QOR_WINCOMMON) std::string to_tstring(const char* source);
#endif

}}}//qor::platform::win

#endif//QOR_PP_H_PLATFORM_OS_WINDOWS_COMMON_STRINGCONV