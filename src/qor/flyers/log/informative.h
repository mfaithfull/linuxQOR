// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_LOG_INFORMATIVE
#define QOR_PP_H_LOG_INFORMATIVE
#include "log.h"

namespace qor{ namespace log {

    class qor_pp_module_interface(QOR_LOG) Informative : public Log
    {
    public:
        Informative(const std::string& message, IFunctionContext* fContext = nullptr);
        Informative(const Informative& src);
        Informative(Informative&& src) noexcept;
        virtual ~Informative() noexcept;
        Informative& operator = (const Informative& src);
        Informative& operator = (Informative&& src) noexcept;
        virtual void Handle();
    };

    template< typename... _p >
    void inform(const std::string& message, _p&&... p1)
    {
        issue<Informative, const std::string&>(std::vformat(std::string_view(message), std::make_format_args(p1...)));
    }

    qor_pp_module_interface(QOR_LOG) void inform(const std::string& message);
    
}}//qor::log

#endif//QOR_PP_H_LOG_INFORMATIVE
