// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "stringsink.h"
#include "src/framework/data/pipeline/source.h"

namespace qor{ namespace pipeline{ namespace components{ 

    StringSink::StringSink() = default;
    
    StringSink::~StringSink() = default;

    std::string StringSink::GetData()
    {
        return m_data;
    }
    
    /*
    bool StringSink::Write(size_t& unitsWritten, size_t unitsToWrite)
    {
        return (unitsToWrite == 0 || Pull(unitsWritten, unitsToWrite)) ? Push(unitsWritten, unitsWritten) : false;
    }

    bool StringSink::Pull(size_t& unitsWritten, size_t unitsToWrite)
    {
        return GetFlowMode() == FlowMode::Pull ? 
        (ActualSource()->Read(unitsWritten, unitsToWrite) && (unitsWritten > 0 || unitsToWrite == 0) ? true : false) : true;        
    }*/
/*
    bool StringSink::Push(size_t& unitsWritten, size_t unitsToWrite)
    {
        if(unitsToWrite > 0)
        {
            pipeline::Buffer* buffer = GetBuffer();
            if(buffer)
            {
                byte* data = buffer->ReadRequest(unitsToWrite);
                size_t bytesWritten = Write(data, buffer->GetUnitSize() * unitsToWrite);
                if(bytesWritten > 0)
                {
                    unitsWritten = bytesWritten / buffer->GetUnitSize();
                    buffer->ReadAcknowledge(unitsWritten);
                    OnWriteSuccess(unitsWritten);
                }
                else //EOF?
                {
                    return false;
                }
                return true;
            }
            return false;
        }
        return true;
    }
*/
    size_t StringSink::WriteBytes(byte* data, size_t bytesToWrite)
    {
        m_data.append(std::string((const char*)(data), bytesToWrite));
        return bytesToWrite;
    }

}}}//qor::pipeline::components
