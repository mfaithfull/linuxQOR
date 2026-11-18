---
Type: WikiPage
PageType: module
Title: "qor_stdofstreamsink"
Target: qor_stdofstreamsink
Status: current
LastVerified: 2026-11-18
Claims: 19
Summary: "The one target only tests link: a configured, judge-green ofstream sink plus a self-contained stdofstreamWriter facade - but the sink reports write success blind without checking stream state, the writer discards Connect()'s failure and silently writes zero bytes, and the header pulls the ifstream connector it does not use while missing the ofstream one it does."
Anchors:
  - { kind: in-tree, path: src/components/io/pipeline/sinks/stdofstreamsink, symbol: qor_stdofstreamsink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sinks/stdofstreamsink/CMakeLists.txt, symbol: qor_stdofstreamsink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sinks/stdofstreamsink/stdofstreamsink.h, symbol: stdofstreamSink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sinks/stdofstreamsink/stdofstreamwriter.h, symbol: stdofstreamWriter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sinks/CMakeLists.txt, symbol: stdofstreamsink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/connectors/stdfstreamconnector/stdofstreamconnector.h, symbol: stdofstreamConnector, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_stdofstreamsink --parallel 8
---

# qor_stdofstreamsink

Configured at `sinks/CMakeLists.txt:1`, judge-green on 18 Nov 2026 (exit 0,
all three declared TUs compiled fresh, import library generated, 1,022,976-
byte dll stamped 12:56 against a newest source of 15 Oct, 32
`*stdofstreamsink*` artifacts) - and the group's only target with no
production consumer: the linking rows are `test/operational/
CMakeLists.txt:33` and the win/linux test rows at `test/CMakeLists.txt:94,
105`. `repo_map.md:109`, `deps.dot:130` and `FEATURES.md:80` index it;
`purposes.md:89` lumps it with four other std/console pipeline parts.

- Two classes in one dll (`CMakeLists.txt:3`): `stdofstreamSink :
  pipeline::iosink<stdofstreamConnector>` (`stdofstreamsink.h:12`) in
  `qor::io::components` (`:10`), and `stdofstreamWriter :
  pipeline::Pipeline` (`stdofstreamwriter.h:16`) - the lowercase-class
  naming of the stdfstream pair continues here.
- The sink half is seven lines of pump: `WriteBytes` gates on
  `m_Connector && m_Connector->IsConnected()` (`stdofstreamsink.cpp:17`)
  and returns 0 otherwise - unlike socketsink/socketsource, no
  `continuable` flyer on the failure branch, just silence (`:14-23`).
- Success is asserted blind: after `m_Connector->Stream().write(...)`
  the sink returns `result = bytesToWrite` unconditionally
  (`stdofstreamsink.cpp:19-20`) - no `fail()`/`bad()` check on the stream,
  so a failed `ofstream` write is reported to the pipeline as a full
  write.
- The cast is a fossil: the `ofstream` write path casts through
  `std::ifstream::char_type*` (`stdofstreamsink.cpp:19`) - wrong stream
  class, harmless typedef, symptomatic of the copy.
- The real product is the writer facade: `stdofstreamWriter` owns a
  `pipeline::ByteBuffer m_buffer`, a `stdofstreamConnector m_connector`,
  a `stdofstreamSink m_sink` and a `pipeline::NullSource` as members
  (`stdofstreamwriter.h:36-39`) - a self-contained write-to-file pipeline
  with no external wiring needed.
- Its header include is wrong in both directions: `stdofstreamwriter.h:11`
  includes the *ifstream* connector header it never uses, while its
  member at `:37` is `stdofstreamConnector`, declared only because
  `:12` pulls `stdofstreamsink.h`, which includes the ofstream connector
  at its `:8` - the header does not compile standalone against its own
  types.
- Setup is lazy and error-discarding: `Setup()` wires buffer, source and
  plug, then `m_connector.Connect()` with the `bool` result discarded
  (`stdofstreamwriter.cpp:61-64`) and `m_Setup = true` regardless
  (`:65`); `Write` calls `Setup()` on demand (`:70-73`) - a failed
  connect leaves a "ready" writer whose every write lands in the
  disconnected branch of the sink.
- The file-name constructor instantiates a throwaway whole
  `io::FileSystem` (`new_ref<io::FileSystem>()` + `Setup()`,
  `stdofstreamwriter.cpp:14-15`) merely to call `CurrentPath()` for a
  stack-local `Index` (`:16`); `SetFile` then copies only the path string
  into the connector (`:29-32`) - no lifetime coupling, matching the
  leaf-connector's pattern.
- The string `Write` loop does its unit arithmetic correctly -
  `memcpy(space, ..., unitsToWrite * m_buffer.GetUnitSize())`
  (`stdofstreamwriter.cpp:88`) - the exact fix the socketconnector's
  `socketwriter.h:53` never made.
- `Read` is refused in-type: `virtual bool Read(...) { return false; }`
  (`stdofstreamwriter.h:33`) - the Pipeline base demands both directions,
  answered with a constant.
- Index vs link: the link line names five libraries
  (`CMakeLists.txt:14-15`: `qor_module qor_signals qor_pipeline
  qor_filesystem qor_stdfstreamconnector`) while `deps.dot:130` and
  `repo_map.md:109` show three - the two invisible edges are genuinely
  used (`filesystem.h` at `stdofstreamwriter.cpp:6`, the connector header
  at `stdofstreamsink.h:8`), and `qor_signals`, in both link and graph,
  is used by no source in the folder - the third repetition of that dead
  edge in this group.
- The guard name here is the fossil's origin:
  `QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STDOFSTREAM` opens and closes
  `stdofstreamsink.h:4,23` - and sits as the wrong `#endif` comment at
  socketconnector's `socketsink.h:26` (that folder's own guard,
  `stdofstreamconnector.h:4,34`, is correct `CONNECTIONS_STDOFSTREAM`).
