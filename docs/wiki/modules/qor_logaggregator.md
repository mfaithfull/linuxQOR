---
Type: WikiPage
PageType: module
Title: "qor_logaggregator"
Target: qor_logaggregator
Status: current
LastVerified: 2026-11-19
Claims: 32
Summary: "The log fan-in service: a ThreadPool-posted listener drains a signal queue into stdout and a rolling .log file, and its compareThreads() override makes an AutoConnection to it queue onto that drain thread. Green judge, 21 consumers, and three real defects in the drain - the pipeline sink is a TODO, the shutdown flag is a plain bool shared across threads, and ReceiveLog writes the log file with no lock while the path FEATURES.md calls Stable."
Sources:
  - hook-154e547dba-qor_logaggregator
Anchors:
  - { kind: in-tree, path: src/components/qor/logaggregator, symbol: qor_logaggregator, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/qor/logaggregator/CMakeLists.txt, symbol: qor_logaggregator, verified: 2026-11-19 }
  - { kind: in-tree, path: src/components/qor/logaggregator/logaggregator.h, symbol: LogAggregatorService, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/qor/logaggregator/logreceiver.h, symbol: LogReceiver, verified: 2026-11-19 }
  - { kind: in-tree, path: src/components/qor/logaggregator/logreceiver.cpp, symbol: LogReceiver::Listen, verified: 2026-11-19 }
  - { kind: in-tree, path: src/components/qor/logaggregator/qor_logaggregator.cpp, symbol: ThisModule, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/qor/logaggregator/logreceiver.cpp, anchor: commit:154e547dba7027165d3124a743f5ea1372846468, verified: 2026-11-19 }
  - { kind: in-tree, path: src/components/qor/logaggregator/logreceiver.h, anchor: commit:154e547dba7027165d3124a743f5ea1372846468, verified: 2026-11-19 }
Judges:
  - cmake --build cmakebuild --target qor_logaggregator --parallel 8
---

# qor_logaggregator

Judge GREEN: `cmake --build cmakebuild --target qor_logaggregator
--parallel 8` exits 0 and links
`cmakebuild/src/components/qor/logaggregator/Debug/qor_logaggregator.dll`,
no warning or error line from this target's three sources.
`src/components/qor/CMakeLists.txt:1` adds the folder. The folder is
under `src/components/qor/`, **not** `src/qor/` - the core logging
module is `qor_log` in the other region, and this is the components
region's own logging service.

- **Folder:** 5 sources + build file, 403 physical lines -
  `CMakeLists.txt` 14, `qor_logaggregator.cpp` 38,
  `logaggregator.cpp` 70, `logaggregator.h` 81, `logreceiver.cpp` 139,
  `logreceiver.h` 61.
- **Link line:** 10 libraries (`CMakeLists.txt:14`) - `qor_module`
  `qor_thread` `qor_sync` `qor_signals` `qor_error` `qor_log`
  `qor_interception` `qor_loghandler` `qor_filesystem` `qor_role`. It
  links a sibling components lib (`qor_loghandler`), and
  `logreceiver.h:19` includes its header, so the edge is declared and
  used.
- **No index drift here, unlike its neighbours:** `docs/agent/repo_map.md:118`
  lists the same 10 dependencies and `docs/agent/deps.dot:138` the same
  10, against the 10 in the link line - `qor_uri.md` and `qor_jip.md`
  both carry a row that drops one.
- **The service:** `class qor_pp_module_interface(QOR_LOGAGGREGATOR)
  LogAggregatorService : public app::IFeature` (`logaggregator.h:47`),
  `Setup()`/`Shutdown()` (`:61-62`), `Receiver()` (`:64`), the
  copy/move quad deleted (`:56-59`), and two members - a
  `ref_of<thread::ThreadPool>` and a `LogReceiver` held **by value**
  (`:68-69`).
- Registered three ways: `SingletonInstancer` (`logaggregator.h:74`),
  `ExternalFactory` (`:75`), and a hard-coded
  `constexpr GUID LogAggregatorServiceGUID = {0x490c8af7, ...}`
  (`:76-77`) - a grep over `src`, `examples`, `test` finds that GUID
  named nowhere outside those two lines, so every consumer reaches the
  service by typed lookup, never by its id.
- The provide/probe pair: `qor_pp_module_will_provide`
  (`logaggregator.h:43`) and `ImplementsLogAggregatorService()`
  declared under `#ifndef QOR_LOGAGGREGATOR` (`:40-42`). No source line
  defines that function, yet the dll exports
  `?ImplementsLogAggregatorService@qor@@YA_NXZ` - the body comes from
  the `qor_pp_module_provide(QOR_LOGAGGREGATOR, LogAggregatorService)`
  macro at `logaggregator.cpp:35`.
