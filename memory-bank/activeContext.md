# Active Context

**Last updated:** 22 Oct 2026 — **qor_mime plan step 9 (`mime_recognises_html_signature`) DONE and
verified green, driven test-first** (RED `[FAIL] #9`, `[TOTAL](10/10) [OK](9/10) [FAIL](1/10)` exit 1 →
GREEN `[TOTAL](10/10) [OK](10/10)` exit 0; logs `tmp/qortestmime_step9_red.log`,
`tmp/qortestmime_step9_green.log`). **7 of the 10 target MIME types now recognised** (PNG, JPEG, GIF,
PDF, ZIP, XML, HTML). Step 8 (`mime_recognises_xml_signature`) and step 7 (`mime_recognises_zip_signature`)
were likewise DONE and verified green test-first (RED `[FAIL] #8` → GREEN `[TOTAL](9/9) [OK](9/9)`;
RED `[FAIL] #7` → GREEN `[TOTAL](8/8) [OK](8/8)`). Details in the section below (and in `src/framework/data/mime/status.md`,
`docs/agent/verification.md`). Earlier the same day: qor_mime plan steps 0-6 were done (step 4
verified after the fact — its RED phase never observed), and the whole JSON generator increment is **DONE**,
including the **full `QORTest` regression run** which is now **green at `[TOTAL](678/678)` / `[OK](678/678)` (process
exit code 0)** — the paragraph in this earlier update was truncated; the full JSON generator
status is in the section below.

## Current Task: qor_mime — MIME recognition pipeline filter (20 Oct 2026)

**Task**: new module in its own folder under `src/framework/data` (`src/framework/data/mime`,
lib `qor_mime`) providing a pipeline filter that recognises ten popular MIME types.
**Approach**: test first, one increment at a time. Full plan: `memory-bank/qor_mime_plan.md`.
- **Rule in force (D8, user 20 Oct 2026): the QOR does not use ctest.** Verification = build the
  lib (`qor_mime`), build the test exe (`QORTestMIME`), run the exe from its build folder, read
  `[OK](n/n)` + shell exit code. `test/framework/data/mime/CMakeLists.txt` has no
  `enable_testing()`/`add_test()` for that reason.


### Done
- Researched QOR pipeline filters first (step R in the plan): `qor::pipeline::Filter` base,
  `base64encodefilter`/`linereader`/`jpegdecodefilter` as the filter pattern, `QORTestYAML`/
  `QORTestJSONGenerator` as the per-module test exe pattern.
- Created the outline: `src/framework/data/mime/{CMakeLists.txt,qor_mime.cpp,mime.h,mime.cpp,purpose.md,status.md}`
  (singleton, `qor::mime::MIMERecognizerFilter : qor::pipeline::Filter`, `MIMEType` enum with the
  ten targets + `OctetStream` fallback + `Unresolved`, placeholder `Inspect()`), registered
  `add_subdirectory(mime)` in `src/framework/data/CMakeLists.txt`.
- Created the mirror test target `test/framework/data/mime` (`QORTestMIME`, `test_main.cpp`,
  placeholder `mime_module_loads`), registered in `test/CMakeLists.txt`.
- Verified the outline: `qor_mime.dll` built clean (import lib generated — `init_mime` export
  guard), `QORTestMIME.exe` ran `[OK](1/1)` exit 0.
- **Plan step 1 (pass octets through) — DONE 20 Oct 2026.** Wrote `mime_filter_passes_octets_through`
  in `test_mime.cpp` FIRST (StringSource → filter → StringSink under a Push-mode `Pipeline` parent,
  128-byte `ByteBuffer`s, assert `sink.GetData() == testData`), ran it RED (`[FAIL] #1`, exit 1,
  "value was : 0" — the base `Filter` does not move octets by itself), then added
  `MIMERecognizerFilter::TransferOctets(size_t&, size_t)` in `mime.h`/`mime.cpp` (buffer→buffer move
  modelled on `CopyFilter::Copy`: sink `WriteRequest`, source `ReadRequest`, `memcpy`, both acks) and
  routed `ReadFilter`/`WriteFilter` through it. Rebuilt → `[TOTAL](2/2) [OK](2/2)` exit 0.
  `Inspect()` and all detection still untouched.
  **Plumbing learnings (record these):** a filter pumps in **Push** mode and needs a `Pipeline`
  parent to carry the flow mode (`Element::SetFlowMode` only forwards to a parent, stores nothing);
  the chain is `Source::Read` → `Source::Push` → `Filter::Write` → `WriteFilter` → sink `Write`.
  Pull mode is a dead end for filters in the base framework: `Filter::Read` calls
  `ReadFilter(unitsRead, unitsRead)` with `unitsRead == 0` at entry.

- **Plan step 2 (capture leading octets into the signature window) — DONE 20 Oct 2026.** Wrote
  `mime_captures_leading_octets` FIRST (40-byte input asserts `GetSignatureByteCount() == 16`;
  fresh elements + 4-byte input asserts `== 4`; no `Reset()` reuse — that is step 15), ran it RED
  (`[FAIL] #2`, `[OK](2/3)`, exit 1 — placeholder `Inspect()` captured nothing and was not even
  called), then implemented `Inspect()` in `mime.cpp` (no `mime.h` change — the hook was already
  declared): the pipeline buffer read protocol is **reserve / use / commit-or-reject** (`buffer.h`,
  `buffer.cpp`), so capture = `ReadRequest(least(unitsToInspect, ReadCapacity(), windowSpace))` →
  `memcpy` into `m_signature` → **`ReadReject(unitsRequested)`** to give the reservation back — the
  source is never consumed and the octets still move on through `TransferOctets`; capture is capped
  by `SIGNATURE_WINDOW_BYTES` (16). `ReadFilter`/`WriteFilter` call `Inspect()` BEFORE
  `TransferOctets()`. Rebuilt → `[TOTAL](3/3) [OK](3/3)` exit 0.
  **New learnings (record):** `ReadReject` is the "peek without consuming" primitive a read-only
  filter needs; a test run issued in the SAME command batch as the build ran the **stale previous
  exe** (the recorded gotcha bit again — build and run must be separate sequential commands);
  `log::debug` inside `qor_mime` produces no output in `QORTestMIME` (the log flyer is not wired
  into that exe), so it is not a usable probe there.

