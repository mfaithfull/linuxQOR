// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/essentials/objectcontext/anyobject.h"
#include "src/qor/memory/reference/reference.h"
#include "src/qor/memory/reference/newref.h"
#include "consolewidget.h"

namespace qor { namespace components {

    ConsoleWidget::ConsoleWidget() 
    {
        m_position = {0,0};
        m_size = {0,0};
        m_view = new_ref<ConsoleView>();
    }
    
    ConsoleWidget::~ConsoleWidget() {}

    Position ConsoleWidget::Move(Position newPosition)
    {
        Position result = m_position;
        m_position = newPosition;
        return result;
    }

    Size ConsoleWidget::Resize(Size newSize)
    {
        Size result = m_size;
        m_size = newSize;
        return result;
    }

    void ConsoleWidget::Render() {}

    ref_of<ConsoleView>::type ConsoleWidget::View()
    {
        return m_view;
    }

}}//qor::components
