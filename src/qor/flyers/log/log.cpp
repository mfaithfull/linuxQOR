// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/flyers/error/error.h"
#include "log.h"
#include "handler.h"

namespace qor{ namespace log {

    Log::Log(Level l, const std::string& message) : Issue()
    {
        auto context = CurrentThread::GetCurrent().Context().FunctionContext();
        m_when = new_ref<When>();
        if(context != nullptr)
        {
            m_where = new_ref<Where>(context->File(), context->Line(), context->Name(), context->TypedAny(),context->Module());            
        }
        m_what = new_ref<LevelWhat>(message,l);
    }

    Log::Log(Level l, const std::string& message, IFunctionContext* fContext) : Issue()
    {
        if(fContext == nullptr)
        {
            fContext = CurrentThread::GetCurrent().Context().FunctionContext();
        }
        m_when = new_ref<When>();
        if(fContext != nullptr)
        {
            m_where = new_ref<Where>(fContext->File(), fContext->Line(), fContext->Name(), fContext->TypedAny(),fContext->Module());            
        }
        m_what = new_ref<LevelWhat>(message,l);
    }

    Log::Log(const Log& src) : Issue<LevelWhat>(src){ }

    Log& Log::operator = (const Log& src)
    {
        Issue<LevelWhat>::operator = (src);
        return *this;
    }

    void Log::Handle(void)
    {
        auto logHandler = new_ref< IssueHandler<Log> >();
        if(!logHandler.IsNull())
        {
            logHandler->Handle(*this);
        }
    }

}}//qor::log