- **Plan step 3 (recognise the PNG signature) — DONE 20 Oct 2026.** Wrote
  `mime_recognises_png_signature` FIRST (Push-mode StringSource → filter → StringSink, stream
  begins with the 8-byte PNG magic `89 50 4E 47 0D 0A 1A 0A` + a short body; asserts
  `sink.GetData() == pngData` — re-checks the pipeline contract, `GetDetectedType() ==
  MIMEType::PNG`, `std::string(GetTypeName()) == "image/png"`), ran it RED (`[FAIL] #3`,
  `[OK](3/4)`, exit 1 — the pass-through assert passed, `GetDetectedType()` still `Unresolved`).
  Smallest fix in `mime.cpp` (no `mime.h` change): a fixed-order signature table inside `Inspect()`
  — after each capture, if `m_detectedType == Unresolved && m_signatureCount >= 8`, `memcmp` the
  first 8 of `m_signature` against the PNG header and set `m_detectedType = MIMEType::PNG`.
  Detection lives in `Inspect()` (the non-consuming capture hook) so it re-evaluates chunk-by-chunk
  as leading octets arrive. Also the complete registered-name table in `GetTypeName()` (switch over
  `MIMEType`, `Unresolved` → `""`), so the accessor is right for every type the recognizer can
  report. Rebuilt → `[TOTAL](4/4) [OK](4/4)` exit 0.
  **Learning (record):** the name table is data (enum → registered name) and may be complete from the
  first detection increment; only the *detectors* have to be added one test at a time.

- **Plan step 4 (recognise the JPEG magic) — VERIFIED DONE 20 Oct 2026, but NOT test-driven.** The
  test `mime_recognises_jpeg_signature` AND the JPEG detector were already sitting in the working
  tree (uncommitted edits to `mime.cpp` + `test_mime.cpp` — the previous session wrote them and
  stopped) and nothing in `status.md`, `verification.md` or the memory bank showed them built or
  run, and `git log` had no step-4 commit. So this increment was verified after the fact: built
  `qor_mime` + `QORTestMIME`, ran the exe from `build\test\framework\data\mime\Debug` →
  `[TOTAL](5/5) [OK](5/5)`, no `[FAIL`, exit 0 (`tmp/qortestmime_step4_green.log`). The code found
  in place is exactly the planned smallest fix (guard `m_signatureCount >= 3`, `memcmp` vs
  `FF D8 FF` in `Inspect()`, `mime.h` untouched, name table already complete). **Lesson: verify
  before writing — unverified working-tree code is not "done", and a session that ends without a
  build+run leaves its own RED phase unrecoverable.**
  **Also hit here: `cmake --build` invocations must not be batched.** Two `cmake --build` calls in
  one command batch run concurrently over the same build tree and race on the shared `.pdb`:
  `error C1090: PDB API call failed` on an unrelated dependency, and `error C1041: cannot open
  program database '…vc145.pdb'; if multiple CL.EXE write to the same .PDB file, please use /FS`.
  Both were transient (retry alone fixed them), but they are real — exactly one `cmake --build` per
  command, strictly sequential.

- **Plan step 5 (recognise the GIF header) — DONE 20 Oct 2026, test-first.** Wrote
  `mime_recognises_gif_signature` FIRST (two streams in the one case with fresh elements for the
  second — `GIF89a` and `GIF87a` six-byte headers each followed by a short body; per stream asserts
  `sink.GetData() == data` pass-through, `GetDetectedType() == MIMEType::GIF`,
  `std::string(GetTypeName()) == "image/gif"`), ran it RED (`mime_recognises_gif_signature [FAIL],
  #5`, `[TOTAL](6/6) [OK](5/6) [FAIL](1/6)`, exit 1 — `expected boolean to have the value of true
  / value was : 0`, `tmp/qortestmime_step5_red.log`), then `Inspect()` gained a third fixed-order
  guard (`m_detectedType == Unresolved && m_signatureCount >= 6`, `memcmp` the window vs
  `gif89Signature` `{47 49 46 38 39 61}` OR `gif87Signature` `{… 37 …}` → `MIMEType::GIF`).
  `mime.h` unchanged. Rebuilt `qor_mime` then `QORTestMIME` (separate commands) →
  `[TOTAL](6/6) [OK](6/6)`, exit 0 (`tmp/qortestmime_step5_green.log`).
  **Learning (record):** a format with more than one magic (GIF87a/GIF89a) is still ONE increment —
  the test asserts every variant, the code gets one guard with one compare per variant.

- **Plan step 6 (recognise the PDF magic) — DONE 20 Oct 2026, test-first.** Wrote
  `mime_recognises_pdf_signature` FIRST (one Push-mode stream beginning `%PDF-1.4\n` — the four byte
  `%PDF` magic plus the version marker as real files have it, then a short body; asserts
  `sink.GetData() == pdfData` pass-through, `GetDetectedType() == MIMEType::PDF`,
  `std::string(GetTypeName()) == "application/pdf"`), ran it RED (`mime_recognises_pdf_signature [FAIL],
  #6`, `[TOTAL](7/7) [OK](6/7) [FAIL](1/7)`, exit 1, `tmp/qortestmime_step6_red.log`), then `Inspect()`
  gained a fourth fixed-order guard (`m_detectedType == Unresolved && m_signatureCount >= 4`,
  `memcmp` the window vs `pdfSignature` `{0x25,0x50,0x44,0x46}` = `'%' 'P' 'D' 'F'` →
  `MIMEType::PDF`). `mime.h` unchanged (name table complete since step 3). Rebuilt `qor_mime` then
  `QORTestMIME` (separate commands, one cmake build each) → `[TOTAL](7/7) [OK](7/7)`, exit 0
  (`tmp/qortestmime_step6_green.log`). No new gotchas — the one-build-per-command and
  build-then-run-separately rules both held this time.

- **Plan step 7 (recognise the ZIP magic) — DONE 20 Oct 2026, test-first.** Wrote
  `mime_recognises_zip_signature` FIRST (one Push-mode stream beginning `PK\x03\x04` — the four byte
  `PK 03 04` local file header magic as raw octets, then a short body; asserts
  `sink.GetData() == zipData` pass-through, `GetDetectedType() == MIMEType::ZIP`,
  `std::string(GetTypeName()) == "application/zip"`), ran it RED (`mime_recognises_zip_signature [FAIL],
  #7`, `[TOTAL](8/8) [OK](7/8) [FAIL](1/8)`, exit 1, `tmp/qortestmime_step7_red.log`), then `Inspect()`
  gained a fifth fixed-order guard (`m_detectedType == Unresolved && m_signatureCount >= 4`,
  `memcmp` the window vs `zipSignature` `{0x50,0x4B,0x03,0x04}` = `'P' 'K' 03 04` →
  `MIMEType::ZIP`). `mime.h` unchanged (name table complete since step 3). Rebuilt `qor_mime` then
  `QORTestMIME` (separate commands, one cmake build each) → `[TOTAL](8/8) [OK](8/8)`, exit 0
  (`tmp/qortestmime_step7_green.log`). No new gotchas — the one-build-per-command and
  build-then-run-separately rules both held this time. **Learning:** the ZIP magic's version
  octets `03 04` are raw bytes, not text — the test writes the magic with `"PK\x03\x04"` hex
  escapes (two escapes back-to-back, so no hex-digit swallowing ambiguity), and the guard
  compares raw `qor::byte` values.

