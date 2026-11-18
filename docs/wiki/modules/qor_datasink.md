---
Type: WikiPage
PageType: module
Title: "qor_datasink"
Target: qor_datasink
Status: current
LastVerified: 2026-11-18
Claims: 15
Summary: "The pull-mode read end of the pipeline: a 115-line Sink whose Extract pumps a delegate callback, swaps byte order chunk-safely, and is wrapped by a header-only reflection-driven Reader - whose Read helpers throw away the one bool that reports truncation."
Anchors:
  - { kind: in-tree, path: src/components/data/pipeline/sinks/datasink, symbol: qor_datasink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sinks/datasink/CMakeLists.txt, symbol: qor_datasink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sinks/datasink/datasink.h, symbol: DataSink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sinks/datasink/datasink.cpp, symbol: DataSink::Extract, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sinks/datasink/reader.h, symbol: Reader, verified: 2026-11-18 }
  - { kind: in-tree, path: test/pipeline/CMakeLists.txt, symbol: qor_datasink, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_datasink --parallel 8
---

# qor_datasink

Configured at `src/components/data/pipeline/sinks/CMakeLists.txt:1`; the folder is 5 files,
323 lines: 13-line `CMakeLists.txt`, 49-line `datasink.h`, 115-line `datasink.cpp`, 110-line
`reader.h`, 36-line `qor_datasink.cpp`. It is the "Stable" one of the four sinks -
`FEATURES.md:52` gives it the family's only full-sentence row ("Binary and structured data sink
for Pipeline"), where stringsink gets `:82`, parsersink `:92` and deserializersink `:103`.

- `DataSink : public qor::pipeline::Sink` (`datasink.h:18`) is a read end, not a write end:
  `Extract(byte*, size_t)` (`datasink.cpp:27-43`) pumps until the count arrives or the source
  runs dry, and its verdict is `totalPumped == byteCount` (`:42`).
- The sink has no pump of its own: `Push` (`:48-67`) only reports
  `min(unitsToWrite, buffer->ReadCapacity())` (`:55`) - the driving force is the
  `Delegate<bool(size_t&, size_t)>` callback (`datasink.h:44`) invoked at `datasink.cpp:35`,
  which `reader.h:103-104` binds to `Pipeline::Pump`. That is why a sink folder ships a
  pipeline-wiring header.
- `reader.h` (110 lines) is a header-only `Reader` (`:22`): it owns `DataSink`, `ByteBuffer` and
  `Pipeline` (`:88-90`), sets `Element::Pull` (`:102`), and exposes `Read<T>` constrained to
  arithmetic types (`:54`) and `operator>>` constrained to standard-layout + trivially-copyable
  (`:67`); `ReadStructure` (`:74-84`) extracts a struct field-by-field through
  `qor::reflection::for_each_field` (`:78`).
- Byte order is a member, defaulting to `arch::Endian::network` (`datasink.h:43`); when it
  differs from the host, `ExtractBufferValue` (`datasink.cpp:98-104`) reverses each arriving
  chunk into `space[index + (size-totalRead)]` - chunks land at their final little-endian
  offsets no matter how the stream splits the value; the host-order path fills forward
  (`:109`).
- The judge compiles both named `.cpp` (`CMakeLists.txt:3`) into a dll with 0 warnings -
  cleanest of the sinks so far.
- Link line is the quartet `qor_module qor_signals qor_pipeline qor_error` (`:13`), matching
  `docs/agent/repo_map.md:90` and `docs/agent/deps.dot:111` - but `qor_error` appears nowhere
  in the four sources: the only "error" in the folder is that link line.
- License is split inside the folder: `datasink.h`, `datasink.cpp` and `reader.h` carry the
  3-line BSL-1.0 SPDX header (`:2` each), while the bootstrap `qor_datasink.cpp` carries a
  23-line MIT-style block dated "2008 - 2025" (`:1-23`) and no SPDX line.
- Consumers are real and configured: the `QORTestPipelines` target links it
  (`test/pipeline/CMakeLists.txt:15`, configured at `test/CMakeLists.txt:121`);
  `testwriter.cpp:37` includes `reader.h` and uses a bare `DataSink` twice (`:81`, `:130`);
  `testttfreader.cpp:37` includes `reader.h`.
- The dependency leaks upward: `src/components/data/formats/ttf/reader.h:30` includes
  `datasink/reader.h`, yet `qor_ttf`'s own link line carries no datasink lib at all - the
  test target pays the difference by linking `qor_datasink qor_datasource qor_fileconnector`
  at `test/pipeline/CMakeLists.txt:15`.
- `docs/agent/purposes.md:91` compresses all four sinks into one row, "pipeline sinks".

## Failure modes

- Truncation reads as zero: `Reader::Read<T>()` (`reader.h:54-60`) discards `Extract`'s bool
  and returns `t{0}`; `operator>>` (`:70`) and `ReadStructure`'s `T result{}` (`:77`) do the
  same - on a dead stream a missing `int` is indistinguishable from a zero one, and the only
  witness (`datasink.cpp:42`) is never read.
- `Extract` is total, not counted: the loop (`:32-41`) copies every delivered chunk into the
  caller's `space` before the final `totalPumped == byteCount` check (`:42`), so a failed
  extract leaves partially-valid bytes with no count of what is valid.
- Callback verdict discarded: `m_callBack(unitsPumped, byteCount - totalPumped)` at `:35`
  throws away the delegate's `bool` - a pump that reports failure yet delivered bytes is
  consumed, and one that fails silently at zero is identical to end-of-stream (`:41-42`).
- Unchecked `dynamic_cast`: the `Plug` ctor (`reader.h:27-30`) casts to `Source*` with no
  null test and `SetupPipeline` dereferences `source->SetBuffer(...)` (`:97`) - a Plug that
  resolves to anything but a Source crashes at construction, not at first read.
- The public header carries an unused cross-component include: `reader.h:13` includes
  `fileconnector/fileconnector.h` and the file never names `FileConnector` - every includer
  of `datasink/reader.h` inherits the connectors' include tree, which is the same coupling
  that forces `qor_ttf` consumers to link `qor_fileconnector`.

## See also

[[qor_module]] [[qor_signals]] [[qor_pipeline]] [[qor_ttf]] [[qor_unicodetextfilter]]
