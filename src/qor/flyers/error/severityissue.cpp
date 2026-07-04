// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "severityissue.h"
#include "src/qor/essentials/current/currentthread.h"
#include "error.h"
#include "handler.h"

namespace qor{ 

    Error::Error(Severity s, const std::string& message) : Issue<SeverityWhat>()
    {
        auto pContext = CurrentThread::GetCurrent().Context().FunctionContext();
        m_when = new_ref<When>();
        if(pContext != nullptr)
        {
            m_where = new_ref<Where>(pContext->File(), pContext->Line(), pContext->Name(), pContext->TypedAny(), pContext->Module());
        }
        m_what = new_ref<SeverityWhat>(message, s);
    }

    Error::Error(const Error & src) : Issue<SeverityWhat>(src) { }

    Error& Error::operator = (const Error& src)
    {
        Issue<SeverityWhat>::operator = (src);
        return *this;
    }

    void Error::Handle(void)
    {
        auto pHandler = new_ref< IssueHandler<Error> >();
        if(pHandler.IsNotNull())
        {
            Resolve(pHandler->Handle(*this));
        }
        else
        {
            Escalate();
        }
    }

}//qor
