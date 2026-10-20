# JSON Generator Plan

**Status:** **ROUNDTRIP (parse ⇄ generate) DONE AND VERIFIED as of 20 Oct 2026 —
`QORTestJSON` exits 0 with every roundtrip compare assert passing.** Generator
states (`src/components/data/formats/json/generator/states/`) cover literals,
escaped strings, `std::to_chars` numbers, non-finite rejection and nested
containers (`[OK](31/31)`), the write-to-file pipeline works in
`QORTestJSONGenerator` (`[OK](36/36)`: `test/components/formats/json/jsonwriter.h`
`JSONWriter` / `JSONPartWriter` plus the buffer/model ctor and `ReadBytes` hook in
`src/components/data/pipeline/sources/generatorsource/generatorsource.h`), and
parse -> generate -> parse now compares equal through the new
`test/components/formats/json/jsoncompare.h` `CompareValue` / `CompareObject` /
`CompareArray` helper (`test/data/json_roundtrip_{object,array,value}.json`).
**Remaining scope:** the full `QORTest` regression run only. See
**"Roundtrip increment — DONE and VERIFIED"** at the end of this file for the
fixtures and the two `qor_jsondata` defects that had to be fixed (the
unconditional `SetValue` in `parser/states/_4/string.cpp` and the missing closing
brace for `m_count == 0` in `generator/states/_object.cpp`), and
**"Pipeline integration increment — VERIFIED"** below for the pipeline shape and
its two gotchas (unregistered `FileSystem` platform subsystem, stale generated
`.vcxproj` → `LNK2019`).
**Assigned by:** the user; the original assignment was recorded in
`memory-bank/activeContext.md` and `memory-bank/progress.md` (both now headed
"▶ JSON generator (ROUNDTRIP VERIFIED in `QORTestJSON` — next: full `QORTest`
regression run)", updated 20 Oct 2026).

## Verified prerequisites (checked against the tree, 19 Oct 2026)
- `src/components/data/formats/json/` has `model/` and `parser/` only — no
  `generator/` folder yet.
- `qor_generator` (`src/components/io/generator`) provides `Generator :
  Workflow`, `GeneratorState : workflow::State`, `ASCIIChar` and a `Context`
  with `PutOctet()`; `qor_generatorsource` (`src/components/data/pipeline/
  sources/generatorsource`) provides the pipeline `GeneratorSource`.
- `qor_jsondata` does **not** yet link `qor_generator` — it must be added.
- `QORTestJSON` (`test/components/formats/json`) is the only consumer of
  `qor_jsondata`; its `main.cpp` now uses the relative path
  `"../../../../../../test/data"` for fixtures.
- Build/verify order: `qor_jsondata` → `QORTestJSON` → `QORTest`.
- **Gotcha**: `src/components/io/generator/CMakeLists.txt` declares
  `target_compile_definitions(${target} PRIVATE QOR_PARSER ...)` — the generator
  module reuses the **`QOR_PARSER`** macro rather than defining `QOR_GENERATOR`,
  and compiles `qor_generator.cpp generator.cpp context.cpp state.cpp` linked
  against `qor_module qor_host qor_thread qor_sync qor_injection qor_workflow
  qor_pipeline`. Check the define before using `qor_pp_module_interface(...)` in
  new generator `.cpp` files.

## Background

The JSON parser (`qor_jsondata`) currently reads JSON text into a
`model::json::Object` through a pipeline:

```
FileConnector(Source) → UnicodeTextFilter → ParserSink(Sink)   [Push mode]
```

At the original 19 Oct 2026 pre-flight check there was **no writing path**.
As of 20 Oct 2026 the plan below is **implemented through the roundtrip step**:
generator states exist and are verified in `QORTestJSONGenerator` (`[OK](36/36)`),
the pipeline/`JSONWriter` integration writes model JSON to `test/data` files, and
the parse -> generate -> parse roundtrip in `QORTestJSON` is green (process exit
code 0, fixtures written and compared equal). Only the full `QORTest` regression
run is still pending.

## Architecture mirror (parser → generator)

