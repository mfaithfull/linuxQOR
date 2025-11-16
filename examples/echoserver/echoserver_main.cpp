#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/platform/platform.h"
#include "src/platform/filesystem/filesystem.h"
#include "src/framework/application/application_builder.h"
#include "src/framework/role/role.h"
#include "src/framework/thread/threadpool.h"
#include "src/components/framework/optionparser/getter.h"
#include "src/framework/pipeline/pipeline.h"
#include "src/platform/network/sockets.h"
#include "src/framework/asyncioservice/asyncioservice.h"
#include "src/components/framework/logaggregator/logaggregator.h"
#include "src/qor/instance/pool.h"

#include "echoserverapp.h"
#include "serverworkflow.h"
#include "errorhandler.h"
#include "serverloghandler.h"

using namespace qor;
using namespace qor::platform;
using namespace qor::framework;
using namespace qor::components;
using namespace qor::components::optparser;
using namespace qor::pipeline;
using namespace qor::network;
using namespace qor::log;

#define appName "Echo Server"

qor_pp_module_requires(Sockets)
qor_pp_module_requires(AsyncIOService)
qor_pp_module_requires(LogAggregatorService)
qor_pp_module_requires(IFileSystem)
qor_pp_module_requires(ICurrentThread)

int main(const int argc, const char** argv, char**)
{	
    ErrorHandler errorHandler;
    ServerLogHandler logHandler;

    ThePlatform()->AddSubsystem<Sockets>();             //What subsystems it needs
    ThePlatform()->AddSubsystem<FileSystem>();

    return AppBuilder().Build<EchoServerApp>(appName,   //What it is
        [](ref_of<EchoServerApp>::type app, const int argc, const char** argv, const char** env)
        {
            OptionGetter options(argc, argv, app());
        }
    )->SetRole<Role>(                                   //What features it has
        [&logHandler](ref_of<IRole>::type role)
        {
            role->AddFeature<ThreadPool>([](ref_of<ThreadPool>::type threadPool)->void
            {
                threadPool->SetThreadCount(5);
                CurrentThread::GetCurrent().SetName("Server Main");
            });
            role->AddFeature<AsyncIOService>([](ref_of<AsyncIOService>::type ioService)->void
            {
                PoolInstancer::SetPoolSize<AsyncIOContext>(2);
            });
            role->AddFeature<LogAggregatorService>(
                [&logHandler](ref_of<LogAggregatorService>::type logAggregator)->void
                {
                    qor::connect(logHandler, &qor::components::LogHandler::forward, 
                        logAggregator->Receiver(), &qor::components::LogReceiver::ReceiveLog, qor::ConnectionKind::QueuedConnection);
                    auto fileSystem = ThePlatform()->GetSubsystem<FileSystem>();
                    auto logPath = fileSystem->ApplicationLogPath() / "echoserver";
                    logAggregator->Receiver().WriteToFileSystem(logPath, "echoserver");
                    logAggregator->Receiver().WriteToStandardOutput(false);
                }
            );
        }
    ).RunWorkflow<ServerWorkflow>();                    //What it does
}

qor_pp_implement_module(appName)

