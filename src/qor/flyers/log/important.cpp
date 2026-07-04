// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/flyers/error/error.h"
#include "important.h"
#include "handler.h"

namespace qor{ namespace log{

    Important::Important(const std::string& message) : LevelTemplateIssue<Level::Important>(message){ }

    Important& Important::operator = (const Important& src)
    {
        LevelTemplateIssue<Level::Important>::operator = (src);
        return *this;
    }
    
    void Important::Handle()
    {
        auto importantHandler = new_ref< IssueHandler<Important> >();
        if(!importantHandler.IsNull())
        {
            importantHandler->Handle(*this);
        }
        else
        {
            auto logHandler = new_ref< IssueHandler<Log> >();
            if(!logHandler.IsNull())
            {
                logHandler->Handle(*this);
            }
        }
    }

}}//qor::log
