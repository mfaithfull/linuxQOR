---
Type: WikiPage
PageType: module
Title: "qor_module"
Target: qor_module
Status: current
LastVerified: 2026-11-16
Claims: 4
Summary: "Base static lib linked into every QOR module; carries the module tag and static-library chaining."
Anchors:
  - { kind: in-tree, path: src/qor/module, symbol: qor_module, verified: 2026-11-16 }
  - { kind: in-tree, path: src/qor/module/CMakeLists.txt, symbol: qor_module, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_module --parallel 8
---

# qor_module

Base static lib linked into EVERY QOR module: it enables static-library chaining and the module
stitching system, the module tag (`docs/agent/purposes.md`).

- Every executable links `qor_module` and provides a `ThisModule` implementation, so a link error
  naming `ThisModule` is a missing implementation, not a missing dependency
  (`memory-bank/systemPatterns.md`).
- It is the only target of the core ten built STATIC: `add_library` names no SHARED keyword
  (`src/qor/module/CMakeLists.txt`).
- It carries no QOR dependency of its own - the file holds no `target_link_libraries` line - which is
  why everything else can reach it (`src/qor/module/CMakeLists.txt`).

## Failure modes

- The `updatebuildnumber.cmake` include runs a rule that needs `BUILD_NUMBER` and
  `BUILDNUMBER_HEADER` in the CMake cache. A tree configured without them fails here first, and
  fails every other target that includes the same file (`cmake/updatebuildnumber.cmake`); the
  measured split is `build` without the cache entry (red) against `cmakebuild` with it (green).
