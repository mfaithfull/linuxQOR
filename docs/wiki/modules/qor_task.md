---
Type: WikiPage
PageType: module
Title: "qor_task"
Target: qor_task
Status: current
LastVerified: 2026-11-17
Claims: 10
Summary: "Vendored cppcoro coroutines wearing QOR's cancellation coat; its link line names the one library it never includes and omits the one it cannot compile without."
Anchors:
  - { kind: in-tree, path: src/framework/parallel/task, symbol: qor_task, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/parallel/task/CMakeLists.txt, symbol: qor_task, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/parallel/task/syncwait.h, symbol: sync_wait, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/parallel/task/detail/whenalltask.h, symbol: make_when_all_task, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/parallel/task/cancellation/cancellationtoken.h, symbol: CancellationToken, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/parallel/task/scheduler/round_robin_scheduler.hpp, symbol: round_robin_scheduler, verified: 2026-11-17 }
Judges:
  - cmake --build cmakebuild --target qor_task --parallel 8
---

# qor_task

"Async tasks: run, schedule, cancel, wait." (`docs/agent/purposes.md:51`); the folder's own
`purpose.md` is one sentence, "Provides asynchronous tasks and the tools to run, schedule, cancel and
wait for them." Built SHARED at version **0.2.0** - one of only 3 folders in `src` at that version
against 43 at 0.5.0 - from five translation units: `qor_task.cpp` (the module stamp) plus the four
`cancellation/*.cpp` (`CMakeLists.txt:5-7`).

- 10 of the 281 `CMakeLists.txt` under `src`, `test` and `examples` name it, 9 of them outside the
  folder; `docs/agent/repo_map.md:64` gives the row `- qor_task ... -> qor_module qor_host`, copied
  token for token from `CMakeLists.txt:17`. The same file has exactly one row mentioning `qor_task`,
  so the index records it as depending on two libraries and as being depended on by none, while nine
  CMakeLists link it and 97 include lines reach its headers.
- 41 files in the folder - 36 headers and 5 `.cpp`. What gets compiled is the cancellation quartet;
  everything else is templates. `QOR_TASK` reflects that exactly: 4 hits in `src`, one class marker
  apiece on `CancellationRegistration`, `CancellationSource`, `CancellationState`,
  `CancellationToken`. No class in `task.h`, `sharedtask.h` or the sequencers is marked.
- The API the rest of the framework uses is two names. `sync_wait` (`syncwait.h:17-24`) is the block-on-
  a-coroutine entry point - `src/framework/io/async/context.h`, `eventprocessor.h`, `filesystem/file/ifile.h`,
  `filesystem/pipe/ipipe.h`, `network/socket.cpp` - and `task<T>` (`task.h:22`) is the awaitable they
  pass to it. 36 headers, and the framework's traffic is a coroutine and a blocker.
- The vendoring is not hidden: `round_robin_scheduler.hpp:18` declares `namespace cppcoro` - the only
  file in the folder that is not in `qor`, which 36 declarations elsewhere claim - `whenalltask.h` tests
  `CPPCORO_COMPILER_MSVC` three times, and `task.h:171` cites
  `https://github.com/lewissbaker/cppcoro/issues/40`. `LICENSE:134` carries "Copyright 2017 Lewis Baker"
  above MIT text, so the provenance is declared.
- Tests are the target's real build. 18 `.cpp` under `test/framework/task`, every one in the QORTest
  source list, with `qor_task` in both of QORTest's OS link lines (`test/CMakeLists.txt:91`, `:102`);
  the sequencer tests, generator tests and `whenalltests.cpp` are compiled cppcoro test bodies against
  these headers.
- `docs/agent/repo_map.md:63-66` puts the three `parallel` siblings on the identical spine -
  `qor_sync`, `qor_task` and `qor_workitem` each `-> qor_module qor_host` - which reads as three
  copies of one row until the headers are opened: two of the three reach each other (see below).

## Failure modes

