// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_TASK_INLINE_SCHEDULER
#define QOR_PP_H_FRAMEWORK_TASK_INLINE_SCHEDULER

#include <coroutine>

namespace qor
{
	class InlineScheduler
	{
	public:

		InlineScheduler() noexcept = default;

		std::suspend_never Schedule() const noexcept
		{
			return {};
		}
	};

}//qor

#endif//QOR_PP_H_FRAMEWORK_TASK_INLINE_SCHEDULER
