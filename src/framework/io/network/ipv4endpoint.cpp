// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/framework/parallel/thread/thread.h"
#include "ipv4endpoint.h"

namespace{ namespace local{

	bool is_digit(char c)
	{
		return c >= '0' && c <= '9';
	}

	std::uint8_t digit_value(char c)
	{
		return static_cast<std::uint8_t>(c - '0');
	}

	std::optional<std::uint16_t> parse_port(std::string_view string)
	{
		if (string.empty()) return std::nullopt;

		std::uint32_t value = 0;
		for (auto c : string)
		{
			if (!is_digit(c)) return std::nullopt;
			value = value * 10 + digit_value(c);
			if (value > 0xFFFFu) return std::nullopt;
		}

		return static_cast<std::uint16_t>(value);
	}

}}//::local

namespace qor{ namespace io { namespace network{

	std::string IPv4Endpoint::to_string() const
	{
		auto s = m_address.to_string();
		s.push_back(':');
		s.append(std::to_string(m_port));
		return s;
	}

	std::optional<IPv4Endpoint>
		IPv4Endpoint::from_string(std::string_view string) noexcept
	{
		auto colonPos = string.find(':');
		if (colonPos == std::string_view::npos)
		{
			return std::nullopt;
		}

		auto address = IPv4Address::from_string(string.substr(0, colonPos));
		if (!address)
		{
			return std::nullopt;
		}

		auto port = local::parse_port(string.substr(colonPos + 1));
		if (!port)
		{
			return std::nullopt;
		}

		return IPv4Endpoint{ *address, *port };
	}

}}}//qor::io::network
