// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>

#include "cancellationregistration.h"
#include "cancellationstate.h"

namespace qor {

	CancellationRegistration::~CancellationRegistration()
	{
		if (m_state != nullptr)
		{
			m_state->deregister_callback(this);
			m_state->release_token_ref();
		}
	}

	void CancellationRegistration::register_callback(CancellationToken&& token)
	{
		auto* state = token.m_state;
		if (state != nullptr && state->can_be_cancelled())
		{
			m_state = state;
			if (state->try_register_callback(this))
			{
				token.m_state = nullptr;
			}
			else
			{
				m_state = nullptr;
				m_callback();
			}
		}
		else
		{
			m_state = nullptr;
		}
	}

}//qor
