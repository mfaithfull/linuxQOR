// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "src/configuration/configuration.h"

#include "severityissue.h"
#include "src/framework/thread/currentthread.h"
#include "error.h"
#include "handler.h"

namespace qor{ 

    Error::Error(Severity s, const std::string& message) : Issue()
    {
        auto pContext = framework::CurrentThread::GetCurrent().Context().FunctionContext();
        m_when = new_ref<When>();
        if(pContext != nullptr)
        {
            m_where = new_ref<Where>(pContext->File(), pContext->Line(), pContext->Name(), pContext->TypedAny());
        }
        m_what = new_ref<SeverityWhat>(message, s);
    }

    Error& Error::operator = (const Error& src)
    {
        Issue<SeverityWhat>::operator = (src);
        return *this;
    }

    void Error::Handle(void) const
    {
        auto pHandler = new_ref< IssueHandler<Error> >();
        if(!pHandler.IsNull())
        {
            Resolve(pHandler->Handle(*this));
        }

    }

}//qor
