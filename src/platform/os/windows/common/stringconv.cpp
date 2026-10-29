// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "stringconv.h"
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"

namespace qor { namespace platform { namespace win {

#if ( qor_pp_unicode )

	qor_pp_export std::wstring to_tstring(const char* data)
	{
        const int size = MultiByteToWideChar(CP_UTF8, 0, data, -1, nullptr, 0);//TODO: route this call through a proper wrapper
        if (size == 0)
        {
            return std::wstring(L"");
        }
        std::wstring wide(static_cast<std::size_t>(size), 0);
        if (MultiByteToWideChar(CP_UTF8, 0, data, -1, wide.data(), size) == 0)//TODO: route this call through a proper wrapper
        {
            return std::wstring(L"");
        }
		return wide;
	}
#else
	qor_pp_export std::string to_tstring(char* source)
	{
		return std::string(source);
	}
#endif

}}}//qor::platform::win