- The link line names the library it never touches and omits the one it cannot compile without.
  `target_link_libraries(${target} qor_module qor_host)` (`CMakeLists.txt:17`) - and the folder has 0
  mentions of `TheHost` or `app::Host` in any of its 41 files. What it does reach is `qor_sync`: 6
  include lines in 4 headers (`syncwait.h:7`, `detail/syncwaittask.h:13`, `multiproducersequencer.h:11`
  and `:13`, `sequencerange.h:10`, `singleproducersequencer.h:7`), and the dependency is not header-only
  - `sync_wait` constructs `detail::LightManualResetEvent` and calls `Wait()` (`syncwait.h:20-22`), a
  class marked `qor_pp_module_interface(QOR_SYNC)` with all six members out-of-line in
  `sync/lightweightmanualresetevent.cpp:15-34`. So `sync_wait` compiles into every consumer's object
  file and resolves only because each of those consumers - `qor_asyncioservice`, `qor_filesystem`,
  `winqor_async` - happens to link `qor_sync` for its own reasons. `qor_task` alone will not answer the
  call, and nothing about this target's declared inputs says it should have to.
- The MSVC workaround is switched off on MSVC. `whenalltask.h` guards three blocks with
  `#if CPPCORO_COMPILER_MSVC` (`:75`, `:297`, `:319`), and the macro is defined nowhere in `src` or
  `test` - 12 mentions across 6 files, every one of them an `#if` or a comment. The preprocessor reads
  an undefined identifier as 0, so this build takes the `#else` arms, including
  `co_yield co_await static_cast<AWAITABLE&&>(awaitable);` (`whenalltask.h:308`) - the very expression
  the discarded arm above it documents: "HACK: Workaround another bug in MSVC where the expression
  'co_yield co_await x' seems to completely ignore the co_yield an never calls promise.yield_value()"
  (`:299-303`, "an never" in place). The same dead guard drives
  `src/components/io/network/scratch/socket_accept_operation.hpp:43` and `:52`.
- The race that upstream's non-symmetric path guarded against is guarded by deleting the race.
  `qor_pp_compiler_supports_symetric_transfer` (sic) appears 9 times in the folder - `task.h:34, 75,
  90, 107, 298`, `generator/asyncgenerator.h:23, 428, 1042`, `scheduler/round_robin_scheduler.hpp:20` -
  and is defined nowhere. Every one of the 9 sits inside a comment (`//#if`, `/*#if !...`, `#endif*/`),
  so what left the build was not a branch but a member: the `std::atomic<bool> m_state;` field
  (`task.h:108-113`), `try_set_continuation()` with its `m_state.exchange(true, acq_rel)` (`:98-104`),
  and the `qor_pp_noinline void await_suspend()` that consumed them (`:41-60`). `task_promise_base`
  today has a bare `m_continuation` and a `set_continuation()` that cannot fail (`:91-94`). The comment
  on the deleted code cites "some tests in async_auto_reset_event_tests.cpp / were crashing under x86
  optimised builds" (`:45-46`); no file of that name exists in `test`.
- The vendored test suite exists twice and only half of it is built. 18 `.cpp` under
  `test/framework/task/` were ported - 0 of them include `<cppcoro/...>`, all reach the library through
  `src/framework/parallel/task/...`, and they occupy `test/CMakeLists.txt:38-55` - while 11 more
  (`test/components/network/` x7, `test/framework/file_tests.cpp`, `io_service_tests.cpp`,
  `scheduling_operator_tests.cpp`, `test/operational/excluded_thread_pool_tests.cpp`) still `#include
  <cppcoro/task.hpp>` and friends, and 0 of those 11 are named in any `CMakeLists.txt` under `test`:
  nothing compiles them, and the include root they need is not in this tree - `3rdparty/` is `angle`
  and `glad`. The library has its own orphan: `round_robin_scheduler.hpp` (3,248 bytes, still
  `namespace cppcoro`) has 7 tree-wide mentions and all 7 are inside itself, and the scheduler its own
  folder uses is `inlinescheduler.h` (`test/framework/task/sequencebarrier_tests.cpp:41`).

## See also

[[qor_sync]] [[qor_asyncioservice]] [[qor_thread]] [[qor_workitem]] [[qor_module]]

