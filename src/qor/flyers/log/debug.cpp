// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/flyers/error/error.h"
#include "log.h"
#include "handler.h"

namespace qor{ namespace log {

    Debug::Debug(const std::string& message) : Log(Level::Debug, message){ }
    Debug::Debug(const Debug& src) : Log(src) { }
    Debug::~Debug() { }

    Debug& Debug::operator = (const Debug& src)
    {
        Log::operator = (src);
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
        
    void debug(const std::string& message)
    {
        //Debug logging will be optimised out of release builds completely
#ifndef NDEBUG
        //issue<Debug, const std::string&>(message);
#endif
    }

}}//qor::log
