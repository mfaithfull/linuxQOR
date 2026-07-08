// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <utility>
#include <cassert>

#include "cancellationtoken.h"
#include "operationcancelled.h"
#include "cancellationstate.h"

namespace qor{

	CancellationToken::CancellationToken() noexcept : m_state(nullptr)
	{
	}

	CancellationToken::CancellationToken(const CancellationToken& other) noexcept : m_state(other.m_state)
	{
		if (m_state != nullptr)
		{
			m_state->add_token_ref();
		}
	}

	CancellationToken::CancellationToken(CancellationToken&& other) noexcept : m_state(other.m_state)
	{
		other.m_state = nullptr;
	}

	CancellationToken::~CancellationToken()
	{
		if (m_state != nullptr)
		{
			m_state->release_token_ref();
		}
	}

	CancellationToken& CancellationToken::operator=(const CancellationToken& other) noexcept
	{
		if (other.m_state != m_state)
		{
			if (m_state != nullptr)
			{
				m_state->release_token_ref();
			}

			m_state = other.m_state;

			if (m_state != nullptr)
			{
				m_state->add_token_ref();
			}
		}

		return *this;
	}

	CancellationToken& CancellationToken::operator=(CancellationToken&& other) noexcept
	{
		if (this != &other)
		{
			if (m_state != nullptr)
			{
				m_state->release_token_ref();
			}

			m_state = other.m_state;
			other.m_state = nullptr;
		}

		return *this;
	}

	void CancellationToken::swap(CancellationToken& other) noexcept
	{
		std::swap(m_state, other.m_state);
	}

	bool CancellationToken::can_be_cancelled() const noexcept
	{
		return m_state != nullptr && m_state->can_be_cancelled();
	}

	bool CancellationToken::is_cancellation_requested() const noexcept
	{
		return m_state != nullptr && m_state->is_cancellation_requested();
	}

	void CancellationToken::throw_if_cancellation_requested() const
	{
		if (is_cancellation_requested())
		{
			throw operation_cancelled{};
		}
	}

	CancellationToken::CancellationToken(detail::CancellationState* state) noexcept : m_state(state)
	{
		if (m_state != nullptr)
		{
			m_state->add_token_ref();
		}
	}

}//qor
