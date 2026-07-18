// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/essentials/objectcontext/anyobject.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/reference/newref.h"
#include "consolecolumnwidget.h"

namespace qor { namespace components {

    ConsoleColumnWidget::ConsoleColumnWidget() : ConsoleWidget() {}

    void ConsoleColumnWidget::AddWidget(ref_of<ConsoleWidget>::type widget)
    {
        m_contents.emplace_back(widget);
    }

    Size ConsoleColumnWidget::Layout(int width, int height)
    {
        Size totalSize = {0,0};
        for( ref_of<ConsoleWidget>::type widget : m_contents)
        {
            Size initialSize = widget->Layout(width, height / static_cast<int>(m_contents.size()));
            totalSize.m_height += initialSize.m_height;
            if(initialSize.m_width > totalSize.m_width)
            {
                totalSize.m_width = initialSize.m_width;
            }
        }
        m_size = totalSize;
        return totalSize;
    }

    Size ConsoleColumnWidget::GetMinimumSize()
    {
        int minWidth = 0;
        int minHeight = 0;
        for( ref_of<ConsoleWidget>::type widget : m_contents)
        {
            Size contentMinSize = widget->GetMinimumSize();
            if(contentMinSize.m_width > minWidth)       //minimum width is the maximum of the minum widths of the contents
            {
                minWidth = contentMinSize.m_width;
            }
            minHeight += contentMinSize.m_height;        //minimum height is the sum of the minimum heights of the contents
        }
        Size minsize = {minWidth, minHeight};
        return minsize;
    }

    Size ConsoleColumnWidget::GetDefaultSize()
    {
        int defaultWidth = 0;
        int defaultHeight = 0;
        for( ref_of<ConsoleWidget>::type widget : m_contents)
        {
            Size contentDefaultSize = widget->GetDefaultSize();
            if(contentDefaultSize.m_width > defaultWidth)       //minimum width is the maximum of the minum widths of the contents
            {
                defaultWidth = contentDefaultSize.m_width;
            }
            defaultHeight += contentDefaultSize.m_height;        //minimum height is the sum of the minimum heights of the contents
        }
        Size defaultSize = {defaultWidth, defaultHeight};
        return defaultSize;
    }

    Size ConsoleColumnWidget::GetMaximumSize()
    {
        int maxWidth = 0;
        int maxHeight = 0;
        for( ref_of<ConsoleWidget>::type widget : m_contents)
        {
            Size contentMaxSize = widget->GetMaximumSize();
            if(contentMaxSize.m_width > maxWidth)
            {
                maxWidth = contentMaxSize.m_width;
            }
            maxHeight += contentMaxSize.m_height;
        }
        Size maxsize = {maxWidth, maxHeight};
        return maxsize;
    }

    void ConsoleColumnWidget::Render()
    {
        if(m_compositor.IsNull())
        {
            m_compositor = new_ref<ConsoleCompositor>();
        }
        int row = 0;
        int column = 0;//TODO: vary the column according to alignment
        m_view->SetWidth(m_size.m_width);
        m_view->SetHeight(m_size.m_height);
        for( ref_of<ConsoleWidget>::type widget : m_contents)
        {
            widget->Render();
            m_compositor->Compose({column,row}, widget->GetSize(), widget->View(), m_view);
            row += widget->GetSize().m_height;
        }
    }

}}//qor::components

