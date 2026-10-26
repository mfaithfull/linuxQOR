# Progress

**Last updated:** 26 Oct 2026 - **winqor_api_kernel threadpool callback-environ six WRAPPED (D11 implemented)**:
`api_layer/library.h` gained `inlineCall` / `voidInlineCall`, `kernel32.h` gained the `qor_pp_usesinlineapi` marker plus
the two previously missing callback-environ declarations, and `kthreadpool.cpp` gained the six compile-time wrappers.
Audit `tmp/_tmp_audit_v8.txt` reads `implemented=368 header_inline_nonexport=2 missing=0 total=370`; `winqor_api_kernel`
rebuilt exit 0 with 0 error lines; `dumpbin` on the fresh `kthreadpool.obj` shows zero undefined `Threadpool` externals
and the six SDK definitions as local symbols, i.e. compile-time binding with no import.
**Next:** decide whether `SetThreadpoolCallbackPriority` / `SetThreadpoolCallbackPersistent` get the same one-line wiring.

**Earlier increment, 22 Oct 2026 - qor_mime COMPLETE**: plan steps 0-17 done and verified green
(`QORTestMIME.exe` `[TOTAL](18/18) [OK](18/18)`, exit 0), and its plan file retired out of
`memory-bank/` to `docs/archive/qor_mime_plan.md`. The JSON generator increment is likewise
**complete**: pipeline integration `[OK](36/36)` in `QORTestJSONGenerator`, the parse -> generate
-> parse roundtrip green in `QORTestJSON`, and the full `QORTest` regression run green at
`[TOTAL](678/678)` / `[OK](678/678)` with process exit code 0 (its plan file already retired to
`docs/archive/jsonGeneratorPlan.md`). **Also 22 Oct 2026:** the `qor_jsondata` JSON **parse-performance**
verification phase is **COMPLETED (round 18)** — a reproducible per-parse record in `QORTestJSON`
(`[perf] 2MB JSON parse (cold|warm)` lines over fixture `test/data/data_2mb.json`, **2,219,028 bytes**, 21 RelWithDebInfo
runs across 7 builds in `cmakebuild_rel` + 1 Debug run, all exit 0) shows the three micro-edits are
**performance-neutral within the noise floor** (the 13.5% spread across builds of identical sources exceeds the
apparent -11% delta); the "56 s -> 18 s" figure is void and the round-16 record (fixture `test/data/big_json.txt`, `tmp/pristine_qorjson`
tree, `PARSE_TIME_MS` env mode) is unreproducible because those artifacts do not exist. TODO 14 is done and the user
**KEPT all three micro-edits** (committed 22 Oct 2026 as `dea24af2 "JSON parser performance review concluded"`, so HEAD
now equals the edited tree); `tmp/` was pruned to the round-18 evidence (45 files / 300,687 B deleted).
**Next:** nothing is queued — the structural refactoring queue (TODO 15) is **DEFERRED 22 Oct 2026 until greater
resources are available**.

## ▶ qor_mime (COMPLETE - plan steps 0-17 done and verified 22 Oct 2026; plan file retired to `docs/archive/qor_mime_plan.md`)

### What works (qor_mime)
- `src/framework/data/mime` (lib `qor_mime`, framework layer) provides `qor::mime::MIMERecognizerFilter`
  - a pipeline filter that recognises ten popular MIME types plus an `application/octet-stream`
  fallback. `QORTestMIME.exe` runs `[TOTAL](18/18) [OK](18/18)` exit 0 (22 Oct 2026).
- Recognition is total on its targets: all 10 types (PNG, JPEG, GIF, PDF, ZIP, XML, HTML, JSON,
  JavaScript, plain text) plus the fallback; proven across pumped chunk boundaries (an 8-byte magic
  split over 3-byte chunks), across `Reset()` (one instance reused for a second stream), and when
  chained in front of an encoding filter (`qor_base64encodefilter`) - it classifies the PRE-encoded
  octets, so a recognising filter stays valid in front of any downstream transform.
