
#include "src/configuration/configuration.h"

#include "controller.h"
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"

extern "C" const ImageDOSHeader __ImageBase;//This must be injected by the Compiler for Windows builds

int main();

//WinMain shouldn't really be necessary except that CMake forces the link to it to make a 'valid' Windows executable
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int){ return main(); }

int main()
{
    RetroPadController retropad((void*)(&__ImageBase));//We grab the instance pointer here which is injected by the compiler for Windows builds
    retropad.Run();
}
