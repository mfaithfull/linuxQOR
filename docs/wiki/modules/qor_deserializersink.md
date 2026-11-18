---
Type: WikiPage
PageType: module
Title: "qor_deserializersink"
Target: qor_deserializersink
Status: current
LastVerified: 2026-11-18
Claims: 14
Summary: "The Experimental sink that feeds pipeline bytes into the qor_serialize Deserializer: a 240-line pre-SPDX folder whose export macro is misspelled, whose link line omits the library its own header names, and whose only consumer is a framework library that closes that edge for it."
Anchors:
  - { kind: in-tree, path: src/components/data/pipeline/sinks/deserializersink, symbol: qor_deserializersink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sinks/deserializersink/CMakeLists.txt, symbol: qor_deserializersink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sinks/deserializersink/deserializersink.h, symbol: BaseDeserializerSink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sinks/deserializersink/deserializersink.cpp, symbol: BaseDeserializerSink::Push, verified: 2026-11-18 }
  - { kind: in-tree, path: src/framework/data/resources/CMakeLists.txt, symbol: qor_deserializersink, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_deserializersink --parallel 8
---

# qor_deserializersink

Configured at `src/components/data/pipeline/sinks/CMakeLists.txt:2`; the folder is 4 files,
240 lines: 13-line `CMakeLists.txt`, 95-line `deserializersink.h`, 74-line
`deserializersink.cpp`, 36-line bootstrap. Every source here carries the 23-line MIT-style
block dated "2008 - 2025" (`:1-23`) and no SPDX line - the only fully pre-SPDX folder of the
four sinks, matching the "Experimental" badge at `FEATURES.md:103`.

- Flow is Write→Pull→Push: `Write` (`deserializersink.cpp:37-40`) pulls from the source in
  Pull mode (`:42-46`) then hands the count to `Push` (`:48-72`), which requests buffer
  bytes, calls the pure-virtual `Read` (`:53`, implemented at `deserializersink.h:85-90` as
  `Context::SetData` + `Deserializer::Read` + return of `Context::GetPosition`), and converts
  `bytesParsed / buffer->GetUnitSize()` into acknowledged units (`:59-60`).
- `DeserializerSink<TObjectState>` (`deserializersink.h:59-91`) is the half the library ships:
  an intrusive-refs state object created with `new_ref<TObjectState>(&m_deserializer)` and
  pinned as the deserializer's initial state (`:64-66`); the base holds a `Deserializer` and
  a `Context` member by value/ref (`:55-56`).
- The export macro is `QOR_DESERIALSINK` - a truncated spelling of the target, applied bare
  via `qor_pp_module_interface(QOR_DESERIALSINK)` on the class (`deserializersink.h:35`) and
  as the compile definition (`CMakeLists.txt:8`); it is missing the middle syllable.
- Link line is the seven-set `qor_module qor_signals qor_pipeline qor_error qor_log
  qor_workflow qor_parser` (`CMakeLists.txt:13`), agreeing with `docs/agent/repo_map.md:91`
  and `docs/agent/deps.dot:112` - the widest link line of the four sinks.
- The header includes `src/components/io/deserializer/deserializer.h` and `context.h`
  (`deserializersink.h:30-31`); those classes are built by the `qor_serialize` target
  (`repo_map.md:99`), which this sink does not link.
- Its only consumer in the tree is a framework library: `qor_resources` links
  `qor_deserializersink` (`src/framework/data/resources/CMakeLists.txt:23`) - a
  framework→components edge - and closes the missing serialize edge itself by naming
  `qor_serialize` in the same `target_link_libraries` command (`:25`).
- Those consumers instantiate the templates the library never does:
  `src/framework/data/resources/reader.h:31` uses `DeserializerSink<DesignatorState>` and
  `:42` uses `DeserializerSink<TTFInitialState>` - no state type is named in this folder, so
  the dll exports the base and the template work happens in the consumer.
- No test target links it: a tree-wide `qor_deserializersink` search finds one linking
  `CMakeLists.txt` line outside docs - `src/framework/data/resources/CMakeLists.txt:23`.
- `docs/agent/purposes.md:91` gives it the shared one-line row "pipeline sinks".

## Failure modes

- The output parameter is the input count: `Write` calls
  `Push(unitsWritten, unitsWritten)` (`deserializersink.cpp:39`) - in Push flow
  (`GetFlowMode() != Pull`, where `Pull` vacuously returns true at `:44-46`) the number of
  units the sink parses equals whatever the caller's `unitsWritten` variable held on entry;
  an entry value of 0 makes `Write` return true without reading the buffer at all (`:71`).
- EOF and parser stall are the same signal: `Push` returns false when `Read` yields zero
  bytes under the author's own questioning comment `//EOF?` (`:63-65`) - a state machine
  that refuses to advance on malformed input is indistinguishable from end-of-stream, and
  the pump sees only repeated false.
- Mid-unit truncation re-feeds bytes: `unitsWritten = bytesParsed / buffer->GetUnitSize()`
  (`:59`) integer-divides with no divisibility test, and `ReadAcknowledge(unitsWritten)`
  (`:60`) acknowledges whole units only - if the parse stops inside a multi-byte unit, the
  partial bytes stay in the buffer and are re-presented to `Read` on the next call.
- The link edge to `qor_serialize` is absent from `CMakeLists.txt:13` and from
  `repo_map.md:91`/`deps.dot:112`: every consumer that includes `deserializersink.h` must
  discover and link `qor_serialize` itself - `qor_resources` does (`:25`), and nothing here
  tells the next one.
- Zero falsification coverage: no test links the target, so the only thing that proves this
  folder compiles is the `qor_resources` build plus the judge above - the `:39` and `:59`
  defects are unobservable to the test tree today.

## See also

[[qor_signals]] [[qor_pipeline]] [[qor_parser]] [[qor_module]] [[qor_datasink]]
[[qor_log]] [[qor_workflow]]
