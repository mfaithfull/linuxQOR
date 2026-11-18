---
Type: WikiPage
PageType: module
Title: "qor_sockets"
Target: qor_sockets
Status: current
LastVerified: 2026-11-17
Claims: 9
Summary: "Socket subsystem over registered OS twins; four of ten .cpp built, CreateSocket answers null, and the linker-pull hook it declares is defined only by those twins."
Anchors:
  - { kind: in-tree, path: src/framework/io/network, symbol: qor_sockets, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/io/network/CMakeLists.txt, symbol: qor_sockets, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/io/network/sockets.cpp, symbol: Sockets, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/io/network/sockets.h, symbol: ImplementsSockets, verified: 2026-11-17 }
  - { kind: in-tree, path: src/framework/io/network/socket.h, symbol: Socket, verified: 2026-11-17 }
  - { kind: in-tree, path: src/platform/os/windows/io/network/winqor_sockets.cpp, symbol: regSockets, verified: 2026-11-17 }
Judges:
  - cmake --build cmakebuild --target qor_sockets --parallel 8
---

# qor_sockets

"Network sockets + addressing via platform layer modules." (`docs/agent/purposes.md:49`); the folder's
`purpose.md` is that sentence in full. Built SHARED from four translation units -
`qor_sockets.cpp sockets.cpp socket.cpp address.cpp` - at version 0.5.0
(`src/framework/io/network/CMakeLists.txt:4`).

- 21 of the 281 `CMakeLists.txt` under `src`, `test` and `examples` name it. Its `docs/agent/repo_map.md`
  row (L62) is the only dependency row in that index written as prose - `-> (portable/os libs only)` -
  instead of target names; the names are `qor_module qor_host qor_current qor_sync qor_platform
  qor_issue qor_interception qor_asyncioservice` plus `ws2_32` in the Windows branch
  (`src/framework/io/network/CMakeLists.txt:14-23`).
- It is one leg of the same three-way split as [[qor_dynamiclibrary]]: `winqor_sockets`
  (`src/platform/os/windows/io/network/CMakeLists.txt:2`) and `linqor_sockets`
  (`src/platform/os/linux/io/network/CMakeLists.txt:1`) link *back against it*
  (`target_link_libraries(${target} qor_sockets ... winqor_api_winsock2)`, Windows CMakeLists:14-15)
  and register the real classes inside their `ThisModule()`:
  `static qor::TypeRegEntry< qor::io::network::win::Sockets, qor::io::network::Sockets > regSockets;`
  plus a `TypeRegEntryWithParams` for `Socket` (`src/platform/os/windows/io/network/winqor_sockets.cpp:45-46`).
  That registration, not a link edge, is what `new_ref<Sockets>()` resolves to.
- The portable half is a documented refuser. `Sockets::CreateSocket` builds an empty ref and returns it,
  `GetAddressInfo` returns -1, `Setup`/`Shutdown` are empty
  (`src/framework/io/network/sockets.cpp:17-30`), and the header says so in advance - "Empty base
  implementation of Sockets subsystem" (`src/framework/io/network/sockets.h:18`). None of the four
  built files names a winsock symbol; the seam is `ExternalFactory`, `SocketsGUID`
  (`sockets.h:40-42`) and the two `TypeRegEntry` lines above.
- `QOR_SOCKETS` has 46 hits in 13 files in `src`: 37 in 6 files in this folder (`socket.h` alone holds
  31), one at `src/framework/io/async/interface.h:12`, and 4 in each OS twin. `Sockets` and
  `ImplementsSockets` carry the compiler-independent
  `qor_pp_module_interface` (`sockets.h:14,19`), but `Socket` carries the GCC-only
  `qor_pp_module_interface_gcc` at class level (`socket.h:22`; forward declarations at `isocket.h:20`
  and `src/framework/io/async/interface.h:12`) - defined empty under MSVC
  (`src/configuration/configuration.h:43`) - so under the judge's own compiler its export direction
  rests entirely on the 31 per-member markers.
- QORTest is its only test consumer (`platform/test_network.cpp` in `test/CMakeLists.txt:69`), and a
  provider twin is always linked with it - `linqor_sockets` (L94) on Linux, `winqor_sockets` (L106) on
  Windows. Line 51 stores `qor::ImplementsSockets()` in a global `bool requiresSockets` that is never
  read; the canonical form of that line is the macro `qor_pp_module_requires(_X)`
  (`src/configuration/configuration.h:35`), which this tree's only network test hand-rolls instead.

## Failure modes

- The linker-pull hook is declared for everyone and defined by none of this. "All libraries providing
  an implementation of Sockets also need to export this function so that the linker can find them"
  (`src/framework/io/network/sockets.h:13-14`) declares `qor::ImplementsSockets()`; no built file in
  this folder defines it. Only the twins do - `qor_pp_module_provide(WINQOR_SOCKETS,Sockets)`
  (`src/platform/os/windows/io/network/sockets.cpp:38`) expands to the definition
  (`src/configuration/configuration.h:37`) - so linking `qor_sockets` alone leaves the one tree call,
  `test/platform/test_network.cpp:51`, unresolved, and QORTest is green only because both its per-OS
  lines force a twin in.
- The addressing family is on disk but compiled into nothing. `CMakeLists.txt:5` is a commented-out
  source list naming the six `.cpp` of the `ipaddress/ipv4address/ipv6address/ipendpoint/ipv4endpoint/
  ipv6endpoint` pairs; all twelve files exist, the largest is `ipv6address.cpp` at 8026 B, and none of
  the six appears in any other source list in the tree. The surviving headers are not inert:
  `ipendpoint.h` is included by four Windows files (`src/platform/os/windows/io/network/addresshelpers.h:36`,
  `io/async/operations/connectop.h:13`, `recvfromop.h:14`, `sendtoop.h:13`), and `IPEndpoint` is inline
  except `to_string()` and `from_string()` (`ipendpoint.h:36,38`), which are defined only in the
  unbuilt `ipendpoint.cpp:10-30`. Nothing calls those two members today, so every current consumer
  links; the first one who does meets an unresolved external from a header that was included.
- The only network test asserts nothing about sockets. `canCreateSocket`
  (`test/platform/test_network.cpp:66-75`) calls `CreateSocket` and shuts the subsystem down without a
  single assertion - null socket or real one, the suite passes; the suite's lone `isNotNull` is at L56,
  against the injection succeeding. The skip signal computed at L51 is never read, so there is no path
  on which these tests abstain, and the fourth `new_ref<Sockets>()` (L81) lives inside the commented-out
  `canConnectSocket` block (L78). That test file also `#include <winsock2.h>` directly with
  `#undef SetPort/min/max` (L31-34) - the abstraction it tests leaks through its own includes.
- The dependency index orders this target before its own dependency. `docs/agent/repo_map.md` puts
  `qor_sockets` at layer 7 (L138: `7. qor_platform qor_arch qor_compiler qor_filesystem qor_sockets`)
  and `qor_asyncioservice` - named in this target's link line
  (`src/framework/io/network/CMakeLists.txt:15`) - at layer 8 (L139). CMake does not read the prose, so
  nothing fails; the numbered order, which `qor_pipeline`'s page and others cite as the build order,
  is wrong here in both directions at once.

## See also

[[qor_asyncioservice]] [[qor_dynamiclibrary]] [[qor_pipeline]] [[qor_module]]

