---
Type: WikiPage
PageType: module
Title: "qor_uri"
Target: qor_uri
Status: current
LastVerified: 2026-11-18
Claims: 26
Summary: "Configured, linked and green - and empty: the URI class declares five ctors and no members or methods (uri.h:150-159), all five are defined with empty bodies, 73 header lines are a hand-copied System.Uri wish-list that exists nowhere in the tree, and the one construction site tree-wide is an assert-free test. The machine layer still holds the folder up as the template for new modules."
Anchors:
  - { kind: in-tree, path: src/components/protocols/uri, symbol: qor_uri, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/uri/CMakeLists.txt, symbol: qor_uri, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/uri/uri.h, symbol: URIComponents, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/uri/uri.h, symbol: URI, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/uri/uri.cpp, symbol: URI::URI, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/uri/qor_uri.cpp, symbol: ThisModule, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_uri --parallel 8
---

# qor_uri

`cmake --build cmakebuild --target qor_uri --parallel 8` exits 0 and
links `cmakebuild/src/components/protocols/uri/Debug/qor_uri.dll`.
`src/components/protocols/CMakeLists.txt:2` adds the folder, so - unlike
its sibling `qor_jip` (`qor_jip.md`) - this target really is in the
solution. What it builds is an empty shell, and the page records what
the emptiness is made of.

- **Folder:** 4 files, 223 physical lines - `CMakeLists.txt` 14,
  `uri.h` 162, `uri.cpp` 33, `qor_uri.cpp` 14.
- **Configured:** `src/components/protocols/CMakeLists.txt:2` is
  `add_subdirectory(uri)`; the judge's log shows the dependency chain
  (`qor_module` -> `qor_host` -> ... -> `qor_parser`) and then
  `qor_uri.cpp`, `uri.cpp`, and the library import.
- **No state:** `uri.h:150-159` declares
  `class qor_pp_module_interface(QOR_URI) URI` with five ctor
  signatures (`:153-158`) and nothing else - no data member, no
  method, no destructor.
- **No behaviour:** `uri.cpp:7-31` defines all five with empty bodies;
  the sixth ctor (`StreamingContext`, i.e. from a DataSource) is
  commented out at both ends - `uri.h:154` and `uri.cpp:12`.
- The compiler says it: the build prints `warning C4100: 'uri':
  unreferenced parameter` at `uri.cpp(7,31)` - the single argument of
  the single-argument ctor is thrown away. 1 warning, 0 errors for
  this target.
- **The header is a wish-list:** three block comments enumerate 16
  scheme names (`uri.h:11-29`), 22 property names (`:31-54`) and 35
  method and operator names (`:56-93`) - 73 names copied from .NET's
  `System.Uri`, none of them declared in this class.
- Verified absence: 8 of those names (`EscapeUriString`,
  `MakeRelativeUri`, `GetComponents`, `IsWellFormed*`,
  `CheckHostName`, `HexUnescape`, `IsBaseOf`, `UriSchemeNetTcp`) match
  only inside `uri.h` itself, 0 hits elsewhere in `src` or `test`.
- The copy kept its errors: `PaathAndQuery` (`uri.h:47`),
  `IsWellFormaedUriString` (`:81`), and three lower-case originals
  `isAbsoluteUri` / `isLoopback` / `isUNC` (`:40,43,44`) - the typos
  are as written, so the list was transcribed by hand, not generated.
- What the header does declare is vocabulary, not behaviour: 4 enums -
  `URIFormat` (`:97-102`), `URIKind` (`:106-111`), `URIHostNameType`
  (`:115-122`), `URIComponents : int64_t` (`:129-148`, 16 values with
  the .NET bit flags `AbsoluteUri = 127`, `KeepDelimiter = 1073741824`)
  - plus `URICreationOptions` (`:124-127`), one field named
  `DangerousDisablePathAndQueryCanonicalization`.
- Nothing reads that vocabulary: `uri.cpp` accepts `URICreationOptions`
  and `URIKind` (`:13,18`) and discards both, and no name of the 16
  `URIComponents` values appears in either `.cpp` in the folder.
- **Namespace drift:** `uri.h:9` opens `qor{ data{`, while every
  sibling under `protocols/` opens the region it lives in -
  `echoprotocol.h:11` `qor::components::protocols::echo`,
  `protocol.h:10` `...::http` - so the type sits outside the namespace
  its folder implies, and its test must say `qor::data::URI` to reach
  it.
- **Link line vs includes:** `CMakeLists.txt:13-14` names 6 libraries;
  the folder's sources include `src/configuration/configuration.h`,
  `uri.h`, `src/qor/module/module.h` and `<string>` (plus
  `<buildnumber.h>` under `#ifdef NDEBUG`, `qor_uri.cpp:5-7`). Only
  `qor_module` of the six is included: `qor_host`, `qor_current`,
  `qor_sync`, `qor_injection` and `qor_parser` are linked, never
  included by a source in this folder.
- Export macro: the name `QOR_URI` appears twice in `src`
  (case-sensitive grep) - the `PRIVATE` compile definition
  (`CMakeLists.txt:8`) and the class decoration (`uri.h:150`).
- Guard: `QOR_PP_H_COMPONENTS_PROTOCOLS_URI` holds 3 hits, all in
  `uri.h` (`:4,5,162`), unique tree-wide.
- **Provenance probe on the green artifact:** `dumpbin /exports` over
  the freshly built `qor_uri.dll` prints 7 exports - the 5 ctors
  (`??0URI@data@qor@@...`) and the two implicitly generated assignment
  operators (`??4URI@data@qor@@...`). No method is exported, because
  the class has none; the export table is the shortest proof of the
  emptiness.
- Index drift again: `docs/agent/repo_map.md:117` gives the row 5
  dependencies and omits `qor_parser`, and `docs/agent/deps.dot:137`
  gives the same 5, while the link line names 6 - the pattern
  `qor_echo.md` and `qor_jip.md:87-90` also record.
- `docs/agent/purposes.md:96` is two words: "URI handling.";
  `FEATURES.md` carries no `qor_uri` row - a case-insensitive grep for
  `uri` in that file returns 0 lines.
- **The machine layer teaches from this folder:**
  `docs/agent/add_module.md:27` "Template (copied from qor_uri; SHARED
  => .dll)" and `:83` "Template (qor_uri + qor_dio)" - the empty class
  is the exemplar a new module is told to start from.
