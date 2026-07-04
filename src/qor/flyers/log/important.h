// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_LOG_IMPORTANT
#define QOR_PP_H_LOG_IMPORTANT

#include "log.h"

namespace qor{ namespace log {

    class qor_pp_module_interface(QOR_LOG) Important :  public LevelTemplateIssue<Level::Important>
    {
    public:
        Important(const std::string& message);
        virtual ~Important() noexcept = default;
        Important& operator = (const Important& src);
        virtual void Handle();
    };

    template< typename... _p >
    void important(const std::string& message, _p&&... p1)
    {
        issue<Important, const std::string&>(std::vformat(std::string_view(message), std::make_format_args(p1...)));
    }

    inline void important(const std::string& message)
    {
        issue<Important, const std::string&>(message);
    }

}}//qor::log

#endif//QOR_PP_H_LOG_IMPORTANT
