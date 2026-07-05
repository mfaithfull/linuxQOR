// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "error.h"
#include "handler.h"

namespace qor{

    Continuable::Continuable(const std::string& message) : ErrorBase(message, Severity::Continuable_Error){ }

    Continuable::Continuable(const Continuable& src) : ErrorBase(src){ }

    Continuable::~Continuable() noexcept { }
    
    Continuable& Continuable::operator = (const Continuable& src)
    {
        ErrorBase::operator = (src);
        return *this;
    }

    void Continuable::Handle()
    {
        auto continuableHandler = new_ref< IssueHandler<Continuable> >();
        if(!continuableHandler.IsNull())
        {
            Resolve(continuableHandler->Handle(*this));
        }
        else
        {
            auto handler = new_ref< IssueHandler<ErrorBase> >();
            if(!handler.IsNull())
            {
                Resolve(handler->Handle(*this));
            }
            else
            {
                Resolve(false);
            }
        }
    }

    void Continuable::Escalate(void) const
    {
        serious("Escalated continuable error: {0}", m_what->Content());
    }

    void Continuable::Ignore() const
    {
        warning("Ignoring a continuable error. {0}", m_what->Content());
    }
    
}//qor