- **Consumers: one, in a test.** `test/uri/test_uri.cpp:12` includes
  the header and `:16` constructs the only `qor::data::URI` tree-wide,
  from a host string ending `.egg`; `test/CMakeLists.txt:127` adds the
  folder and `test/uri/CMakeLists.txt:15` links `qor_uri` with
  `qor_application` and `qor_role`.
- The test asserts nothing: grep `assert|EXPECT|CHECK|TEST(|REQUIRE`
  over `test/uri/*.cpp` returns 1 line, and it is
  `qor_pp_module_requires(ICurrentThread)` (`test_main.cpp:11`) - a
  module requirement, not a check on any URI field.
- Production routes around the type: the 9 `->URI()` call sites under
  `src/framework/data/resources` return `std::string`
  (`resource.h:34`, `resource.cpp:33`), and `HTTPRequest::GetURI()`
  (`src/components/protocols/http/request/request.h:48`) does too -
  every URI the built tree handles is a string, and `qor_http.md:78`
  records http asking its own `GetURI()` for a "URI object".
- The composition pair cannot be called with a temporary:
  `URI(URI&, std::string_view)` and `URI(URI&, URI&)` (`uri.h:157-158`)
  take non-const lvalue references, so no base URI built on the fly
  binds - and the bodies drop the base anyway (`uri.cpp:23,28`).

## Failure modes

- The failure is silent vacuity, not a crash: the ctors compile, link
  and export (`dumpbin`: 7 exports), so a caller that believes it
  parsed a URI gets an object that holds nothing and no diagnostic -
  the C4100 at `uri.cpp(7,31)` is the only evidence the build gives,
  and it is a warning, not an error.
- The test cannot catch a regression: `test/uri/test_uri.cpp` is 17
  lines with no assertion, so a future implementation - or none -
  passes identically; what it really proves is that the header parses
  and the import library resolves.
- The wish-list is the trap: 73 enumerated names, 0 declared, so
  anyone adding `Parse` or `EscapeUriString` writes the first one into
  a class with no member to hold a result - while
  `docs/agent/add_module.md:27,83` sends new modules to copy this
  exact folder, which multiplies the empty skeleton rather than the
  missing parser.
- A green judge here certifies only compilation: the same command
  would pass if the folder held nothing but the bootstrap, because the
  link line's five unlinked-in libraries (`qor_host`, `qor_current`,
  `qor_sync`, `qor_injection`, `qor_parser`) contribute nothing that
  this folder's sources reference.

## See also

[[qor_module]] [[qor_host]] [[qor_current]] [[qor_sync]]
[[qor_injection]] [[qor_parser]] [[qor_echo]] [[qor_http]]
[[qor_pipeline]]
