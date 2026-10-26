# Tech Context

## Language & compiler requirements
- **C++20** (`CMAKE_CXX_STANDARD 20`, `CMAKE_CXX_STANDARD_REQUIRED ON`);
  GCC/Clang flags pass `-std=c++2a`. RTTI enabled (`CMAKE_CXX_RTTI ON`).
  `CMAKE_POSITION_INDEPENDENT_CODE ON`.
- Supported compilers (BUILDING.md; clang support docs from commit `3f2c5128`):
  - **MSVC / Visual Studio 2022 CL** — `/EHsc /GR /utf-8`;
    `CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF`;
    `CMAKE_CXX_VISIBILITY_PRESET hidden`; RelWithDebInfo `/INCREMENTAL:NO`
  - **GCC 13.1.0 / 13.3.0** (default on Ubuntu 24.04) —
    `-std=c++2a -fcoroutines -fvisibility=hidden`
    (Debug adds `-fconcepts-diagnostics-depth=3 -Wno-attributes -Wall -Wextra -Wpedantic`)
  - **clang 18 on Ubuntu 24.04** — `-std=c++2a -fvisibility=hidden`
    (Debug adds `-Wall -Wextra -Wpedantic -Wno-gnu-zero-variadic-macro-arguments`)
- Global define `qor_pp_unicode` (`global_defines` in `cmake/flags.cmake`).
- Heavy PCH usage (build speed and preparation for future C++ modules); macros
  heavily used. **C++ language modules are not used.**
- New compiler versions: add support under `src/platform/compiler/` (usually
  trivial for another GCC/MSVC version).

## Windows SDK headers & include resolution (verified 26 Oct 2026)
- The MSVC 14.50 include tree (`...VC\Tools\MSVC\14.50.35717\include`) holds 112 CRT/C++ headers and NO
  Windows API headers at all - no `Windows.h`, no `winbase.h`.
- `<Windows.h>` resolves from `C:\Program Files (x86)\Windows Kits\10\include\10.0.26100.0\um` (plus `\ucrt`),
  supplied through the `INCLUDE` environment variable that CMake/MSVC set - no CMakeLists adds a `/I` for it.
  Consequence: the Windows SDK headers, including their `FORCEINLINE` API definitions, are visible in every
  api_layer TU, which is what makes header-inline (variant I) wrappers possible - see DECISIONS.md D11.
- `WINAPI_FAMILY_PARTITION` and `_WIN32_WINNT` decide which SDK definitions exist. The threadpool callback-environ
  family is in the APP partition at `_WIN32_WINNT >= 0x0600`; `SetThreadpoolCallbackPriority` needs
  `>= _WIN32_WINNT_WIN7`; `SetThreadpoolCallbackPersistent` is DESKTOP-partition only.
- Header-only entry points are NOT dll exports and must never be bound with `qor_pp_useswinapi` /
  `GetProcAddress` - that path returns null and throws at run time (DECISIONS.md D10-CORRECTION, D11).
- Executor (`run_commands`) quirks are kept as a project rule set in `.clinerules/tool-execution.md`:
  `$_` is mangled to `$'` (use `$PSItem`), array elements run concurrently so dependent steps must be
  sequenced with `;` inside ONE string, and long builds report "completion could not be observed" - always
  verify out of band with a redirected log plus `$LASTEXITCODE`.

## Build system
- **CMake** minimum `3.10.0`, CMake 4+ recommended. Top-level
  `CMakeLists.txt`: `project(QOR VERSION 0.5.0 … LANGUAGES C CXX)`,
  `CMAKE_SUPPRESS_REGENERATION true`, `include_directories` (source root),
  `link_directories(${CMAKE_SOURCE_DIR}/lib)`, includes `cmake/flags.cmake`,
  `find_library` for `GLESv2`/`EGL` (Windows HINT:
  `F:/Develop/Native_SDK/lib/Windows_x86_64`), then `add_subdirectory(src)`,
  `add_subdirectory(test)`, `add_subdirectory(examples)`.
- `cmake/` helpers: `flags.cmake`, `buildnumber.cmake`,
  `updatebuildnumber.cmake` (injects `qor_pp_ver_major/minor/patch` from
  `PROJECT_VERSION`), `postbuild.cmake`, `update_directory.cmake`.
