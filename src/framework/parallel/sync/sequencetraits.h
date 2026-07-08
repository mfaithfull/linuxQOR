// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SYNC_SEQUENCETRAITS
#define QOR_PP_H_SYNC_SEQUENCETRAITS

#include <type_traits>

namespace qor{ namespace detail{

    template<typename SEQUENCE>
	struct sequence_of
	{
		using value_type = SEQUENCE;
		using difference_type = std::make_signed_t<SEQUENCE>;
		using size_type = std::make_unsigned_t<SEQUENCE>;

		static constexpr value_type initial_sequence = static_cast<value_type>(-1);

		static constexpr difference_type difference(value_type a, value_type b)
		{
			return static_cast<difference_type>(a - b);
		}

		static constexpr bool precedes(value_type a, value_type b)
		{
			return difference(a, b) < 0;
		}
	};

}}//qor::detail

#endif//QOR_PP_H_SYNC_SEQUENCETRAITS
