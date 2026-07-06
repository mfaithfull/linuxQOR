// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "stdifstreamreader.h"
#include "src/framework/io/filesystem/filesystem.h"

namespace qor{ namespace io{ namespace components{ 

    stdifstreamReader::stdifstreamReader() : pipeline::Pipeline(), m_Setup(false){ }

    stdifstreamReader::stdifstreamReader(const std::string& fileName, size_t bufferByteCapacity)
    {
        auto fileSystem = new_ref<io::FileSystem>();
        fileSystem->Setup();
        io::filesystem::Index file(fileSystem->CurrentPath(), fileName);
        SetFile(file);
        SetBufferCapacity(bufferByteCapacity);
        Setup();
    }

    stdifstreamReader::stdifstreamReader(io::filesystem::Index& fileIndex, size_t bufferByteCapacity) : pipeline::Pipeline(), m_Setup(false)
    {
        SetFile(fileIndex);
        SetBufferCapacity(bufferByteCapacity);
        Setup();
    }

    void stdifstreamReader::SetFile(io::filesystem::Index& fileIndex)
    {
        m_connector.SetFile(fileIndex);
    }

    void stdifstreamReader::SetBufferCapacity(size_t itemCount)
    {
        m_buffer.SetCapacity(itemCount);
    }

    void stdifstreamReader::Setup()
    {
        if(m_buffer.Capacity() < 1 )
        {
            m_buffer.SetCapacity(4096);
        }

        if(!source.HasBuffer())
        {
            source.SetBuffer(&m_buffer);
        }

        if(!source.HasSink())
        {
            source.SetSink(&m_sink);
        }

        if(!source.HasPlug())
        {
            source.SetPlug(&m_connector);
        }

        if(!m_connector.IsConnected())
        {
            m_connector.Connect();
            if(!m_connector.IsConnected())
            {
                continuable("Failed to open {0}.", this->m_connector.GetFileName());
            }
        }
        m_Setup = true;
    }

    bool stdifstreamReader::Read(size_t& unitsRead, size_t unitsToRead)
    {        
        if(!m_Setup)
        {
            Setup();
        }
        return source.Read(unitsRead, unitsToRead);
    }

    void stdifstreamReader::ReadAll(size_t& unitsRead, stdifstreamReader::ProcessDataFn&& process)
    {
        size_t unitsReadAtOnce = 0;
        do
        {            
            unitsReadAtOnce = 0;
            size_t unitsToRead = m_buffer.WriteCapacity();
            Read(unitsReadAtOnce, unitsToRead);
            unitsRead += unitsReadAtOnce;
            size_t unitsToProcess = unitsReadAtOnce;            
            byte* data = m_buffer.ReadRequest(unitsToProcess);
            size_t bytesToProcess = unitsToProcess * m_buffer.GetUnitSize();
            size_t bytesProcessed = process(data, bytesToProcess);
            if(bytesProcessed > 0 )
            {
                size_t unitsProcessed = bytesProcessed / m_buffer.GetUnitSize();
                m_buffer.ReadAcknowledge(unitsProcessed);
            }
        } while (unitsReadAtOnce > 0);
    }

    std::string stdifstreamReader::Read()
    {
        size_t unitsRead = 0;
        std::string strText;
        ReadAll(unitsRead, [&strText](byte* data, size_t byteCount) -> size_t
        {
            std::string temp((const char*)data,byteCount);
            strText = strText + temp;
            return byteCount;
        });
        return strText;
    }

}}}//qor::io::components
