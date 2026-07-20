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
#include "src/framework/io/filesystem/filesystem.h"
#include "src/components/qor/logaggregator/logaggregator.h"
#include "src/framework/ui/serial/serialconsole.h"
#include "src/framework/ui/terminal/terminal.h"

using namespace qor;
using namespace qor::log;
using namespace qor::io;

const char* appName = "Terminal App Example";
const char* logTag = "termapp";

qor_pp_implement_module(appName)

qor_pp_module_requires(LogAggregatorService)
qor_pp_module_requires(IFileSystem)
qor_pp_module_requires(ICurrentThread)
qor_pp_module_requires(SerialConsole)
qor_pp_module_requires(Terminal)

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
                role->AddFeature<qor::ui::SerialConsole>();
                role->AddFeature<qor::ui::Terminal>();
            }
        ).Run([]()->int
            {
                auto serialConsole = AppBuilder().TheApplication(qor_shared)->GetRole(qor_shared)->GetFeature<qor::ui::SerialConsole>();

                auto serconRenderer = serialConsole->GetRenderer();
                auto serconSurface = serialConsole->CreateSurface();
                ui::sercon::TokenBuffer* old;                
                serconRenderer->SwapTokenBuffer(old, serconSurface);
                serconRenderer->ClearWindow(0,0,80,25);
                serconRenderer->FillWindow(0,0,80,1,qor::ui::sercon::TokenBuffer::Tokens::MID_FILL);
                serconRenderer->FillWindow(0,1,80,1,qor::ui::sercon::TokenBuffer::Tokens::DARK_FILL);
                serconRenderer->FillWindow(0,2,80,1,qor::ui::sercon::TokenBuffer::Tokens::MID_FILL);
                serconRenderer->FillWindow(0,3,80,1,qor::ui::sercon::TokenBuffer::Tokens::DARK_FILL);
                serconRenderer->FillWindow(0,4,80,1,qor::ui::sercon::TokenBuffer::Tokens::MID_FILL);
                serconRenderer->FillWindow(0,5,80,1,qor::ui::sercon::TokenBuffer::Tokens::DARK_FILL);
                serconRenderer->FillWindow(0,6,80,1,qor::ui::sercon::TokenBuffer::Tokens::MID_FILL);
                serconRenderer->FillWindow(0,7,80,1,qor::ui::sercon::TokenBuffer::Tokens::DARK_FILL);
                serconRenderer->FillWindow(0,8,80,1,qor::ui::sercon::TokenBuffer::Tokens::MID_FILL);
                serconRenderer->FillWindow(0,9,80,1,qor::ui::sercon::TokenBuffer::Tokens::DARK_FILL);
                serconRenderer->FillWindow(0,10,80,1,qor::ui::sercon::TokenBuffer::Tokens::MID_FILL);
                serconRenderer->FillWindow(0,11,80,1,qor::ui::sercon::TokenBuffer::Tokens::DARK_FILL);
                serconRenderer->FillWindow(0,12,80,1,qor::ui::sercon::TokenBuffer::Tokens::MID_FILL);
                serconRenderer->FillWindow(0,13,80,1,qor::ui::sercon::TokenBuffer::Tokens::DARK_FILL);
                serconRenderer->FillWindow(0,14,80,1,qor::ui::sercon::TokenBuffer::Tokens::MID_FILL);
                serconRenderer->FillWindow(0,15,80,1,qor::ui::sercon::TokenBuffer::Tokens::DARK_FILL);
                serconRenderer->FillWindow(0,16,80,1,qor::ui::sercon::TokenBuffer::Tokens::MID_FILL);
                serconRenderer->FillWindow(0,17,80,1,qor::ui::sercon::TokenBuffer::Tokens::DARK_FILL);
                serconRenderer->FillWindow(0,18,80,1,qor::ui::sercon::TokenBuffer::Tokens::MID_FILL);
                serconRenderer->FillWindow(0,19,80,1,qor::ui::sercon::TokenBuffer::Tokens::DARK_FILL);
                serconRenderer->FillWindow(0,20,80,1,qor::ui::sercon::TokenBuffer::Tokens::MID_FILL);
                serconRenderer->FillWindow(0,21,80,1,qor::ui::sercon::TokenBuffer::Tokens::DARK_FILL);
                serconRenderer->FillWindow(0,22,80,1,qor::ui::sercon::TokenBuffer::Tokens::MID_FILL);
                serconRenderer->FillWindow(0,23,80,1,qor::ui::sercon::TokenBuffer::Tokens::DARK_FILL);
                serconRenderer->FillWindow(0,24,80,1,qor::ui::sercon::TokenBuffer::Tokens::MID_FILL);                
                serconRenderer->DrawWindow(1,1,60,5);
                serconRenderer->FillWindow(2,2,58,3, qor::ui::sercon::TokenBuffer::Tokens::DARK_FILL);
                serconRenderer->DrawString(2,2, _TXT("Hello Serial Console"));
                serconRenderer->Present();
                serconRenderer->SwapTokenBuffer(old, old);

                //auto terminal = AppBuilder().TheApplication(qor_shared)->GetRole(qor_shared)->GetFeature<qor::ui::Terminal>();
                //auto toolkit = terminal(qor_shared).GetToolkit();
                return EXIT_SUCCESS;
            });
}
