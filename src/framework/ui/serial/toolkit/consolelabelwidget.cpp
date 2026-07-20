// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "consolewidget.h"

namespace qor { namespace components {

    ConsoleLabelWidget::ConsoleLabelWidget() : ConsoleWidget() {}
    
    ConsoleLabelWidget::ConsoleLabelWidget(const string_t& labelText) : ConsoleWidget(), m_labelText(labelText){ }

    void ConsoleLabelWidget::SetText(const string_t& newText)
    {
        m_labelText = newText;
    }

    string_t ConsoleLabelWidget::GetText()
    {
        return m_labelText;
    }

    Size ConsoleLabelWidget::Layout(int width, int height)
    {
        m_position.m_column = 0;
        m_position.m_row = 0;
        m_size.m_height = 1;
        int length = static_cast<int>(m_labelText.size()+1);
        m_size.m_width = length;
        while(length > width && m_size.m_height < height)
        {
            m_size.m_width = width;
            length -= width;
            m_size.m_height++;
        }
        return m_size;
    }

    Size ConsoleLabelWidget::GetMinimumSize()
    {
        Size minsize = {static_cast<int>(std::min(static_cast<size_t>(6),m_labelText.size()+1)), 1};
        return minsize;
    }

    Size ConsoleLabelWidget::GetDefaultSize()
    {
        Size defaultSize = {static_cast<int>(m_labelText.size()+1), 1};
        return defaultSize;
    }

    Size ConsoleLabelWidget::GetMaximumSize()
    {
        Size maxsize = {static_cast<int>(m_labelText.size()+1), static_cast<int>(m_labelText.size()+1)};
        return maxsize;
    }

    void ConsoleLabelWidget::Render()
    {
        m_view->SetWidth(m_size.m_width);
        m_view->SetHeight(m_size.m_height);
        m_view->DrawLabel(m_position.m_column, m_position.m_row, m_labelText);
    }

}}//qor::components