- **Export probe:** `dumpbin /exports` on the fresh dll prints 17 -
  2 ctors, 2 dtors, 2 vtables (`??_7`), and the members; the deleted
  copy/move quad leaves nothing to export, which is what the header
  promises.
- **Setup can do nothing at all:** `logaggregator.cpp:43-57` fetches
  `m_Role->GetFeature<thread::ThreadPool>()` and only then posts
  `m_receiver.Listen()` (`:49-55`); the `if(m_threadPool.IsNotNull())`
  guard at `:47` has no `else`, no log, no error - a role without a
  ThreadPool gets a log service that never listens, silently.
- `Shutdown()` (`logaggregator.cpp:59-62`) calls `m_receiver.Stop()`
  and nothing else: `Stop` only flips a flag and notifies
  (`logreceiver.cpp:72-80`), so the pool thread may still be inside
  `Listen()` after the service reports shutdown.
- The type registration is lazy: `qor_logaggregator.cpp:36` places
  `static qor::TypeRegEntry<LogAggregatorService, LogAggregatorService>
  regLogAggregatorService;` **inside** `ThisModule()` (`:32-38`), so
  registration happens on the bootstrap's first call, not at load.
- **The receiver is a signal slot:** `LogReceiver : public SlotBase`
  (`logreceiver.h:25`) overriding `queueSlot(PendingSlot,
  ConnectionKind)` (`:42`) and `compareThreads() const` (`:43`), with a
  `std::deque<PendingSlot> m_array`, a `std::mutex` and a
  `std::condition_variable` (`:45-47`); `queueSlot` pushes and notifies
  (`logreceiver.cpp:82-87`) and `Listen()` drains. `compareThreads`
  returns `false` (`:89-92`, commit `154e547dba`), so an
  `AutoConnection` here takes [[qor_signals]]' queueing branch.
- `Listen()` (`logreceiver.cpp:94-118`) sets `m_running = true` and
  loops: pop-and-call when the deque is non-empty (`:102-108`, the slot
  is called with the lock released at `:106`), otherwise
  `m_alarm.wait_for(lock, 250ms)` (`:96,116`). It is run as the body of
  the posted task, on a thread renamed "Log Aggregator"
  (`logaggregator.cpp:52`).
- **Three sinks, two of them real:** `WriteToStandardOutput`,
  `WriteToFileSystem`, `WriteToPipeline` (`logreceiver.h:36-38`) behind
  three flags, initialised `true` / `false` / `false`
  (`logreceiver.cpp:18-20`) - stdout is on by default.
- The pipeline sink is a placeholder: `WriteToPipeline` only sets the
  flag (`logreceiver.cpp:66-70`), and `ReceiveLog`'s
  `if(m_writeToPipeline)` branch is `//TODO:` (`:133-136`). The public
  API exists and its effect is nothing.
- Rolling is capped by count and total bytes only: the ctor sets
  `m_logRollLimit(100)`, `m_logRollNumber(0)`, `m_totalSizeLimit(100 *
  1024 * 1024)`, `m_fileSizeLimit(10 * 1024 * 1024)`
  (`logreceiver.cpp:21-24`), and the do-while picks `{prefix}{N}.log`
  on those limits (`:48-52`) - but `m_fileSizeLimit` is read nowhere in
  the tree (declaration `logreceiver.h:56` plus that initializer), so
  the 10 MiB per-file cap the field names is never enforced.
- `m_running` is a plain `bool` (`logreceiver.h:48`): written by
  `Stop()` on the caller's thread (`logreceiver.cpp:76,79`) and read by
  the pool thread at `:97,98,111`. `<atomic>` is included in both
  headers (`logaggregator.h:28`, `logreceiver.h:7`) and used for
  nothing in either class.
- `Stop()` writes the flag twice - inside the lock at `:76` and again
  unguarded at `:79`.
- `Listen()`'s `else if (!m_running) break;` (`:111-114`) is
  unreachable: `while (m_running)` (`:98`) tests the same flag on every
  pass, so the loop has already exited before that branch.
- The whole listener is a fossil twin of the resource manager:
  `src/framework/data/resources/manager.cpp` carries the same shape -
  `m_running(false)` `:15`, `= true` `:31`, `while (m_running)` `:49`,
  `else if (!m_running)` `:61`, `m_running = false` twice `:106,109`,
  `bool m_running` at `manager.h:61`. Same two defects, two folders.
