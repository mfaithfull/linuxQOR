// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_LOG_LEVELISSUE
#define QOR_PP_H_LOG_LEVELISSUE

#include "src/qor/flyers/issue/issue.h"
#include "level.h"
#include "levelwhat.h"

namespace qor{ namespace log {

    class Log : public Issue<LevelWhat>
    {
    public:
        
        qor_pp_module_interface(QOR_LOG) Log(Level s, const std::string& message);
        qor_pp_module_interface(QOR_LOG) Log(Level s, const std::string& message, IFunctionContext* fContext);
        qor_pp_module_interface(QOR_LOG) Log(const Log& src);
        qor_pp_module_interface(QOR_LOG) virtual ~Log() noexcept;
        qor_pp_module_interface(QOR_LOG) Log& operator = (const Log & src);
        qor_pp_module_interface(QOR_LOG) virtual void Handle();
        qor_pp_module_interface(QOR_LOG) qor_pp_noinline virtual void Escalate() const;
        qor_pp_module_interface(QOR_LOG) Level GetLevel() const;
    };

}}//qor::log

#endif//QOR_PP_H_LOG_LEVELISSUE
