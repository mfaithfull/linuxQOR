// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_EVENTWAITER
#define QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE_EVENTWAITER

#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <coroutine>
#include "iouringeventprocessor.h"
#include "src/framework/io/async/initiator.h"
#include "readop.h"
#include "writeop.h"
#include "listenop.h"
#include "bindop.h"
#include "acceptop.h"
#include "sendop.h"
#include "shutdownop.h"
#include "recvop.h"
#include "src/framework/io/network/socket.h"
#include "src/qor/flyers/log/informative.h"

namespace qor{ namespace io{ namespace async{ namespace lin{

    class qor_pp_module_interface(QOR_LINUXASYNCIOSERVICE) IOUringInitiator : public qor::io::async::Initiator
    {
    public:

        IOUringInitiator() = default;
        virtual ~IOUringInitiator() noexcept = default;

        virtual void ConnectToProcessor(qor::io::async::EventProcessor* processor);

        virtual qor::io::async::IOTask Send(io::Descriptor* ioDescriptor, const byte* buffer, size_t len, int flags)
        {
            m_Ring->m_guard.lock();
            return Send2(ioDescriptor, buffer, len, flags);
        }

        qor::io::async::IOTask Send2(io::Descriptor* ioDescriptor, const byte* buffer, size_t len, int flags)
        {
            int result = co_await SendOperation(*m_Ring, ioDescriptor->m_fd, buffer, len, flags);

            co_return qor::io::async::Result{
                .status_code = result,
                .ioObject = ioDescriptor
            };
        }

        virtual qor::io::async::IOTask Recv(io::Descriptor* ioDescriptor, byte* buffer, size_t len)
        {
            m_Ring->m_guard.lock();
            return Recv2(ioDescriptor, buffer, len);
        }

        qor::io::async::IOTask Recv2(io::Descriptor* ioDescriptor, byte* buffer, size_t len)
        {
            int result = co_await RecvOperation(*m_Ring, ioDescriptor->m_fd, buffer, len);

            co_return qor::io::async::Result{
                .status_code = result,
                .ioObject = ioDescriptor
            };
        }

        virtual qor::io::async::IOTask Read(io::Descriptor* ioDescriptor, byte* buffer, size_t len, long offset)
        {
            m_Ring->m_guard.lock();//we must grab the ring to submit to it. The awaiter will unlock before suspending
            co_return qor::io::async::Result{
                .status_code = co_await ReadOperation(*m_Ring, ioDescriptor->m_fd, buffer, len, offset),
                .ioObject = ioDescriptor
            };
        }

        virtual qor::io::async::IOTask Write(io::Descriptor* ioDescriptor, byte* buffer, size_t len, long offset)
        {
            m_Ring->m_guard.lock();//we must grab the ring to submit to it. The awaiter will unlock before suspending
            co_return qor::io::async::Result{
                .status_code = co_await WriteOperation(*m_Ring, ioDescriptor->m_fd, buffer, len, offset),
                .ioObject = ioDescriptor
            };
        }

        virtual qor::io::async::IOTask Listen(io::Descriptor* ioDescriptor, int backlog)
        {
            m_Ring->m_guard.lock();//we must grab the ring to submit to it. The awaiter will unlock before suspending
            co_return qor::io::async::Result{
                .status_code = co_await ListenOperation(*m_Ring, ioDescriptor->m_fd, backlog),
                .ioObject = ioDescriptor
            };
        }

        virtual qor::io::async::IOTask Bind(io::Descriptor* ioDescriptor, const io::network::Address& Address)
        {
            sockaddr_in addr;
            memset(&addr, 0, sizeof(struct sockaddr_in));
            addr.sin_family = Address.sa_family;
            addr.sin_addr.s_addr = Address.sa.IPAddress.sin_addr.S_un.S_addr;
            addr.sin_port = Address.sa.IPAddress.sin_port;
            m_Ring->m_guard.lock();//we must grab the ring to submit to it. The awaiter will unlock before suspending
            co_return qor::io::async::Result{
                .status_code = co_await BindOperation(*m_Ring, ioDescriptor->m_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)),
                .ioObject = ioDescriptor
            };
        }

        virtual qor::io::async::IOTask Accept(io::Descriptor* ioDescriptor, const io::network::Address& /*Address*/, io::network::Socket* new_socket)
        {
            sockaddr addr;
            socklen_t len = 0;
            m_Ring->m_guard.lock();//we must grab the ring to submit to it. The awaiter will unlock before suspending
            int socket_number = co_await AcceptOperation(*m_Ring, ioDescriptor->m_fd, &addr, &len, 0);

            new_socket->m_fd = socket_number;
            co_return qor::io::async::Result{
                .status_code = socket_number,
                .ioObject = ioDescriptor
            };
        }

        virtual qor::io::async::IOTask Shutdown(io::Descriptor* ioDescriptor, int how)
        {
            m_Ring->m_guard.lock();//we must grab the ring to submit to it. The awaiter will unlock before suspending
            return Shutdown2(ioDescriptor, how);
        }

        qor::io::async::IOTask Shutdown2(io::Descriptor* ioDescriptor, int how)
        {
            co_await ShutdownOperation(*m_Ring, ioDescriptor->m_fd, how);
            co_return qor::io::async::Result{
                .status_code = 0,
                .ioObject = ioDescriptor
            };
        }

    protected:

        IOUring* m_Ring{nullptr};
    };

}}}}//qor::io::async::lin

#endif//QOR_PP_H_OS_LINUX_FRAMEWORK_ASYNCIOSERVICE