- **Plan step 8 (recognise the XML `<?xml` declaration marker) — DONE 20 Oct 2026, test-first.**
  This active-context body had only reached step 7 when that session ended; the increment is recorded
  in full in `memory-bank/progress.md`, `src/framework/data/mime/status.md` and
  `docs/agent/verification.md`: RED `[FAIL] #8`, `[TOTAL](9/9) [OK](8/9) [FAIL](1/9)`, exit 1 →
  GREEN `[TOTAL](9/9) [OK](9/9)` exit 0; sixth fixed-order guard in `Inspect()`, `memcmp` vs
  `{0x3C,0x3F,0x78,0x6D,0x6C}`, `mime.h` unchanged.

- **Plan step 9 (recognise HTML) — DONE 22 Oct 2026, test-first.** Wrote
  `mime_recognises_html_signature` FIRST (TWO streams in the one case with fresh elements for the
  second — mirroring step 5's two GIF variants: lower case `<!DOCTYPE html>\n<html><head>...` and
  no-doctype upper case `<HTML>\n<head>...`; per stream asserts `sink.GetData() == data` pass-through,
  `GetDetectedType() == MIMEType::HTML`, `std::string(GetTypeName()) == "text/html"`), ran it RED
  (`mime_recognises_html_signature [FAIL], #9`, `[TOTAL](10/10) [OK](9/10) [FAIL](1/10)`, exit 1,
  `tmp/qortestmime_step9_red.log`), then `Inspect()` gained a seventh fixed-order guard after the
  XML marker: a case-folded 14-octet `<!DOCTYPE HTML` compare, and while still unresolved a
  document-level first-tag compare (`m_signature[0] == 0x3C` `'<'` then case-folded `html`/`head`/`body`/`title`).
  **Learning (record):** text heuristics cannot use `memcmp` — HTML markup spelling is case-insensitive,
  so `mime.cpp` gained a file-local `markupMatch()` ASCII case-fold compare helper (internal: no
  `qor_pp_module_interface`, folds ASCII a-z/A-Z only, no locale, no new dependency; it also checks
  the window length so each compare decides as soon as its own octets are captured). A format with
  two recognition paths (doctype OR first tag) is still ONE increment — the test asserts every path,
  the code gets one guard with one compare per path (same shape as step 5's GIF87a/GIF89a pair).
  `mime.h` unchanged (name table complete since step 3). Rebuilt `qor_mime` then `QORTestMIME`
  (separate commands, one cmake build each) → `[TOTAL](10/10) [OK](10/10)`, no `[FAIL`, exit 0
  (`tmp/qortestmime_step9_green.log`). **7 of 10 target types recognised.** No new build gotchas.

### Next up
- [ ] Plan step 10 — write `mime_recognises_json_signature` FIRST (first non-space octet is `{` or `[` →
  `GetDetectedType() == MIMEType::JSON`, `GetTypeName() == "application/json"`), then the json guard
  in `Inspect()` after the html guard (`GetTypeName()` name table is already complete from step 3).
- [ ] Plan steps 10..13 — the remaining three types one test at a time (json,
  javascript, plain text) + octet-stream fallback.
- [ ] Plan steps 14..17 — chunk boundary split, `Reset()` reuse, chaining with
  `qor_base64encodefilter`, final full-QORTest regression run (rebuild + rerun the base pipeline
  test exe to prove no framework regression).
- Do NOT skip ahead to writing all ten detectors at once — one test, then the code that passes it.

### Key files
`memory-bank/qor_mime_plan.md` (the plan) | `src/framework/data/mime/{mime.h,mime.cpp,qor_mime.cpp,CMakeLists.txt}` |
`src/framework/data/mime/{purpose.md,status.md}` | `test/framework/data/mime/{test_mime.cpp,test_main.cpp,CMakeLists.txt}` |
pattern sources: `src/framework/data/pipeline/{filter.h,filter.cpp}`,
`src/components/data/pipeline/filters/base64encodefilter/*`, `test/components/protocols/yaml/*`


the CMake project and building `QORTest` (build exit 0), running the base suite
from `cmakebuild/test/Debug` produced no `[FAIL` and no `[ALL FAILURES]` block, so
the `qor_jsondata`/`qor_generatorsource` library fixes did not regress the base
framework. Before that, the JSON generator was **done through the roundtrip step**:
parse -> generate -> parse in `QORTestJSON` is green (process exit code 0), on top
of the **pipeline integration** step (a JSON model written to a real file through a
real pipeline: `GeneratorSource` -> `FileConnector`) that is **DONE and verified at
`[OK](36/36)`** in the dedicated `QORTestJSONGenerator` target. The root cause of
the first runtime failure
(unregistered `FileSystem` platform subsystem) and the `LNK2019` link failure
(stale generated Visual Studio project files) are both recorded here, as are the
two `qor_jsondata` defects found while getting the roundtrip green. Earlier
init was built from `STATUS.md`
29/07/2026, `CHANGES.md`, `FEATURES.md` and git log at `6b65785a`; `STATUS.md`
has **not** been updated since, so `git log` and `docs/agent/STATE.MD` are the
newest repo-wide sources.

## ▶ JSON generator (COMPLETE — ROUNDTRIP VERIFIED in `QORTestJSON`; full `QORTest` regression run GREEN at `[OK](678/678)` — plan file retired to `docs/archive/jsonGeneratorPlan.md` (no longer a memory-bank file)

**Pipeline integration done and verified 20 Oct 2026 (`[OK](36/36)`)** — the
writing half was built incrementally in
a NEW test target `QORTestJSONGenerator` (`test/components/formats/json/generator/`,
gated by `QOR_BUILD_QORTESTJSONGEN`; separate from `QORTestJSON` so the
established parser test stays untouched). Generator states live in the new
folder `src/components/data/formats/json/generator/states/` in namespace
`qor::data::generator::json` and are `qor::components::generator::GeneratorState`
(`qor::workflow::State`) descendants driven by a `qor::components::generator::Generator`
that writes through `qor::components::generator::Context::PutOctet()`.

Done and VERIFIED working (20 Oct 2026, all in `qor_jsondata`, all green):
- Step 1: empty `QORTestJSONGenerator` exe built, ran, exit code 0, `[OK](1/1)`.
- Step 2: `generator/states/_null.h/.cpp` (composite state pushing
  `qor::components::generator::ASCIIChar` sub-states) compiled into `qor_jsondata`
  (which now also links `qor_generator`), and the test
  `json_generator_writes_null` (buffer 16 bytes -> `Context` -> `Generator` ->
  `SetInitialState(new_ref<_null>(&generator))` -> `Generate()`) passes:
  `GetPosition() == 4` and buffer == "null". `[OK](1/1)`, exit 0.
- Steps 3-8, one increment each, verified at every step (8 tests -> `[OK](8/8)`):
  `_true`/`_false` (same composite pattern as `_null`), `_string` (quotation mark
  + payload + quotation mark), `_number` (integer digits + optional leading `-`,
  fixed 21-byte text buffer filled in the ctor), `_array` -> `[]`, `_object` -> `{}`.
- Step 9: `_string` extended to escape the payload itself (raw text in, JSON text
  out): `"` -> `\"`, `\` -> `\\`, `\b \f \n \r \t` short forms, other
  `ch < 0x20` -> `\u00xx` lower-case hex. Verified by
  `json_generator_escapes_quotation_mark` and
  `json_generator_escapes_control_character` -> `[OK](10/10)`.
- Step 10: `generator/states/_value.h/.cpp` — the model-driven entry state. It
  reads `qor::data::model::json::Value::GetType()` and pushes the matching write
  state (`_null`/`_true`/`_false`); other value types set
  `qor::components::generator::Result::FAILURE` and pop (the states that read
  model data come later). Verified by `json_generator_writes_model_value`
  (default-constructed `Value` has the `_null` type) -> **`[OK](11/11)` green**.
- Step 11: model-driven `_number`, `_array`, `_object`, and nested
  `polymorphic<qor::data::model::json::ValueModel>(inner)` routing, with
  `std::to_chars` scientific-notation double rendering and `std::isfinite`
  non-finite guards — verified in `QORTestJSONGenerator` at `[OK](31/31)`.

- Step 12 — **pipeline integration, DONE and VERIFIED 20 Oct 2026 (`[OK](36/36)`)**:
  a JSON model's text is now written into a real file by a real pipeline.
  - New test helper `test/components/formats/json/jsonwriter.h`:
    `JSONPartWriter<JSONPartStateT, JSONModelTypeT>` holds a
    `qor::pipeline::PODBuffer<byte> m_buffer` shared by both pipeline ends,
    builds `qor::io::components::FileConnector sinkConnector(outFile, m_buffer,
    qor::io::OpenFor::WriteOnly, qor::io::WithFlags::None)` (sink) and
    `qor::pipeline::components::GeneratorSource<JSONPartStateT> source(m_buffer,
    model)` (source), then drives
    `qor::pipeline::Pipeline(source, sinkConnector, qor::pipeline::Element::FlowMode::Push).Connect().PumpAll()`.
    Typedefs `JSONObjectWriter` (`_object`/`Object`), `JSONArrayWriter`
    (`_array`/`Array`), `JSONValueWriter` (`_value`/`Value`), and the top-level
    `JSONWriter` (1024-byte buffer, object model).
  - `src/components/data/pipeline/sources/generatorsource/generatorsource.h`
    gained the **route (a)** ctor mirroring `ParserSink(const pipeline::Buffer&)`:
    `template<typename... TArgs> GeneratorSource(const pipeline::Buffer&, TArgs&&... args)`
    that `SetBuffer(buffer)`, creates
    `m_context = new_ref<qor::components::generator::Context>()`, calls
    `m_generator.SetContext(m_context)` and builds the initial state with
    `new_ref<TObjectState>(&m_generator, std::forward<TArgs>(args)...)`
    (needed `#include <utility>`); route (b) (`generator/nodes/` adapters
    + `QueueNode`/`DequeueNode`) was **not** needed. It also gained the
    pipeline `Source` hook `virtual size_t ReadBytes(byte* space, size_t bytesToRead)`
    returning 0 when `m_context.IsNull() || m_generator.IsComplete()` (ends the
    pipeline) and `Generate(space, bytesToRead)` otherwise — a state that runs
    out of buffer space keeps its stack intact, so the next pull resumes at the
    saved position (multi-pull blocker resolved).
  - 5 new tests in `test_jsongenerator.cpp` (36 total):
    `json_generator_object_model_written_to_file_by_pipeline`,
    `json_generator_array_model_written_to_file_by_pipeline`,
    `json_generator_top_level_value_written_to_file_by_pipeline`,
    `json_generator_nested_object_written_to_file_in_parts`,
    `json_generator_writer_writes_object_to_file` → **`[OK](36/36)`** exit 0.
    Artefacts written: `test/data/json_generator_object.json` (7 bytes),
    `json_generator_writer.json` (7), `json_generator_array.json` (5),
    `json_generator_value.json` (1), `json_generator_nested.json` (16).
  - `test/components/formats/json/generator/CMakeLists.txt` link list needed
    `qor_fileconnector qor_filesystem` (the Windows branch already had
    `winqor_filesystem`, plus `winqor_common`).
  - **Runtime failure root cause**: `FileConnector` resolves its
    `qor::io::filesystem::Index` through the `qor::io::FileSystem` **platform
    subsystem**, and `ThePlatform(qor_shared)` starts with **no subsystems**, so
    the module requirements for `FileSystem` and `IFile` fail as soon as a
    pipeline is hooked to a file. Fixed in the test entry point
    `test/components/formats/json/generator/main.cpp`: include
    `src/framework/io/filesystem/filesystem.h` and `src/platform/platform.h`,
    add `qor_pp_module_requires(IFileSystem)`, and call
    `ThePlatform(qor_shared)->AddSubsystem<FileSystem>();` **before**
    `AppBuilder().Build<Application>(...)`.
  - **`LNK2019` on `qor_fileconnector` was not a code problem**: the generated
    MSVC `.vcxproj` pre-dated the CMakeLists edit and `cmake --build` alone never
    re-emitted it, so there was no `qor_fileconnector.lib` import library to link
    (confirmed by dumping PE import libraries with the `tmp/pefilter.py` helper).
    Explicit `cmake -B cmakebuild -S . -DCMAKE_BUILD_TYPE=Debug` regeneration
    fixed it; the target then built and ran 36/36 green.
  - **Run location matters**: the file tests use
    `constexpr const char* outputFolder = "../../../../../../../test/data";`
    (line 675 of `test_jsongenerator.cpp`) relative to the **executable's own
    build directory**. Verified recipe: `Set-Location` to
    `cmakebuild/test/components/formats/json/generator/Debug`, then
    `.\QORTestJSONGenerator.exe *> <log>` → **`[OK](36/36)`, exit 0**. Run the
    same exe from the repo root and the 5 file tests throw
    `unknown exception` (`[FAIL](5/36)`, exit 5) while the other 31 pass — a
    path/cwd artefact, **not** a pipeline fault. The 31 buffer-only tests are
    cwd-independent.
  - `jsonwriter.h` already used `qor::io::OpenFor::WriteOnly` — no enum mismatch
    to fix; the final code passes `qor::io::WithFlags::None`, letting the
    platform open with `CREATE_ALWAYS` and truncate an existing file.

