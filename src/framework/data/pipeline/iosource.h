// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_PIPELINE_IOSOURCE
#define QOR_PP_H_FRAMEWORK_PIPELINE_IOSOURCE

#include "src/framework/data/pipeline/source.h"
#include "src/framework/data/pipeline/plug.h"

namespace qor{ namespace pipeline{ 

    class qor_pp_module_interface_gcc(QOR_PIPELINE) iosource_base : public Source
    {
    public:

        qor_pp_module_interface(QOR_PIPELINE) iosource_base();
        qor_pp_module_interface(QOR_PIPELINE) virtual ~iosource_base();

        qor_pp_module_interface(QOR_PIPELINE) virtual void SetPlug(Element* pPlug);
        qor_pp_module_interface(QOR_PIPELINE) virtual Element* GetPlug() const;
        qor_pp_module_interface(QOR_PIPELINE) virtual bool HasPlug();
        qor_pp_module_interface(QOR_PIPELINE) virtual const char* Name() const override;

    protected:
        
        qor_pp_module_interface(QOR_PIPELINE) virtual size_t ReadBytes(byte* space, size_t bytesToRead);
        qor_pp_module_interface(QOR_PIPELINE) Plug* ActualPlug();

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
        
        PlugType* m_Connector;
    };

}}//qor::pipeline

#endif//QOR_PP_H_FRAMEWORK_PIPELINE_IOSOURCE
