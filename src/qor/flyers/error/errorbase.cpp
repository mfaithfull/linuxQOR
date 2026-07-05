// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "errorbase.h"
#include "src/qor/essentials/current/currentthread.h"
#include "error.h"
#include "handler.h"

namespace qor{ 

    ErrorBase::ErrorBase(const std::string& message, Severity severity) : Issue<SeverityWhat>()
    {
        auto context = CurrentThread::GetCurrent().Context().FunctionContext();
        m_when = new_ref<When>();
        if(context != nullptr)
        {
            m_where = new_ref<Where>(context->File(), context->Line(), context->Name(), context->TypedAny(), context->Module());
        }
        m_what = new_ref<SeverityWhat>(message, severity);
    }

    ErrorBase::ErrorBase(const ErrorBase & src) : Issue<SeverityWhat>(src) { }
    ErrorBase::~ErrorBase() noexcept { }

    ErrorBase& ErrorBase::operator = (const ErrorBase & src)
    {
        Issue<SeverityWhat>::operator = (src);
        return *this;
    }

    void ErrorBase::Handle()
    {
        auto handler = new_ref< IssueHandler<ErrorBase> >();
        if(handler.IsNotNull())
        {
            Resolve(handler->Handle(*this));
        }
        else
        {
            Escalate();
        }
    }

    void ErrorBase::Catch()
    { 
        Handle(); 
    }

    void ErrorBase::Escalate() const
    { 
        throw (*this); 
    }

    Severity ErrorBase::GetSeverity() const 
    { 
        return m_what.IsNotNull() ? m_what->GetSeverity() : Severity::Fatal_Error; 
    }

}//qor
