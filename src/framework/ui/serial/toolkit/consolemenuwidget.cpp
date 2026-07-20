// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <format>

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/essentials/objectcontext/anyobject.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/reference/newref.h"
#include "consolemenuwidget.h"


namespace qor { namespace components {

    ConsoleMenuWidget::ConsoleMenuWidget(const string_t& name) : m_selected(0), m_name(name), ConsoleWidget() {}

    void ConsoleMenuWidget::AddMenuItem(ref_of<MenuItem>::type item)
    {
        m_contents.emplace_back(item);
    }

    void ConsoleMenuWidget::AddMenuItem(const string_t& itemText)
    {
        int itemIndex = 0;
        for( ref_of<MenuItem>::type item : m_contents)
        {
            if(item->index > itemIndex)
            {
                itemIndex = item->index;
            }
        }
        itemIndex++;
        auto menuItem = new_ref<MenuItem>();
        menuItem->itemText = itemText;
        menuItem->index = itemIndex;
        AddMenuItem(menuItem);
    }

    Size ConsoleMenuWidget::Layout(int /*width*/, int /*height*/)
    {
        m_size = GetSize();
        return m_size;
    }

    Size ConsoleMenuWidget::GetSize()
    {
        size_t maxWidth = 0;
        size_t maxHeight = (m_contents.size() + 4);//Add 2 for box, 1 for menu title, one for title separator
        for( ref_of<MenuItem>::type item : m_contents)
        {
            if(item->itemText.size() > maxWidth)
            {
                maxWidth = item->itemText.size();
            }
        }
        Size maxsize = {static_cast<long>(maxWidth + 4), static_cast<long>(maxHeight)};
        return maxsize;
    }

    Size ConsoleMenuWidget::GetMinimumSize()
    {
        return GetSize();
    }

    Size ConsoleMenuWidget::GetDefaultSize()
    {
        return GetSize();
    }

    Size ConsoleMenuWidget::GetMaximumSize()
    {
        return GetSize();
    }

    void ConsoleMenuWidget::Render()
    {
        int row = 3;
        int column = 1;
        m_view->SetWidth(m_size.m_width);
        m_view->SetHeight(m_size.m_height);
        m_view->DrawWindow(0,0,m_size.m_width-1,m_size.m_height-1);
        for( ref_of<MenuItem>::type item : m_contents)
        {
            if(item->selected)
            {
                m_view->DrawLabel(column, row, std::format("{:x}>", item->index));
            }
            else
            {
                m_view->DrawLabel(column, row, std::format("{:x}|", item->index));
            }
            m_view->DrawString(column + 2, row, item->itemText);
            row += 1;
        }
        m_view->DrawString(1 + (((m_size.m_width - 2) - (int)m_name.size()) / 2),1, m_name);
        m_view->DrawInternalHLine(0,2,m_size.m_width);
    }

    bool ConsoleMenuWidget::HandleInput(string_t input)
    {
        bool bHandled = false;
        if(input.size() >= 1 )
        {
            char_t c = input[0];
            if(c > '0' && c <= '0' + m_contents.size())
            {
                int selectedIndex = c - '0';
                for( ref_of<MenuItem>::type item : m_contents)
                {
                    if(item->index == m_selected)
                    {
                        item->selected = false;
                    }
                    if(item->index == selectedIndex)
                    {
                        bHandled = true;
                        item->selected = true;
                    }
                }
                m_selected = selectedIndex;
            }
        }
        return bHandled;
    }


}}//qor::components

