// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_NETWORK_IPENDPOINT
#define QOR_PP_H_PLATFORM_NETWORK_IPENDPOINT

#include <cassert>
#include <optional>
#include <string>

#include "ipaddress.h"
#include "ipv4endpoint.h"
#include "ipv6endpoint.h"

namespace qor { namespace io { namespace network {

	class IPEndpoint
	{
	public:

		// Constructs to IPv4 end-point 0.0.0.0:0
		IPEndpoint() noexcept;

		IPEndpoint(IPv4Endpoint endpoint) noexcept;
		IPEndpoint(IPv6Endpoint endpoint) noexcept;

		bool is_ipv4() const noexcept { return m_family == family::ipv4; }
		bool is_ipv6() const noexcept { return m_family == family::ipv6; }

		const IPv4Endpoint& to_ipv4() const;
		const IPv6Endpoint& to_ipv6() const;

		IPAddress address() const noexcept;
		std::uint16_t port() const noexcept;

		std::string to_string() const;

		static std::optional<IPEndpoint> from_string(std::string_view string) noexcept;

		bool operator==(const IPEndpoint& rhs) const noexcept;
		bool operator!=(const IPEndpoint& rhs) const noexcept;

		//  ipv4_endpoint sorts less than ipv6_endpoint
		bool operator<(const IPEndpoint& rhs) const noexcept;
		bool operator>(const IPEndpoint& rhs) const noexcept;
		bool operator<=(const IPEndpoint& rhs) const noexcept;
		bool operator>=(const IPEndpoint& rhs) const noexcept;

	private:

		enum class family
		{
			ipv4,
			ipv6
		};

		family m_family;

		union
		{
			IPv4Endpoint m_ipv4;
			IPv6Endpoint m_ipv6;
		};

	};

	inline IPEndpoint::IPEndpoint() noexcept
		: m_family(family::ipv4)
		, m_ipv4()
	{
	}

	inline IPEndpoint::IPEndpoint(IPv4Endpoint endpoint) noexcept
		: m_family(family::ipv4)
		, m_ipv4(endpoint)
	{
	}

	inline IPEndpoint::IPEndpoint(IPv6Endpoint endpoint) noexcept
		: m_family(family::ipv6)
		, m_ipv6(endpoint)
	{
	}

	inline const IPv4Endpoint& IPEndpoint::to_ipv4() const
	{
		assert(is_ipv4());
		return m_ipv4;
	}

	inline const IPv6Endpoint& IPEndpoint::to_ipv6() const
	{
		assert(is_ipv6());
		return m_ipv6;
	}

	inline IPAddress IPEndpoint::address() const noexcept
	{
		if (is_ipv4())
		{
			return m_ipv4.address();
		}
		else
		{
			return m_ipv6.address();
		}
	}

	inline std::uint16_t IPEndpoint::port() const noexcept
	{
		return is_ipv4() ? m_ipv4.port() : m_ipv6.port();
	}

	inline bool IPEndpoint::operator==(const IPEndpoint& rhs) const noexcept
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

	inline bool IPEndpoint::operator!=(const IPEndpoint& rhs) const noexcept
	{
		return !(*this == rhs);
	}

	inline bool IPEndpoint::operator<(const IPEndpoint& rhs) const noexcept
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

	inline bool IPEndpoint::operator>(const IPEndpoint& rhs) const noexcept
	{
		return rhs < *this;
	}

	inline bool IPEndpoint::operator<=(const IPEndpoint& rhs) const noexcept
	{
		return !(rhs < *this);
	}

	inline bool IPEndpoint::operator>=(const IPEndpoint& rhs) const noexcept
	{
		return !(*this < rhs);
	}

}}}//qor::io::network

#endif//QOR_PP_H_PLATFORM_NETWORK_IPENDPOINT
