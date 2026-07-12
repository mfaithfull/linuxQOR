// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#ifdef NDEBUG
#include <buildnumber.h>
#endif

#include "src/qor/flyers/error/defaulterrorhandler.h"
#include "src/qor/flyers/log/defaultloghandler.h"
#include "src/framework/app/application/builder.h"
#include "src/framework/app/role/role.h"
#include "src/framework/parallel/thread/threadpool.h"
#include "src/components/io/network/client/netclient.h"
#include "src/framework/io/filesystem/filesystem.h"
#include "src/components/qor/logaggregator/logaggregator.h"
#include "workflow.h"

using namespace qor;
using namespace qor::log;
using namespace qor::io;
using namespace qor::io::components;

const char* appName = "Example Client";
const char* logTag = "client";

qor_pp_implement_module(appName)

qor_pp_module_requires(Sockets)
qor_pp_module_requires(LogAggregatorService)
qor_pp_module_requires(IFileSystem)
qor_pp_module_requires(ICurrentThread)

void SetupLogging(DefaultLogHandler& logHandler, qor::components::LogAggregatorService::ref logAggregator)
{
    qor::connect(
        logHandler, logHandler.GetForwardSignal(), 
        logAggregator(qor_shared).Receiver(), &qor::components::LogReceiver::ReceiveLog, 
        qor::ConnectionKind::QueuedConnection);

    auto fileSystem = ThePlatform(qor_shared)->GetSubsystem<FileSystem>();
    auto logPath = fileSystem->ApplicationLogPath() / logTag;
    logAggregator(qor_shared).Receiver().WriteToFileSystem(logPath, logTag);
    logAggregator(qor_shared).Receiver().WriteToStandardOutput(true);
}

int main(const int argc, const char** argv, char**)
{
    DefaultErrorHandler errorHandler;
    DefaultLogHandler logHandler(Level::Debug);    

    ThePlatform(qor_shared)->AddSubsystem<Sockets>();
    ThePlatform(qor_shared)->AddSubsystem<FileSystem>();

    return AppBuilder().Build(appName)(qor_unlocked).
        SetRole<app::Role>(
            [&logHandler](app::IRole::ref role)
            {
                role->AddFeature<thread::ThreadPool>(
                    [](ref_of<thread::ThreadPool>::type threadPool)->void
                    {
                        threadPool->SetThreadCount(1);
                        CurrentThread::Get().SetName("Main");
                    }
                );
                role->AddFeature<qor::components::LogAggregatorService>(
                    [&logHandler](qor::components::LogAggregatorService::ref logAggregator)->void
                    {
                        SetupLogging(logHandler, logAggregator);
                    }
                );
            }
        ).RunWorkflow<ClientWorkflow>();
}
