// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/flyers/error/error.h"
#include "log.h"
#include "handler.h"

namespace qor{ namespace log {

    Debug::Debug(const std::string& message) : LevelTemplateIssue<Level::Debug>(message){ }

    Debug& Debug::operator = (const Debug& src)
    {
        LevelTemplateIssue<Level::Debug>::operator = (src);
        return *this;
    }
    
    void Debug::Handle()
    {
        auto debugHandler = new_ref< IssueHandler<Debug> >();
        if(debugHandler.IsNotNull())
        {
            debugHandler->Handle(*this);
        }
        else
        {
            auto logHandler = new_ref< IssueHandler<Log> >();
            if(logHandler.IsNotNull())
            {
                logHandler->Handle(*this);
            }
        }
    }
        
}}//qor::log
