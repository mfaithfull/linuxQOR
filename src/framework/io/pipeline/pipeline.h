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
        virtual ~Pipeline() = default;
        Pipeline( const Pipeline& src ) = delete;

        Pipeline(Connection* SourceConnection, Connection* SinkConnection, Element::FlowMode flowmode = Element::FlowMode::Pull) : m_flowmode(flowmode)
        {
            SetSink(SinkConnection->GetSink());
            SetSource(SourceConnection->GetSource());
        }

        Pipeline(const Plug& SourceConnection, Sink* sink, Element::FlowMode flowmode = Element::FlowMode::Pull) : m_flowmode(flowmode)
        {
            SetSink(sink);
            SetSource(SourceConnection.GetSource());
        }

        Pipeline(Plug& SourceConnection, Plug& SinkConnection, Element::FlowMode flowmode = Element::FlowMode::Pull) : m_flowmode(flowmode)
        {
            SetSink(SinkConnection.GetSink());
            SetSource(SourceConnection.GetSource());
            SinkConnection.Connect();
            SourceConnection.Connect();
        }

        Pipeline(const Plug& SourceConnection, const Plug& SinkConnection, Element::FlowMode flowmode = Element::FlowMode::Pull) : m_flowmode(flowmode)
        {
            SetSink(SinkConnection.GetSink());
            SetSource(SourceConnection.GetSource());
        }

        Pipeline(const Plug& SourceConnection, Sink& sink, Element::FlowMode flowmode = Element::FlowMode::Pull) : m_flowmode(flowmode)
        {
            SetSource(SourceConnection.GetSource());
            SetSink(&sink);            
        }

        Pipeline(const Plug& SourceConnection, const Sink& sink, Element::FlowMode flowmode = Element::FlowMode::Pull) : m_flowmode(flowmode)
        {
            SetSource(SourceConnection.GetSource());
            SetSink(sink);            
        }

        Pipeline(Source& source, const Plug& SinkConnection, Element::FlowMode flowmode = Element::FlowMode::Push) : m_flowmode(flowmode)
        {
            SetSource(&source);
            SetSink(SinkConnection.GetSink());
        }

        Pipeline(const Source& source, const Plug& SinkConnection, Element::FlowMode flowmode = Element::FlowMode::Push) : m_flowmode(flowmode)
        {
            SetSource(source);
            SetSink(SinkConnection.GetSink());
        }

        Pipeline& Connect()
        {
            if(m_sink && m_source)
            {
                auto sinkPlug = dynamic_cast<Plug*>(ActualSink()->GetPlug());
                auto sourcePlug = dynamic_cast<Plug*>(ActualSource()->GetPlug());
                if(sinkPlug)
                {
                    sinkPlug->Connect();
                }
                if(sourcePlug)
                {
                    sourcePlug->Connect();
                }
            }
            return *this;
        }

        bool IsConnected()  //assumes true if there is nothing setup to connect to, like a string source or parser sink.
        {
            bool bConnected = false;
            bool sinkConnected = true;  
            bool sourceConnected = true;
            if(m_sink && m_source)
            {
                auto sinkPlug = dynamic_cast<Plug*>(ActualSink()->GetPlug());
                auto sourcePlug = dynamic_cast<Plug*>(ActualSource()->GetPlug());
                if(sinkPlug)
                {
                    sinkConnected = sinkPlug->IsConnected();                    
                }
                if(sourcePlug)
                {
                    sourceConnected = sourcePlug->IsConnected();
                }
                bConnected = sinkConnected && sourceConnected;
            }
            return bConnected;
        }

        void SetSource(Element* source, Buffer* buffer)
        {
            if(buffer && source && source->IsSource())
            {
                source->SetBuffer(buffer);
            }
            SetSource(source);
        }

        void SetSource(const Source& source)
        {
            SetSource( &(const_cast<Source&>(source)));
        }

        void SetSource(Element* source)
        {
            if(source && source->IsSource())
            {
                m_source = source;
                m_source->SetParent(this);
                if(m_sink)
                {
                    m_source->SetSink(m_sink);
                    m_sink->SetSource(m_source);
                }
            }
        }

        void SetSink(Element* sink, Buffer* buffer)
        {
            if(buffer && sink && sink->IsSink())
            {
                sink->SetBuffer(buffer);
            }
            SetSink(sink);
        }

        void SetSink(const Sink& sink)
        {
            SetSink( &(const_cast<Sink&>(sink)));
        }

        void SetSink(Element* sink)
        {
            if(sink && sink->IsSink())
            {
                m_sink = sink;
                m_sink->SetParent(this);
                if(m_source)
                {
                    m_sink->SetSource(m_source);
                    m_source->SetSink(m_sink);
                }
            }
        }

        void SetSinkConnector(Element* plug)
        {
            if(plug && plug->IsPlug() && m_sink)
            {
                dynamic_cast<Sink*>(m_sink)->SetPlug(plug);
            }
        }

        void SetSourceConnector(Element* plug)
        {
            if(plug && plug->IsPlug() && m_source)
            {
                dynamic_cast<Source*>(m_source)->SetPlug(plug);
            }
        }

        inline Plug* GetSourceConnector()
        {
            return dynamic_cast<Plug*>(ActualSource()->GetPlug());
        }

        void SetConnector(Element* plug)
        {
            SetSourceConnector(plug);
            SetSinkConnector(plug);
        }

        inline Plug* GetSinkConnector()
        {
            return dynamic_cast<Plug*>(ActualSink()->GetPlug());
        }

        virtual Element::FlowMode GetFlowMode()
        {
            auto pipelineparent = dynamic_cast<Pipeline*>(GetParent());
            return (pipelineparent != nullptr) ? pipelineparent->GetFlowMode() : m_flowmode;
        }

        void SetFlowMode(Element::FlowMode flowmode)
        {
            auto pipelineparent = dynamic_cast<Pipeline*>(GetParent());
            if(pipelineparent != nullptr)
            {
                pipelineparent->SetFlowMode(flowmode);
            }
            else
            {
                m_flowmode = flowmode;
            }
        }

        virtual void Run(void);
        //bool Pump(void);

        virtual bool Pump(size_t& unitsPumped, size_t unitsToPump)
        {
            return Filter::Pump(unitsPumped, unitsToPump);
        }

        size_t PumpAll()
        {
            CheckComplete();
            size_t unitsPumped = 0;
            size_t unitsToPump = 0;
            do
            {
                unitsToPump = m_flowmode == FlowMode::Push ? m_source->GetBuffer()->WriteCapacity() : m_sink->GetBuffer()->WriteCapacity();
                log::debug("Capacity for {0} units.", unitsToPump);
                if(unitsToPump == 0)
                {
                    log::debug("Buffer full. Pipeline stalled.");
                }
            } while (unitsToPump > 0 && Filter::Pump(unitsPumped, unitsToPump));
            return unitsPumped;
        }

        bool PumpOne()
        {
            size_t unitsPumped = 0;
            size_t unitsToPump = 1;
            return Filter::Pump(unitsPumped, unitsToPump);
        }

        virtual Pipeline& InsertFilter(Filter* filter, FilterPos Pos = BeforeSink);
        virtual Pipeline& InsertInlineFilter(const Buffer& filter, FilterPos Pos = BeforeSink);

        virtual void ResetStream(size_t streamSize = 0)
        {
            ActualSink()->GetBuffer()->Reset(streamSize);
            ActualSource()->GetBuffer()->Reset(streamSize);
        }

        bool CheckComplete();
        
    protected:

        FlowMode m_flowmode;

    };

}}//qor::pipeline

#endif//QOR_PP_H_FRAMEWORK_PIPELINE
