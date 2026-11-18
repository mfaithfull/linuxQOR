---
Type: WikiPage
PageType: module
Title: "qor_netclient"
Target: qor_netclient
Status: current
LastVerified: 2026-11-18
Claims: 12
Summary: "The Stable plug-in-protocol client: a 92-line header wrapping two push-mode pipelines over a SocketConnector, the group's most under-indexed lib (13 libraries linked, 5 in the graph), exercised only by two intermediate examples - and whose protocol member is checked at connect and then used by nothing."
Anchors:
  - { kind: in-tree, path: src/components/io/network/client, symbol: qor_netclient, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/client/CMakeLists.txt, symbol: qor_netclient, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/client/netclient.h, symbol: NetworkClient, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/client/netclient.cpp, symbol: NetworkClient::Connect, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/client/qor_netclient.cpp, symbol: ThisModule, verified: 2026-11-18 }
  - { kind: in-tree, path: examples/intermediate/client/CMakeLists.txt, symbol: qor_netclient, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_netclient --parallel 8
---

# qor_netclient

Configured under `src/components/io/network`; the folder is the group's smallest -
4 files, 199 lines: 15-line `CMakeLists.txt` (with the conventional nested
`project(QOR VERSION 0.5.0 ...)` line, one of 62 such in `src/`), 92-line
`netclient.h`, 77-line `netclient.cpp`, 15-line bootstrap. `FEATURES.md:66` carries it
"Stable - Network Client Module, Plug in a protocol to use it." and
`docs/agent/purposes.md:82` points at the intermediate server/client examples.

- The group's widest index gap: the link line names thirteen libraries (`CMakeLists.txt:14-15`
  adds `qor_issue qor_log qor_error qor_pipeline qor_sockets qor_socketconnector
  qor_task qor_asyncioservice` to the five `docs/agent/repo_map.md:101` and
  `docs/agent/deps.dot:122` list) - eight edges invisible to the graph, including the
  sockets and connector the class is built on.
- The purposes row's examples are its only configured consumers: the `client` example
  links it (`examples/intermediate/client/CMakeLists.txt:14`) and the `server` example
  does (`:24`), both under the configured `examples/intermediate` tree; no test target
  links it, and the `curlike` example that does link it (`examples/experimental/curlike/CMakeLists.txt:16`)
  is not in the configured experimental list (`examples/experimental/CMakeLists.txt:10-12`).
- Two classes, one marked: `BaseClient` carries no export marker at all
  (`netclient.h:22`), its only pure virtual being `Connect` (`:28`), while
  `NetworkClient : public BaseClient` takes the `QOR_NETCLIENT` mark (`:66`) and deletes
  copy and assignment (`:71-72`) - the only lib in the group to forbid copies.
- The header comment is the contract: "simple bufferred, stateless, pipelines which can
  be reset for each send/receive" and "this must be specialized" (`:17-21`); the two
  `pipeline::Pipeline` members are the entire state (`:46-47`), and
  `Send`/`Receive` each just `ResetStream()` then test `PumpSome() > 0` (`:30-42`).
- The wiring is two push-mode pipelines over one connector: `SetSink` builds the
  response side (connector source into the caller's sink, buffer on the source element,
  push flow) (`netclient.cpp:22-31`) and `SetSource` builds the request side (caller's
  source into the connector's sink) (`:33-43`), with the sink edge additionally set
  element-to-element at `:41`; the connector itself is a member,
  `SocketConnector m_socketClientConnector` (`netclient.h:85`), an eight-argument
  `Configure` passthrough away from host/port/timeout (`netclient.cpp:45-48`).
- The protocol is bookkeeping: `m_protocol` is stored (`netclient.cpp:13,19`), checked
  at `Connect()` against null with a `serious` flyer (`:60-63`), and then never handed
  to any pipeline, connector or buffer in the TU - "plug in a protocol" (FEATURES.md:66)
  happens in the caller's own sink/source elements, not through this class.
- Both type checks fail silently: `SetSink` acts only `if(sink && sink->IsSink())` and
  `SetSource` only `if(source && source->IsSource())` (`netclient.cpp:24,35`) - no
  flyer, no return value - so a wrong element type leaves the pipeline empty and the
  misuse surfaces later only as `Send`/`Receive` returning false.
- Unlike its siblings, this lib does link its flyer: `qor_error` is on the link line
  (`CMakeLists.txt:15`) even though `serious` fires from one site (`netclient.cpp:62`) -
  but the graph still drops it (`deps.dot:122`).
- The namespace breaks the group's convention: `qor::io::network::components`
  (`netclient.h:15`) lives in the `io` tree's namespace, not the `qor::components::*`
  namespace the pipeline-source and serializer libs of this group share.
- The license is the SPDX shape end to end: 2-line BSL-1.0 "2008 - Present" in all four
  files (`:1-2`) and the 15-line bootstrap naming "Querysoft Open Runtime: Network
  Client Module" (`qor_netclient.cpp:12`). The judge compiled both TUs fresh (exit 0,
  run 18 Nov 2026) into a 1,008,128-byte dll stamped 11:42, newer than every folder file
  (newest, `netclient.h`, 15 Oct 2026).

## Failure modes

- Success is "at least one octet": `Send` and `Receive` reduce `PumpSome()` to a bool
  on `> 0` (`netclient.h:34,41`), so a three-byte partial on a four-byte protocol reads
  as a clean success, and the caller has no way to see how much crossed.
- Connect does not use the protocol it demands: the null check gates on
  `m_protocol.IsNull()` (`netclient.cpp:60`) but `Connect()` then only calls
  `m_socketClientConnector.Connect()` (`:67`) - a client wired with a protocol object
  that no sink or source knows about will connect and pump an empty pipeline,
  returning false from `Send` with no flyer to explain why.

## See also

[[qor_pipeline]] [[qor_sockets]] [[qor_module]] [[qor_host]] [[qor_current]]
[[qor_sync]] [[qor_injection]] [[qor_error]] [[qor_task]] [[qor_generatorsource]]

Not yet paged: `qor_httpclient` (group 3 leaf 7), `qor_socketconnector` and
`qor_asyncioservice` (group 4-5).
