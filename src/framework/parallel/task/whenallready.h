// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_TASK_WHENALLREADY
#define QOR_PP_H_FRAMEWORK_TASK_WHENALLREADY

#include <tuple>
#include <utility>
#include <vector>
#include <type_traits>

#include "awaitabletraits.h"
#include "isawaitable.h"

#include "detail/whenallreadyawaitable.h"
#include "detail/whenalltask.h"
#include "detail/unwrapreference.h"

namespace qor
{
	template< typename... AWAITABLES, std::enable_if_t<std::conjunction_v< is_awaitable<detail::unwrap_reference_t<std::remove_reference_t<AWAITABLES>>>...>, int> = 0>
	[[nodiscard]]
	qor_pp_forceinline auto when_all_ready(AWAITABLES&&... awaitables)
	{
		return detail::WhenAllReadyAwaitable<std::tuple<detail::WhenAllTask<
			typename awaitable_of<detail::unwrap_reference_t<std::remove_reference_t<AWAITABLES>>>::await_result_t>...>>(
				std::make_tuple(detail::make_when_all_task(std::forward<AWAITABLES>(awaitables))...));
	}

	// TODO: Generalise this from vector<AWAITABLE> to arbitrary sequence of awaitable.

	template< typename AWAITABLE, typename RESULT = typename awaitable_of<detail::unwrap_reference_t<AWAITABLE>>::await_result_t>
	[[nodiscard]] auto when_all_ready(std::vector<AWAITABLE> awaitables)
	{
		std::vector<detail::WhenAllTask<RESULT>> tasks;

		tasks.reserve(awaitables.size());

		for (auto& awaitable : awaitables)
		{
			tasks.emplace_back(detail::make_when_all_task(std::move(awaitable)));
		}

		return detail::WhenAllReadyAwaitable<std::vector<detail::WhenAllTask<RESULT>>>(
			std::move(tasks));
	}
}

#endif//QOR_PP_H_FRAMEWORK_TASK_WHENALLREADY
