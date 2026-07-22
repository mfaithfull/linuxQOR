// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "prole.h"
#include "src/framework/app/application/builder.h"

using namespace qor;
using namespace qor::app;
using namespace qor::components;

qor_pp_module_requires(LogAggregatorService)
qor_pp_module_requires(IFileSystem)

/*The initial setting up of the log aggregator and connecting the main thread to it*/
void ParallelAppRole::SetupLogging(DefaultLogHandler& logHandler, LogAggregatorService::ref logAggregator)
{
    /*Connect the forward signal from logHandler to the ReceiveLog slot on the log aggregator's Receiver*/
    connect(
        logHandler, logHandler.GetForwardSignal(), 
        logAggregator(qor_shared).Receiver(), &LogReceiver::ReceiveLog, 
        ConnectionKind::QueuedConnection);

    auto fileSystem = ThePlatform(qor_shared)->GetSubsystem<io::FileSystem>();
    auto logPath = fileSystem(qor_shared).ApplicationLogPath() / logTag;

    /*Configure the log aggregator to write to the file system and to standard output*/
    logAggregator(qor_shared).Receiver().WriteToFileSystem(logPath, logTag);
    logAggregator(qor_shared).Receiver().WriteToStandardOutput(true);
}

void ParallelAppRole::Setup()
{
    /*We'll need the file system to be able to write logs*/
    ThePlatform(qor_shared)->AddSubsystem<io::FileSystem>();

    //Add the ThreadPool feature to the Role
    IRole::template AddFeature<thread::ThreadPool>(
        [](ref_of<thread::ThreadPool>::type threadPool)
        {
            /*Put 2 threads in the pool in addition to the main thread already executing*/
            threadPool->SetThreadCount(2);
            /*Label the main thread so we can idenity it in the logs*/
            CurrentThread::GetCurrent().SetName("Main");
        }
    );

    /*Add the log aggregator service which uses the ThreadPool feature to run in the background.
    It will consume 1 of the pool threads*/
    IRole::template AddFeature<LogAggregatorService>(
        [this](LogAggregatorService::ref logAggregator)->void
        {
            auto logHandler = new_ref< IssueHandler<log::Log> >();
            DefaultLogHandler* defaultLogHandler = dynamic_cast<DefaultLogHandler*>(logHandler.operator qor::IssueHandler<log::Log> *());
            if(defaultLogHandler)
            {
                SetupLogging(*defaultLogHandler, logAggregator);
            }
        }
    );

    Role::Setup();
}

void ParallelAppRole::Shutdown()
{
    auto logHandler = new_ref< IssueHandler<log::Log> >();
    DefaultLogHandler* defaultLogHandler = dynamic_cast<DefaultLogHandler*>(logHandler.operator qor::IssueHandler<log::Log> *());
    if(defaultLogHandler)
    {
        disconnect(*defaultLogHandler, defaultLogHandler->GetForwardSignal(), 
            AppBuilder().TheApplication(qor_shared)->GetRole(qor_shared)->GetFeature<LogAggregatorService>(qor_shared)->Receiver(),
            &LogReceiver::ReceiveLog);
    }

    Role::Shutdown();
}