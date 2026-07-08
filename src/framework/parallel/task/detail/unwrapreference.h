// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_TASK_DETAIL_UNWRAP_REFERENCE
#define QOR_PP_H_FRAMEWORK_TASK_DETAIL_UNWRAP_REFERENCE

#include <functional>

namespace qor{	namespace detail{

		template<typename T>
		struct unwrap_reference
		{
			using type = T;
		};

		template<typename T>
		struct unwrap_reference<std::reference_wrapper<T>>
		{
			using type = T;
		};

		template<typename T> using unwrap_reference_t = typename unwrap_reference<T>::type;
	}
}

#endif//QOR_PP_H_FRAMEWORK_TASK_DETAIL_UNWRAP_REFERENCE
