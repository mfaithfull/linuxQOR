# Skill: Adding a new QOR module (and its test)

Distilled from the qor_yaml experiment (built + test-passed on Windows/MSVC).
A future agent can add any module by following this file; do not re-read the
250k-line tree. Verified 2026 against src/components/protocols/yaml.

## 0. Naming conventions (pick one, stay consistent)
- library target : qor_<name>        (lowercase, e.g. qor_yaml, qor_uri)
- module dir     : src/components/<subsystem>/<name>/   (lowercase)
- impl file      : <name>.cpp          (e.g. yaml.cpp)
- module singleton file: qor_<name>.cpp   (MANDATORY, see §3)
- compile define : QOR_<NAME> uppercase (e.g. QOR_YAML)
- test target    : QORTest<Name>     CamelCase exe (e.g. QORTestYAML)
- test dir       : test/components/<subsystem>/<name>/
- test files     : test_main.cpp + test_<name>.cpp
- test case      : <name>_...        (e.g. yaml_module_loads)

## 1. Register in the CMake subdirectory chain
add_subdirectory(<name>) at every level from root to the leaf dir:
  CMakeLists.txt -> add_subdirectory(src)
  src/CMakeLists.txt -> add_subdirectory(components)
  src/components/CMakeLists.txt -> add_subdirectory(<subsystem>)
  src/components/<subsystem>/CMakeLists.txt -> add_subdirectory(<name>)     <-- ADD THIS
  test/CMakeLists.txt -> add_subdirectory(components/<subsystem>/<name>)     <-- ADD THIS

## 2. Module CMakeLists.txt  (src/components/<subsystem>/<name>/CMakeLists.txt)
Template (copied from qor_uri; SHARED => .dll):

    set(target qor_yaml)
    add_library(${target} SHARED qor_yaml.cpp yaml.cpp)
    include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
    include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
    target_compile_definitions(${target} PRIVATE QOR_YAML
    qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
    qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
    qor_pp_ver_patch=${PROJECT_VERSION_PATCH})
    target_link_libraries(${target} qor_module qor_host qor_current qor_sync qor_injection qor_parser)

Notes:
- add_library(... SHARED ...) => qor_<name>.dll. A plain add_library w/o SHARED => .lib static.
- The three qor_pp_ver_* defines feed qor_pp_module_ver_string (built from qor_pp_ver_major/minor/patch by configuration.h + buildnumber.cmake). Required in every module.
- Protocol/leaf modules core-deps = qor_module qor_host qor_current qor_sync qor_injection qor_parser (uri/echo/http use this set). Add subsystem-specific deps as needed.

## 3. The mandatory module singleton  (src/.../qor_yaml.cpp)
Copy verbatim (only the name string changes). Every QOR module MUST define this;
src/qor/module/module.h forward-declares qor::Module& ThisModule(); and links
require the definition (module.cpp/library.cpp register it into the module chain).

    // Copyright Querysoft Limited 2008 - Present
    // SPDX-License-Identifier: BSL-1.0
    #include "src/configuration/configuration.h"
    #ifdef NDEBUG
    #include <buildnumber.h>
    #endif
    #include "src/qor/module/module.h"

    qor::Module& ThisModule(void)
    {
        static qor::Module QORModule("Querysoft Open Runtime: YAML Module", qor_pp_module_ver_string );
        return QORModule;
    }

## 4. !!! GOTCHA -- shared QOR module MUST export >=1 symbol on Windows !!!
flags.cmake sets CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF + hidden visibility.
A SHARED target that exports NOTHING produces NO import lib (.lib). A test exe
that does target_link_libraries(exe ... qor_<name>) then fails:
    LNK1104: cannot open file '...qor_<name>.lib'
=> Put at least one qor_pp_export symbol in the module's implementation file.
qor_pp_export == __declspec(dllexport) on MSVC (src/platform/compiler/msvc/msvc.h);
qor_pp_export_non_msvc expands to NOTHING on MSVC (it is the visibility attribute
for GCC/Clang only). So on Windows use qor_pp_export, not qor_pp_export_non_msvc.

    // src/components/protocols/yaml/yaml.cpp
    #include "src/configuration/configuration.h"
    namespace qor { namespace protocols {
        qor_pp_export void init_yaml(void)   { /* placeholder future API */ }
    }}//qor::protocols

On Linux the .so is linked directly so the dll-lib issue does not arise, but keep
the qor_pp_export marker for Windows correctness (and to give the public API a real signature).

