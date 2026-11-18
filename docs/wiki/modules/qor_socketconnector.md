---
Type: WikiPage
PageType: module
Title: "qor_socketconnector"
Target: qor_socketconnector
Status: current
LastVerified: 2026-11-18
Claims: 20
Summary: "The one connector with real consumers: judge-green dll, used by netclient, netserver and ten examples; the Connector doubles as Plug and ISocket and can adopt an accepted socket, but the connect path tries only the first resolved address, silently discards nodelay/v6only/timeout, and the deprecated SocketWriter still stubs out byte-order conversion."
Anchors:
  - { kind: in-tree, path: src/components/io/pipeline/connectors/socketconnector, symbol: qor_socketconnector, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/socketconnector/CMakeLists.txt, symbol: qor_socketconnector, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/socketconnector/socketconnector.h, symbol: SocketConnector, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/socketconnector/socketconnector.cpp, symbol: AttemptConnections, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/socketconnector/socketwriter.h, symbol: SocketWriter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/CMakeLists.txt, symbol: add_subdirectory, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_socketconnector --parallel 8
---

# qor_socketconnector

The connector family's workhorse: configured at `connectors/CMakeLists.txt:1`,
judge-green on 18 Nov 2026 (exit 0, 1,122,816-byte dll stamped 11:42 against
a newest source of 15 Oct), and the only sibling with consumers - 16
`qor_socketconnector` lines across 13 CMakeLists: ten example directories,
plus `qor_netclient` (`src/components/io/network/client/CMakeLists.txt:15`),
`qor_netserver` (`src/components/io/network/server/CMakeLists.txt:15`) and
the never-configured ghost `qor_socketclient` (its `CMakeLists.txt:14`).
`repo_map.md:107`, `purposes.md:88` and `deps.dot:128` index it;
`FEATURES.md:74` rates it "Stable".

- The class is a hybrid: `SocketConnector : public pipeline::Plug, virtual
  public qor::io::network::ISocket` (`socketconnector.h:17`) in
  `qor::io::network::components` (`:15`) - a pipeline element that also
  exposes the full socket surface (`Bind`/`Listen`/`Accept`/`Connect`/
  `Receive`/`Send`/`Shutdown`... `socketconnector.h:38-61`) over an owned
  `m_Socket` (`:73`).
- The second constructor adopts an already-connected socket and session and
  sets `m_connected = true` directly (`socketconnector.cpp:30-38`) - the
  server-side path: `netserver`'s header-only session pipeline holds one per
  accepted connection (`src/components/io/network/server/sessionpipeline.h:11-13`).
- `AttemptConnections` loops over the `getaddrinfo` results but `return
  m_connected` sits inside the loop body (`socketconnector.cpp:101-114`), so
  only the first resolved address is ever tried; the loop shape promises
  fall-through it does not have.
- The address copy is a hardcoded `memcpy(..., 30/*address.address.byte_size*/)`
  (`socketconnector.cpp:107`) - the real size commented out.
- `Configure` stores `tcp_nodelay`, `ipv6_v6only` and `timeout_sec`
  (`socketconnector.cpp:124-126`) but `ConnectToAddress` declares all three
  as commented-out parameters - `bool /*tcp_nodelay*/, bool /*ipv6_v6only*/,
  time_t /*timeout_sec*/` (`:129`) - and the `GetAddressInfo` call passes a
  commented `/*timeout_sec*/` (`:144`): the options are kept in members and
  never applied to any socket.
- `ConnectToAddress` returns false outright for `AF_Unix` with
  `//TODO: ConnectUnixSocketToAddress()` (`socketconnector.cpp:131-134`) -
  unix sockets are the pipeconnector's unimplemented job too.
- `SetupHintsForGetAddressInfo` prefers a literal IP over the host name
  (`socketconnector.cpp:82-88`: `AF_Unspecified` + `DontUseHostNameResolution`),
  else takes the configured family and flags verbatim (`:91-97`).
- The deprecated half is real code with stubbed intent: `SocketWriter`'s
  `HToN` specializations for 16/32/64-bit return the value unchanged with
  the `htons`/`htonl` calls commented as TODOs
  (`socketwriter.cpp:54-71`), while the template `Write<T>`
  (`socketwriter.h:37-68`) is documented by use as network-order output.
- The same template's copy is wrong in units: `memcpy(space, ...,
  unitsToWrite)` (`socketwriter.h:53`) moves `unitsToWrite` bytes, not
  `unitsToWrite * unitSize`, and its offset arithmetic mixes bytes
  (`unitsWritten * unitSize`) with that unit-count third argument.
- `SocketWriter`'s convenience constructor connects synchronously in-body
  and discards the `bool` from `ConnectToAddress` (`socketwriter.cpp:24`) -
  a failed connect still yields a constructed writer.
- Deprecation is documented and narrow: `purpose.md` (8 lines, tracked at
  `repo_map.md:158`) and `purposes.md:88` mark SocketReader/SocketWriter
  DEPRECATED, and outside the folder they are referenced only by the ghost
  `qor_socketclient` (`socketclient.h:50-51,127,192-194`,
  `socketclient.cpp:38,153`) - SocketConnector itself carries all 50 other
  external hits.
- `SocketWriter` is additionally not exported: the class line carries no
  `qor_pp_module_interface` (`socketwriter.h:16`) while `SocketReader` does
  (`socketreader.h:15`) - anything but the ghost could not link it out of
  the dll.
- Copy-paste fossil in the guard: `socketsink.h:26` closes with
  `//QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STDOFSTREAM` - the stdofstreamsink
  folder's macro name, copied along with the rest of the file.
