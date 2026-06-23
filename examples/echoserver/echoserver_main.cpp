#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/platform/platform.h"
#include "src/framework/io/filesystem/filesystem.h"
#include "src/framework/app/application/application_builder.h"
#include "src/framework/app/role/role.h"
#include "src/framework/parallel/thread/threadpool.h"
#include "src/framework/app/optionparser/getter.h"
#include "src/framework/io/pipeline/pipeline.h"
#include "src/framework/io/network/sockets.h"
#include "src/framework/parallel/asyncioservice/asyncioservice.h"
#include "src/components/qor/logaggregator/logaggregator.h"
#include "src/qor/instance/pool.h"

#include "echoserverapp.h"
#include "serverworkflow.h"
#include "errorhandler.h"
#include "serverloghandler.h"

using namespace qor;
using namespace qor::platform;
using namespace qor::components;
using namespace qor::app::optparser;
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
    ServerLogHandler logHandler(Level::Debug);

    ThePlatform(qor_shared)->AddSubsystem<Sockets>();             //What subsystems it needs
    ThePlatform(qor_shared)->AddSubsystem<FileSystem>();

    return AppBuilder().Build<EchoServerApp>(appName,   //What it is
        [argc,argv](ref_of<EchoServerApp>::type app)
        {
            OptionGetter options(argc, argv, app(qor_shared));
        }
    )->SetRole<Role>(                                   //What features it has
        [&logHandler](ref_of<IRole>::type role)
        {
            role->AddFeature<thread::ThreadPool>(
                [](ref_of<thread::ThreadPool>::type threadPool)->void
                {
                    threadPool(qor_shared).SetThreadCount(16);
                    CurrentThread::GetCurrent().SetName("Server Main");
                }
            );
            role->AddFeature<async::AsyncIOService>(
                [](ref_of<async::AsyncIOService>::type ioService)->void
                {
                    PoolInstancer::SetPoolSize<async::AsyncIOContext>(4);
                }
            );
            role->AddFeature<LogAggregatorService>(
                [&logHandler](ref_of<LogAggregatorService>::type logAggregator)->void
                {
                    qor::connect(logHandler, &qor::components::LogHandler::forward, 
                        logAggregator(qor_shared).Receiver(), &qor::components::LogReceiver::ReceiveLog, qor::ConnectionKind::QueuedConnection);
                    auto fileSystem = ThePlatform(qor_shared)->GetSubsystem<FileSystem>();
                    auto logPath = fileSystem(qor_shared).ApplicationLogPath() / "echoserver";
                    logAggregator(qor_shared).Receiver().WriteToFileSystem(logPath, "echoserver");
                    logAggregator(qor_shared).Receiver().WriteToStandardOutput(true);
                }
            );
        }
    ).RunWorkflow<ServerWorkflow>();                    //What it does
}

qor_pp_implement_module(appName)

