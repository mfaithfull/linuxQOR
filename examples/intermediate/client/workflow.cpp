// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <iostream>

#include "workflow.h"
#include "src/qor/flyers/log/imperative.h"
#include "src/qor/flyers/log/impactful.h"
#include "src/components/protocols/echo/echoprotocol.h"

using namespace qor::components::protocols;

ClientWorkflow::ClientWorkflow() : 
    connect(new_ref<qor::workflow::State>(this)),
    send(new_ref<qor::workflow::State>(this)),
    receive(new_ref<qor::workflow::State>(this))
{    
    qor_pp_ofcontext;
        
    //Setup the protocol, buffers and std::iostream source and sink
    auto echoProtocol = new_ref<echo::EchoProtocol>();
    size_t maxEchoSize = echoProtocol->GetMaxEchoSize();
    m_requestBuffer.SetCapacity(maxEchoSize);
    m_responseBuffer.SetCapacity(maxEchoSize);        
    m_source.SetBuffer(m_requestBuffer);
    m_sink.SetBuffer(m_responseBuffer);

    //Wire up the standard Network client to use our provided source, sink and buffers with the Echo Protocol on local port 12345
    m_client.SetProtocol(echoProtocol);        
    m_client.SetSource(&m_source, m_requestBuffer);
    m_client.SetSink(&m_sink, m_responseBuffer);        
    m_client.Configure("localhost", 12345);

    connect->Enter = [this]()->void
    {
        qor_pp_ofcontext;
                
        if( m_client.Connect() )
        { 
            SetState(send); 
        }
        else
        {
            std::string err(strerror(errno));
            log::impact("Failed to connect to Echo service on port 12345 @ localhost: {0}", err);
            SetResult(EXIT_FAILURE);
            SetComplete();
        }
    };

    send->Enter = [this]()->void
    {        
        qor_pp_ofcontext;
        if(m_client.Send())
        {
            SetState(receive);
        }
        else
        {
            std::string err(strerror(errno));
            log::imperative("Failed to send to server: {0}",err);
            SetComplete(EXIT_FAILURE);         
        }
    };

    receive->Enter = [this]()->void
    {
        qor_pp_ofcontext;

        if(m_client.Receive())
        {
            log::debug("Echo received.");
            m_client.Disconnect();
            SetComplete(EXIT_SUCCESS);
        }
        else
        {
            std::string err(strerror(errno));
            log::imperative("Failed to receive from server: {0}", err);
            SetComplete(EXIT_FAILURE);            
        }
    };

    SetInitialState(connect);
}