- **The write path is the unlocked one:** `ReceiveLog(std::string
  logEntry)` (`logreceiver.cpp:120`, by value) reads the three flags
  and calls `m_refLogFile->Write(...)` (`:130`) with no lock, while
  `WriteToFileSystem`, `WriteToPipeline`, `Stop` and `queueSlot` each
  take `std::scoped_lock<std::mutex>` (`:47,68,75,84`).
- File selection races its own open: the do-while probes
  `index.Exists()` (`:52`) and then `Open` chooses
  `WithFlags::Append` or `CreateNew | Exclusive` on a second
  `logFileIndex.Exists()` (`:56-59`) - check and create are separate
  calls.
- **Consumers: 21 build files outside the folder**, 20 of them under
  `examples/` and `test/`, and exactly 1 under `src/` -
  `src/components/io/network/server/CMakeLists.txt:14`, whose code
  fetches the feature at `netserver.cpp:128`
  (`GetFeature<qor::components::LogAggregatorService>()`).
- The examples are the only working contract:
  `examples/advanced/httpserver/httpserverapp.cpp:32`
  `qor_pp_module_requires(LogAggregatorService)`, `:68`
  `role->AddFeature<LogAggregatorService>()`, and `:189` connects a
  `LogHandler`'s forward signal to `&LogReceiver::ReceiveLog` - the
  aggregator is driven by `qor_loghandler`, not by `qor_log`.
- Index rows: `docs/agent/purposes.md:97` "Log Aggregator (thread pool
  + signals + filesystem + role)"; `FEATURES.md:64` rates it "Stable"
  as a "Multi threading log aggregator and handler".
- **Two licenses in one target:** `logaggregator.cpp`,
  `logaggregator.h` and `qor_logaggregator.cpp` carry the 23-line
  permission block dated "2008 - 2025", while `logreceiver.cpp` and
  `logreceiver.h` carry `SPDX-License-Identifier: BSL-1.0` dated
  "2008 - Present". Both families are large tree-wide: over the 2815
  `.h`/`.cpp` files under `src/`, 1563 carry the SPDX line and 1174 the
  permission block.
- Guards: `QOR_PP_H_COMPONENTS_LOGAGGREGATOR` (`logaggregator.h:25,26,81`)
  and `QOR_PP_H_COMPONENTS_LOGRECEIVER` (`logreceiver.h:4,5,61`) - 3
  hits each, unique tree-wide, and neither names the `qor` region its
  path sits in. `logreceiver.h` carries its own guard but the *other*
  file's export macro: `class qor_pp_module_interface(QOR_LOGAGGREGATOR)
  LogReceiver` (`:25`), so `LogReceiver`'s visibility is owned by this
  target's `PRIVATE QOR_LOGAGGREGATOR` definition (`CMakeLists.txt:9`).
- `<coroutine>` is included twice (`logaggregator.h:29`,
  `logreceiver.h:8`); neither header declares anything
  coroutine-shaped.

## Failure modes

- Silent vacuity at Setup: the `if(m_threadPool.IsNotNull())` guard
  (`logaggregator.cpp:47`) has no else, so a role assembled without a
  ThreadPool feature owns a log service that never listens - and the
  module that would report it is the one that failed to start.
- A data race on the shutdown handshake: `bool m_running`
  (`logreceiver.h:48`) is written from `Stop()` on one thread
  (`:76,79`) and read on the pool thread (`:98`), and the duplicated
  write plus the unreachable break at `:111` show a handshake that no
  synchronization implements. The judge stays green because a race is
  not a compile error.
- A sink that promises nothing: `WriteToPipeline(true)`
  (`logreceiver.cpp:66-70`) sets a flag whose only consumer is
  `//TODO:` (`:135`), so a caller believes its log lines are in the
  pipeline while no code puts them there and no diagnostic exists.
- Torn file logging under rotation: `ReceiveLog` writes through
  `m_refLogFile` unlocked (`:130`) while `WriteToFileSystem` may
  re-open that same member under the lock (`:56-59`) - a rotation
  during traffic can write to a handle being replaced.
- `FEATURES.md:64` says "Stable" for a library linked by 21 build
  files whose two unsynchronized paths are exactly what a stability
  claim should be weighed against, and whose rolling limits
  (`:21-24`) are asserted by nothing but their initializers.

## See also

[[qor_log]] [[qor_signals]] [[qor_thread]] [[qor_filesystem]]
[[qor_role]] [[qor_netserver]] [[qor_sync]] [[qor_interception]]
