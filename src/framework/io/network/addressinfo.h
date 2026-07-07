// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_NETWORK_ADDRESSINFO
#define QOR_PP_H_PLATFORM_NETWORK_ADDRESSINFO

#include "address.h"
#include "socketdefs.h"

namespace qor{ namespace io { namespace network{

    struct addrinfo_flags
    {
        addrinfo_flags() : value(0){}
        addrinfo_flags(int v) : value(v){}
        addrinfo_flags(addrinfo_flags&& src) noexcept : value(std::move(src.value)){}
        addrinfo_flags(const addrinfo_flags& src) : value(src.value){}

        addrinfo_flags& operator =(const addrinfo_flags& src)
        {
            value = src.value;
            return *this;
        }

        operator int()
        {
            return value;
        }

        int value;
        enum eBits
        {
            Passive         = 1 << 0,
            CannonName      = 1 << 1,
            NumericHost     = 1 << 2,
            V4Mapped        = 1 << 3,
            All             = 1 << 4,
            AddrConfig      = 1 << 5,
            IDN             = 1 << 6,
            CanonIDN        = 1 << 7,
            NumericServ     = 1 << 8,
        };

        bool SocketAddressIsIntentedForBind() const
        {
            return value & Passive;
        }

        void SocketAddressIsIntendedForBind(bool intendedForBind)
        {
            intendedForBind ? value |= Passive : value &= ~Passive;
        }

        bool RequestForCanonicalName() const
        {
            return value & CannonName;
        }

        void RequestForCanonicalName(bool requestForCanonicalName)
        {
            requestForCanonicalName ? value |= CannonName : value &= ~CannonName;
        }

        bool DontUseHostNameResolution() const
        {
            return value & NumericHost;
        }

        void DontUseHostNameResolution(bool dontUseNameResollution)
        {
            dontUseNameResollution ? value |= NumericHost : value &= ~NumericHost;
        }

        bool IPV4MappedAddressesAreAcceptable() const
        {
            return value & V4Mapped;
        }

        void IPV4MappedAddressesAreAcceptable(bool ipv4MappedAddressesAreAcceptable)
        {
            ipv4MappedAddressesAreAcceptable ? value |= V4Mapped : value &= ~V4Mapped;
        }

        bool ReturnIP46and6() const
        {
            return value & All;
        }

        void ReturnIP46and6(bool returnipv4and6)
        {
            returnipv4and6 ? value |= All : value &= ~All;
        }

        bool UseConfigurationOfHost() const
        {
            return value & AddrConfig;
        }

        void UseConfigurationOfHost(bool useConfigurationOfHost)
        {
            useConfigurationOfHost ? value |= AddrConfig : value &= ~AddrConfig;
        }

        bool IDNEncodeInputFromCurrentLocale() const
        {
            return value & IDN;
        }

        void IDNEncodeInputFromCurrentLocale(bool idnEncodeInputFromCurrentLocale)
        {
            idnEncodeInputFromCurrentLocale ? value |= IDN : value &= ~IDN;
        }

        bool TranslateCanonicalNameFromIDN() const
        {
            return value & CanonIDN;
        }

        void TranslateCanonicalNameFromIDN(bool translateCanonincalNameFromIDN)
        {
            translateCanonincalNameFromIDN ? value |= CanonIDN : value &= ~CanonIDN;
        }

        bool DontUseServiceNameResolution() const
        {
            return value & NumericServ;
        }

        void DontUseServiceNameResolution(bool dontUseServiceNameResolution)
        {
            dontUseServiceNameResolution ? value |= NumericServ : value &= ~NumericServ;
        }
    };

    class AddressInfo
    {
    public:

        addrinfo_flags flags;                  //Input flags.
        sockets::eAddressFamily family;
        sockets::eType socktype;
        sockets::eProtocol protocol;
        Address address;
        std::string canonname;      //Canonical name for service location.
    };

}}}//qor::io::network

#endif//QOR_PP_H_PLATFORM_NETWORK_ADDRESSINFO
