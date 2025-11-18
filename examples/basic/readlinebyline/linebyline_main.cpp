#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/platform/platform.h"
#include "src/platform/filesystem/filesystem.h"
#include "src/framework/application/application_builder.h"
#include "src/framework/role/role.h"
#include "src/framework/thread/threadpool.h"

#include "linebylineworkflow.h"

using namespace qor;
using namespace qor::platform;
using namespace qor::framework;

#define appName "Line by line"

int main(const int argc, const char** argv, char**)
{	
    ThePlatform(qor_shared)->AddSubsystem<FileSystem>();
    return AppBuilder().Build<Application>(appName)->
        SetRole<Role>([](ref_of<IRole>::type role)
        {
            role->AddFeature<ThreadPool>();
        }).
        RunWorkflow<LineByLineWorkflow>();
}

qor_pp_implement_module(appName)
