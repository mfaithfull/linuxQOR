// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_AUTORESETEVENT
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_AUTORESETEVENT

#include "src/platform/os/windows/common/handles/handle.h"

#include <cstdint>
#include "overlapped.h"

namespace qor { namespace io { namespace async { namespace win {

	class AutoResetEvent final
    {
    public:

        AutoResetEvent();
        ~AutoResetEvent();

        const platform::win::Handle& Handle();

        bool Set();

    private:

        platform::win::Handle m_handle;
    };

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_AUTORESETEVENT
