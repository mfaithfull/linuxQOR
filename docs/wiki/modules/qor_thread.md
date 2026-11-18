---
Type: WikiPage
PageType: module
Title: "qor_thread"
Target: qor_thread
Status: current
LastVerified: 2026-11-18
Claims: 11
Summary: "std::jthread wrapper plus a vendored MIT thread pool; the default constructor starts the thread before the object is a derived object."
Anchors:
  - { kind: in-tree, path: src/framework/parallel/thread, symbol: qor_thread, verified: 2026-11-18 }
  - { kind: in-tree, path: src/framework/parallel/thread/CMakeLists.txt, symbol: qor_thread, verified: 2026-11-18 }
  - { kind: in-tree, path: src/framework/parallel/thread/thread.h, symbol: Thread, verified: 2026-11-18 }
  - { kind: in-tree, path: src/framework/parallel/thread/thread.cpp, symbol: swap, verified: 2026-11-18 }
  - { kind: in-tree, path: src/framework/parallel/thread/threadpool.h, symbol: ThreadPool, verified: 2026-11-18 }
  - { kind: in-tree, path: src/platform/os/windows/app/thread/CMakeLists.txt, symbol: winqor_thread, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_thread --parallel 8
---

# qor_thread

"Platform-agnostic manual thread mgmt over std jthread; + Thread Pool feature (recommended even
alongside custom threads; asyncio/log-aggregation depend on it)." (`docs/agent/purposes.md:52`). Built
SHARED from `qor_thread.cpp thread.cpp threadpool.cpp` at version 0.5.0
(`src/framework/parallel/thread/CMakeLists.txt:4`).

- 106 of the 281 `CMakeLists.txt` under `src`, `test` and `examples` name it - third behind
  [[qor_module]] (222) and [[qor_host]] (172). The link line
  `qor_module qor_host qor_current qor_sync qor_injection` (`CMakeLists.txt:22`) is
  `docs/agent/repo_map.md:65` token for token.
- `find_package(Threads REQUIRED)` appears exactly twice in the whole tree - here (`CMakeLists.txt:6`)
  and at `src/qor/essentials/current/CMakeLists.txt:6` - the two `Threads` consumers of the repository,
  one of which links the other. The lines it feeds use the target literally where the rest of the file
  uses `${target}` (`CMakeLists.txt:8`, `:11`), and `-pthread` sits under
  `if(THREADS_HAVE_PTHREAD_ARG)`, which the MSVC generator never sets.
- `QOR_THREAD` has 40 hits in exactly 2 files, both of them this folder's (`thread.h` 16,
  `threadpool.h` 24) - the only module in this slice whose token is used nowhere else in `src`. Every
  class-level marker is `qor_pp_module_interface_gcc`, which `src/configuration/configuration.h:43`
  expands to nothing off GCC, so on the judge's toolchain the direction of every export is carried by
  the member markers alone.
- The target is two libraries of different vintage under one target name. `threadpool.h` is 39,894
  bytes against a 7,927-byte `threadpool.cpp` - 2nd largest header under `src/framework` after
  `app/argparse/argumentparser.h` (47,716) - with the queueing API inline: `PostBlocks`
  (`threadpool.h:118`), `PostTask` (`:196`), `WaitFor` (`:440`), `Schedule` with its function-local
  `Awaiter` (`:489-501`). 231 `ThreadPool` mentions in 59 files.
- `ThreadPool` is a Feature of the Role system: `class qor_pp_module_interface_gcc(QOR_THREAD)
  ThreadPool : public app::IFeature` (`threadpool.h:61`, whose `#include` is `:52`) and
  `TypeRegEntry<ThreadPool, ThreadPool>` (`qor_thread.cpp:20`). It is reached as a feature at 4 sites,
  and the two the purpose file promises are among them: `m_threadPool = m_Role->GetFeature<thread::ThreadPool>()`
  (`src/framework/io/async/service.cpp:25`) and the same call in
  `src/components/qor/logaggregator/logaggregator.cpp:45`.
- The per-OS siblings of the same name are a different abstraction.
  `src/platform/os/windows/app/thread/CMakeLists.txt` is "QOR Windows **Current Thread** Module" (L1),
  target `winqor_thread` (L2), sources `winqor_thread.cpp currentthread.cpp heap.cpp` (L4), gate token
  `QOR_WINDOWSTHREAD` plus `qor_pp_implements_icurrentthread` (L10-14), implementing
  `ImplementsICurrentThread()` (`currentthread.cpp:19`) and `class CurrentThread : public
  qor::ICurrentThread` (`currentthread.h:18`). `qor_thread` links neither sibling, and
  `docs/agent/repo_map.md:65` says nothing about them - where the two rows that do have per-OS parts
  do say so (`:50` `(win|lin)qor_*`, `:78` `(win: winqor_sharedbootstrap)`).

## Failure modes

