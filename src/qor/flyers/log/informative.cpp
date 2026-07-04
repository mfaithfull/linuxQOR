// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>

#include "src/qor/flyers/error/error.h"
#include "informative.h"
#include "handler.h"

namespace qor{ namespace log{

    Informative::Informative(const std::string& message, IFunctionContext* fContext) : LevelTemplateIssue<Level::Informative>(message,fContext){ }

    Informative& Informative::operator = (const Informative& src)
    {
        LevelTemplateIssue<Level::Informative>::operator = (src);
        return *this;
    }
    
    Informative& Informative::operator = (Informative&& src) noexcept
    {
        LevelTemplateIssue<Level::Informative>::operator = (src);
        return *this;
    }

    void Informative::Handle()
    {
        auto informHandler = new_ref< IssueHandler<Informative> >();
        if(informHandler.IsNotNull())
        {
            informHandler->Handle(*this);
        }
        else
        {
            auto logHandler = new_ref< IssueHandler<Log> >();
            if(logHandler.IsNotNull())
            {
                logHandler->Handle(*this);
            }
            else
            {
                //TODO: Send this out through qor_reporting instead
                std::cerr << "No log handler installed." << this->m_what->Content() << std::endl;
            }
        }
    }

}}//qor::log
