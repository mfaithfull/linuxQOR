// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "threadcontext.h"

namespace qor{ namespace detail{

	ThreadContext::ThreadContext() = default;
	ThreadContext::~ThreadContext() = default;

    IFunctionContext* ThreadContext::RegisterFunctionContext(IFunctionContext* fContext)
    {
		IFunctionContext* parent = m_CurrentContext;
		m_CurrentContext = fContext;
		if (parent == nullptr)
		{
			m_RootContext = m_CurrentContext;
		}
		return parent;
    }
    
    void ThreadContext::UnregisterFunctionContext(IFunctionContext*, IFunctionContext* parent)
    {
		if (m_CurrentContext == m_RootContext)
		{
			m_CurrentContext = nullptr;
		}
		else
		{
			m_CurrentContext = parent;
		}		
    }

	FlyerMap& ThreadContext::GetFlyerMap(void)
	{
		return m_FlyerMap;
	}

	IFunctionContext* ThreadContext::FunctionContext()
	{
		return m_CurrentContext;
	}

}}//qor::detail
