// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SYNC_SPINMUTEX
#define QOR_PP_H_SYNC_SPINMUTEX

#include <atomic>

namespace qor
{
	class SpinMutex
	{
	public:

		/// Initialise the mutex to the unlocked state.
		SpinMutex() noexcept;

		/// Attempt to lock the mutex without blocking
		///
		/// \return
		/// true if the lock was acquired, false if the lock was already held
		/// and could not be immediately acquired.
		bool TryLock() noexcept;

		/// Block the current thread until the lock is acquired.
		///
		/// This will busy-wait until it acquires the lock.
		///
		/// This has 'acquire' memory semantics and synchronises
		/// with prior calls to unlock().
		void Lock() noexcept;

		/// Release the lock.
		///
		/// This has 'release' memory semantics and synchronises with
		/// lock() and try_lock().
		void Unlock() noexcept;

	private:

		std::atomic<bool> m_isLocked;

	};
}

#endif//QOR_PP_H_SYNC_SPINMUTEX