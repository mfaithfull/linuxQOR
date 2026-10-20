# Progress

**Last updated:** 20 Oct 2026 — the JSON generator increment is **COMPLETE**: the
full `QORTest` regression run (the base framework suite) is **green at
`[TOTAL](678/678)` / `[OK](678/678)`, process exit code 0**, on top of the parse ->
generate -> parse **roundtrip** being green in `QORTestJSON` (process exit code 0)
and the **pipeline integration** step (a JSON model written to a real file through a
real pipeline: `GeneratorSource` -> `FileConnector`) **DONE and verified at
## MIME Recognition — qor_mime (20 Oct 2026, outline only)

### What was done
- Researched QOR pipeline filter anatomy first (see `memory-bank/qor_mime_plan.md` step R):
  `qor::pipeline::Filter` (Sink+Source), filter overrides `ReadFilter`/`WriteFilter`,
  existing filters `base64encodefilter`/`linereader`/`jpegdecodefilter`/`unicodetextfilter`,
  test exe pattern from `QORTestYAML`.
- New module `src/framework/data/mime` → lib `qor_mime` (SHARED, `QOR_MIME`), namespace `qor::mime`,
  class `MIMERecognizerFilter`, `enum class MIMEType` covering the ten popular types plus
  `application/octet-stream` fallback and `Unresolved`.
- New test target `test/framework/data/mime` → exe `QORTestMIME` (placeholder `mime_module_loads`).
- ctest removed project-wide (user rule "the QOR does not use ctest"): deleted the
  `enable_testing()` + `add_test(...)` blocks from the four leaf test CMakeLists that had them
  (`test/framework/data/mime`, `test/components/protocols/yaml`, `test/components/formats/json/generator`,
  `test/uri`). Repo-wide grep of every `CMakeLists.txt` for `add_test|enable_testing` → **0 hits**.


### Outcomes
- ✅ `qor_mime` compiles and links: `build\src\framework\data\mime\Debug\qor_mime.dll` + import lib,
  no warnings.
- ✅ `QORTestMIME.exe` runs `[OK](1/1)` exit 0 — module loads, filter constructs.
- ✅ Re-verified after the ctest removal: reconfigure SUCCESS; every affected exe rebuilt and
  re-run **from its own build folder** — `QORTestURI` `[OK](1/1)`, `QORTestYAML` `[OK](1/1)`,
  `QORTestJSONGenerator` `[OK](36/36)`, `QORTestMIME` `[OK](1/1)`, all exit code 0. Note: the
  JSON generator tests write to `../../../../../../../test/data` **relative to the exe**, so
  running that exe from the repo root reports false `[FAIL](5/36)`.

- ⚠️ `framework/data` and `framework/data/mime` were NOT in the namespace docs (`namespace.md`
  0 hits for `qor::mime`) but they ARE live at build time — `src/framework/data/CMakeLists.txt`
  already registered `pipeline` (lib `qor_pipeline`), so the layer is real and unbuilt-to-test
  only because `test/CMakeLists.txt` never included `test/framework/data`. Now it does.
- ⚠️ Naming/layer decision recorded: user specified the framework layer
  (`src/framework/data/mime`); existing filters live at the components layer. Kept framework layer
  because the class derives from the framework `qor::pipeline::Filter` (the default filter for any
  pipeline). If a components-layer home is preferred later, the move is folder + one CMakeLists line.

### What was NOT done
- ❌ NO MIME detection logic yet — `Inspect()` is a documented placeholder (pass through).
  Detection is added test-first, one type per increment (plan steps 2..13).
- ❌ No consumer wired up yet (the HTTP Content-Type TODO, UI renderer loader,
  `Resource::Type()` remain untouched).


`[OK](36/36)`** in the dedicated `QORTestJSONGenerator` target. Repo-wide `STATUS.md`
still trails the tree, so `git log`, `docs/agent/STATE.MD`, and this memory bank are
the fresher records.

## ▶ JSON generator (COMPLETE — roundtrip GREEN in `QORTestJSON`; full `QORTest` regression run GREEN at `[OK](678/678)`)

