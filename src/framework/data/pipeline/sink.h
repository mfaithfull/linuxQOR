// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_SINK
#define QOR_PP_H_PIPELINE_SINK

#include "element.h"
#include "src/framework/event/signals/signal.h"

namespace qor{ namespace pipeline{

#ifndef QOR_PP_H_PIPELINE_SOURCE
    class qor_pp_module_interface(QOR_PIPELINE) Source;
#endif//QOR_PP_H_PIPELINE_SOURCE

    class qor_pp_module_interface(QOR_PIPELINE) Sink: public virtual Element, public virtual SignalBase
    {
    public:

        qor_pp_signal_func WriteSuccessSignal(size_t unitsWritten);
        qor_pp_signal_func WriteErrorSignal(size_t unitsWritten, size_t error);

        Sink();
        virtual ~Sink();
        Sink( const Sink& src);
        Sink& operator = (const Sink& src);

        virtual void SetSource(Element* source);
        virtual Element* GetSource();
        virtual bool HasSource();
        virtual bool Write(size_t& unitsWritten, size_t unitstoWrite = 1 );
        virtual void OnWriteSuccess(size_t unitsWritten);
        virtual void OnWriteError(size_t error, size_t unitsWritten);
        virtual bool IsSink();
        virtual void SetPlug(Element* plug);
        virtual Element* GetPlug() const;
        virtual bool CheckComplete();
        virtual const char* Name() const override;

    protected:
        virtual void SetSink(Element*){};

        Element* m_source;
        Source* ActualSource();

        virtual bool Push(size_t& unitsWritten, size_t unitsToWrite);
        virtual bool Pull(size_t& unitsWritten, size_t unitsToWrite);
        virtual size_t WriteBytes(byte* data, size_t bytesToWrite);

    };

    class qor_pp_module_interface(QOR_PIPELINE) NullSink : public Sink
    {
    public:
        NullSink();
        virtual ~NullSink();
        virtual bool Write(size_t& unitsWritten, size_t unitsToWrite = 1 );
    };

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_SINK
