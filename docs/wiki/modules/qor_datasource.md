---
Type: WikiPage
PageType: module
Title: "qor_datasource"
Target: qor_datasource
Status: current
LastVerified: 2026-11-18
Claims: 17
Summary: "The write end of the datasink pair: a Stable source that inserts structs through a header-only Push-mode Writer whose Write discards Insert's bool - and Insert itself reports success for bytes it never acknowledged when the last write grant comes back null."
Anchors:
  - { kind: in-tree, path: src/components/data/pipeline/sources/datasource, symbol: qor_datasource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/datasource/CMakeLists.txt, symbol: qor_datasource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/datasource/datasource.h, symbol: DataSource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/datasource/datasource.cpp, symbol: DataSource::Insert, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/datasource/writer.h, symbol: Writer, verified: 2026-11-18 }
  - { kind: in-tree, path: test/pipeline/CMakeLists.txt, symbol: qor_datasource, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_datasource --parallel 8
---

# qor_datasource

Configured at `src/components/data/pipeline/sources/CMakeLists.txt:5`; the folder is 5 files,
263 lines: 13-line `CMakeLists.txt`, 48-line `datasource.h`, 102-line `datasource.cpp`,
15-line bootstrap, 85-line `writer.h`. `FEATURES.md:53` carries it as "Stable - Binary and
structured data source for Pipeline", the direct twin of datasink's row at `:52`; the pair is
the same product read from each end.

- The mirror of datasink: `DataSource : public pipeline::Source` (`datasource.h:18`),
  `m_outputByteOrder{arch::Endian::network}` (`:42`) matching datasink's `:43` default, and
  the `Insert<T>` constraint `std::is_standard_layout_v<T> && std::is_trivially_copyable_v<T>`
  (`:30`) matching `reader.h`'s `operator>>`.
- Marking is the linereader family, not the sink family: the class carries the gcc form
  `qor_pp_module_interface_gcc(QOR_DATASOURCE)` (`datasource.h:18`) plus seven individually
  marked members (`:23-28`, `:40`).
- Namespace split: the class lives in `qor::pipeline` (`datasource.h:16`) while all four
  sibling sinks sit in `qor::pipeline::components` (`stringsink.h:10`) - the mirror pair is
  not symmetric even in its namespace.
- `writer.h` (85 lines) is a header-only `Writer` (`:21`) owning `DataSource`, `ByteBuffer`
  and `Pipeline` (`:62-64`); it sets `Element::Push` (`:77`) where datasink's `Reader` sets
  Pull, and binds the source's callback to `Pipeline::Pump` (`:78`).
- `Insert` (`datasource.cpp:43-63`) is the product: loop `WriteRequest(byteCount -
  totalWritten)` (`:53`), place via `InsertBufferValue` and `WriteAcknowledge(realWrite)`
  (`:57-58`), pump `m_callBack` between chunks (`:62`), verdict `totalWritten == byteCount`
  (`:63`).
- Byte order is swapped chunk-safely: `space[realWrite - (index + 1)] = data[index +
  (size - totalWritten)]` (`:92`) lands each arriving chunk at its final offset - the same
  trick datasink's `ExtractBufferValue` (`datasink.cpp:98-104`) runs on the read side; the
  host-order path memcpy's at `space + (totalWritten - realWrite)` (`:98`).
- `Pull` (`datasource.cpp:66-83`): null buffer returns false (`:81`); otherwise
  `unitsRead = std::min(unitsToRead, buffer->ReadCapacity())` (`:72`) and true.
- Link line is the quartet `qor_module qor_signals qor_pipeline qor_error`
  (`CMakeLists.txt:13`), matching `docs/agent/repo_map.md:95` - and `qor_error` is named by
  no source in the folder, the same orphan edge datasink carries.
- License splits down the middle of the folder: `datasource.h` and the bootstrap carry the
  BSL-1.0 SPDX ("2008 - Present", `:1-2`), while `datasource.cpp` carries the 23-line MIT
  block dated "2008 - 2025" - and the 15-line bootstrap is the short SPDX-era `ThisModule()`
  shape that unicodetextfilter uses, naming "Querysoft Open Runtime: Data Source Module".
- Consumers are the pipeline test tree: `QORTestPipelines` links it
  (`test/pipeline/CMakeLists.txt:15`, configured at `test/CMakeLists.txt:121`);
  `testwriter.cpp:38` and `testttfreader.cpp:38` include `writer.h`, and `testwriter.cpp`
  instantiates bare `DataSource` objects twice (`:59`, `:107`).
- The judge compiles both named `.cpp` (`CMakeLists.txt:3`) into an 86,016-byte dll with 0
  warnings (run 18 Nov 2026).

## Failure modes

- The wrapper discards the verdict: `Write` (`writer.h:50`) and `operator<<` (`:56`) drop
  the `bool` from `Insert`, whose only witness is `datasource.cpp:63` - a failed insert is
  invisible at the Writer end, the same shape as datasink's `Reader::Read` throwing away
  `Extract`'s bool.
- The grant is counted before it succeeds: `totalWritten += realWrite` (`datasource.cpp:54`)
  runs before the `if(space)` test (`:55`), so a null final `WriteRequest` skips the write
  and acknowledge yet still reports `totalWritten == byteCount` as success (`:63`) - the
  caller gets `true` for bytes that were never acknowledged.
- Unchecked buffer: `Insert` takes `GetBuffer()` (`:45`) and dereferences it at `:53` with
  no null test - a bare `DataSource` with no `Writer` behind it (legal at
  `testwriter.cpp:59`) crashes on first insert, while its own `Pull` does check (`:69`).
- Pull promises bytes it never copies: `unitsRead` is the capacity clamp (`:72`) and the
  call returns true without moving a byte - data only appears from a prior `Insert` or the
  Push callback, and `OnEndOfData()` fires whenever that clamp hits zero (`:73-75`), which
  includes the buffer being full.
- The callback's progress out-param is dead: `unitsPumped = 0` (`:61`) is handed to
  `m_callBack(unitsPumped, realWrite)` (`:62`) and never read afterwards - the Pump report
  is discarded at the source end, mirroring datasink.cpp:35.
- Unchecked Plug cast: `dynamic_cast<Sink*>` (`writer.h:27`) with no null test before
  `sink->SetBuffer(...)` (`:71`) - a Plug that resolves to anything but a Sink crashes at
  Writer construction, the twin of the defect datasink's `Reader` carries.

## See also

[[qor_datasink]] [[qor_module]] [[qor_signals]] [[qor_pipeline]] [[qor_error]]
[[qor_aggregatesource]]

Not yet paged: `qor_fileconnector` (`ComponentsPagesGroup4` - `writer.h:13` carries its
dead include exactly as datasink's `reader.h:13` does), the `QORTestPipelines` target, and
the `Pipeline`/`ByteBuffer` types wiring both ends (`src/framework/data/pipeline`, in scope
of `qor_pipeline`).
