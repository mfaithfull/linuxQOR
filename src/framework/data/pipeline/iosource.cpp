// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "iosource.h"
#include "sink.h"
#include "src/qor/flyers/error/error.h"

namespace qor{ namespace pipeline{ 

    iosource_base::iosource_base() : pipeline::Source(), m_plug(nullptr) { }

    iosource_base::~iosource_base() { }

    void iosource_base::SetPlug(Element* plug)
    {
        m_plug = plug;
    }

    pipeline::Element* iosource_base::GetPlug() const
    {
        return m_plug;
    }

    bool iosource_base::HasPlug()
    {
        return m_plug != nullptr;
    }

    pipeline::Plug* iosource_base::ActualPlug()
    {
        return dynamic_cast<pipeline::Plug*>(m_plug);
    }
/*
    bool iosource_base::Read(size_t& unitsRead, size_t unitsToRead)
    {        
        return Pull(unitsRead, unitsToRead) ? Push(unitsRead, unitsRead) : false;
    }

    //pull the requested amount of data from the stream
    bool iosource_base::Pull(size_t& unitsRead, size_t unitsToRead)
    {
        pipeline::Buffer* buffer = GetBuffer();
        if(buffer)
        {
            byte* space = buffer->WriteRequest(unitsToRead);            
            size_t bytesRead = ReadBytes(space, buffer->GetUnitSize() * unitsToRead);
            if(bytesRead > 0)
            {
                unitsRead = bytesRead / buffer->GetUnitSize();
                buffer->WriteAcknowledge(unitsRead);
                OnReadSuccess(unitsRead);
            }
            else //EOF
            {
                OnEndOfData();
            }
            return true;
        }
        else
        {
            return false;
        }
    }

    //push the requested amount of data up the pipeline
    bool iosource_base::Push(size_t& unitsRead, size_t unitsToRead)
    {
        return ( GetFlowMode() == FlowMode::Push ) ? (ActualSink()->Write(unitsRead, unitsToRead) && (unitsRead > 0)) : true;
    }
*/
    const char* iosource_base::Name() const
    {
        return "iosource_base";
    }

    size_t iosource_base::ReadBytes(byte* space, size_t bytesToRead)
    {
        fatal("Empty base called. Please overrride bool ReadBytes(byte*, size_t); in your pipeline::io_source derived class.");
        return 0;
    }

}}//qor::components
