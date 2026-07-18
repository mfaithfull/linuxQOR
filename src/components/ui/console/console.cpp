// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/essentials/objectcontext/anyobject.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/factory/factory.h"
#include "src/qor/memory/instance/instance.h"
#include "src/qor/memory/reference/ref.h"
#include "src/qor/memory/reference/newref.h"
#include "iconsole.h"
#include "console.h"
#include <iostream>

namespace qor { namespace components {

    Console::Console()
    {
        m_pimpl = new_ref<IConsole>();
    }

    string_t Console::ReadLine()
    {
        return m_pimpl->ReadLine();
    }

    void Console::WriteLine(const string_t& line)
    {
        m_pimpl->WriteLine(line);
    }
    
    char_t Console::ReadChar()
    {
        return m_pimpl->ReadChar();
    }
    
    void Console::WriteChar(char_t c)
    {
        m_pimpl->WriteChar(c);        
    }

}}//qor::components

