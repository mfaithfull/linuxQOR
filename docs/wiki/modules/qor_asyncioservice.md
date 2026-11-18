---
Type: WikiPage
PageType: module
Title: "qor_asyncioservice"
Target: qor_asyncioservice
Status: current
LastVerified: 2026-11-16
Claims: 10
Summary: "Async IO service, the portable half of an IOCP/io_uring split; its exported ImplementsAsyncIOService() has no definition, and Service never initialises its context count."
Anchors:
  - { kind: in-tree, path: src/framework/io/async, symbol: qor_asyncioservice, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/io/async/CMakeLists.txt, symbol: qor_asyncioservice, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/io/async/service.h, symbol: Service, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/io/async/context.h, symbol: Context, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/io/async/service.cpp, symbol: GetSession, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/io/async/eventprocessor.h, symbol: Run, verified: 2026-11-16 }
Judges:
  - cmake --build cmakebuild --target qor_asyncioservice --parallel 8
---

# qor_asyncioservice

"Provides asynchronous IO through platform layer implementations. IO is initiated by applications and
waited on by background IO Tasks running on the thread pool." (`src/framework/io/async/purpose.md`,
compressed at `docs/agent/purposes.md:47`). Built SHARED from `qor_asyncioservice.cpp service.cpp
initiator.cpp` at 0.5.0; link line `qor_module qor_host qor_injection qor_current qor_thread
qor_interception qor_sync qor_issue qor_error qor_log` (`src/framework/io/async/CMakeLists.txt:14`),
copied token for token by `docs/agent/repo_map.md:60` and placed at layer 8 of its build order
(L139). 23 of the 281 `CMakeLists.txt` under `src`, `test` and `examples` name it.

- It is the portable half of a three-target split. `winqor_asyncioservice`
  (`src/platform/os/windows/io/async/CMakeLists.txt:2`, completion ports) and `linqor_asyncioservice`
  (`src/platform/os/linux/io/async/CMakeLists.txt:1`, io_uring) derive `Service`, `EventProcessor` and
  `Initiator` and register all six through `TypeRegEntry` statics inside `ThisModule()`
  (`src/platform/os/windows/io/async/winqor_asyncioservice.cpp:22-24`), which is what the
  `ExternalFactory` declarations on the base classes (`initiator.h:42`, `eventprocessor.h:53`) exist
  to receive. Neither OS sibling appears anywhere in `docs/agent/repo_map.md` - the word
  `asyncioservice` matches only this target's two lines - though 8 `CMakeLists.txt` link them.
- The wiring is constructor-driven: `Context::Context` does `new_ref<Initiator>()`, asks it
  `RequiresBackgroundProcessor()`, news an `EventProcessor` if so, connects the two and calls
  `Inflate()` (`context.h:32-41`); `Inflate` submits the processor's `Run()` to the ThreadPool, where
  the pool worker renames itself "IO Thread" (`context.h:50-54`), and `Deflate` sets the stop flag and
  joins through the stored `std::future` (`context.h:58-65`).
- `Service::Setup` sizes its context array from the pool - `PoolInstancer::GetPoolSize<Context>()`,
  the instancer `Context` registered for at `context.h:209` - news one `ref_of<Context>` per slot, and
  `GetSession` round-robins through them handing out `Session` wrappers
  (`service.cpp:23-50`). `Service` itself is a Role feature: SingletonInstancer, ExternalFactory, its
  own GUID (`service.h:54-57`), fetched by name at the consumers:
  `GetFeature<async::Service>()->GetSession()` (`src/components/io/network/server/netserver.cpp:38`),
  `AddFeature<async::Service>` in the filesystem test's role
  (`test/filesystem/test_main.cpp:38`).
- `QOR_ASYNCIOSERVICE` has 3 hits in 2 files (`initiator.h:20`, `service.h:23` and `:27`) - the first
  of the service.h pair is the free function `ImplementsAsyncIOService()` in namespace `qor`. The
  folder's other token is borrowed: `interface.h:12` forward-declares `qor::io::network::Socket`
  marked with `QOR_SOCKETS`, another target's gate.
- `Session` is the synchronisation story: a nested class of `Context` deriving `Interface`
  (`context.h:125`), whose comment reads "Provides synchronisaed access to a shared IO Context"
  (sic, `context.h:124`), holding a `Mutex` member of the Context as friend and wrapping all nine
  verbs as `Lock lock(m_sharedContext.m_access); return m_sharedContext.X(...);`
  (`context.h:133-178`).

## Failure modes

- `ImplementsAsyncIOService()` is declared and exported and never exists. It has exactly 1 hit across
  `src`, `test` and `examples` - the declaration at `service.h:23` - no definition in this target, no
  definition in the two OS siblings, and no caller anywhere. The predicate the design wanted ("does
  this platform actually implement async IO?") is answered instead at the far end of the pipeline:
  nine `fatal(Error_empty_base_called)` calls, eight in the `Initiator` verbs
  (`initiator.cpp:18-72`) and one in `EventProcessor::Run` (`eventprocessor.h:33`).
- `m_contextCount` is never initialised. `Service::Service()` sets `m_contextArray` and
  `m_contextIndex` but not the `unsigned m_contextCount;` that has no default initialiser
  (`service.cpp:15-19`, `service.h:47`), and both readers run without a `Setup()` guard:
  `Shutdown()` loops `context < m_contextCount` dereferencing the array and `GetSession()` takes
  `% m_contextCount` (`service.cpp:36-49`). `Shutdown()` without `Setup()` on a Debug build walks a
  null array a garbage number of times; `GetSession()` divides by garbage. And `GetSession()`
  increments `m_contextIndex` unguarded - `Service` has no mutex member - so the round-robin the
  `Session` wrapper exists to make safe races on the index that selects it.
- `Session::Send` is not the override it looks like. The pure virtual is
  `Send(io::Descriptor*, const byte*, const size_t, const int) const` (`interface.h:29`); `Context`
  matches it (`context.h:97`); `Session` declares `Send(io::Descriptor*, byte*, size_t, int) const`
  with `byte` = `uint8_t` (`context.h:163`, `src/platform/compiler/compiler.h:40`) - an overload, not
  an override, so it is the one of the nine locked verbs that does not fill the vtable slot it sits in.
- With no platform registration the base classes do not fail fast, they fail spinning. The base
  `Initiator::RequiresBackgroundProcessor()` returns `true` (`initiator.cpp:16`), so a `Context` whose
  `new_ref<Initiator>()` fell through to the base also news a base `EventProcessor`, and `Inflate()`
  posts its `Run()` to the thread pool, where it issues `fatal(Error_empty_base_called)` and reaches
  `while(!m_StopRequested) {}` - a busy spin on a pool thread renamed "IO Thread"
  (`eventprocessor.h:30-38`, `context.h:50-54`). The same class holds `bool m_freeRun{false};` with 1
  hit tree-wide, never read, never written (`eventprocessor.h:47`).
- The index orders these two targets backwards. `qor_sockets` links `qor_asyncioservice`
  (`src/framework/io/network/CMakeLists.txt:15`), while this target reaches into `qor_sockets`'
  headers - `src/framework/io/network/address.h` at `service.h:16`, `isocket.h` at `context.h:19` -
  without naming it in `target_link_libraries` (`CMakeLists.txt:14`). It links because only headers are
  needed and neither calls into the other; `docs/agent/repo_map.md` presents it as a strict order,
  layer 7 for `qor_sockets` (L138), layer 8 for this one (L139).

## See also

[[qor_sockets]] [[qor_pipeline]] [[qor_role]] [[qor_log]] [[qor_module]]

