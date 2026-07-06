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

    class qor_pp_module_interface_gcc(QOR_PIPELINE) Pipeline : public Filter
    {
    public:

        enum FilterPos
        {
            BeforeSink,
            AfterSource,
        };

        qor_pp_module_interface(QOR_PIPELINE) Pipeline();
        qor_pp_module_interface(QOR_PIPELINE) virtual ~Pipeline();
        Pipeline( const Pipeline& src ) = delete;
        Pipeline& operator = ( const Pipeline& src ) = delete;

        qor_pp_module_interface(QOR_PIPELINE) Pipeline(Connection* SourceConnection, Connection* SinkConnection, Element::FlowMode flowmode = Element::FlowMode::Pull);
        qor_pp_module_interface(QOR_PIPELINE) Pipeline(const Plug& SourceConnection, Sink* sink, Element::FlowMode flowmode = Element::FlowMode::Pull);
        qor_pp_module_interface(QOR_PIPELINE) Pipeline(Plug& SourceConnection, Plug& SinkConnection, Element::FlowMode flowmode = Element::FlowMode::Pull);
        qor_pp_module_interface(QOR_PIPELINE) Pipeline(const Plug& SourceConnection, const Plug& SinkConnection, Element::FlowMode flowmode = Element::FlowMode::Pull);
        qor_pp_module_interface(QOR_PIPELINE) Pipeline(const Plug& SourceConnection, Sink& sink, Element::FlowMode flowmode = Element::FlowMode::Pull);
        qor_pp_module_interface(QOR_PIPELINE) Pipeline(const Plug& SourceConnection, const Sink& sink, Element::FlowMode flowmode = Element::FlowMode::Pull);
        qor_pp_module_interface(QOR_PIPELINE) Pipeline(Source& source, const Plug& SinkConnection, Element::FlowMode flowmode = Element::FlowMode::Push);
        qor_pp_module_interface(QOR_PIPELINE) Pipeline(const Source& source, const Plug& SinkConnection, Element::FlowMode flowmode = Element::FlowMode::Push);
        qor_pp_module_interface(QOR_PIPELINE) Pipeline& Connect();
        qor_pp_module_interface(QOR_PIPELINE) bool IsConnected();  //assumes true if there is nothing setup to connect to, like a string source or parser sink.
        qor_pp_module_interface(QOR_PIPELINE) void SetSource(Element* source, Buffer* buffer);
        qor_pp_module_interface(QOR_PIPELINE) void SetSource(const Source& source);
        qor_pp_module_interface(QOR_PIPELINE) void SetSource(Element* source);
        qor_pp_module_interface(QOR_PIPELINE) void SetSink(Element* sink, Buffer* buffer);
        qor_pp_module_interface(QOR_PIPELINE) void SetSink(const Sink& sink);
        qor_pp_module_interface(QOR_PIPELINE) void SetSink(Element* sink);
        qor_pp_module_interface(QOR_PIPELINE) void SetSinkConnector(Element* plug);
        qor_pp_module_interface(QOR_PIPELINE) void SetSourceConnector(Element* plug);
        qor_pp_module_interface(QOR_PIPELINE) Plug* GetSourceConnector();
        qor_pp_module_interface(QOR_PIPELINE) void SetConnector(Element* plug);
        qor_pp_module_interface(QOR_PIPELINE) Plug* GetSinkConnector();
        qor_pp_module_interface(QOR_PIPELINE) virtual Element::FlowMode GetFlowMode() override;
        qor_pp_module_interface(QOR_PIPELINE) void SetFlowMode(Element::FlowMode flowmode);
        qor_pp_module_interface(QOR_PIPELINE) virtual void Run(void);        
        qor_pp_module_interface(QOR_PIPELINE) virtual bool Pump(size_t& unitsPumped, size_t unitsToPump);
        qor_pp_module_interface(QOR_PIPELINE) size_t PumpAll();
        qor_pp_module_interface(QOR_PIPELINE) bool PumpOne();
        qor_pp_module_interface(QOR_PIPELINE) virtual Pipeline& InsertFilter(Filter* filter, FilterPos Pos = BeforeSink);
        qor_pp_module_interface(QOR_PIPELINE) virtual Pipeline& InsertInlineFilter(const Buffer& filter, FilterPos Pos = BeforeSink);
        qor_pp_module_interface(QOR_PIPELINE) virtual void ResetStream(size_t streamSize = 0);
        qor_pp_module_interface(QOR_PIPELINE) bool CheckComplete();
        
    protected:

        FlowMode m_flowmode;

    };

}}//qor::pipeline

#endif//QOR_PP_H_FRAMEWORK_PIPELINE
