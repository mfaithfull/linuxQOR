// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_SINK
#define QOR_PP_H_PIPELINE_SINK

#include "element.h"
#include "src/framework/event/signals/signal.h"

namespace qor{ namespace pipeline{

#ifndef QOR_PP_H_PIPELINE_SOURCE
    class qor_pp_module_interface_gcc(QOR_PIPELINE) Source;
#endif//QOR_PP_H_PIPELINE_SOURCE

    class qor_pp_module_interface_gcc(QOR_PIPELINE) Sink: public virtual Element, public virtual SignalBase
    {
    public:

        qor_pp_module_interface(QOR_PIPELINE) qor_pp_signal_func WriteSuccessSignal(size_t unitsWritten);
        qor_pp_module_interface(QOR_PIPELINE) qor_pp_signal_func WriteErrorSignal(size_t unitsWritten, size_t error);

        qor_pp_module_interface(QOR_PIPELINE) Sink();
        qor_pp_module_interface(QOR_PIPELINE) virtual ~Sink();
        qor_pp_module_interface(QOR_PIPELINE) Sink( const Sink& src);
        qor_pp_module_interface(QOR_PIPELINE) Sink& operator = (const Sink& src);
        
        qor_pp_module_interface(QOR_PIPELINE) virtual void SetSource(Element* source);
        qor_pp_module_interface(QOR_PIPELINE) virtual Element* GetSource();
        qor_pp_module_interface(QOR_PIPELINE) virtual bool HasSource();
        qor_pp_module_interface(QOR_PIPELINE) virtual bool Write(size_t& unitsWritten, size_t unitstoWrite = 1 );
        qor_pp_module_interface(QOR_PIPELINE) virtual void OnWriteSuccess(size_t unitsWritten);
        qor_pp_module_interface(QOR_PIPELINE) virtual void OnWriteError(size_t error, size_t unitsWritten);
        qor_pp_module_interface(QOR_PIPELINE) virtual bool IsSink();
        qor_pp_module_interface(QOR_PIPELINE) virtual void SetPlug(Element* plug);
        qor_pp_module_interface(QOR_PIPELINE) virtual Element* GetPlug() const;
        qor_pp_module_interface(QOR_PIPELINE) virtual bool CheckComplete();
        qor_pp_module_interface(QOR_PIPELINE) virtual const char* Name() const override;

    protected:

        qor_pp_module_interface(QOR_PIPELINE) virtual void SetSink(Element*);

        Element* m_source;
        qor_pp_module_interface(QOR_PIPELINE) Source* ActualSource();

        qor_pp_module_interface(QOR_PIPELINE) virtual bool Push(size_t& unitsWritten, size_t unitsToWrite);
        qor_pp_module_interface(QOR_PIPELINE) virtual bool Pull(size_t& unitsWritten, size_t unitsToWrite);
        qor_pp_module_interface(QOR_PIPELINE) virtual size_t WriteBytes(byte* data, size_t bytesToWrite);

    };

    class qor_pp_module_interface_gcc(QOR_PIPELINE) NullSink : public Sink
    {
    public:
        qor_pp_module_interface(QOR_PIPELINE) NullSink();        
        qor_pp_module_interface(QOR_PIPELINE) virtual ~NullSink();
        qor_pp_module_interface(QOR_PIPELINE) NullSink& operator = (const NullSink&);
        qor_pp_module_interface(QOR_PIPELINE) virtual bool Write(size_t& unitsWritten, size_t unitsToWrite = 1 );
    };

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_SINK
