---
Type: WikiPage
PageType: module
Title: "qor_generatorsource"
Target: qor_generatorsource
Status: current
LastVerified: 2026-11-18
Claims: 15
Summary: "The pipeline source that drives a qor_generator state machine into the pipeline's write buffer: a header-only template on a three-member cpp base whose Pull truncates byte counts to whole units and reads a zero-byte pull as end-of-data."
Anchors:
  - { kind: in-tree, path: src/components/data/pipeline/sources/generatorsource, symbol: qor_generatorsource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/generatorsource/CMakeLists.txt, symbol: qor_generatorsource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/generatorsource/generatorsource.h, symbol: BaseGeneratorSource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/generatorsource/generatorsource.h, symbol: GeneratorSource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/generatorsource/generatorsource.cpp, symbol: BaseGeneratorSource::Pull, verified: 2026-11-18 }
  - { kind: in-tree, path: test/components/formats/json/CMakeLists.txt, symbol: qor_generatorsource, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_generatorsource --parallel 8
---

# qor_generatorsource

Configured at `src/components/data/pipeline/sources/CMakeLists.txt:3` (family order
`stringsource` :1, `randomsource` :2, `generatorsource` :3, `aggregatesource` :4,
`datasource` :5); the folder is 4 files, 206 lines: 13-line `CMakeLists.txt`, 93-line
`generatorsource.h`, 64-line `generatorsource.cpp`, 36-line bootstrap. `FEATURES.md:106`
carries it as "Experimental" - as Experimental as its dependency `qor_generator` at
`:105` - and `docs/agent/purposes.md:92` lumps all five pipeline sources onto one line.

- Its job is the source that runs a `qor_generator` state machine into the pipeline's
  write buffer; the header names its own mirror, "Mirrors ParserSink(const
  pipeline::Buffer&)" (`generatorsource.h:48`), directly above the pipeline-driven
  constructor (`:54`).
- Shape: `BaseGeneratorSource : public pipeline::Source` (`:16`) in
  `qor::pipeline::components` (`:14`) - the sink-family namespace, not datasource's
  `qor::pipeline` anomaly; marking is the MSVC form `qor_pp_module_interface(QOR_GENERATORSOURCE)`
  (`:16`) paired with the compile definition `QOR_GENERATORSOURCE` (`CMakeLists.txt:8`),
  with no individually marked members (datasource carries seven).
- The dll is a base-class holder: `add_library` (`CMakeLists.txt:3`) compiles only the
  bootstrap and the three `BaseGeneratorSource` members; `GeneratorSource<TObjectState>`
  (`:39`) is header-only throughout, and the only in-tree instantiation is
  `test/components/formats/json/jsonwriter.h:52` (`GeneratorSource<JSONPartStateT>
  source(m_buffer, model);`).
- The dependency is embedded by value: `m_generator` is a whole
  `qor::components::generator::Generator` member (`:35`) beside the `ref_of<Context>`
  (`:34`) - that is why `qor_generator` is on the link line.
- Link line, index and graph agree: the quartet `qor_module qor_signals qor_pipeline
  qor_generator` (`CMakeLists.txt:13`) is `docs/agent/repo_map.md:96` is
  `docs/agent/deps.dot:117`. The inversions: the folder's one flyer user includes
  `src/qor/flyers/error/error.h` (`generatorsource.cpp:29`) and fires `continuable`
  (`:45`) without linking `qor_error`, and `qor_signals` is on the line named by no
  source in the folder - the orphan-edge anomaly datasource carries, run backwards.
- The commented-out declaration is load-bearing: `BaseGeneratorSource` does not declare
  `ReadBytes` - the pure-virtual sits commented at `:32` - so the call in `Pull`
  (`generatorsource.cpp:48`) binds the framework virtual `pipeline::Source::ReadBytes`
  (`src/framework/data/pipeline/source.h:50`), whose empty base fatals
  (`src/framework/data/pipeline/source.cpp:187-189`); the template's override (`:81`)
  is found through the vtable alone.
