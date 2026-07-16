// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_FILTER
#define QOR_PP_H_PIPELINE_FILTER

#include <functional>
#include "source.h"
#include "sink.h"

namespace qor{ namespace pipeline{

    class qor_pp_module_interface_gcc(QOR_PIPELINE) Filter : public Sink, public Source
    {
    public:

        qor_pp_module_interface(QOR_PIPELINE) Filter();
        qor_pp_module_interface(QOR_PIPELINE) virtual ~Filter();
        qor_pp_module_interface(QOR_PIPELINE) Filter(const Filter& src);
        qor_pp_module_interface(QOR_PIPELINE) Filter& operator = (const Filter& src);
        qor_pp_module_interface(QOR_PIPELINE) virtual void SetSink(Element* sink);
        qor_pp_module_interface(QOR_PIPELINE) virtual Element* GetSink();
        qor_pp_module_interface(QOR_PIPELINE) virtual bool HasSink();
        qor_pp_module_interface(QOR_PIPELINE) virtual void SetSource(Element* source);
        qor_pp_module_interface(QOR_PIPELINE) virtual Element* GetSource();
        qor_pp_module_interface(QOR_PIPELINE) virtual bool HasSource();
        qor_pp_module_interface(QOR_PIPELINE) virtual bool IsSource();
        qor_pp_module_interface(QOR_PIPELINE) virtual bool IsSink();
        qor_pp_module_interface(QOR_PIPELINE) virtual size_t PumpSome();
        qor_pp_module_interface(QOR_PIPELINE) virtual bool PumpSome(size_t& unitsPumped, size_t maxUnitsToPump);
        qor_pp_module_interface(QOR_PIPELINE) virtual bool Pump(size_t& unitsPumped, size_t unitsToPump);
        qor_pp_module_interface(QOR_PIPELINE) virtual bool Pump(size_t& unitsPumped, size_t unitsToPump, std::function<bool(size_t&, size_t)> filterFn);
        qor_pp_module_interface(QOR_PIPELINE) virtual bool ReadFilter(size_t& unitsProcessed, size_t unitsToProcess);
        qor_pp_module_interface(QOR_PIPELINE) virtual bool WriteFilter(size_t& unitsProcessed, size_t unitsToProcess);
        qor_pp_module_interface(QOR_PIPELINE) virtual const char* Name() const override;

    protected:
        qor_pp_module_interface(QOR_PIPELINE) virtual bool Read(size_t& numberOfUnitsRead, size_t numberOfUnitsToRead = 1);
        qor_pp_module_interface(QOR_PIPELINE) virtual bool Write(size_t& unitsWritten, size_t unitstoWrite = 1 );

    private:

        qor_pp_module_interface(QOR_PIPELINE) bool Pull(size_t& unitsRead, size_t unitsToRead);
        qor_pp_module_interface(QOR_PIPELINE) bool Push(size_t& unitsWritten, size_t unitsToWrite);

    };

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_FILTER
