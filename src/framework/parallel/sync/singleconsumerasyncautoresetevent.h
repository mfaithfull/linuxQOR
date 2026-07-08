// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SYNC_SINGLECONSUMERASYNCAUTORESETEVENT
#define QOR_PP_H_SYNC_SINGLECONSUMERASYNCAUTORESETEVENT

#include <coroutine>
#include <atomic>
#include <cstdint>
#include <cassert>

namespace qor
{
	class SingleConsumerAsyncAutoResetEvent
	{
	public:

		SingleConsumerAsyncAutoResetEvent(bool initiallySet = false) noexcept : m_state(initiallySet ? this : nullptr) {}

		void set() noexcept
		{
			void* oldValue = m_state.exchange(this, std::memory_order_release);
			if (oldValue != nullptr && oldValue != this)
			{
				// There was a waiting coroutine that we now need to resume.
				auto handle = *static_cast<std::coroutine_handle<>*>(oldValue);

				// We also need to transition the state back to 'not set' before
				// resuming the coroutine. This operation needs to be 'acquire'
				// so that it synchronises with other calls to .set() that execute
				// concurrently with this call and execute the above m_state.exchange(this)
				// operation with 'release' semantics.
				// This needs to be an exchange() instead of a store() so that it can have
				// 'acquire' semantics.
				(void)m_state.exchange(nullptr, std::memory_order_acquire);

				// Finally, resume the waiting coroutine.
				handle.resume();
			}
		}

		auto operator co_await() const noexcept
		{
			class awaiter
			{
			public:

				awaiter(const SingleConsumerAsyncAutoResetEvent& event) noexcept : m_event(event) {}

				bool await_ready() const noexcept { return false; }
				
				bool await_suspend(std::coroutine_handle<> awaitingCoroutine) noexcept
				{
					m_awaitingCoroutine = awaitingCoroutine;

					void* oldValue = nullptr;
					if (!m_event.m_state.compare_exchange_strong(
						oldValue,
						&m_awaitingCoroutine,
						std::memory_order_release,
						std::memory_order_relaxed))
					{
						// This will only fail if the event was already 'set'
						// In which case we can just reset back to 'not set'
						// Need to use exchange() rather than store() here so we can make this
						// operation an 'acquire' operation so that we get visibility of all
						// writes prior to all preceding calls to .set().
						assert(oldValue == &m_event);
						(void)m_event.m_state.exchange(nullptr, std::memory_order_acquire);
						return false;
					}

					return true;
				}

				void await_resume() noexcept {}

			private:

				const SingleConsumerAsyncAutoResetEvent& m_event;
				std::coroutine_handle<> m_awaitingCoroutine;
			};

			return awaiter{ *this };
		}

	private:

		// nullptr - not set, no waiter
		// this    - set
		// other   - not set, pointer is address of a coroutine_handle<> to resume.
		mutable std::atomic<void*> m_state;

	};

}//qor

#endif//QOR_PP_H_SYNC_SINGLECONSUMERASYNCAUTORESETEVENT