- Every target `include("cmake/flags.cmake")`. Examples declare their own
  `project(...)` and link `qor_module qor_host qor_compiler qor_platform
  qor_application`.
- Generator: **Ninja** for the checked-in `CMakeSettings.json`
  (`x64-Debug`, `inheritEnvironments: msvc_x64_x64`,
  `buildRoot = ${projectDir}\cmakebuild`,
  `installRoot = ${projectDir}\out\install\${name}`).
- Build output dirs: `cmakebuild/` (contains `src`, `test`, `examples`,
  `ALL_BUILD.dir`, `CppDependOut`, `x64`), `build/` (IDE), `out/` (install).
  All git-ignored.
- CI: `.github/workflows/cmake-multi-platform.yml` (badge in README) builds on
  Ubuntu-latest; full build ~26 min; < 30 min on a reasonable PC; clean build
  of the incomplete HTTPServer example ~38 s.

## IDE / developer environment
- Developed in **VSCode on Windows and Ubuntu** (known-good). VS 2022 also
  works; other IDEs need project files generated.
- VSCode extensions (all Microsoft): C/C++, C/C++ DevTools, C/C++ Extension
  Pack, C/C++ Themes, CMake, optionally PowerShell.
- Windows workflow: Windows Terminal → VS 2022 Developer Command Prompt
  (Administrator) → `code .` in the QOR root. Ubuntu: `code .&` (WSL uses the
  Windows host VSCode).
- `.vscode/settings.json` maps STL headers (`vector`, `memory`, `coroutine`,
  `ranges`, `netfwd`, `socket`, `resumable`, `*.tcc`, `*.rh`, `*.inl`, …) to
  `cpp` for IntelliSense.
- `CMakeSettings.json` is active; `not_CMakePresets.json` is not.

## External dependencies
- **Linux**: `liburing` built from source
  (`git clone https://github.com/axboe/liburing` → `./configure && make &&
  sudo make install` ), linked as `uring` in `test/CMakeLists.txt`.
  X11/Wayland dev packages needed for everything to build:
  `libegl1-mesa-dev libgles2-mesa-dev libx11-dev libxext-dev libxrandr-dev
  libxinerama-dev libxcursor-dev libxi-dev libxkbcommon-dev libwayland-dev
  libwayland-egl-backend-dev wayland-protocols` (GitHub workflow setup steps
  in BUILDING.md; box also needs build-essentials).
- **Windows**: EGL/OpenGLES libraries from e.g. the PowerVR SDK
  (`Native_SDK`), referenced by HINT paths in the root `CMakeLists.txt`
  (machine-specific — adjust on another machine).
- In-tree 3rd party: `3rdparty/glad` (OpenGL loader), `3rdparty/Windows`.
- Forked / attributed upstreams (CREDITS.md): **Boost** (preprocessor library,
  pfr, top-level license), **Yaffut** (mrAtari) → `qor_test`,
  **HippoMocks** → `qor_mock`, **pfr_non_boost** → `qor_reflection`,
  Boost.Any-inspired `qor_objectcontext`, **SAKryukov** delegate (CodeProject),
  **CopperSpice cs_signal** → `qor_signals` + rculist in `qor_datastructures`,
  **BS thread_pool** → `qor_thread`, **Lewis Baker cppcoro** → AsyncIO and
  Tasks, **blIteratorAPI** → custom container iterators, **nlohmann fifo_map**,
  **simfeo FancyArgumentParser**, FTXUI (terminal code mostly ported).

## Testing setup
- Test tree mirrors `src/` areas: application, argparse, assert, components
  (formats/network/pipeline/protocols/threadmemory), contract, data,
  datastructures, delegate, dio, error, factory, filesystem, framework, frozen,
  injection, interception, issue, iterators, mock, mpl, objectcontext,
  observer, operational, pipeline, platform, property, reference, reflection,
  singleton, tef, text, ui, uri.
- **`QORTest`** is the main aggregated unit-test executable
  (`test/CMakeLists.txt`): one `add_executable` listing ~70 test `.cpp` files,
  compiled with `CONSOLE` and `qor_pp_ver_*` defines, linked with OS-specific
  target lists.
