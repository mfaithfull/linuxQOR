// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_ELEMENT
#define QOR_PP_H_PIPELINE_ELEMENT

#include "buffer.h"

namespace qor{ namespace pipeline{

    class qor_pp_module_interface_gcc(QOR_PIPELINE) Source;
    class qor_pp_module_interface_gcc(QOR_PIPELINE) Sink;

    //a base class for pipeline elements of various kinds
    class qor_pp_module_interface(QOR_PIPELINE) Element
    {
    public:

        enum FlowMode
        {
            Pull,
            Push
        };

        Element();
        virtual ~Element() noexcept (false);
        Element( const Element& src );
        Element& operator = (const Element& src);
        Element* GetParent(void);
        void SetParent(Element* parent);

        virtual bool IsBuffered();
        virtual bool IsSource();
        virtual bool IsSink();
        virtual bool IsPlug();
        virtual void SetBuffer(Buffer* buffer);
        virtual void SetBuffer(const Buffer& buffer);
        Buffer* GetBuffer();
        virtual bool HasBuffer();
        virtual void SetSource(Element* source) = 0;
        virtual Element* GetSource();
        virtual bool HasSource();
        virtual void SetSink(Element* sink) = 0;
        virtual Element* GetSink() const;
        virtual bool HasSink();
        virtual FlowMode GetFlowMode();
        virtual void SetFlowMode(FlowMode flowmode);
        virtual const char* Name() const;

    protected:

        Element* m_parent;// The owning element or pipeline
        Buffer* m_buffer;
    };

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_ELEMENT
