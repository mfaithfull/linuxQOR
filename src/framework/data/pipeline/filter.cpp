// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "filter.h"
#include "src/qor/flyers/log/debug.h"
#include "src/qor/flyers/interception/functioncontext.h"

namespace qor{ namespace pipeline{

    Filter::Filter() : Sink(), Source() {}

    Filter::Filter(const Filter& src) : Sink(), Source(), Element()
    {
        *this = src;
    }

    Filter::~Filter() = default;

    Filter& Filter::operator = (const Filter& src)
    {
        if(&src != this)
        {
            Source::operator = (src);
            Sink::operator = (src);
        }
        return *this;
    }

    void Filter::SetSource(Element* source)
    {
        if(source->IsSource())
        {
            m_source = source;
            if(!m_source->HasSink())
            {
                m_source->SetSink(this);
            }
        }
    }

    Element* Filter::GetSource()
    {
        return m_source;
    }

    bool Filter::HasSource()
    {
        return m_source != nullptr;
    }

    void Filter::SetSink(Element* sink)
    {
        if(sink->IsSink())
        {
            m_sink = sink;
            if(!m_sink->HasSource())
            {
                m_sink->SetSource(this);
            }
        }
    }

    Element* Filter::GetSink()
    {
        return m_sink;
    }

    bool Filter::HasSink()
    {
        return m_sink != nullptr;
    }

    bool Filter::IsSource()
    {
        return true;
    }

    bool Filter::IsSink()
    {
        return true;
    }

    //Pump up to the buffer capacity. In Pull mode the Sink owns the relevant buffer as it's pulling. In Push mode it's the Source.
    size_t Filter::PumpSome()
    {
        size_t unitsPumped = 0;
        size_t unitsToPump = GetFlowMode() == FlowMode::Pull ?
        ActualSink()->GetBuffer()->WriteCapacity() : 
        ActualSource()->GetBuffer()->WriteCapacity();
        PumpSome(unitsPumped, unitsToPump);
        return unitsPumped;
    }

    //Pump up to the ammount requested. 
    //In Pull mode this is a request to write that much into the sink. It will pull what it needs to achieve that
    //In Push mode this is a request to push that much from the source. What arrives at the sink will depend on any filters present
    bool Filter::PumpSome(size_t& unitsPumped, size_t unitsToPump)
    {    
        if(GetFlowMode() == FlowMode::Pull)
        {
            return ActualSink()->Write(unitsPumped, unitsToPump);
        }
        else
        {
            return ActualSource()->Read(unitsPumped, unitsToPump);
        }
    }

    //Keep pumping until the requests units have been pumped or the well is empty 
    //returns true if we got all the units we asked for without breaking the pipe
    bool Filter::Pump(size_t& unitsPumped, size_t unitsToPump)
    {    
        qor_pp_ofcontext;
        bool working = unitsToPump > 0 ? true : false;
        while(working && unitsToPump > 0)
        {
            size_t unitsPumpedAtOnce = 0;
            if(GetFlowMode() == FlowMode::Pull)
            {
                //log::debug("Pulling for {0} units", unitsToPump);
                working = ActualSink()->Write(unitsPumpedAtOnce, unitsToPump);
                if(unitsPumpedAtOnce == 0)
                {
                    log::debug("Nothing pulled");
                    if(working)
                    {
                        log::debug("Will retry.");
                    }
                    break;
                }
                //log::debug("Pulled for {0} units", unitsPumpedAtOnce);
            }
            else
            {
                //log::debug("Pushing up to {0} units", unitsToPump);
                working = ActualSource()->Read(unitsPumpedAtOnce, unitsToPump);
                if(unitsPumpedAtOnce == 0)
                {
                    log::debug("Nothing pushed.");
                    if(working)
                    {
                        log::debug("Will retry.");
                    }
                    break;
                }
                //log::debug("Pushed {0} units", unitsPumpedAtOnce);
            }
            unitsToPump -= unitsPumpedAtOnce;
            unitsPumped += unitsPumpedAtOnce;
        };
        //log::debug("Pipeline is {0}.", working ? "active" : "deactivated");
        return working;
    }

    bool Filter::Read(size_t& unitsRead, size_t unitsToRead)
    {
        return (GetFlowMode() == FlowMode::Pull) ? 
        (ReadFilter(unitsRead, unitsRead)) : ActualSource()->Read(unitsRead, unitsToRead);
/*
        return ActualSource()->Read(unitsRead, unitsToRead) ? 
        (
            (GetFlowMode() == FlowMode::Pull) ? 
                (ReadFilter(unitsRead, unitsRead)): true
        ) : false;
*/
    }

    bool Filter::Write(size_t& unitsWritten, size_t unitsToWrite)
    {
        return Pull(unitsWritten, unitsToWrite) ? 
        (
            WriteFilter(unitsWritten, unitsWritten) ? 
                ActualSink()->Write(unitsWritten, unitsToWrite) : false
        ) : false;

    }

    bool Filter::Pull(size_t& unitsRead, size_t unitsToRead)
    {
        return (GetFlowMode() == FlowMode::Pull) ? ActualSource()->Read(unitsRead, unitsToRead) : true;
    }

    bool Filter::Push(size_t& unitsWritten, size_t unitsToWrite)
    {
        return (GetFlowMode() == FlowMode::Push) ? ActualSink()->Write(unitsWritten, unitsToWrite) : true;
    }

    bool Filter::Pump(size_t& unitsPumped, size_t unitsToPump, std::function<bool(size_t&, size_t)> filterFn)
    {
        if(GetFlowMode() == FlowMode::Pull)
        {
            return Pull(unitsPumped, unitsToPump) ? 
            (
                filterFn(unitsPumped, unitsPumped) ? 
                    Push(unitsPumped,unitsPumped) : false
            ) : false;
        }
        else
        {
            return filterFn(unitsPumped, unitsPumped) ? 
            (
                Push(unitsPumped, unitsToPump) ? 
                    Pull(unitsPumped, unitsToPump) : false
            ) : false;
        }
    }

    bool Filter::ReadFilter(size_t& unitsProcessed, size_t unitsToProcess)
    {
        unitsProcessed = unitsToProcess;
        return true;
    }

    bool Filter::WriteFilter(size_t& unitsProcessed, size_t unitsToProcess)
    {
        unitsProcessed = unitsToProcess;
        return true;
    }

    const char* Filter::Name() const
    {
        return "Filter";
    }

}}//qor::pipeline
