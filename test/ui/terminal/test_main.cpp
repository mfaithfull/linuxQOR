#include "src/configuration/configuration.h"
#include "src/qor/tdd/test/test.h"
#include "src/qor/module/module.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/framework/app/application/builder.h"
#include "src/framework/app/role/role.h"
#include "src/framework/parallel/thread/threadpool.h"
#include "src/framework/io/filesystem/filesystem.h"
#include "src/qor/flyers/log/defaultloghandler.h"
#include "src/platform/platform.h"

qor_pp_module_requires(ICurrentThread)
qor_pp_module_requires(IFileSystem)

using namespace qor;
using namespace qor::io;
using namespace qor::log;
using namespace qor::platform;
using namespace qor::async;

int main(int argc, const char** argv, char**)
{	
	DefaultLogHandler logHandler(Level::Debug);
	ThePlatform(qor_shared)->AddSubsystem<FileSystem>();

	return AppBuilder().Build<Application>("Test FileSystem")->SetRole<Role>(
		[](ref_of<IRole>::type role)
		{
		}
	).Run([argv, argc]()->int
		{
			return qor::test::Factory::Instance().main(argc, argv);
		}
	);
}

extern "C"
{
	qor::Module& ThisModule(void)
	{
		static qor::Module QORModule("Querysoft Open Runtime: Terminal Tests Module", qor_pp_module_ver_string );
		return QORModule;
	}
}