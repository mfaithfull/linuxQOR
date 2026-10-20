# JSON Generator — status

**Last updated:** 20 Oct 2026

## Verified (QORTestJSONGenerator, `[OK](36/36)`)

Model-driven generator states write: `null` / `true` / `false`, strings with JSON
escaping (`\"`, `\\`, short forms, `\u00xx`), numbers through `std::to_chars`
(shortest round-trip decimal/scientific), non-finite rejection, arrays and objects
with `_comma` / `_colon` / `_member`, and nested containers routed back through
`_value`.

**Also verified — write-to-file pipeline (5 of the 36 tests):**
`GeneratorSource<State>(buffer, model)` source +
`FileConnector(outFile, buffer, OpenFor::WriteOnly, WithFlags::None)` sink sharing
one `qor::pipeline::PODBuffer`, driven by `.Connect().PumpAll()`. The helper lives
in the test tree (`test/components/formats/json/jsonwriter.h`:
`JSONPartWriter` / `JSONWriter`), and the tests write
`test/data/json_generator_{object,writer,array,value,nested}.json`. The test exe
registers the `FileSystem` platform subsystem in `main.cpp`. Run the exe **from its
own build directory** (`cmakebuild/test/components/formats/json/generator/Debug`) —
its `../../../../../../../test/data` output path is calibrated to that location.

## Full `QORTest` regression run — DONE and VERIFIED 20 Oct 2026

Regenerated the CMake project (`cmake -B cmakebuild -S . -DCMAKE_BUILD_TYPE=Debug`),
built the base test target (`cmake --build cmakebuild --target QORTest --config Debug`,
build exit 0, no `error C`/`LNK`/`fatal`), then ran the base suite from its own build
directory (`cmakebuild/test/Debug`). Result: **`[TOTAL](678/678)` / `[OK](678/678)`,
process exit code 0**, no `[FAIL` and no `[ALL FAILURES]` block in the captured log
(both streams). The JSON generator library fixes (`qor_jsondata`
`parser/states/_4/string.cpp` guard, `generator/states/_object.cpp` `m_count == 0`
branch, and the `GeneratorSource` buffer ctor / `ReadBytes` hook in
`qor_generatorsource`) did not regress the base framework suite. This closes the
last open item in the JSON generator increment.

## Roundtrip (parse ⇄ generate) — DONE and VERIFIED 20 Oct 2026 (`QORTestJSON` exit code 0)

Run from the existing `QORTestJSON` target (no new exe): parse a fixture with
`JSONReader` / `JSONPartReader` / `JSONValueReader`, write that model out with
`JSONWriter` (from `test/components/formats/json/jsonwriter.h`) into
`test/data/json_roundtrip_{object,array,value}.json`, re-parse the file and
compare the two models with the new
`test/components/formats/json/jsoncompare.h` helpers
`CompareValue` / `CompareObject` / `CompareArray` (mutually recursive, decoded
values, exact `double` compare — safe because numbers are written with
`std::to_chars`). Artefacts: `{"asd":"sdf"}`, `[null,1,"1",{}]`, `42`.

The block sits **before** the `y_object_string_korean_utf16{le,be}` fixtures
because their per-character `Parser::InnerParse` debug line dominates the run log
(~476s profile), and it is wrapped in `try` / `catch` reporting
`JSON roundtrip failure` / `JSON roundtrip error` through `qor::log::Debug`
(a failing `qor_pp_assert_that` throws `qor::test::failure` and would otherwise
abort the rest of the `Run` lambda).

Two `qor_jsondata` defects had to be fixed to get there (shared code, so
`QORTestJSONGenerator` was rebuilt and re-verified `[OK](36/36)` afterwards):
`parser/states/_4/string.cpp` now guards `SetValue` with
`if(!stringValue.empty())` (`character::Emit` appends decoded characters
directly into the live StringNode model, so `stringValue` is empty on that path),
and `generator/states/_object.cpp` gained an explicit `m_count == 0` branch that
writes the closing `}` at position 1 (the `m_position == (m_count * 2)` test
evaluates `1 == 0` for an empty object). The dangling-ownership theory was a
red herring — `NodeAdapter<T>` holds a ref-counted `ref_of<T>::type m_t` and
`GetObject()` returns it by value.

Details and the ordered plan: `docs/archive/jsonGeneratorPlan.md`
("Roundtrip increment", "Prepared next increment").

## Reusable-part gaps found in the 20 Oct 2026 pre-flight — ALL FIXED the same day

- `qor_generatorsource`'s `GeneratorSource<TObjectState>` gained a variadic
  `GeneratorSource(const pipeline::Buffer&, TArgs&&... args)` ctor (route (a) of
  the two planned; `#include <utility>` added) that creates the output
  `qor::components::generator::Context`, calls `SetContext`/`SetBuffer`, and
  builds the initial state with `new_ref<TObjectState>(&m_generator, std::forward<TArgs>(args)...)`.
- `GeneratorSource` now implements the pipeline `Source` hook
  `virtual size_t ReadBytes(byte*, size_t)`: it generates into the reserved
  buffer space while the generator is incomplete and returns 0 once
  `m_context.IsNull() || m_generator.IsComplete()`, which ends the pipeline. A
  state that runs out of space leaves its stack intact, so the next pull resumes
  at the saved position.

Original findings (kept for the record):

- `qor_generatorsource`'s `GeneratorSource<TObjectState>` ctor builds
  `new_ref<TObjectState>(&m_generator)` only — it cannot pass the model pointer
  that every model-driven JSON state ctor requires.
- `GeneratorSource` never creates/assigns `m_context` nor calls
  `Generator::SetContext()`, yet `Generate()` derefs `m_context`.
- Multi-pull generation needs the state stack to survive across `Pull()` calls
  (`GeneratorState::Reset()` sets `MORE_DATA`).

Details and the ordered plan: `docs/archive/jsonGeneratorPlan.md`
("Prepared next increment").
