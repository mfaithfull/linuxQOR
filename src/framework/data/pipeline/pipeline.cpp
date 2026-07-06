// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/flyers/error/error.h"
#include "pipeline.h"

namespace qor{ namespace pipeline{

    Pipeline::Pipeline() : Filter() { }

    Pipeline::~Pipeline() = default;

    Pipeline::Pipeline(Connection* SourceConnection, Connection* SinkConnection, Element::FlowMode flowmode) : m_flowmode(flowmode)
    {
        SetSink(SinkConnection->GetSink());
        SetSource(SourceConnection->GetSource());
    }

    Pipeline::Pipeline(const Plug& SourceConnection, Sink* sink, Element::FlowMode flowmode) : m_flowmode(flowmode)
    {
        SetSink(sink);
        SetSource(SourceConnection.GetSource());
    }

    Pipeline::Pipeline(Plug& SourceConnection, Plug& SinkConnection, Element::FlowMode flowmode) : m_flowmode(flowmode)
    {
        SetSink(SinkConnection.GetSink());
        SetSource(SourceConnection.GetSource());
        SinkConnection.Connect();
        SourceConnection.Connect();
    }

    Pipeline::Pipeline(const Plug& SourceConnection, const Plug& SinkConnection, Element::FlowMode flowmode) : m_flowmode(flowmode)
    {
        SetSink(SinkConnection.GetSink());
        SetSource(SourceConnection.GetSource());
    }

    Pipeline::Pipeline(const Plug& SourceConnection, Sink& sink, Element::FlowMode flowmode) : m_flowmode(flowmode)
    {
        SetSource(SourceConnection.GetSource());
        SetSink(&sink);            
    }

    Pipeline::Pipeline(const Plug& SourceConnection, const Sink& sink, Element::FlowMode flowmode) : m_flowmode(flowmode)
    {
        SetSource(SourceConnection.GetSource());
        SetSink(sink);            
    }

    Pipeline::Pipeline(Source& source, const Plug& SinkConnection, Element::FlowMode flowmode) : m_flowmode(flowmode)
    {
        SetSource(&source);
        SetSink(SinkConnection.GetSink());
    }

    Pipeline::Pipeline(const Source& source, const Plug& SinkConnection, Element::FlowMode flowmode) : m_flowmode(flowmode)
    {
        SetSource(source);
        SetSink(SinkConnection.GetSink());
    }

    Pipeline& Pipeline::Connect()
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

    bool Pipeline::IsConnected()  //assumes true if there is nothing setup to connect to, like a string source or parser sink.
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

    void Pipeline::SetSource(Element* source, Buffer* buffer)
    {
        if(buffer && source && source->IsSource())
        {
            source->SetBuffer(buffer);
        }
        SetSource(source);
    }
    
    void Pipeline::SetSource(const Source& source)
    {
        SetSource( &(const_cast<Source&>(source)));
    }

    void Pipeline::SetSource(Element* source)
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

    void Pipeline::SetSink(Element* sink, Buffer* buffer)
    {
        if(buffer && sink && sink->IsSink())
        {
            sink->SetBuffer(buffer);
        }
        SetSink(sink);
    }

    void Pipeline::SetSink(const Sink& sink)
    {
        SetSink( &(const_cast<Sink&>(sink)));
    }

    void Pipeline::SetSink(Element* sink)
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

    void Pipeline::SetSinkConnector(Element* plug)
    {
        if(plug && plug->IsPlug() && m_sink)
        {
            dynamic_cast<Sink*>(m_sink)->SetPlug(plug);
        }
    }

    void Pipeline::SetSourceConnector(Element* plug)
    {
        if(plug && plug->IsPlug() && m_source)
        {
            dynamic_cast<Source*>(m_source)->SetPlug(plug);
        }
    }

    Plug* Pipeline::GetSourceConnector()
    {
        return dynamic_cast<Plug*>(ActualSource()->GetPlug());
    }

    void Pipeline::SetConnector(Element* plug)
    {
        SetSourceConnector(plug);
        SetSinkConnector(plug);
    }

    Plug* Pipeline::GetSinkConnector()
    {
        return dynamic_cast<Plug*>(ActualSink()->GetPlug());
    }

    Element::FlowMode Pipeline::GetFlowMode()
    {
        auto pipelineparent = dynamic_cast<Pipeline*>(GetParent());
        return (pipelineparent != nullptr) ? pipelineparent->GetFlowMode() : m_flowmode;
    }

    void Pipeline::SetFlowMode(Element::FlowMode flowmode)
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

    void Pipeline::Run(void)
    {
    }

    bool Pipeline::Pump(size_t& unitsPumped, size_t unitsToPump)
    {
        return Filter::Pump(unitsPumped, unitsToPump);
    }

    size_t Pipeline::PumpAll()
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

    bool Pipeline::PumpOne()
    {
        size_t unitsPumped = 0;
        size_t unitsToPump = 1;
        return Filter::Pump(unitsPumped, unitsToPump);
    }

    Pipeline& Pipeline::InsertFilter(Filter* filter, FilterPos Pos)
    {
        switch(Pos)
        {
        case FilterPos::AfterSource:
            {
                if(m_source)
                {
                    auto sink = m_source->GetSink();
                    m_source->SetSink(filter);
                    filter->SetSource(m_source);
                    if(sink)
                    {
                        filter->SetSink(sink);
                        sink->SetSource(filter);
                    }
                }
            }
            break;
        case FilterPos::BeforeSink:
            {
                if(m_sink)
                {
                    auto source = m_sink->GetSource();
                    filter->SetSink(m_sink);                                        
                    m_sink->SetSource(filter);
                    if(source)
                    {
                        filter->SetSource(source);
                        source->SetSink(filter);
                    }
                }
            }
            break;
        }
        return *this;
    }

    Pipeline& Pipeline::InsertInlineFilter(const Buffer& filter, FilterPos Pos)
    {
        switch(Pos)
        {
        case FilterPos::AfterSource:
            {
                if(m_source)
                {
                    m_source->SetBuffer(filter);
                    auto sink = m_source->GetSink();
                    if(sink)
                    {
                        sink->SetBuffer(filter);
                    }
                }
            }
            break;
        case FilterPos::BeforeSink:
            {
                if(m_sink)
                {
                    m_sink->SetBuffer(filter);
                    auto source = m_sink->GetSource();
                    if(source)
                    {
                        source->SetBuffer(filter);
                    }
                }
            }
            break;
        }
        return *this;
    }

    void Pipeline::ResetStream(size_t streamSize)
    {
        ActualSink()->GetBuffer()->Reset(streamSize);
        ActualSource()->GetBuffer()->Reset(streamSize);
    }

    bool Pipeline::CheckComplete()
    {
        bool bComplete = false;        
        if(HasSource() && HasSink())
        {
            if(m_flowmode == FlowMode::Push)
            {
                bComplete = dynamic_cast<Source*>(m_source)->CheckComplete();
            }
            else
            {
                bComplete = dynamic_cast<Sink*>(m_sink)->CheckComplete();
            }
        }
        else
        {
            if(!HasSource())
            {
                warning("Pipeline source is not set.");
            }
            else
            {
                warning("Pipeline sink is not set.");
            }
        }
        return bComplete;
    }
}}//qor::pipeline

