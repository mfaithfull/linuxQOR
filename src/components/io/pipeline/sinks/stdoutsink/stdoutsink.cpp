// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "stdoutsink.h"
#include "src/framework/data/pipeline/source.h"

namespace qor{ namespace io{ namespace components{ 

    StdOutSink::StdOutSink(){ }

    StdOutSink::~StdOutSink()
    {
        fflush(stdout);
    }

    bool StdOutSink::Write(size_t& unitsWritten, size_t unitsToWrite)
    {
        return Pull(unitsWritten, unitsToWrite) ? Push(unitsWritten, unitsWritten) : false;
    }

    //pull the requested amount of data from the stream
    bool StdOutSink::Pull(size_t& unitsWritten, size_t unitsToWrite)
    {
        if( GetFlowMode() == FlowMode::Pull )
        {
            return (ActualSource()->Read(unitsWritten, unitsToWrite) && (unitsWritten > 0)) ? true : false;
        }
        return true;
    }

    //push the requested amount of data out of the door
    bool StdOutSink::Push(size_t& unitsWritten, size_t unitsToWrite)
    {
        pipeline::Buffer* buffer = GetBuffer();
        if(buffer)
        {
            byte* pData = reinterpret_cast<byte*>(buffer->ReadRequest(unitsToWrite));
            size_t unitSize = buffer->GetUnitSize();							        //The Buffer knows the Unit size in bytes
            size_t bytesWritten = fwrite(pData, unitSize, unitsToWrite, stdout);
            if( bytesWritten > 0 )
            {
                unitsWritten = bytesWritten / buffer->GetUnitSize();
                buffer->ReadAcknowledge(unitsWritten);
                OnWriteSuccess(unitsWritten);                
            }
            else //EOD?
            {
                return false;
            }
            return true;
        }
        return false;
    }
    
}}}//qor::io::components