### Roundtrip (parse ⇄ generate) increment — DONE and VERIFIED 20 Oct 2026 (`QORTestJSON` exit code 0)

Implemented in `test/components/formats/json/main.cpp` (inside the `Run` lambda of
the existing `QORTestJSON` exe — no new target) plus the new header
`test/components/formats/json/jsoncompare.h`:

- **Shape:** parse a fixture with `JSONReader` / `JSONPartReader` /
  `JSONValueReader` -> write that model out with `JSONWriter` / the part writers
  from `jsonwriter.h` into `test/data` -> re-parse the file into a second model
  -> compare with `CompareValue` / `CompareObject` / `CompareArray`
  (`jsoncompare.h`, mutually recursive, `QOR_PP_H_TEST_JSON_JSONCOMPARE`). Numbers
  compare as exact doubles (safe because the generator renders with `std::to_chars`),
  strings compare **decoded** (the model keeps no escapes), containers compare in
  stored order.
- **Fixtures:** `json_roundtrip_object.json` -> `{"asd":"sdf"}` (model from
  `y_object_basic.json`), `json_roundtrip_array.json` -> `[null,1,"1",{}]`,
  `json_roundtrip_value.json` -> `42`.
- **Ordering:** the block sits **before** the `y_object_string_korean_utf16le` /
  `y_object_string_korean_utf16be` fixtures, whose per-character
  `Parser::InnerParse` debug line dominates the run log; a failing
  `qor_pp_assert_that` throws `qor::test::failure`, so the block is wrapped in
  `try` / `catch` (`qor::test::failure`, `qor::test::exception`, `std::exception`,
  `...`) logging `JSON roundtrip failure` / `JSON roundtrip error` through
  `qor::log::Debug` (`log::Error` is not part of the `Log` interface), and `String`
  had to be qualified because the file also pulls in `qor::text` string types.
