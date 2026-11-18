---
Type: WikiPage
PageType: module
Title: "qor_stdifstreamsource"
Target: qor_stdifstreamsource
Status: current
LastVerified: 2026-11-18
Claims: 23
Summary: "The ifstream half of the std pipeline pair: a configured, judge-green source plus a self-contained stdifstreamReader facade with the flyer discipline its ofstream sibling dropped - but ReadAll never breaks on a zero-returning process function (permanent spin), and a failed open still stamps the reader ready."
Anchors:
  - { kind: in-tree, path: src/components/io/pipeline/sources/stdifstreamsource, symbol: qor_stdifstreamsource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sources/stdifstreamsource/CMakeLists.txt, symbol: qor_stdifstreamsource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sources/stdifstreamsource/stdifstreamsource.h, symbol: stdifstreamSource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sources/stdifstreamsource/stdifstreamreader.h, symbol: stdifstreamReader, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sources/stdifstreamsource/stdifstreamreader.cpp, symbol: stdifstreamReader::ReadAll, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sources/CMakeLists.txt, symbol: stdifstreamsource, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_stdifstreamsource --parallel 8
---

# qor_stdifstreamsource

Configured at `sources/CMakeLists.txt:2`, judge-green on 18 Nov 2026 (exit
0, all three declared TUs compiled fresh, import library generated,
1,044,480-byte dll stamped 13:15). Test-only reachability: no example or
production consumer names any symbol here - the only consumers in the
grep are `test/components/pipeline/test_ifstreamsource.cpp` and the
*ofstream sink's own test* (`test_ofstreamsink.cpp:36-37` include both of
this folder's headers). `repo_map.md:111`, `deps.dot:132` and
`FEATURES.md:78` index it; `purposes.md:89` lumps it with four other
std/console pipeline parts.

- Two classes in one dll (`CMakeLists.txt:3`): `stdifstreamSource :
  pipeline::iosource<stdifstreamConnector>` (`stdifstreamsource.h:12`)
  and the facade `stdifstreamReader : pipeline::Pipeline`
  (`stdifstreamreader.h:16`) - the lowercase-class naming of the
  stdfstream pair, mirrored from the ofstream sink folder.
- The sink-side asymmetry is corrected here: `ReadBytes` gates on
  `m_Connector && m_Connector->IsConnected()` (`stdifstreamsource.cpp:39`),
  reads via `Stream().read` + `gcount()` (`:41-42`), and the unconnected
  branch fires `continuable("Attempting to read from closed stream.")`
  (`:46`) - the flyer discipline socketsink/socketsource keep and
  `stdofstreamsink` dropped.
- Two license dialects in one folder: `stdifstreamsource.cpp:1-23` carries
  the old BSD-style full-text QOR license with a pinned "2008 - 2025"
  range (`:1`), while every sibling file - including its own header - uses
  the one-line SPDX BSL-1.0 header.
- Fossil includes in that TU: `<stdio.h>` (`:26`) and `pipeline/sink.h`
  (`:28`) - the file names no stdio function and no `Sink` symbol.
- The reader's members make it a whole read-file pipeline with no
  external wiring: `ByteBuffer m_buffer`, `stdifstreamConnector
  m_connector`, a `stdifstreamSource source` member spelled lower-case
  (`stdifstreamreader.h:42`) and a `pipeline::NullSink m_sink` (`:43`).
- `Write` is refused in-type: `virtual bool Write(...) { return false; }`
  (`stdifstreamreader.h:37`) - the exact mirror of the writer sibling's
  constant-false `Read` (`stdofstreamwriter.h:33`).
- Dead typedef: the raw C function-pointer `process_data_func`
  (`stdifstreamreader.h:20`) appears in no signature anywhere; the live
  API is the `std::function` alias `ProcessDataFn` (`:21`).
- `Setup()` checks what the writer ignores: after `m_connector.Connect()`
  it re-tests `IsConnected()` and fires `continuable("Failed to open
  {0}.")` (`stdifstreamreader.cpp:61-67`) - the ofstream writer discards
  that same `bool` (`stdofstreamwriter.cpp:63`) - yet `m_Setup = true`
  still lands unconditionally after the flyer (`:69`).
- Constructor fossil repeated: the string-name ctor instantiates a
  throwaway whole `io::FileSystem` (`new_ref<io::FileSystem>()` +
  `Setup()`, `stdifstreamreader.cpp:14-15`) just for `CurrentPath()` -
  the identical pattern the writer page records at
  `stdofstreamwriter.cpp:14-15`; the Index-taking ctor (`:22`) skips it.
- `m_Setup` is initialized in two of three ctor init-lists
  (`stdifstreamreader.cpp:10,22`) but not in the string-name ctor
  (`:12-20`) - harmless only because that body ends by calling `Setup()`
  (`:19`), which stamps it.
- `Read` is lazy: `if(!m_Setup) Setup();` then delegate to
  `source.Read` (`stdifstreamreader.cpp:72-79`) - construction never
  opens the file unless a ctor path did.
- `ReadAll` pumps by buffer capacity: `WriteCapacity()` units per
  iteration (`:87-88`), then `process(data, bytesToProcess)` (`:93`) and
  `ReadAcknowledge` **only when `bytesProcessed > 0`** (`:94-98`), looping
  `while (unitsReadAtOnce > 0)` (`:99`).
- The whole-file convenience `std::string Read()` (`:102-113`) runs
  `ReadAll` with a lambda that appends by value: `strText = strText +
  temp` (`:109`) - every chunk copies the whole text accumulated so far.
- Index vs link, this time with the index behind: the link line names
  five libraries (`CMakeLists.txt:13-14`: `qor_module qor_signals
  qor_pipeline qor_filesystem qor_stdfstreamconnector`) while
  `deps.dot:132` and `repo_map.md:111` show three - and the two invisible
  edges are genuinely used (`filesystem.h` at `stdifstreamreader.cpp:6`;
  the connector header at `stdifstreamsource.h:8` and
  `stdifstreamreader.h:11`, the edge the `qor_stdfstreamconnector` page
  already cites); `qor_signals` is named by no source in the folder and
  arrives through the base classes.
- Guards are clean both ways: `QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STDIFSTREAM`
  (`stdifstreamsource.h:4,26`) and
  `..._STDIFSTREAMREADER` (`stdifstreamreader.h:4,48`) each appear at
  their own six sites only - no misplaced-`#endif` fossil like the one
  socketconnector's page records for the ofstream guard.
- `FEATURES.md:78` rates it "Stable - std::ifstream Source for Pipeline";
  it sits one row above the phantom `qor_stdifstreamconnector` row at
  `:77` that the group 4 close recorded, another reminder the column is
  not maintained against the build graph.
- Test surface, counted not read: `test/CMakeLists.txt:66` compiles
  `test_ifstreamsource.cpp`, whose suite drives the source
  (`:57,66`) and the reader (`canSetupstdifstreamReader` `:78`,
  construction `:84`); `test_ofstreamsink.cpp:87` uses
  `stdifstreamReader(...).Read()` - the ofstream sink's test is this
  target's heaviest user.
- Bootstrap and export: 15-line bootstrap naming "Querysoft Open Runtime:
  std::ifstream Source Module" (`qor_stdifstreamsource.cpp:12`); the
  export macro rides both public classes
  (`stdifstreamsource.h:12`, `stdifstreamreader.h:16`) - no export miss.
- Folder: 6 files, 269 physical lines - TUs 181 (bootstrap 15,
  `stdifstreamsource.cpp` 51, `stdifstreamreader.cpp` 115), headers 74
  (26+48), `CMakeLists.txt` 14; no `purpose.md` in the folder.
- Judge evidence: `cmake --build cmakebuild --target qor_stdifstreamsource
  --parallel 8` exit 0 on 18 Nov 2026, three fresh compiles and a fresh
  link - green certifies compilation only: the spin and ready-after-failed-open
  paths below compile clean.

## Failure modes

- `ReadAll` spins forever on a refusing consumer: if the process function
  returns 0, nothing is ever acknowledged (`stdifstreamreader.cpp:94-98`)
  and `unitsReadAtOnce` keeps coming back positive for the same
  unacknowledged region, so the `while (unitsReadAtOnce > 0)` loop
  (`:99`) has no exit - a consumer that rejects a chunk hangs the reader
  instead of stopping it (the ofstream writer's analogue returned
  `true` having written nothing; this one never returns at all).
- Failed open is still "ready": `Setup()` fires the flyer but stamps
  `m_Setup = true` regardless (`stdifstreamreader.cpp:69`), so every
  later `Read` walks into `ReadBytes`' closed-stream branch and fires
  `continuable` again (`stdifstreamsource.cpp:46`) while returning 0 -
  a typo'd path is a per-read flyer storm with no distinct status code.
- `Read()` has no size bound: the 4096-byte buffer capacity (ctor
  default, `stdifstreamreader.h:24-25`) bounds each chunk, not the
  whole - `ReadAll` re-enters until EOF and the lambda
  grows one string by value-copying it per chunk (`stdifstreamreader.cpp:106-111`),
  so total RAM is the file size and the copy work is quadratic in it.

## See also

[[qor_stdofstreamsink]] [[qor_stdfstreamconnector]] [[qor_stdoutsink]]
[[qor_pipeline]] [[qor_filesystem]] [[qor_signals]] [[qor_module]]
[[qor_fileconnector]]

Not yet paged: `qor_stdinsource` - the next leaf of this group.
