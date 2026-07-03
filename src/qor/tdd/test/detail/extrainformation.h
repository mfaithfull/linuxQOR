// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TESTDETAIL_EXTRAINFORMATION
#define QOR_PP_H_TESTDETAIL_EXTRAINFORMATION

#include "src/platform/compiler/compiler.h"

namespace qor{ namespace test { namespace detail {

	template <typename Expected, typename Actual, bool has_extra = false>
	struct extra_information
	{
		static void print(std::ostringstream& /*os*/, const Expected& /*e*/, const Actual& /*a*/)
		{
			//TODO: ?
		}
	};

	template <typename Expected, typename Actual>
	struct extra_information<Expected, Actual, true>
	{
		static void print(std::ostringstream& os, const Expected& e, const Actual& a)
		{
			os << "\nexpected: " << "(" << ::qor::compiler::demangle<Expected>() << ") " << e
				<< " != actual: " << "(" << ::qor::compiler::demangle<Actual>() << ") " << a;
		}
	};

}}}//qor::test::detail

#endif//QOR_PP_H_TESTDETAIL_EXTRAINFORMATION