// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "window.h"

namespace qor{ namespace ui{

    Window::Window() : Widget()
    {
        m_impl = new_ref<IWindowImpl>();
    }

    Window::Window(ref_of<IWindowImpl>::type impl) : m_impl(impl)
    {
    }
    
}}//qor::ui
