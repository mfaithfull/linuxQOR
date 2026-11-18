---
Type: WikiPage
PageType: module
Title: "qor_httpclient"
Target: qor_httpclient
Status: current
LastVerified: 2026-11-18
Claims: 14
Summary: "The declared-but-never-configured lib: its CMakeLists sits in a folder no add_subdirectory reaches, its own judge command fails with MSB1009, its headers include a src/components/network tree that does not exist, and half its 5,923 lines are compiled by no target in the repo."
Anchors:
  - { kind: in-tree, path: src/components/io/network/clients/http, symbol: qor_httpclient, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/clients/http/CMakeLists.txt, symbol: qor_httpclient, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/clients/CMakeLists.txt, symbol: add_subdirectory, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/clients/http/protohttpclient.h, symbol: ProtoHTTPClient, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/network/clients/http/httpsocketclient.h, symbol: HTTPSocketClient, verified: 2026-11-18 }
  - { kind: in-tree, path: examples/experimental/testclient/CMakeLists.txt, symbol: qor_httpclient, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_httpclient --parallel 8
---

# qor_httpclient

The group's declared ghost: the target is a census member and an index row, but no
`add_subdirectory` reaches its folder, so the build system never generates it. The
judge command was run 18 Nov 2026 and failed: `MSBUILD : error MSB1009: Project file
does not exist` (exit 1) - there is no `qor_httpclient*` artifact anywhere in
`cmakebuild`, and the configured `clients` build folder holds only `CMakeFiles`
and `cmake_install.cmake`. `docs/agent/repo_map.md:102`, `docs/agent/deps.dot:123`
and `docs/agent/purposes.md:83` all describe it; `FEATURES.md` has no row.

- The folder is unreachable by design-or-accident: `src/components/io/network/clients/CMakeLists.txt` is one line, a commented-out `#add_subdirectory(socket)` (`:1`), and it never mentions `http` - grepping every `CMakeLists.txt` under `src`, `examples` and `test` for `clients/http` returns zero references, so the target exists only in its own file (`http/CMakeLists.txt:1`).
- The 6-library link line matches the graph exactly (`CMakeLists.txt:13` vs `deps.dot:123` and `repo_map.md:102` - `qor_http qor_module qor_host qor_thread qor_sync qor_injection`) - the index is more real than the build.
- Even wired up it would not be a dll like its siblings: `add_library` omits `SHARED` (`CMakeLists.txt:3`) and no `BUILD_SHARED_LIBS` exists in the root `CMakeLists.txt` or `cmake/flags.cmake`, so it would produce a static `.lib`.
- Its headers include a tree that is not there: `protohttpclient.h:40,42` and `httpsocketclient.h:29` point at `src/components/network/clients/...`, which `Test-Path` reports false; the real `text.h`, `itransportclient.h` and `socket/socketclient.h` sit under `src/components/io/network/clients/`, and three files in the tree carry that dead `src/components/network/` include prefix - the TUs would die at the include stage before any link.
- 14 files, 5,923 lines, of which the declared TUs are 1,203 (`qor_httpclient.cpp` 36, `protohttpclient.cpp` 1137, `httpsocketclient.cpp` 30); the other three `.cpp` files - `clientimpl.cpp` 2,014, `client.cpp` 666, `sslclientimpl.cpp` 512, half the folder's lines - are named by no `CMakeLists.txt` in the repo at all.
- The built face is a vendor port: `#define CPPHTTPLIB_VERSION "0.26.0"` (`protohttpclient.h:44`), and `ProtoHTTPClient` (`:62`) is a 30-overload `Get`/`Post`/`Put`/`Patch`/`Head` surface returning `protocols::http::Result`, riding the live `qor_http` protocol headers (`:30-39`) that do resolve.
- The transport seam is a raw pointer: `ITransportClient* m_transport` (`protohttpclient.h:542`) is assigned with `new SocketClient(host, port)` in `HTTPSocketClient`'s constructor (`httpsocketclient.h:44`, under `//TODO: fix this leaky patchwork` at `:43`), `delete`d in the destructor (`:49`), defaulted to `localhost:80` (`:37`) - and the inheritance itself is the "leaky patchwork": `HTTPSocketClient : public ProtoHTTPClient` (`:33`) adds only transport wiring.
- `httpsocketclient.cpp` is 30 lines whose entire namespace body is empty, closed with a misspelled fossil comment `}//qor::compponents::network` (`:30`); `httpsocketclient.h:59` closes with `#endif//...HTTP_ITRANSPORTCLIENT`, naming the wrong header - a copy-paste guard from the itransportclient file.
- The namespace is `qor::components::network` (`protohttpclient.h:46`, `httpsocketclient.h:31`) - a third shape in the io/network tree: `qor_netclient` uses `qor::io::network::components` and the pipeline libs use their own target-named forms.
- The license is the old MIT-prose block ("Copyright Querysoft Limited 2008 - 2025", 24 lines) in every file - this folder was skipped by the SPDX BSL-1.0 conversion that `qor_netclient` and the pipeline sources carry; the 36-line bootstrap names "Querysoft Open Runtime: HTTP Client Module" (`qor_httpclient.cpp:33`).
- Export marking is internally consistent, unusually: `QOR_HTTPCLIENT` is both the compile definition (`CMakeLists.txt:8`) and the class marker (`protohttpclient.h:62`, `httpsocketclient.h:33`) - moot for a lib that never compiles.
- The only consumer is also unconfigured: `examples/experimental/testclient/CMakeLists.txt:17,24` links it (with `qor_pipeline qor_socketconnector qor_log`), but `examples/experimental/CMakeLists.txt:10-12` adds only `flow`, `osspecific` and `resourced` - so even if the folder were wired up, no configured target in the tree would ask for it.

## Failure modes

- A wrong target name and a missing target are the same error: the judge exits 1 with `MSB1009: Project file does not exist. Switch: qor_httpclient.vcxproj` - a build script referencing this target cannot tell ghost from typo.
- A bare `ProtoHTTPClient` is a null dereference waiting to happen: the default constructor (`protohttpclient.h:67`) never sets `m_transport` and the header gives it no initializer, yet request paths dereference it unconditionally (`:157`) - and the repo's only transport-wiring subclass is itself un-includable because its `socketclient.h` include path (`httpsocketclient.h:29`) does not exist.

## See also

[[qor_netclient]] [[qor_module]] [[qor_host]] [[qor_thread]]
[[qor_sync]] [[qor_injection]]

Not yet paged: `qor_http` (its protocol lib), `qor_socketconnector` and
`qor_asyncioservice` (group 4-5), `qor_socketclient` (its CMakeLists folder is
the commented-out line above).
