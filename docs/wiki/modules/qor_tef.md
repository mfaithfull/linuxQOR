---
Type: WikiPage
PageType: module
Title: "qor_tef"
Target: qor_tef
Status: current
LastVerified: 2026-11-16
Claims: 4
Summary: "Type-erased functions ported from magic_func; qor_pp_make_function yields a callable object."
Anchors:
  - { kind: in-tree, path: src/qor/function, symbol: qor_tef, verified: 2026-11-16 }
  - { kind: in-tree, path: src/qor/function/CMakeLists.txt, symbol: qor_tef, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_tef --parallel 8
---

# qor_tef

Type-erased functions (port of magic_func); `qor_pp_make_function` yields a callable type-erased
object, at about twice `std::function` call performance (`docs/agent/purposes.md`).

- Two translation units, `qor_tef.cpp` and `allocator.cpp`, built SHARED, with `QOR_TEF` and the
  project version compiled in as private definitions (`src/qor/function/CMakeLists.txt`).
- Its QOR dependencies are `qor_module`, `qor_host` and `qor_injection`
  (`src/qor/function/CMakeLists.txt`) - the type-erased function object is built on the memory
  subsystem's dependency injection, not beside it.
- `project(QOR VERSION 1.0.1 ...)` heads the file: each of the 17 core targets declares `QOR` with
  its own version, and this is the highest of them.

## Failure modes

- The folder is `src/qor/function` and the target is `qor_tef` - the only one of the core 17 whose
  leaf directory shares no token with the target name, so a diff-to-page map keyed on the folder
  name misses it and lands in `function/` instead (`src/qor/function/CMakeLists.txt`).
- A green judge does not mean it was tested: `QORTest` does not link `qor_tef`
  (`test/CMakeLists.txt`). The binary that does is `QORTestTEF` (`test/tef/CMakeLists.txt`), added
  by `add_subdirectory(tef)` (`test/CMakeLists.txt`), so running the tests means building a second
  target the judge does not name.
