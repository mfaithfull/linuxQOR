// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ERROR_SEVERITYISSUE
#define QOR_PP_H_ERROR_SEVERITYISSUE

#include "src/platform/compiler/compiler.h"
#include "src/qor/flyers/issue/issue.h"
#include "severity.h"
#include "severitywhat.h"

namespace qor{ 
    
    template qor_pp_module_interface(QOR_ERROR) class Issue<SeverityWhat>;

    class ErrorBase : public Issue<SeverityWhat>
    {
    public:

        qor_pp_module_interface(QOR_ERROR) ErrorBase(const std::string& message, Severity severity);
        qor_pp_module_interface(QOR_ERROR) ErrorBase(const ErrorBase & src);
        qor_pp_module_interface(QOR_ERROR) virtual ~ErrorBase() noexcept;
        qor_pp_module_interface(QOR_ERROR) ErrorBase& operator = (const ErrorBase & src);
        qor_pp_module_interface(QOR_ERROR) virtual void Handle();        
        qor_pp_module_interface(QOR_ERROR) virtual void Catch();
        qor_pp_module_interface(QOR_ERROR) virtual void Escalate() const;
        qor_pp_module_interface(QOR_ERROR) Severity GetSeverity() const;

    };

}//qor

#endif//QOR_PP_H_ERROR_SEVERITYISSUE
