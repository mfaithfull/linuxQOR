// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_ASYNCIOTYPES
#define QOR_PP_H_FRAMEWORK_ASYNCIOTYPES

#include <coroutine>
#include "src/framework/parallel/task/task.h"
#include "src/framework/io/iodescriptor.h"

namespace qor { namespace io{ namespace async{

    struct AsyncIOResult
    {
        union
        {
            int status_code;
            unsigned long long result{ 0 };
        };
        io::IODescriptor* ioObject;
    };

    using IOTask = task<AsyncIOResult>;

    struct AsyncIORequest
    {
        std::coroutine_handle<> handle;
        int statusCode{-1};
    };

}}}//qor::io::async

#endif//QOR_PP_H_FRAMEWORK_ASYNCIOTYPES
