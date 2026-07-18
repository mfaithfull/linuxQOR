// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_WINDOW
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_WINDOW

#include "../widgets/widget.h"
#include "../layout/nodes/vbox.h"

#include "iwindowimpl.h"

namespace qor{ namespace ui {
    
    class qor_pp_module_interface(QOR_DESKTOPUI) Window : public Widget
    {
    public:

        Window();
        Window(ref_of<IWindowImpl>::type impl);
        virtual ~Window() = default;

    protected:

        ref_of<IWindowImpl>::type m_impl;
    };

}}//qor::ui

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_WINDOW

