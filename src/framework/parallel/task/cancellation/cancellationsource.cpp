// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>

#include "cancellationsource.h"
#include "cancellationstate.h"

namespace qor{

	CancellationSource::CancellationSource() : m_state(detail::CancellationState::create())
	{
	}

	CancellationSource::CancellationSource(const CancellationSource& other) noexcept : m_state(other.m_state)
	{
		if (m_state != nullptr)
		{
			m_state->add_source_ref();
		}
	}

	CancellationSource::CancellationSource(CancellationSource&& other) noexcept : m_state(other.m_state)
	{
		other.m_state = nullptr;
	}

	CancellationSource::~CancellationSource()
	{
		if (m_state != nullptr)
		{
			m_state->release_source_ref();
		}
	}

	CancellationSource& CancellationSource::operator=(const CancellationSource& other) noexcept
	{
		if (m_state != other.m_state)
		{
			if (m_state != nullptr)
			{
				m_state->release_source_ref();
			}

			m_state = other.m_state;

			if (m_state != nullptr)
			{
				m_state->add_source_ref();
			}
		}

		return *this;
	}

	CancellationSource& CancellationSource::operator=(CancellationSource&& other) noexcept
	{
		if (this != &other)
		{
			if (m_state != nullptr)
			{
				m_state->release_source_ref();
			}

			m_state = other.m_state;
			other.m_state = nullptr;
		}

		return *this;
	}

	bool CancellationSource::can_be_cancelled() const noexcept
	{
		return m_state != nullptr;
	}

	CancellationToken CancellationSource::token() const noexcept
	{
		return CancellationToken(m_state);
	}

	void CancellationSource::request_cancellation()
	{
		if (m_state != nullptr)
		{
			m_state->request_cancellation();
		}
	}

	bool CancellationSource::is_cancellation_requested() const noexcept
	{
		return m_state != nullptr && m_state->is_cancellation_requested();
	}

}//qor
