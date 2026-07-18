// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/reference/newref.h"

#include "consoleviewcontroller.h"

namespace qor { namespace components {

    ConsoleViewController::ConsoleViewController()
    {
        m_console = new_ref<Console>();
        m_view = new_ref<ConsoleView>();
        m_model = new_ref<ConsoleViewModel>();
        m_compositor = new_ref<ConsoleCompositor>();
    }

    ConsoleViewController::~ConsoleViewController() {}

    ref_of<ConsoleViewModel>::type ConsoleViewController::Model()
    {
        return m_model;
    }

    void ConsoleViewController::Render()
    {
        
        unsigned int width = m_view->GetWidth();        //Get the constraints of the view
        unsigned int height = m_view->GetHeight();
        m_model->Layout(width, height);                 //Layout the view model
        m_model->Render();                              //Have the View model widgets render themselves to their own views
        m_compositor->Compose(m_model,m_view);          //Composite them into the main view
        m_view->Render();                               //Render the view
    }

    void ConsoleViewController::GetUserInput()
    {
        string_t input = m_console->ReadLine();         //Collect input from the user
        bool handled = false;   
        //Process the input
        //Check for Overall view level intercepts for modifying the view itself
        if( input == "\\")
        {
            /*push a layer onto the basic layers widget that contains a menu widget for modifying the view parameters*/
            handled = true;
        }

        //If the input is unhandled pass it to each widget's controller until it's handled
        if(!handled)
        {
            handled = m_model->HandleInput(input);
        }

        //If the input is unhandled raise an event to the application to handle it.
        if(!handled)
        {

        }
    }

    /*Operations that modify the view
    \ to bring up View menu
    1. Width
    2. Height
    3. Style
    
    Width and heigh just take an input of a number
    
    Style will require a list of available style which will swap out the TOKEN->char_t mapping table*/


}}//qor::components