| Parser (reading)                     | Generator (writing)                          |
|--------------------------------------|----------------------------------------------|
| `ParserSink` (pipeline Sink)         | `GeneratorSource` (pipeline Source)          |
| wraps `Parser : Fastflow`            | wraps `Generator : Workflow`                 |
| `ParserState : fastflow::Step`      | `GeneratorState : workflow::State`           |
| Parser Context = CodePointContext   | Generator Context = output buffer (PutOctet) |
| `parser/nodes/` NodeAdapter wraps   | `generator/nodes/` NodeAdapter feeds         |
| empty model, states fill it         | populated model, states consume it           |
| Grammar in `parser/states/_1/`–`_4/`| Grammar in `generator/states/`               |
| Prepare pushes node, Emit fills it  | Prepare reads model, Emit writes octets      |

## Step 1: Create the generator source tree

Create `src/components/data/formats/json/generator/` with this layout:

```
generator/
  purpose.md
  status.md
  tokens.h              alias/reuse jsonToken enum from ../../parser/tokens.h
  helpers.h             StandardPrepare/TrivialEmit/SimpleEmit analogues
  states/
    value.h / value.cpp         value = object / array / string / number / true / false / null
    object.h / object.cpp       object = begin_object members end_object
    array.h / array.cpp         array = begin_array values end_array
    member.h / member.cpp       member = string colon value
    string.h / string.cpp       string = quotation chars quotation
    number.h / number.cpp       number = signed int frac exp
    _true.h  / _true.cpp        writes "true"
    _false.h / _false.cpp       writes "false"
    _null.h  / _null.cpp        writes "null"
    begin_object.h / begin_object.cpp writes '{'
    end_object.h   / end_object.cpp   writes '}'
    begin_array.h  / begin_array.cpp  writes '['
    end_array.h    / end_array.cpp    writes ']'
    name_separator.h  / name_separator.cpp   writes ':'
    value_separator.h / value_separator.cpp  writes ','
    ws.h / ws.cpp                   optional whitespace writer
```

Each state uses `GeneratorState : workflow::State` and `ASCIIChar` composite or
`Sequence_t`-equivalent composites. The existing `qor_generator` library
(`src/components/io/generator`) provides `Generator`, `GeneratorState`, `ASCIIChar`,
and `Context` (with `PutOctet()`).

Key state responsibilities:
- **Prepare**: read the relevant model data through the **public** accessors
  (verified against `src/components/data/formats/json/model/`):
  `Object::m_members` (`std::vector<ref_of<Member>::type>`, public member),
  `Array::m_values` (`std::vector<ref_of<Value>::type>`, public member),
  `Member::GetName()` / `Member::GetValue()` (returns `Value&`; `Member` stores
  `Value m_value` **by value**),
  `Value::GetValue()` (returns `polymorphic<ValueModel>&`) and
  `Value::GetType()` (`ValueType` enum declared in `model/valuemodel.h`),
  `String::GetValue()` (`std::string`), `Number::GetValue()` (`double`).
  `String::m_value` and `Number::m_value` are **private** — always go through
  the getters.
- **Enter lambda**: push sub-states onto the workflow stack via `PushState()`
- **Emit**: confirm output was written, `Workflow()->PopState()`
- **Fail**: undo any partial writes (should not normally happen in generation)
## Step 2: Create JSON generator node types

Create `generator/nodes/` with `NodeAdapter<T>` types for each model type. These mirror
the parser's `parser/nodes/` classes but feed **already-populated** model data into
the generator states rather than building model data.

Files and classes (one class per file, follow include-guard convention
`QOR_PP_H_COMPONENTS_DATA_FORMATS_JSON_GENERATOR_NODES_<NAME>`):
- `ObjectNode final : NodeAdapter< model::json::Object >`
- `ArrayNode final : NodeAdapter< model::json::Array >`
- `StringNode final : NodeAdapter< model::json::String >`
- `NumberNode final : NodeAdapter< model::json::Number >`
- `MemberNode final : NodeAdapter< model::json::Member >`
- `ValueNode final : NodeAdapter< model::json::Value >`
- `TrueNode`, `FalseNode`, `NullNode` (or model equivalents `_true`/`_false`/`_null`)

