// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_MAINWINDOW
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_MAINWINDOW

#include "window.h"
#include "imainwindowimpl.h"

namespace qor{ namespace ui {
    
    class qor_pp_module_interface(QOR_DESKTOPUI) MainWindow : public Window
    {
    public:

        MainWindow();
        MainWindow(const std::wstring& title);
        MainWindow(ref_of<IMainWindowImpl>::type impl);
        virtual ~MainWindow() = default;

    private:

        ref_of<IMainWindowImpl>::type Impl();
    };

}}//qor::ui

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_MAINWINDOW

