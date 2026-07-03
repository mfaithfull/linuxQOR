// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "threadcontext.h"

namespace qor{ namespace detail{

    ThreadContext::ThreadContext() : m_pRootContext(nullptr), m_pCurrentContext(nullptr){ }

    IFunctionContext* ThreadContext::RegisterFunctionContext(IFunctionContext* pFContext)
    {
		IFunctionContext* pParent = m_pCurrentContext;
		m_pCurrentContext = pFContext;
		if (pParent == nullptr)
		{
			m_pRootContext = m_pCurrentContext;
		}
		return pParent;
    }
    
    void ThreadContext::UnregisterFunctionContext(IFunctionContext*, IFunctionContext* pParent)
    {
		if (m_pCurrentContext == m_pRootContext)
		{
			m_pCurrentContext = nullptr;
		}
		else
		{
			m_pCurrentContext = pParent;
		}		
    }

}}//qor::detail