## Step 3: Wire the generator into a pipeline Source

The `qor_generatorsource` library (`src/components/data/pipeline/sources/generatorsource`)
already provides the general `GeneratorSource<TObjectState>` with a `Generate()`
override that calls `generator.Generate()` after `context->SetSpace(space, size)`.
The JSON-specific grammar states plug in as the `TObjectState`.

Optional: create a `JSONGeneratorSource` typedef/alias in the JSON library for
convenience.

## Step 4: Update qor_jsondata CMakeLists.txt

Add the new generator `.cpp` files to the `add_library` source list:

```cmake
add_library(${target} SHARED qor_json.cpp
  # ... existing parser .cpp files ...
  generator/states/value.cpp
  generator/states/object.cpp
  generator/states/array.cpp
  generator/states/member.cpp
  generator/states/string.cpp
  generator/states/number.cpp
  generator/states/_true.cpp
  generator/states/_false.cpp
  generator/states/_null.cpp
  generator/states/begin_object.cpp
  generator/states/end_object.cpp
  generator/states/begin_array.cpp
  generator/states/end_array.cpp
  generator/states/name_separator.cpp
  generator/states/value_separator.cpp
  generator/states/ws.cpp
)
```

Add `qor_generator` to the link list if not already available via `qor_pipeline`:

```cmake
target_link_libraries(${target} qor_module qor_host qor_current qor_sync qor_injection
  qor_workflow qor_pipeline qor_parser qor_issue qor_error qor_log
  qor_generator)
```
## Step 5: Add JSON writer test header

Create `test/components/formats/json/jsonwriter.h` — analogous to `jsonreader.h`.

Sketch:

```cpp
class JSONWriter {
public:
    JSONWriter() : m_filter(), m_source(m_filter) { }

    void operator()(const qor::io::filesystem::Index& outFile,
                    const qor::ref_of<qor::data::model::json::Object>::type obj)
    {
        operator()(qor::io::components::FileConnector(outFile, m_filter,
                    qor::io::OpenFor::WriteOnly, qor::io::WithFlags::None), obj);
    }

    void operator()(const qor::pipeline::Plug& sinkConnector,
                    const qor::ref_of<qor::data::model::json::Object>::type obj)
    {
        m_filter.Reset(128, 128);
        m_source.Reset();
        m_source.Generator().QueueNode(
            new_ref<JSONObjectNode>(obj).template AsRef<generator::Node>());
        qor::pipeline::Pipeline(m_source, sinkConnector,
            qor::pipeline::Element::Pull).Connect().PumpAll();
    }

private:
    qor::text::components::UnicodeTextFilter m_filter;
    qor::pipeline::components::GeneratorSource<json::value> m_source;
};
```

Note: the include-guard pattern, the `qor::` namespace nesting, and the
`new_ref<T>()` smart-reference pattern used in `jsonreader.h` apply verbatim.

## Step 6: Update test main.cpp for roundtrip

Extend `test/components/formats/json/main.cpp` to:
1. Read JSON from file via `JSONReader` → get `model::json::Object`
2. Write the object back to a temp file via `JSONWriter`
3. Read the temp file back via `JSONReader` → get second `model::json::Object`
4. Compare the two objects (structural equality or token-by-token comparison)

Add test cases (camelCase sentence names per QOR conventions):
- `roundtripObjectPreservesMembers`
- `roundtripArrayPreservesValues`
- `roundtripStringPreservesEscapes`
- `roundtripNumberPreservesValue`
- `roundtripNullTrueFalse`
- `roundtripNestedObject`
- `roundtripEmptyObjectAndArray`
- `roundtripWhitespaceAndSeparators`
- `roundtripUnicodeEscapedStrings`
- `roundtripKoreanTextUTF16`
## Step 7: Update test CMakeLists.txt

Add `qor_generator` and `qor_generatorsource` to the link list for both Linux and
Windows branches:

```cmake
target_link_libraries(${target} qor_module qor_host qor_compiler
  qor_platform qor_application qor_filesystem qor_pipeline qor_error qor_fileconnector
  qor_jsondata qor_parsersink qor_parser qor_generator qor_generatorsource
  qor_log qor_loghandler qor_logaggregator qor_thread qor_unicodetextfilter
  winqor_thread winqor_filesystem)
```