- Separate test executables built from sub-directories: `operational`
  (`QOROpTests` — thread pool, profiling, excluded tests), `filesystem`, `ui`,
  `pipeline`, `argparse`, `components/formats` (`QORTestJSON`),
  `components/formats/json/generator` (`QORTestJSONGenerator`, added Oct 2026),
  `components/protocols/yaml` (`QORTestYAML`, added Oct 2026),
  `components/protocols/http` (`QORTestHTTPRequestGen`, added Oct 2026),
  `tef`, `frozen`, `dio`, `uri`, `mpl`, `framework/parallel/signaltree`,
  `framework/parallel/workitem`. The `test/CMakeLists.txt` `add_subdirectory`
  list is the authoritative inventory.
- `test/data/` fixtures: JSON conformance files (`y_*.json`), large JSON
  (2mb–250mb), encoding files (`utf8BOM.txt`, `utf16BEBOM.txt`,
  `utf16LEBOM.txt`, `noBOM.txt`), `hve.html`, `Alice.txt`.
  Test executables reach them by **relative** path — `QORTestJSON` uses
  `"../../../../../../test/data"` (depth depends on the executable's
  location under the build tree); never re-introduce an absolute
  `F:/Develop/...` path.
- TDD frameworks: `qor_assert` (fluent assertions header library), `qor_test`
  (Yaffut fork), `qor_mock` (HippoMocks fork) — designed to work together.
- **Recommended build/verify order**: build `examples/basic/helloworld` first
  (smallest dependency set), then `QORTest` (if it builds and runs you're good
  to go; one or two failures are tolerable).
- Dedicated test exes are under their own subdirectories in the build tree. For
  example, `QORTestJSONGenerator.exe` is at
  `cmakebuild/test/components/formats/json/generator/Debug/QORTestJSONGenerator.exe`,
  not directly under `cmakebuild/`.
- Windows runs need DLLs copied next to the executable (post-build
  `$<TARGET_RUNTIME_DLLS>` copy via `cmake/postbuild.cmake`).

## Build / verify recipe for a file-using test exe (Windows MSVC, 20 Oct 2026)

```
cmake -B cmakebuild -S . -DCMAKE_BUILD_TYPE=Debug          # re-emit VS project files
cmake --build cmakebuild --target QORTestJSONGenerator
Set-Location cmakebuild/test/components/formats/json/generator/Debug   # run FROM the exe dir
.\QORTestJSONGenerator.exe *> ../../../../../tmp/run.log               # -> [OK](36/36)
```

- **Run a test exe from its own build directory**, not from the repo root: the
  test sources address `test/data` by a relative path calibrated to the exe's
  location (the generator test uses
  `../../../../../../../test/data`). Run from the repo root and the 5 file tests
  throw `unknown exception` (`[FAIL](5/36)`, exit 5) even though the pipeline is
  fine; from the exe directory it is `[OK](36/36)`, exit 0.
- **Regenerate before rebuilding after any CMakeLists edit.** The build tree holds
  generated `.vcxproj` files and `cmake --build` does not re-emit them when only a
  `target_link_libraries` list changed, so symbols of the newly linked library fail
  with `LNK2019` unresolved externals (its `.lib` import library is never a
  dependency). Use `cmake -B cmakebuild -S . -DCMAKE_BUILD_TYPE=Debug` — **no**
  `-G`; the cached generator is the Visual Studio one and
  `-G "Visual Studio 2026"` is not a valid generator name in this tree.
- A test exe that hooks a pipeline to a file must link
  `qor_fileconnector qor_filesystem` plus the OS implementation
  (`winqor_filesystem`/`linqor_filesystem`, Windows also needs
  `winqor_common`) **and** register the subsystem in its `main.cpp`
  (`ThePlatform(qor_shared)->AddSubsystem<FileSystem>()`,
  `qor_pp_module_requires(IFileSystem)`) — otherwise the failure is a **runtime**
  module-requirement failure, not a link error.