- **Verified:** `msbuild QOR.sln /t:QORTestJSON` -> build exit 0; running
  `QORTestJSON.exe` from `cmakebuild/test/components/formats/json/Debug` ->
  process exit code **0**, no `roundtrip failure` / `roundtrip error` line in the
  captured log, and the three fixture files are written non-empty and faithful.
- **Two `qor_jsondata` library fixes were required** (shared code, so
  `QORTestJSONGenerator` was rebuilt and re-verified `[OK](36/36)` afterwards):
  1. `parser/states/_4/string.cpp` — `SetValue` guarded by
     `if(!stringValue.empty())`. `character::Emit` appends each decoded character
     **directly into the StringNode's model** while that node is on the parser node
     stack, so `stringValue` is empty on the live path and the unconditional
     overwrite emptied every string (`{"": ""}`, `"1"` -> `""`).
  2. `generator/states/_object.cpp` — explicit `m_count == 0` branch that writes
     the closing `}` at position 1 and completes. Without it, a zero-member object
     wrote only `{` because the closing-brace test `m_position == (m_count * 2)`
     evaluates `1 == 0`. The dedicated empty-object (`nullptr`) ctor path is
     unchanged.
- **Ownership theory disproved:** `qor::components::generator::NodeAdapter<T>`
  holds a **ref-counted** `ref_of<T>::type m_t` and `GetObject()` returns it
  **by value**, so `push_back((*it)->GetObject())` copies an owning reference — no
  dangling model data. The writer call shape
  `JSONPartWriter<State, Model>(Index(testsPath, name), model) -> size_t` driven by
  `PumpAll()` was always correct.
- **Repo state at this update:** HEAD `78ce2d83` "memory bank update for Cline";
  the two library fixes plus `test/components/formats/json/{main.cpp,CMakeLists.txt}`
  are **modified in the working tree**, and `jsoncompare.h` and the
  `test/data/json_{generator,roundtrip}_*.json` artefacts are untracked.

### Full `QORTest` regression run — DONE and VERIFIED 20 Oct 2026 (base suite green)

The last open item of the JSON generator increment. Executed in the gitignored
`tmp/` scratch area; result captured from both streams:

- Regenerated the CMake project first: `cmake -B cmakebuild -S . -DCMAKE_BUILD_TYPE=Debug`
  (cached generator is Visual Studio 17 2022, build type Debug — **no `-G`**).
- Built the base test target: `cmake --build cmakebuild --target QORTest --config Debug`
  → build exit 0, no `error C` / `LNK…` / `fatal error` lines. `QORTest.exe`
  (`cmakebuild/test/Debug`) links the base framework libs (no `qor_jsondata`/
  `qor_generator`), so this run is the framework-wide sanity check.
- Ran the exe **from its own build directory** (`cmakebuild/test/Debug`) capturing
  both streams (`.\QORTest.exe *> <log>`), twice; both returned **process exit code
  0** and identical summary: **`[TOTAL](678/678)` / `[OK](678/678)`**, with **no**
  `[FAIL` line and **no** `[ALL FAILURES]` block in the 826-line log.
- Conclusion: the `qor_jsondata` (`string.cpp` `SetValue` guard, `_object.cpp`
  `m_count == 0` branch) and `qor_generatorsource` (`GeneratorSource` buffer ctor
  + `ReadBytes` hook) changes made on 20 Oct 2026 did **not** regress the base
  suite. Combined with the green `QORTestJSON` roundtrip and `QORTestJSONGenerator`
  `[OK](36/36)`, the JSON generator increment is **complete**.

