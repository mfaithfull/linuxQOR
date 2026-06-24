#include "src/configuration/configuration.h"
#include "src/qor/test/test.h"
#include "src/qor/module/module.h"
#include "src/qor/current/currentthread.h"

int main(int argc, const char** argv, char**)
{	
	return qor::test::Factory::Instance().main(argc, argv);
}

extern "C"
{
	qor::Module& ThisModule(void)
	{
		static qor::Module QORModule("Querysoft Open Runtime: Test FileSystem Module", 
			qor_pp_stringize(qor_pp_ver_major) "." \
			qor_pp_stringize(qor_pp_ver_minor) "." \
			qor_pp_stringize(qor_pp_ver_patch));			
		return QORModule;
	}
}