- Run the exe in a **separate command** from the build (a stale artifact may run
  otherwise) and capture **both** streams (`exe *> log`) — `[FAIL](n/n)` and the
  `[ALL FAILURES]` block go to stderr. `QORTestJSONGenerator` currently reports
  `[OK](36/36)` and writes `test/data/json_generator_*.json`.
- A `qor_*` module's `log::debug` calls produce **no output** in a minimal test exe like
  `QORTestMIME` — the log flyer/subsystem is not wired into that `main.cpp`. Do not use log
  output as a test probe there; assert on the module's own observable state instead
  (verified 20 Oct 2026 while working qor_mime plan step 2).



## Build / verify recipe for the `QORTestJSON` roundtrip increment (20 Oct 2026)

    msbuild QOR.sln /t:QORTestJSON              # BUILD_EXIT=0
    Set-Location cmakebuild/test/components/formats/json/Debug
    .\QORTestJSON.exe *> <log>                  # process exit code 0
    Select-String -Path <log> -Pattern 'roundtrip'   # expect no matches

- `QORTestJSON` `main.cpp` addresses fixtures with
  `"../../../../../../test/data"` (six levels up from
  `cmakebuild/test/components/formats/json/Debug`), so like every QOR test exe it
  must run from its own build directory.
- Its link list in `test/components/formats/json/CMakeLists.txt` now also carries
  `qor_generator qor_generatorsource` (plus the file-pipeline libraries), and the
  test exe registers the `FileSystem` subsystem as `QORTestJSONGenerator` does.
- The run log is **huge**: the `y_object_string_korean_utf16le` /
  `y_object_string_korean_utf16be` fixtures emit a per-character
  `Parser::InnerParse` debug line, so grep for markers (`roundtrip`, `FAIL`,
  `OK(`) rather than dumping the log, and keep new assertion blocks **before**
  those fixtures so a failure is still findable.
- Transient build/run logs belong in the gitignored `tmp/` folder — do not leave
  them at the repo root.

## Repo conventions
- `.gitignore`: object/obj/so/dll/exe artifacts, `.vs`, `build/`, `.vscode`,
  `.VSCodeCounter`, `/cmakebuild`, `/out/build/x64-Debug`, generated CMake
  files in `build/`, `/libs`.
- Branches: `main`, `Contract`, `DomainInterfaceObjects`,
  `FileSystemRefactor`, `PipeConnectorExperiment`, `day1`, `module_support`.
- `sdk/` is generated by this project for use by other projects (`app.h`,
  `qor.h`, `framework.h`, `essentials.h`, `memory.h`, `error.h`, `log.h`,
  `tdd.h`, plus `framework/` and `platform/` sub-headers and a `status.md`).
- Docs: `docs/` (tutorial, structure.md, module system.md, linking.md,
  globalobjects.md, levels_of_abstraction.md, power_of_context.md,
  smart_references.md, sfumato.md, typetraits.md, compilers.md, articles/,
  skills/QORconventions.md).
- **Agent tooling in `docs/agent/`**: `repo_map*.md` (root, details, js, test,
  examples, deps), `purposes.md`, `verification.md`, `deps.dot` (+
  `deps_check.ps1`), per-library **ctags** in `docs/agent/ctags/`,
  `STATE.MD`/`DECISIONS.md`/`TODO.md`, skill files `add_module.md` +
  `parsers.md`, `skill.md` (the `TODO.md`-driven workflow), `STATUS.MD`,
  `yaml_spec_raw.md`, `yaml_cfg_ebnf.md`. Root `yaml_exports.txt` and
  `docs/agent/_tmp_*` are transient scratch.
- CMake uses `##` comments; disabled targets/dependencies appear as
  `##qor_...` / `##add_subdirectory(...)` — always check before assuming a
  module is built.

## qor_mime lib + QORTestMIME exe (20 Oct 2026)

New framework-layer data module and its dedicated test exe (verified this session):
`cmake --build build --target qor_mime` → `build\src\framework\data\mime\Debug\qor_mime.dll` (+ `.lib`/`.exp`)
`cmake --build build --target QORTestMIME` → `build\test\framework\data\mime\Debug\QORTestMIME.exe`
  → run it: `[OK](1/1)` exit 0. Test judge = exe output + exit code; the QOR does not use ctest (D8).