Same change for the Linux branch (with `linqor_thread linqor_filesystem`).

## Step 8: Add purpose.md and status.md for new folders

Per QOR conventions:
- `src/components/data/formats/json/generator/purpose.md` — explain the generator's
  role (mirrors parser; writes `model::json::*` types to a pipeline buffer as JSON
  text).
- `src/components/data/formats/json/generator/status.md` — volatile dev state.
- `src/components/data/formats/json/generator/nodes/purpose.md` if a separate
  nodes subfolder is created.

## Key design notes

- The generator uses `Workflow` (push/pop states), while the parser uses `Fastflow`
  (push/pop steps). Both are `workflow::IWorkflow` descendants but with different
  step/state types. The generator states are `GeneratorState : workflow::State`.
- `ASCIIChar` writes one byte via `Context::PutOctet()`. Composite generator states
  push multiple `ASCIIChar` states onto the workflow stack to emit multi-character
  sequences like `"true"` or `"null"`.
- For strings, the generator must walk `model::json::String::GetValue()` character
  by character and emit each, handling escape sequences (`\"`, `\\`, `\n`, `\t`,
  `\r`, `\b`, `\f`, `\uXXXX`). Note: `String` stores a **decoded** `std::string`
  (the parser accumulates raw bytes with `Append(const char*, size_t)` and
  escapes/`\uXXXX` are not retained in the model), so a parse→generate roundtrip
  will **not** reproduce the original escape text — roundtrip tests must compare
  **decoded values**, not source text.
- For numbers, `model::json::Number::GetValue()` (a `double`) must be converted to
  JSON number text. Use a minimal representation (e.g. format with default precision
  and strip redundant trailing zeros); consider round-trip precision issues for
  non-representable doubles in the test.
- `Value` dispatches on `ValueType` (`object`/`array`/`string`/`number`/`true`/
  `false`/`null` — check `model/valuemodel.h` for the exact enumerators) via
  `Value::GetType()`; the generator should switch on that to pick the sub-state.
- The generator is a **Source** in the pipeline (it produces bytes), so the flow mode
  is `Pull` (the sink pulls bytes from the source). This mirrors the parser being a
  **Sink** with `Push` mode.
- The `UnicodeTextFilter` sits on the output side, encoding generated bytes to the
  target text encoding. Alternatively, raw ASCII/UTF-8 JSON output can bypass the
  text filter.
- Keep the rule-of-7 in mind when adding subfolders under `generator/`.

## Verification

Build order per QOR conventions:
1. `examples/basic/helloworld` (sanity)
2. `QORTestJSON` (in `test/components/formats/json`) — currently the only consumer
   of `qor_jsondata`; new test cases added to its `main.cpp`
3. Full `QORTest` to ensure no regressions

Roundtrip test pattern (compare model equality):
- Provide a `CompareModel(a, b)` helper that walks both `model::json::Object`
  trees and reports mismatches.
- For number roundtrip, format with a representation that is guaranteed to round
  (e.g. `%.17g` then strip trailing zeros, or use a string-preserving model type
  for exact roundtrip semantics).

## Implemented increment — 20 Oct 2026

- `_number.cpp` now uses `std::to_chars` for the model `Number` double path
  instead of the previous manual integer/fraction rendering loop. This emits
  shortest round-trip decimal text and exposes JSON-compatible exponent forms
  such as `1e+10` and `1e-10`.
- Non-finite doubles are rejected in `_value.cpp` before entering `_number`, and
  `_number.cpp` also guards `m_numberValue` with `std::isfinite` so invalid model
  numbers cannot enter the number state machine.
- Nested containers are supported by attaching an inner model to a `Value` slot
  with `polymorphic<qor::data::model::json::ValueModel>(inner)`. `_value`
  dispatches on `Value::GetType()` and routes to `_array`, `_object`, `_string`,
  or `_number`, so arrays and objects can be nested recursively.
- Corrected object-member output assertions: `{"a":7}` is 7 bytes, not 6.
- `QORTestJSONGenerator` is verified green at **31/31** tests.

