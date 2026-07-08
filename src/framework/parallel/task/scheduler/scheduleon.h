// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_TASK_SCHEDULE_ON
#define QOR_PP_H_FRAMEWORK_TASK_SCHEDULE_ON

#include "src/framework/parallel/task/task.h"
#include "src/framework/parallel/task/sharedtask.h"
#include "src/framework/parallel/task/generator/asyncgenerator.h"
#include "src/framework/parallel/task/awaitabletraits.h"

#include "src/framework/parallel/task/detail/removervaluereference.h"

namespace qor
{
	template<typename SCHEDULER>
	struct ScheduleOnTransform
	{
		explicit ScheduleOnTransform(SCHEDULER& scheduler) noexcept : scheduler(scheduler) {}

		SCHEDULER& scheduler;
	};

	template<typename SCHEDULER>
	ScheduleOnTransform<SCHEDULER> ScheduleOn(SCHEDULER& scheduler)
	{
		return ScheduleOnTransform<SCHEDULER>{ scheduler };
	}

	template<typename T, typename SCHEDULER>
	decltype(auto) operator|(T&& value, ScheduleOnTransform<SCHEDULER> transform)
	{
		return ScheduleOn(transform.scheduler, std::forward<T>(value));
	}

	template<typename SCHEDULER, typename AWAITABLE>
	auto ScheduleOn(SCHEDULER& scheduler, AWAITABLE awaitable) -> task<detail::remove_rvalue_reference_t<typename awaitable_of<AWAITABLE>::await_result_t>>
	{
		co_await scheduler.Schedule();
		co_return co_await std::move(awaitable);
	}

	template<typename T, typename SCHEDULER>
	AsyncGenerator<T> ScheduleOn(SCHEDULER& scheduler, AsyncGenerator<T> source)
	{
		// Transfer exection to the scheduler before the implicit calls to
		// 'co_await begin()' or subsequent calls to `co_await iterator::operator++()`
		// below. This ensures that all calls to the Generator's coroutine_handle<>::resume()
		// are executed on the execution context of the scheduler.
		co_await scheduler.Schedule();

		const auto itEnd = source.end();
		auto it = co_await source.begin();
		while (it != itEnd)
		{
			co_yield *it;

			co_await scheduler.Schedule();

			(void)co_await ++it;
		}
	}
	
}//qor

#endif//QOR_PP_H_FRAMEWORK_TASK_SCHEDULE_ON