- The default constructor starts the thread before the object is a derived object.
  `Thread::Thread() : m_std_thread(Delegate<void(void)>::Create<Thread, &Thread::Setup>(this)),`
  (`src/framework/parallel/thread/thread.cpp:11-13`) puts a live `std::jthread` in the base's
  member-initialiser list, `Setup()` calls `Run()` (`thread.cpp:84`), and `Run()` is the virtual the
  whole design is built around (`thread.h:47`, empty body `thread.cpp:78`). Nothing orders the new
  thread's first instruction against the derived constructor installing its vtable, so the call lands
  on `qor::Thread::Run()` - does nothing, returns - whenever the scheduler wins. All three classes in
  the tree that override it are in one test file (`Worker`, `IntrospectiveWorker`,
  `SelfAwareWorker`: `test/framework/test_thread.cpp:39`, `:49`, `:64`), and the case that exercises
  them is `auto worker = new_ref<Worker>(); worker->Join();` (`test_thread.cpp:81-85`) - it asserts
  only that `Join()` returns, so it passes whether or not `Worker::Run()` ever ran. The template
  constructor (`thread.h:33-37`) takes the callable directly and does not have the problem.

- The stop path the class installs cannot be extended, because the function it calls is neither
  virtual nor reachable. `CleanUp()` is declared under `private:` and without `virtual`
  (`src/framework/parallel/thread/thread.h:49`, `:52`) and its body is empty
  (`thread.cpp:102`). All three constructors hand a delegate to exactly that member -
  `m_callback(m_std_thread.get_stop_token(), Delegate<void(void)>::Create<Thread, &Thread::CleanUp>(this))`
  (`thread.cpp:13`, `:17`, `thread.h:36`) - so `RequestStop()` (`thread.cpp:61-64`) requests a stop
  that fires a no-op: a derived thread either polls `GetStopToken()` itself or is never told. 0 files
  in `src`, `test` or `examples` mention `CleanUp` in this context, which is what a private
  non-virtual guarantees.
- `swap()` is not a swap, and it strands the stop callback. It exchanges the jthread
  (`std::swap(m_std_thread, other.m_std_thread)`, `thread.cpp:73`) and then assigns the pointer with
  `m_pCurrent = other.m_pCurrent; other.m_pCurrent = nullptr;` (`:74-75`) - asymmetric, and it never
  touches `m_callback`, which each object built from *its own* token at construction. After `a.swap(b)`,
  `a`'s callback is still registered against the stop source that `b` now owns: `b.RequestStop()` runs
  `a`'s `CleanUp`, and `a`'s own thread's stop request runs nothing. `operator=(Thread&&)` is
  `other.swap(*this); return *this;` (`thread.cpp:25-29`), so every move-assign of a `Thread` takes
  that path, and the move constructor is the one path that gets it right (it rebuilds `m_callback`
  after moving in the thread, `thread.cpp:15-17`).
- `m_pCurrent` is carried by every `Thread` and has no defined value on any of them. It is a raw
  `CurrentThread*` with no default member initialiser (`thread.h:54`); the default constructor's
  member-initialiser list names only `m_std_thread` and `m_callback` (`thread.cpp:11-13`), the template
  constructor's names the same two (`thread.h:34-37`), and the only 4 writes anywhere in `src`, `test`
  and `examples` are the two shuffles above (`thread.cpp:19-20`, `:74-75`) - which read it first. The
  class includes `src/qor/essentials/current/currentthread.h` (`thread.h:12`) for the type and never
  resolves it: the portable `qor::CurrentThread` accessors live in [[qor_current]], and this target
  links `qor_current` (`CMakeLists.txt:22`) without ever calling it.
- The pool half of the target ships its licence only in the repository root. `threadpool.h` is 39,894
  bytes of upstream library - its API vocabulary is `pr_task`, `multifuture`, `blocks`, `PostBlocks`,
  `SubmitBlocks`, `WaitFor`, and `threadpool.cpp:9` still carries upstream's doc comment verbatim
  ("The number of threads will be derived from total number of hardware threads available, as reported
  by the implementation... If a core is hyperthreaded, it will count as two threads.") - and `LICENSE`
  keeps the credit: `@file BS_thread_pool.hpp`, `@author Barak Shoshany`, "Licensed under the MIT
  license" (`LICENSE:121-127`). But that block sits under the un-named heading "Portions licensed under
  the following license:" (`LICENSE:112`), and all 8 in-tree files - `threadpool.h`, `threadpool.cpp`
  and the six `pool/*.h` - open `Copyright Querysoft Limited 2008 - Present` /
  `SPDX-License-Identifier: BSL-1.0` with 0 hits for `BS::`, `Barak`, `Shoshany`, `MIT` or any
  repository URL between them. A reader of the file alone is told Boost-1.0; only the root file says
  otherwise. `src/framework/event/signals/` is the counter-example in the same tree: its 11 vendored
  CsSignal files keep their notice in-file.

## See also

[[qor_current]] [[qor_sync]] [[qor_role]] [[qor_threadmemory]] [[qor_asyncioservice]] [[qor_module]]

