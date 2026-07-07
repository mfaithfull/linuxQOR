// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "server.h"
#include "src/qor/flyers/error/defaulterrorhandler.h"
#include "src/qor/flyers/log/debug.h"
#include "src/qor/flyers/log/defaultloghandler.h"
#include "src/platform/platform.h"
#include "src/framework/app/optionparser/getter.h"
#include "src/framework/app/application/builder.h"
#include "src/framework/app/role/role.h"
#include "src/framework/io/network/sockets.h"
#include "src/framework/io/async/service.h"
#include "src/components/io/network/server/netserver.h"
#include "src/components/protocols/echo/echoprotocol.h"
#include "src/components/qor/logaggregator/logaggregator.h"
#include "src/framework/io/filesystem/filesystem.h"

using namespace qor;
using namespace qor::log;
using namespace qor::platform;
using namespace qor::io;
using namespace qor::io::network;
using namespace qor::io::network::components;
using namespace qor::components::protocols;

constexpr const char* logTag = "server";

qor_pp_implement_module(Server::Name)

qor_pp_module_requires(Sockets)
qor_pp_module_requires(AsyncIOService)
qor_pp_module_requires(LogAggregatorService)
qor_pp_module_requires(IFileSystem)
qor_pp_module_requires(ICurrentThread)

using namespace qor::components;

int main(const int argc, const char** argv, char** env)
{
    DefaultErrorHandler errorHandler;
    DefaultLogHandler logHandler(Level::Debug);
    logHandler.WriteToStandardOutput();

    ThePlatform(qor_shared)->AddSubsystem<Sockets>();
    ThePlatform(qor_shared)->AddSubsystem<FileSystem>();

    return AppBuilder().Build<Server>(Server::Name,
        [argc,argv,env](ref_of<Server>::type server)
        {
            app::OptionGetter options(argc, argv, server(qor_shared));
        }
    )->SetRole<app::Role>(
        [&logHandler](ref_of<app::IRole>::type role)
        {
            role->AddFeature<thread::ThreadPool>(
                [](ref_of<thread::ThreadPool>::type threadPool)->void
                {
                    threadPool->SetThreadCount(8);
                    CurrentThread::Get().SetName("Main");
                }                
            );
            role->AddFeature<async::Service>(
                [](ref_of<async::Service>::type ioService)->void
                {
                    PoolInstancer::SetPoolSize<async::Context>(2);
                }
            );
            role->AddFeature<LogAggregatorService>(
                [&logHandler](ref_of<LogAggregatorService>::type logAggregator)->void
                {
                    connect(logHandler, logHandler.GetForwardSignal(), 
                        logAggregator(qor_shared).Receiver(), &LogReceiver::ReceiveLog, ConnectionKind::QueuedConnection);
                    auto fileSystem = ThePlatform(qor_shared)->GetSubsystem<FileSystem>();
                    auto logPath = fileSystem->ApplicationLogPath() / logTag;
                    logAggregator(qor_shared).Receiver().WriteToFileSystem(logPath, logTag);
                    logAggregator(qor_shared).Receiver().WriteToStandardOutput(false);
                }
            );
        }
    ).RunWorkflow(new_ref<NetworkServer>(12345, new_ref<echo::EchoProtocol>()));

    disconnect(logHandler, logHandler.GetForwardSignal(), 
        AppBuilder().TheApplication(qor_shared)->GetRole(qor_shared)->GetFeature<LogAggregatorService>(qor_shared)->Receiver(),
         &LogReceiver::ReceiveLog);
}