- Test `.cpp` files must `#include "src/..."` (project-root relative); the
  `"components/..."` short form only resolves inside library trees.
- `byte` is `qor::byte` (`typedef uint8_t byte` in `src/platform/compiler/compiler.h`)
  — test `.cpp` bodies are wrapped in `namespace qor { ... }` so `byte`/`new_ref`
  resolve unqualified.
- `qor_test` is NOT a CMake target (stale entry in the http test link list); do
  not link it — `tdd.h` is header-only. Linking it gives `LNK1104: cannot open
  file 'qor_test.lib'`.
- Test exe exit code is 0 on success (main.cpp returns 0 unconditionally);
  pass/fail is reported by the run log lines `[OK](n/n)` / `[FAILED]`.
- Generator states set their `Enter`/`Resume`/`Leave`/`Prepare`/`Emit`/`Fail`
  `std::function` members in the ctor (lambda capture `this`), mirroring
  `ASCIIChar` and the http `requestgenerator` states.
- `Generator::Generate()` returns `bool` (true == m_result SUCCESS); the
  workflow completes when the last state pops (stack empty -> `SetComplete()`).
- Assertions MUST chain a proposition: `qor_pp_assert_that(expr).isTrue();`
  otherwise they are vacuous (`BoolSubject` temporary, nothing checked).
- Capture BOTH stdout and stderr when running a QOR test exe (`exe *> log`);
  `[FAIL](n/n)` and the `[ALL FAILURES]` block appear on stderr.
- Current verified green state: **36 tests** in `QORTestJSONGenerator` (the 31 model-driven state tests plus 5 file-pipeline tests) -> **`[OK](36/36)`**, 20 Oct 2026.
  Roundtrip increment: **VERIFIED 20 Oct 2026** in `QORTestJSON` (process exit
  code 0; see the "Roundtrip (parse ⇄ generate) increment" section above).
  Next increment: the full `QORTest` regression run.
  Completed increments: `std::to_chars` scientific-notation rendering for model
  `Number` doubles, non-finite rejection in `_value` before entering `_number`
  (and defensively in `_number`), and nested `Array`/`Object` model values routed
  through `_value` via `polymorphic<qor::data::model::json::ValueModel>(inner)`.
  Next increment at that point: `JSONWriter` / `GeneratorSource` pipeline
  integration, roundtrip tests in `QORTestJSON`, and full `QORTest`
  regression verification — the first two are now **done** (see above).
- Prep done 20 Oct 2026 for that next increment: `tmp/` scratch folder emptied
  (logs/probe binaries were transient; `/tmp` is gitignored) and the
  `generator/` folder gained `status.md` recording volatile dev state.
  Pre-flight blockers (since **resolved** — see Step 12) recorded in `docs/archive/jsonGeneratorPlan.md`
  ("Prepared next increment"): `GeneratorSource<TObjectState>` cannot pass model
  data to the model-driven JSON state ctors, `GeneratorSource` never creates/sets
  its `Context` (unlike `ParserSink(buffer)`), and multi-pull output must resume
  across `Pull()` calls — **all three are now fixed** in
  `generatorsource.h` by the variadic buffer ctor and the `ReadBytes` hook (Step
  12). The verified pipeline used `qor::pipeline::Element::FlowMode::Push`
  + `.Connect().PumpAll()` (the `JSONReader` style), **not** the Pull +
  `Delegate` style of `test/pipeline/testwriter.cpp`.
- Nested model containers are attached to a `Value` slot through
  `polymorphic<qor::data::model::json::ValueModel>(inner)` where `inner` is an
  `Object`, `Array`, `String`, or `Number` model. `_value::Prepare()` reads
  `Value::GetType()` and routes to `_object`, `_array`, `_string`, or `_number`
  again, which makes nested arrays/objects work without adding a separate
  nested-container state.
- Build gotchas hit while adding generator states (20 Oct 2026), all resolved:
  - A new `.cpp` under `src/components/data/formats/json/` must be added to the
    `add_library(qor_jsondata ...)` source list **and** the VS project files must
    be regenerated, otherwise the file is silently not compiled and the link
    fails with `LNK2019` unresolved externals. `cmake --build` alone does not
    always re-emit the `.vcproj`; run
    `cmake -B cmakebuild -S . -DCMAKE_BUILD_TYPE=Debug` (no `-G`; the cached
    generator is the VS one, `-G "Visual Studio 2026"` is not a valid name here).
  - Any translation unit that pulls in a model header (`model/value.h` ->
    `currentthread.h`/`polymorphic.h` -> `platform/compiler.h`) must
    `#include "src/configuration/configuration.h"` **first** or the build stops
    with `error C1189`. Copy the prelude of `generator/states/_null.cpp`
    (configuration, `<cstddef>`, generator.h, state.h, result.h, then the short
    relative state header).
  - Running the test exe in the same command batch as the build can execute the
    **previous** exe (stale artifact); always run it in a separate command after
    the build finished, otherwise test counts lag behind the sources.
  - **A `FileConnector` in a pipeline needs the `FileSystem` platform subsystem
    registered by the executable** (20 Oct 2026): `ThePlatform(qor_shared)->AddSubsystem<FileSystem>();`
    plus `qor_pp_module_requires(IFileSystem)` and the
    `src/framework/io/filesystem/filesystem.h` / `src/platform/platform.h`
    includes in the test `main.cpp`. Nothing fails at compile or link time —
    the `FileSystem`/`IFile` module requirements fail **at run time** the moment
    a pipeline touches a file, which makes it look like a pipeline bug.
  - **`LNK2019` for a newly linked library (e.g. `qor_fileconnector`) is usually
    stale project files, not stale code**: `cmake --build` reuses the generated
    `.vcxproj` and never adds the new library's import-lib dependency, so
    regenerate with `cmake -B cmakebuild -S . -DCMAKE_BUILD_TYPE=Debug` before
    rebuilding. `tmp/pefilter.py` (dump PE import libraries) confirmed no
    `qor_fileconnector.lib` was being linked.
  - The `read_files` tool may serve **pre-edit (stale) content** for files that
    were just changed; re-read edited sources with a shell read
    (`Get-Content <path>`) when confirming an edit.
- `src/components/data/formats/json/` contains `model/` and `parser/` only —
  there is no `generator/` folder yet.
- Writing infrastructure exists and is registered: `qor_generator`
  (`src/components/io/generator`: `Generator : Workflow`, `GeneratorState :
  workflow::State`, `ASCIIChar : GeneratorState`, `Context::PutOctet()`,
  `NodeAdapter<T>`) and `qor_generatorsource`
  (`src/components/data/pipeline/sources/generatorsource`).
