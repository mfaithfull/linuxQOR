// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//text reading example

#include "sdk/app.h"
#include "sdk/memory.h"
#include "sdk/error.h"
#include "sdk/log.h"
#include "src/qor/tdd/profiling/profiling.h"
#include "src/qor/tdd/profiling/profilereceiver.h"
#include "src/qor/memory/instance/threadsingleton.h"
#include "src/components/qor/logaggregator/logaggregator.h"
#include "src/components/data/formats/text/parser/textparser.h"
#include "src/components/data/pipeline/sinks/parsersink/parsersink.h"
#include "src/components/io/pipeline/connectors/fileconnector/fileconnector.h"
#include "src/framework/io/filesystem/filesystem.h"
#include "src/platform/platform.h"
#include "textreader.h"

qor_pp_module_requires(ICurrentThread);
qor_pp_module_requires(ThreadHeap);
qor_pp_module_requires(IFileSystem);
qor_pp_module_requires(LogAggregatorService)

constexpr const char* appName = "Text Read";
constexpr const char* logTag = "textread";

using namespace qor;
using namespace qor::log;
using namespace qor::io;
using namespace qor::io::filesystem;
using namespace qor::io::components;
using namespace qor::components;

qor_pp_implement_module(appName)

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

class Test_ProfileReporter : public ProfileReceiver
{
public:

    std::chrono::duration<int64_t, std::micro> m_recordedDuration;

    virtual void Profile(const std::chrono::duration<int64_t, std::micro> durationMicroseconds, IFunctionContext* fContext)
    {
        m_recordedDuration = durationMicroseconds;
	    issue<log::Informative, const std::string&>(std::format("Profile: {0}", durationMicroseconds), fContext);
    }
};

int main(const int /*argc*/, const char** /*argv*/, char** /*env*/)
{
    ThePlatform(qor_shared)->AddSubsystem<FileSystem>();
    qor_pp_fcontext;
    DefaultLogHandler logHandler(log::Level::Debug);

    return AppBuilder().Build(appName)(qor_unlocked).SetRole<app::Role>(
        [&logHandler](ref_of<app::IRole>::type role)
        {
            role->AddFeature<thread::ThreadPool>(
                [](ref_of<thread::ThreadPool>::type threadPool)
                {
                    threadPool->SetThreadCount(2);
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
        []()->int
        {            
            qor_pp_fcontext;

            auto filesystem = ThePlatform(qor_shared)->GetSubsystem<FileSystem>();
            Path testsPath("F:/Develop/linuxQOR/test/data");

            //std::string test = "중앙일보";
            
            TextReader textReader;
            /*
            auto text = textReader(Index(testsPath, "noBOM.txt"));

            qor_pp_assert_that(*text).isEqualTo(test);
            log::debug(text);

            text = textReader(Index(testsPath, "utf8BOM.txt"));

            qor_pp_assert_that(*text).isEqualTo("UTF-8 BOM");

            text = textReader(Index(testsPath, "utf16LEBOM.txt"));

            qor_pp_assert_that(*text).isEqualTo("UTF-16LE BOM");

            text = textReader(Index(testsPath, "utf16BEBOM.txt"));

            qor_pp_assert_that(*text).isEqualTo("UTF-16BE BOM");
            */
#include qor_pp_profile_begin
            {
                Test_ProfileReporter reporter;
                FunctionProfiler profiler(dynamic_cast<ProfileReceiver*>(&reporter), qor_pp_profile_enabled);
                auto text = textReader(Index(testsPath, "Alice.txt"));
            }
#include qor_pp_profile_end            
            return EXIT_SUCCESS;
        });
}
