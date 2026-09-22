// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_INT
#define QOR_PP_H_MPL_INT

namespace qor { namespace mpl {

		template <unsigned long long Value>
		struct uint_ 
        {
			static constexpr unsigned long long value = Value;
		};

		template <long long Value>
		struct int_ 
        {
			static constexpr long long value = Value;
		};

}}//qor::mpl

#endif//QOR_PP_H_MPL_INT
