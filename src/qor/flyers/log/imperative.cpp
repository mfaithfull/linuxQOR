// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/flyers/error/error.h"
#include "imperative.h"
#include "handler.h"

namespace qor{ namespace log{

    Imperative::Imperative(const std::string& message) : LevelTemplateIssue<Level::Imperative>(message){ }

    Imperative& Imperative::operator = (const Imperative& src)
    {
        LevelTemplateIssue<Level::Imperative>::operator = (src);
        return *this;
    }
    
    void Imperative::Handle()
    {
        auto imperativeHandler = new_ref< IssueHandler<Imperative> >();
        if(imperativeHandler.IsNotNull())
        {
            imperativeHandler->Handle(*this);
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
