// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/informative.h"
#include "source.h"
#include "sink.h"
#include "plug.h"

namespace qor{ namespace pipeline{


    qor_pp_signal_func Source::ReadSuccessSignal(size_t unitsRead)
    {
        qor_pp_signal(ReadSuccessSignal, unitsRead);
    }

    qor_pp_signal_func Source::EndOfDataSignal(void)
    {
        qor_pp_signal(EndOfDataSignal);
    }

    qor_pp_signal_func Source::ReadErrorSignal(size_t unitsRead, size_t error)
    {
        qor_pp_signal(ReadErrorSignal, unitsRead, error);
    }

    Source::Source() : m_sink(nullptr) {}
    
    Source::~Source() = default;

    Source::Source(const Source& src) : Element(src)
    {
        *this = src;
    }

    Source& Source::operator = (const Source& src)
    {
        if(&src != this)
        {
            Element::operator = (src);
            m_sink = src.m_sink;
        }
        return *this;
    }

    void Source::SetSink(Element* sink)
    {
        m_sink = sink;
    }

    void Source::SetSource(Element* /*source*/)
    {
        //a Source can't have a source. Do nothing
    }

    Element* Source::GetSink()
    {
        return m_sink;
    }

    bool Source::HasSink()
    {
        return m_sink != nullptr;
    }

    bool Source::Read(size_t& unitsRead, size_t unitsToRead)
    {
        return Pull(unitsRead, unitsToRead) ? Push(unitsRead, unitsRead) : false;
    }

    void Source::OnReadSuccess(size_t unitsRead)
    {
        ReadSuccessSignal(unitsRead);
    }
    
    void Source::OnReadError(size_t error, size_t unitsRead)
    {
        ReadErrorSignal(unitsRead, error);
    }

    void Source::OnEndOfData()
    {
        EndOfDataSignal();
    }
    
    bool Source::IsSource()
    {
        return true;
    }

    Sink* Source::ActualSink()
    {
        return dynamic_cast<Sink*>(m_sink);
    }

    void Source::SetPlug(Element* /*plug*/){}

    Element* Source::GetPlug() const
    {
        return nullptr;
    }

    const char* Source::Name() const
    {
        return "Source";
    }

    bool Source::CheckComplete()
    {
        auto name = Name();
        log::inform("|{0}", name);
        if(GetFlowMode() == Element::FlowMode::Push)
        {
            if(m_sink && m_sink->IsSink())
            {
                auto actualSink = dynamic_cast<Sink*>(m_sink);
                return actualSink->CheckComplete();
            }
            else
            {
                warning("{0} is in push mode but has no sink or sink is not a valid sink.", name);
            }
        }
        else
        {
            if(!GetBuffer())
            {
                warning("{0} has no buffer.", name);
                return false;
            }
            if(GetPlug() && GetPlug()->IsPlug())
            {                
                return dynamic_cast<Plug*>(GetPlug())->CheckComplete();
            }
            else
            {
                warning("{0} is in pull mode but has no plug or plug is not a valid plug.", name);
            }
        }
        return false;
    }

    bool Source::Push(size_t& unitsRead, size_t unitsToRead)
    {
        return ( GetFlowMode() == FlowMode::Push ) ? (ActualSink()->Write(unitsRead, unitsToRead) && (unitsRead > 0)) : true;        
    }

    bool Source::Pull(size_t& unitsRead, size_t unitsToRead)
    {
        if(!unitsToRead)
        {
            return true;
        }
        Buffer* buffer = GetBuffer();
        if(buffer)
        {
            byte* space = buffer->WriteRequest(unitsToRead);            
            if(!space)
            {
                continuable("Pipeline stall. No space in source buffer.");
                return false;
            }
            
            size_t bytesRead = ReadBytes(space, buffer->GetUnitSize() * unitsToRead);
            if(bytesRead > 0)
            {
                unitsRead = bytesRead / buffer->GetUnitSize();
                buffer->WriteAcknowledge(unitsRead);
                OnReadSuccess(unitsRead);
            }
            else
            {
                OnEndOfData();
            }
            return true;
        }
        else
        {
            return false;
        }
    }

    size_t Source::ReadBytes(byte* space, size_t bytesToRead)
    {
        fatal("Empty base called. Please override bool ReadBytes(byte*, size_t); in your pipeline::Source derived class.");
        return 0;
    }


    NullSource::NullSource() : Source() {}

    NullSource::~NullSource() = default;

    bool NullSource::Read(size_t& unitsRead, size_t unitsToRead)
    {        
        unitsRead = unitsToRead;        
        return true;
    }


}}//qor::pipeline
