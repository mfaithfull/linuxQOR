// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_TASK_CANCEL_OPCANCELLED
#define QOR_PP_H_FRAMEWORK_TASK_CANCEL_OPCANCELLED

#include <exception>

namespace qor
{
	class operation_cancelled : public std::exception
	{
	public:

		operation_cancelled() noexcept : std::exception() {}

		const char* what() const noexcept override { return "operation cancelled"; }
	};

}//qor

#endif//QOR_PP_H_FRAMEWORK_TASK_CANCEL_OPCANCELLED