`qor_mime` links `qor_module qor_host qor_current qor_sync qor_signals qor_pipeline`.
`QORTestMIME` additionally pulls `qor_stringsource`/`qor_stringsink` (pipeline pumping in tests) —
first time those two component libs were ever built for a test exe.
Gotcha: `cmake --build build --target X -- -j4` fails on the VS 2026 generator (`MSB1001`);
omit `-j4` or use CMake's own `-j N`.


## Build / verify a new QOR module and its dedicated test exe (verified recipe + gotchas, qor_mime 20-22 Oct 2026)

The sequence that took `src/framework/data/mime` (lib `qor_mime`) and `test/framework/data/mime`
(exe `QORTestMIME`) from nothing to green, and the pitfalls it exposed:

    cmake -B cmakebuild -S . -DCMAKE_BUILD_TYPE=Debug            # only when adding a new .cpp/target
    cmake --build cmakebuild --target qor_<NAME> --config Debug   # ONE target per command
    cmake --build cmakebuild --target QORTest<NAME> --config Debug
    Set-Location cmakebuild\test\<area>\<name>\Debug
    .\QORTest<NAME>.exe *> <abs path>\tmp\<name>.log              # capture stdout AND stderr
    # green = `[OK](n/n)` in the log AND shell exit code 0 read in the SAME command

- **One `cmake --build` per command.** Two builds launched in one command race on the same `.pdb`
  and fail with `C1001` / `LNK1120` "cannot open file"; sequential commands build fine.
- **Build and run in separate sequential commands.** In one combined command the run can start
  before the link finishes, so the exe is stale and a freshly added case reports *missing* - a
  false "missing", not a test failure. Re-run alone after the build and the case shows up.
- The VS 2026 generator rejects `-j4` passed after `--` (`MSB1001`); use CMake's own `-j N` or omit.
- `LNK2019` unresolved externals for a lib a test exe links means the **build tree is stale or
  missing that lib**, or the generated VS project files are stale after adding a `.cpp`. Build in
  the ACTIVE configured tree (`cmakebuild`, not the older `build`), and regenerate with
  `cmake -B cmakebuild -S . -DCMAKE_BUILD_TYPE=Debug` - no `-G`; the cached generator is the VS
  one and `-G "Visual Studio 2026"` is not a valid name in this workspace.
- A QOR test exe addresses its data by paths relative to **its own build directory** (e.g.
  `"../../../../../../test/data"`) - always run it from there.
- A QOR test run log can be huge (unrelated cases emit per-character debug lines). Capture both
  streams (`exe *> log`: `[FAIL](n/n)` and the `[ALL FAILURES]` block are on stderr), then grep for
  markers (`FAIL`, `OK(`, the case name) instead of dumping, and keep new assertion blocks **early**
  in the test file so a failure stays findable.
- Transient build/run logs belong in the gitignored `tmp/` folder, never at the repo root.
- `log::debug` is **not** wired to output in these test exes - it is not a usable probe. Assert on
  observable API state (counters, accessors, sink contents) instead.

- Header-inline (variant I) Windows API wrappers, implemented 26 Oct 2026 in `api_layer/library.h`: `Library::inlineCall`
  and `Library::voidInlineCall` take the **address of the SDK header definition** (`::Name` at global scope) instead of a
  `DefProc`, so they keep the `__try` / `__except(ExceptionFilterFunction(GetExceptionInformation()))` guard while emitting
  no import and no null test. Call them with **explicit** template arguments, as the export-bound wrappers already do:
  `Library::voidInlineCall< ::PTP_CALLBACK_ENVIRON >( ::InitializeThreadpoolEnvironment, pcbe )`. Pair them with the
  `qor_pp_usesinlineapi( Name )` marker in `kernel32.h`, which expands to nothing and exists only so audit and grep tooling
  see one binding marker per wrapper. Decide (E) export-bound vs (I) header-inline from the SDK header text - grep the Windows
  Kits include tree for a `FORCEINLINE` definition - never from `dumpbin` absence. See DECISIONS.md D11 and D11-IMPLEMENTED.

