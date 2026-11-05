// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//=== api_layer conventions ========================================= Reviewed 5 Nov 2026
//   (session 37: correctness / consistency / safety / UB / conventions; the module glue, 0 wrappers)
//
//Last TU of the folder review, and the only one with no wrapper in it: 0 User32:: definitions, 0 declarations in
//user32.h, 0 qor_pp_useswinapi bindings, 0 qor_pp_fcontext, 0 Library::Call, 0 CheckReturn, 0 Win32ErrorHandler,
//0 CONST and 0 lowercase const (measured pre-banner: 386 bytes, 14 CRLF, final newline present, 2 tabs). Steps 1-5b
//have nothing here to pair, bind or scan, so U4, U11, U13, U17, U18, U19, U25, U26 and P1/P2/P6 are 0 in this TU by
//construction rather than by measurement - the export-table capture, the /EP binding digest, the wrong-stem scan and
//the LAYER/BYPASS scan have no input to read, and none was run.
//
//1 declaration, 1 definition, and they pair. module.h:49 declares "qor::Module& ThisModule();" at global scope with
//the header's own contract attached ("each and every module that includes this header must provide a ThisModule
//function returning its module representation object"); the definition at :79 spells the parameter list "(void)",
//which is the same function type. No namespace block, correctly - the declaration it answers is at global scope.
//
//The TU is live, and it is the folder's module identity. qor_pp_fcontext and qor_pp_ofcontext expand to
//"qor::FunctionContext _FContext_(std::source_location::current(), ThisModule().Name(), ...)"
//(src/qor/flyers/interception/functioncontext.h:19, :22), so every wrapper in the folder reads the object defined
//here: over the 38 .cpp there are 433 qor_pp_fcontext sites and 479 qor_pp_useswinapi occurrences on 477 lines, all
//resolving to this one Module object and to the name literal at :81. (479 is not the wrong-stem scan's 428-binding
//population - that scan parses the call form, this grep counts every spelling of the macro.)
//
//Linkage is safe as spelled: ThisModule has external linkage at global scope but is not exported -
//CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS is OFF (cmake/flags.cmake:16) and the definition carries no qor_pp_module_interface.
//Measured on a built image (dumpbin /exports of the cmakebuild Debug winqor_api_user.dll - a stale 29 Oct 2026 build,
//used as linkage evidence only, never as a gate): 427 exported names, 0 of them named ThisModule. The twelve sibling
//api_layer glue TUs each define the same-named function at global scope and cannot collide with this one or bind
//another DLL's module object: the idiom is per-DLL by construction.
//The guarded include at :74-:76 is load-bearing, not a T7 dead include - the session-36 pre-flag called it otherwise
//and the pre-flag is wrong. qor_pp_module_ver_string's NDEBUG arm (src/macros/moduleversion.hpp:25-29) stringizes
//qor_pp_buildnumber, which only the generated buildnumber.h defines, and the guard here matches that macro's own
//#ifdef NDEBUG exactly. Measured three ways (tmp/_tmp_s37_probe.bat, tmp/_tmp_s37_epdig.bat): gate defines with no
//NDEBUG, exit 0 and no diagnostics; /DNDEBUG with the header absent, fatal C1083 at :75, exit 2; /DNDEBUG with a probe
//header defining 9999, exit 0 and /EP hands the ctor "0" "." "5" "." "0" "." "9999". CMakeLists.txt:42
//(updatebuildnumber.cmake) is what puts that header on the include path. End to end in shipped images: the
//RelWithDebInfo winqor_api_user.dll contains the literal 0.5.0.1, the Debug one 0.5.0, both beside the name at :81.
//
//The other half of that asymmetry is silent, and it is this TU's only real exposure. qor_pp_ver_major/minor/patch
//exist only as target_compile_definitions (CMakeLists.txt:45-48, PUBLIC). Compile with them absent and the TU builds
//clean - exit 0, zero diagnostics at /W4 - because qor_pp_stringize_i is #text (src/macros/stringize.hpp:43), so an
//undefined name stringizes to its own token: /EP then hands the Module ctor "qor_pp_ver_major" "." "qor_pp_ver_minor"
//"." "qor_pp_ver_patch". A missing build number fails loud (C1083); a missing version triple is never diagnosed, and
//the module then reports its own macro names to every FunctionContext in the folder, forever. Nothing to fix in this
//TU - its own wiring is correct - and per user decision (5 Nov 2026) no register is opened: guarding it (an #error on
//an undefined qor_pp_ver_major, or a CMake-side check) is a build-system change across 163 CMakeLists.txt, not a
//folder-pass one.
//
//Idiom conformance: the 13 winqor_api_*.cpp glue TUs of api_layer are one shape - the same three includes in the same
//order, "qor::Module& ThisModule(void)", a function-local static qor::Module, qor_pp_module_ver_string as the second
//argument - under 13 distinct module names, "Windows API User Module" occurring exactly once in src. The object is a
//Meyers singleton: constructed on first call, thread-safe since C++11, destroyed at exit, so it takes no
//static-init-order dependency on the Library -> Module -> Host chain (memory-bank/systemPatterns.md:33-34) and the TU
//has 0 UB.
//
//Recorded, not fixed (U14 is frozen by D15, no tally booked): module.h:51-58 offers the macro form of this very
//idiom, qor_pp_implement_module(_X), and neither this TU nor its 12 siblings use it. The two are not interchangeable:
//the macro adds qor_pp_export_non_msvc, which expands to nothing on MSVC (configuration.h:42-45) so it cannot matter
//in a Windows-only folder, but it also builds the version string WITHOUT the build number (module.h:56), so adopting
//it would change what Release reports. All 13 differ only in the name literal and the licence header (5 SPDX 2-line,
//8 old MIT), all leave the space before ")", and all indent the two body lines with a tab, not 4 spaces.
//
//T7: 0 dead includes here. configuration.h is required first (configuration.h:4-6), module.h supplies qor::Module and
//the ThisModule declaration, and the guarded buildnumber.h is load-bearing as above; this TU carries no user32.h,
//../library.h or ../returncheck.h, so it adds nothing to the folder's dead-include count. U1 (no handler installed)
//and U7 (Library::Call's default-constructed return) cannot arise: 0 calls.
//
//Gate: this TU standalone at /W4 exit 0, then the whole folder 38/38 exit 0 with no diagnostics, both re-run after
//this banner (dated by tmp/obj/winqor_api_user.obj being newer than the source); cl /EP after the banner still yields
//the version and name literals quoted above.
#include "src/configuration/configuration.h"
#ifdef NDEBUG
#include <buildnumber.h>
#endif
#include "src/qor/module/module.h"

qor::Module& ThisModule(void)
{
	static qor::Module QORModule("Querysoft Open Runtime: Windows API User Module", qor_pp_module_ver_string );
	return QORModule;
}
