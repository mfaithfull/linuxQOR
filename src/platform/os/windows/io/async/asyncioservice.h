// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE

#include <vector>
#include <coroutine>
#include "src/framework/io/async/service.h"
#include "src/framework/io/async/eventprocessor.h"

qor_pp_module_will_provide(QOR_WINDOWSASYNCIOSERVICE, AsyncIOService)
qor_pp_module_will_provide(QOR_WINDOWSASYNCIOSERVICE, AsyncIOEventProcessor)
qor_pp_module_will_provide(QOR_WINDOWSASYNCIOSERVICE, AsyncIOInitiator)

namespace qor{ namespace io{ namespace async{ namespace win{

    class qor_pp_module_interface(QOR_WINDOWSASYNCIOSERVICE) Service : public qor::io::async::Service
    {
    public:

    };

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE
