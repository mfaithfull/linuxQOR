// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"


#include "spinmutex.h"
#include "spinwait.h"

namespace qor{

	SpinMutex::SpinMutex() noexcept
		: m_isLocked(false){ }

	bool SpinMutex::TryLock() noexcept
	{
		return !m_isLocked.exchange(true, std::memory_order_acquire);
	}

	void SpinMutex::Lock() noexcept
	{
		SpinWait wait;
		while (!TryLock())
		{
			while (m_isLocked.load(std::memory_order_relaxed))
			{
				wait.SpinOne();
			}
		}
	}

	void SpinMutex::Unlock() noexcept
	{
		m_isLocked.store(false, std::memory_order_release);
	}

}//qor
