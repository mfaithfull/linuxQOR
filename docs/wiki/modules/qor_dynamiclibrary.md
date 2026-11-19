---
Type: WikiPage
PageType: module
Title: "qor_dynamiclibrary"
Target: qor_dynamiclibrary
Status: current
LastVerified: 2026-11-19
Claims: 7
Summary: "Dynamic library loading; the gate token is misspelled and every user copies the misspelling."
Sources:
  - hook-25c080074a-qor_dynamiclibrary
Anchors:
  - { kind: in-tree, path: src/framework/app/dynamiclibrary, symbol: qor_dynamiclibrary, verified: 2026-11-19 }
  - { kind: in-tree, path: src/framework/app/dynamiclibrary/CMakeLists.txt, symbol: qor_dynamiclibrary, verified: 2026-11-19 }
  - { kind: in-tree, path: src/framework/app/dynamiclibrary/dynamiclibrary.h, symbol: DynamicLibrary, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/app/dynamiclibrary/purpose.md, symbol: agnostic, verified: 2026-11-19 }
  - { kind: in-tree, path: src/framework/app/dynamiclibrary/purpose.md, anchor: commit:25c080074a4fc59cb06100c76fb8be5255907953, verified: 2026-11-19 }
Judges:
  - cmake --build cmakebuild --target qor_dynamiclibrary --parallel 8
---

# qor_dynamiclibrary

Dynamic library loading support (`docs/agent/purposes.md`); the folder said it itself from commit
`25c080074a` on: "Provides platform agnostic dynamic library access at runtime. DLLs on Windows, SOs
on Linux" - for "libraries loaded conditionally when the application is already running, not QOR
modules which are implicitly linked and automatically loaded at process startup before main is
reached" (`src/framework/app/dynamiclibrary/purpose.md`). Built SHARED at version 0.0.5 from
`qor_dynamiclibrary.cpp` and `dynamiclibrary.cpp` (`src/framework/app/dynamiclibrary/CMakeLists.txt`).

- Its Windows link line is `qor_module qor_host qor_thread winqor_sharedbootstrap winqor_thread` and
  its Linux one swaps the last two for `linqor_thread`
  (`src/framework/app/dynamiclibrary/CMakeLists.txt`); both thread targets are declared under
  `src/platform/os/<os>/app/thread/CMakeLists.txt`.
- The gate token is `QOR_DYNAMICLIBARY` - no `R` before the `Y` - defined by its own CMakeLists and
  used by the only header that exports (`src/framework/app/dynamiclibrary/dynamiclibrary.h`). Since
  both sides spell it the same, the export works; the tree never spells it `QOR_DYNAMICLIBRARY`
  (0 occurrences in any `CMakeLists.txt`).
- Nothing outside a directory named `dynamiclibrary` links it: 4 CMakeLists mention it, 0 of them
  outside (`src/framework/app/dynamiclibrary/CMakeLists.txt`).
- Its per-OS siblings declare different target names - `linqor_dynamiclibrary`
  (`src/platform/os/linux/app/dynamiclibrary/CMakeLists.txt`) and `winqor_dynamiclibrary`
  (`src/platform/os/windows/app/dynamiclibrary/CMakeLists.txt`) - so this is one of three targets in
  three same-named folders.
- `ThisModule()` is the module object, constructed with the string "Querysoft Open Runtime: Dynamic
  Library Module" (`src/framework/app/dynamiclibrary/qor_dynamiclibrary.cpp`).

## Failure modes

- `qor_dynamiclibrary.cpp` guards `#include <buildnumber.h>` with `#ifdef NDBEUG` - the token is
  transposed, so the include is dead and the version string the module registers is never stamped
  (`src/framework/app/dynamiclibrary/qor_dynamiclibrary.cpp`; `NDBEUG` appears 4 times in the tree
  against 155 correct `NDEBUG` uses).
- `ImplementsDynamicLibrary()` is declared as the module's capability probe and is never defined or
  called: 1 occurrence in the whole tree, the declaration in
  `src/framework/app/dynamiclibrary/dynamiclibrary.h`. A `qor_pp_module_requires(DynamicLibrary)`
  would not link.

## See also

[[qor_host]] [[qor_module]]
