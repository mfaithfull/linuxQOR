// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_PIPELINE_IOSINK
#define QOR_PP_H_FRAMEWORK_PIPELINE_IOSINK

#include "src/framework/data/pipeline/sink.h"
#include "src/framework/data/pipeline/plug.h"

namespace qor{ namespace pipeline{ 

    class qor_pp_module_interface(QOR_PIPELINE) iosink_base : public Sink
    {
    public:

        iosink_base();
        virtual ~iosink_base();

        virtual bool Write(size_t& unitsWritten, size_t unitsToWrite = 1 );
        
        virtual void SetPlug(Element* pPlug);
        virtual Element* GetPlug() const;
        virtual bool HasPlug();
        virtual const char* Name() const override;
        //virtual size_t WriteBytes(byte* data, size_t bytesToWrite);

    protected:
        
        //bool Pull(size_t& unitsWritten, size_t unitsToWrite);
        //bool Push(size_t& unitsWritten, size_t unitsToWrite);
        Plug* ActualPlug();

        Element* m_plug;

    };

    template<class PlugType >
    class iosink : public iosink_base
    {
    public:

        iosink() : iosink_base(), m_Connector(nullptr){ }

        virtual ~iosink() = default;

        virtual void SetPlug(Element* pPlug)
        {
            iosink_base::SetPlug(pPlug);
            m_Connector = dynamic_cast<PlugType*>(m_plug);
            if(m_plug == nullptr)
            {
                //Wrong type of plug
            }
        }

        virtual Element* GetPlug() const
        {
            return m_Connector;
        }

        //virtual size_t WriteBytes(byte* space, size_t bytesToRead) = 0;

    protected:
        
        PlugType* m_Connector;
    };

}}//qor::pipeline

#endif//QOR_PP_H_FRAMEWORK_PIPELINE_IOSINK
