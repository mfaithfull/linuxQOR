// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "src/configuration/configuration.h"

#include "src/framework/thread/currentthread.h"
#include "src/qor/injection/typeidentity.h"
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
