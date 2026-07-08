// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "buffer.h"
#include "src/qor/flyers/error/error.h"

namespace qor{ namespace pipeline{

    Buffer::Buffer() : m_unitSize(1)
    {
        SetCapacity(0);
    }

    Buffer::Buffer(size_t unitSize, size_t itemCount) : m_unitSize(unitSize)
    {
        SetCapacity(itemCount);
    }

    void Buffer::SetUnitSize(size_t unitSize)
    {
        if(m_allocationCount > 0 )
        {
            SetCapacity((size_t)0);
        }
        m_unitSize = unitSize;
    }

    Buffer::Buffer(const Buffer& src)
    {
        *this = src;
    }

    Buffer::~Buffer()
    {
        SetCapacity((size_t)0);
    }

    Buffer& Buffer::operator = (const Buffer& src)
    {
        if(&src != this)
        {
            SetUnitSize(src.GetUnitSize());
            SetCapacity(src.Capacity());
            m_readBegin = src.m_readBegin;
            m_readEnd = src.m_readEnd;
            m_writeBegin = src.m_writeBegin;
            m_writeEnd = src.m_writeEnd;
        }
        return *this;
    }

    void Buffer::SetCapacity(size_t itemCount)
    {
        m_allocationCount = itemCount;
        m_readBegin = 0;
        m_readEnd = 0;
        m_writeBegin = 0;
        m_writeEnd = 0;
    }

    void Buffer::Reset(size_t itemCount)
    {
        m_readBegin = 0;
        m_readEnd = 0;
        m_writeBegin = 0;
        m_writeEnd = 0;
        if(itemCount != 0 && itemCount != m_allocationCount)
        {   //Only reallocate if the size actually changes
            SetCapacity(itemCount);
        }
    }

    size_t Buffer::Capacity(void) const
    {
        return m_allocationCount;
    }

    size_t Buffer::ReadAcknowledge(size_t& itemCount)
    {
        if(itemCount > (m_readEnd - m_readBegin))
        {
            itemCount = static_cast<size_t>(m_readEnd - m_readBegin);
        }

        m_readBegin += itemCount;
        m_readEnd = m_readBegin;
        return ReadCapacity();
    }

    size_t Buffer::ReadReject(size_t& itemCount)
    {
        if(itemCount > (m_readEnd - m_readBegin))
        {
            itemCount = static_cast<size_t>(m_readEnd - m_readBegin);
        }

        m_readEnd -= itemCount;
        return ReadCapacity();
    }

    size_t Buffer::WriteAcknowledge(size_t& itemCount)
    {
        if(itemCount > (m_writeEnd - m_writeBegin))
        {
            itemCount = static_cast<size_t>(m_writeEnd - m_writeBegin);
        }

        m_writeBegin += itemCount;
        m_writeEnd = m_writeBegin;
        return WriteCapacity();
    }

    size_t Buffer::GetUnitSize() const
    {
        return m_unitSize;
    }

    size_t Buffer::WriteCapacity() const
    {
        size_t result = m_allocationCount - static_cast<size_t>(m_writeEnd - m_readBegin);
        return result;
    }

    size_t Buffer::ReadCapacity() const
    {
        size_t result = static_cast<size_t>(m_writeBegin - m_readEnd);
        return result;
    }

    byte* Buffer::WriteRequest(size_t& /*itemCount*/)
    {
        fatal("Empty base called. Please override byte* WriteRequest(size_t&); in your pipeline::Buffer derived class.");
        return nullptr;
    }

    byte* Buffer::ReadRequest(size_t& /*itemCount*/)
    {
        fatal("Empty base called. Please override byte* ReadRequest(size_t&); in your pipeline::Buffer derived class.");
        return nullptr;
    }

}}//qor::pipeline