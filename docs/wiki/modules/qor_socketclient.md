---
Type: WikiPage
PageType: module
Title: "qor_socketclient"
Target: qor_socketclient
Status: current
LastVerified: 2026-11-18
Claims: 16
Summary: "The second ghost in the clients folder: the one commented-out line above it hides the socket subdirectory as well as the http one, its judge fails with MSB1009 like its sibling's, and three independent compile blockers - a dead include prefix, an undeclared member, a namespace keyword lost - mean the folder could not compile if it were wired up."
Anchors:
  - { kind: in-tree, path: src/components/io/network/clients/socket, symbol: qor_socketclient, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/clients/CMakeLists.txt, symbol: add_subdirectory, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/clients/socket/CMakeLists.txt, symbol: qor_socketclient, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/clients/socket/socketclient.h, symbol: SocketClient, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/clients/socket/socketclient.cpp, symbol: SocketClient::Connect, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/clients/socket/error.h, symbol: SocketError, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_socketclient --parallel 8
---

# qor_socketclient

The other unreachable half of `src/components/io/network/clients`: its
14-line `CMakeLists.txt` sits in a folder that no `add_subdirectory` reaches, so
the target is a census member and an index row with no generated project. The
judge was run 18 Nov 2026 and failed: `MSBUILD : error MSB1009: Project file
does not exist. Switch: qor_socketclient.vcxproj` (exit 1), and `cmakebuild`
holds zero `qor_socketclient*` artifacts. `docs/agent/repo_map.md:103`,
`docs/agent/deps.dot:124` and `docs/agent/purposes.md:84` ("Socket client.")
describe it; `FEATURES.md` has no row.

- The same one commented-out line that buried `qor_httpclient` also hides this
  target: `clients/CMakeLists.txt:1` is `#add_subdirectory(socket)`, and
  grepping every `CMakeLists.txt` under `src`, `examples` and `test` for
  `clients/socket` returns 0 references - the target exists only in its own
  file (`socket/CMakeLists.txt:1`).
- Three independent compile blockers mean the folder would not build even if
  the line were uncommented: `socketclient.h:49` includes
  `src/components/network/clients/itransportclient.h` and `Test-Path
  src/components/network` is false (the real `itransportclient.h` sits one
  level up in `clients/`); `socketclient.cpp:90-92` defines
  `SocketClient::SetSocketOptions` and moves into `socket_options_`, neither of
  which the class body (`socketclient.h:55-236`, read whole) declares or
  contains; and `durationdetails.h:30` opens the nesting as
  `namespace network{ components{` - the keyword before `components` is lost.
- The dead include prefix is the folder pair's signature, not one file's:
  `qor_httpclient`'s page counts three files carrying
  `src/components/network/`, and this header is one of them (`:49`).
- `sslsocketclient.h` is 0 lines and is named by no file in `src`, `examples`
  or `test` (grep: 0 hits) - an empty header parked next to a target that is
  itself never built.
- The link line names seven libraries (`CMakeLists.txt:13-14`:
  `qor_module qor_host qor_thread qor_sync qor_injection qor_asyncioservice
  qor_socketconnector`) while `deps.dot:124` and `repo_map.md:103` list only
  the first five - two edges invisible to the graph, including the connector
  the class is built on.
- The cpp-httplib port is unmistakable: nine `//CPPHTTPLIB_*` fossils sit in
  the folder's member initialisers (`socketclient.h:208-213`:
  connection and read timeouts default to 300 s, write to 5 s), and the
  private helpers carry the port's names - `handle_EINTR` (`:140`),
  `adjust_host_string` (`:156`), the abstract-namespace unix-domain
  escape/unescape pair (`:165,176`).
