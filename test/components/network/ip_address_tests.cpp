///////////////////////////////////////////////////////////////////////////////
// Copyright (c) Lewis Baker
// Licenced under MIT license. See LICENSE.txt for details.
///////////////////////////////////////////////////////////////////////////////

#include <cppcoro/net/ip_address.hpp>

#include "doctest/doctest.h"

TEST_SUITE_BEGIN("ip_address");

using net::ip_address;
using net::ipv4_address;
using net::ipv6_address;

TEST_CASE("default constructor")
{
	ip_address x;
	qor_pp_assert_that(x.is_ipv4());
	qor_pp_assert_that(x.to_ipv4() == ipv4_address{});
}

TEST_CASE("to_string")
{
	ip_address a = ipv6_address{ 0xAABBCCDD00112233, 0x0102030405060708 };
	ip_address b = ipv4_address{ 192, 168, 0, 1 };

	qor_pp_assert_that(a.to_string() == "aabb:ccdd:11:2233:102:304:506:708");
	qor_pp_assert_that(b.to_string() == "192.168.0.1");
}

TEST_CASE("from_string")
{
	qor_pp_assert_that(ip_address::from_string("") == std::nullopt);
	qor_pp_assert_that(ip_address::from_string("foo") == std::nullopt);
	qor_pp_assert_that(ip_address::from_string(" 192.168.0.1") == std::nullopt);
	qor_pp_assert_that(ip_address::from_string("192.168.0.1asdf") == std::nullopt);

	qor_pp_assert_that(ip_address::from_string("192.168.0.1") == ipv4_address(192, 168, 0, 1));
	qor_pp_assert_that(ip_address::from_string("::192.168.0.1") == ipv6_address(0, 0, 0, 0, 0, 0, 0xc0a8, 0x1));
	qor_pp_assert_that(ip_address::from_string("aabb:ccdd:11:2233:102:304:506:708") ==
		ipv6_address{ 0xAABBCCDD00112233, 0x0102030405060708 });
}

TEST_SUITE_END();
