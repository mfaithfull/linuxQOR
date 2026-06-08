#include "src/configuration/configuration.h"
#include <buildnumber.h>
#include "src/qor/test/test.h"
#include "src/qor/module/module.h"
#include "src/framework/thread/currentthread.h"

int main(int argc, const char** argv, char**)
{	
	return qor::test::Factory::Instance().main(argc, argv);
}

extern "C"
{
	qor::Module& ThisModule(void)
	{
		static qor::Module QORModule("Querysoft Open Runtime: Test Executable Module", 
			qor_pp_stringize(qor_pp_ver_major) "." \
			qor_pp_stringize(qor_pp_ver_minor) "." \
			qor_pp_stringize(qor_pp_ver_patch) "." \
			qor_pp_stringize(qor_pp_buildnumber));
		return QORModule;
	}
}