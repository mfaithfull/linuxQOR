// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "mainwindow.h"

namespace qor{ namespace ui{

    MainWindow::MainWindow() : Window (new_ref<IMainWindowImpl>(L"Unnamed"))
    {

    }
    
    MainWindow::MainWindow(const std::wstring& title) : Window(new_ref<IMainWindowImpl>(title))
    {        
    }

    MainWindow::MainWindow(ref_of<IMainWindowImpl>::type impl) : Window(impl)
    {
    }

    ref_of<IMainWindowImpl>::type MainWindow::Impl()
    {
        return m_impl;
    }
    
}}//qor::ui
