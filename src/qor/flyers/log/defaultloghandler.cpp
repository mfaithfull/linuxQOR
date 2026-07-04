// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <iostream>
#include <format>
#include "src/qor/flyers/error/error.h"
#include "defaultloghandler.h"

namespace qor {

    qor_pp_signal_func DefaultLogHandler::forward(std::string logMessage)//don't call this outgoing interface directly
    {
        qor_pp_emit(forward,logMessage);
    }

    DefaultLogHandler::forwardFn DefaultLogHandler::GetForwardSignal() const
    {
        return &DefaultLogHandler::forward;
    }

    DefaultLogHandler::DefaultLogHandler(log::Level logLevel) : qor::IssueHandler<log::Log>()
        , m_forward(true)
        , m_writeToStandardOutput(false)
        , m_level(logLevel)
    { }

    std::string DefaultLogHandler::WhereText(const char* module, const char* file, const char* function, int line, const std::string& exceptionText, const std::string& instanceText, const std::string& threadText) const
    {
        return std::format("Module: {0}| File: {1}| Function: {2}| Line: {3}| {4}| {5}| Thread: {6}", 
            module, 
            file, 
            function, 
            line,
            exceptionText,
            instanceText,
            threadText);
    }

    std::string DefaultLogHandler::InstanceText(bool inInstance, const qor::AnyObject& any) const
    {
        return inInstance ?
            std::format("Object at: {0}", any.Ptr()->operator void*()) : 
            "Free function";
    }

    std::string DefaultLogHandler::InExceptionText(bool inException) const
    {
        return inException ? "Exceptional flow" : "Normal flow";
    }

    std::string DefaultLogHandler::MessageText(const std::string_view& level, const std::string& what, const std::string& where, const std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds>& when) const
    {                
        auto nscount = when.time_since_epoch().count();
        return std::vformat("{0}:{3}: |{2} \"{1}\"\n", std::make_format_args(level, what, where, nscount));
    }

    DefaultLogHandler& DefaultLogHandler::Forward(bool forward)
    {
        m_forward = forward;
        return *this;    
    }

    DefaultLogHandler& DefaultLogHandler::WriteToStandardOutput(bool write)
    {
        m_writeToStandardOutput = write;
        return *this;
    }

    DefaultLogHandler& DefaultLogHandler::WriteToFileSystem()
    {
        //Look for a LogFileWriter to write to the filesystem for us
        return *this;
    }

    DefaultLogHandler& DefaultLogHandler::WriteToPipeline()
    {
        //Look for a LogPipelineWriter to write to a custom Pipeline for us
        return *this;
    }

    std::string DefaultLogHandler::GetWhereClause(const qor::log::Log& log) const
    {
        std::string where = "location unknown";
        if(log.HasWhere())
        {
            where = WhereText(log.where().GetModuleContext(),log.where().GetFile(),log.where().GetFunction(),log.where().GetLine(),
                InExceptionText(log.where().GetInException()), InstanceText(log.where().GetInInstance(), log.where().GetObjectContext()), 
                log.where().GetThreadContext());
        }
        return where;
    }

    bool DefaultLogHandler::Handle(const log::Log& log)
    { 
        if(m_level > log.what().GetLevel())
        {
            return false;
        }

        const std::string_view logLevel = log::LevelNames[(int)log.what().GetLevel()];
        std::string logMessage = MessageText(logLevel, log.what().Content(), GetWhereClause(log), log.when().TimePoint());
        
        if(m_forward)
        {
            forward(logMessage);
        }

        if(m_writeToStandardOutput)
        {
            std::cout << logMessage << std::endl;
        }

        return true;
    }

}//qor