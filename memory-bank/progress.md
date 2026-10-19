# Progress

**Last updated:** 19 Oct 2026 at HEAD `93668d3d` ("smal fixes"). `STATUS.md`
still shows 29/07/2026, so this file (plus `git log`) is the fresher record.

## ▶ Assigned next task: JSON generator

Add a qor_pipeline-based JSON **generator** to `qor_jsondata` — the writing half
mirroring the JSON parser — plus a `JSONWriter` helper and roundtrip tests in
`QORTestJSON` (`test/components/formats/json`). Detailed step-by-step plan:
`memory-bank/jsonGeneratorPlan.md`. **Not started**; the prerequisite (no
`generator/` folder under `src/components/data/formats/json/`) was verified on
19 Oct 2026.

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
  **no writing path at all**. The **JSON generator is the assigned next task**
  (top of this file + `memory-bank/jsonGeneratorPlan.md`); it must still be
  layered over Unicode before it can carry UI scene templates.
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
  test-data path made relative. **JSON generator assigned as the next task.**

## What's left before 0.5
**JSON generator (assigned first step)** · HTTP completion + generator
infrastructure · pipeline components · YAML beyond the flow subset · JSON over
Unicode · text-file and portable-binary generator frameworks · Unicode database
data structures / normalisation · UI scene-template assembly · multi-executable
test build · remaining export-interface scrubbing · structural and control-flow
framework modules · memory-management customization components · Mac support
(clang-llvm first, FileSystem next).

## Assigned next task — JSON generator (detailed plan)

See `memory-bank/jsonGeneratorPlan.md` for the full step-by-step plan to add a
qor_pipeline-based JSON generator to `qor_jsondata` and matching roundtrip test
code to `test/components/formats/json`. Its status is **assigned, not started**
(as of 19 Oct 2026); pre-flight facts verified against the tree are recorded at
the top of that plan.
