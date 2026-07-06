// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "socketreader.h"

namespace qor{ namespace io{ namespace network{ namespace components{ 

    SocketReader::SocketReader() : pipeline::Pipeline()
    {        
        m_source.SetParent(this);
        m_source.SetSink(this);        
        this->SetSink(&m_sink);
        this->SetSource(&m_source);
    }

    SocketReader::SocketReader(pipeline::Buffer* buffer) : SocketReader()
    {
        SetBuffer(buffer);
    }

    SocketReader::~SocketReader() = default;

    void SocketReader::SetPlug(ref_of<SocketConnector>::type plug)
    {
        m_connector = plug;
        m_source.SetPlug(plug);
    }

    void SocketReader::SetBuffer(pipeline::Buffer* buffer)
    {
        Pipeline::SetBuffer(buffer);        
        m_source.SetBuffer(buffer);
        m_sink.SetBuffer(buffer);
    }

    void SocketReader::SetBufferCapacity(size_t itemCount)
    {
        auto buffer = GetBuffer();
        if(buffer)
        {
            buffer->SetCapacity(itemCount);
        }
    }

    bool SocketReader::Read(size_t& unitsRead, size_t unitsToRead)
    {        
        return ActualSource()->Read(unitsRead, unitsToRead);
    }

    bool SocketReader::Write(size_t& /*unitsWritten*/, size_t /*unitstoWrite*/){ return false; }

}}}}//qor::io::network::components
