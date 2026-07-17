// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/qor/flyers/interception/functioncontext.h"
#include "src/qor/flyers/error/error.h"
#include "pseudoconsole.h"
#include "consolehelper.h"

//Windows specific headers must be last to prevent contaminating generic headers with Windows specific types and definitions
#include "src/platform/os/windows/api_layer/kernel/kernel32.h" //kernel32.h must be the first windows header as it's the primary inclusion point for windows.h
#include "src/platform/os/windows/common/stringconv.h"
#include "src/platform/os/windows/common/constants.h"
#include "src/platform/os/windows/common/structures.h"

using namespace qor::win::api;
using namespace qor::platform::win;

namespace qor { namespace ui { namespace win {

    PseudoConsole::PseudoConsole(void* handle) : m_handle(handle)
    { }

    PseudoConsole::PseudoConsole(qor::platform::win::Coord size, void* hInput, void* hOutput, unsigned long flags)
    {
        m_handle = ConsoleHelper().CreatePseudo(size, hInput, hOutput, flags);
    }

    PseudoConsole::~PseudoConsole()
    {
        ConsoleHelper().ClosePseudo(m_handle);
    }
    
    void* PseudoConsole::Handle() const 
    { 
        return m_handle; 
    }

    long PseudoConsole::Resize(platform::win::Coord size)
    {
        return ConsoleHelper().ResizePseudo(m_handle, size);
    }
    
}}}//qor::ui::win