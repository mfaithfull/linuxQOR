// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ERROR_SEVERITYISSUE
#define QOR_PP_H_ERROR_SEVERITYISSUE

#include "src/qor/flyers/issue/issue.h"
#include "severity.h"
#include "severitywhat.h"

namespace qor{ 

    class qor_pp_module_interface(QOR_ERROR) Error : public Issue<SeverityWhat>
    {
    public:

        Error(Severity s, const std::string& message);
        Error(const Error & src);
        virtual ~Error() noexcept = default;
        Error& operator = (const Error & src);
        virtual void Handle();

        virtual void Catch(){ Handle(); }

        virtual void Escalate(void) const{ throw (*this); }
    };

    template<Severity S>
    class SeverityTemplateIssue : public Error
    {
    public:
        SeverityTemplateIssue(const std::string& message) : Error(S, message) { }
        SeverityTemplateIssue(const SeverityTemplateIssue& src) : Error(src) { }
        virtual ~SeverityTemplateIssue() noexcept = default;

        SeverityTemplateIssue& operator = (const SeverityTemplateIssue& src)
        {
            Error::operator=(src);
            return *this;
        }

        Severity GetSeverity() const { return S; }
    };

}//qor

#endif//QOR_PP_H_ERROR_SEVERITYISSUE
