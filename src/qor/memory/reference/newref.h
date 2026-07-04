// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_REFERENCE_NEW
#define QOR_PP_H_REFERENCE_NEW

#include "src/platform/compiler/compiler.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/instance/instance.h"
#include "src/qor/memory/instance/default.h"

namespace qor{

	template< typename T >
	qor_pp_forceinline ref_of<T>::type new_ref()
	{
		return instancer_of<T>::type::template Instance<T>(1);
	}

	template< typename T, typename... _p >
	qor_pp_forceinline ref_of<T>::type new_ref(_p&&... p1)
	{
		constexpr size_t default_count = 1;
		return instancer_of<T>::type::template Instance<T>(default_count, std::forward<_p&&>(p1)...);
	}

	template< typename T >
	qor_pp_forceinline ref_of<T>::type new_array_ref(size_t count)
	{
		return instancer_of<T>::type::template Instance<T>(count);
	}
	
	template< typename T, typename... _p >
	qor_pp_forceinline ref_of<T>::type new_array_ref(size_t count, _p&&... p1)
	{
		return instancer_of<T>::type::template Instance<T>(count, std::forward<_p&&>(p1)...);
	}

	template< typename T>
	qor_pp_forceinline void internal_del_ref(T* p)
	{
		instancer_of<T>::type::template Release<T>(p, 1);
	}

}//qor

#endif//QOR_PP_H_REFERENCE_NEW