- `qor_jsondata` links `qor_module qor_host qor_current qor_sync qor_injection
  qor_workflow qor_pipeline qor_parser qor_issue qor_error qor_log` —
  `qor_generator` must be added.
- `QORTestJSON` is the only consumer of `qor_jsondata`; its `main.cpp` now
  resolves test data through the relative path
  `"../../../../../../test/data"` (previously the machine-specific
  `F:/Develop/linuxQOR/test/data`).
- Build/verify order for this task: `qor_jsondata` → `QORTestJSON` → `QORTest`.
- Gotcha: `qor_generator`'s CMake declares `QOR_PARSER` (not `QOR_GENERATOR`) as
  its compile definition — see `docs/archive/jsonGeneratorPlan.md` for details
  before writing new generator `.cpp` files.

## Current focus (STATUS.md, 20/10/2026)
- Adding a generator for JSON so we can roundtrip JSON model <--> JSON file

## Recent commits (`main`, newest first, at HEAD `93668d3d`, 19 Oct 2026)
- `93668d3d` smal fixes — `QORTestJSON` test-data path made relative
  (`F:/Develop/linuxQOR/test/data` → `../../../../../../test/data`);
  `src/components/ui/terminal/CMakeLists.txt` tweak
- `746aeef7` merge with origin/main
- `67c1199d` **prototype generated YAML parser** — new lib `qor_yaml`
  (`src/components/protocols/yaml`: `model/`, `parser/{nodes,states,tokens,helpers}`)
  + `QORTestYAML` (`test/components/protocols/yaml`); ~2,125 insertions; also
  touched `fastflow.cpp` and `sharedref.h`
- `6e6f0280` review-based improvements in `qor_interception` (+ `qor_issue`,
  `threadheap`)
- `11deaaca` HTTP test request-gen scaffold — new target `QORTestHTTPRequestGen`
  (`test/components/protocols/http`; `test_httprequestgen.cpp` is main-only, no
  generator assertions yet)
- `f1631228` small bits of HTTP work (`http/client/requestgenerator` slimmed)
- `715254cc` **Cline memory bank added** (this `memory-bank/` folder)
- older: `6b65785a` DIO MPL namespace fixes · `fefab2aa` mixins ·
  `04f8167f`/`d46e3e55` non-MSVC fixes · `3b6a626a` kvasir-based MPL ·
  `6b1f253a` signal tree tests + JSON test data · `9fedc59f` signal tree ·
  `bb704b47` uri/jip · `b2ea83c4` const/noexcept

## Active branches
`main` (HEAD, = `origin/main`), `Contract`, `DomainInterfaceObjects`,
`FileSystemRefactor`, `PipeConnectorExperiment`, `day1`, `module_support`.

## Next steps (queue — item 1 is the JSON generator task, roundtrip now done)
1. **JSON generator (ROUNDTRIP DONE in `QORTestJSON` — process exit code 0)**:
   model-driven generator states cover literals, strings, scientific-notation
   numbers, non-finite rejection, nested arrays and nested objects, the write-to-file
   pipeline (`GeneratorSource` -> `FileConnector`, `JSONWriter` in
   `test/components/formats/json/jsonwriter.h`) writes model JSON into
   `test/data/json_generator_*.json` in `QORTestJSONGenerator` (`[OK](36/36)`), and
   the parse -> generate -> parse roundtrip in `QORTestJSON` compares equal
   (`jsoncompare.h`, artefacts `test/data/json_roundtrip_*.json`). Remaining:
   the full `QORTest` regression run only. Full plan:
   `docs/archive/jsonGeneratorPlan.md` ("Roundtrip increment" records what was
   verified on 20 Oct 2026).
2. HTTP: systematic refactors + generator infrastructure; complete response
   generation (`QORTestHTTPRequestGen` exists as a scaffold only — add real
   request-generation assertions).
3. YAML: extend the `qor_yaml` prototype past the flow subset (comments, block
   indentation, escapes, numbers/scalars, multi-document).
4. Pipelines: "still need a lot of work"; add pipeline components.
5. Parser edge cases to bottom out: tiny buffer sizes, parse completion.
6. Generator framework for writing text-based files, plus one for portable
   binary formats (the JSON generator is the first concrete instance).
7. Unicode: more code pages, Unicode database → usable data structures,
   normalisation, Bidi; text shaping (hardest).
8. Serialization over pipelines → font and image loading, web server responses.
9. Multiple test executables: separate operational tests (e.g. thread pool
   performance) from fast loop-unrolled unit tests — needs CMake work.
10. Structural and control-flow modules for the framework, and initial
    components for customizing memory management.
11. Remaining export-interface scrubbing (pipeline-component consumers).
12. UI: assemble scene template loading once JSON/YAML sit over Unicode.

## Active decisions & considerations
- **Roundtrip semantics (decided and verified 20 Oct 2026 in `QORTestJSON`)**:
  model equality is defined on **decoded** values, never source text (the model
  retains no escapes); numbers compare as exact doubles because the generator
  renders with `std::to_chars` (round-trip exact); output stays compact (no
  whitespace writer) raw ASCII/UTF-8 with **no** `UnicodeTextFilter` on the write
  side. A string-preserving model type remains open only if exact source-text
  roundtrip is ever wanted.
- **Generator (JSON — pipeline integration AND roundtrip VERIFIED 20 Oct 2026)**: the writing path
  mirrors the reading path — `GeneratorSource` (pipeline **Source**) wrapping
  `Generator : Workflow` with `GeneratorState : workflow::State` states, rather
  than a plain recursive serializer. Note: the **verified** pipeline used `Push`
  flow mode + `.Connect().PumpAll()` (the `JSONReader` drive style), not the
  `Pull` mode that was first assumed. Decisions still open: whitespace policy
  (compact vs pretty), where the `UnicodeTextFilter` sits on output (currently
  bypassed — raw ASCII/UTF-8). The number-formatting decision is **resolved**: the
  generator uses `std::to_chars` (shortest round-trip representation), which is
  what makes the exact-`double` compare in `jsoncompare.h` valid; a string-preserving
  model type is only needed if exact source-text roundtrip is ever wanted.
- **YAML**: the prototype parser was generated from the YAML spec/EBNF
  (`docs/agent/yaml_spec_raw.md`, `docs/agent/yaml_cfg_ebnf.md`) and deliberately
  covers only a flow subset (escapes/numbers deferred); it lives under
  `src/components/protocols/yaml`, not `src/components/data/formats`.
