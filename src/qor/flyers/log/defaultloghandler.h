// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_LOG_DEFAULTHANDLER
#define QOR_PP_H_LOG_DEFAULTHANDLER

#include "log.h"
#include "handler.h"
#include "src/framework/event/signals/signal.h"

namespace qor{

    class qor_pp_module_interface(QOR_LOG) DefaultLogHandler : public qor::IssueHandler<log::Log>, public SignalBase
    {
    public:

        DefaultLogHandler(qor::log::Level logLevel = qor::log::Level::Debug);
        virtual ~DefaultLogHandler() noexcept = default;
        
        virtual bool Handle(const log::Log& log);

        DefaultLogHandler& Forward(bool forward = true);
        DefaultLogHandler& WriteToStandardOutput(bool write = true);
        DefaultLogHandler& WriteToFileSystem();
        DefaultLogHandler& WriteToPipeline();

    protected:

        virtual std::string WhereText(const char* module, const char* file, const char* function, int line, const std::string& exceptionText, const std::string& instanceText, const std::string& threadText) const;
        virtual std::string InstanceText(bool inInstance, const qor::AnyObject& any) const;
        virtual std::string InExceptionText(bool inException) const;
        virtual std::string MessageText(const std::string_view& level, const std::string& what, const std::string& where, 
        const std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds>& when) const;
        qor_pp_signal_func forward(std::string logMessage);

    public:

        //Clever compiler optimization that implements the forward signal inside executables that include this header
        //rather than using a single instance of the function inside the log module, means we have to do this.
        //Nailing down the address of the forward signal within the build of the log module, by using it,
        //prevents the 'optimization'.
        typedef void (DefaultLogHandler::*forwardFn)(std::string);
        forwardFn GetForwardSignal() const;

    private:

        std::string GetWhereClause(const log::Log& log) const;

        bool m_forward;
        bool m_writeToStandardOutput;
        log::Level m_level;

    };

}//qor

#endif//QOR_PP_H_LOG_DEFAULTHANDLER