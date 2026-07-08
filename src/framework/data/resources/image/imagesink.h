// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_RESOURCES_IMAGE_CONNECTOR
#define QOR_PP_H_FRAMEWORK_RESOURCES_IMAGE_CONNECTOR

#include "src/qor/essentials/current/currentthread.h"
#include "src/qor/memory/injection/typeidentity.h"
#include "src/qor/memory/reference/newref.h"
#include "src/framework/data/pipeline/connection.h"
#include "src/framework/data/pipeline/source.h"
#include "src/framework/data/pipeline/iosink.h"
#include "src/framework/data/pipeline/iosource.h"
#include "image.h"

namespace qor{ namespace framework{ 

    class qor_pp_module_interface(QOR_IMAGE) ImageSink : public pipeline::Sink
    {
    public:

        ImageSink(Image& image) : m_image(image){}
        virtual ~ImageSink() = default;

        bool Write(size_t& unitsWritten, size_t unitsToWrite) override
        {        
            return (unitsToWrite == 0 || Pull(unitsWritten, unitsToWrite)) ? Push(unitsWritten, unitsWritten) : false;
        }

        //pull the requested amount of data from the source to the buffer
        bool Pull(size_t& unitsWritten, size_t unitsToWrite)
        {
            return GetFlowMode() == FlowMode::Pull ? 
            (ActualSource()->Read(unitsWritten, unitsToWrite) && (unitsWritten > 0 || unitsToWrite == 0) ? true : false) : true;
        }

        //push the requested amount of data from the buffer to the sink
        bool Push(size_t& unitsWritten, size_t unitsToWrite)
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

        size_t WriteBytes(byte* data, size_t bytesToWrite)
        {
            size_t result = 0;
            memcpy(m_image[m_index], data, bytesToWrite);
            result = bytesToWrite;
            m_index+= result;
            return result;
        }

    protected:

        Image& m_image;
        size_t m_index{0};
    };

}}//qor::framwork

#endif//QOR_PP_H_FRAMEWORK_RESOURCES_IMAGE_CONNECTOR
