// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef QOR_PP_H_REFERENCE_NEW
#define QOR_PP_H_REFERENCE_NEW

#include "src/qor/reference/reference.h"
#include "src/qor/factory/factory.h"
#include "src/qor/instance/instance.h"
#include "src/qor/instance/default.h"

namespace qor{

	template< typename T >
	ref_of<T>::type new_ref()
	{
		return instancer_of<T>::type::template Instance<T>(1);
	}

	template< typename T, typename... _p >
	ref_of<T>::type new_ref(_p&&... p1)
	{
		constexpr size_t default_count = 1;
		return instancer_of<T>::type::template Instance<T>(default_count, std::forward<_p&&>(p1)...);
	}

	template< typename T >
	ref_of<T>::type new_array_ref(size_t count)
	{
		return instancer_of<T>::type::template Instance<T>(count);
	}
	
	template< typename T, typename... _p >
	ref_of<T>::type new_array_ref(size_t count, _p&&... p1)
	{
		return instancer_of<T>::type::template Instance<T>(count, std::forward<_p&&>(p1)...);
	}

	template< typename T>
	void internal_del_ref(T* p)
	{
		instancer_of<T>::type::template Release<T>(p, 1);
	}

}//qor

#endif//QOR_PP_H_REFERENCE_NEW
