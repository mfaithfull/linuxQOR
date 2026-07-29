// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "src/platform/platform.h"
#include "pipeconnector.h"
#include "pipesink.h"
#include "pipesource.h"

namespace qor{ namespace io { namespace components{

    PipeConnector::PipeConnector() : Plug()
    {
        m_name = "";
        m_sink = new_ref<PipeSink>();
        m_source = new_ref<PipeSource>();
        m_sink->SetPlug(this);
        m_source->SetPlug(this);
    }

    PipeConnector::~PipeConnector() noexcept
    {
        if (m_connected)
        {
            Disconnect();
        }
    }

    qor::pipeline::Element* PipeConnector::GetSink() const
    {
        return m_sink;
    }

    qor::pipeline::Element* PipeConnector::GetSource() const
    {
        return m_source;
    }

    bool PipeConnector::Connect()
    {
        if(!m_connected)
        {
            m_connected = ConnectToName(m_name);
        }
        return m_connected;
    }

    bool PipeConnector::ConnectToName(const std::string /*name*/)
    {
        return false;
    }

    void PipeConnector::Disconnect()
    {
        //m_pipe->
        m_connected = false;
    }

    ref_of<io::Pipe>::type PipeConnector::Pipe(void)
    {
        return m_pipe;
    }

}}}//qor::io::components
