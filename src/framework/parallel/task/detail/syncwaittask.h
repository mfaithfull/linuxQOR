// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_TASK_DETAIL_SYNCWAITTASK
#define QOR_PP_H_FRAMEWORK_TASK_DETAIL_SYNCWAITTASK

#include <coroutine>
#include <cassert>
#include <exception>
#include <utility>

#include "src/framework/parallel/task/awaitabletraits.h"
#include "src/framework/parallel/sync/lightweightmanualresetevent.h"

namespace qor{	namespace detail{

	template<typename RESULT>
	class SyncWaitTask;

	template<typename RESULT>
	class SyncWaitTaskPromise final
	{
		using coroutine_handle_t = std::coroutine_handle<SyncWaitTaskPromise<RESULT>>;

	public:

		using reference = RESULT&&;

		SyncWaitTaskPromise() noexcept {}

		void start(detail::LightManualResetEvent& event)
		{
			m_event = &event;
			coroutine_handle_t::from_promise(*this).resume();
		}

		auto get_return_object() noexcept
		{
			return coroutine_handle_t::from_promise(*this);
		}

		std::suspend_always initial_suspend() noexcept
		{
			return{};
		}

		auto final_suspend() noexcept
		{
			class completion_notifier
			{
			public:

				bool await_ready() const noexcept 
				{ 
					return false; 
				}

				void await_suspend(coroutine_handle_t coroutine) const noexcept
				{
					coroutine.promise().m_event->Set();
				}

				void await_resume() noexcept {}
			};

			return completion_notifier{};
		}

		auto yield_value(reference result) noexcept
		{
			m_result = std::addressof(result);
			return final_suspend();
		}

		void return_void() noexcept
		{
			// The coroutine should have either yielded a value or thrown
			// an exception in which case it should have bypassed return_void().
			assert(false);
		}

		void unhandled_exception()
		{
			m_exception = std::current_exception();
		}

		reference result()
		{
			if (m_exception)
			{
				std::rethrow_exception(m_exception);
			}

			return static_cast<reference>(*m_result);
		}

	private:

		detail::LightManualResetEvent* m_event;
		std::remove_reference_t<RESULT>* m_result;
		std::exception_ptr m_exception;

	};

	template<>
	class SyncWaitTaskPromise<void>
	{
		using coroutine_handle_t = std::coroutine_handle<SyncWaitTaskPromise<void>>;

	public:

		SyncWaitTaskPromise() noexcept {}

		void start(detail::LightManualResetEvent& event)
		{
			m_event = &event;
			coroutine_handle_t::from_promise(*this).resume();
		}

		auto get_return_object() noexcept
		{
			return coroutine_handle_t::from_promise(*this);
		}

		std::suspend_always initial_suspend() noexcept
		{
			return{};
		}

		auto final_suspend() noexcept
		{
			class completion_notifier
			{
			public:

				bool await_ready() const noexcept 
				{ 
					return false; 
				}

				void await_suspend(coroutine_handle_t coroutine) const noexcept
				{
					coroutine.promise().m_event->Set();
				}

				void await_resume() noexcept {}
			};

			return completion_notifier{};
		}

		void return_void() {}

		void unhandled_exception()
		{
			m_exception = std::current_exception();
		}

		void result()
		{
			if (m_exception)
			{
				std::rethrow_exception(m_exception);
			}
		}

	private:

		detail::LightManualResetEvent* m_event;
		std::exception_ptr m_exception;

	};

	template<typename RESULT>
	class SyncWaitTask final
	{
	public:

		using promise_type = SyncWaitTaskPromise<RESULT>;

		using coroutine_handle_t = std::coroutine_handle<promise_type>;

		SyncWaitTask(coroutine_handle_t coroutine) noexcept : m_coroutine(coroutine) {}

		SyncWaitTask(SyncWaitTask&& other) noexcept : m_coroutine(std::exchange(other.m_coroutine, coroutine_handle_t{})) {}

		~SyncWaitTask()
		{
			if (m_coroutine) m_coroutine.destroy();
		}

		SyncWaitTask(const SyncWaitTask&) = delete;
		SyncWaitTask& operator=(const SyncWaitTask&) = delete;

		void start(LightManualResetEvent& event) noexcept
		{
			m_coroutine.promise().start(event);
		}

		decltype(auto) result()
		{
			return m_coroutine.promise().result();
		}

	private:

		coroutine_handle_t m_coroutine;

	};

	template< typename AWAITABLE, typename RESULT = typename awaitable_of<AWAITABLE&&>::await_result_t, std::enable_if_t<!std::is_void_v<RESULT>, int> = 0>
	SyncWaitTask<RESULT> make_sync_wait_task(AWAITABLE&& awaitable)
	{
		co_yield co_await std::forward<AWAITABLE>(awaitable);
	}

	template< typename AWAITABLE, typename RESULT = typename awaitable_of<AWAITABLE&&>::await_result_t, std::enable_if_t<std::is_void_v<RESULT>, int> = 0>
	SyncWaitTask<void> make_sync_wait_task(AWAITABLE&& awaitable)
	{
		co_await std::forward<AWAITABLE>(awaitable);
	}

}}//qor::detail

#endif//QOR_PP_H_FRAMEWORK_TASK_DETAIL_SYNCWAITTASK
