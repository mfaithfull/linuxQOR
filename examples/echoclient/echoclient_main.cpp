#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/platform/platform.h"
#include "src/platform/network/sockets.h"
#include "src/framework/pipeline/pipeline.h"
#include "src/framework/application/application_builder.h"

#include "echoclientworkflow.h"

using namespace qor;
using namespace qor::platform;
using namespace qor::framework;
using namespace qor::pipeline;
using namespace qor::network;

#define appName "Echo Client"
qor_pp_implement_module(appName)
qor_pp_module_requires(Sockets)

int main(const int argc, const char** argv, char**)
{
    ThePlatform()->AddSubsystem<Sockets>();

    return AppBuilder().Build(appName)->
        SetRole().
        RunWorkflow<EchoClientWorkflow>();
}
