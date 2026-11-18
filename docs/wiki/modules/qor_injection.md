---
Type: WikiPage
PageType: module
Title: "qor_injection"
Target: qor_injection
Status: current
LastVerified: 2026-11-16
Claims: 4
Summary: "Dependency injection for the memory subsystem; the shortest dependency line of the core 17."
Anchors:
  - { kind: in-tree, path: src/qor/memory/injection, symbol: qor_injection, verified: 2026-11-16 }
  - { kind: in-tree, path: src/qor/memory/injection/CMakeLists.txt, symbol: qor_injection, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_injection --parallel 8
---

# qor_injection

Dependency injection, in the memory subsystem (`docs/agent/purposes.md`).

- Two translation units, `qor_injection.cpp` and `typeregistry.cpp`, built SHARED, defining
  `QOR_INJECTION` and the project version privately (`src/qor/memory/injection/CMakeLists.txt`).
- Its link line names `qor_module` and `qor_host` alone
  (`src/qor/memory/injection/CMakeLists.txt`) - the shortest dependency line of the core 17, which
  is what lets it sit under `qor_tef` without a cycle.
- It is one of eight subdirectories of `src/qor/memory` (`allocators`, `exceptions`, `factory`,
  `injection`, `instance`, `reference`, `sources`, `thread`), so the folder name is the only thing
  that tells its page from its siblings' (`src/qor/memory`).

## Failure modes

- The registry is the thing that changes: `typeregistry.cpp` is one of only two TUs, and the type
  registry it builds is what `qor_tef` and the JSON/parser components reach through
  (`src/qor/function/CMakeLists.txt`), so a signature change here rebuilds far above the memory
  layer the name suggests.
- It is genuinely covered, unlike its neighbours here: `injection/test_injection.cpp` is in
  `QORTest`'s source list and the lib is linked on both platform branches
  (`test/CMakeLists.txt`) - so a green build plus a red `QORTest` points at this lib, not at the
  judge.
