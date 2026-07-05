// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "error.h"
#include "handler.h"

namespace qor{

    Warning::Warning(const std::string& message) : ErrorBase(message, Severity::Warning){ }

    Warning::Warning(const Warning& src) : ErrorBase(src){ }

    Warning::~Warning() noexcept { }
 
    Warning& Warning::operator = (const Warning& src)
    {
        ErrorBase::operator = (src);
        return *this;
    }

    void Warning::Handle()
    {
        auto warningHandler = new_ref< IssueHandler<Warning> >();
        if(warningHandler.IsNotNull())
        {
            Resolve(warningHandler->Handle(*this));
        }
        else
        {
            auto handler = new_ref< IssueHandler<ErrorBase> >();
            Resolve(handler.IsNotNull() ? handler->Handle(*this) : false);
        }
    }

    void Warning::Escalate(void) const
    {
        continuable("Escalated Warning: {0}", m_what->Content());
    }

}//qor
