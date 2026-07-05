// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ERROR_SEVERITYWHAT
#define QOR_PP_H_ERROR_SEVERITYWHAT

#include "src/qor/flyers/issue/what.h"
#include "severity.h"

namespace qor{ 

    class SeverityWhat : public What
    {
    public:

        SeverityWhat(const std::string& what, Severity severity) : What(what), m_severity(severity){ }

        SeverityWhat(const ref_of<SeverityWhat>::type& src) : What(src->Content()), m_severity(src->m_severity){ }

        virtual ~SeverityWhat() noexcept = default;

        Severity GetSeverity() const
        {
            return m_severity;
        }
        
    protected:

        Severity m_severity;
    };

}//qor

#endif//QOR_PP_H_ERROR_SEVERITYWHAT
