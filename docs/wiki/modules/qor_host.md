---
Type: WikiPage
PageType: module
Title: "qor_host"
Target: qor_host
Status: current
LastVerified: 2026-11-19
Claims: 10
Summary: "Host-process module registry singleton; the widest shared-library dependency in the tree, and its cycle with qor_module is suppressed rather than broken."
Sources:
  - hook-25c080074a-qor_host
Anchors:
  - { kind: in-tree, path: src/framework/app/host, symbol: qor_host, verified: 2026-11-19 }
  - { kind: in-tree, path: src/framework/app/host/CMakeLists.txt, symbol: qor_host, verified: 2026-11-19 }
  - { kind: in-tree, path: src/framework/app/host/host.h, symbol: Host, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/host/host.cpp, symbol: TheHost, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/host/host_module.cpp, symbol: ThisModule, verified: 2026-11-16 }
  - { kind: in-tree, path: src/qor/module/module.cpp, symbol: TheHost, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/host/purpose.md, symbol: Registry, verified: 2026-11-19 }
  - { kind: in-tree, path: src/framework/app/host/purpose.md, anchor: commit:25c080074a4fc59cb06100c76fb8be5255907953, verified: 2026-11-19 }
Judges:
  - cmake --build cmakebuild --target qor_host --parallel 8
---

# qor_host

"the center of the module stitching system which ties the QOR modules together. It provides the one
and only Module Registry" (`src/framework/app/host/purpose.md` - the registry sentence and the fix
of the file's "representing a the host process singleton" typo both arrived at commit `25c080074a`):
one process-wide `qor::Host` holding one `ModuleRegistry`.
Built SHARED from `host.cpp` and `host_module.cpp`; the folder carries one public header
(`src/framework/app/host/CMakeLists.txt`).

- Second only to `qor_module` in fan-in: 172 of the 281 `CMakeLists.txt` under `src`, `test` and
  `examples` name `qor_host` (190 lines; `qor_module` is named by 222 files), and
  `docs/agent/repo_map.md` opens its build order with `1. qor_host -> qor_module`.
- The link line is one `target_link_libraries` per OS branch: Windows takes
  `qor_module winqor_sharedbootstrap`, Linux takes `qor_module` alone
  (`src/framework/app/host/CMakeLists.txt`) - which is exactly that repo_map row,
  `-> qor_module (win: winqor_sharedbootstrap)`. There is no per-OS sibling target: 0 `CMakeLists.txt`
  in `src` declares `linqor_host` or `winqor_host`, unlike the three-way split of
  [[qor_dynamiclibrary]].
- The gate token is `QOR_PP_HOST`, not `QOR_HOST` - 2 hits in 1 file, both in `host.h`, defined PRIVATE
  by its own CMakeLists. Of the 165 `target_compile_definitions(${target} PRIVATE ...)` lines in `src`
  it is one of only two still opening with a `QOR_PP_`-prefixed token; the other is `QOR_PP_ARCH`
  (`src/platform/architecture/CMakeLists.txt`).
- `host.cpp` guards `#include <buildnumber.h>` with `#ifdef NDEBUG` and spells the token right, which
  is what makes the version it registers honest: `src/macros/moduleversion.hpp` defines
  `qor_pp_module_ver_string` as `major.minor.patch.buildnumber` under `NDEBUG` and as
  `major.minor.patch` without it, so a Debug build of every module is unstamped by design
  (`src/framework/app/host/host.cpp`).
- It registers as `qor::Module("Querysoft Open Runtime: Host Module", qor_pp_module_ver_string, false)`
  (`src/framework/app/host/host.cpp`) against `project(QOR VERSION 0.5.0 ...)`
  (`src/framework/app/host/CMakeLists.txt`). That line is one of 65 `project(QOR VERSION` lines in the
  tree carrying 9 distinct values, so the number is the folder's, not the repository's -
  `src/qor/module/CMakeLists.txt` says 1.0.0.
- `TheHost()` has 9 mentions across `src`, `test` and `examples` in 4 files: declared in `host.h`,
  defined in `host.cpp`, hand-declared in `src/qor/module/module.cpp`, and used by exactly one consumer
  outside the mechanism - `examples/basic/outline/main.cpp` calls
  `TheHost()->Modules()->VisitModules`. No test touches it: `test/framework/host.cpp`, compiled into
  `QORTest` at `test/CMakeLists.txt`, reaches the same object through `Host::Instance()`.

## Failure modes

- The cycle with [[qor_module]] is suppressed, not broken. `src/qor/module/module.cpp` declares
  `qor_pp_import Module* TheHost();` itself and calls it from the `Module` constructor and destructor,
  while `src/qor/module/CMakeLists.txt` holds no `target_link_libraries` line at all - so `qor_module`
  never links this target and the call is satisfied only by a binary that links both. Building the host
  resolves `module.cpp`'s call against a definition in this same target, which is the warning
  `src/framework/app/host/CMakeLists.txt` silences with
  `target_link_options(${target} PRIVATE "/ignore:4217")` - the file's only link option, and the one
  `module.cpp`'s own comment describes: "we'll get a linker warning when, and only when, we link this
  file into qor_host which implements TheHost".
- The null guards on that call cannot fire. `TheHost()` returns `&(ThisModule())`, the address of a
  function-local static (`src/framework/app/host/host.cpp`, `src/framework/app/host/host_module.cpp`),
  so `if (host)` in `src/qor/module/module.cpp` is always true, and constructing any `Module` forces
  the host into existence. `Host::Host()` passing `false` as `Module`'s third argument is the only
  thing preventing that from recursing (`src/framework/app/host/host.cpp`): the default `true` would
  make `Module::Module` call `TheHost()` while `ThisModule()` is still constructing the object that
  call returns.
- `Module::RegisterModule` and `Module::UnregisterModule` are virtual with empty bodies, not pure
  (`src/qor/module/module.h`). `Host` overrides both, but `TheHost()` hands out a plain `Module*`, so a
  second host-shaped `Module` that forgot the override would drop every registration silently and
  nothing at the call site could tell.
- `docs/agent/repo_map.md` places this target in two rows of its own numbered build order -
  `1. qor_host -> qor_module` and again at row 10, `qor_host (in framework, part of every top-level
  exe)` - so the index the page-leaves cite for a target's dependencies both opens with the target and
  lists it near the end.

## See also

[[qor_module]] [[qor_application]] [[qor_dynamiclibrary]] [[qor_arch]]
