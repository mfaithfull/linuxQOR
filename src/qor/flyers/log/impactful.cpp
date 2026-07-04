// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/flyers/error/error.h"
#include "impactful.h"
#include "handler.h"

namespace qor{ namespace log{

    Impactful::Impactful(const std::string& message) : LevelTemplateIssue<Level::Impactful>(message){ }

    Impactful& Impactful::operator = (const Impactful& src)
    {
        LevelTemplateIssue<Level::Impactful>::operator = (src);
        return *this;
    }
    
    void Impactful::Handle()
    {
        auto impactfulHandler = new_ref< IssueHandler<Impactful> >();
        if(impactfulHandler.IsNotNull())
        {
            impactfulHandler->Handle(*this);
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