- **UI**: single scene template driving both TUI and GUI; the whole UI subset is
  treated as experimental; Windows/Linux UI bits work on their own terms but
  integration into portable code is largely missing.
- **Pipes**: abstract platform-agnostic pipe over sockets (Linux) vs files
  (Windows).
- **MPL**: new kvasir-based meta programming library alongside `qor/essentials/mpl`.
- **qor_error + strings**: integration works but requires **double linking under
  GCC** — unfortunate and maybe resolvable; otherwise the error-raising interface
  moves into another module.
- Experimental code lives in `qor::exp::` and by 1.0 should graduate to
  `qor::components::…` (`qor::io::components`, `qor::io::network::components`,
  `qor::pipeline::components`, `qor::data::components`) or into main `qor` if
  fundamental.
- Serialization format for UI scenes is a live design question (JSON vs a
  human-readable format). The **YAML parser prototype** is the first concrete
  step toward the human-readable option; the **JSON generator** (write path now
  verified end to end, `[OK](36/36)`) supplies the write path for the JSON option.

- **Subsystem registration is the executable's responsibility** (20 Oct 2026):
  `ThePlatform(qor_shared)` starts with no subsystems, so any executable (test
  exe included) that hooks a pipeline to a file must call
  `ThePlatform(qor_shared)->AddSubsystem<FileSystem>()` and declare
  `qor_pp_module_requires(IFileSystem)`. `test/components/formats/json/generator/main.cpp`
  is now the reference shape for a QOR test exe that uses files: register
  subsystems → `AppBuilder().Build<Application>(...)` → delegate to
  `qor::test::Factory::Instance().main(argc, argv)`.
- **Verified write-side pipeline shape** (20 Oct 2026):
  `GeneratorSource<State>(buffer, model) → FileConnector(file, buffer, OpenFor::WriteOnly, WithFlags::None)`
  driven by `.Connect().PumpAll()`; `GeneratorSource` owns the output
  `qor::components::generator::Context` and implements `Source::ReadBytes`, so
  generation larger than the buffer resumes across pulls. This is the mirror of
  the proven read-side `FileConnector → UnicodeTextFilter → ParserSink(buffer)`.
- **QOR test exes are mini-applications**: they build an `Application` with a
  `Role` and run the test factory inside it; anything the application needs at
  runtime (platform subsystems) must be set up there, not assumed by libraries.

## Learnings & project insights
- `qor_pp_assert_that(expr)` alone is vacuous and `qor_pp_assert_that(expr).isTrue();`
  must be chained; a failing chain **throws `qor::test::failure`**, so a test block
  that is not wrapped in `try` / `catch` aborts the rest of the `Run` lambda and
  hides later fixtures — wrap new assertion blocks and report with
  `qor::log::Debug` (`log::Error` is not in the `Log` interface).
- JSON model ownership is **ref-counted**: `NodeAdapter<T>` holds a
  `ref_of<T>::type m_t` and `GetObject()` returns it **by value**, so
  `push_back((*it)->GetObject())` keeps the model alive. Generator "dangling data"
  theories are usually wrong; the real defects were state-population ones, recorded
  in `memory-bank/systemPatterns.md` (`string::Emit` overwriting live-appended text,
  `_object` skipping the closing brace when `m_count == 0`).
- The parser pattern is now proven **twice**: JSON (`src/components/data/formats/json`)
  and the generated YAML prototype (`src/components/protocols/yaml`). ABNF/EBNF →
  combinator composites (`Sequence`/`Optional`/`ZeroOrMore`/`OneOrMore`/`Specific`/`AnyOneOf`)
  → `states/_1`–`_4` + `nodes/` adapters + `model/` types + `tokens.h` enum.
  `docs/agent/parsers.md` and `docs/agent/add_module.md` are the skill files
  capturing this; the JSON generator is the mirror image of the same pattern.
- A brand-new module + its own test executable was verified end-to-end in Oct 2026
  (`qor_yaml` → `qor_yaml.dll`, `QORTestYAML` → `yaml_module_loads` OK, exit 0):
  the "adding a module is 5–10 minutes of work" claim holds, but adding a
  **grammar-based** module is a multi-day job — budget accordingly.
- Gotcha hit during YAML work: `AnyOneOf_t` has **no single-alternative form**
  (it requires two), which forced stubbing `character.h` as
  `AnyOneOf_t<unescaped, unescaped>`. Expect similar shape constraints on
  generator composites.
- The organizational structure is holding at large scale: ~66 cross-platform
  libraries, ~2,810 source files, 207 CMake targets in `src/`, with
  dependencies and sizes not out of control — "this kind of proves it works at
  least at this scale".
- Pedantic builds (`-Wall -Wextra -Wpedantic`, hidden visibility) are now
  achievable for most `/qor` libraries; some (e.g. `qor_mock`) probably never
  will be, given what they do.
- "Nothing scales forever"; the structure is judged good for anything one person
  can linearly write in a year or two.
- Style of QOR code is now clear; suggestions of extensions, specializations and
  improvements to qor modules are welcome.
- Build-time expectations: full build < 30 min on a reasonable PC (~26 min on
  GitHub); clean build of the incomplete HTTPServer example ~38 s.

## Working preferences observed
- Follow `docs/skills/QORconventions.md` exactly (license header, `configuration.h`
  first include, `QOR_PP_H_*` guards, `qor_pp_module_interface` qualifiers).
- New folder ⇒ add `purpose.md` (and `status.md` for volatile state).
- Keep the "rule of 7" in mind when creating folders/namespaces.
- Prefer `new_ref<T>()` smart references over raw `new`/`delete`/`unique_ptr`.
- A **second, parallel agent-state system** exists in `docs/agent/` (`TODO.md`
  queue, `STATE.MD`, `DECISIONS.md`, `skill.md`, per-lib ctags in
  `docs/agent/ctags/`, `repo_map.md`, `purposes.md`, `deps.dot`,
  `verification.md`, skill files `add_module.md` + `parsers.md`). Keep it in
  sync with this memory bank when working; `docs/agent/skill.md` drives that
  system from `TODO.md`.
- Scratch/transient artifacts live tmp folder and in
  `docs/agent/_tmp_*` — do not treat them as documentation.
- Grammar-driven work is done by **generating** the states/nodes/model code from
  the ABNF/EBNF (`docs/agent/yaml_cfg_ebnf.md`), not hand-writing it; the JSON
  generator should follow the same route.
