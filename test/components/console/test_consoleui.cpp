// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "src/configuration/configuration.h"

#include "src/qor/test/test.h"
#include "src/qor/assert/assert.h"
#include "src/qor/injection/typeidentity.h"
#include "src/qor/objectcontext/anyobject.h"
#include "src/framework/thread/currentthread.h"
#include "src/qor/reference/newref.h"
#include "src/components/framework/console/console.h"
#include "src/components/framework/console/consolerenderer.h"
#include "src/components/framework/console/consolecompositor.h"
#include "src/components/framework/console/consolewidget.h"
#include "src/components/framework/console/consoleviewcontroller.h"
#include "src/components/framework/console/consolecolumnwidget.h"
#include "src/components/framework/console/consolemenuwidget.h"

using namespace qor;
using namespace qor::test;
using namespace qor::components;

struct ConsoleUITestSuite {};
bool requiresConsole2 = qor::ImplementsIConsole();

qor_pp_test_suite_case(ConsoleUITestSuite, testCompilesWithqor_consoleuiLinked)
{
    qor_pp_assert_that(true).isTrue();
}

qor_pp_test_suite_case(ConsoleUITestSuite, canGetInstanceOfConsoleForConsoleUI)
{
    auto console = new_ref<Console>();
    qor_pp_assert_that(console.IsNotNull()).isTrue();
}

qor_pp_test_suite_case(ConsoleUITestSuite, checkRendererRenders)
{
    auto renderer = new_ref<ConsoleRenderer>();
    qor_pp_assert_that(renderer.IsNotNull()).isTrue();
    TokenBuffer buffer;
    buffer.SetSize(80,20);
    for(int x = 0; x < 16; ++x)
    {
        for(int y = 0; y < 16; ++y)
        {
            buffer.SetToken(y,x,(16*y)+x);
        }
    }
    TokenBuffer* old = nullptr;
    renderer->SwapTokenBuffer(old, &buffer);
    renderer->Render();
    qor_pp_assert_that(old).isNull();
}

qor_pp_test_suite_case(ConsoleUITestSuite, checkCompositorComposes)
{
    auto view = new_ref<ConsoleView>();
    view->SetWidth(80);
    view->SetHeight(20);
    auto label1 = new_ref<ConsoleLabelWidget>();
    auto label2 = new_ref<ConsoleLabelWidget>();

    label1->SetText("A Label");
    label2->SetText("Another longer label");

    label1->Layout(80,20);
    label2->Layout(80,20);

    label1->Render();
    label2->Render();


    auto compositor = new_ref<ConsoleCompositor>();
    qor_pp_assert_that(compositor.IsNotNull()).isTrue();

    view->DrawWindow(2,2,40,6);
    compositor->Compose({3,3}, label1->GetSize(), label1->View(), view);
    compositor->Compose({3,4}, label2->GetSize(), label2->View(), view);
    view->Render();
}

qor_pp_test_suite_case(ConsoleUITestSuite, checkViewControllerControlsView)
{
    auto viewController = new_ref<ConsoleViewController>();
    qor_pp_assert_that(viewController.IsNotNull()).isTrue();

    auto model = viewController->Model();
    qor_pp_assert_that(model.IsNotNull()).isTrue();

    auto columnWidget = new_ref<ConsoleColumnWidget>();
    columnWidget->AddWidget(new_ref<ConsoleLabelWidget>("Label - 1").template AsRef<ConsoleWidget>());
    columnWidget->AddWidget(new_ref<ConsoleLabelWidget>("Label - 2").template AsRef<ConsoleWidget>());
    columnWidget->AddWidget(new_ref<ConsoleLabelWidget>("Label - 3").template AsRef<ConsoleWidget>());
    columnWidget->AddWidget(new_ref<ConsoleLabelWidget>("Label - 4").template AsRef<ConsoleWidget>());
    model->AddWidget(columnWidget.template AsRef<ConsoleWidget>());

    viewController->Render();
    //viewController->GetUserInput();
}

qor_pp_test_suite_case(ConsoleUITestSuite, checkMenuWidgetMenus)
{
    auto viewController = new_ref<ConsoleViewController>();
    qor_pp_assert_that(viewController.IsNotNull()).isTrue();

    auto model = viewController->Model();
    qor_pp_assert_that(model.IsNotNull()).isTrue();

    auto menuWidget = new_ref<ConsoleMenuWidget>("Main");
    menuWidget->AddMenuItem("Window");
    menuWidget->AddMenuItem("File");
    menuWidget->AddMenuItem("Edit");
    menuWidget->AddMenuItem("Search");
    menuWidget->AddMenuItem("View");
    menuWidget->AddMenuItem("Encoding");
    menuWidget->AddMenuItem("Language");
    menuWidget->AddMenuItem("Settings");
    menuWidget->AddMenuItem("Tools");
    menuWidget->AddMenuItem("Macro");
    menuWidget->AddMenuItem("Run");
    menuWidget->AddMenuItem("Plugins");
    menuWidget->AddMenuItem("Tab");
    menuWidget->AddMenuItem("Help");
    menuWidget->AddMenuItem("Exit");

    model->AddWidget(menuWidget.template AsRef<ConsoleWidget>());
    //do
    {
        viewController->Render();
        viewController->GetUserInput();
    } //while (exitMenuItem->selected == false);
    
}
