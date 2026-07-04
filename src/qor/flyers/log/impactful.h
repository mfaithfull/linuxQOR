// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_LOG_IMPACTFUL
#define QOR_PP_H_LOG_IMPACTFUL

#include "log.h"

namespace qor{ namespace log {

    class qor_pp_module_interface(QOR_LOG) Impactful : public LevelTemplateIssue<Level::Impactful>
    {
    public:
        Impactful(const std::string& message);
        virtual ~Impactful() noexcept = default;
        Impactful& operator = (const Impactful& src);
        virtual void Handle();
    };

    template< typename... _p >
    void impact(const std::string& message, _p&&... p1)
    {
        issue<Impactful, const std::string&>(std::vformat(std::string_view(message), std::make_format_args(p1...)));
    }

    inline void impact(const std::string& message)
    {
        issue<Impactful, const std::string&>(message);
    }

}}//qor::log

#endif//QOR_PP_H_LOG_IMPACTFUL
