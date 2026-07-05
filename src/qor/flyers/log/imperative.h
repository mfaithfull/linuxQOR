// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_LOG_IMPERATIVE
#define QOR_PP_H_LOG_IMPERATIVE

#include "log.h"

namespace qor{ namespace log {

    class qor_pp_module_interface(QOR_LOG) Imperative : public Log
    {
    public:
        Imperative(const std::string& message);
        Imperative(const Imperative& src);
        virtual ~Imperative() noexcept;
        Imperative& operator = (const Imperative& src);
        virtual void Handle();
    };

    template< typename... _p >
    void imperative(const std::string& message, _p&&... p1)
    {
        issue<Imperative, const std::string&>(std::vformat(std::string_view(message), std::make_format_args(p1...)));
    }

    qor_pp_module_interface(QOR_LOG) void imperative(const std::string& message);

}}//qor::log

#endif//QOR_PP_H_LOG_IMPERATIVE
