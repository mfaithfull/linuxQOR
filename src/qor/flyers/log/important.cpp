// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/flyers/error/error.h"
#include "important.h"
#include "handler.h"

namespace qor{ namespace log{

    Important::Important(const std::string& message) : Log(Level::Important, message){ }

    Important::Important(const Important& src) : Log(src) { }
    
    Important::~Important() noexcept = default;

    Important& Important::operator = (const Important& src)
    {
        Log::operator = (src);
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

    void important(const std::string& message)
    {
        issue<Important, const std::string&>(message);
    }

}}//qor::log
