// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_CONSOLE_VIEW_CONTROLLER
#define QOR_PP_H_COMPONENTS_CONSOLE_VIEW_CONTROLLER

#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/reference/newref.h"
#include "console.h"
#include "consoleviewmodel.h"
#include "consoleview.h"
#include "consolecompositor.h"

/*ConsoleViewController get input from the Console and splits in between purely View related input which is handled here by modifying the ViewModel and
Application related input which is passed on to the Application controller*/
namespace qor { namespace components {

    class qor_pp_module_interface(QOR_CONSOLE) ConsoleViewController
    {
    public:

        ConsoleViewController();
        ~ConsoleViewController();

        ref_of<ConsoleViewModel>::type Model();

        void Render();
        void GetUserInput();

    private:

        ref_of<ConsoleViewModel>::type m_model;
        ref_of<Console>::type m_console;
        ref_of<ConsoleView>::type m_view;
        ref_of<ConsoleCompositor>::type m_compositor;
    };

}}//qor::components

#endif//QOR_PP_H_COMPONENTS_CONSOLE_VIEW_CONTROLLER
