#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/platform/platform.h"
#include "src/platform/filesystem/filesystem.h"
#include "src/framework/application/application_builder.h"
#include "src/framework/role/role.h"
#include "src/framework/thread/threadpool.h"
#include "src/components/framework/optionparser/getter.h"

#include "fileprocessorapp.h"
#include "fileprocessorworkflow.h"

using namespace qor;
using namespace qor::platform;
using namespace qor::framework;
using namespace qor::components::optparser;

#define appName "File Processor"

int main(const int argc, const char** argv, char**)
{	
    ThePlatform()->AddSubsystem<FileSystem>();
    return AppBuilder().Build<FileProcessorApp>(appName, 
        [](ref_of<FileProcessorApp>::type app, const int argc, const char** argv, const char** env)
        {
            OptionGetter options(argc, argv, app());
        })->
        SetRole<Role>([](ref_of<IRole>::type role)
        {
            role->AddFeature<ThreadPool>();
        }).
        RunWorkflow<FileProcessorWorkflow>();
}

qor_pp_implement_module(appName)
