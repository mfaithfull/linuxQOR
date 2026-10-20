// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/tdd/test/test.h"
#include "src/qor/module/module.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/framework/io/filesystem/filesystem.h"
#include "src/platform/platform.h"
#include "sdk/app.h"

qor_pp_module_requires(ICurrentThread)
qor_pp_module_requires(IFileSystem)

using namespace qor;
using namespace qor::app;
using namespace qor::platform;
using namespace qor::io;

int main(int argc, const char** argv, char** /*env*/)
{
    //The file connector sink at the end of the JSON pipeline resolves its FileIndex
    //through the file system subsystem. That subsystem is only available once the
    //platform has it registered, and the platform starts without any subsystem, so
    //the file system is added before the application (and the platform setup that
    //goes with it) is built. Without this the module requirements for FileSystem
    //and IFile fail as soon as a pipeline is hooked to a file.
    ThePlatform(qor_shared)->AddSubsystem<FileSystem>();

    return AppBuilder().Build<Application>("Test JSON Generator")(qor_unlocked).SetRole<app::Role>().
        Run([argv, argc]()->int
            {
                return qor::test::Factory::Instance().main(argc, argv);
            });
}

qor_pp_export_non_msvc qor::Module& ThisModule(void)
{
    static qor::Module QORModule("Querysoft Open Runtime: Test JSON Generator Module", qor_pp_module_ver_string );
    return QORModule;
}
