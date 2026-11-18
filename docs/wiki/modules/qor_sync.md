---
Type: WikiPage
PageType: module
Title: "qor_sync"
Target: qor_sync
Status: current
LastVerified: 2026-11-17
Claims: 13
Summary: "Curated std sync primitives; its sync_of trait decides the lock behind every shared reference and defaults to a no-op, and its Key class is never constructed."
Anchors:
  - { kind: in-tree, path: src/framework/parallel/sync, symbol: qor_sync, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/parallel/sync/CMakeLists.txt, symbol: qor_sync, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/parallel/sync/sync.h, symbol: sync_of, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/parallel/sync/lock.h, symbol: Key, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/parallel/sync/syncobject.h, symbol: SyncObject, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/parallel/sync/mutex.h, symbol: Mutex, verified: 2026-11-17 }
Judges:
  - cmake --build cmakebuild --target qor_sync --parallel 8
---

# qor_sync

"Curated std sync primitives w/ coherent interfaces (choice of primitive transparent to caller);
scoped lock as \"the one true lock\" + scoped key class to invert." (`docs/agent/purposes.md:50`).
Built SHARED from nine translation units at version 0.5.0
(`src/framework/parallel/sync/CMakeLists.txt:4-6`).

- 54 of the 281 `CMakeLists.txt` under `src`, `test` and `examples` name it. `docs/agent/repo_map.md:63`
  gives `-> qor_module qor_host`, which `CMakeLists.txt:16` copies exactly, and its build order puts the
  target in the third tier (`L134: 3. qor_sync qor_thread qor_reporting qor_interception qor_issue ->
  (1,2)`) - and 26 of the file's other dependency rows list it (L45-L122).
- 20 headers, 9 `.cpp`: eleven headers carry no translation unit at all. That is the shape the purpose
  line asks for (a `Mutex` is a header plus `mutex.cpp`, a `NullSection` is a header), but it makes the
  folder's include surface far wider than its link surface - see the two headers nobody includes below.
- `QOR_SYNC` has 18 hits in 6 files (`mutex.h` 6, `asyncmutex.h` 4, `asyncautoresetevent.h` 3,
  `asyncmanualresetevent.h` 3, `lightweightmanualresetevent.h` 1, `recursive_mutex.h` 1). The folder
  uses 17 plain `qor_pp_module_interface(...)` markers and one `qor_pp_module_interface_gcc(...)`
  (`mutex.h:12`), and `configuration.h:43` defines the `_gcc` variant to **nothing** on any toolchain
  that is not GCC or clang - so on this MSVC build the one class-level marker in the folder compiles
  away and only the member markers survive (`cmake/flags.cmake:16` sets
  `CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF`, so nothing else fills the gap).
- The folder's real customer is `qor`'s reference counting, one layer below it: `sharedref.h` holds
  `mutable typename sync_of< R >::type m_Section;` (`src/qor/memory/reference/sharedref.h:248`) and
  asks `LockIsRealInternal< typename sync_of<R>::type >` whether the section does anything
  (`sharedref.h:147`). `sync_of` is declared here (`sync.h:14-18`), and its primary template resolves
  to `NullSection`, whose `Acquire()`/`Release()` are empty (`nullsection.h:17-18`).
- Three types arm it by hand: `qor_pp_declare_sync_of(Application, RecursiveMutex)`
  (`src/framework/app/application/application.h:116`), `arch::CPU`
  (`src/platform/architecture/cpu.h:34`) and `platform::Platform` (`src/platform/platform.h:55`).
- `Lock` is the scoped lock the purpose line names: `final`, holds `SyncObject&`, `Acquire()`s in the
  constructor and `Release()`s in the destructor (`lock.h:11-33`). Exactly three files include it by
  path, all one layer *below* the target: `src/qor/memory/instance/pool.h:12` (4 uses - `:35`, `:41`,
  `:69`, `:96`), `singleton.h:14` (`:43`, `:51`, `:73`) and `threadsingleton.h:12`.
- `SyncObject` is the whole polymorphic surface: `Acquire()`/`Release()` pure, `IsLocked()` virtual
  returning `false` (`syncobject.h:17-19`), and it carries no export marker, nor do `Lock`, `Key`,
  `NullSection` or `NullLock`. One file outside the folder names it:
  `src/framework/data/pipeline/plug.h:40-41` exports a virtual `GetSyncObject()`/`SetSyncObject()`
  pair over a raw `SyncObject* m_syncobject` (`plug.h:48`).
