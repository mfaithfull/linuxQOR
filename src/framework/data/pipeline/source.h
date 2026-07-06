// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_SOURCE
#define QOR_PP_H_PIPELINE_SOURCE

#include "element.h"
#include "src/framework/event/signals/signal.h"

namespace qor{ namespace pipeline{

#ifndef QOR_PP_H_PIPELINE_SINK
    class qor_pp_module_interface(QOR_PIPELINE) Sink;
#endif

    class qor_pp_module_interface(QOR_PIPELINE) Source : public virtual Element, public virtual SignalBase
    {
    public:

        qor_pp_signal_func ReadSuccessSignal(size_t unitsRead);
        qor_pp_signal_func EndOfDataSignal(void);        
        qor_pp_signal_func ReadErrorSignal(size_t unitsRead, size_t error);        

        Source();
        virtual ~Source();
        Source(const Source& src);
        Source& operator = (const Source& src);

        virtual void SetSink(Element* pSink);
        virtual void SetSource(Element* source);
        virtual Element* GetSink(void);
        virtual bool HasSink(void);
        virtual bool Read(size_t& numberOfUnitsRead, size_t numberOfUnitsToRead = 1);
        virtual void OnReadSuccess(size_t unitsRead);
        virtual void OnReadError(size_t error, size_t unitsRead);
        virtual void OnEndOfData();
        virtual bool IsSource();
        virtual void SetPlug(Element* plug);
        virtual Element* GetPlug() const;
        virtual bool CheckComplete();
        virtual const char* Name() const override;

    protected:

        Element* m_sink;
        Sink* ActualSink();
    };

    class qor_pp_module_interface(QOR_PIPELINE) NullSource : public Source
    {
    public:
        NullSource();
        virtual ~NullSource();
        virtual bool Read(size_t& numberOfUnitsRead, size_t numberOfUnitsToRead = 1);
    };

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_SOURCE
