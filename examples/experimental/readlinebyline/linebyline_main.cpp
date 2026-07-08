// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/module/module.h"
#include "src/platform/platform.h"
#include "src/framework/io/filesystem/filesystem.h"
#include "src/framework/app/application/builder.h"
#include "src/framework/app/role/role.h"
#include "src/framework/parallel/thread/threadpool.h"

#include "linebylineworkflow.h"

using namespace qor;
using namespace qor::app;
using namespace qor::io;
using namespace qor::thread;
using namespace qor::platform;

#define appName "Line by line"

qor_pp_module_requires(ICurrentThread)
qor_pp_implement_module(appName)

int main(const int argc, const char** argv, char**)
{	
    ThePlatform(qor_shared)->AddSubsystem<FileSystem>();
    return AppBuilder().Build<Application>(appName)->SetRole<Role>(
        [](ref_of<IRole>::type role)
        {
            role->AddFeature<ThreadPool>();
        }).RunWorkflow<LineByLineWorkflow>();
}
