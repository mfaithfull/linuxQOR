// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SYNC_WAIT
#define QOR_PP_H_SYNC_WAIT

#include "src/framework/parallel/sync/lightweightmanualresetevent.h"
#include "src/framework/parallel/task/detail/syncwaittask.h"
#include "src/framework/parallel/task/awaitabletraits.h"

#include <cstdint>
#include <atomic>

namespace qor
{
	template<typename AWAITABLE>
	auto sync_wait(AWAITABLE&& awaitable) -> typename awaitable_of<AWAITABLE&&>::await_result_t
	{
		auto task = detail::make_sync_wait_task(std::forward<AWAITABLE>(awaitable));
		detail::LightManualResetEvent event;
		task.start(event);
		event.Wait();
		return task.result();
	}
}

#endif//QOR_PP_H_SYNC_WAIT
