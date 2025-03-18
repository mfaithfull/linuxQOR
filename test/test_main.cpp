#include "../src/configuration/configuration.h"
#include "../src/qor/test/test.h"

//------------------------------------------------------------------------------
int main(int argc, const char** argv, char** env)
{	
	static const char* params[] = {argv[0],"-h"};
	return qor::test::Factory::Instance().main(2, params);
}