- Context discipline: `ReadBytes` returns 0 when the context is null or the generator is
  complete (`:83-85`); null because the default ctor (`:43`) never makes one - only the
  pipeline-driven ctor (`:54-60`) sets the buffer, `new_ref`s the `Context` and installs
  the initial state, its variadic `TArgs` handing model data to the state (the shape
  recorded at `src/components/data/formats/json/generator/status.md:72`).
- `Pull` (`generatorsource.cpp:37-62`): null buffer false (`:39-40`, `:61`); null space
  flyer (`:45`) and false (`:46`); `charsRead > 0` units conversion (`:51`),
  `WriteAcknowledge` (`:52`), `OnReadSuccess` (`:53`); `charsRead == 0` `OnEndOfData()`
  (`:57`); either non-null-buffer path returns true (`:59`).
- Consumers are the json test trees: `QORTestJSON` links it on both branches
  (`test/components/formats/json/CMakeLists.txt:17`, `:24`), `QORTestJSONGenerator`
  through its library set (`test/components/formats/json/generator/CMakeLists.txt:18`),
  and `jsonwriter.h:24` includes the header - the `QORTestPipelines` tree that consumes
  the datasource page does not link this target.
- The license split follows its siblings: the header carries the BSL-1.0 SPDX
  "2008 - Present" (`:1-2`), both `.cpp` files the full MIT block "2008 - 2025"
  (`generatorsource.cpp:1-23`, `qor_generatorsource.cpp:1-23`); the bootstrap's
  `ThisModule` names "Querysoft Open Runtime: Generator Source Module"
  (`qor_generatorsource.cpp:33`).
- The judge compiled both named `.cpp` fresh and linked a 1,028,608-byte dll
  (run 18 Nov 2026 - an order of magnitude over the sibling datasource dll's 86,016 B
  the same day); the re-run exits 0 as a no-op with the dll stamped 18 Nov 11:04, newer
  than every folder file (newest source 20 Oct 2026).

## Failure modes

- Units truncate on the way up: `unitsRead = (charsRead * sizeof(char)) /
  buffer->GetUnitSize()` (`:51`) is integer division - an odd-byte generation into a
  wide-unit buffer lands `unitsRead = 0`, so `WriteAcknowledge(0)` publishes nothing
  (`:52`), `OnReadSuccess(0)` fires (`:53`) and `Pull` still returns true (`:59`):
  generated bytes are silently lost and the pump re-enters.
- A transient zero reads as end-of-data: any pull with `charsRead == 0` calls
  `OnEndOfData()` (`:55-57`) - but the header promises "Generation that runs out of
  space mid state leaves the state stack intact, so the next pull resumes the suspended
  state at its saved position" (`generatorsource.h:79-80`), and the `.cpp` branch ends
  the pipeline before any such next pull; the two contracts sit 40 lines apart and the
  `.cpp` one is the one that runs.
- The stall flyer is continuable and then dropped: `continuable("Pipeline stall. No
  space in source buffer.")` (`:45`) is followed by a bare `return false` (`:46`) with
  no `OnEndOfData` and no `OnReadSuccess` - the pipeline sees a plain false unless the
  flyer itself is caught, with no record that the buffer was the culprit.
- The acknowledge count trusts a sibling library: `Generate` reports
  `m_context->GetPosition()` (`generatorsource.h:72`) - a Context position, not a
  per-call byte delta - and the Context is made once per pipeline-driven construction
  (`:57`) with `SetSpace` only re-windowing it (`:70`); `Pull` acknowledges that number
  as this call's bytes (`:48-52`), so the ack arithmetic is a `qor_generator` contract
  this page cannot verify - its leaf is five steps down in this group.

## See also

[[qor_parsersink]] [[qor_datasource]] [[qor_aggregatesource]] [[qor_jsondata]]
[[qor_pipeline]] [[qor_module]] [[qor_signals]]

Not yet paged: `qor_generator` (group 3 leaf 5 - `src/components/io/generator`, also
"Experimental" at `FEATURES.md:105`), the `QORTestJSON` / `QORTestJSONGenerator`
targets, and the `pipeline::Source` interface (`src/framework/data/pipeline`, in scope
of `qor_pipeline`).
