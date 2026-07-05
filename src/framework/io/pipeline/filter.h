// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PIPELINE_FILTER
#define QOR_PP_H_PIPELINE_FILTER

#include <functional>
#include "source.h"
#include "sink.h"

namespace qor{ namespace pipeline{

    class qor_pp_module_interface(QOR_PIPELINE) Filter : public Sink, public Source
    {
    public:

        Filter();
        virtual ~Filter() = default;
        Filter(const Filter& src);
        Filter& operator = (const Filter& src);
        virtual void SetSink(Element* sink);
        virtual Element* GetSink();
        virtual bool HasSink();
        virtual void SetSource(Element* source);
        virtual Element* GetSource();
        virtual bool HasSource();        
        virtual bool IsSource();
        virtual bool IsSink();
        virtual size_t PumpSome();
        virtual bool PumpSome(size_t& unitsPumped, size_t maxUnitsToPump);
        virtual bool Pump(size_t& unitsPumped, size_t unitsToPump);
        virtual bool Pump(size_t& unitsPumped, size_t unitsToPump, std::function<bool(size_t&, size_t)> filterFn);
        virtual bool ReadFilter(size_t& unitsProcessed, size_t unitsToProcess);
        virtual bool WriteFilter(size_t& unitsProcessed, size_t unitsToProcess);
        virtual const char* Name() const override;

    protected:
        virtual bool Read(size_t& numberOfUnitsRead, size_t numberOfUnitsToRead = 1);
        virtual bool Write(size_t& unitsWritten, size_t unitstoWrite = 1 );

    private:

        bool Pull(size_t& unitsRead, size_t unitsToRead);
        bool Push(size_t& unitsWritten, size_t unitsToWrite);

    };

}}//qor::pipeline

#endif//QOR_PP_H_PIPELINE_FILTER