## Pipeline integration increment — DONE and VERIFIED 20 Oct 2026 (`[OK](36/36)`)

The shape that actually works (verified on disk, `QORTestJSONGenerator` green
36/36, artefacts written to `test/data`):

- **Source**: `qor::pipeline::components::GeneratorSource<JSONPartStateT> source(m_buffer, model)`
  — new **route (a)** ctor mirroring `ParserSink(const pipeline::Buffer&)`:
  ```cpp
  template<typename... TArgs>
  GeneratorSource(const pipeline::Buffer& buffer, TArgs&&... args) :
      BaseGeneratorSource(), m_objectState(new_ref<TObjectState>(&m_generator, std::forward<TArgs>(args)...))
  { SetBuffer(buffer); m_context = new_ref<qor::components::generator::Context>();
    m_generator.SetContext(m_context); m_generator.SetInitialState(m_objectState); }
  ```
  (`#include <utility>` added). Route (b) (`generator/nodes/` adapters +
  `QueueNode`/`DequeueNode`) was **not needed**.
- **Sink**: `qor::io::components::FileConnector sinkConnector(outFile, m_buffer, qor::io::OpenFor::WriteOnly, qor::io::WithFlags::None)`
  — `WithFlags::None` leaves the platform to open with `CREATE_ALWAYS`, so a
  missing file is created and an existing one truncated.
- **Drive**: `qor::pipeline::Pipeline(source, sinkConnector, qor::pipeline::Element::FlowMode::Push).Connect().PumpAll()`
  — the `JSONReader` style, **not** the Pull + `Delegate` style of
  `test/pipeline/testwriter.cpp`.
- **Multi-buffer resume**: `GeneratorSource` now implements
  `virtual size_t ReadBytes(byte* space, size_t bytesToRead)` — returns
  `Generate(space, bytesToRead)` while the generator is incomplete and `0` once
  `m_context.IsNull() || m_generator.IsComplete()`, which ends the pipeline. A
  state that runs out of space keeps its stack intact, so the next pull resumes
  at the saved position (blocker 3 resolved).
- **Test helper** `test/components/formats/json/jsonwriter.h`:
  `JSONPartWriter<JSONPartStateT, JSONModelTypeT>` (owns
  `qor::pipeline::PODBuffer<byte> m_buffer`), typedef'd to
  `JSONObjectWriter` (`_object`/`Object`), `JSONArrayWriter` (`_array`/`Array`),
  `JSONValueWriter` (`_value`/`Value`); top-level `JSONWriter` wraps
  `JSONObjectWriter` with a 1024-byte buffer.
- **New tests** (36 total, 5 new): `json_generator_object_model_written_to_file_by_pipeline`,
  `json_generator_array_model_written_to_file_by_pipeline`,
  `json_generator_top_level_value_written_to_file_by_pipeline`,
  `json_generator_nested_object_written_to_file_in_parts`,
  `json_generator_writer_writes_object_to_file`. Artefacts:
  `test/data/json_generator_object.json` (7), `json_generator_writer.json` (7),
  `json_generator_array.json` (5), `json_generator_value.json` (1),
  `json_generator_nested.json` (16).
- **CMake**: `test/components/formats/json/generator/CMakeLists.txt` link list
  gained `qor_fileconnector qor_filesystem`; `qor_jsondata` gained the 11
  `generator/states/*.cpp` sources and links `qor_generator`.
- **Two fixes were needed to get there**:
  1. **Runtime** — `FileConnector` resolves its `qor::io::filesystem::Index`
     through the `qor::io::FileSystem` **platform subsystem**, and
     `ThePlatform(qor_shared)` starts with none, so the `FileSystem`/`IFile`
     module requirements fail as soon as a pipeline touches a file. The test
     entry point `test/components/formats/json/generator/main.cpp` now includes
     `src/framework/io/filesystem/filesystem.h` and `src/platform/platform.h`,
     declares `qor_pp_module_requires(IFileSystem)`, and calls
     `ThePlatform(qor_shared)->AddSubsystem<FileSystem>();` before
     `AppBuilder().Build<Application>(...)`.
  2. **Link** — `LNK2019` unresolved `qor_fileconnector` symbols were a
     **stale generated `.vcxproj`**, not a code problem: `cmake --build` never
     re-emitted the project files after the link-list edit, so no
     `qor_fileconnector.lib` import library was linked. Explicit
     `cmake -B cmakebuild -S . -DCMAKE_BUILD_TYPE=Debug` regeneration cleared it
     (confirmed by dumping PE import libraries with `tmp/pefilter.py`).