## 5. Test project CMakeLists.txt  (test/components/<subsystem>/<name>/CMakeLists.txt)
Template (qor_uri + qor_dio). qor_<name> MUST appear in qor_libraries:

    set(target QORTestYAML)
    include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
    add_executable(${target} test_main.cpp
      test_yaml.cpp)
    target_compile_definitions(${target} PRIVATE CONSOLE 
    qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
    qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
    qor_pp_ver_patch=${PROJECT_VERSION_PATCH})
    set(qor_libraries qor_module qor_compiler qor_host qor_arch qor_current qor_thread
    qor_injection qor_sync qor_text qor_issue qor_error qor_log qor_reporting qor_platform
    qor_application qor_role qor_yaml)
    if(CMAKE_SYSTEM_NAME MATCHES Linux)
    target_link_libraries(${target} ${qor_libraries} linqor_filesystem linqor_thread)
    endif()
    if(CMAKE_SYSTEM_NAME MATCHES Windows)
    target_link_libraries(${target} ${qor_libraries} winqor_api_kernel winqor_filesystem 
    winqor_thread winqor_common) 
    endif()
    if (WIN32)
      add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
        COMMAND_EXPAND_LISTS
      )
    endif ()
    enable_testing()
    add_test(TestYAML ${target})

Notes:
- CONSOLE define => test exe writes to console.
- Platform link block mirrors qor_dio: Linux gets linqor_*, Windows gets winqor_*.
- POST_BUILD DLL copy is required on Windows so the exe can find sibling DLLs at runtime.

## 6. test_main.cpp  (test/components/.../test_yaml/test_main.cpp)  copy verbatim, rename string

    // Copyright Querysoft Limited 2008 - Present
    // SPDX-License-Identifier: BSL-1.0
    #include "src/configuration/configuration.h"
    #include "src/qor/tdd/test/test.h"
    #include "src/qor/module/module.h"
    #include "src/qor/essentials/current/currentthread.h"
    #include "src/platform/platform.h"
    #include "sdk/app.h"

    qor_pp_module_requires(ICurrentThread)
    using namespace qor;
    using namespace qor::app;
    using namespace qor::platform;

    int main(int argc, const char** argv, char** /*env*/)
    {
        return AppBuilder().Build<Application>("Test YAML")(qor_unlocked).SetRole<app::Role>().
            Run([argv, argc]()->int
                {
                    return qor::test::Factory::Instance().main(argc, argv);
                });
    }

    qor_pp_export_non_msvc qor::Module& ThisModule(void)
    {
        static qor::Module QORModule("Querysoft Open Runtime: Test YAML Module", qor_pp_module_ver_string );
        return QORModule;
    }

## 7. test_<name>.cpp  (the actual test)
Use the qor_pp_test_case macro (NOT a bare assert()). Bare assert collides with
the qor::assert namespace -> error C2882 'illegal use of namespace identifier'.

    // Copyright Querysoft Limited 2008 - Present
    // SPDX-License-Identifier: BSL-1.0
    #include "src/configuration/configuration.h"
    #include "src/qor/tdd/tdd.h"
    using namespace qor;

    qor_pp_test_case(yaml_module_loads)
    {
        // qor_yaml.dll is linked and its module singleton is registered on load.
        // This placeholder confirms the module is present in the executable image.
    }

## 8. Build + verify (Windows/MSVC, VS2026 toolchain on this box)
cmake -S . -B build
cmake --build build --target qor_yaml          # => build/.../Debug/qor_yaml.dll (+ .lib)
cmake --build build --target QORTestYAML       # <= qor_yaml.lib now found (no LNK1104)
"build/test/components/protocols/yaml/Debug/QORTestYAML.exe"
# expected output:
#   QOR Test.  Version 0.5 based on YAFFUT 1.1
#   ...test_yaml.cpp(9) : yaml_module_loads
#   [TOTAL](1/1)  [OK](1/1)  exit=0

## 9. Failure-mode quick reference
- LNK1104 cannot open qor_<name>.lib          -> missing qor_pp_export symbol (GOTCHA §4).
- C2882 illegal use of namespace identifier   -> bare assert(); use qor_pp_test_case.
- link line lacks a lib at all                -> module not registered via add_subdirectory (§1).
- undefined ThisModule                        -> qor_<name>.cpp singleton missing (§3).

## Verification of this experiment
src/components/protocols/yaml/{CMakeLists.txt,qor_yaml.cpp,yaml.cpp}  -> qor_yaml.dll PASS
test/components/protocols/yaml/{CMakeLists.txt,test_main.cpp,test_yaml.cpp} -> QORTestYAML.exe PASS, test OK(1/1) exit=0