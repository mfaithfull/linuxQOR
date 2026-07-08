// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_PLATFORM_IO_DESC
#define QOR_PP_H_OS_PLATFORM_IO_DESC

#include "src/qor/essentials/datastructures/guid.h"

namespace qor{ namespace io{

    struct Descriptor
    {
        union 
        {
            int m_fd;
            void* m_handle;
            unsigned long long m_socket;
        };
        const GUID* m_objectType;        
    };
}}//qor::io

#endif//QOR_PP_H_OS_PLATFORM_IO_DESC

