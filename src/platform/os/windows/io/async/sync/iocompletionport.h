// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCOMPLETIONPORT
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCOMPLETIONPORT

#include <cstdint>
#include "src/platform/os/windows/common/handles/handle.h"
#include "overlapped.h"

namespace qor { namespace io{
    struct Descriptor;
}}//qor::io

namespace qor { namespace io { namespace async { namespace win {

	class IOCompletionPort
    {
    public:

        IOCompletionPort(std::uint32_t concurrenyHint);
        ~IOCompletionPort();

        const platform::win::Handle& Handle();
        bool PostQueuedCompletionStatus(unsigned long numberOfBytesTransferred, unsigned long long completionKey, overlapped* pOverlapped);
        bool GetQueuedCompletionStatus(unsigned long& numberOfBytesTransferred, unsigned long long& completionKey, overlapped*& pOverlapped, unsigned long timeoutms);
        bool Enroll(io::Descriptor& ioDescriptor) const;

    private:

        platform::win::Handle m_handle;
    };

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCOMPLETIONPORT
