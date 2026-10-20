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

## Not started

Roundtrip (parse ⇄ generate) tests in `QORTestJSON`, full `QORTest` regression run.

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

Details and the ordered plan: `memory-bank/jsonGeneratorPlan.md`
("Prepared next increment").
