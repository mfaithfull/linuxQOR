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
#include "serialconsole.h"

namespace qor { namespace ui {

    SerialConsole::SerialConsole()
    {
        m_renderer = new_ref<sercon::Renderer>(this);
    }

    SerialConsole::~SerialConsole() noexcept = default;

    void SerialConsole::Setup() { }
            
    void SerialConsole::Shutdown() { }

    string_t SerialConsole::ReadLine()
    {
        return string_t();
    }

    void SerialConsole::WriteLine(const string_t& line){ }
    
    char_t SerialConsole::ReadChar()
    {
        return 0;
    }
    
    void SerialConsole::WriteChar(char_t c){ }

    ref_of<sercon::Renderer>::type SerialConsole::GetRenderer()
    {
        return m_renderer;
    }

    ref_of<sercon::TokenBuffer>::type SerialConsole::CreateSurface()
    {
        return new_ref<sercon::TokenBuffer>();
    }

}}//qor::ui

