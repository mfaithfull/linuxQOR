#include "src/configuration/configuration.h"

#include <stdio.h>

#include "src/qor/module/module.h"
#include "src/platform/platform.h"
#include "src/framework/app/application/builder.h"
#include "src/framework/app/role/role.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/framework/parallel/thread/threadpool.h"
#include "src/framework/io/network/sockets.h"
#include "src/framework/app/workflow/workflow.h"
#include "src/components/network/clients/http/httpsocketclient.h"

using namespace qor;
using namespace qor::platform;
using namespace qor::framework;
using namespace qor::io::network;
using namespace qor::workflow;
using namespace qor::components::network;

#define appName "Test Client"
qor_pp_module_requires(Sockets)

int main(const int argc, const char** argv, char**)
{
    ThePlatform(qor_shared)->AddSubsystem<Sockets>();
    return AppBuilder().Build(appName)->
    SetRole<Role>([](ref_of<IRole>::type role)
    {
        role->AddFeature<ThreadPool>();
    }).
    Run(make_runable([]()->int
    {
        HTTPSocketClient cli("192.168.1.1", 8006);

        if (auto res = cli.Get("/hi"))
        {
            std::cout << res->status << std::endl;
            std::cout << res->get_header_value("Content-Type") << std::endl;
            std::cout << res->body << std::endl;
        }
        else
        {
            std::cout << "error code: " << res.error() << std::endl;
        }

       return 0;
    })
    );
}

qor_pp_implement_module(appName)
