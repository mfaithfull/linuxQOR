// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_CONSOLE_VIEW_MODEL
#define QOR_PP_H_COMPONENTS_CONSOLE_VIEW_MODEL

#include <vector>
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/reference/newref.h"
#include "consolewidget.h"

/*ConsoleViewModel collects ConsoleWidgetModels built by observation of domain models and injection of Style and Console UI specific data*/
namespace qor { namespace components {

    class qor_pp_module_interface(QOR_CONSOLE) ConsoleViewModel
    {
    public:

        ConsoleViewModel();
        ~ConsoleViewModel();

        //RemoveWidget
        void Layout(unsigned int width, unsigned int height);// - position and size everything within the contraints of width and height
        //Each widget then renders to it's own view. The views are composited to the main view
        void Render();
        bool HandleInput(string_t input);
        //Add Layer Widgets, Row Widgets, Column Widgets, Grid Widgets, Flow Widgets which exist just as containers to layout their contents
        std::vector<ref_of<ConsoleWidget>::type>::const_iterator cbegin();
        std::vector<ref_of<ConsoleWidget>::type>::const_iterator cend();
        std::vector<ref_of<ConsoleWidget>::type>::iterator begin();
        std::vector<ref_of<ConsoleWidget>::type>::iterator end();

        void AddWidget(ref_of<ConsoleWidget>::type);
        
    private:

        std::vector<ref_of<ConsoleWidget>::type> m_itemVector;
    };

}}//qor::components

#endif//QOR_PP_H_COMPONENTS_CONSOLE_VIEW_MODEL
