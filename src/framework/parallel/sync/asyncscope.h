// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SYNC_ASYNCSCOPE
#define QOR_PP_H_SYNC_ASYNCSCOPE

#include <atomic>
#include <coroutine>
#include <type_traits>
#include <cassert>

#include "onscopeexit.h"

namespace qor
{
	class AsyncScope
	{
	public:

		AsyncScope() noexcept : m_count(1u) {}

		~AsyncScope()
		{
			// scope must be co_awaited before it destructs.
			assert(m_continuation);
		}

		template<typename AWAITABLE>
		void Spawn(AWAITABLE&& awaitable)
		{
			[](AsyncScope* scope, std::decay_t<AWAITABLE> awaitable) -> oneway_task
			{
				scope->on_work_started();
				auto decrementOnCompletion = on_scope_exit([scope] { scope->on_work_finished(); });
				co_await std::move(awaitable);
			}(this, std::forward<AWAITABLE>(awaitable));
		}

		[[nodiscard]] auto Join() noexcept
		{
			class awaiter
			{
				AsyncScope* m_scope;
			public:
				awaiter(AsyncScope* scope) noexcept : m_scope(scope) {}

				bool await_ready() noexcept
				{
					return m_scope->m_count.load(std::memory_order_acquire) == 0;
				}

				bool await_suspend(std::coroutine_handle<> continuation) noexcept
				{
					m_scope->m_continuation = continuation;
					return m_scope->m_count.fetch_sub(1u, std::memory_order_acq_rel) > 1u;
				}

				void await_resume() noexcept
				{}
			};

			return awaiter{ this };
		}

	private:

		void on_work_finished() noexcept
		{
			if (m_count.fetch_sub(1u, std::memory_order_acq_rel) == 1)
			{
				m_continuation.resume();
			}
		}

		void on_work_started() noexcept
		{
			assert(m_count.load(std::memory_order_relaxed) != 0);
			m_count.fetch_add(1, std::memory_order_relaxed);
		}

		struct oneway_task
		{
			struct promise_type
			{
				std::suspend_never initial_suspend() { return {}; }
				std::suspend_never final_suspend() { return {}; }
				void unhandled_exception() { std::terminate(); }
				oneway_task get_return_object() { return {}; }
				void return_void() {}
			};
		};

		std::atomic<size_t> m_count;
		std::coroutine_handle<> m_continuation;

	};

}//qor

#endif//QOR_PP_H_SYNC_ASYNCSCOPE
