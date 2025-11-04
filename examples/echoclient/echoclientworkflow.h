#ifndef QOR_PP_H_EXAMPLES_ECHOCLIENT_WORKFLOW
#define QOR_PP_H_EXAMPLES_ECHOCLIENT_WORKFLOW

#include "src/framework/workflow/workflow.h"
#include "src/platform/network/sockets.h"
#include "src/platform/network/socket.h"
#include "src/components/framework/console/console.h"
#include "src/framework/pipeline/podbuffer.h"

class EchoClientWorkflow : public qor::workflow::Workflow
{
public:

    EchoClientWorkflow();
    virtual ~EchoClientWorkflow() = default;

private:

    qor::workflow::State setup;
    qor::workflow::State connect;
    qor::workflow::State input;    
    qor::workflow::State send;
    qor::workflow::State receive;
    qor::workflow::State output;

    qor::ref_of<qor::components::Console>::type m_Console;
    std::string m_Input;
    qor::ref_of<qor::network::Socket>::type m_ClientSocket;
    qor::network::Address m_ServerAddress;
    qor::pipeline::ByteBuffer m_ResponseBuffer;
    std::string m_Output;
};

#endif//QOR_PP_H_EXAMPLES_ECHOCLIENT_WORKFLOW