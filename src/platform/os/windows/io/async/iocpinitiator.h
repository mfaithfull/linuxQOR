// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCPINITIATOR
#define QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCPINITIATOR

#include <vector>
#include <coroutine>
#include <WinSock2.h>
#undef SetPort
#undef min
#undef max
#include "iocpeventprocessor.h"
#include "src/framework/io/async/initiator.h"
#include "operations/recvop.h"
#include "operations/acceptop.h"
#include "operations/sendop.h"
#include "operations/readop.h"
#include "operations/writeop.h"
#include "src/framework/io/network/socket.h"

namespace qor { namespace io { namespace async { namespace win {

    class IOCPInitiator : public qor::io::async::Initiator
    {
    public:

        inline IOCPInitiator() = default;
        inline virtual ~IOCPInitiator() noexcept = default;

        inline virtual void ConnectToProcessor(qor::io::async::EventProcessor * processor) override
        {
            m_eventProcessor = dynamic_cast<IOCPEventProcessor*>(processor);
        }

        inline virtual bool RequiresBackgroundProcessor() override
        {
            return true;
        }

        inline virtual qor::io::async::IOTask Send(io::Descriptor * ioDescriptor, const byte * buffer, size_t len, int flags) override
        {
            co_return qor::io::async::Result{
                .result = co_await SocketSendOperation(ioDescriptor, buffer, len),
                .ioObject = ioDescriptor
            };
        }

        inline virtual qor::io::async::IOTask Read(io::Descriptor* ioDescriptor, byte* buffer, size_t len, long offset) override
        {
            co_return qor::io::async::Result{
                .result = co_await ReadOperation(ioDescriptor, buffer, len, offset),
                .ioObject = ioDescriptor
            };
        }

        inline virtual qor::io::async::IOTask Write(io::Descriptor* ioDescriptor, byte* buffer, size_t len, long offset) override
        {
            co_return qor::io::async::Result{
                .result = co_await WriteOperation(ioDescriptor, buffer, len, offset),
                .ioObject = ioDescriptor
            };
        }

        inline virtual qor::io::async::IOTask Recv(io::Descriptor* ioDescriptor, byte* buffer, size_t len) override
        {
            co_return qor::io::async::Result{
                .result = co_await SocketRecvOperation(ioDescriptor, buffer, len),
                .ioObject = ioDescriptor
            };
        }

        inline virtual qor::io::async::IOTask Listen(io::Descriptor * ioDescriptor, int backlog) override
        {
            co_return qor::io::async::Result{
                .status_code = -1,//co_await ListenOperation(*m_Ring, ioDescriptor->m_fd, 0),
                .ioObject = ioDescriptor
            };
        }

        inline virtual qor::io::async::IOTask Bind(io::Descriptor * ioDescriptor, const io::network::Address & Address) override
        {
            //Windows doesn't provide async bind
            co_return qor::io::async::Result{
                .status_code = -1,//co_await BindOperation(*m_Ring, ioDescriptor->m_fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)),
                .ioObject = ioDescriptor
            };
        }

        inline virtual qor::io::async::IOTask Accept(io::Descriptor * ioDescriptor, const io::network::Address & Address, io::network::Socket * new_socket) override
        {
            int status = co_await SocketAcceptOperation(ioDescriptor, new_socket);

            co_return qor::io::async::Result{
                .status_code = status,
                .ioObject = ioDescriptor
            };
        }

    protected:

        IOCPEventProcessor* m_eventProcessor;
    };

}}}}//qor::io::async::win

#endif//QOR_PP_H_OS_WINDOWS_FRAMEWORK_ASYNCIOSERVICE_IOCPINITIATOR
