#include "src/configuration/configuration.h"
#include "echoclientworkflow.h"
#include "src/platform/platform.h"
#include "src/components/framework/console/console.h"

using namespace qor;
using namespace qor::workflow;
using namespace qor::components;

qor_pp_module_requires(IConsole)

EchoClientWorkflow::EchoClientWorkflow() : 
    setup(this),
    input(this),
    connect(this),
    send(this),
    receive(this),
    output(this)
{    
    setup.Enter = [this]()->void
    {
        m_Console = new_ref<Console>();
        m_ResponseBuffer.SetCapacity(4096);
        SetState(input);
    };

    input.Enter = [this]()->void
    {            
        m_Input = m_Console->ReadLine();
        if(m_Input == "quit")
        {
            SetResult(EXIT_SUCCESS);
            SetComplete();
        } 
        else if(!m_Input.empty())
        {
            PushState(connect);
        }
    };

    connect.Enter = [this]()->void
    {
        auto sockets_subsystem = ThePlatform()->GetSubsystem<network::Sockets>().AsRef<network::Sockets>();

        m_ClientSocket = sockets_subsystem->CreateSocket(
            network::sockets::eAddressFamily::AF_INet, 
            network::sockets::eType::Sock_Stream, 
            network::sockets::eProtocol::IPProto_IP);        

        m_ServerAddress.sa_family = network::sockets::eAddressFamily::AF_INet;
        m_ServerAddress.SetPort(12345);
        m_ServerAddress.SetIPV4Address(127,0,0,1);

        if(m_ClientSocket->Connect(m_ServerAddress) == 0)
        {
            SetState(send);
        }
        else
        {
            SetResult(EXIT_FAILURE);
            SetComplete();
        }
    };

    send.Enter = [this]()->void
    {
        for(size_t o{}, w(1); o != m_Input.length() && 0 < w; o += w)
        {
            w = m_ClientSocket->Send(m_Input.c_str() + o, m_Input.length() - o);
        }
        SetState(receive);
    };

    receive.Enter = [this]()->void
    {
        size_t requestSize = 0;
        byte* buffer = m_ResponseBuffer.WriteRequest(requestSize);
        size_t n = m_ClientSocket->Receive((char*)buffer, requestSize, 0);
        if(n > 0 && n <= requestSize)
        {
            m_ResponseBuffer.WriteAcknowledge(n);
            SetState(output);
        }
        else
        {
            n = 0;
            m_ResponseBuffer.WriteAcknowledge(n);
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

    SetInitialState(setup);
}