- The application-level payoff works too: the recognised registered name drives sink selection
  (glue in the test, not in the module).

### What was done (compressed - the step-by-step record is in `docs/archive/qor_mime_plan.md` and `src/framework/data/mime/status.md`)
- Researched QOR pipeline filter anatomy FIRST (`docs/agent/add_module.md`, `src/framework/data/pipeline/*`,
  the existing filters and their tests), then scaffolded the module (SHARED lib, module singleton,
  `purpose.md` + `status.md`) and its mirror test target (`test/framework/data/mime` -> `QORTestMIME`).
- Grew the recogniser test-first, one increment at a time: pass octets through -> capture leading
  octets into the signature window -> recognise PNG, JPEG, GIF, PDF, ZIP, XML, HTML, JSON,
  JavaScript and plain text one type at a time -> the octet-stream fallback -> the chunk-boundary
  proof -> the `Reset()` reuse proof -> chaining with `qor_base64encodefilter` -> the recognised
  name driving a sink.

### Constraints found on the way (now recorded as patterns, not step logs)
- Base `qor::pipeline::Filter` moves no octets and its Pull path is broken at the framework level
  (`Filter::Read` hands `ReadFilter` a zero unit count at entry); tests pump in
  `Element::FlowMode::Push` under a `Pipeline` parent. -> `memory-bank/systemPatterns.md`
- Peeking without consuming requires the three-step buffer read protocol (`ReadRequest` -> copy ->
  `ReadReject`); a filter must never acknowledge source data it has not passed on. -> `systemPatterns.md`
- A pass-through filter must keep the units it CONSUMED apart from the units its sink buffer PRODUCED,
  because an `InlineFilter` sink rewrites the count handed to `WriteAcknowledge`. -> `systemPatterns.md`
