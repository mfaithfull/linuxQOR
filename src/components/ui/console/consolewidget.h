// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_CONSOLE_WIDGET
#define QOR_PP_H_COMPONENTS_CONSOLE_WIDGET

//Base class for Console Widgets

#include "consoleview.h"
#include "position.h"
#include "size.h"

namespace qor { namespace components {

    class qor_pp_module_interface(QOR_CONSOLE) ConsoleWidget
    {
    public:

        ConsoleWidget();
        ~ConsoleWidget();

        Position Move(Position newPosition);
        Size Resize(Size newSize);

        virtual Size Layout(int /*width*/, int /*height*/) { return {0,0};}
        virtual Size GetMinimumSize() { return {0,0};}
        virtual Size GetDefaultSize() { return {0,0};}
        virtual Size GetMaximumSize() { return {0,0};}
        virtual void Render();
        ref_of<ConsoleView>::type View();
        Position GetPosition() {return m_position;}
        Size GetSize() {return m_size;}
        virtual bool HandleInput(string_t /*input*/) {return false;}

    protected:

        ref_of<ConsoleView>::type m_view;
        Position m_position;
        struct Size m_size;
    };

    class qor_pp_module_interface(QOR_CONSOLE) ConsoleLabelWidget : public ConsoleWidget
    {
    public:

        ConsoleLabelWidget();
        ConsoleLabelWidget(const string_t& labelText);
        virtual ~ConsoleLabelWidget() = default;
        void SetText(const string_t& newText);
        string_t GetText();
        virtual Size Layout(int width, int height);
        virtual Size GetMinimumSize();
        virtual Size GetDefaultSize();
        virtual Size GetMaximumSize();
        virtual void Render();
        
    private:
        string_t m_labelText;
    };

}}//qor::components

#endif//QOR_PP_H_COMPONENTS_CONSOLE_WIDGET
