// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>
#include <format>
#include "src/qor/flyers/error/error.h"
#include "loghandler.h"

namespace qor{ namespace components {

    LogHandler::LogHandler(log::Level logLevel) : qor::IssueHandler<log::Log>()        
        , m_forward(true)
        , m_writeToStandardOutput(false)
        , m_level(logLevel)
    { }

    LogHandler::LogHandler() : LogHandler(log::Level::Debug)
    { }

    LogHandler::~LogHandler() noexcept { }

    std::string LogHandler::WhereText(const char* module, const char* file, const char* function, int line, const std::string& exceptionText, const std::string& instanceText, const std::string& threadText) const
    {
        return std::format("Module: {0}, File: {1}, Function: {2}, Line: {3} with runtime context, {4}, {5} on thread {6}", 
            module, 
            file, 
            function, 
            line,
            exceptionText,
            instanceText,
            threadText);
    }

    std::string LogHandler::InstanceText(bool inInstance, const AnyObject& any) const
    {
        return inInstance ?
            std::format("on an instance of an object at {0}", any.Ptr()->operator void*()) : 
            "in a free function or lambda";
    }

    std::string LogHandler::InExceptionText(bool inException) const
    {
        return inException ? "inside an exception handler" : "outside any exception handler";
    }

    std::string LogHandler::MessageText(const std::string_view& level, const std::string& what, const std::string& where, const std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds>& when) const
    {
        auto nscount = when.time_since_epoch().count();        
        return std::format("{0}: \"{1}\" \nLogged from {2} at time {3}:",level, what, where, nscount);
    }

    LogHandler& LogHandler::Forward(bool forward)
    {
        m_forward = forward;
        return *this;    
    }

    LogHandler& LogHandler::WriteToStandardOutput(bool write)
    {
        m_writeToStandardOutput = write;
        return *this;
    }

    LogHandler& LogHandler::WriteToFileSystem()
    {
        //Look for a LogFileWriter to write to the filesystem for us
        return *this;
    }

    LogHandler& LogHandler::WriteToPipeline()
    {
        //Look for a LogPipelineWriter to write to a custom Pipeline for us
        return *this;
    }

    std::string LogHandler::GetWhereClause(const qor::log::Log& log) const
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

    bool LogHandler::Handle(const log::Log& log)
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

    qor_pp_signal_func LogHandler::forward(std::string logMessage)//don't call this outgoing interface directly
    {
        qor_pp_emit(forward,logMessage);
    }

}}//qor::components
