// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/flyers/error/error.h"
#include "src/qor/flyers/log/informative.h"
#include "plug.h"
#include "source.h"
#include "sink.h"

namespace qor{ namespace pipeline{

    qor_pp_signal_func Plug::ConnectedSignal()
    {
        qor_pp_signal(ConnectedSignal);
    }

    qor_pp_signal_func Plug::ConnectionErrorSignal()
    {
        qor_pp_signal(ConnectionErrorSignal);
    }

    qor_pp_signal_func Plug::DisconnectedSignal()
    {
        qor_pp_signal(DisconnectedSignal);
    }

    qor_pp_signal_func Plug::DisconnectionErrorSignal()
    {
        qor_pp_signal(DisconnectionErrorSignal);
    }

    Plug::Plug() : m_syncobject(nullptr)
    {
        m_connected = false;
    }

    Plug::~Plug()
    {
        if (m_connected)
        {
            Disconnect();
        }
        m_connected = false;
    }

    void Plug::SetSink(Element* /*ignored*/){}

    void Plug::SetSource(Element* /*ignored*/){}

    Element* Plug::GetSink() const { return nullptr; }

    Element* Plug::GetSource() const { return nullptr; }

    bool Plug::IsPlug() { return true; }
    
    bool Plug::Connect()
    {
        return false;
    }

    void Plug::Disconnect()
    {
    }

    /*
    bool Plug::HandlePendingConnectionResult(bool connected)
    {
        return connected;
    }*/

    void Plug::OnConnected()
    {
        m_connected = true;
        ConnectedSignal();
    }
    
    void Plug::OnConnectionError()
    {
        m_connected = false;
        ConnectionErrorSignal();
    }

    void Plug::OnDisconnected()
    {
        m_connected = false;
        DisconnectedSignal();
    }

    void Plug::OnDisconnectionError()
    {
        m_connected = false;
        DisconnectionErrorSignal();
    }

    bool Plug::IsConnected() const
    {
        return m_connected;
    }

    bool Plug::AsyncConnection() const
    {
        return false;
    }

    SyncObject* Plug::GetSyncObject()
    {
        return m_syncobject;
    }

    void Plug::SetSyncObject(SyncObject* syncobject)
    {
        m_syncobject = syncobject;
    }

    const char* Plug::Name() const
    {
        return "Plug";
    }

    bool Plug::CheckComplete()
    {
        auto name = Name();
        log::inform("T{0}", name);
        if(GetFlowMode() == Element::FlowMode::Push)
        {
            if(GetSink() && GetSink()->IsSink())
            {
                auto actualSink = dynamic_cast<Sink*>(GetSink());
                return actualSink->CheckComplete();
            }
            else
            {
                warning("Plug is in push mode but has no sink or sink is not a valid sink.");
            }
        }
        else
        {
            return true;
        }
        return false;
    }

}}//qor::pipeline