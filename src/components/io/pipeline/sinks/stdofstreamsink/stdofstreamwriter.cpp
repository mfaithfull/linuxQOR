// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "stdofstreamwriter.h"
#include "src/framework/io/filesystem/filesystem.h"

namespace qor{ namespace io{ namespace components{ 

    stdofstreamWriter::stdofstreamWriter() : pipeline::Pipeline(), m_Setup(false) { }

    stdofstreamWriter::stdofstreamWriter(const std::string& fileName, size_t bufferByteCapacity) : pipeline::Pipeline(), m_Setup(false)
    {
        auto fileSystem = new_ref<io::FileSystem>();
        fileSystem->Setup();
        io::filesystem::Index file(fileSystem->CurrentPath(), fileName);
        SetFile(file);
        SetBufferCapacity(bufferByteCapacity);
        Setup();
    }

    stdofstreamWriter::stdofstreamWriter(io::filesystem::Index& fileIndex, size_t bufferByteCapacity) : pipeline::Pipeline(), m_Setup(false)
    {
        SetFile(fileIndex);
        SetBufferCapacity(bufferByteCapacity);
        Setup();
    }

    void stdofstreamWriter::SetFile(io::filesystem::Index& fileIndex)
    {
        m_connector.SetFile(fileIndex);
    }

    void stdofstreamWriter::SetBufferCapacity(size_t itemCount)
    {
        m_buffer.SetCapacity(itemCount);
    }

    void stdofstreamWriter::Setup()
    {
        if(m_buffer.Capacity() < 1 )
        {
            m_buffer.SetCapacity(4096);
        }

        if(!m_sink.HasBuffer())
        {
            m_sink.SetBuffer(&m_buffer);
        }

        if(!m_sink.HasSource())
        {
            m_sink.SetSource(&m_source);
        }

        if(!m_sink.HasPlug())
        {
            m_sink.SetPlug(&m_connector);
        }

        if(!m_connector.IsConnected())
        {
            m_connector.Connect();
        }
        m_Setup = true;
    }

    bool stdofstreamWriter::Write(size_t& unitsWritten, size_t unitsToWrite)
    {        
        if(!m_Setup)
        {
            Setup();
        }
        return m_sink.Write(unitsWritten, unitsToWrite);
    }

    bool stdofstreamWriter::Write(const std::string& strData)
    {
        size_t unitsWritten = 0;        
        size_t unitsWrittenAtOnce = 0;

        do
        {
            unitsWrittenAtOnce = 0;
            size_t unitsToWrite = strData.length() - unitsWritten;
            if(unitsToWrite > 0)
            {
                byte* space = m_buffer.WriteRequest(unitsToWrite);
                memcpy( space, strData.c_str() + unitsWritten, unitsToWrite * m_buffer.GetUnitSize());
                m_buffer.WriteAcknowledge(unitsToWrite);
                if(!Write(unitsWrittenAtOnce, unitsToWrite))
                {
                    return false;
                }
                if(unitsWrittenAtOnce == 0)
                {
                    break;
                }
                unitsWritten += unitsWrittenAtOnce;
            }
        }while(unitsWritten < strData.length());
        return true;
    }

}}}//qor::io::components
