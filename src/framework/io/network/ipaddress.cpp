// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/framework/parallel/thread/thread.h"
#include "ipaddress.h"

namespace qor{ namespace io { namespace network{

	std::string IPAddress::to_string() const
	{
		return is_ipv4() ? m_ipv4.to_string() : m_ipv6.to_string();
	}

	std::optional<IPAddress>
		IPAddress::from_string(std::string_view string) noexcept
	{
		if (auto ipv4 = IPv4Address::from_string(string); ipv4)
		{
			return *ipv4;
		}

		if (auto ipv6 = IPv6Address::from_string(string); ipv6)
		{
			return *ipv6;
		}

		return std::nullopt;
	}

}}}//qor::io::network
