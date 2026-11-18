---
Type: WikiPage
PageType: module
Title: "qor_parsersink"
Target: qor_parsersink
Status: current
LastVerified: 2026-11-18
Claims: 14
Summary: "The code-point sink that closes the filter-to-parser chain: its Push parses through a CodePointContext but the line that would report what Parse consumed is commented out - and that discarded count gates the only success signal the pipeline gets."
Anchors:
  - { kind: in-tree, path: src/components/data/pipeline/sinks/parsersink, symbol: qor_parsersink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sinks/parsersink/CMakeLists.txt, symbol: qor_parsersink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sinks/parsersink/parsersink.h, symbol: BaseParserSink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sinks/parsersink/parsersink.cpp, symbol: BaseParserSink::Push, verified: 2026-11-18 }
  - { kind: in-tree, path: test/components/protocols/yaml/CMakeLists.txt, symbol: qor_parsersink, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_parsersink --parallel 8
---

# qor_parsersink

Configured at `src/components/data/pipeline/sinks/CMakeLists.txt:3`; the folder is 4 files,
208 lines: 13-line `CMakeLists.txt`, 85-line `parsersink.h`, 74-line `parsersink.cpp`,
36-line bootstrap. The `.h`/`.cpp` carry the BSL SPDX line (`:2` each), the bootstrap the
23-line MIT block with no SPDX (`:1-3`). `FEATURES.md:92` carries it as bare "Semi stable" -
a status with no description sentence.

- `BaseParserSink : public pipeline::Sink` (`parsersink.h:17`) holds a `data::Parser` by value
  (`:32`) and a `ref_of<data::AbstractDataContext>` (`:31`); `Push`
  (`parsersink.cpp:29-67`) calls `Parse` and never touches the buffer itself -
  "Parse will read buffer incrementally through context" (`:39` comment).
- `ParserSink<TObjectState>`'s buffer ctor (`parsersink.h:46-51`) pins the context to
  `data::parser::CodePointContext(&buffer)` (`:49`) - this sink consumes code points, not
  bytes, which is why it is the sink half of the `unicodetextfilter`-to-parser chain:
  `test/components/formats/json/jsonreader.h:52-53` holds the filter and this sink side by
  side.
- Template state wiring mirrors the deserializer sibling: `new_ref<TObjectState>(&m_parser)`
  plus `SetInitialStep` (`:41-43`). The tree instantiates it four ways: `JSONPartObjectT`
  (`jsonreader.h:53`), `data::parser::json::object` (`:86` and `resources/reader.h:44`),
  `protocols::yaml::mapping` (`test_yaml.cpp:25`), and the http `request` state
  (`examples/experimental/requestread/requestreader.h:37`).
- Link line is the same seven-set as `qor_deserializersink` - `qor_module qor_signals
  qor_pipeline qor_error qor_log qor_workflow qor_parser` (`CMakeLists.txt:13`,
  `repo_map.md:92`, `deps.dot:113`) - and here the parser edge is actually present, unlike
  that sibling's missing `qor_serialize`.
- It has the most configured consumers of the four sinks: the json test
  (`test/components/formats/json/CMakeLists.txt:17`, `:24`), the yaml test
  (`test/components/protocols/yaml/CMakeLists.txt:15`), and `qor_resources`
  (`src/framework/data/resources/CMakeLists.txt:22`).
- The fourth consumer is not built: `examples/experimental/requestread/CMakeLists.txt:15`
  links it, but `requestread` is not among the subdirectories
  `examples/experimental/CMakeLists.txt` configures - that `ParserSink<request>`
  instantiation never compiles.
- The judge compiles both named `.cpp` into a dll with 0 warnings.
- The sink is single-use by state: `SetEOD` (`parsersink.h:24`) flips `m_EOD` (`:33`) and
  `Reset` (`:55-68`) refuses to re-arm once set - it only logs `Debug("EOD")` (`:66`).
- `docs/agent/purposes.md:91` gives it the shared "pipeline sinks" row.

## Failure modes

- The consumption count is commented away: `/*unitsWritten =*/ Parse();`
  (`parsersink.cpp:39`) - the assignment is literally commented out, so the `if(unitsWritten
  > 0)` at `:40` reads the caller's entry value, `OnWriteSuccess` (`:42`) fires for units
  that were never measured, and the "Parse did not consume any data?" debug (`:46`) is
  wrong whenever the caller passed a nonzero variable. `Push` returns true regardless
  (`:54`).
- Unreachable EOD path: the `if(unitsToWrite > 0)` test is duplicated at `:32` and `:37`,
  so the inner else's `SetEOD(); return false;` (`:49-53`) can never run - the only way to
  set EOD is the public `SetEOD` (`parsersink.h:24`) from outside.
- The dead base `Parse` message misstates the API: `fatal("...Please overrride bool
  Parse(byte*, size_t)...")` (`parsersink.cpp:71`) - the real virtual is `size_t Parse()`
  (`parsersink.h:29`), the spelling is `overrride`, and the closing namespace comment
  (`cpp:74`) says `qor::parser::components` where the code says `qor::pipeline::components`
  (`parsersink.h:15`).
- Default-ctor template nulls the context: the no-buffer ctor (`parsersink.h:41`) leaves
  `m_sourceContext` empty, and both `Reset` (`:60`) and `Parse` (`:78-79`) dereference it
  with no test - a consumer that constructs `ParserSink<T>()` and later resets or parses
  crashes on the ref, not on a check.
- Two logging entry points in one header: `log::Debug("EOD")` is the flyer class
  (`parsersink.h:66`, `src/qor/flyers/log/debug.h:12`) while `log::debug(...)` at
  `parsersink.h:78` and `parsersink.cpp:46` is the release-elided function
  (`debug.h:23-31`) - mixed casing with different lifetime semantics in the same file.

## See also

[[qor_module]] [[qor_signals]] [[qor_pipeline]] [[qor_parser]] [[qor_unicodetextfilter]]
[[qor_deserializersink]] [[qor_datasink]] [[qor_jsondata]]

Not yet paged: the `requestread` example (never configured, so not a wiki unit), the
`CodePointContext` inside `qor_parser`, and `qor_resources`.
