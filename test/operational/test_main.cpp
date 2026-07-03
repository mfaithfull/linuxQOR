#include "src/configuration/configuration.h"
#ifdef NDEBUG
#include <buildnumber.h>
#endif
#include "src/qor/tdd/test/test.h"
#include "src/qor/module/module.h"
#include "src/qor/essentials/current/currentthread.h"

int main(int argc, const char** argv, char**)
{	
	return qor::test::Factory::Instance().main(argc, argv);
}

extern "C"
{
	qor::Module& ThisModule(void)
	{
		static qor::Module QORModule("Querysoft Open Runtime: Operational Tests Executable Module", qor_pp_module_ver_string );
		return QORModule;
	}
}