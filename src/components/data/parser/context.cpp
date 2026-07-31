
// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "context.h"
#include "src/qor/flyers/log/debug.h"

namespace qor { namespace data { namespace parser {

    Context::Context() = default;

    Context::Context(Context&& src)
    {
        m_octetStream = src.m_octetStream;
        m_position = src.m_position;
        m_size = src.m_size;
        src.m_octetStream = nullptr;
        src.m_position = 0;
        src.m_size = 0;
    }

    Context::Context(byte* data, size_t itemCount) : m_octetStream(data), m_position(0), m_size(itemCount){ }

    size_t Context::GetPosition()
    {
        return m_position;
    }

    bool Context::ConsumeOctet()
    {
        ++m_position;
        //log::debug("Context: ConsumeOctet: position: {0}, size: {1}", m_position, m_size);
        return m_position < m_size;
    }

    bool Context::GetOctet(byte*& data)
    {
        if(m_position < m_size)
        {
            data = m_octetStream + m_position;
            //log::debug("Context: GetOctet: position: {0}, size: {1}, octet: {2:02x}", m_position, m_size, (unsigned char)(*data));
            return true;
        }
        return false;
    }

    bool Context::PutOctet(byte data)
    {
        if(m_position < m_size)
        {
            m_octetStream[m_position++] = data;
            return true;
        }
        return false;
    }

    void Context::Diagnostic()
    {
        std::string sample((char*)(m_octetStream + m_position - 1), std::min(m_size - m_position, (size_t)10));
        log::debug("Context diagnostics:\nstream size: {0}, position: {1}\ndata sample: {2}", m_size, m_position, sample);
    }

    void Context::SetData(byte* data, size_t itemCount)
    {
        m_octetStream = data;
        m_position = 0;
        m_size = itemCount;
    }

    bool Context::HasUnparsedData()
    {
        return m_size - m_position > 0;
    }

    bool Context::HasSpace()
    {
        return m_size - m_position > 0;
    }

    void Context::Reset()
    {
        m_octetStream = nullptr;
        m_position = 0;
        m_size = 0;
    }

}}}//qor::data::parser
