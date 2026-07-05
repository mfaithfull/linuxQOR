// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/flyers/error/error.h"
#include "impactful.h"
#include "handler.h"

namespace qor{ namespace log{

    Impactful::Impactful(const std::string& message) : Log(Level::Impactful, message){ }

    Impactful::Impactful(const Impactful& src) : Log(src) { }
    
    Impactful::~Impactful() noexcept = default;

    Impactful& Impactful::operator = (const Impactful& src)
    {
        Log::operator = (src);
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

    void impact(const std::string& message)
    {
        issue<Impactful, const std::string&>(message);
    }

}}//qor::log
