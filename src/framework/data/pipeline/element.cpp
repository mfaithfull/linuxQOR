// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "element.h"

namespace qor{ namespace pipeline{

    Element::Element() : m_parent(nullptr), m_buffer(nullptr) { }

    Element::Element( const Element& src )
    {
        *this = src;
    }

    Element::~Element() noexcept (false) { }

    Element& Element::operator = (const Element& src)
    {
        if(&src != this)
        {
            //NOTE: It's far from obvious that this behaviour will be correct in all cases
            m_parent = src.m_parent;
            if(m_buffer)
            {
                *m_buffer = *src.m_buffer;
            }            
        }
        return *this;
    }

    Element* Element::GetParent(void)
    {
        return m_parent;
    }

    void Element::SetParent(Element* parent)
    {
        m_parent = parent;
    }

    bool Element::IsBuffered()
    {
        return m_buffer != nullptr;
    }

    bool Element::IsSource()
    {
        return false;
    }

    bool Element::IsSink()
    {
        return false;
    }

    bool Element::IsPlug()
    {
        return false;
    }

    void Element::SetBuffer(Buffer* buffer)
    {
        m_buffer = buffer;
    }

    void Element::SetBuffer(const Buffer& buffer)
    {
        m_buffer = &(const_cast<Buffer&>(buffer));
    }

    Buffer* Element::GetBuffer()
    {
        return m_buffer;
    }

    bool Element::HasBuffer()
    {
        return m_buffer != nullptr;
    }

    Element* Element::GetSource()
    {
        return nullptr;
    }

    bool Element::HasSource()
    {
        return false;
    }

    Element* Element::GetSink() const
    {
        return nullptr;
    }

    bool Element::HasSink()
    {
        return false;
    }

    Element::FlowMode Element::GetFlowMode()
    {
        if(m_parent)
        {
            return m_parent->GetFlowMode();
        }
        else
        {
            return Pull;
        }
    }

    void Element::SetFlowMode(Element::FlowMode flowmode)
    {
        if(m_parent)
        {
            m_parent->SetFlowMode(flowmode);
        }
    }

    const char* Element::Name() const
    {
        return "Element";
    }

}}//qor::pipeline