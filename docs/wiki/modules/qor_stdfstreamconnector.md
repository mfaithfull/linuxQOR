---
Type: WikiPage
PageType: module
Title: "qor_stdfstreamconnector"
Target: qor_stdfstreamconnector
Status: current
LastVerified: 2026-11-18
Claims: 18
Summary: "The smallest configured connector: three std::fstream-owning classes in one dll whose base is a path-holder that never connects, pumping externalized to the sibling source and sink targets; judge-green, index-consistent for once - but FEATURES.md advertises a second target that does not exist and qor_signals is carried unused."
Anchors:
  - { kind: in-tree, path: src/components/io/pipeline/connectors/stdfstreamconnector, symbol: qor_stdfstreamconnector, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/stdfstreamconnector/CMakeLists.txt, symbol: qor_stdfstreamconnector, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/stdfstreamconnector/stdfstreamconnector.h, symbol: stdfstreamConnector, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/stdfstreamconnector/stdifstreamconnector.h, symbol: stdifstreamConnector, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/stdfstreamconnector/stdofstreamconnector.h, symbol: stdofstreamConnector, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sinks/stdofstreamsink/CMakeLists.txt, symbol: qor_stdfstreamconnector, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_stdfstreamconnector --parallel 8
---

# qor_stdfstreamconnector

The last connector `connectors/CMakeLists.txt:4` configures, and the most
honest shape in the family: it owns no pump elements, claims none, and the
judge is green on a fresh compile - exit 0 on 18 Nov 2026, all four declared
TUs compiled and the import library generated, 144,384-byte dll stamped
12:52, 32 `*stdfstreamconnector*` artifacts in `cmakebuild`. `repo_map.md:
108`, `deps.dot:129`, `purposes.md:89` and `FEATURES.md:76` index it.

- Three classes, one dll (`CMakeLists.txt:3`): base `stdfstreamConnector`
  (`stdfstreamconnector.h:13`), and `stdifstreamConnector` /
  `stdofstreamConnector` deriving from it (`stdifstreamconnector.h:13`,
  `stdofstreamconnector.h:13`) - the ifstream/ofstream variants are
  translation units of this target, not separate libraries.
- Naming breaks the family pattern: the base class starts lowercase -
  `stdfstreamConnector` (`stdfstreamconnector.h:13,17`) - where every other
  connector in the group is PascalCase (`FileConnector`, `PipeConnector`,
  `SocketConnector`); the namespace is the plain `qor::io::components`
  shared with file/pipe (`:11`).
- The base is a path-holder that never connects: it carries only
  `m_filePath` (`stdfstreamconnector.h:28`), its `Connect()` just returns
  `IsConnected()` (`stdfstreamconnector.cpp:29-32`) and `Disconnect()`
  resets the flag (`:34-37`) - the stream lives exclusively in the derived
  classes (`stdifstreamconnector.h:28`, `stdofstreamconnector.h:29`).
- `Connect()` in the derived pair is a real open: `m_ifstream.open(
  m_filePath, in | binary)` then `m_connected = is_open() && !fail()`
  (`stdifstreamconnector.cpp:24-25`), mirrored with `out | binary` at
  `stdofstreamconnector.cpp:24-25`; both then return
  `stdfstreamConnector::Connect()`, which echoes the flag they just set
  (`:26`).
- Pumping is deliberately externalized: unlike the three sibling
  connectors, there is no `iosource`/`iosink` template and no
  `GetSink`/`GetSource` here - the elements live in `qor_stdifstreamsource`
  and `qor_stdofstreamsink`, which link this target for exactly that
  reason (`sources/stdifstreamsource/CMakeLists.txt:14`,
  `sinks/stdofstreamsink/CMakeLists.txt:14`) and include the derived
  header directly (`stdifstreamsource.h:8`, `stdifstreamreader.h:11`).
- `SetFile(io::filesystem::Index&)` copies only the path string -
  `m_filePath = fileIndex.GetPath().ToString()`
  (`stdfstreamconnector.cpp:19-22`) - so unlike `FileConnector`'s stored
  caller-owned pointer, no lifetime question outlives the call.
- The derived destructors disconnect-if-connected
  (`stdifstreamconnector.cpp:14-20`, `stdofstreamconnector.cpp:14-20`)
  while the base destructor is `= default`
  (`stdfstreamconnector.cpp:17`) - the stream closes with its owner,
  nothing else to unwind.
- `qor_signals` is carried but unused: it is on the link line
  (`CMakeLists.txt:14`) and in the graph (`deps.dot:129`), yet no source in
  the folder includes anything from it - the same dead edge the
  fileconnector page recorded; `qor_pipeline` and `qor_filesystem` by
  contrast are genuinely used via `plug.h` (`stdfstreamconnector.h:9`) and
  `fileindex.h` (`:8`).
- Index consistency, a family first: `repo_map.md:108` lists exactly the
  four link libraries and `deps.dot:129` matches it verbatim - vs
  socketconnector's 8-vs-2 and pipeconnector's 5-vs-3 mismatches.
- `FEATURES.md:77` advertises `qor_stdifstreamconnector` as a separate
  "Stable" shared library - a phantom: the string occurs nowhere else in
  the tree and `add_library` creates only this one target
  (`CMakeLists.txt:3`); `FEATURES.md:76` rates the real target "Stable".
- `purposes.md:89` gives this target no dedicated row at all - one line
  lumps five std/console pipeline parts ("std-fstream/console pipeline
  parts") without naming what this connector adds.
- Consumer grep: five `qor_stdfstreamconnector` lines outside its own
  CMakeLists - the two family siblings above plus test wiring only
  (`test/operational/CMakeLists.txt:32`, `test/CMakeLists.txt:93` on the
  linux row, `:104` on the win row); no example links it.
- The guard-name fossil runs the other way: this folder's
  `stdofstreamconnector.h` guards are correct (`CONNECTIONS_STDOFSTREAM`,
  `:4,5,34`), while `socketconnector`'s `socketsink.h:26` closes with
  `//QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STDOFSTREAM` - the *sink* folder's
  guard name (`stdofstreamsink.h:4,5,23`), copied in as the endif comment.
- Folder: 8 files, 248 physical lines - four TUs (bootstrap 15,
  `stdfstreamconnector.cpp` 39, `stdifstreamconnector.cpp` 40,
  `stdofstreamconnector.cpp` 40) plus three headers (33+33+34) and the
  14-line `CMakeLists.txt`; no `purpose.md` in the folder.
- Judge evidence: `cmake --build cmakebuild --target qor_stdfstreamconnector
  --parallel 8` exit 0 on 18 Nov 2026, fresh compile of all four TUs and a
  new dll - the cheapest green judge in the group at 144 KB of output.

## Failure modes

- The base class is a compile-able dead end: `stdfstreamConnector` is
  exported (`stdfstreamconnector.h:13`) and usable as a `Plug`, but its
  `Connect()` can only return the flag nobody ever sets
  (`stdfstreamconnector.cpp:29-32`) - a pipeline wired against the base
  type fails silently at pump time, and nothing in the API says "use a
  derived class".
- `stdofstreamConnector::Connect()` is destructive before it is checked:
  `open(m_filePath, out | binary)` truncates or creates the file
  (`stdofstreamconnector.cpp:24`) on the way to computing `m_connected`
  (`:25`) - a caller that connects then abandons the pipeline has already
  wiped the target file.
- Trusting `FEATURES.md:77` breaks the build, not the compile: a
  `target_link_libraries` line citing `qor_stdifstreamconnector` fails at
  CMake generate time with a missing-target error - the phantom index row
  is one copy-paste away from a configure-time failure.

## See also

[[qor_fileconnector]] [[qor_socketconnector]] [[qor_pipeconnector]]
[[qor_pipeline]] [[qor_filesystem]] [[qor_module]] [[qor_signals]]

Not yet paged: `qor_stdofstreamsink` (this group, next leaf - its writer
header already mis-includes this folder's *ifstream* connector), and
`qor_stdifstreamsource` (later group).