- `log::debug` is not wired to output in these test exes, so it is not a usable probe - assert on
  observable API state instead. Build/run gotchas (one target per `cmake --build`, separate build
  then run commands, run from the exe's own build dir, logs in gitignored `tmp/`) ->
  `memory-bank/techContext.md`

### What was NOT done (qor_mime) - nothing outstanding
- No open scope: plan steps 0-17 are done and verified, including the optional application-level
  step 17.
- Deliberate non-scope kept (still the right call for future work): no file sniffing, no extension
  mapping, no full RFC 2046 type registry, no streaming special case for huge files (the 16-byte
  signature window is by design), no new stream/detector types beyond the ten targets.

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

## ▶ qor_jsondata JSON parse performance (COMPLETED round 18, 22 Oct 2026 — TODO 14 DONE, all three micro-edits KEPT, TODO 15 DEFERRED)

### What works (reproducible on this tree)
- Per-parse timing harness in `test/components/formats/json/main.cpp` lines 284-305: the 2 MB
  `test/data/data_2mb.json` (**2,219,028 bytes**) parse (`pipeline.Connect().PumpAll()`) is timed with
  `std::chrono::steady_clock` and printed as `[perf] 2MB JSON parse (cold): N ms` then `(warm): N ms`.
  There is **no `PARSE_TIME_MS` env mode and no `[JSON parse time]` line** in the committed harness, and the
  handler is `DefaultLogHandler logHandler(log::Level::Informative)` (main.cpp line 82), so every run log is
  7 non-blank lines / 1332-1336 bytes in Debug AND RelWithDebInfo — no log flood is involved. This exe prints no `[OK](n/n)`
  summary; judge it on exit `0` plus absence of `Test failed` / `Failure:` in the run log.
- Build/run: `cmake --build C:\RAID0\Develop\linuxQOR\cmakebuild_rel --target QORTestJSON --config RelWithDebInfo
  --parallel 4` then run `.\QORTestJSON.exe` from its own `RelWithDebInfo` dir; 21 runs over 7 builds, all exit
  `0`, logs `tmp/json_final_rel_{A,A2,E,F,D,D2,E2}_run{1,2,3}.log` (raw logs deleted from `tmp/` 22 Oct 2026; the
  values recorded below are the durable record).
- FINAL baseline, RelWithDebInfo unless stated:
  | sources | parse time |
  |---|---|
  | git HEAD (all three micro-edits reverted, via `git stash push`) | **2110 ms** mean (18 samples; per-build 1959.6 / 2182.5 / 2188.6) |
  | podbuffer `& m_wrapMask` only | 2259 ms (2264/2254/2278 cold) |
  | the two json edits only (podbuffer reverted) | 2255 ms (2242/2261/2260 cold) |
  | current working tree (all three edits) | **1879 ms** mean (12 samples; builds 1875.4 / 1881.9) |
  | same sources, earlier build (`json_perf_rel_now.log`, since deleted) | 2128.5 ms (2138.12 / 2118.92) |
  | current tree, Debug (`json_perf_debug_now.log`, since deleted) | 54.6-54.8 s: cold 54779.4 / warm 54614.1 ms |
- Noise floor: <=0.6% between runs of one binary (cold vs warm), **13.5% across independent builds of IDENTICAL
  sources** (the current tree measured 1875.4 / 1881.9 / 2128.5 ms means in three builds; git-HEAD spread 11.8%;
  MSVC layout moves the hot loop across cache lines and MSBuild recompiles different object sets - build logs
  132 vs 154 non-blank lines). Only deltas over ~14% measured across builds are evidence — and none clears that bar.

### What was disproved
- The recorded "2MB parse ~56 s -> ~18 s" improvement does not reproduce in either config: Debug with the
  current tree is 54.6-54.8 s (`json_perf_debug_now.log`, since deleted) and RelWithDebInfo is 1.88-2.26 s — no config
  ever measured ~18 s, and no 3x win exists.
- The apparent A -> D gain (-11.0%) is NOT evidence: independent builds of IDENTICAL sources spread 13.5% for D
  (1875.4 / 1881.9 / 2128.5 ms means) and 11.8% for A (1959.6 / 2182.5 / 2188.6), and single-edit attribution is
  self-contradictory (mask-only 2259 ms and mask-less 2255 ms are both ~+7% vs HEAD while the combination
  measures 1879 ms). Verdict: the three micro-edits are **performance-neutral within the noise floor**.
- The round-16 record is unreproducible and must not be cited: its fixture `test/data/big_json.txt`
  (2,102,067 bytes), its `tmp/pristine_qorjson` / `tmp/pristine_json` build tree, its `PARSE_TIME_MS` env mode
  and its `[OK](42/42)` summary line do not exist in this tree (verified by file listing and grep). The
  "log flood of ~6.7M `log::debug` lines" attribution is void — the handler is `log::Level::Informative`
  and run logs are 7 non-blank lines in both configs.

### Known issues / open items
- RESOLVED (no performance credit either way): `% m_size` -> `& m_wrapMask` in `PODBuffer::AddressOf`
  (`src/framework/data/pipeline/podbuffer.h`) is neither a win nor a measured regression here; keep it because
  it bounds every index read (the `assert` removal stays), or revert purely on review grounds.
- `Parser::FinalParse()` -> `AnyObject::Cast<Fastflow>` still does a live `dynamic_cast`
  (`typedany.h:158` `CastToBase_`) per model node while RTTI is live; the RTTI-removal edit only pays off
  once node adapters stop routing results through `AnyObject`.
- Harness note: the timed scope parses the 2 MB fixture twice (cold then warm sample) and the surrounding
  cases parse their own JSON fixtures; treat the `[perf]` lines as the only parse metric.
- DEFERRED (TODO 15, structural refactoring, parked 22 Oct 2026 until greater resources are available — nothing
  started): bulk scan in `parser/states/_2/character.cpp` (+`_4/string.cpp`, `_3/integer.cpp`,
  `_3/fraction.cpp`); devirtualize the `ParserState` hierarchy (`src/components/data/parser/{state.cpp,states/*}`,
  48 JSON state files / 17 .cpp under `src/components/data/formats/json/parser/states/_1.._4`); model-node pooling
  after removing the `AnyObject::Cast<Fastflow>` hop. Measure each with the `[perf]` timer; believe only >~14% deltas.
- Cleanup 22 Oct 2026 (two passes): pass 1 deleted 45 files / 300,687 B of superseded round-16/17 artifacts and
  scratch scripts; pass 2 deleted the 31 round-18 logs (146.5 KB — the 28 `tmp/json_final_rel_*_{build,run1,run2,run3}.log`,
  `tmp/json_perf_rel_now.log`, `tmp/json_perf_debug_now.log`, `tmp/rel_D_build.log`). **`tmp/` now holds only the 3 dirs
  `tmp/dcast` / `tmp/edited` / `tmp/jsonread`** — no JSON-perf log survives anywhere, so the `docs/agent/verification.md`
  FINAL entry's table is the sole durable record and every `tmp/...` citation (here and in the SUPERSEDED verification
  entries) is provenance, not a readable artifact.

