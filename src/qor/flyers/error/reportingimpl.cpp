// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "error.h"
#include "reportingimpl.h"

namespace qor
{ 
    void ReportingImpl::note(const std::string& message)
    {
        qor::note(message);
    }

    void ReportingImpl::warning(const std::string& message)
    {
        qor::warning(message);
    }

    void ReportingImpl::continuable(const std::string& message)
    {
        qor::continuable(message);
    }

    void ReportingImpl::serious(const std::string& message)
    {
        qor::serious(message);
    }

    void ReportingImpl::fatal(const std::string& message)
    {
        qor::fatal(message);
    }

    void ReportingImpl::AssertionFailure(const std::string& message)
    {
        qor::serious(message);
    }

    void ReportingImpl::TestFailure(const std::string& message)
    {
        qor::serious(message);
    }
}//qor
