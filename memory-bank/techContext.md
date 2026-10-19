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
  `pipeline`, `argparse`, `components/formats`, `tef`, `frozen`, `dio`, `uri`,
  `mpl`, `framework/parallel/signaltree`, `framework/parallel/workitem`,
  `components/protocols/http`.
- `test/data/` fixtures: JSON conformance files (`y_*.json`), large JSON
  (2mb–250mb), encoding files (`utf8BOM.txt`, `utf16BEBOM.txt`,
  `utf16LEBOM.txt`, `noBOM.txt`), `hve.html`, `Alice.txt`.
- TDD frameworks: `qor_assert` (fluent assertions header library), `qor_test`
  (Yaffut fork), `qor_mock` (HippoMocks fork) — designed to work together.
- **Recommended build/verify order**: build `examples/basic/helloworld` first
  (smallest dependency set), then `QORTest` (if it builds and runs you're good
  to go; one or two failures are tolerable).
- Windows runs need DLLs copied next to the executable (post-build
  `$<TARGET_RUNTIME_DLLS>` copy via `cmake/postbuild.cmake`).

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
