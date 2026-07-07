// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>

#include "asyncmanualresetevent.h"

namespace qor{

    AsyncManualResetEvent::AsyncManualResetEvent() noexcept
        : m_state(nullptr) {}

    AsyncManualResetEvent::AsyncManualResetEvent(bool initiallySet) noexcept
        : m_state(initiallySet ? static_cast<void*>(this) : nullptr) {}

    AsyncManualResetEvent::~AsyncManualResetEvent()
    {
        // There should be no coroutines still awaiting the event.
        assert(
            m_state.load(std::memory_order_relaxed) == nullptr ||
            m_state.load(std::memory_order_relaxed) == static_cast<void*>(this));
    }

    bool AsyncManualResetEvent::isSet() const noexcept
    {
        return m_state.load(std::memory_order_acquire) == static_cast<const void*>(this);
    }

    AsyncManualResetEventOperation AsyncManualResetEvent::operator co_await() const noexcept
    {
        return AsyncManualResetEventOperation{ *this };
    }

    void AsyncManualResetEvent::Set() noexcept
    {
        void* const setState = static_cast<void*>(this);

        // Needs 'release' semantics so that prior writes are visible to event awaiters
        // that synchronise either via 'is_set()' or 'operator co_await()'.
        // Needs 'acquire' semantics in case there are any waiters so that we see
        // prior writes to the waiting coroutine's state and to the contents of
        // the queued AsyncManualResetEventOperation objects.
        void* oldState = m_state.exchange(setState, std::memory_order_acq_rel);
        if (oldState != setState)
        {
            auto* current = static_cast<AsyncManualResetEventOperation*>(oldState);
            while (current != nullptr)
            {
                auto* next = current->m_next;
                current->m_awaiter.resume();
                current = next;
            }
        }
    }

    void AsyncManualResetEvent::Reset() noexcept
    {
        void* oldState = static_cast<void*>(this);
        m_state.compare_exchange_strong(oldState, nullptr, std::memory_order_relaxed);
    }

    AsyncManualResetEventOperation::AsyncManualResetEventOperation(
        const AsyncManualResetEvent& event) noexcept
        : m_event(event) {}

    bool AsyncManualResetEventOperation::await_ready() const noexcept
    {
        return m_event.isSet();
    }

    bool AsyncManualResetEventOperation::await_suspend( std::coroutine_handle<> awaiter) noexcept
    {
        m_awaiter = awaiter;

        const void* const setState = static_cast<const void*>(&m_event);

        void* oldState = m_event.m_state.load(std::memory_order_acquire);
        do
        {
            if (oldState == setState)
            {
                // State is now 'set' no need to suspend.
                return false;
            }

            m_next = static_cast<AsyncManualResetEventOperation*>(oldState);
        } while (!m_event.m_state.compare_exchange_weak(
            oldState,
            static_cast<void*>(this),
            std::memory_order_release,
            std::memory_order_acquire));

        // Successfully queued this waiter to the list.
        return true;
    }

    void AsyncManualResetEventOperation::await_resume() const noexcept { }
}//qor
