// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "server.h"
#include "serverrole.h"
#include "src/framework/io/network/sockets.h"
#include "src/components/io/network/server/netserver.h"
#include "src/components/protocols/echo/echoprotocol.h"

using namespace qor;
using namespace qor::log;
using namespace qor::platform;
using namespace qor::io;
using namespace qor::io::network;
using namespace qor::io::network::components;
using namespace qor::components::protocols::echo;

qor_pp_module_requires(Sockets)
qor_pp_module_requires(AsyncIOService)
qor_pp_module_requires(LogAggregatorService)
qor_pp_module_requires(IFileSystem)
qor_pp_module_requires(ICurrentThread)

qor_pp_implement_module(Server::Name)

int main(const int argc, const char** argv, char** env)
{
    DefaultErrorHandler errorHandler;
    DefaultLogHandler logHandler(Level::Debug);
    logHandler.WriteToStandardOutput();

    ThePlatform(qor_shared)->AddSubsystem<Sockets>();
    ThePlatform(qor_shared)->AddSubsystem<FileSystem>();

    return AppBuilder().Build<Server>(Server::Name)(qor_unlocked).
        SetRole<ServerRole>().
        RunWorkflow(new_ref<NetworkServer>(12345, new_ref<EchoProtocol>()));
}
