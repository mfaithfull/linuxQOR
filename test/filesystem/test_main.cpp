#include "src/configuration/configuration.h"
#include "src/qor/tdd/test/test.h"
#include "src/qor/module/module.h"
#include "src/qor/essentials/current/currentthread.h"
#include "src/framework/app/application/builder.h"
#include "src/framework/app/role/role.h"
#include "src/framework/parallel/thread/threadpool.h"
#include "src/framework/io/async/service.h"
#include "src/framework/io/filesystem/filesystem.h"
#include "src/qor/flyers/log/defaultloghandler.h"
#include "src/platform/platform.h"

qor_pp_module_requires(AsyncIOService)
qor_pp_module_requires(ICurrentThread)

using namespace qor;
using namespace qor::io;
using namespace qor::log;
using namespace qor::platform;
using namespace qor::thread;
using namespace qor::io::async;

int main(int argc, const char** argv, char**)
{
	DefaultLogHandler logHandler(Level::Debug);
	ThePlatform(qor_shared)->AddSubsystem<FileSystem>();

	return AppBuilder().Build<Application>("Test FileSystem")->SetRole<app::Role>(
		[](ref_of<app::IRole>::type role)
		{
			role->template AddFeature<ThreadPool>(
				[](ref_of<ThreadPool>::type threadPool)->void
				{
					threadPool->SetThreadCount(3);
					CurrentThread::Get().SetName("Main");
				}
			);
			role->template AddFeature<async::Service>(
				[](ref_of<async::Service>::type /*ioService*/)->void
				{
					PoolInstancer::SetPoolSize<Context>(2);
				}
			);
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
		static qor::Module QORModule("Querysoft Open Runtime: Test FileSystem Module", qor_pp_module_ver_string );
		return QORModule;
	}
}