- The async primitives are tested from the other library's suite: the five `async*_tests.cpp` files
  that exercise them are compiled into QORTest from `test/CMakeLists.txt:38-42,50,52`, and every one of
  them sits in `test/framework/task/`. Nothing in `test/` is named for this folder, and none of them
  reaches `Lock` or `Key`.
- `Mutex` wraps `std::mutex` behind the base (`mutex.h:26`) and keeps its own
  `bool m_Locked{ false }` (`mutex.h:25`), set inside `Acquire()`/`Release()`
  (`mutex.cpp:13-23`). `RecursiveMutex` is the same shape over `std::recursive_mutex`
  (`recursive_mutex.h`), and it is the type all three `sync_of` arms pick.

## Failure modes

- The interface's only query is a race, and the reference counter calls it. `SyncObject` answers
  `IsLocked()` with `false` for any derived class that does not override (`syncobject.h:19`). `Mutex`
  answers from `bool m_Locked{ false }` (`mutex.h:25`), set *after* `m_Impl.lock()` and cleared *before*
  `m_Impl.unlock()` (`mutex.cpp:13-23`); `RecursiveMutex` answers `m_Locked > 0` from a plain `size_t`
  (`recursive_mutex.h:24`, `recursive_mutex.cpp:18-27`). Neither is atomic, and both leave a window
  where the flag says "free" while the OS lock is still held. `sharedref.h:153` publishes it with the
  comment "Not reliable, just a guide for warning about unlocked access", and `ref.h:153`, `:189`,
  `:204` each gate the unlocked-access diagnostic on `m_p->LockIsReal() && !m_p->IsLocked()` - so the
  code that detects unsynchronised access performs an unsynchronised read to do it.
- `Key`, the second half of the purpose line, is never constructed. A `Key x(` shaped search over
  `src`, `test` and `examples` returns 2 hits, both prose about keyboard keys
  (`src/components/ui/terminal/ui/inputparser.cpp:54`,
  `src/platform/os/windows/api_layer/user/keyboard.cpp:495`). It is also the sole `friend` of the
  `final` `Lock` (`lock.h:13`) purely to reach the private reference, and the pair is unguarded: two
  `Key`s built from one `Lock` call `Release()` twice on the same object (`lock.h:39-42`), `Key` keeps
  no state to notice, and `SyncObject` exposes no state to ask (`syncobject.h:17-18`).
- The lock behind every shared reference defaults to none, and one of the two ways out of that default
  does not mention locking. `sync_of`'s primary template is `NullSection` (`sync.h:14-18`), whose
  `Acquire()`/`Release()` have empty bodies (`nullsection.h:17-18`), and `sharedref.h:248`
  instantiates it as `m_Section` for every `ref_of<R>`. Three types arm it by hand
  (`application.h:116`, `src/platform/architecture/cpu.h:34`, `src/platform/platform.h:55`). The other
  route is `qor_pp_declare_singleton`, whose expansion specialises `sync_of< _CLASS >` to
  `RecursiveMutex` as a side effect of naming a class a singleton (`singleton.h:18-28`) - its 4 users
  are all examples (`httpserverapp.h:50`, `optionsapp.h:38`, `rdapp.h:27`, `server.h:23`), so nothing in
  `src` is armed that way, and a type that used the singleton macro silently gains a recursive mutex.
- Two primitives are built but unreachable, and two headers are dead. `spinmutex.h` and `spinwait.h`
  have 0 path-includers tree-wide - each is reached only from its own `.cpp` - and neither declares a
  marker, while `cmake/flags.cmake:16` sets `CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF`: `SpinMutex` is
  compiled into the DLL (`CMakeLists.txt:6`) with nothing exported, so no other module can link it.
  `asyncscope.h` and `nulllock.h` have 0 includers anywhere, and `NullLock`'s body -
  `NullLock(SyncObject& SyncObject) {}` (`nulllock.h:16`) - is what `NullSection` already is.

## See also

[[qor_threadmemory]] [[qor_module]] [[qor_pipeline]] [[qor_asyncioservice]]
