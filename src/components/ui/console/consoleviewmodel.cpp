// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "consoleviewmodel.h"

namespace qor { namespace components {

    ConsoleViewModel::ConsoleViewModel() {}
    ConsoleViewModel::~ConsoleViewModel() {}

    void ConsoleViewModel::Layout(unsigned int width, unsigned int height)
    {
        for(auto consoleWidgetModel : m_itemVector)
        {
            consoleWidgetModel->Layout(width, height);
        }
    }

    void ConsoleViewModel::Render()
    {
        for(auto consoleWidgetModel : m_itemVector)
        {
            consoleWidgetModel->Render();
        }
    }

    bool ConsoleViewModel::HandleInput(string_t input)
    {
        bool handled = false;
        auto it = begin();
        while(!handled && it != end())
        {
            handled = (*it)->HandleInput(input);
            it++;
        }
        return handled;
    }

    std::vector<ref_of<ConsoleWidget>::type>::const_iterator ConsoleViewModel::cbegin()
    {
        return m_itemVector.cbegin();
    }
    std::vector<ref_of<ConsoleWidget>::type>::const_iterator ConsoleViewModel::cend()
    {
        return m_itemVector.cend();
    }

    std::vector<ref_of<ConsoleWidget>::type>::iterator ConsoleViewModel::begin()
    {
        return m_itemVector.begin();
    }

    std::vector<ref_of<ConsoleWidget>::type>::iterator ConsoleViewModel::end()
    {
        return m_itemVector.end();
    }

    void ConsoleViewModel::AddWidget(ref_of<ConsoleWidget>::type widget)
    {
        m_itemVector.emplace_back(widget);
    }


}}//qor::components
