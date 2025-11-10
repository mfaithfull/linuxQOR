#include <iostream>

#include "src/configuration/configuration.h"
#include "echoclientworkflow.h"
#include "src/platform/platform.h"
#include "src/components/framework/console/console.h"

using namespace qor;
using namespace qor::workflow;
using namespace qor::components;
using namespace qor::network;
using namespace qor::network::sockets;

qor_pp_module_requires(IConsole)

EchoClientWorkflow::EchoClientWorkflow() : 
    setup(this),
    connect(this),
    input(this),    
    send(this),
    receive(this),
    output(this)
{    
    setup.Enter = [this]()->void
    {
        m_Console = new_ref<Console>();
        m_ResponseBuffer.SetCapacity(1024);
        SetState(&connect);
    };

    connect.Enter = [this]()->void
    {
        auto sockets_subsystem = ThePlatform()->GetSubsystem<Sockets>();

        m_Console->WriteLine("Creating socket\n");
        m_ClientSocket = sockets_subsystem->CreateSocket(
            eAddressFamily::AF_INet, 
            eType::Sock_Stream, 
            eProtocol::IPProto_IP);        

        m_ServerAddress.sa_family = eAddressFamily::AF_INet;
        m_ServerAddress.SetPort(12345);
        m_ServerAddress.SetIPV4Address(127,0,0,1);

        m_Console->WriteLine("Connecting to server @ 127.0.0.1:12345\n");
        if(m_ClientSocket->Connect(m_ServerAddress) == 0)
        {
            SetState(&input);
        }
        else
        {
            std::cout << "failed to connect to server: " << strerror(errno) << "\n";

            SetResult(EXIT_FAILURE);
            SetComplete();
        }
    };

    input.Enter = [this]()->void
    {            
        m_Console->WriteLine("\nReady:");
        m_Input = m_Console->ReadLine();
        if(!m_Input.empty())
        {
            PushState(&send);
        }
    };

    input.Resume = [this]()->void
    {
        if(m_Input == "close")
        {
            m_Console->WriteLine("Shutting down\n");
            m_ClientSocket->Shutdown(ShutdownReadWrite);
            SetResult(EXIT_SUCCESS);
            SetComplete();
            PopState();
        } 
    };

    send.Enter = [this]()->void
    {        
        for(size_t o{}, w(1); o != m_Input.length() && 0 < w; o += w)
        {
            m_Console->WriteLine("Sending to server\n");
            w = m_ClientSocket->Send(m_Input.c_str() + o, m_Input.length() - o);

            if(w < 0)
            {
                std::cout << "Failed to send to server: " << strerror(errno) << "\n";
                SetResult(EXIT_FAILURE);
                SetComplete();
                break;
            }
        }
        SetState(&receive);
    };

    receive.Enter = [this]()->void
    {
        size_t requestSize = 0;
        byte* buffer = m_ResponseBuffer.WriteRequest(requestSize);
        m_Console->WriteLine("Waiting for reply\n");
        size_t n = m_ClientSocket->Receive((char*)buffer, requestSize, 0);
        if(n > 0 && n <= requestSize)
        {
            m_ResponseBuffer.WriteAcknowledge(n);
            SetState(&output);
        }
        else
        {
            n = 0;
            m_ResponseBuffer.WriteAcknowledge(n);
            std::cout << "failed to receive from server: " << strerror(errno) << "\n";
            SetResult(EXIT_FAILURE);
            SetComplete();
        }
    };

    output.Enter = [this]()->void
    {
        size_t requestSize = m_ResponseBuffer.ReadCapacity();
        byte* buffer = m_ResponseBuffer.ReadRequest(requestSize);
        std::string output((const char*)(buffer), requestSize);
        m_ResponseBuffer.ReadAcknowledge(requestSize);
        m_Console->WriteLine(output);        
        PopState();
    };

    SetInitialState(&setup);
}
