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
#include <sys/resource.h>

using namespace qor;
using namespace qor::platform;
using namespace qor::framework;
using namespace qor::components::optparser;

#define appName "File Processor"

int main(const int argc, const char** argv, char**)
{	
    const rlim_t kStackSize = 32 * 1024 * 1024;   // min stack size = 32 MB
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }

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
