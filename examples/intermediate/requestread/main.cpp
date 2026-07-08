// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//HTTP Request parsing

#include "src/configuration/configuration.h"
#include "src/components/io/pipeline/connectors/fileconnector/fileconnector.h"
#include "src/components/protocols/json/parser/tokens.h"
#include "src/components/protocols/json/parser/nodes/object.h"
#include "src/components/data/pipeline/sinks/parsersink/parsersink.h"
#include "src/framework/io/filesystem/filesystem.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/platform/platform.h"
#include "src/framework/app/application/application.h"
#include "src/framework/app/application/builder.h"
#include "src/framework/app/role/role.h"

#include "requestreader.h"

using namespace qor;
using namespace qor::io;
using namespace qor::components;

const char* appName = "HTTPRequest Read";
qor_pp_implement_module(appName)
qor_pp_module_requires(IFileSystem);
qor_pp_module_requires(ICurrentThread);

int main()
{
    ThePlatform(qor_shared)->AddSubsystem<FileSystem>();

    return AppBuilder().Build(appName)->SetRole<app::Role>().Run(
        []()->int
        {
            RequestReader reader;
            
            auto request = reader(
                io::components::FileConnector(
                    filesystem::Index(
                        ThePlatform(qor_shared)->
                            GetSubsystem<FileSystem>()->
                                CurrentPath(), "request.txt"
                    ),
                    reader.Buffer(),
                    OpenFor::ReadOnly,
                    WithFlags::None                    
                )
            );

            return 0;
        });
}
