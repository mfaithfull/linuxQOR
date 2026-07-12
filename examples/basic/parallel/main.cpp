// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//An Application with a ThreadPool using the QOR SDK
//For a simpler example of the flow of a QOR application see the plumbing project

#include "src/configuration/configuration.h"
#include "src/components/qor/logaggregator/logaggregator.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/log.h"
#include "src/qor/flyers/log/defaultloghandler.h"
#include "src/framework/app/application/application.h"
#include "src/framework/app/application/builder.h"
#include "src/framework/app/role/role.h"
#include "src/framework/io/filesystem/filesystem.h"

using namespace qor;
using namespace qor::components;
using namespace qor::app;
using namespace qor::log;
using namespace qor::io;

constexpr const char* appName = "Parallel";
qor_pp_implement_module(appName)

//A valid partial file name to identify logs from this app
constexpr const char* logTag = "parallel";

qor_pp_module_requires(LogAggregatorService)
qor_pp_module_requires(IFileSystem)
qor_pp_module_requires(ICurrentThread)

void SetupLogging(DefaultLogHandler& logHandler, LogAggregatorService::ref logAggregator);
int ParallelTask();

int main()
{    
    /*This macro is used to gather data about the point of execution 
    for use by logging and error handling. It sets up a FunctionContext Flyer
    making the code self aware of which function is executing, on which object,
    in which module, at which line, in which file and on which thread.*/
    qor_pp_fcontext;

    /*Set up a default log handler to handle logs on the main thread
    this Flyer will be available in all functions on this thread during
    it's lifetime*/
    DefaultLogHandler logHandler(Level::Debug);

    /*We'll need the file system to be able to write logs*/
    ThePlatform(qor_shared)->AddSubsystem<FileSystem>();

    return AppBuilder().Build(appName)(qor_unlocked).SetRole<Role>(
        [&logHandler](ref_of<IRole>::type role)
        {
            qor_pp_fcontext;
            //Add the ThreadPool feature to the Role
            role->AddFeature<thread::ThreadPool>(
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
            role->AddFeature<LogAggregatorService>(
                [&logHandler](LogAggregatorService::ref logAggregator)->void
                {
                    SetupLogging(logHandler, logAggregator);
                }
            );
        }
    ).Run([&logHandler]()->int
        {
            qor_pp_fcontext;
            auto threadPool = AppBuilder().TheApplication(qor_shared)->GetRole(qor_shared)->GetFeature<thread::ThreadPool>();
            size_t runningTaskCount = threadPool->GetCountOfTasksRunning();

            debug("{0} thread is in use by the log aggregator. \
    This log message will be written out by the aggregator while the Main thread that logged it continues in parallel.",runningTaskCount);

            /*Now we submit a task to the pool that will run on the remaining spare thread*/
            std::future<int> taskResult = threadPool->SubmitTask( 
                []()->int
                {
                    return ParallelTask();
                }
            );
            
            debug("While the task is executing we can carry on doing other things, including logging to the shared aggregator.");            

            //wait for the secondary task to finish
            taskResult.wait();

            /*We should disconnect the main thread log handler from the aggregator when we're done as well.
            The extra scope makes sure we let go of the log aggregator, a shared resource, as soon as possible*/
            {
                auto logAggregator = AppBuilder().TheApplication(qor_shared)->GetRole(qor_shared)->GetFeature<LogAggregatorService>();
                disconnect(
                    logHandler, logHandler.GetForwardSignal(), 
                    logAggregator(qor_shared).Receiver(), &LogReceiver::ReceiveLog);
            }

            return 0;
        });
}

/*The initial setting up of the log aggregator and connecting the main thread to it*/
void SetupLogging(DefaultLogHandler& logHandler, LogAggregatorService::ref logAggregator)
{
    /*Connect the forward signal from logHandler to the ReceiveLog slot on the log aggregator's Receiver*/
    connect(
        logHandler, logHandler.GetForwardSignal(), 
        logAggregator(qor_shared).Receiver(), &LogReceiver::ReceiveLog, 
        ConnectionKind::QueuedConnection);

    auto fileSystem = ThePlatform(qor_shared)->GetSubsystem<FileSystem>();
    auto logPath = fileSystem(qor_shared).ApplicationLogPath() / logTag;

    /*Configure the log aggregator to write to the file system and to standard output*/
    logAggregator(qor_shared).Receiver().WriteToFileSystem(logPath, logTag);
    logAggregator(qor_shared).Receiver().WriteToStandardOutput(true);
}

int ParallelTask()
{
    qor_pp_fcontext;
    /*This runs on a pool thread*/
    CurrentThread::GetCurrent().SetName("Task");

    /*Make a log handler on this thread*/
    DefaultLogHandler secondaryLogHandler(Level::Debug);

    /*to get our thread local log handler hooked up to the global Log Aggregator we need to connect them*/
    {
        auto logAggregator = AppBuilder().TheApplication(qor_shared)->GetRole(qor_shared)->GetFeature<LogAggregatorService>();
        /*We connect the forward signal from our local instance of a DefaultLogHandler to the log aggregator's log receiver on the ReceiveLog slot*/
        connect(
            secondaryLogHandler, secondaryLogHandler.GetForwardSignal(), //source instance, source signal function
            logAggregator(qor_shared).Receiver(), &LogReceiver::ReceiveLog, //sink instance, sink slot function
            ConnectionKind::QueuedConnection); //we want a queued connection because we're crossing threads and don't want to wait for reception
    }

    /*Now when we log from this secondary thread, the thread local log handler will forward the message to the aggregator*/
    debug("Log from a task running on a pooled thread");

    CurrentThread::Get().Sleep(5000);

    {
        auto logAggregator = AppBuilder().TheApplication(qor_shared)->GetRole(qor_shared)->GetFeature<LogAggregatorService>();
        /*disconnecting is just like connecting but we don't have to specify the ConnectionKind*/
        disconnect(
            secondaryLogHandler, secondaryLogHandler.GetForwardSignal(), 
            logAggregator(qor_shared).Receiver(), &LogReceiver::ReceiveLog);
    }
    return 0;
}