- The pumps gate on the connector: `ReadBytes`/`WriteBytes` test
  `m_Connector && m_Connector->IsConnected()` and otherwise raise the flyer
  `continuable("Attempting to read from disconnected socket.")`
  (`socketsource.cpp:19-26`) / write to a disconnected socket
  (`socketsink.cpp:19-26`); the source maps a -1 receive to 0
  (`socketsource.cpp:27-30`).
- Link line vs graph: eight libraries (`CMakeLists.txt:14-15`: `qor_module
  qor_thread qor_signals qor_pipeline qor_platform qor_sockets
  qor_asyncioservice qor_error`) against two in `deps.dot:128` and
  `repo_map.md:107` (`qor_module`, `qor_thread`); grep across the folder
  finds no use of `qor_signals` or `qor::thread` at all - the only
  near-miss is the `currentthread.h` include at `socketconnector.h:7`,
  which belongs to `qor_current`, also absent from the link line.
- Folder: 13 files, 809 physical lines - six declared TUs (`CMakeLists.txt:
  3-4`: bootstrap 15, `socketconnector.cpp` 309, `socketsink.cpp` 30,
  `socketwriter.cpp` 73, `socketreader.cpp` 53, `socketsource.cpp` 34 =
  514 lines), five headers (87+92+39+26+28 = 272), `purpose.md` 8,
  `CMakeLists.txt` 15.
- Judge evidence: `cmake --build cmakebuild --target qor_socketconnector
  --parallel 8` exit 0 on 18 Nov 2026; the dll predates the run (11:42,
  already built as a dependency) and is newer than every source in the
  folder - a fresh no-op.

## Failure modes

- A dual-stack host whose first `getaddrinfo` result is unreachable fails
  the connect even though a working address was returned: the loop returns
  after one attempt (`socketconnector.cpp:111`), and nothing in the result
  vector is tried - the failure looks like "host down" when it is
  "first A/AAAA record down".
- Latent socket misbehaviour from silently-dropped options: `Configure(host,
  port, ip, fam, flags, true /*nodelay*/, ..., 5 /*timeout*/)` compiles,
  stores, connects - and applies none of the three (`socketconnector.cpp:
  124-126` vs the commented parameters at `:129`); latency and hang
  symptoms will be blamed on the network, not the connector.
- The deprecated `SocketWriter<T>` path corrupts payloads without complaining:
  host-order bytes are written despite the network-order intent
  (`socketwriter.cpp:54-71`), and for unit sizes > 1 byte the `memcpy`
  length bug (`socketwriter.h:53`) truncates each chunk to one byte per
  unit - the ghost `qor_socketclient` is the only consumer, and it is the
  only thing that would ever notice.

## See also

[[qor_pipeline]] [[qor_sockets]] [[qor_asyncioservice]] [[qor_netclient]]
[[qor_netserver]] [[qor_socketclient]] [[qor_fileconnector]]
[[qor_pipeconnector]] [[qor_error]] [[qor_module]] [[qor_platform]]

Not yet paged: `qor_stdfstreamconnector`, `qor_stdofstreamsink` (the last
two leaves of this group - the sink folder whose guard-name fossil this
folder already leaked).
