---
Type: WikiPage
PageType: module
Title: "qor_threadmemory"
Target: qor_threadmemory
Status: current
LastVerified: 2026-11-16
Claims: 5
Summary: "Thread-aware memory subsystem: fastheap, threadheap and smallobjectheap, one heap per thread."
Anchors:
  - { kind: in-tree, path: src/qor/memory/thread, symbol: qor_threadmemory, verified: 2026-11-16 }
  - { kind: in-tree, path: src/qor/memory/thread/CMakeLists.txt, symbol: qor_threadmemory, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_threadmemory --parallel 8
---

# qor_threadmemory

Thread-aware memory subsystem (`docs/agent/purposes.md`), the allocator layer that keeps a heap per
thread.

- Eight translation units in three named families - `fastheap/` (stackpage, fastbucket, fastheap),
  `threadheap/threadheap.cpp`, `smallobjectheap/` (smallobjectpage, smallobjectbucket,
  smallobjectheap) beside `qor_threadmemory.cpp` (`src/qor/memory/thread/CMakeLists.txt`).
- Its QOR dependencies are `qor_module`, `qor_host`, `qor_current`, `qor_thread` and `qor_sync`
  (`src/qor/memory/thread/CMakeLists.txt`).
- Three of those five are not in `src/qor`: `qor_thread` and `qor_sync` are declared in
  `src/framework/parallel/` and `qor_host` in `src/framework/app/host/`, so a memory subsystem sits
  on top of the parallelism layer rather than below it (`src/framework/parallel/thread/CMakeLists.txt`).
- `QOR_THREADMEMORY` is the gate that names it, and it is the busiest of this slice's defines: 12
  matches over `src` and `include` on 16 Nov 2026, against 2 for `QOR_TEF`.

## Failure modes

- The target is `qor_threadmemory` in the folder `src/qor/memory/thread`: the second case of a folder
  whose name is not the target's, so `thread` on its own resolves to neither the sibling `qor_thread`
  nor this lib (`src/qor/memory/thread/CMakeLists.txt`).
- `project(QOR VERSION 0.0.1 ...)` is the floor of the 17 declarations, and each target stamps its
  own version into its own build-number header: the tree has no single answer to "which QOR", and
  0.0.1 here is not a stale copy of 1.0.1 over there (`src/qor/memory/thread/CMakeLists.txt`).
- Seven of `QORTest`'s sources are `components/threadmemory/test_*.cpp`
  (`test/CMakeLists.txt`), so this is one of the core targets whose judge and whose test binary move
  together - a build that stays green while `QORTest` fails on a heap page is this lib's failure, not
  the test's.
