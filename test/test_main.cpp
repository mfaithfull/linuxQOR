#include "../src/configuration/configuration.h"
#include "../src/qor/test/test.h"
#include "../src/qor/module/module.h"

int main(int argc, const char** argv, char** env)
{	
	return qor::test::Factory::Instance().main(argc, argv);
}

qor::Module& ThisModule(void)
{
	static qor::Module QORModule("Querysoft Open Runtime: Executable Module", 
        qor_pp_stringize(qor_pp_ver_major) "." qor_pp_stringize(qor_pp_ver_minor) "." qor_pp_stringize(qor_pp_ver_patch) "." __DATE__ "_" __TIME__);
	return QORModule;
}
