// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "stdoutsink.h"
#include "src/framework/data/pipeline/source.h"

namespace qor{ namespace io{ namespace components{ 

    StdOutSink::StdOutSink() = default;

    StdOutSink::StdOutSink(const StdOutSink& src) : pipeline::Sink(src){ }

    StdOutSink& StdOutSink::operator = (const StdOutSink& src)
    {
        pipeline::Sink::operator=(src);
        return *this;
    }

    StdOutSink::~StdOutSink()
    {
        fflush(stdout);
    }

    //push the requested amount of data out of stdout
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
                unitsWritten = bytesWritten / unitSize;
                buffer->ReadAcknowledge(unitsWritten);
                OnWriteSuccess(unitsWritten);                
                return true;
            }
        }
        return false;
    }
    
}}}//qor::io::components
