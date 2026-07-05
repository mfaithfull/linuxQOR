// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_FRAMEWORK_LOGHANDLER
#define QOR_PP_H_COMPONENTS_FRAMEWORK_LOGHANDLER

#include <string>
#include <string_view>
#include <chrono>

#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/handler.h"
#include "src/framework/event/signals/signal.h"

namespace qor{ namespace components {

    class LogHandler : public qor::IssueHandler<log::Log>, public SignalBase
    {
    public:

        qor_pp_module_interface(QOR_LOGHANDLER) LogHandler();
        qor_pp_module_interface(QOR_LOGHANDLER) LogHandler(log::Level legLevel);
        qor_pp_module_interface(QOR_LOGHANDLER) virtual ~LogHandler() noexcept;
        qor_pp_module_interface(QOR_LOGHANDLER) virtual bool Handle(const log::Log& log);

        qor_pp_module_interface(QOR_LOGHANDLER) LogHandler& Forward(bool forward = true);
        qor_pp_module_interface(QOR_LOGHANDLER) LogHandler& WriteToStandardOutput(bool write = true);
        qor_pp_module_interface(QOR_LOGHANDLER) LogHandler& WriteToFileSystem();
        qor_pp_module_interface(QOR_LOGHANDLER) LogHandler& WriteToPipeline();

    protected:

        qor_pp_module_interface(QOR_LOGHANDLER) virtual std::string WhereText(const char* module, const char* file, const char* function, int line, const std::string& exceptionText, const std::string& instanceText, const std::string& threadText) const;
        qor_pp_module_interface(QOR_LOGHANDLER) virtual std::string InstanceText(bool inInstance, const AnyObject& any) const;
        qor_pp_module_interface(QOR_LOGHANDLER) virtual std::string InExceptionText(bool inException) const;
        qor_pp_module_interface(QOR_LOGHANDLER) virtual std::string MessageText(const std::string_view& level, const std::string& what, const std::string& where, 
            const std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds>& when) const;

    public:

        qor_pp_module_interface(QOR_LOGHANDLER) qor_pp_signal_func forward(std::string logMessage);//don't call this outgoing interface directly

    private:

        qor_pp_module_interface(QOR_LOGHANDLER) std::string GetWhereClause(const log::Log& log) const;

        bool m_forward;
        bool m_writeToStandardOutput;
        log::Level m_level;
    };
}}//qor::components

#endif//QOR_PP_H_COMPONENTS_FRAMEWORK_LOGHANDLER