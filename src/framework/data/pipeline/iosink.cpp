// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/framework/data/pipeline/source.h"
#include "iosink.h"
#include "src/qor/flyers/error/error.h"

namespace qor{ namespace pipeline{ 

    iosink_base::iosink_base() : pipeline::Sink(), m_plug(nullptr) { }

    iosink_base::~iosink_base() { }

    void iosink_base::SetPlug(Element* source)
    {
        m_plug = source;
    }

    pipeline::Element* iosink_base::GetPlug() const
    {
        return m_plug;
    }

    bool iosink_base::HasPlug()
    {
        return m_plug != nullptr;
    }

    pipeline::Plug* iosink_base::ActualPlug()
    {
        return dynamic_cast<pipeline::Plug*>(m_plug);
    }

    bool iosink_base::Write(size_t& unitsWritten, size_t unitsToWrite)
    {        
        return (unitsToWrite == 0 || Pull(unitsWritten, unitsToWrite)) ? Push(unitsWritten, unitsWritten) : false;
    }

    //pull the requested amount of data from the source to the buffer
    bool iosink_base::Pull(size_t& unitsWritten, size_t unitsToWrite)
    {
        return GetFlowMode() == FlowMode::Pull ? 
        (ActualSource()->Read(unitsWritten, unitsToWrite) && (unitsWritten > 0 || unitsToWrite == 0) ? true : false) : true;
    }

    //push the requested amount of data from the buffer to the sink
    bool iosink_base::Push(size_t& unitsWritten, size_t unitsToWrite)
    {
        if(unitsToWrite > 0)
        {
            pipeline::Buffer* buffer = GetBuffer();
            if(buffer)
            {
                byte* data = buffer->ReadRequest(unitsToWrite);
                size_t bytesWritten = WriteBytes(data, buffer->GetUnitSize() * unitsToWrite);
                if(bytesWritten > 0)
                {
                    unitsWritten = bytesWritten / buffer->GetUnitSize();
                    buffer->ReadAcknowledge(unitsWritten);
                    OnWriteSuccess(unitsWritten);
                    return true;
                }
                else
                {
                    return false;//Nothing was written, the sink should have raised an error if that's a problem
                }                
            }
            else
            {
                return false;//TODO: Raise no source buffer error
            }
        }
        else
        {
            return true;//Writing zero units always succeeds
        }
    }

    const char* iosink_base::Name() const
    {
        return "iosink_base";
    }

    size_t iosink_base::WriteBytes(byte* data, size_t bytesToWrite)
    {
        fatal("Empty base called. Please overrride bool WriteBytes(byte*, size_t); in your pipeline::io_sink derived class.");
        return 0;
    }
}}//qor::components
