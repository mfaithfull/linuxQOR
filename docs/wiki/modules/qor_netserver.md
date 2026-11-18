---
Type: WikiPage
PageType: module
Title: "qor_netserver"
Target: qor_netserver
Status: current
LastVerified: 2026-11-18
Claims: 17
Summary: "The group's only configured lib with a green judge: a three-state bind/listen/accept Workflow that posts each accepted client to a ThreadPool running its own three-state session pipeline, matching its unusually long purposes row almost claim for claim - behind a link line of 14 libraries the graph shows as 2, and behind two NDEBUG guards misspelled so they never hold."
Anchors:
  - { kind: in-tree, path: src/components/io/network/server, symbol: qor_netserver, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/server/CMakeLists.txt, symbol: qor_netserver, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/server/netserver.h, symbol: NetworkServer, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/server/netsession.h, symbol: NetworkSession, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/server/sessionpipeline.h, symbol: SessionPipeline, verified: 2026-11-18 }
  - { kind: in-tree, path: examples/intermediate/server/CMakeLists.txt, symbol: qor_netserver, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_netserver --parallel 8
---

# qor_netserver

The configured sibling of the clients ghosts: both `TUs` compile fresh on 18
Nov 2026 (exit 0, 4 warnings) into a 1,239,552-byte dll stamped 12:31, newer
than every folder file (newest, `qor_netserver.cpp`, 15 Oct 2026).
`FEATURES.md:67` carries it "Stable - Network Server Module, Plug in a
protocol to use it." and `docs/agent/purposes.md:85` makes the group's longest
claim: "production-ready: Thread Pool + Async IO + Workflows,
protocol-agnostic, per-connection logging; serves Echo Protocol on a port".

- The server itself is a workflow: `NetworkServer : public Workflow`
  (`netserver.h:20`) with exactly three states - `bind`, `listen`, `accept`
  (`:31-33`) - created in the constructor (`netserver.cpp:23-25`) with
  `SetInitialState(bind)` (`:108`); each state's `Enter` lambda does one
  socket step and moves on (`:30-106`).
- The subsystems are fetched at bind time, not held at construction: the
  platform `Sockets` subsystem (`netserver.cpp:34`), then the
  `thread::ThreadPool` and `async::Service` session from the Application
  role's features (`:36-38`); the listening socket is created with the
  protocol's own address family, framing and type (`:41-44`) - that is what
  "plug in a protocol" means concretely.
- Every accepted client is handed to the pool: `accept` calls
  `Accept` once, then `m_threadPool->PostTask` wraps
  `ServeClient(ClientSocket, protocol)` (`netserver.cpp:98-103`) - the
  purposes row's "Thread Pool" is real.
- Per-connection logging is real too: `ServeClient` builds a fresh
  `DefaultErrorHandler` + `DefaultLogHandler(Debug)` per session and
  forwards its signal to the `LogAggregatorService` receiver over a
  `QueuedConnection` (`netserver.cpp:123-134`); the aggregator lookup is
  null-checked (`:129`).
- "Serves Echo Protocol" is the configured example: `examples/intermediate/
  server/CMakeLists.txt:16` links `qor_netserver qor_echo`, and both
  configured consumers exist (`:16` and `examples/advanced/httpserver/
  CMakeLists.txt:20`, trees added at `examples/CMakeLists.txt:2-3`).
- The index gap is the repo's widest in this group: the link line names 14
  libraries (`CMakeLists.txt:13-15` adds `qor_thread qor_sync qor_injection
  qor_log qor_loghandler qor_logaggregator qor_error qor_sockets
  qor_socketconnector qor_workflow qor_role qor_application`) while
  `deps.dot:125` and `repo_map.md:104` list only `qor_module qor_host` -
  12 edges invisible to the graph.
- Two NDEBUG guards are misspelled `NDBEUG` (`netserver.cpp:114,125`), so
  they always pass: the per-session thread rename to "Client {fd}" (`:116`)
  and `defaultLogHandler.WriteToStandardOutput()` (`:126`) run in every
  build, release included; the same file's comment carries "porotocol
  transfered" (`:111`).
- The per-client session is header-only: `NetworkSession : public Workflow`
  with `connected`/`running`/`disconnect` states (`netsession.h:22,80-82`),
  all wiring in the constructor's lambdas - `connected` grabs its own async
  session and a `SessionPipeline` (`:38-39`), `running` pumps once and
  falls to `disconnect` when `PumpSome()` returns 0 (`:46-50`), `disconnect`
  shuts the socket `ShutdownReadWrite` and completes (`:61-63`); the class
  is instantiated and run in the pool task (`netserver.cpp:137`, result
  `[[maybe_unused]]`).
- Every session exits as success: `disconnect->Enter` calls
  `SetComplete(EXIT_SUCCESS)` unconditionally (`netsession.h:62`) - a
  connection that failed mid-pump completes the session workflow with 0.
- `SessionPipeline` (`sessionpipeline.h:17`) is a push-mode pipeline
  (`:28`) whose one protocol filter object serves as both write buffer
  (source side, `:32`) and read buffer (sink side, `:36`), source and sink
  cross-wired (`:39-40`), over a member `SocketConnector` whose `Connect()`
  carries the honest comment "In practice this is noop as we are passed an
  already connected socket" (`:42-43`).
- The pool-thread identity dance is debug bookkeeping: `ServeClient` renames
  the worker to the client fd, and `NetworkSession`'s destructor renames it
  back to "pool" before it is returned (`netsession.h:73`) - thread naming
  standing in for session tracking.
- The compiler already flags the address plumbing: the judge's run printed
  four warnings, `netserver.cpp(48,65)` C4244 assigning
  `eAddressFamily` to `sa_family` (unsigned short), `(49,35)` C4244
  narrowing `m_port` (unsigned) through `SetPort` (unsigned short), and two
  C4996 `strerror` deprecations (`:53,71`).
- `accept` has no re-arm and no quit path in one pass: after posting the
  task the handler returns without `SetState` or `SetComplete`
  (`netserver.cpp:82-106`), and the code's own TODOs say why it is unfinished
  - "Accept needs to be interruptable" (`:87`), "Quit if Accept was
  cancelled" (`:105`) - with the listen backlog hardcoded to 2 (`:67`).
- All six files carry the 2-line SPDX BSL-1.0 header ("2008 - Present") -
  unlike the clients folders next door; the 15-line bootstrap names "Querysoft
  Open Runtime: Network Server Module" (`qor_netserver.cpp:12`).
- Folder: 6 files, 374 physical lines - declared TUs 155 (bootstrap 15 +
  `netserver.cpp` 140), header-only classes 204 (`netserver.h` 47,
  `netsession.h` 93, `sessionpipeline.h` 64), the latter two included by
  `netserver.cpp` and named by no other TU.

## Failure modes

- The misspelled guards are a release-build leak: because `#ifndef NDBEUG`
  never matches `NDEBUG` (`netserver.cpp:114,125`), a production server
  renames every pool thread per connection and writes every session's debug
  log to stdout - the cost of a nine-letter typo.
- Port and family are narrowed at the one place that matters: the enum-to-
  `sa_family` and unsigned-to-unsigned-short conversions the judge printed
  (`netserver.cpp:48-49`) mean a port above 65535 binds silently wrong, and
  neither `Bind` nor `Listen` failure can be seen by a caller - the
  workflow just `SetComplete(EXIT_FAILURE)`s (`:54,73`).

## See also

[[qor_netclient]] [[qor_httpclient]] [[qor_socketclient]] [[qor_module]]
[[qor_host]] [[qor_workflow]] [[qor_role]] [[qor_application]]
[[qor_thread]] [[qor_sync]] [[qor_sockets]] [[qor_asyncioservice]]
[[qor_pipeline]] [[qor_log]] [[qor_error]]

Not yet paged: `qor_loghandler` and `qor_logaggregator` (group 6),
`qor_socketconnector` (this group's leaf 5 - the `SessionPipeline` member).
