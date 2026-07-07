// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <algorithm>

#include "src/framework/parallel/thread/thread.h"
#include "ipv6endpoint.h"

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

	std::string IPv6Endpoint::to_string() const
	{
		std::string result;
		result.push_back('[');
		result += m_address.to_string();
		result += "]:";
		result += std::to_string(m_port);
		return result;
	}

	std::optional<IPv6Endpoint>
	IPv6Endpoint::from_string(std::string_view string) noexcept
	{
		// Shortest valid endpoint is "[::]:0"
		if (string.size() < 6)
		{
			return std::nullopt;
		}

		if (string[0] != '[')
		{
			return std::nullopt;
		}

		auto closeBracketPos = string.find("]:", 1);
		if (closeBracketPos == std::string_view::npos)
		{
			return std::nullopt;
		}

		auto address = IPv6Address::from_string(string.substr(1, closeBracketPos - 1));
		if (!address)
		{
			return std::nullopt;
		}

		auto port = local::parse_port(string.substr(closeBracketPos + 2));
		if (!port)
		{
			return std::nullopt;
		}

		return IPv6Endpoint{ *address, *port };
	}

}}}//qor::io::network
