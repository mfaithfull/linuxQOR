// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "functioncontextlock.h"
#include "src/qor/essentials/current/currentthread.h"

namespace qor {

    FunctionContextLock::FunctionContextLock()
    {
        m_FunctionContext = CurrentThread::GetCurrent().Context().FunctionContext();
        if (m_FunctionContext)
        {
            m_Level = m_FunctionContext->Lock();
            m_CallContext = dynamic_cast<CallContext*>(m_FunctionContext->GetCallContext());
            if (m_FunctionContext->GetParent())
            {
                m_ParentCallContext = dynamic_cast<CallContext*>(m_FunctionContext->GetParent()->GetCallContext());
            }
        }
    }
    
    CallContext* FunctionContextLock::CallContextPtr(void)
    {
        return m_CallContext;
    }

    CallContext* FunctionContextLock::ParentCallContextPtr(void)
    {
        return m_ParentCallContext;
    }
    
    unsigned long FunctionContextLock::Level(void)
    {
        return m_Level;
    }
    
    FunctionContextLock::~FunctionContextLock()
    {
        if (m_FunctionContext != nullptr)
        {
            m_FunctionContext->Unlock();
        }
    }

}//qor
