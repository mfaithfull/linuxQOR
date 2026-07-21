// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/reference/newref.h"

#include "renderer.h"
#include "../terminal.h"

namespace qor { namespace ui { namespace tty {

    Renderer::Renderer(Terminal* display) : m_buffer(nullptr), m_display(display){ }

    Renderer::~Renderer() = default;

    void Renderer::SwapBuffers()
    {
    }

    void Renderer::Present()
    {
    }

    Surface* Renderer::GetSurface()
    {
        return m_currentSurface;
    }

}}}//qor::ui::sercon
