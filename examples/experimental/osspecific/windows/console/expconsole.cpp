
#include "src/configuration/configuration.h"

#include <iostream>

#include "src/platform/os/windows/app/exebootstrap/winqorexeboot.h"
#include "src/platform/os/windows/ui/wincon/console.h"
#include "src/platform/os/windows/ui/wincon/termcon.h"
#include "src/platform/os/windows/ui/wincon/graphcon.h"

using namespace qor;
using namespace qor::ui::win;
using namespace qor::platform::win;
using namespace std;

qor_pp_module_requires(ICurrentThread);

void ReportInputMode(Console& console)
{
    auto inputMode = console.GetInput()->GetMode();

    if(inputMode & static_cast<unsigned long>(ConsoleInputModeFlags::ProcessedInput))
    {
        console.WriteLine(L"Input processing is on.");
    }
    else
    {
        console.WriteLine(L"Input processing is off.");
        inputMode |= static_cast<unsigned long>(ConsoleInputModeFlags::ProcessedInput);
        console.WriteLine(L"Turning Input processing on.");
    }

    if(inputMode & static_cast<unsigned long>(ConsoleInputModeFlags::LineInput))
    {
        console.WriteLine(L"Line input enabled.");
        inputMode &= ~(static_cast<unsigned long>(ConsoleInputModeFlags::LineInput));
        console.WriteLine(L"Turning Line input off.");
    }
    else
    {
        console.WriteLine(L"Line input disabled.");
    }
    
    if(inputMode & static_cast<unsigned long>(ConsoleInputModeFlags::EchoInput))
    {
        console.WriteLine(L"Echo enabled.");
        inputMode &= ~(static_cast<unsigned long>(ConsoleInputModeFlags::EchoInput));
        console.WriteLine(L"Turning Echo off.");
    }
    else
    {
        console.WriteLine(L"Echo disabled.");
    }

    if(inputMode & static_cast<unsigned long>(ConsoleInputModeFlags::WindowInput))
    {
        console.WriteLine(L"Window input enabled.");
    }
    else
    {
        console.WriteLine(L"Window input disabled.");
        inputMode |= static_cast<unsigned long>(ConsoleInputModeFlags::WindowInput);
        console.WriteLine(L"Turning Window input on.");

    }

    if(inputMode & static_cast<unsigned long>(ConsoleInputModeFlags::MouseInput))
    {
        console.WriteLine(L"Mouse input enabled.");
    }
    else
    {
        console.WriteLine(L"Mouse input disabled.");
        inputMode |= static_cast<unsigned long>(ConsoleInputModeFlags::MouseInput);
        console.WriteLine(L"Turning Mouse input on.");
    }

    if(inputMode & static_cast<unsigned long>(ConsoleInputModeFlags::InsertMode))
    {
        console.WriteLine(L"Insert Mode on.");
        inputMode &= ~(static_cast<unsigned long>(ConsoleInputModeFlags::InsertMode));
        console.WriteLine(L"Turning Insert mode off.");
    }
    else
    {
        console.WriteLine(L"Insert Mode off.");        
    }

    if(inputMode & static_cast<unsigned long>(ConsoleInputModeFlags::QuickEditMode))
    {
        console.WriteLine(L"Quick Edit Mode on.");
        inputMode &= ~(static_cast<unsigned long>(ConsoleInputModeFlags::QuickEditMode));
        console.WriteLine(L"Turning Quick Edit mode off.");
    }
    else
    {
        console.WriteLine(L"Quick Edit Mode off.");
    }

    if(inputMode & static_cast<unsigned long>(ConsoleInputModeFlags::ExtendedFlags))
    {
        console.WriteLine(L"Extended Flags true.");
    }
    else
    {
        console.WriteLine(L"Extended Flags false.");
    }

    if(inputMode & static_cast<unsigned long>(ConsoleInputModeFlags::AutoPosition))
    {
        console.WriteLine(L"Auto Position on.");
    }
    else
    {
        console.WriteLine(L"Auto Position off.");
    }

    if(inputMode & static_cast<unsigned long>(ConsoleInputModeFlags::VertualTerminalInput))
    {
        console.WriteLine(L"Virtual Terminal Input on.");
    }
    else
    {
        console.WriteLine(L"Virtual Terminal Input off.");
        inputMode |= static_cast<unsigned long>(ConsoleInputModeFlags::VertualTerminalInput);
        console.WriteLine(L"Turning Virtual Terminal input on.");
    }
}


void ReportOutputMode(Console& console)
{
    auto outputMode = console.GetActiveScreenBuffer()->GetMode();

    if(outputMode & static_cast<unsigned long>(ConsoleOutputModeFlags::ProcessedOutput))
    {
        console.WriteLine(L"Output processing is on.");
    }
    else
    {
        console.WriteLine(L"Output processing is off.");
        outputMode |= static_cast<unsigned long>(ConsoleOutputModeFlags::ProcessedOutput);
        console.WriteLine(L"Turning output processing on.");
    }

    if(outputMode & static_cast<unsigned long>(ConsoleOutputModeFlags::WrapAtEOL))
    {
        console.WriteLine(L"Wrap at EOL is on.");
        //outputMode &= ~(static_cast<unsigned long>(ConsoleOutputModeFlags::WrapAtEOL));
        //console.WriteLine(L"Turning Wrap at EOL off.");
    }
    else
    {
        console.WriteLine(L"Wrap at EOL is off.");
    }

    if(outputMode & static_cast<unsigned long>(ConsoleOutputModeFlags::VirtualTerminalProcessing))
    {
        console.WriteLine(L"Virtual Terminal Processing is on.");
    }
    else
    {
        console.WriteLine(L"Virtual Terminal Processing is off.");
        outputMode |= static_cast<unsigned long>(ConsoleOutputModeFlags::VirtualTerminalProcessing);
        console.WriteLine(L"Turning Virtual Terminal Processing on.");

    }

    if(outputMode & static_cast<unsigned long>(ConsoleOutputModeFlags::DisableNewlineAutoReturn))
    {
        console.WriteLine(L"Disable Newline Auto Return is on.");
    }
    else
    {
        console.WriteLine(L"Disable Newline Auto Return is off.");
        outputMode |= static_cast<unsigned long>(ConsoleOutputModeFlags::DisableNewlineAutoReturn);
        console.WriteLine(L"Turning Disable Newline Auto Return on.");
    }

    if(outputMode & static_cast<unsigned long>(ConsoleOutputModeFlags::LVBGridWorldwide))
    {
        console.WriteLine(L"LVB Grid Worldwide is on.");
        outputMode &= ~(static_cast<unsigned long>(ConsoleOutputModeFlags::LVBGridWorldwide));
        console.WriteLine(L"Turning LVB Grid Worldwide off.");
    }
    else
    {
        console.WriteLine(L"LVB Grid Worldwide is off.");
    }

    console.GetActiveScreenBuffer()->SetMode(outputMode);
}

int main()
{
    std::cout << "Experiments on the Windows Console." << std::endl;

    TermCon terminal;

    CurrentThread::Get().Sleep(5000);
    
    GraphicalConsole screen(120, 80, 7, 9);

    screen.DrawTriangle(2,2, 30, 6, 4,20);
    screen.DrawString(57,48, L"Console Screen", 6);
    screen.DrawStringAlpha(58,49, L"Alpha String", 5);
    screen.DrawLine(57,50,70,50, 9608, 3);
    screen.FillCircle(64, 70, 10, '@', 2);
    screen.Present();
    
    CurrentThread::Get().Sleep(5000);
    return EXIT_SUCCESS;
}

