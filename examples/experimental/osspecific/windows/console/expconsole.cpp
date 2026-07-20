
#include "src/configuration/configuration.h"

#include <iostream>

#include "src/platform/os/windows/app/exebootstrap/winqorexeboot.h"
#include "src/platform/os/windows/ui/wincon/console.h"
#include "src/platform/os/windows/ui/terminal/termcon.h"
#include "src/platform/os/windows/ui/wincon/graphcon.h"
#include "src/platform/os/windows/ui/wincon/graphconrenderer.h"
#include "src/framework/ui/uiitem.h"
#include "src/framework/ui/visible.h"
#include "src/framework/ui/compound.h"
#include "src/framework/ui/contained.h"
#include "src/framework/ui/layout/arrangeable.h"
#include "src/framework/ui/layout/sizeconstrainer.h"
#include "src/framework/ui/layout/vbox.h"
#include "src/framework/ui/layout/alignright.h"

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


class TestArticle : public qor::ui::Compound, public virtual qor::ui::layout::IArrangeable
{
public:
};

class TestMiddleArticle : public qor::ui::Contained, public virtual qor::ui::Compound, public virtual qor::ui::layout::IArrangeable, public virtual qor::ui::IVisible
{
public:

    virtual ~TestMiddleArticle(){}
};

class TestChildArticle : public qor::ui::Contained, public virtual qor::ui::layout::IArrangeable, public virtual qor::ui::IVisible
{
public:

    virtual ~TestChildArticle(){}
};

int main()
{
    std::cout << "Experiments on the Windows Console." << std::endl;

    auto testarticle1 = new_ref<TestArticle>();

    auto middle = new_ref<qor::ui::layout::SizeConstrainer<TestMiddleArticle>>(qor::ui::layout::HEIGHT, qor::ui::layout::GREATER_THAN, 10);

    testarticle1->Add(middle);

    middle->Add(new_ref<qor::ui::layout::AlignRight>(new_ref<TestChildArticle>()));

    qor::ui::layout::VBox vbox;
    vbox.Add(testarticle1);
    vbox.ComputeRequirement();

    TermCon terminal;

    GraphicalConsole screen(120, 80, 7, 9);

    auto surface = screen.CreateSurface();
    GraphConRenderer renderer(&screen, surface);

    renderer.DrawTriangle(2,2, 30, 6, 4,20);
    renderer.DrawString(57,48, L"Console Screen", 6);
    renderer.DrawStringAlpha(58,49, L"Alpha String", 5);
    renderer.DrawLine(57,50,70,50, 9608, 3);
    renderer.FillCircle(64, 70, 10, '@', 2);
    renderer.Present();
    
    CurrentThread::Get().Sleep(5000);
    return EXIT_SUCCESS;
}

