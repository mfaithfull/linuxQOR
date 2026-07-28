// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/essentials/objectcontext/anyobject.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "pipe.h"
#include "src/framework/io/filesystem/ifilesystem.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <aio.h>

//Export this trivial function so the linker will pull in this library to fulfil the ImplementsPipe requirement.
namespace qor{ bool qor_pp_module_interface(QOR_LINUXFILESYSTEM) ImplementsPipe() { return true; } }//qor

namespace qor{ namespace io{ namespace lin{

    Pipe::Pipe() : io::Pipe(-1){}

    Pipe::Pipe(int fd) : io::Pipe(fd){}

    Pipe::Pipe(const Pipe& src) : io::Pipe()
    {
        if(src.m_fd != -1)
        {
            Descriptor::m_fd = fcntl(src.m_fd, F_DUPFD, 0);
        }
    }

    Pipe::Pipe(const io::filesystem::Index& direntry, const network::sockets::eType& Type, const network::sockets::eProtocol& Protocol) : io::Pipe()
    {
        Descriptor::m_fd = -1;

        int domain = AF_UNIX;
        int type = TypeToLinux(Type, true);
        int protocol = ProtocolToLinux(Protocol);

        m_fd = ::socket(domain, type, protocol);
    }

    Pipe::~Pipe()
    {
        if(m_fd != -1)
        {
            ::fsync(m_fd);
            ::close(m_fd);
        }
    }

    int32_t Pipe::Bind(const qor::io::async::Interface& ioContext, const network::Address& Address)
    {
        return sync_wait(ioContext.Bind(this, Address));
    }

    int32_t Pipe::Listen(const qor::io::async::Interface& ioContext, int32_t backlog)
    {
        return sync_wait(ioContext.Listen(this, backlog));
    }

    task<int32_t> Pipe::AcceptAsync(const qor::io::async::Interface& ioContext, network::Address& Address, network::Socket* Socket)
    {
        return ioContext.Accept(this, Address, Socket);
    }

    int32_t Pipe::Bind(const network::Address& Address)
    {
        sockaddr_un addr;
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, Address.sa.UnixAddress.sun_path, sizeof(addr.sun_path)-1);
        return ::bind(m_fd, (struct sockaddr*)&addr, sizeof(addr));
    }

    int32_t Pipe::Listen(int32_t iBacklog)
    {
        return ::listen(m_fd, iBacklog);
    }

    ref_of<io::Pipe>::type Pipe::Accept(io::network::Address& /*Address*/)
    {
        ref_of<io::Pipe>::type newpipe;
        sockaddr addr;
        socklen_t len = 0;
        int iresult = ::accept(m_fd, &addr, &len);
        if(iresult == -1)
        {
            //TODO:Raise error
        }
        else
        {
            newpipe = new_ref<Pipe>(iresult).AsRef<io::Pipe>();
        }
        return newpipe;
    }

    int32_t Pipe::Connect(const network::Address& Address)
    {
        return -1;// ::connect(m_fd, (const sockaddr*)&addr, len);
    }

    int32_t Pipe::GetPeerName(network::Address& /*Address*/)
    {
        sockaddr addr;
        socklen_t len;
        return ::getpeername(m_fd, &addr, &len);
    }

    int32_t Pipe::GetSockName(network::Address& /*Address*/)
    {
        sockaddr addr;
        socklen_t len;
        return ::getsockname(m_fd, &addr, &len);
    }

    int32_t Pipe::GetSockOpt(int32_t level, int32_t optname, char* optval, int32_t* len)
    {
        return ::getsockopt(m_fd, level, optname, optval, (socklen_t*)&len);
    }

    int32_t Pipe::SetSockOpt(int32_t level, int32_t optname, const char* optval, int32_t optlen)
    {
        return ::setsockopt(m_fd, level, optname, optval, optlen);
    }

    task<int32_t> Pipe::AsyncReceive(const qor::io::async::Interface& ioContext, char* pBuffer, int32_t iLen)
    {
        return ioContext.Recv(this, (byte*)pBuffer, iLen);
    }

    int32_t Pipe::Receive(char* buf, int32_t len, int32_t flags)
    {
        return ::recv(m_fd, buf, len, flags);
    }

    int32_t Pipe::Peek(char* buf, int32_t len)
    {
        return ::recv(m_fd, buf, len, MSG_PEEK);
    }

    int32_t Pipe::ReceiveFrom(char* Buffer, int32_t iLen, int32_t iFlags, network::Address& From)
    {
        sockaddr_in addr;
        addr.sin_family = From.sa_family;
        addr.sin_addr.s_addr = From.sa.IPAddress.sin_addr.S_un.S_addr;
        addr.sin_port = From.sa.IPAddress.sin_port;
        socklen_t socklen = sizeof(addr);
        return ::recvfrom(m_fd, Buffer, iLen, iFlags, (sockaddr*)&addr, &socklen);
    }

    task<int32_t> Pipe::AsyncSend(const qor::io::async::Interface& ioContext, const char* Buffer, int32_t iLen)
    {
        return ioContext.Send(this, (byte*)Buffer, iLen, 0);
    }

    int32_t Pipe::Send(const char* Buffer, int32_t iLen)
    {
        return ::send(m_fd, Buffer, iLen, 0);
    }

    int32_t Pipe::SendTo(const char* Buffer, int32_t len, int32_t flags, const network::Address& To)
    {
        sockaddr_in addr;
        addr.sin_family = To.sa_family;
        addr.sin_addr.s_addr = To.sa.IPAddress.sin_addr.S_un.S_addr;
        addr.sin_port = To.sa.IPAddress.sin_port;
        return ::sendto(m_fd, Buffer, len, flags, (sockaddr*)&addr, len);
    }

    int32_t Pipe::Shutdown(network::sockets::eShutdown how)
    {
        int iHow = 0;
        iHow = ( how & network::sockets::eShutdown::ShutdownRead ) ? SHUT_RD : iHow;
        iHow = ( how & network::sockets::eShutdown::ShutdownWrite ) ? SHUT_WR : iHow;
        iHow = ( how & network::sockets::eShutdown::ShutdownReadWrite ) ? SHUT_RDWR : iHow;
        return ::shutdown(m_fd, iHow);
    }

    task<int32_t> Pipe::AsyncShutdown(const qor::io::async::Interface& ioContext,  network::sockets::eShutdown how)
    {
        int iHow = 0;
        iHow = ( how & network::sockets::eShutdown::ShutdownRead ) ? SHUT_RD : iHow;
        iHow = ( how & network::sockets::eShutdown::ShutdownWrite ) ? SHUT_WR : iHow;
        iHow = ( how & network::sockets::eShutdown::ShutdownReadWrite ) ? SHUT_RDWR : iHow;
        return ioContext.Shutdown(this, iHow);
    }

    std::size_t Pipe::ID(void)
    {
        return m_fd;
    }

    int32_t Pipe::GetLastError(void)
    {
        return -1;
    }

    bool Pipe::SetNonBlocking(bool nonBlocking)
    {
        auto flags = ::fcntl(m_fd, F_GETFL, 0);
        return fcntl(m_fd, F_SETFL, nonBlocking ? (flags | O_NONBLOCK) : (flags & (~O_NONBLOCK))) == 0 ? true : false;
    }

}}}//qor::io::lin