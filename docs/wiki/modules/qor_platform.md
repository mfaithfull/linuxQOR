---
Type: WikiPage
PageType: module
Title: "qor_platform"
Target: qor_platform
Status: current
LastVerified: 2026-11-16
Claims: 5
Summary: "Umbrella of arch+os+compiler; the base the rest of QOR stands on."
Anchors:
  - { kind: in-tree, path: src/platform, symbol: qor_platform, verified: 2026-11-16 }
  - { kind: in-tree, path: src/platform/CMakeLists.txt, symbol: qor_platform, verified: 2026-11-16 }
  - { kind: in-tree, path: src/platform/platform.cpp, symbol: ThePlatform, verified: 2026-11-16 }
  - { kind: in-tree, path: src/platform/isubsystem.h, symbol: ISubsystem, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_platform --parallel 8
---

# qor_platform

Umbrella of arch + os + compiler; the base the rest of QOR stands on (`docs/agent/purposes.md`). What
the compiled library actually is: a registry of OS subsystems.

- One singleton, `qor::platform::Platform`, holding a `std::map<GUID, ref_of<ISubsystem>>` with
  `Setup()` over the map in insertion order and `Shutdown()` over it in reverse
  (`src/platform/platform.cpp#ThePlatform`); a subsystem added after `Setup()` is set up immediately,
  which is what makes late registration legal. `ISubsystem` is three virtuals with empty bodies and a
  deleted copy (`src/platform/isubsystem.h#ISubsystem`).
- Two translation units, built SHARED: `platform.cpp` (the registry) and `qor_platform.cpp`
  (the `ThisModule()` identity string every QOR library carries) (`src/platform/CMakeLists.txt`).
- The "umbrella" is the directory, not the dependency: `src/platform/CMakeLists.txt` ends with
  `add_subdirectory(compiler)`, `(architecture)`, `(os)` - 73 target declarations sit under it, 70 of
  them under `os/` - but the library links only `qor_module qor_host qor_sync qor_thread`
  (`src/platform/CMakeLists.txt`). It links neither `qor_arch` nor `qor_compiler`, the other two
  libs in its own directory.

## Failure modes

- It is the widest platform dependency in the tree: 68 `CMakeLists.txt` files name it in a link line,
  from `qor_application` (`src/framework/app/application/CMakeLists.txt`) to
  `qor_socketconnector` (`src/components/io/pipeline/connectors/socketconnector/CMakeLists.txt`), and
  `ThePlatform()` has 70 matches in the sources, `qor::app::Application` among its callers
  (`src/framework/app/application/application.h`). Changing its header rebuilds the framework.
- The `QOR_PLATFORM` private definition gates nothing. `qor_pp_module_interface( _X )` expands to
  `qor_pp_if(qor_pp_is_empty(_X), qor_pp_import, qor_pp_export)`
  (`src/configuration/configuration.h#qor_pp_module_interface`): the token only has to be non-empty, so
  the name carries no switch and every one of the 8 `QOR_PLATFORM` sites compiles to
  `__declspec(dllexport)` (`src/platform/compiler/msvc/msvc.h`). A green judge therefore proves the
  registry compiles, not that export/import is being chosen per library.
