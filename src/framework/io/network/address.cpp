// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/framework/parallel/thread/thread.h"
#include "address.h"

#ifdef WIN32
#   include <winsock2.h>
#undef SetPort
#endif

#if(qor_pp_os_target == qor_pp_os_linux)
#   include <arpa/inet.h>
#endif

namespace qor{ namespace io { namespace network{

    Address::Address()
    {
        sa_family = 0;
        memset(sa.sa_data, 0, 30);
    }

    Address::Address(unsigned short AddressFamily) : Address()
    {
        sa_family = AddressFamily;
    }

    Address::Address(const Address& src) : 
        sa_family(src.sa_family),
        sa(src.sa)
    { }

    Address& Address::operator = (const Address& src)
    {
        if(&src != this)
        {
            sa_family = src.sa_family;
            sa = src.sa;
        }
        return *this;
    }

    Address::~Address() = default;

    void Address::SetIPV4Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
    {
        sa.IPAddress.sin_addr.S_un.b.s_b1 = a;
        sa.IPAddress.sin_addr.S_un.b.s_b2 = b;
        sa.IPAddress.sin_addr.S_un.b.s_b3 = c;
        sa.IPAddress.sin_addr.S_un.b.s_b4 = d;
    }

    std::string Address::GetIPV4Address() const
    {
        return std::format("{0}.{1}.{2}.{3}",
            sa.IPAddress.sin_addr.S_un.b.s_b1,
            sa.IPAddress.sin_addr.S_un.b.s_b2,
            sa.IPAddress.sin_addr.S_un.b.s_b3,
            sa.IPAddress.sin_addr.S_un.b.s_b4);
    }

    void Address::SetPort(unsigned short port)
    {
        sa.IPAddress.sin_port = ::htons(static_cast<uint16_t>(port));
    }

    unsigned short Address::GetPort() const
    {
        return ::ntohs(sa.IPAddress.sin_port);
    }

}}}//qor::io::network
