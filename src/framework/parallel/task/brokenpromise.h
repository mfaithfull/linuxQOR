// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_TASK_BROKEN_PROMISE
#define QOR_PP_H_FRAMEWORK_TASK_BROKEN_PROMISE

#include <stdexcept>

namespace qor{

	// Exception thrown when you attempt to retrieve the result of a task that has been detached from its promise/coroutine.
	class broken_promise : public std::logic_error
	{
	public:
		broken_promise() : std::logic_error("broken promise") {}
	};
	
}//qor

#endif//QOR_PP_H_FRAMEWORK_TASK_BROKEN_PROMISE
