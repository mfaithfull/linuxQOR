---
Type: WikiPage
PageType: module
Title: "qor_stringsink"
Target: qor_stringsink
Status: current
LastVerified: 2026-11-18
Claims: 17
Summary: "The simplest of the four sinks: a 26-line append-to-std::string terminal that never refuses a grant, never shrinks, and is copied whole by its only reader - the only sink of the group with a named falsification case, and the terminal of every MIME chain test."
Anchors:
  - { kind: in-tree, path: src/components/data/pipeline/sinks/stringsink, symbol: qor_stringsink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sinks/stringsink/CMakeLists.txt, symbol: qor_stringsink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sinks/stringsink/stringsink.h, symbol: StringSink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sinks/stringsink/stringsink.cpp, symbol: StringSink::WriteBytes, verified: 2026-11-18 }
  - { kind: in-tree, path: test/CMakeLists.txt, symbol: qor_stringsink, verified: 2026-11-18 }
  - { kind: in-tree, path: test/framework/data/mime/CMakeLists.txt, symbol: qor_stringsink, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_stringsink --parallel 8
---

# qor_stringsink

Configured at `src/components/data/pipeline/sinks/CMakeLists.txt:4`; the folder is 4 files,
105 lines: 13-line `CMakeLists.txt`, 30-line `stringsink.h`, 26-line `stringsink.cpp`, 36-line
bootstrap. The simplest of the four sinks - no templates, no contexts, no reflection - and the
second to earn a full `FEATURES.md` row: `:82` gives it "Stable" plus "String Sink for
Pipeline", beside datasink's `:52`.

- `StringSink : public Sink` (`stringsink.h:11`) carries exactly one member, `std::string
  m_data` (`:25`), and one override, `WriteBytes(byte*, size_t)` (`:23`) - the whole product
  is the 26-line `.cpp`.
- `WriteBytes` (`stringsink.cpp:20-25`) appends `(const char*)data` for `bytesToWrite` bytes
  and always returns `bytesToWrite` - the sink never refuses, so upstream flow control never
  sees a short write from this end.
- `GetData()` (`stringsink.cpp:15-18`) returns `std::string` by value: every read copies the
  whole payload, and the 30-line header exposes no const-ref or move accessor.
- The export macro `QOR_STRINGSINK` is spelled correctly on the class (`stringsink.h:11`) and
  as the compile definition (`CMakeLists.txt:8`) - unlike the sibling deserializersink's
  truncated `QOR_DESERIALSINK`.
- Link line is the filter-family trio `qor_module qor_signals qor_pipeline`
  (`CMakeLists.txt:13`), matching `docs/agent/repo_map.md:93` and `docs/agent/deps.dot:114` -
  no unused `qor_error` tail, the cleanest line of the four sinks.
- The judge compiles both named `.cpp` (`CMakeLists.txt:3`) into a 105,472-byte dll with 0
  warnings (run 18 Nov 2026).
- The sink includes the source header: `stringsink.cpp:7` includes
  `src/framework/data/pipeline/source.h` and `Source` is never named in the file - the same
  dead cross-include shape datasink's `reader.h:13` fileconnector carry shows.
- Three configured consumers link it: `QORTest` (`test/CMakeLists.txt:95` Linux branch, `:106`
  Windows), `QOROpTests` (`test/operational/CMakeLists.txt:24`/`:34`, configured at
  `test/CMakeLists.txt:118`), and `QORTestMIME`
  (`test/framework/data/mime/CMakeLists.txt:15`, configured at `test/CMakeLists.txt:133`).
- It is the only sink of the group with a named falsification case: `validateStringSink`
  (`test/framework/test_pipeline.cpp:280`) instantiates it twice (`:289`, `:325`) -
  parsersink and deserializersink print no such case.
- `QORTestMIME` is its heaviest user: 34 of the test tree's 37 `StringSink` hits are in
  `test/framework/data/mime/test_mime.cpp`, where the MIME chain ends
  `Base64EncodeFilter -> StringSink` (`:112`) and recognized type names route the stream into
  one of two sinks through `SelectSinkByMIMETypeName` (`:1349`, used `:1413`).
- License split follows the family pattern: `stringsink.h` and `stringsink.cpp` carry the
  3-line BSL-1.0 SPDX (`:2` each), the bootstrap carries the 23-line MIT block (`:1-23`) and
  names the module "Querysoft Open Runtime: String Sink Module" in `ThisModule()`.
- `docs/agent/purposes.md:91` compresses all four sinks into one row, "pipeline sinks"; the
  folder carries no `purpose.md` of its own.

## Failure modes

- Unbounded retention: `WriteBytes` appends with no cap (`stringsink.cpp:23`) and nothing
  ever clears `m_data` - a sink left attached to a live stream accumulates until allocation
  failure, and the header (`stringsink.h:9-27`) offers no way to observe size short of
  copying the payload.
- No backpressure: always returning the full grant (`:24`) means a Push-mode chain reports
  the sink consumed everything forever - flow never throttles at this end, which compounds
  the retention above.
- Copy-per-read: `GetData()` by value (`stringsink.cpp:17`) doubles the payload in memory on
  every call; the MIME test sidesteps it by declaring a fresh sink per case rather than
  reading one repeatedly.
- Mutable grant accepted: `WriteBytes(byte* data, ...)` (`stringsink.h:23`) takes non-const
  bytes, so a StringSink is free to rewrite the stream in flight - nothing in these 26 lines
  does, and nothing prevents the next override from doing so.
- The "Stable" badge (`FEATURES.md:82`) rests on `validateStringSink` alone
  (`test/framework/test_pipeline.cpp:280`) - the MIME suite stresses it as a terminal, but
  retention and copy cost are tested by nobody.

## See also

[[qor_module]] [[qor_signals]] [[qor_pipeline]] [[qor_datasink]] [[qor_deserializersink]]
[[qor_parsersink]] [[qor_base64encodefilter]]

Not yet paged: `QORTestMIME` and `QOROpTests` (test targets, not wiki units), the
`Sink` base it derives from (`src/framework/data/pipeline/sink.h`, in scope of
`qor_pipeline`), and the connectors that feed it in the operational tests (`qor_stdfstreamconnector`, group 4).