## 26 Oct 2026 — winqor_api_shell: the shell32.dll API layer completed

- WORKS: `src/platform/os/windows/api_layer/shell` builds green as the shared library `winqor_api_shell`
  (target `winqor_api_shell`, class `qor::win::api::Shell`, QOR_WINAPISHELL). `shell.h` declares 60 static
  prototypes; `shell.cpp` 14 / `drag.cpp` 5 / `fileops.cpp` 11 / `icon.cpp` 12 / `notify.cpp` 7 /
  `recycle.cpp` 6 / `shellprops.cpp` 5 define exactly those 60 in the kernel pattern
  (`qor_pp_fcontext` + `Win32ErrorHandler _` + `qor_pp_useswinapi(shell32, Name)` + `Library::Call`).
- VERIFIED: `cmake --build cmakebuild --target winqor_api_shell --parallel 8` exit 0, 0 error lines
  (`tmp/build_shell2.log`), 16/16 objects, DLL relinked 26 Oct 2026 09:21; `dumpbin /imports` on the fresh DLL
  shows `winqor_api_kernel.dll` + `KERNEL32.dll` and NO `SHELL32.DLL` (GetProcAddress binding, as designed);
  audit `tmp/_tmp_shell_full_audit.py` reports sdk 60 / header 60 / impl 60, `header ^ impl` empty,
  duplicates empty, `impl not exported` empty against the live `shell32.dll` export table.
- LEFT TO BUILD / OPEN: the four live-but-unwrapped shellapi.h entry points
  (`SHEnumerateUnreadMailAccountsW`, `SHGetUnreadMailCountW`, `SHSetUnreadMailCountW`, `SHGetDriveMedia`) —
  the locked scope was 60 endpoints, so these stay out until review says otherwise; their ANSI variants do not
  exist in the live export table at all. Legacy Shell Object Library (`<shobjidl.h>`, ~127 names) and the
  ~290 undocumented shell32.dll internal exports remain out of scope by decision.
- KNOWN ISSUES / traps recorded: SDK prototypes span lines, so any header audit must join lines before matching
  or return types silently drop; `REFIID` bare resolves to the QOR `UUIDGMP` type in this module while `::REFIID`
  is the SDK GUID typedef; `SHTestTokenMembership` is declared `STDAPI_(BOOL)` in shellapi.h (not `SHSTDAPI_`)
  yet is a shell32.dll export; DESKTOP-partition-only entry points (`Shell_NotifyIconA`,
  `Shell_NotifyIconGetRect`, `SHAppBarMessage`) are wrapped unconditionally.
- Docs kept in sync: `src/platform/os/windows/api_layer/shell/purpose.md` (new), `docs/agent/DECISIONS.md`
  D12, `docs/agent/verification.md` top entry, `docs/agent/TODO.md` item 16 [x], `docs/agent/STATE.MD` current focus.

