// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/reference/newref.h"

#include "consoleui.h"

namespace qor { namespace components {

    ConsoleUI::ConsoleUI()
    {
        m_console = new_ref<Console>();
        m_controller = new_ref<ConsoleViewController>();
    }

    ConsoleUI::~ConsoleUI() {}

}}//qor::components
