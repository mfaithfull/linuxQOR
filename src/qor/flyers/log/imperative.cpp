// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/flyers/error/error.h"
#include "imperative.h"
#include "handler.h"

namespace qor{ namespace log{

    Imperative::Imperative(const std::string& message) : Log(Level::Imperative, message){ }

    Imperative::Imperative(const Imperative& src) : Log(src) { }

    Imperative::~Imperative() noexcept = default;

    Imperative& Imperative::operator = (const Imperative& src)
    {
        Log::operator = (src);
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

    void imperative(const std::string& message)
    {
        issue<Imperative, const std::string&>(message);
    }

}}//qor::log
