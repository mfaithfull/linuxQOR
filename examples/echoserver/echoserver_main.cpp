#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/platform/platform.h"
#include "src/platform/filesystem/filesystem.h"
#include "src/framework/application/application_builder.h"
#include "src/framework/role/role.h"
#include "src/framework/thread/threadpool.h"
#include "src/components/framework/optionparser/getter.h"
#include "src/framework/pipeline/pipeline.h"
#include "src/platform/network/sockets.h"

#include "echoserverapp.h"
#include "echoserverworkflow.h"

using namespace qor;
using namespace qor::platform;
using namespace qor::framework;
using namespace qor::components::optparser;
using namespace qor::pipeline;
using namespace qor::network;

#define appName "Echo Server"

int main(const int argc, const char** argv, char**)
{	
    ThePlatform()->AddSubsystem<Sockets>();             //What subsystems it needs

    return AppBuilder().Build<EchoServerApp>(appName,   //What it is
        [](ref_of<EchoServerApp>::type app, const int argc, const char** argv, const char** env)
        {
            OptionGetter options(argc, argv, app());
        }
    )->SetRole<Role>(
        [](ref_of<IRole>::type role)                    //What features it has
        {
            role->AddFeature<ThreadPool>();
        }
    ).RunWorkflow<EchoServerWorkflow>();                //What it does
}

qor_pp_implement_module(appName)
