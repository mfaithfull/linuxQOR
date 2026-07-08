// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SYNC_SPINWAIT
#define QOR_PP_H_SYNC_SPINWAIT

#include <cstdint>

namespace qor{

    class SpinWait
	{
	public:

		SpinWait() noexcept;
		bool NextSpinWillYield() const noexcept;
		void SpinOne() noexcept;
		void Reset() noexcept;

	private:

		std::uint32_t m_count;

	};

}//qor

#endif//QOR_PP_H_SYNC_SPINWAIT
