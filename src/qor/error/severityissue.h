// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef QOR_PP_H_ERROR_SEVERITYISSUE
#define QOR_PP_H_ERROR_SEVERITYISSUE

#include "src/qor/issue/issue.h"
#include "severity.h"
#include "severitywhat.h"

namespace qor{ 

    class qor_pp_module_interface(QOR_ISSUE) SeverityIssue : public Issue<SeverityWhat>
    {
    public:

        SeverityIssue(Severity s, const std::string& message);
        virtual ~SeverityIssue() noexcept = default;
        SeverityIssue& operator = (const SeverityIssue& src);
        virtual void Handle(void);   
    };

    template<Severity S>
    class SeverityTemplateIssue : public SeverityIssue
    {
    public:
        SeverityTemplateIssue(const std::string& message) : SeverityIssue(S, message) {}
        virtual ~SeverityTemplateIssue() noexcept = default;
        SeverityTemplateIssue& operator = (const SeverityTemplateIssue& src)
        {
            SeverityIssue::operator=(src);
            return *this;
        }
        const Severity GetSeverity() const { return S; }

    };

}//qor

#endif//QOR_PP_H_ERROR_SEVERITYISSUE
