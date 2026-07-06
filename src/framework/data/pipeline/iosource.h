// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_PIPELINE_IOSOURCE
#define QOR_PP_H_FRAMEWORK_PIPELINE_IOSOURCE

#include "src/framework/data/pipeline/source.h"
#include "src/framework/data/pipeline/plug.h"

namespace qor{ namespace pipeline{ 

    class qor_pp_module_interface(QOR_PIPELINE) iosource_base : public Source
    {
    public:

        iosource_base();
        virtual ~iosource_base();

        virtual bool Read(size_t& numberOfUnitsRead, size_t numberOfUnitsToRead = 1);
        
        virtual void SetPlug(Element* pPlug);
        virtual Element* GetPlug() const;
        virtual bool HasPlug();
        virtual const char* Name() const override;

    protected:
        
        virtual size_t ReadBytes(byte* space, size_t bytesToRead);
        bool Pull(size_t& unitsRead, size_t unitsToRead);
        bool Push(size_t& unitsRead, size_t unitsToRead);
        Plug* ActualPlug();

        Element* m_plug;

    };

    template<class PlugType >
    class iosource : public iosource_base
    {
    public:

        iosource() : iosource_base(), m_Connector(nullptr){ }

        inline virtual ~iosource() = default;

        virtual void SetPlug(Element* pPlug)
        {
            iosource_base::SetPlug(pPlug);
            m_Connector = dynamic_cast<PlugType*>(m_plug);
            if(m_plug == nullptr)
            {
                //TODO: Wrong type of plug
            }
        }

    protected:

        virtual size_t ReadBytes(byte* space, size_t bytesToRead) = 0;
        
        PlugType* m_Connector;
    };

}}//qor::pipeline

#endif//QOR_PP_H_FRAMEWORK_PIPELINE_IOSOURCE
