# Active Context

**Last updated:** 19 Oct 2026 — refreshed at HEAD `93668d3d` ("smal fixes",
19 Oct 2026). Earlier init was built from `STATUS.md` 29/07/2026, `CHANGES.md`,
`FEATURES.md` and git log at `6b65785a`; `STATUS.md` has **not** been updated
since, so `git log` and `docs/agent/STATE.MD` are the newest sources.

## ▶ Assigned next task: JSON generator (start here)

**Assigned 19 Oct 2026, not yet started.** Add a qor_pipeline-based JSON
**generator** to `qor_jsondata` — the writing half that mirrors the JSON parser —
plus a roundtrip test in `test/components/formats/json` (`QORTestJSON`). The
step-by-step plan lives in `memory-bank/jsonGeneratorPlan.md`.

Verified prerequisites (checked against the tree on 19 Oct 2026):
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
  its compile definition — see `memory-bank/jsonGeneratorPlan.md` for details
  before writing new generator `.cpp` files.

## Current focus (STATUS.md, 29/07/2026)
- **UI design** is still in progress; things "are coalescing slowly around a
  reasonable compromise".
- The **parser** has had a lot of work and now reads most Unicode text
  encodings.
- The **JSON parser** needs upgrading to be layered over Unicode text before it
  can be used to load UI scene templates.
- Trying out **Pipes in a platform-agnostic form**: sockets on Linux, files on
  Windows (see branch `PipeConnectorExperiment`).
- Planning to post to r/cpp Show and Tell.

## July 2026 priorities
- Portable modules are now **versioned**, identifying those ready for **0.5**.
- **HTTP** is the next major element to be cleaned up and completed — needs a
  couple of systematic refactors and generator infrastructure.
- A **version 0.5 interim release plan** is assembled with a long list of things
  to do before and after.
- **Scrubbing the module export interfaces** is under way — a pretty major piece
  of work that won't be complete soon. Base libraries are now clean and clean
  examples can be built; **anything using pipeline components won't** — that's
  the next target.
- June/July quality push: builds with all the pedantry turned on are possible;
  many `/qor` libraries are clean even with everything cranked up.

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

## Next steps (queue — item 1 is the assigned task)
1. **JSON generator (ASSIGNED — do this next)**: qor_pipeline-based generator
   states + `generator/nodes/` adapters inside `qor_jsondata`, wired through
   `qor_generatorsource`, plus a `JSONWriter` test helper and roundtrip tests in
   `QORTestJSON`. Full plan: `memory-bank/jsonGeneratorPlan.md`.
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
- **Generator (JSON, assigned task)**: the writing path must mirror the reading
  path — `GeneratorSource` (pipeline **Source**, `Pull` mode) wrapping
  `Generator : Workflow` with `GeneratorState : workflow::State` states, rather
  than a plain recursive serializer. Decisions still open: whitespace policy
  (compact vs pretty), where the `UnicodeTextFilter` sits on output (encode to
  target text encoding, or emit raw ASCII/UTF-8 and bypass it), and number
  formatting (round-trip-safe `%.17g`-style vs minimal representation; a string-
  preserving model type may be needed for exact roundtrip semantics).
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
  step toward the human-readable option; the **JSON generator** (assigned task)
  supplies the write path for the JSON option.

## Learnings & project insights
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
- Scratch/transient artifacts live at the repo root (`yaml_exports.txt`) and in
  `docs/agent/_tmp_*` — do not treat them as documentation.
- Grammar-driven work is done by **generating** the states/nodes/model code from
  the ABNF/EBNF (`docs/agent/yaml_cfg_ebnf.md`), not hand-writing it; the JSON
  generator should follow the same route.
