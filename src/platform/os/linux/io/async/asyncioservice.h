// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE
#define QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE

#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <coroutine>
#include "src/framework/io/async/service.h"
#include "iouringservice/iouringeventprocessor.h"
#include "iouringservice/readop.h"
#include "iouringservice/listenop.h"
#include "iouringservice/bindop.h"
#include "iouringservice/acceptop.h"
#include "src/framework/io/network/socket.h"
#include "src/framework/io/async/eventprocessor.h"

qor_pp_module_will_provide(QOR_LINUXASYNCIOSERVICE, Service)
qor_pp_module_will_provide(QOR_LINUXASYNCIOSERVICE, EventProcessor)
qor_pp_module_will_provide(QOR_LINUXASYNCIOSERVICE, Initiator)

namespace qor{ namespace io { namespace async{ namespace lin{

    class qor_pp_module_interface(QOR_LINUXASYNCIOSERVICE) Service : public qor::io::async::Service
    {
    public:

    };

}}}}//qor::io::async::lin

#endif//QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE
