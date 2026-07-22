// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "termapprole.h"
#include "src/framework/parallel/thread/threadpool.h"
#include "src/framework/io/filesystem/filesystem.h"
#include "src/components/qor/logaggregator/logaggregator.h"

using namespace qor;
using namespace qor::log;
using namespace qor::platform;
using namespace qor::io;
using namespace qor::components;

qor_pp_module_requires(LogAggregatorService)
qor_pp_module_requires(IFileSystem)
qor_pp_module_requires(ICurrentThread)
qor_pp_module_requires(Terminal)

TermAppRole::TermAppRole() = default;

TermAppRole::~TermAppRole() = default;

void TermAppRole::Setup()
{
    ThePlatform(qor_shared)->AddSubsystem<FileSystem>();
    
    IRole::AddFeature<thread::ThreadPool>(
        [](ref_of<thread::ThreadPool>::type threadPool)->void
        {
            threadPool->SetThreadCount(2);
            CurrentThread::Get().SetName("Main");
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

    IRole::AddFeature<qor::ui::Terminal>();

    qor::app::Role::Setup();
}

void TermAppRole::Shutdown()
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
