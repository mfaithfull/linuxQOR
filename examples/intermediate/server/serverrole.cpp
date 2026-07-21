// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "serverrole.h"

using namespace qor;
using namespace qor::log;
using namespace qor::platform;
using namespace qor::io;
using namespace qor::components;

ServerRole::ServerRole() = default;

ServerRole::~ServerRole() = default;

void ServerRole::Setup()
{
    IRole::AddFeature<thread::ThreadPool>(
        [](ref_of<thread::ThreadPool>::type threadPool)->void
        {
            threadPool->SetThreadCount(8);
            CurrentThread::Get().SetName("Main");
        }                
    );
    IRole::AddFeature<async::Service>(
        [](ref_of<async::Service>::type /*ioService*/)->void
        {
            PoolInstancer::SetPoolSize<async::Context>(2);
        }
    );
    IRole::AddFeature<LogAggregatorService>(
        [](ref_of<LogAggregatorService>::type logAggregator)->void
        {
            auto logHandler = new_ref< IssueHandler<Log> >();
            DefaultLogHandler* defaultLogHandler = dynamic_cast<DefaultLogHandler*>(logHandler.operator qor::IssueHandler<Log> *());

            if(defaultLogHandler)
            {
                connect(*defaultLogHandler, defaultLogHandler->GetForwardSignal(), 
                    logAggregator(qor_shared).Receiver(), &LogReceiver::ReceiveLog, ConnectionKind::QueuedConnection);
                auto fileSystem = ThePlatform(qor_shared)->GetSubsystem<FileSystem>();
                auto logPath = fileSystem->ApplicationLogPath() / logTag;
                logAggregator(qor_shared).Receiver().WriteToFileSystem(logPath, logTag);
                logAggregator(qor_shared).Receiver().WriteToStandardOutput(false);
            }
        }
    );

    qor::app::Role::Setup();
}

void ServerRole::Shutdown()
{
    auto logHandler = new_ref< IssueHandler<Log> >();
    DefaultLogHandler* defaultLogHandler = dynamic_cast<DefaultLogHandler*>(logHandler.operator qor::IssueHandler<Log> *());
    if(defaultLogHandler)
    {
        disconnect(*defaultLogHandler, defaultLogHandler->GetForwardSignal(), 
            AppBuilder().TheApplication(qor_shared)->GetRole(qor_shared)->GetFeature<LogAggregatorService>(qor_shared)->Receiver(),
            &LogReceiver::ReceiveLog);
    }
    qor::app::Role::Shutdown();
}
