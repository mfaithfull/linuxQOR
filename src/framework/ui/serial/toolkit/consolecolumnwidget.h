// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_CONSOLE_COLUMN_WIDGET
#define QOR_PP_H_COMPONENTS_CONSOLE_COLUMN_WIDGET

#include <vector>
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/reference/newref.h"
#include "consolewidget.h"
#include "consolecompositor.h"

namespace qor { namespace components {

    class qor_pp_module_interface(QOR_CONSOLE) ConsoleColumnWidget : public ConsoleWidget
    {
    public:

        enum VJustify
        {
            VJTop,
            VJFull,
            VJCenter,
            VJBottom,
        };

        enum HAlign
        {
            HALeft,
            HACenter,
            HARight,
        };

        ConsoleColumnWidget();
        virtual ~ConsoleColumnWidget() = default;

        virtual Size Layout(int width, int height);
        virtual Size GetMinimumSize();
        virtual Size GetDefaultSize();
        virtual Size GetMaximumSize();
        virtual void Render();
        void AddWidget(ref_of<ConsoleWidget>::type);

    private:

        std::vector<ref_of<ConsoleWidget>::type> m_contents;
        ref_of<ConsoleCompositor>::type m_compositor;
    };

}}//qor::components

#endif//QOR_PP_H_COMPONENTS_CONSOLE_COLUMN_WIDGET