- **Run location**: the file tests use
  `constexpr const char* outputFolder = "../../../../../../../test/data";`
  (line 675 of `test_jsongenerator.cpp`) relative to the **exe's own build
  directory**. Run from `cmakebuild/test/components/formats/json/generator/Debug`
  the target reports `[OK](36/36)` (exit 0) and rewrites
  `test/data/json_generator_*.json`; run from the repo root the same exe reports
  `[FAIL](5/36)` (exit 5, `unknown exception`) on the 5 file tests only — a
  path/cwd artefact, not a pipeline fault.

## Prepared next increment (20 Oct 2026 — pre-flight verified against the tree)

Scratch cleanup: `tmp/` emptied (35 transient build/test logs, a scratch probe
`.obj` and two `_tmp_to_chars*.exe` probes removed; the folder itself is kept).
Nothing there is git-referenced — `/tmp` is in `.gitignore`.

### Verified prerequisites for the pipeline step

- `qor_jsondata` already links `qor_generator` and already compiles every
  generator `.cpp` (`_null/_true/_false/_string/_number/_array/_object/_value/_comma/_colon/_member`).
- `QORTestJSONGenerator` CMake already links `qor_generator qor_generatorsource`
  (so both targets are proven buildable/linkable here); `QORTestJSON` does **not**
  — it still needs both added (Step 7).
- `qor_generatorsource` is a registered built target (`qor_generatorsource.dll`)
  linking `qor_module qor_signals qor_pipeline qor_generator`.
- Model accessors confirmed: `Object::m_members` is
  `std::vector<ref_of<Member>::type>`; `Member` stores `Value m_value` **by value**
  and exposes `GetValue()` (`Value&`), `GetName()`, `SetName()`; `Array::m_values`
  is `std::vector<ref_of<Value>::type>`.
- State ctors available for the writer: `_value(Generator*, Value*)`,
  `_object(Generator*, Object*)`, `_array(Generator*, Array*)`,
  `_member(Generator*, Member*)`, `_string(Generator*, const char*, size_t)`,
  `_number(Generator*, Number*)` / `_number(Generator*, long long)`,
  `_comma(Generator*)`, `_colon(Generator*)`.
- `qor_generator::NodeAdapter<T>` ctors: `NodeAdapter()` (owns a fresh `T`) and
  `NodeAdapter(ref_of<T>, uint64_t token)`; `Generator` has `QueueNode()` /
  `DequeueNode()` over `std::deque<ref_of<Node>::type>` — that is the intended
  node handoff route. Parser mirror for the token-tag pattern:
  `parser/nodes/object.h` (`ObjectNode : NodeAdapter<model::json::Object>`
  tagged `jsonToken::object`).
- Working Pull-mode write pipeline to study: `test/pipeline/testwriter.cpp`
  (`FileConnector(outFile, buffer, OpenFor::WriteOnly, WithFlags::Truncate|Append)`,
  `Pipeline(source, sink, Element::Pull)`, and the
  `Delegate<bool(size_t&,size_t)>::Create<Pipeline,&Pipeline::Pump>` callback set on
  the **sink** in Pull mode). JSONReader instead uses `Pipeline(...).Connect().PumpAll()`
  — decide which drive style `JSONWriter` needs.

### Blockers in the reusable parts (20 Oct 2026 pre-flight — all three RESOLVED, see "Pipeline integration increment — VERIFIED")

