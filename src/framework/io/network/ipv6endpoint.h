// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_NETWORK_IPV6ENDPOINT
#define QOR_PP_H_PLATFORM_NETWORK_IPV6ENDPOINT

#include "ipv6address.h"

namespace qor { namespace io { namespace network {

	class IPv6Endpoint
	{
	public:

		// Construct to [::]:0
		IPv6Endpoint() noexcept
			: m_address()
			, m_port(0)
		{ }

		explicit IPv6Endpoint(IPv6Address address, std::uint16_t port = 0) noexcept
			: m_address(address)
			, m_port(port)
		{ }

		const IPv6Address& address() const noexcept { return m_address; }

		std::uint16_t port() const noexcept { return m_port; }

		std::string to_string() const;

		static std::optional<IPv6Endpoint> from_string(std::string_view string) noexcept;

	private:

		IPv6Address m_address;
		std::uint16_t m_port;

	};

	inline bool operator==(const IPv6Endpoint& a, const IPv6Endpoint& b)
	{
		return a.address() == b.address() &&
			a.port() == b.port();
	}

	inline bool operator!=(const IPv6Endpoint& a, const IPv6Endpoint& b)
	{
		return !(a == b);
	}

	inline bool operator<(const IPv6Endpoint& a, const IPv6Endpoint& b)
	{
		return a.address() < b.address() ||
			(a.address() == b.address() && a.port() < b.port());
	}

	inline bool operator>(const IPv6Endpoint& a, const IPv6Endpoint& b)
	{
		return b < a;
	}

	inline bool operator<=(const IPv6Endpoint& a, const IPv6Endpoint& b)
	{
		return !(b < a);
	}

	inline bool operator>=(const IPv6Endpoint& a, const IPv6Endpoint& b)
	{
		return !(a < b);
	}

}}}//qor::io::network

#endif//QOR_PP_H_PLATFORM_NETWORK_IPV6ENDPOINT
