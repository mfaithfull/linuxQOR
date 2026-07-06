// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_PIPELINE
#define QOR_PP_H_FRAMEWORK_PIPELINE

#include "element.h"
#include "filter.h"
#include "inlinefilter.h"
#include "connection.h"
#include "src/qor/flyers/log/debug.h"

namespace qor{ namespace pipeline{

    class qor_pp_module_interface(QOR_PIPELINE) Pipeline : public Filter
    {
    public:

        enum FilterPos
        {
            BeforeSink,
            AfterSource,
        };

        Pipeline();
        virtual ~Pipeline();
        Pipeline( const Pipeline& src ) = delete;

        Pipeline(Connection* SourceConnection, Connection* SinkConnection, Element::FlowMode flowmode = Element::FlowMode::Pull);
        Pipeline(const Plug& SourceConnection, Sink* sink, Element::FlowMode flowmode = Element::FlowMode::Pull);
        Pipeline(Plug& SourceConnection, Plug& SinkConnection, Element::FlowMode flowmode = Element::FlowMode::Pull);
        Pipeline(const Plug& SourceConnection, const Plug& SinkConnection, Element::FlowMode flowmode = Element::FlowMode::Pull);
        Pipeline(const Plug& SourceConnection, Sink& sink, Element::FlowMode flowmode = Element::FlowMode::Pull);
        Pipeline(const Plug& SourceConnection, const Sink& sink, Element::FlowMode flowmode = Element::FlowMode::Pull);
        Pipeline(Source& source, const Plug& SinkConnection, Element::FlowMode flowmode = Element::FlowMode::Push);
        Pipeline(const Source& source, const Plug& SinkConnection, Element::FlowMode flowmode = Element::FlowMode::Push);
        Pipeline& Connect();
        bool IsConnected();  //assumes true if there is nothing setup to connect to, like a string source or parser sink.
        void SetSource(Element* source, Buffer* buffer);
        void SetSource(const Source& source);
        void SetSource(Element* source);
        void SetSink(Element* sink, Buffer* buffer);
        void SetSink(const Sink& sink);
        void SetSink(Element* sink);
        void SetSinkConnector(Element* plug);
        void SetSourceConnector(Element* plug);
        Plug* GetSourceConnector();
        void SetConnector(Element* plug);
        Plug* GetSinkConnector();
        virtual Element::FlowMode GetFlowMode() override;
        void SetFlowMode(Element::FlowMode flowmode);
        virtual void Run(void);        
        virtual bool Pump(size_t& unitsPumped, size_t unitsToPump);
        size_t PumpAll();
        bool PumpOne();
        virtual Pipeline& InsertFilter(Filter* filter, FilterPos Pos = BeforeSink);
        virtual Pipeline& InsertInlineFilter(const Buffer& filter, FilterPos Pos = BeforeSink);
        virtual void ResetStream(size_t streamSize = 0);
        bool CheckComplete();
        
    protected:

        FlowMode m_flowmode;

    };

}}//qor::pipeline

#endif//QOR_PP_H_FRAMEWORK_PIPELINE
