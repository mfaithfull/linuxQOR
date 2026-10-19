# JSON Generator Plan

**Status:** **ASSIGNED as the next task** (assigned 19 Oct 2026, not yet
started). This is the task to implement in the next coding session.
**Scope:** Add a qor_pipeline-based JSON generator to `qor_jsondata` and matching
roundtrip test code to `test/components/formats/json`.
**Assigned by:** the user; the assignment is also recorded in
`memory-bank/activeContext.md` ("▶ Assigned next task: JSON generator") and
`memory-bank/progress.md`.

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

There is **no writing path** — no generator, no serializer. The plan below adds a
pipeline-based JSON generator mirroring the parser architecture, then a roundtrip test.

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