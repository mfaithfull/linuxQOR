---
Type: WikiPage
PageType: module
Title: "qor_current"
Target: qor_current
Status: current
LastVerified: 2026-11-16
Claims: 4
Summary: "Current-thread and current-process abstractions; the platform supplies the implementation."
Anchors:
  - { kind: in-tree, path: src/qor/essentials/current, symbol: qor_current, verified: 2026-11-16 }
  - { kind: in-tree, path: src/qor/essentials/current/CMakeLists.txt, symbol: qor_current, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_current --parallel 8
---

# qor_current

Abstractions of the current thread and the current process, with the implementation supplied per
platform under `src/platform/os` (`docs/agent/purposes.md`).

- It is linked even by single-threaded code, because Flyers bind their data to the current thread
  (`docs/agent/purposes.md`).
- Its QOR dependencies are `qor_module`, `qor_host` and `qor_injection`, listed on one
  `target_link_libraries` line (`src/qor/essentials/current/CMakeLists.txt`).
- The context it owns lives in `detail/threadcontext.cpp` beside `currentthread.cpp`, both listed in
  the same `add_library` (`src/qor/essentials/current/CMakeLists.txt`).

## Failure modes

- `find_package(Threads REQUIRED)` runs before any target is declared, so a toolchain that cannot
  find Threads fails at configure time, not at compile time
  (`src/qor/essentials/current/CMakeLists.txt`).
- Like every target that includes it, it dies on the buildnumber rule when the cache has no
  `BUILD_NUMBER` (`cmake/updatebuildnumber.cmake`).
