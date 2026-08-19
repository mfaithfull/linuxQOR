// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "sink.h"
#include "src/framework/data/pipeline/source.h"
#include "../responseparser/responsenode.h"

namespace qor { namespace components { namespace protocols { namespace http {

    HTTPSink::HTTPSink() : m_context(new_ref<pipeline::BufferContext<uint32_t>()>())
    {
        m_parser.SetInitialStep(new_ref<response::Initial>(&m_parser));
    }

    ref_of<HTTPResponse>::type HTTPSink::GetResponse()
    {
        return m_response;
    }    

    bool HTTPSink::Write(size_t& unitsWritten, size_t unitsToWrite)
    {
        return (unitsToWrite == 0 || Pull(unitsWritten, unitsToWrite)) ? Push(unitsWritten, unitsWritten) : false;
    }

    bool HTTPSink::Pull(size_t& unitsWritten, size_t unitsToWrite)
    {
        return GetFlowMode() == FlowMode::Pull ? 
        (ActualSource()->Read(unitsWritten, unitsToWrite) && (unitsWritten > 0 || unitsToWrite == 0) ? true : false) : true;        
    }

    bool HTTPSink::Push(size_t& unitsWritten, size_t unitsToWrite)
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
                else
                {
                    return false;
                }
                return true;
            }
            return false;
        }
        return true;
    }

    size_t HTTPSink::Write(byte* data, size_t bytesToWrite)
    {
        //m_context->SetData(data, bytesToWrite);
        m_parser.Parse();
        if(m_parser.IsComplete())
        {
            auto responseNode = m_parser.PopNode().template AsRef<response::ResponseNode>();
            m_response = responseNode->GetObject();
        }
        return m_context->GetPosition();
    }

}}}}//qor::components::protocols::http
