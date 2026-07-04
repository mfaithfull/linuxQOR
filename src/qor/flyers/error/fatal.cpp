// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "error.h"
#include "handler.h"

namespace qor{

    Fatal::Fatal(const std::string& message) : SeverityTemplateIssue<Severity::Fatal_Error>(message){ }

    Fatal& Fatal::operator = (const Fatal& src)
    {
        SeverityTemplateIssue<Severity::Fatal_Error>::operator = (src);
        return *this;
    }
    
    void Fatal::Handle()
    {
        auto fatalHandler = new_ref< IssueHandler<Fatal> >();
        if(fatalHandler.IsNotNull())
        {
            fatalHandler->Handle(*this);                    //Fatal errors are handled but always fail to Resolve
            Resolve(false);
        }
        else
        {
            auto handler = new_ref< IssueHandler<Error> >();
            if(handler.IsNotNull())
            {
                handler->Handle(*this);
            }
            Resolve(false);
        }
    }
        
    void Fatal::Escalate() const
    {
        //Pause before we quit to allow other threads that may be logging to catch up or clean up
        CurrentThread::GetCurrent().Sleep(2000);    
        std::terminate();
    }
    
    void Fatal::Ignore() const
    {
        Escalate();             //Can't ignore fatal issues.
    }

}//qor