- The system includes are POSIX-only - `sys/un.h`, `sys/socket.h`,
  `netinet/in.h`, `arpa/nameser.h`, `resolv.h`, `poll.h`, `netdb.h`,
  `fcntl.h` (`socketclient.h:34-42`) - and the configured tree is MSVC/msbuild
  (the judge's own error is a `.vcxproj` switch), so the folder is
  out-of-platform as well as unreached.
- The class face: `SocketClient : public ITransportClient` carries the
  `qor_pp_module_interface(QOR_SOCKETCLIENT)` mark (`socketclient.h:55`) in
  namespace `qor::io::network::components` (`:53`); copy and assignment are
  deleted (`:224-225`), and the three const members `m_host`, `m_port`,
  `m_hostAndPort` (`:188-190`) would forbid assignment even without the
  delete.
- All transport is delegated to the connector lib on its link line: the
  constructor makes `m_Connector = new_ref<SocketConnector>()` and plugs the
  member `SocketWriter`/`SocketReader` into it (`socketclient.cpp:38-40`);
  `Connect(host, ip, port)` is exactly `ConnectToAddress(...)` then
  `IsConnected()` (`:149-150`), and `Connect()` prefers the proxy host/port
  when set (`:160-162`), else resolves a custom IP from `m_AddressMap`
  (`:167-173`).
- Request bookkeeping is the port's thread model: `socket_mutex_` guards
  `socket_requests_in_flight_`, `socket_requests_are_from_thread_` and
  `socket_should_be_closed_when_request_is_done_` (`socketclient.h:198-203`),
  and `CloseSocket` asserts the ownership pairing (`socketclient.cpp:122`).
- `error.h` keeps four live states of its `enum class SocketError`
  (`Success`/`Unknown`/`Connection` at `:33-35`, `ConnectionTimeout` at `:47`);
  every other name - SSL, redirect, parsing, proxy, about two dozen - is
  commented out in two blocks (`:36-46`, `:48`-on), and the namespace closer
  is misspelled `qor::io::network::compnents` (`:73`).
- `error.h:70-71` declares `to_string(SocketError)` and the `operator<<`, but
  no `.cpp` or `.h` under `src` defines either (grep `to_string(SocketError`:
  0 hits) - declared, never provided.
- The license is the old MIT prose block ("Copyright Querysoft Limited
  2008 - 2025", 23 lines) in all five non-empty files - this folder was
  skipped by the SPDX BSL-1.0 conversion, exactly like the `http` folder
  beside it; the 36-line bootstrap names "Querysoft Open Runtime: Socket
  Client Module" (`qor_socketclient.cpp:33`).
- Folder size: 7 files, 729 physical lines - declared TUs 358 (bootstrap 36 +
  `socketclient.cpp` 322), headers 357 (`socketclient.h` 240, `error.h` 75,
  `durationdetails.h` 42, `sslsocketclient.h` 0); the only consumer in the
  repo, `examples/experimental/testclient/CMakeLists.txt:16,23`, is itself
  unconfigured (`examples/experimental/CMakeLists.txt:10-12` adds only
  `flow`, `osspecific`, `resourced`).

## Failure modes

- A wrong target name and a missing target are the same error, exactly as with
  its sibling: the judge exits 1 with `MSB1009: Project file does not exist.
  Switch: qor_socketclient.vcxproj` - a build script cannot tell ghost from
  typo.
- The compile blockers are latent, not loud: uncommenting
  `clients/CMakeLists.txt:1` would die at the first TU - dead include, then
  the undeclared `SetSocketOptions`/`socket_options_`, then the malformed
  `durationdetails.h` namespace - and because the folder's only consumer is
  also unconfigured, no configured target in the tree is scheduled to
  discover any of it.

## See also

[[qor_netclient]] [[qor_httpclient]] [[qor_module]] [[qor_host]]
[[qor_thread]] [[qor_sync]] [[qor_injection]] [[qor_asyncioservice]]
[[qor_sockets]] [[qor_pipeline]]

Not yet paged: `qor_socketconnector` (this group's leaf 5 - the connector this
class is built on), `qor_http` (group 5).
