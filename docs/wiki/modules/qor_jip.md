---
Type: WikiPage
PageType: module
Title: "qor_jip"
Target: qor_jip
Status: current
LastVerified: 2026-11-18
Claims: 21
Summary: "The only target in the group that never enters the configure tree: no add_subdirectory anywhere (judge fails MSB1009), and its add_library names two .cpp files that do not exist on disk. The folder is a half-migrated copy of echo with the entire parser layer deleted mid-move, zero consumers tree-wide, and no FEATURES.md row - a declared target the build has never seen."
Anchors:
  - { kind: in-tree, path: src/components/protocols/jip, symbol: qor_jip, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/jip/CMakeLists.txt, symbol: qor_jip, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/jip/jipprotocol.cpp, symbol: JipProtocol::GetAddressFamily, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/jip/jipservicefilter.cpp, symbol: JipServiceFilter::WriteAcknowledge, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/jip/jipresponsefilter.h, symbol: JipResponseFilter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/protocols/jip/qor_jip.cpp, symbol: ThisModule, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_jip --parallel 8
---

# qor_jip

Judge-RED on 18 Nov 2026, and red at the front door:
`cmake --build cmakebuild --target qor_jip --parallel 8` exits 1 with
`MSBUILD : error MSB1009: Project file does not exist. Switch:
qor_jip.vcxproj` - the target has no project in the solution because
configure never saw the folder. Index rows exist anyway:
`repo_map.md:116`, `deps.dot:50,136`, `deps.svg:2336-2368`,
`purposes.md:95`. The census counts the `set(target qor_jip)` line,
and this page answers what the line is standing for.

- **Never configured:** `protocols/CMakeLists.txt:1-4` adds `echo`,
  `uri`, `yaml`, `http` - not `jip` - and no CMakeLists anywhere else
  mentions the folder (grep, all CMakeLists). The target is declared
  (`CMakeLists.txt:1`) but no build system ever reads the file.
- **Not even configurable:** `add_library` lists `jipresponseparser.cpp`
  and `jiprequestparser.cpp` (`CMakeLists.txt:4-5`) - neither file
  exists in the folder, which holds 10 files (listing). Adding the
  subdirectory would die at "Cannot find source file".
- The parser layer is missing, not hidden: no `jiprequestparser.*`,
  no `jipresponseparser.*`, no `requestnode.h`, no `responsenode.h` -
  yet `jipservicefilter.h:9` includes `"requestnode.h"` (resolves
  nowhere in the folder) and `jipservicefilter.cpp:21-22` instantiates
  `qor::components::protocols::jip::request`, a class that does not
  exist.
- Cross-folder graft: `jipresponsefilter.h:11` includes
  `"echoresponse.h"` - the **echo** header - while its own jip
  includes sit commented out one line above: `//#include
  "jipresponseparser.h"`, `//#include "responsenode.h"` (`:9-10`).
  The .cpp still uses the commented-away classes: `new_ref<response>`
  (`jipresponsefilter.cpp:19`) and `AsRef<ResponseNode>()` (`:33`).
- Folder: 10 files, 353 physical lines - against echo's 16/714.
  The shape is the echo skeleton minus every parser file.
- The filters are echo clones down to the structure - ctor,
  `WriteAcknowledge` body, `Handle*` memcpy, parser re-arm
  (`jipservicefilter.cpp:19-59` vs `echoservicefilter.cpp:19-58`) -
  but the buffer is one unit: `InlineFilter<byte>(1)`
  (`jipservicefilter.cpp:19`, `jipresponsefilter.cpp:17`). echo
  hard-codes 512; http defaults to 0; jip says 1.
- Different context type: `data::parser::Context m_sourceContext`
  (`jipservicefilter.h:29`, `jipresponsefilter.h:28`) constructed from
  the buffer directly (`:19`), where echo uses
  `pipeline::BufferContext<byte>` (`echoservicefilter.h:29`).
- The discarded parse result repeats: `int parseResult =
  ...FinalParse()` never read (`jipservicefilter.cpp:32`,
  `jipresponsefilter.cpp:29`) - echo's C4189 pattern, uncompiled here.
- JSON over IP contains no JSON: the module bills itself "Querysoft
  Open Runtime: JSON over IP Protocol Module" (`qor_jip.cpp:14`);
  `JipRequest`/`JipResponse` are the echo string-holders carrying
  `//TODO: This gets replaced with the proper JSON Object`
  (`jiprequest.h:9`, `jipresponse.h:9`).
- The server is echo's bounce with an apology: `//TODO: Replace this
  with JSON to pipeline generator` above the same memcpy of request
  to sink (`jipservicefilter.cpp:47-54`).
- `JipProtocol` is 24 lines: `: public qor::pipeline::Protocol`
  (`jipprotocol.h:11`), `AF_INet` (`jipprotocol.cpp:15-18`), two
  factory filters (`:20-28`) - echo's shape with no size-cap method
  at all.
- Bootstrap differs once: 18 lines with `ThisModule` wrapped in
  `extern "C"` (`qor_jip.cpp:10-17`) - echo's and http's bootstraps
  do not do this (`qor_echo.cpp:10`, `qor_http.cpp:10`).
- The override asymmetry repeats: `WriteAcknowledge` marked `override`
  on the service filter (`jipservicefilter.h:23`) but not on the
  response filter (`jipresponsefilter.h:22`) - echo's same drift.
- Guards: `QOR_PP_H_COMPONENTS_PROTOCOLS_JIP_*` in 5 headers, verified
  unique tree-wide (15 hits, all in-folder).
- Index drift, third repetition: link line names six libraries
  (`CMakeLists.txt:15-16`, ending `qor_parser`); `repo_map.md:116`,
  `deps.dot:136` and the five `deps.svg:2342-2368` edges all show
  five.
- `FEATURES.md` has no `qor_jip` row at all (grep) - `qor_echo`
  appears at `:54` and `qor_http` at `:97`; `purposes.md:95` says
  two words: "JIP protocol."
- Consumers: zero. No example, no test, no CMakeLists, no source
  outside the folder names `qor_jip` or any `Jip*` class (greps over
  `src`, `examples`, `test`).
- Other wiki pages already lean on it: `qor_parser.md:103` counts
  `qor_jip` among the grammars that consume `qor_parser`, and
  `qor_uiselector.md:31` lists it among targets - both true at
  source level, both describing a library the build has never made.

## Failure modes

- The target is unbuildable at two levels and fixing one does not
  reach the other: configure never adds the folder (MSB1009 on the
  judge), and adding it fails immediately on the two nonexistent
  sources in `CMakeLists.txt:4-5`; past that, compilation dies on the
  `requestnode.h` include (`jipservicefilter.h:9`) and the
  nonexistent `request`/`response` instantiations
  (`jipservicefilter.cpp:21`, `jipresponsefilter.cpp:19`).
- The response path is mid-graft: its parser headers are commented
  out (`jipresponsefilter.h:9-10`) and replaced by the **echo**
  header (`:11`) whose types do not match the code that uses them
  (`response`, `ResponseNode` at `jipresponsefilter.cpp:19,33`) -
  nothing in that translation unit resolves, in this tree or any
  other.
- Even a hypothetical fix negotiates one byte at a time:
  `InlineFilter<byte>(1)` in both filter ctors
  (`jipservicefilter.cpp:19`, `jipresponsefilter.cpp:17`) caps every
  WriteRequest negotiation at one unit - for a protocol whose name
  promises JSON documents; `purposes.md:95` and the module banner
  promise JSON-over-IP, the code memcpys bytes back unchanged
  (`:48-54`).

## See also

[[qor_echo]] [[qor_http]] [[qor_parser]] [[qor_pipeline]] [[qor_uri]]
[[qor_module]]