Add a qor_pipeline-based JSON **generator** to `qor_jsondata` — the writing half
mirroring the JSON parser — plus a `JSONWriter` helper and roundtrip tests in
`QORTestJSON` (`test/components/formats/json`). Detailed step-by-step plan:
`docs/archive/jsonGeneratorPlan.md`. **Pipeline integration done and verified
20 Oct 2026 at `[OK](36/36)`**: `qor_jsondata` contains `generator/states`
model-driven states, `test/components/formats/json/jsonwriter.h` provides
`JSONWriter` = `GeneratorSource<state>(buffer, model)` source +
`FileConnector(file, buffer, OpenFor::WriteOnly, WithFlags::None)` sink sharing
one `qor::pipeline::PODBuffer`, driven by `.Connect().PumpAll()`, and the 5 file
tests write `test/data/json_generator_*.json`. Roundtrip now done and verified
20 Oct 2026 (see item 13 below): three fixtures written by
`JSONWriter`/`JSONPartWriter` and re-parsed with equal
`CompareValue` / `CompareObject` / `CompareArray` asserts
(`test/components/formats/json/jsoncompare.h`), so `QORTestJSON` exits 0. A full
`QORTest` regression run is now DONE and green as well (`[TOTAL](678/678)` /
`[OK](678/678)`, process exit code 0 — see the "Full QORTest regression run" note
below). The original
prerequisite check on 19 Oct 2026 found no `generator/` folder.

## What works (verified in STATUS.md / FEATURES.md)
- The full tree **builds on GitHub Ubuntu-latest** as of 27/07/2026.
- Base libraries are clean even with all pedantry flags; clean examples can be
  built.
