// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "datasink.h"

namespace qor{ namespace pipeline {

    DataSink::DataSink() = default;
    DataSink::~DataSink() = default;

    void DataSink::SetCallback( const Delegate<bool(size_t&, size_t)>& callback)
    {
        m_callBack = callback;
    }
    
    void DataSink::SetByteOrder(const arch::Endian byteOrder)
    {
        m_inputByteOrder = byteOrder;
    }

    arch::Endian DataSink::GetByteOrder()
    {
        return m_inputByteOrder;
    }

    bool DataSink::Extract(byte* space, size_t byteCount)
    {        
        size_t totalPumped = 0;
        size_t unitsPumped = 0;
        size_t totalRead = 0;
        do //Pump the pipeline until we get byteCount bytes or we run out
        {
            unitsPumped = 0;
            m_callBack(unitsPumped, byteCount - totalPumped);
            if(unitsPumped > 0)
            {
                totalPumped += unitsPumped;
                ExtractBufferData(byteCount, space, unitsPumped, totalRead);
            }
        }while(unitsPumped > 0 && totalPumped < byteCount);
        return totalPumped == byteCount;
    }

    //push the requested amount of data from the buffer to the sink
    //This is the sink so the implementation reduces to returning
    //the amount of data already in the buffer.
    bool DataSink::Push(size_t& unitsWritten, size_t unitsToWrite)
    {
        if(unitsToWrite > 0)
        {
            Buffer* buffer = GetBuffer();
            if(buffer)
            {
                unitsWritten = std::min(unitsToWrite, buffer->ReadCapacity());
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return true;
        }
    }

    bool DataSink::ExtractBufferData(size_t size, byte* space, size_t byteCount, size_t& totalRead)
    {
        //The buffer contains 'byteCount' out of the 'size' bytes we need from the last pump
        Buffer* buffer = GetBuffer();
        int index = 0;
        byte* data = nullptr;                
        size_t realRead = 0;
        size_t readFromBuffer = 0;
        do
        {
            realRead = byteCount - readFromBuffer;
            data = buffer->ReadRequest(realRead);
            totalRead += realRead;
            readFromBuffer += realRead;
            if(data)
            {
                ExtractBufferValue(size, space, data, totalRead, realRead);
                buffer->ReadAcknowledge(realRead);                    
            }
        }while(data && (readFromBuffer < byteCount));
        //Due to buffer wrapping it may take more than one ReadRequest to get all 'byteCount' bytes from the buffer
        //for a minimal 1 byte buffer it will take byteCount reads
        return readFromBuffer == byteCount ? true : false;
    }

    void DataSink::ExtractBufferValue(size_t size, byte* space, byte* data, size_t totalRead, size_t realRead)
    {
        //size is the size of output space
        //space is output space to write into
        //totalRead is how much of the output space will be written in total when this returns.
        //realRead is how much data there is in data that we haven't extracted yet.
        if(arch::endian != m_inputByteOrder)
        {   
            //Write the bytes from data in reverse to flip endianness (e.g. Big/Network to Little/Host on x86)
            for(int index = 0; index < realRead; ++index)
            {
                space[index + (size-totalRead)] = data[realRead - (index + 1)];            
            }
        }
        else
        {
            //fill the last realRead bytes up to totalRead from data
            memcpy(space + static_cast<int>(totalRead - realRead), data, realRead);            
        }
        //This function always consumes exactly realRead bytes. There is presumed to be enough space
        //or this wouldn't get called.
    }

}}//qor::pipleline