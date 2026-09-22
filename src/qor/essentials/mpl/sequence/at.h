// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MPL_SEQUENCE_AT
#define QOR_PP_H_MPL_SEQUENCE_AT

#include "../functional/call.h"
#include "drop.h"
#include "front.h"
#include "../int.h"

namespace qor { namespace mpl {

		template <typename N, typename C = identity>
		using at = drop<N, front<C>>;

		template <typename C = identity>
		using at0            = drop<uint_<0>, front<C>>;

		template <typename C = identity>
		using at1            = drop<uint_<1>, front<C>>;

		template <typename C = identity>
		using at2            = drop<uint_<2>, front<C>>;

		template <typename C = identity>
		using at3            = drop<uint_<3>, front<C>>;

		template <typename C = identity>
		using at4            = drop<uint_<4>, front<C>>;

		template <typename C = identity>
		using at5            = drop<uint_<5>, front<C>>;

		template <typename C = identity>
		using at6            = drop<uint_<6>, front<C>>;

		template <typename C = identity>
		using at7            = drop<uint_<7>, front<C>>;

		template <typename C = identity>
		using at8            = drop<uint_<8>, front<C>>;

		template <typename C = identity>
		using at9            = drop<uint_<9>, front<C>>;

		namespace eager {
			// get the n-th element of the list
			template <typename List, unsigned N>
			using at = call<unpack<mpl::at<mpl::uint_<N>>>, List>;
		}//eager

}}//qor::mpl

#endif.//QOR_PP_H_MPL_SEQUENCE_AT
