// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_SOURCE
#define QOR_PP_H_PIPELINE_SOURCE

#include "element.h"
#include "src/framework/event/signals/signal.h"

namespace qor{ namespace pipeline{

#ifndef QOR_PP_H_PIPELINE_SINK
    class qor_pp_module_interface_gcc(QOR_PIPELINE) Sink;
#endif

    class qor_pp_module_interface_gcc(QOR_PIPELINE) Source : public virtual Element, public virtual SignalBase
    {
    public:

        qor_pp_module_interface(QOR_PIPELINE) qor_pp_signal_func ReadSuccessSignal(size_t unitsRead);
        qor_pp_module_interface(QOR_PIPELINE) qor_pp_signal_func EndOfDataSignal(void);        
        qor_pp_module_interface(QOR_PIPELINE) qor_pp_signal_func ReadErrorSignal(size_t unitsRead, size_t error);        

        qor_pp_module_interface(QOR_PIPELINE) Source();
        qor_pp_module_interface(QOR_PIPELINE) virtual ~Source();
        qor_pp_module_interface(QOR_PIPELINE) Source(const Source& src);
        qor_pp_module_interface(QOR_PIPELINE) Source& operator = (const Source& src);

        qor_pp_module_interface(QOR_PIPELINE) virtual void SetSink(Element* pSink);
        qor_pp_module_interface(QOR_PIPELINE) virtual void SetSource(Element* source);
        qor_pp_module_interface(QOR_PIPELINE) virtual Element* GetSink(void);
        qor_pp_module_interface(QOR_PIPELINE) virtual bool HasSink(void);
        qor_pp_module_interface(QOR_PIPELINE) virtual bool Read(size_t& unitsRead, size_t unitsToRead = 1);
        qor_pp_module_interface(QOR_PIPELINE) virtual void OnReadSuccess(size_t unitsRead);
        qor_pp_module_interface(QOR_PIPELINE) virtual void OnReadError(size_t error, size_t unitsRead);
        qor_pp_module_interface(QOR_PIPELINE) virtual void OnEndOfData();
        qor_pp_module_interface(QOR_PIPELINE) virtual bool IsSource();
        qor_pp_module_interface(QOR_PIPELINE) virtual void SetPlug(Element* plug);
        qor_pp_module_interface(QOR_PIPELINE) virtual Element* GetPlug() const;
        qor_pp_module_interface(QOR_PIPELINE) virtual bool CheckComplete();
        qor_pp_module_interface(QOR_PIPELINE) virtual const char* Name() const override;

    protected:

        Element* m_sink;
        qor_pp_module_interface(QOR_PIPELINE) Sink* ActualSink();

        qor_pp_module_interface(QOR_PIPELINE) virtual bool Push(size_t& unitsRead, size_t unitsToRead);
        qor_pp_module_interface(QOR_PIPELINE) virtual bool Pull(size_t& unitsRead, size_t unitsToRead);
        qor_pp_module_interface(QOR_PIPELINE) virtual size_t ReadBytes(byte* space, size_t bytesToRead);

    };

    class qor_pp_module_interface_gcc(QOR_PIPELINE) NullSource : public Source
    {
    public:
        qor_pp_module_interface(QOR_PIPELINE) NullSource();
        qor_pp_module_interface(QOR_PIPELINE) NullSource& operator = (const NullSource&);
        qor_pp_module_interface(QOR_PIPELINE) virtual ~NullSource();
        qor_pp_module_interface(QOR_PIPELINE) virtual bool Read(size_t& numberOfUnitsRead, size_t numberOfUnitsToRead = 1);
    };

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_SOURCE
