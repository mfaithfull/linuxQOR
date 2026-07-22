// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//An Application with a ThreadPool using the QOR SDK
//For a simpler example of the flow of a QOR application see the plumbing project

/*This example sets up a Thread Pool and Runs a background Log Aggregator on it.
The Log Aggregator collects logs from any thread, sent to it by signals, from
the individual per thread Log Handlers.
It also demonstrates how to wait for a task to complete using a std::future*/

#include "sdk/app.h"
#include "prole.h"
#include "src/framework/app/role/getfeature.h"

using namespace qor;
using namespace qor::components;
using namespace qor::app;
using namespace qor::log;
using namespace qor::io;

qor_pp_module_requires(ICurrentThread)
qor_pp_implement_module("Parallel")

int ParallelTask();

int main()
{    
    qor_pp_fcontext;

    DefaultLogHandler logHandler(Level::Debug);

    qor_pp_run_role(ParallelAppRole)(
    [&logHandler]()->int
    {
        qor_pp_fcontext;
        auto threadPool = GetFeature<thread::ThreadPool>();
        size_t runningTaskCount = threadPool->GetCountOfTasksRunning();

        debug("{0} thread is in use by the log aggregator. This log message will be written out by the aggregator while the Main thread that logged it continues in parallel.",runningTaskCount);

        /*Now we submit a task to the pool that will run on the remaining spare thread*/
        std::future<int> taskResult = threadPool->SubmitTask( 
            []()->int
            {
                return ParallelTask();
            }
        );
        
        debug("While the task is executing we can carry on doing other things, including logging to the shared log aggregator.");
        debug("At this point there may be 3 things happening at once. The main thread, the ParellelTask running on a pool thread and the Log Aggregator running on another pool thread.");

        //wait for the secondary task to finish and return the result;        
        return taskResult.get();
    });
}

int ParallelTask()
{
    qor_pp_fcontext;
    /*This runs on a pool thread*/
    CurrentThread::GetCurrent().SetName("Task");
    //Make a log handler on this thread
    DefaultLogHandler threadLocalLogHandler(Level::Debug);
    /*To get our thread local log handler hooked up to the global Log Aggregator we need to connect them*/
    {
        auto logAggregator = GetFeature<LogAggregatorService>();
        /*We connect the forward signal from our local instance of a DefaultLogHandler to the log aggregator's log receiver on the ReceiveLog slot*/
        connect(
            threadLocalLogHandler, threadLocalLogHandler.GetForwardSignal(), //source instance, source signal function
            logAggregator(qor_shared).Receiver(), &LogReceiver::ReceiveLog, //sink instance, sink slot function
            ConnectionKind::QueuedConnection); //we want a queued connection because we're crossing threads and don't want to wait for reception
    }

    /*Now when we log from this secondary thread, the thread local log handler will forward the message to the Aggregator*/
    debug("Log from a task running on a pool thread");
    CurrentThread::Get().Sleep(5000);//Simulate doing a lot more work

    {
        auto logAggregator = GetFeature<LogAggregatorService>();
        //disconnecting is just like connecting but we don't have to specify the ConnectionKind
        disconnect(
            threadLocalLogHandler, threadLocalLogHandler.GetForwardSignal(), 
            logAggregator(qor_shared).Receiver(), &LogReceiver::ReceiveLog);
    }
    return 0;
}