- **Stable portable modules** (FEATURES.md): `qor_application`,
  `qor_asyncioservice`, `qor_base64{encode,decode}filter`, `qor_current`,
  `qor_datasink`, `qor_datasource`, `qor_echo`, `qor_error`,
  `qor_fileconnector`, `qor_filesystem`, `qor_host` (module system host + type
  registry), `qor_injection` (DI + type identity), `qor_interception` (AOP),
  `qor_issue` (base flyer for errors and logs), `qor_linereader`, `qor_log`,
  `qor_logaggregator`, `qor_module` (static), `qor_netclient`, `qor_netserver`,
  `qor_optionparser`, `qor_pipeline` (anything→anything, push/pull, filters),
  `qor_platform`, `qor_randomsource`, `qor_role`, `qor_signals`,
  `qor_socketconnector`, `qor_sockets`, `qor_stdfstreamconnector`,
  `qor_stdifstreamconnector`, `qor_stdifstreamsource`, `qor_stdinsource`,
  `qor_stdofstreamsink`, `qor_stdoutsink`, `qor_stringsink`, `qor_text`
  (CoW strings, string functions, UTF-n and code pages), `qor_thread`
  (threads + thread pool), `qor_workflow` (state machine).
  Stable-but-incomplete: `qor_compiler`, `qor_arch`, `qor_loghandler`.
  Stable with changes pending: `qor_sync` ("naming inconsistency — it's a zoo
  right now"), `qor_task` (namespace/naming).
- **Stable platform modules**: `winqor_api_winsock2`,
  `winqor_api_asyncioservice` (IOCP, cancellation, coroutine based),
  `winqor_common` (handles), `winqor_exebootstrap`, `winqor_sharedbootstrap`,
  `winqor_filesystem`, `winqor_process`, `winqor_sockets`, `winqor_thread`;
  `linqor_filesystem`, `linqor_process`, `linqor_sockets`, `linqor_thread`,
  `linqor_asyncioservice` (needs cancellation adding).
- **Working (insufficient testing to trust)**: `qor_reporting`,
  `qor_threadmemory`, `qor_stringsource`, `qor_dynamiclibrary`,
  `winqor_dynamiclibrary`, `linqor_dynamiclibrary`, `qor_parser`,
  `qor_parsersink` (semi-stable, will be refactored for Unicode).
- **Stable sample apps**: `helloworld`, `client` (echo), `server` (echo),
  `options`, `outline` (self version reporting), `parallel` (threads + log
  aggregation), `plumbing` (pipelines), `work` (workflows), `roller` (roles),
  `httpserver` (stable, incomplete).

- TDD tool set (assert + test + mock) exercised by `QORTest`; UTF-8/UTF-16/UCS4
  string classes done and tested (integration with `qor_error` worked out);
  RFC 5234 parser framework "just about done", reads most Unicode text
  encodings.
- Dedicated test executables exist for `tef`, `frozen`, `dio`, `uri`, `mpl`,
  `argparse`, `signaltree`, `workitem`, `operational` (`QOROpTests`: thread
  pool + profiling), `filesystem`, `pipeline`, `ui`, `components/formats`
  (`QORTestJSON`), `components/protocols/yaml` (`QORTestYAML`),
  `components/protocols/http` (`QORTestHTTPRequestGen`).
- **20 Oct 2026 verified addition — JSON generator file pipeline**: `QORTestJSONGenerator`
  runs **`[OK](36/36)`** with 5 tests that write JSON files through
  `GeneratorSource` → `FileConnector`; artefacts land in `test/data`
  (`json_generator_object.json` 7 bytes, `json_generator_writer.json` 7,
  `json_generator_array.json` 5, `json_generator_value.json` 1,
  `json_generator_nested.json` 16). The test exe registers the `FileSystem`
  platform subsystem in `main.cpp`
  (`ThePlatform(qor_shared)->AddSubsystem<FileSystem>()` +
  `qor_pp_module_requires(IFileSystem)`) and links
  `qor_fileconnector qor_filesystem` (Windows branch also
  `winqor_filesystem winqor_common`).
- **Oct 2026 verified additions**: `qor_yaml` (`src/components/protocols/yaml`)
  prototype parser — model + `parser/{tokens,helpers,nodes,states}` generated
  from the YAML EBNF — builds and `QORTestYAML` runs
  `yaml_module_loads` OK (exit 0); `QORTestHTTPRequestGen` target builds
  (scaffold only: `test_httprequestgen.cpp` has `main` + `ThisModule`, no
  assertions yet); `qor_interception`/`qor_issue` review fixes compile.
  `QORTestJSON` reads `test/data` via the relative path
  `"../../../../../../test/data"` after the workspace move to
  `c:\RAID0\Develop\linuxQOR`.
- Platform UI primitives: Wayland layer, X Windows layer, Windows GDI layers
  written; layout library works; terminal code mostly ported from FTXUI;
  Windows UI bits (comctl32, direct2d, dwm, gdi, opengl, egl, console,
  desktopui, termscreen) and Linux UI bits (x, xegl, xdgshell, waylandclient,
  waylandegl, viewporter, presentationtime, egl, opengles, termscreen, console)
  exist as experimental modules.

## In progress / incomplete
- **UI design iterating** — the entire UI subset is considered experimental.
  Windows and Linux specific UI works on its own terms; integration into
  portable code is largely missing.
- **JSON** — parsing works, builds a minimally simple read-only object model;
  **generator states and the file-writing pipeline both work** (literals, strings,
  scientific-notation numbers, nested arrays/objects written straight into
  `test/data` files — `[OK](36/36)`). Remaining for the generator:
  the full `QORTest` regression run (the roundtrip comparison in `QORTestJSON` is
  done and green); it must still
  be layered over Unicode before it can carry UI scene templates.
- **YAML** — `qor_yaml` prototype covers a **flow subset** only: `ws`,
  `unescaped`, `quotation_mark`, `value_separator`, `character`,
  `begin_sequence`/`end_sequence`, `begin_mapping`/`end_mapping`,
  `name_separator`, `_true`/`_false`/`_null`, `value`, `string`,
  `sequence`, `mapping`, `pair`. Deferred: comments, block indentation,
  escapes, numbers/scalars, multi-document, real fixture tests.
- **HTTP** — request parser works, response generator mostly complete;
  incomplete; needs systematic refactors + generator infrastructure
  (`QORTestHTTPRequestGen` is a scaffold, request generation untested).
- **Pipelines** — "still need a lot of work"; serialization over pipelines
  started (working, incomplete examples of parsing/generating file content).
- **Incomplete / experimental modules**: `qor_image`, `qor_ttf`, `qor_unicode`,
  `qor_json`, `qor_jpegdecodefilter`, `qor_deserializersink`, `qor_events`,
  `qor_generator`, `qor_generatorsource`, `qor_aggregatesource`,
  `qor_property`, `qor_resources`, `qor_textshaping`, `qor_serialize`, plus all
  UI modules (`qor_canvas`, `qor_console`, `qor_egl`, `qor_layout`,
  `qor_opengles`, `qor_tui`, `qor_ui`, `qor_uiselector`, `qor_widgets`,
  `qor_renderer`).
- **Module export interface scrubbing** — major work in progress; anything using
  pipeline components does not build cleanly yet.
- **No release shipped**: `CHANGES.md` has no entries; the first potentially
  releasable milestone is **0.5** (originally sketched as "a Linux and Windows
  C++ testing framework that combines yaffut, hippomocks and assertcc, maybe"
  — the project has grown far beyond that).
- Sample UI apps (`button`, `dialogs`, `editbox`, `mainwindow`, `menu`,
  `minimal`, `expconsole`) are experimental, mostly Windows only.

## Known issues
- **Machine-specific paths** — the workspace moved to
  `c:\RAID0\Develop\linuxQOR`; `QORTestJSON` was fixed to the relative path
  `"../../../../../../test/data"`, but root `CMakeLists.txt` EGL/OpenGLES
  `HINT` paths (`F:/Develop/Native_SDK/...`) are still machine-specific and
  Windows-only.
- `qor_error` + text/string integration requires **double linking under GCC** —
  unfortunate and maybe resolvable; fallback plan is moving the error-raising
  interface into another module.
- `qor_sync` naming is inconsistent ("it's a zoo right now"); `qor_task`
  namespace/naming changes pending.
- Parser edge cases not bottomed out: tiny buffer sizes, parse completion.
- Some modules can never pass full pedantic compilation (e.g. `qor_mock`).
- Test executables get too bulky/too slow; operational (performance) tests need
  separating from fast unit tests → CMake work for multiple executables.
- Windows builds require an external EGL/OpenGLES SDK; root `CMakeLists.txt`
  HINT paths are machine-specific (`F:/Develop/Native_SDK/...`).
- `winqor_exebootstrap` / `winqor_sharedbootstrap` "may be redundant".
- `qor_http_old` remains alongside `http` (legacy).
- Non-MSVC compilers need repeated fixes (commits `04f8167f`, `d46e3e55`).

## Evolution timeline (STATUS.md)
- **Mar 2026** — repo set up, project laid out.
- **End Jan 2026** — recognized as a large project; 66 cross-platform libraries;
  organizational structure holding, dependencies and sizes not out of control;
  next: get a real grip on serialization over pipelines.
- **1st Mar 2026** — serialization over pipelines occurring; font loading
  started; techniques applied to HTTP requests/responses.
- **End May 2026** — lots of Win32/GUI primitives; project restructuring for
  cross-platform GUI; multiple test executables identified as needed.
- **June 2026** — fundamentals quality work; pedantic builds possible; UTF-8,
  UTF-16, UCS4 strings done; `qor_error` integration worked out; RFC 5234
  parser framework near done; "may be in 0.5 territory".
- **July 2026** — 0.5 interim release plan; portable modules versioned for 0.5;
  export interface scrubbing; HTTP next; r/cpp engagement.
- **29 Jul 2026** — UI coalescing; parser reads most Unicode encodings; JSON
  needs Unicode layering; platform-agnostic pipes experiment.
- **16–19 Oct 2026** — agent tooling added (`docs/agent/` repo map, per-lib
  ctags, `deps.dot`, skill files `add_module.md`/`parsers.md`); Cline
  **memory bank added** (`715254cc`); HTTP generator work +
  `QORTestHTTPRequestGen` scaffold; `qor_interception` review fixes;
  **generated YAML parser prototype** (`qor_yaml`) + `QORTestYAML`;
  workspace migrated to `c:\RAID0\Develop\linuxQOR` and the `QORTestJSON`
  test-data path made relative. **JSON generator** work continued into 20 Oct 2026: scientific notation,
non-finite guards and nested containers verified at `[OK](31/31)`, then the
**model → pipeline → file write path (`GeneratorSource` + `FileConnector`,
`JSONWriter`) verified at `[OK](36/36)`**.

## What's left before 0.5
**full `QORTest` regression run (JSON generator roundtrip verified in
`QORTestJSON`)** · HTTP completion + generator infrastructure ·
pipeline components · YAML beyond the flow subset · JSON over Unicode · text-file
and portable-binary generator frameworks · Unicode database
data structures / normalisation · UI scene-template assembly · multi-executable
test build · remaining export-interface scrubbing · structural and control-flow
framework modules · memory-management customization components · Mac support
(clang-llvm first, FileSystem next).

## JSON generator — detailed plan (roundtrip now done)

See `docs/archive/jsonGeneratorPlan.md` for the full step-by-step plan to add a
qor_pipeline-based JSON generator to `qor_jsondata` and matching roundtrip test
code to `test/components/formats/json`. Its status as of 20 Oct 2026 is
**implemented through the pipeline step**: generator states, scientific-notation
doubles, non-finite rejection and nested model containers are implemented and
tested in the dedicated `QORTestJSONGenerator` target, and the model → pipeline →
file write path is verified there too (36/36 overall). `JSONWriter` / `GeneratorSource`
pipeline integration is **done**, and so is the roundtrip (`QORTestJSON` parses a
fixture, writes it with `JSONWriter`/`JSONPartWriter`, re-parses it and compares
via `jsoncompare.h`, process exit code 0); the only remaining step is the full
`QORTest` regression run.

## JSON generator — increments verified through 20 Oct 2026 (13 increments, roundtrip verified, built incrementally)

Done and VERIFIED (each step rebuilt + run green, see
`memory-bank/activeContext.md` for details):

1. Empty test target `QORTestJSONGenerator` (`test/components/formats/json/generator/`,
   `QOR_BUILD_QORTESTJSONGEN` gate, registered by `add_subdirectory(generator)` in
   the json test CMakeLists) builds, runs, `[OK](1/1)`.
2. `qor_jsondata` gained `generator/states/_null.cpp` (+ `qor_generator` link) and
   a `json_generator_writes_null` test passes on a real `Generator`+`Context` run.
3. Same for `_true`/`_false` — 3 tests, `[OK](3/3)` with real assertions.
4. `_string` (payload + length, emits `"payload"`) — 4 tests.
5. `_number` (`long long` -> decimal text in a 21-byte buffer, optional `-`) —
   `json_generator_writes_zero` + `json_generator_writes_negative_integer`,
   `[OK](6/6)`.
6. `_array` -> `[]` and `_object` -> `{}` — `[OK](8/8)`.
7. `_string` extended to escape raw payload text (`\"`,`\\`,`\b\f\n\r\t`,
   `\u00xx`) — `[OK](10/10)`.
8. `_value` — model-driven entry state reading
   `qor::data::model::json::Value::GetType()`, pushing `_null`/`_true`/`_false`,
   `Result::FAILURE` + pop for the types whose states still need model data —
   `[OK](11/11)` first-run green.

Second run (19->22 tests) — same states, now with the model-driven routing that
reads real model data:

9. `_array` model-driven (`m_values` element count drives brackets + commas) and
   `_object` model-driven (`m_members` drives braces + colons + commas), wired by
   `_value` for `ValueType::array`/`object` via `dynamic_cast` — `[OK](19/19)`.
10. `_number` second ctor reading a `double` from `qor::data::model::json::Number`
   and rendering `integer [ frac ]` (no leading plus, no exponent, non-finite
   rejected via `std::isfinite` -> `Result::FAILURE`), wired by `_value` for
   `ValueType::number` — `[OK](22/22)` green as of 20 Oct 2026.
11. Model `Number` doubles are now rendered through `std::to_chars`, producing
   shortest round-trip decimal/exponent text (`1e+10`, `-1e-10`, `1.5`).
   Non-finite values are rejected in `_value` before entering `_number`, with a
   defensive `std::isfinite` guard in `_number` as well. Nested `Array`/`Object`
   models are attached with `polymorphic<qor::data::model::json::ValueModel>(inner)`
   and re-entered through `_value`. Corrected byte-count assertions for object-member
   output (`{"a":7}` is 7 bytes, not 6). Verified by scientific notation,
   non-finite rejection, nested array, nested object, nested number/string, and
   object-member tests -> `[OK](31/31)`.

12. **Pipeline integration — VERIFIED 20 Oct 2026 (`[OK](36/36)`)**: `qor_generatorsource`
    gained a variadic `GeneratorSource(const pipeline::Buffer&, TArgs&&... args)`
    ctor (creates `m_context = new_ref<qor::components::generator::Context>()`,
    `SetContext`, `SetBuffer`, builds the initial state with
    `new_ref<TObjectState>(&m_generator, std::forward<TArgs>(args)...)`) and a
    `virtual size_t ReadBytes(byte*, size_t)` pipeline-`Source` hook that returns
    0 once `m_generator.IsComplete()` (ends the pipeline) and otherwise generates
    into the reserved space (a state that runs out of space keeps its stack, so
    the next pull resumes at the saved position). `test/components/formats/json/jsonwriter.h`
    adds `JSONPartWriter<State, Model>` / `JSONWriter`
    (`GeneratorSource` source + `FileConnector(outFile, buffer, OpenFor::WriteOnly, WithFlags::None)`
    sink on a shared `qor::pipeline::PODBuffer`, driven by
    `.Connect().PumpAll()`), and 5 new tests write
    `test/data/json_generator_*.json`. Two fixes were needed to get green: the
    exe registers the `FileSystem` platform subsystem in `main.cpp`, and the
    Visual Studio project files had to be regenerated to clear `LNK2019`.

13. **Roundtrip (parse -> generate -> parse) — DONE and VERIFIED 20 Oct 2026**
    in `QORTestJSON` (the existing target, no new exe): parse a fixture with
    `JSONReader` / `JSONPartReader` / `JSONValueReader`, write that model out
    through `JSONWriter` / `JSONPartWriter` into `test/data`, re-parse the file
    into a second model and compare with the new
    `test/components/formats/json/jsoncompare.h`
    (`CompareValue`/`CompareObject`/`CompareArray`, mutually recursive, decoded
    values only, exact `double` compare enabled by the generator's `std::to_chars`).
    Artefacts `test/data/json_roundtrip_object.json` (`{"asd":"sdf"}`),
    `json_roundtrip_array.json` (`[null,1,"1",{}]`),
    `json_roundtrip_value.json` (`42`); `msbuild QOR.sln /t:QORTestJSON` build
    exit 0, run exit code **0**. Two `qor_jsondata` defects had to be fixed first:
    unconditional `SetValue` in `parser/states/_4/string.cpp` (wiped text already
    appended live by `character::Emit`) and the missing `}` for `m_count == 0`
    objects in `generator/states/_object.cpp`. The dangling-ownership theory was
    disproved — `NodeAdapter<T>` holds a ref-counted `ref_of<T>::type m_t` and
    `GetObject()` returns by value. **Coverage caveat:** only 3 fixture shapes
    were compared, not the ~11 named Step 6 cases — escapes / `\uXXXX`, Korean
    UTF-16 through generate, whitespace normalisation, empty `[]` (the `_object`
    empty fix has no array counterpart) and nested object-in-object are still
    untested; see `docs/archive/jsonGeneratorPlan.md` ("Coverage vs the Step 6
    case list").

**Full `QORTest` regression run — DONE and VERIFIED 20 Oct 2026:** regenerated
`cmake -B cmakebuild -S . -DCMAKE_BUILD_TYPE=Debug` (VS 17 2022 / Debug, no `-G`),
built `cmake --build cmakebuild --target QORTest --config Debug` (build exit 0),
then ran `.\QORTest.exe` **from `cmakebuild/test/Debug`** capturing both streams —
**`[TOTAL](678/678)` / `[OK](678/678)`, process exit code 0**, no `[FAIL` and no
`[ALL FAILURES]` block. The base suite is unaffected by the `qor_jsondata` /
`qor_generatorsource` changes, so the JSON generator increment is complete. The
roundtrip comparison and the `JSONWriter` / `GeneratorSource` pipeline integration
(`[OK](36/36)` in `QORTestJSONGenerator`) were already **done** (verified 20 Oct 2026
in `QORTestJSON`, process exit code 0 — see item 13 above).

**Pre-flight for that increment (and the increment itself, 20 Oct 2026):** `tmp/`
scratch folder emptied (transient build/test logs + probe binaries; `/tmp` is
gitignored), `generator/` folder gained `status.md`, and the three reusable-part
blockers (`GeneratorSource` could not pass model data, never created/set its
`Context`, multi-pull generation had to resume across `Pull()`) were **fixed** in
`src/components/data/pipeline/sources/generatorsource/generatorsource.h` by the
variadic buffer ctor and the `ReadBytes` hook. They remain recorded in
`docs/archive/jsonGeneratorPlan.md` ("Prepared next increment") as the original
pre-flight findings.

Build gotchas found on 20 Oct 2026 while adding generator states and wiring the
file pipeline (all recorded in
`memory-bank/activeContext.md`): a new library `.cpp` needs the `qor_jsondata`
source list **plus** a CMake project regeneration (`cmake -B cmakebuild -S .
-DCMAKE_BUILD_TYPE=Debug`) or it is silently not compiled (`LNK2019`); model
headers force `#include "src/configuration/configuration.h"` to be first (`error
C1189`); running the test exe in the same command batch as the build can run the
previous exe; **a `FileConnector` in a pipeline needs the `FileSystem` platform
subsystem registered by the executable**
(`ThePlatform(qor_shared)->AddSubsystem<FileSystem>()` +
`qor_pp_module_requires(IFileSystem)` + the `filesystem.h`/`platform.h` includes
in `main.cpp`) — it fails only **at run time**, so it masquerades as a pipeline
bug; and **`LNK2019` on a newly linked library such as `qor_fileconnector` was
stale generated `.vcxproj` files, not bad code** — `cmake --build` alone never
re-emitted them after the link-list edit, so an explicit
`cmake -B cmakebuild -S . -DCMAKE_BUILD_TYPE=Debug` regeneration was required
before the target rebuilt and linked (diagnosed by dumping PE import libraries
with the `tmp/pefilter.py` helper, which showed no
`qor_fileconnector.lib` being linked). Also on 20 Oct 2026: the file tests are
cwd-sensitive — `test_jsongenerator.cpp` uses
`constexpr const char* outputFolder = "../../../../../../../test/data";`
(line 675) relative to the **exe's own build directory**, so the exe must be run
from `cmakebuild/test/components/formats/json/generator/Debug` (`[OK](36/36)`,
exit 0); run from the repo root the 5 file tests throw `unknown exception`
(`[FAIL](5/36)`, exit 5) — a path artefact, not a pipeline fault.

Critical test-framework lesson found on 20 Oct 2026 (must be respected by all
later JSON generator tests):
- `qor_pp_assert_that(expr)` alone is VACUOUS — it builds a `BoolSubject`
  temporary and nothing is checked. The proposition must be chained:
  `qor_pp_assert_that(expr).isTrue();` (see `test/assert/bool.cpp`).
- Failures are reported on stderr: capture BOTH streams (`exe *> log`) or the
  summary looks falsely clean. With a real false assert the harness prints
  `[FAIL](1/4)` and an `[ALL FAILURES]` block, so `[OK](n/n)` is trustworthy
  only when both streams are captured.
