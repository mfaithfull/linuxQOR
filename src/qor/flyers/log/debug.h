// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_LOG_DEBUG
#define QOR_PP_H_LOG_DEBUG

#include <format>
#include "log.h"

namespace qor{ namespace log {

    class qor_pp_module_interface(QOR_LOG) Debug : public Log
    {
    public:
        Debug(const std::string& message);
        Debug(const Debug&);
        virtual ~Debug() noexcept;
        Debug& operator = (const Debug& src);
        virtual void Handle();
    };

    template< typename... _p >
    void debug(const std::string& message, _p&&... p1)
    {
        //Debug logging will be optimised out of release builds completely
#ifndef NDEBUG
        issue<Debug, const std::string&>(std::vformat(std::string_view(message), std::make_format_args(p1...)));
#endif        
    }

    qor_pp_module_interface(QOR_LOG) void debug(const std::string& message);

}}//qor::log

#endif//QOR_PP_H_LOG_DEBUG
