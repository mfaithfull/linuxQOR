// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/informative.h"
#include "sink.h"
#include "source.h"
#include "plug.h"

namespace qor{ namespace pipeline{

    qor_pp_signal_func Sink::WriteSuccessSignal(size_t unitsWritten)
    {
        qor_pp_signal(WriteSuccessSignal, unitsWritten);
    }

    qor_pp_signal_func Sink::WriteErrorSignal(size_t unitsWritten, size_t error)
    {
        qor_pp_signal(WriteErrorSignal, unitsWritten, error);
    }

    Sink::Sink() : m_source(nullptr) { }

    Sink::~Sink() = default;

    Sink::Sink( const Sink& src) : Element(src), SignalBase()
    {
        *this = src;
    }

    Sink& Sink::operator = (const Sink& src)
    {
        if(&src != this)
        {
            Element::operator=(src);
            m_source = src.m_source;
        }
        return *this;
    }

    void Sink::SetSink(Element* /*sink*/)
    {
        //Can't set a sink on a sink
    }

    void Sink::SetSource(Element* source)
    {
        if( source->IsSource() )
        {
            m_source = source;
        }
    }

    Element* Sink::GetSource()
    {
        return m_source;
    }

    bool Sink::HasSource()
    {
        return m_source != nullptr;
    }

    bool Sink::Write(size_t& unitsWritten, size_t unitsToWrite)
    {
        return (unitsToWrite == 0 || Pull(unitsWritten, unitsToWrite)) ? Push(unitsWritten, unitsWritten) : false;
    }

    void Sink::OnWriteSuccess(size_t unitsWritten)
    {
        WriteSuccessSignal(unitsWritten);
    }

    void Sink::OnWriteError(size_t error, size_t unitsWritten)
    {
        WriteErrorSignal(unitsWritten, error);
    }

    bool Sink::IsSink()
    {
        return true;
    }

    Source* Sink::ActualSource()
    {
        return dynamic_cast<Source*>(m_source);
    }

    void Sink::SetPlug(Element* /*plug*/){ }

    Element* Sink::GetPlug() const
    {
        return nullptr;
    }

    const char* Sink::Name() const
    {
        return "Sink";
    }

    bool Sink::CheckComplete()
    {
        auto name = Name();
        log::inform("|{0}", name);
        if(GetFlowMode() == Element::FlowMode::Push)
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
                //warning("{0} is in push mode but has no plug or plug is not a valid plug.", name);
            }
        }
        else
        {
            if(GetSource() && GetSource()->IsSource())
            {
                auto actualSource = dynamic_cast<Source*>(GetSource());
                return actualSource->CheckComplete();
            }
            else
            {
                warning("{0} is in pull mode but has no source or source is not a valid source.", name);
            }
        }
        return false;
    }

    //pull the requested amount of data from the source to the buffer
    bool Sink::Pull(size_t& unitsWritten, size_t unitsToWrite)
    {
        return GetFlowMode() == FlowMode::Pull ?
        (ActualSource()->Read(unitsWritten, unitsToWrite) && (unitsWritten > 0 || unitsToWrite == 0) ? true : false) : true;
    }

    bool Sink::Push(size_t& unitsWritten, size_t unitsToWrite)
    {
        if(unitsToWrite == 0)
        {
            return true;
        }
        pipeline::Buffer* buffer = GetBuffer();
        if(buffer)
        {
            byte* data = buffer->ReadRequest(unitsToWrite);
            if(!data)
            {
                continuable("Pipeline stall. No space in sink buffer.");
                return false;
            }

            size_t bytesWritten = WriteBytes(data, buffer->GetUnitSize() * unitsToWrite);
            if(bytesWritten > 0)
            {
                unitsWritten = bytesWritten / buffer->GetUnitSize();
                buffer->ReadAcknowledge(unitsWritten);
                OnWriteSuccess(unitsWritten);
                return true;
            }
            else
            {
                return false;//Nothing was written, the sink should have raised an error if that's a problem
            }
        }
        else
        {
            return false;//TODO: Raise no source buffer error
        }
    }

    size_t Sink::WriteBytes(byte* /*data*/, size_t /*bytesToWrite*/)
    {
        fatal("Empty base called. Please overrride bool WriteBytes(byte*, size_t); in your pipeline::Sink derived class.");
        return 0;
    }

    NullSink::NullSink() : Sink() {}
    NullSink::~NullSink() = default;

    NullSink& NullSink::operator = (const NullSink& /*src*/)
    {
        return *this;
    }

    bool NullSink::Write(size_t& unitsWritten, size_t unitsToWrite)
    {
        GetBuffer()->ReadRequest(unitsToWrite);
        unitsWritten = unitsToWrite;
        GetBuffer()->ReadAcknowledge(unitsWritten);
        return true;
    }

}}//qor::pipeline