1. `GeneratorSource<TObjectState>` ctor only does
   `new_ref<TObjectState>(&m_generator)` + `SetInitialState`, so it can only build
   states constructible from a bare `Generator*`. Every JSON state that reads model
   data needs an extra argument. Two routes:
   (a) add a `GeneratorSource(const pipeline::Buffer&, Model*)` ctor mirroring
   `ParserSink(const pipeline::Buffer&)`; (b) keep the ctor and hand the model
   through `Generator::QueueNode(new_ref<JSONObjectNode>(obj).AsRef<generator::Node>())`
   (the plan's sketch) — then the initial state must `DequeueNode()` in `Prepare()`.
2. `GeneratorSource` never creates `m_context` (`ref_of<Context>`) and never calls
   `m_generator.SetContext(...)`, yet `Generate()` does `m_context->SetSpace(...)` and
   `Generator::InnerGenerate()` derefs `m_context->HasSpace()`. `ParserSink(buffer)`
   is the mirror that does set it
   (`new_ref<data::parser::CodePointContext>(&buffer)` then `SetContext`). Expect a
   null/empty-ref deref until a `Context` is created — verify this first.
3. `BaseGeneratorSource::Pull` calls `OnEndOfData()` when `ReadBytes` returns 0, and
   `InnerGenerate()` stops on `!m_context->HasSpace()`; so output larger than one
   pipeline buffer must resume across pulls (`GeneratorState::Reset()` sets
   `Result::MORE_DATA`). Test with a buffer smaller than the JSON text.

### Ordered plan for the next increment (items 1–5 DONE and verified 20 Oct 2026; 6–7 done — only the full `QORTest` regression run remains)

1. Extend `src/components/data/pipeline/sources/generatorsource/generatorsource.h`
   (+`.cpp`): buffer ctor that creates the `Context`, `SetContext()` on the
   generator, plus model handoff (route (a) or (b) above). Remember a new library
   `.cpp` also needs the CMake source list and
   `cmake -B cmakebuild -S . -DCMAKE_BUILD_TYPE=Debug` regeneration.
2. **[SKIPPED]** Route (b) was not taken, so no `generator/nodes/` adapters were
   created — the variadic `GeneratorSource(buffer, model)` ctor (route (a)) hands
   the model straight to the initial state.
3. **[DONE — verified]** The writer test header exists as
   `test/components/formats/json/jsonwriter.h` (`JSONPartWriter` + `JSONWriter`),
   but it is driven from the **generator** test `QORTestJSONGenerator`, not from
   `QORTestJSON`. Deviations from the sketch: `FileConnector` uses
   `OpenFor::WriteOnly` with `qor::io::WithFlags::None` (the platform opens with
   `CREATE_ALWAYS` and truncates), output is raw ASCII/UTF-8 with **no**
   `UnicodeTextFilter`, and the pipeline is driven
   `FlowMode::Push` + `.Connect().PumpAll()` rather than the planned Pull mode.
4. **[DONE — verified 20 Oct 2026]** Roundtrip cases live in
   `test/components/formats/json/main.cpp` as a guarded block inside the `Run`
   lambda (placed **before** the Korean UTF-16 fixtures so their debug flood
   cannot bury a failure); the comparison helper is the new
   `test/components/formats/json/jsoncompare.h`
   (`CompareValue` / `CompareObject` / `CompareArray`, mutually recursive,
   decoded-value semantics — the sketch name `CompareModel(a, b)` was specialised
   into these three). `QORTestJSON` **shares** `jsonwriter.h` /
   `jsonreader.h` with `QORTestJSONGenerator` — no duplicated flow.
5. **[DONE]** `qor_generator qor_generatorsource` are in the `QORTestJSON` link
   list in `test/components/formats/json/CMakeLists.txt` (both branches).
6. **[DONE except the last step]** Build/verify order:
   `qor_generatorsource` -> `qor_jsondata` -> `QORTestJSONGenerator` (regression,
   re-verified `[OK](36/36)`) -> `QORTestJSON` (roundtrip, verified process exit
   code 0 via `msbuild QOR.sln /t:QORTestJSON`) -> full `QORTest` regression run
   (**still pending**).
7. **[RESPECTED]** Both streams captured (`exe *> log`) and assertions chained
   (`qor_pp_assert_that(x).isTrue();`).

## Roundtrip increment — DONE and VERIFIED 20 Oct 2026 (`QORTestJSON` exit code 0)

The writing half is now exercised from `QORTestJSON` itself: parse a fixture,
write the model back out through `JSONWriter` / `JSONPartWriter`, re-parse the
file into a second model and compare the two models.

### Implemented shape

- **Helper** `test/components/formats/json/jsoncompare.h`
  (`QOR_PP_H_TEST_JSON_JSONCOMPARE`): mutually recursive
  `CompareValue` / `CompareObject` / `CompareArray` over
  `model::json::Value` / `Object` / `Array`. Compares **decoded** values (the
  model retains no escapes), exact `double` equality for numbers (correct because
  the generator writes with `std::to_chars`), and containers in stored order.
- **Roundtrip block** in `test/components/formats/json/main.cpp` inside the
  `Run` lambda, before the `y_object_string_korean_utf16le` /
  `y_object_string_korean_utf16be` fixtures whose per-character
  `Parser::InnerParse` debug flood dominates the run log. A failing
  `qor_pp_assert_that` throws `qor::test::failure`, so the block is wrapped in
  `try` / `catch` (`qor::test::failure`, `qor::test::exception`, `std::exception`,
  `...`) that reports through `qor::log::Debug` — `log::Error` is **not** part of
  the `Log` interface, and `String` had to be qualified because the file also
  pulls in `qor::text` string types.
- **Fixtures written and re-read:** `test/data/json_roundtrip_object.json`
  (`{"asd":"sdf"}`, generated from `y_object_basic.json`),
  `test/data/json_roundtrip_array.json` (`[null,1,"1",{}]`),
  `test/data/json_roundtrip_value.json` (`42`).
- **Measured result:** `msbuild QOR.sln /t:QORTestJSON` -> build exit 0; running
  `QORTestJSON.exe` from `cmakebuild/test/components/formats/json/Debug` ->
  process exit code **0**, no `roundtrip failure` / `roundtrip error` line in the
  captured log, and all three compare asserts pass.

### Two `qor_jsondata` defects found and fixed on the way

1. **String content lost on parse** (`{"": ""}`, `"1"` -> `""`):
   `character::Emit` appends each decoded character **directly into the
   StringNode's model** while that node is on the parser node stack, so by the
   time `string::Emit` runs its own collection loop `stringValue` is empty and the
   unconditional `SetValue(stringValue)` wiped the text. Fix:
   `if(!stringValue.empty())` in `parser/states/_4/string.cpp`.
2. **Empty nested object written as `{`** (no closing brace): the model-driven
   `_object(generator, object)` ctor with `m_count == 0` writes the opening brace
   at position 0, but the closing-brace branch
   `m_position == (m_count * 2)` is `1 == 0` and never fires, so the state fell
   through to SUCCESS without `}`. Fix: an explicit `m_count == 0` branch in
   `generator/states/_object.cpp` that writes `}` at position 1 and completes.
   The dedicated empty-object (`nullptr`) ctor path is unchanged.

### Coverage vs the Step 6 case list (honest gap — 20 Oct 2026)

Step 6 of this plan named eleven roundtrip cases. The verified increment covers
the **shapes**, not all the named cases: object members (`{"asd":"sdf"}`), array
values incl. `null` / number / string and a **nested empty object**
(`[null,1,"1",{}]`), and a bare `Number` value (`42`). Not exercised as
roundtrips: escapes / `\uXXXX` string preservation, Korean UTF-16 text through
generate -> parse, whitespace/separator normalisation, empty **array**
(`[]` — note the `_object` empty fix has no array counterpart yet, so verify
`[]` before relying on it), and nested object-inside-object. If the full
`QORTest` run is green, these remain the open roundtrip cases before the
generator can be called complete.

### Ownership theory — disproved (20 Oct 2026)

The initial "the writer reads dangling model data" theory was wrong:
`qor::components::generator::NodeAdapter<T>` holds a **ref-counted**
`ref_of<T>::type m_t` and `GetObject()` returns it **by value**, so
`push_back((*it)->GetObject())` copies an owning reference and the model stays
alive. The defects were the two above, both in `qor_jsondata`; the writer call
shape `w(Index(testsPath, name), Model*) -> size_t` driven by `PumpAll()` was
already correct.
