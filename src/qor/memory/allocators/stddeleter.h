// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_STDDELETER
#define QOR_PP_H_STDDELETER

#include <memory>
#include "../memory.h"

namespace qor{

	template <class _Ty>
	struct qor_std_deleter
	{
		constexpr qor_std_deleter() noexcept = default;

		template <class _Ty2, std::enable_if_t<std::is_convertible_v<_Ty2*, _Ty*>, int> = 0>
		qor_std_deleter(const qor_std_deleter<_Ty2>&) noexcept {}

		void operator()(const _Ty* _Ptr) const noexcept /* strengthened */ 
		{
			static_assert(0 < sizeof(_Ty), "can't delete an incomplete type");
			source_of<_Ty>::type::Free( reinterpret_cast<byte*>(const_cast<std::remove_cv<_Ty>::type*>(_Ptr)), sizeof(_Ty) );
		}
	};

	template <class _Ty>
	struct qor_std_deleter<_Ty[]> { // default deleter for unique_ptr to array of unknown size
		constexpr qor_std_deleter() noexcept = default;

		template <class _Uty, std::enable_if_t<std::is_convertible_v<_Uty(*)[], _Ty(*)[]>, int> = 0>
		qor_std_deleter(const qor_std_deleter<_Uty[]>&) noexcept {}

		template <class _Uty, std::enable_if_t<std::is_convertible_v<_Uty(*)[], _Ty(*)[]>, int> = 0>
		void operator()(_Uty* _Ptr) const noexcept /* strengthened */ 
		{
			static_assert(0 < sizeof(_Uty), "can't delete an incomplete type");
			source_of<_Uty>::type::Free(reinterpret_cast<byte*>(_Ptr), sizeof(_Uty));
		}
	};

}//qor

#endif//QOR_PP_H_STDDELETER
