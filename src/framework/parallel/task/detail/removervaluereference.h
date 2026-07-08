// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_TASK_DETAIL_REMOVE_RVALUE_REFERENCE
#define QOR_PP_H_FRAMEWORK_TASK_DETAIL_REMOVE_RVALUE_REFERENCE

namespace qor{	namespace detail{

	template<typename T>
	struct remove_rvalue_reference
	{
		using type = T;
	};

	template<typename T>
	struct remove_rvalue_reference<T&&>
	{
		using type = T;
	};

	template<typename T> using remove_rvalue_reference_t = typename remove_rvalue_reference<T>::type;

}}//qor::detail

#endif//QOR_PP_H_FRAMEWORK_TASK_DETAIL_REMOVE_RVALUE_REFERENCE
