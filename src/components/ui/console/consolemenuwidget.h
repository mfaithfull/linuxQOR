// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_CONSOLE_MENU_WIDGET
#define QOR_PP_H_COMPONENTS_CONSOLE_MENU_WIDGET

#include <vector>
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/reference/newref.h"
#include "consolewidget.h"
#include "consolecompositor.h"

namespace qor { namespace components {

    struct MenuItem
    {
        MenuItem() : index(-1), selected(false) {}

        string_t itemText;
        int index;
        bool selected;
    };

    class qor_pp_module_interface(QOR_CONSOLE) ConsoleMenuWidget : public ConsoleWidget
    {
    public:

        ConsoleMenuWidget(const string_t& name);
        virtual ~ConsoleMenuWidget() = default;

        virtual Size Layout(int width, int height);
        virtual Size GetMinimumSize();
        virtual Size GetDefaultSize();
        virtual Size GetMaximumSize();
        virtual void Render();
        void AddMenuItem(ref_of<MenuItem>::type);
        void AddMenuItem(const string_t& itemText);
        virtual bool HandleInput(string_t input);

    private:

        Size GetSize();

        int m_selected;
        string_t m_name;
        std::vector<ref_of<MenuItem>::type> m_contents;
        ref_of<ConsoleCompositor>::type m_compositor;
    };

}}//qor::components

#endif//QOR_PP_H_COMPONENTS_CONSOLE_MENU_WIDGET
