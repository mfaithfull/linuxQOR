// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "error.h"
#include "handler.h"

namespace qor{

    Serious::Serious(const std::string& message) : ErrorBase(message, Severity::Serious_Error){ }

    Serious::Serious(const Serious& src) : ErrorBase(src){ }

    Serious::~Serious() noexcept { }
    
    Serious& Serious::operator = (const Serious& src)
    {
        ErrorBase::operator = (src);
        return *this;
    }
    
    void Serious::Handle()
    {
        auto pSeriousHandler = new_ref< IssueHandler<Serious> >();
        if(!pSeriousHandler.IsNull())
        {
            Resolve(pSeriousHandler->Handle(*this));
        }
        else
        {
            auto handler = new_ref< IssueHandler<ErrorBase> >();
            Resolve(handler.IsNotNull() ? handler->Handle(*this) : false);
        }
    }

    void Serious::Escalate() const
    {
        throw(*this);
    }
    
    void Serious::Ignore() const
    {
        warning("Ignoring a serious issue. Extreme caution advised! {0}", m_what->Content());
    }

}//qor
