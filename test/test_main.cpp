#include "../src/configuration/configuration.h"
#include "../src/qor/test/test.h"

int main(int argc, const char** argv, char** env)
{	
	return qor::test::Factory::Instance().main(argc, argv);
}