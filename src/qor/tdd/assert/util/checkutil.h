// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#ifndef QOR_PP_H_SRC_QOR_TDD_ASSERT_UTIL_CHECKUTIL
#define QOR_PP_H_SRC_QOR_TDD_ASSERT_UTIL_CHECKUTIL

#include <initializer_list>

namespace qor{ namespace assert{ namespace util{

struct CheckUtil 
{
	template <template <typename...> typename List, typename T>
	static bool checkIsInRange(List<T>& r, const T& value) 
	{
		for (const auto& e : r) 
		{
			if (value == e) return true;
		}
		return false;
	}

	template <typename T>
	static bool checkIsInRange(std::initializer_list<T> r, const T& value) 
	{
		for (const auto& e : r) 
		{
			if (value == e) return true;
		}
		return false;
	}
};

}}}//qor::assert::util

#endif//QOR_PP_H_SRC_QOR_TDD_ASSERT_UTIL_CHECKUTIL
