// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_LOG_LEVELISSUE
#define QOR_PP_H_LOG_LEVELISSUE

#include "src/qor/flyers/issue/issue.h"
#include "level.h"
#include "levelwhat.h"

namespace qor{ namespace log {

    class qor_pp_module_interface(QOR_LOG) Log : public Issue<LevelWhat>
    {
    public:
        
        Log(Level s, const std::string& message);
        Log(Level s, const std::string& message, IFunctionContext* fContext);
        Log(const Log& src);
        virtual ~Log() noexcept = default;
        Log& operator = (const Log & src);
        virtual void Handle();
        virtual void Escalate() const {/*nothing to do. Can't escalate a Log issue*/}
    };

    template<Level S>
    class LevelTemplateIssue : public Log
    {
    public:
        
        LevelTemplateIssue(const std::string& message, IFunctionContext* fContext = nullptr) : Log(S, message, fContext) {}
        LevelTemplateIssue(const LevelTemplateIssue& src) : Log(src) {} 
        LevelTemplateIssue(LevelTemplateIssue&& src) noexcept : Log(src) {}
        LevelTemplateIssue& operator=(LevelTemplateIssue&& src) noexcept 
        {
            Log::operator = (src);
            return *this;
        }
        
        virtual ~LevelTemplateIssue() noexcept = default;

        LevelTemplateIssue& operator = (const LevelTemplateIssue& src)
        {
            Log::operator=(src);
            return *this;
        }
        
        Level GetLevel() const { return S; }
    };

}}//qor::log

#endif//QOR_PP_H_LOG_LEVELISSUE
