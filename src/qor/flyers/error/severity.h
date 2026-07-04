// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ERROR_SEVERITY
#define QOR_PP_H_ERROR_SEVERITY

namespace qor{ 

    enum class Severity
    {
        Note = 0,
        Warning,
        Continuable_Error,
        Serious_Error,
        Fatal_Error
    };

    constexpr std::string_view SeverityNames[] = {
        "Note",
        "Warning",
        "Continuable",
        "Serious",
        "Fatal"
    };

}//qor

#endif//QOR_PP_H_ERROR_SEVERITY
