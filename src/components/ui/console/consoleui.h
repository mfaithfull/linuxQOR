// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_CONSOLE_UI
#define QOR_PP_H_COMPONENTS_CONSOLE_UI

#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/reference/newref.h"
#include "console.h"
#include "consoleviewcontroller.h"

namespace qor { namespace components {

    class qor_pp_module_interface(QOR_CONSOLE) ConsoleUI
    {
    public:

        ConsoleUI();
        ~ConsoleUI();

    private:

        ref_of<ConsoleViewController>::type m_controller;
        ref_of<Console>::type m_console;

    };

}}//qor::components

#endif//QOR_PP_H_COMPONENTS_CONSOLE_UI
