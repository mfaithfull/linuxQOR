// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_NETWORK_IPADDRESS
#define QOR_PP_H_PLATFORM_NETWORK_IPADDRESS

#include <cassert>

#include "ipv4address.h"
#include "ipv6address.h"

namespace qor { namespace io { namespace network {

	class IPAddress
	{
	public:

		// Constructs to IPv4 address 0.0.0.0
		IPAddress() noexcept;

		IPAddress(IPv4Address address) noexcept;
		IPAddress(IPv6Address address) noexcept;

		bool is_ipv4() const noexcept { return m_family == family::ipv4; }
		bool is_ipv6() const noexcept { return m_family == family::ipv6; }

		const IPv4Address& to_ipv4() const;
		const IPv6Address& to_ipv6() const;

		const std::uint8_t* bytes() const noexcept;

		std::string to_string() const;

		static std::optional<IPAddress> from_string(std::string_view string) noexcept;

		bool operator==(const IPAddress& rhs) const noexcept;
		bool operator!=(const IPAddress& rhs) const noexcept;

		//  ipv4_address sorts less than ipv6_address
		bool operator<(const IPAddress& rhs) const noexcept;
		bool operator>(const IPAddress& rhs) const noexcept;
		bool operator<=(const IPAddress& rhs) const noexcept;
		bool operator>=(const IPAddress& rhs) const noexcept;

	private:

		enum class family
		{
			ipv4,
			ipv6
		};

		family m_family;

		union
		{
			IPv4Address m_ipv4;
			IPv6Address m_ipv6;
		};

	};

	inline IPAddress::IPAddress() noexcept
		: m_family(family::ipv4)
		, m_ipv4()
	{
	}

	inline IPAddress::IPAddress(IPv4Address address) noexcept
		: m_family(family::ipv4)
		, m_ipv4(address)
	{
	}

	inline IPAddress::IPAddress(IPv6Address address) noexcept
		: m_family(family::ipv6)
		, m_ipv6(address)
	{
	}

	inline const IPv4Address& IPAddress::to_ipv4() const
	{
		assert(is_ipv4());
		return m_ipv4;
	}

	inline const IPv6Address& IPAddress::to_ipv6() const
	{
		assert(is_ipv6());
		return m_ipv6;
	}

	inline const std::uint8_t* IPAddress::bytes() const noexcept
	{
		return is_ipv4() ? m_ipv4.bytes() : m_ipv6.bytes();
	}

	inline bool IPAddress::operator==(const IPAddress& rhs) const noexcept
	{
		if (is_ipv4())
		{
			return rhs.is_ipv4() && m_ipv4 == rhs.m_ipv4;
		}
		else
		{
			return rhs.is_ipv6() && m_ipv6 == rhs.m_ipv6;
		}
	}

	inline bool IPAddress::operator!=(const IPAddress& rhs) const noexcept
	{
		return !(*this == rhs);
	}

	inline bool IPAddress::operator<(const IPAddress& rhs) const noexcept
	{
		if (is_ipv4())
		{
			return !rhs.is_ipv4() || m_ipv4 < rhs.m_ipv4;
		}
		else
		{
			return rhs.is_ipv6() && m_ipv6 < rhs.m_ipv6;
		}
	}

	inline bool IPAddress::operator>(const IPAddress& rhs) const noexcept
	{
		return rhs < *this;
	}

	inline bool IPAddress::operator<=(const IPAddress& rhs) const noexcept
	{
		return !(rhs < *this);
	}

	inline bool IPAddress::operator>=(const IPAddress& rhs) const noexcept
	{
		return !(*this < rhs);
	}

}}}//qor::io::network

#endif//QOR_PP_H_PLATFORM_NETWORK_IPADDRESS
