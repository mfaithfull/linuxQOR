#include "src/configuration/configuration.h"
#include "src/framework/application/application_builder.h"
#include "src/framework/role/role.h"
#include "src/framework/workflow/workflow.h"
#include "src/platform/network/sockets.h"
#include "src/framework/thread/threadpool.h"

using namespace qor;
using namespace qor::framework;
using namespace qor::workflow;
using namespace qor::network;

qor_pp_implement_module("Web Server")

void SetupRole(ref_of<IRole>::type role);

int main()
{	
    ThePlatform(qor_shared)->AddSubsystem<Sockets>();
    auto application = AppBuilder().Build("QOR HTTP Server");
    application->SetRole<Role>( &SetupRole );
    return application->RunWorkflow<Workflow>();
}

void SetupRole(ref_of<IRole>::type role)
{
    role->template AddFeature<ThreadPool>();
}

