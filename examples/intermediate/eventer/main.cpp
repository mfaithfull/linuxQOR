// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cassert>

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/defaultloghandler.h"
#include "src/framework/app/role/role.h"
#include "src/platform/platform.h"
#include "src/framework/io/filesystem/filesystem.h"
#include "src/components/qor/logaggregator/logaggregator.h"
#include "src/framework/app/application/builder.h"
#include "src/framework/app/optionparser/getter.h"
#include "src/framework/app/role/getfeature.h"
#include "src/framework/event/events/eventsource.h"
#include "src/framework/event/events/eventsink.h"

const char* logTag = "eventer";

qor_pp_module_requires(LogAggregatorService)
qor_pp_module_requires(IFileSystem)
qor_pp_module_requires(ICurrentThread)

using namespace qor;
using namespace qor::app;
using namespace qor::platform;
using namespace qor::io;
using namespace qor::components;

void SetupLogging(DefaultLogHandler& logHandler, LogAggregatorService::ref logAggregator)
{    
    connect(
        logHandler, logHandler.GetForwardSignal(), 
        logAggregator(qor_shared).Receiver(), &LogReceiver::ReceiveLog, 
        ConnectionKind::QueuedConnection);

    auto fileSystem = ThePlatform(qor_shared)->GetSubsystem<FileSystem>();
    auto logPath = fileSystem(qor_shared).ApplicationLogPath() / logTag;

    //Configure the log aggregator to write to the file system and to standard output
    logAggregator(qor_shared).Receiver().WriteToFileSystem(logPath, logTag);
    logAggregator(qor_shared).Receiver().WriteToStandardOutput(true);
}

qor_pp_implement_module("eventer")

struct customEvent
{
    int a;
    float b;
    void Reset()
    {
        a = 0;
        b = 3.141593f;
    }
};

int main(const int /*argc*/, const char** /*argv*/, char** /*env*/)
{    
    qor_pp_fcontext;
    DefaultLogHandler logHandler(log::Level::Debug);
    ThePlatform(qor_shared)->AddSubsystem<FileSystem>();

    return AppBuilder().Build(
        "eventer"
    )->SetRole<Role>(
        [&logHandler](ref_of<IRole>::type role)
        {
            qor_pp_fcontext;

            role->AddFeature<thread::ThreadPool>(
                [](ref_of<thread::ThreadPool>::type threadPool)
                {
                    threadPool->SetThreadCount(6);
                    CurrentThread::GetCurrent().SetName("Main");
                }
            );
            
            role->AddFeature<LogAggregatorService>(
                [&logHandler](LogAggregatorService::ref logAggregator)->void
                {
                    SetupLogging(logHandler, logAggregator);
                }
            );
            
        }
    ).Run(
        [&logHandler]()->int
        {
            qor::events::EventQueue eventQueue(100);
            qor::events::Source<customEvent> eventSource(0x00005678, 10, &eventQueue);
            qor::events::Sink eventSink(&eventQueue);

            customEvent* custom = eventSource.GetMessage(56);
            custom->a = 101;
            eventSource.Send();
            
            auto e = eventSink.GetMessage();
            customEvent cevt = *(reinterpret_cast<customEvent*>(e.data));
            e.release();//Release the message so it can be recycled by the sender.

            assert(cevt.a == 101);
            return 0;
        });
}