- `FEATURES.md:80` rates it "Stable - std::ofstream Sink for Pipeline";
  the phantom sibling row at `FEATURES.md:77` (a
  `qor_stdifstreamconnector` that no `add_library` ever creates) shows
  the stability column is not maintained against the build graph.
- Bootstrap and module name: 15-line bootstrap naming "Querysoft Open
  Runtime: std::ofstream Sink Module" (`qor_stdofstreamsink.cpp:12`);
  both public classes carry the export macro (`stdofstreamsink.h:12`,
  `stdofstreamwriter.h:16`) - no socketwriter-style export miss here.
- Folder: 6 files, 225 physical lines - TUs 143 (bootstrap 15,
  `stdofstreamsink.cpp` 25, `stdofstreamwriter.cpp` 104), headers 67
  (23+44), `CMakeLists.txt` 14; no `purpose.md` in the folder.
- Judge evidence: `cmake --build cmakebuild --target qor_stdofstreamsink
  --parallel 8` exit 0 on 18 Nov 2026, fresh compile and link - green
  again certifies compilation only: every silent-failure path below
  compiles clean.

## Failure modes

- Silent zero-byte loss with a success return: unconnected or failed
  stream makes `WriteBytes` return 0 (`stdofstreamsink.cpp:16-22`); the
  writer's string loop then breaks on `unitsWrittenAtOnce == 0` and
  `return true` (`stdofstreamwriter.cpp:94-96`) - the caller gets `true`
  having written nothing, and the sink's blind `result = bytesToWrite`
  (`:20`) covers for a mid-stream ofstream failure the same way.
- Lazy connect truncates before anything is checked: the first `Write`
  triggers `Setup()` → `Connect()` → `open(m_filePath, out | binary)`
  (`stdofstreamwriter.cpp:63` → `stdofstreamconnector.cpp:24`), which
  creates-or-truncates the target file, and the `bool` it returns is
  discarded (`stdofstreamwriter.cpp:63`) - a typo'd or unwritable path
  still costs an existing file's contents.
- The writer header is a include-graph landmine: `stdofstreamwriter.h`
  alone (without `stdofstreamsink.h` arriving first) fails to compile -
  its own member type comes from a header it never includes
  (`:11` names the wrong one) - so any new consumer that trusts the
  header's own includes gets a wall of undeclared-identifier errors.

## See also

[[qor_stdfstreamconnector]] [[qor_socketconnector]] [[qor_fileconnector]]
[[qor_pipeline]] [[qor_filesystem]] [[qor_module]] [[qor_signals]]
[[qor_sync]]

Not yet paged: `qor_stdifstreamsource` (its companion on the same test
rows) and `qor_stdoutsink` / `qor_stdinsource` - later groups.
