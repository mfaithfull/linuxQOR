# Agent State

## Current task
Current focus 26 Oct 2026 — **TODO 16 winqor_api_shell COMPLETED**: the whole modern Shell API of shell32.dll
(60 documented `shellapi.h` entry points) is wrapped as stateless `qor::win::api::Shell` functions across
`shell.h` + seven .cpp files; build green, audit 60/60/60 with zero missing and zero non-exported names, no
SHELL32.DLL import by design (GetProcAddress binding). Open surface decision left for review: the four live-but
out-of-scope entry points `SHEnumerateUnreadMailAccountsW`, `SHGetUnreadMailCountW`, `SHSetUnreadMailCountW`,
`SHGetDriveMedia` (DECISIONS.md D12, src/platform/os/windows/api_layer/shell/purpose.md).


#15 — qor_jsondata STRUCTURAL parse-performance refactoring — **DEFERRED 22 Oct 2026 until greater resources are
available**; no structural work started, the inventory below is kept as the parked plan. #14 (parse-perf
verification FINALIZATION, round 18) is **COMPLETED** 22 Oct 2026 — reproducible record in docs/agent/verification.md
top entry, verdict NEUTRAL (all three micro-edits inside the 13.5% same-source build spread); the user decision is
recorded: **all three micro-edits are KEPT** (committed 22 Oct 2026 as `dea24af2 "JSON parser performance review
concluded"` - 24 `src/` paths +105/-79 plus the harness +233/-192, so HEAD now equals variant D); TODO 14 marked [x];
`tmp/` pruned to the round-18 evidence.
(Prior task #13 — qor_mime: MIME recognition pipeline filter (plan steps 0-17 DONE 20-22 Oct 2026: outline + pass-through +
signature window + PNG/JPEG/GIF/PDF/ZIP/XML/HTML/JSON/JavaScript/plain text detection + the application/octet-stream
fallback + recognition of a signature split across pumped chunks + recognizer reuse across `Reset()` + chaining with `qor_base64encodefilter`; **ALL 10 of the 10 target types recognised, the
recognizer total — a stream pumped in full always ends with a type and a non-empty name — and it co-operates with an encoding filter behind it**) → new framework-layer module
`src/framework/data/mime` (lib `qor_mime`, class `qor::mime::MIMERecognizerFilter :
qor::pipeline::Filter`) + test target `test/framework/data/mime` (exe `QORTestMIME`, 10 popular MIME types +
application/octet-stream fallback). Test-first plan: `memory-bank/qor_mime_plan.md`. Next action: optional plan step
17 `mime_type_name_drives_a_sink` (application-level glue — the recognised type name driving a sink). Plan step 16
`mime_recogniser_chained_with_qor_base64encodefilter` is DONE and verified test-first — `StringSource -> MIMERecognizerFilter -> Base64EncodeFilter -> StringSink`;
the encode filter is an `InlineFilter<byte>` (a Buffer pair, not an Element) inserted as the buffer shared by the elements
either side of it, the recognizer still classifies the PRE-encoded stream (`image/png`) and the sink text is exactly the
base64 of the stream; it went RED (the sink got only the first 18 of the 24 encoded characters) and was made GREEN by
separating the consumed from the produced unit count in `mime.cpp::TransferOctets`. Plan step 15
`mime_recognizer_resets_between_streams` was likewise DONE and verified — ONE recognizer serving TWO streams
(PNG of step 3, `Reset()`, PDF of step 6 through fresh elements), GREEN on its FIRST run with NO implementation change.
Steps 0-16 of the plan are done and verified (`QORTestMIME` `[TOTAL](17/17) [OK](17/17)`
exit 0; step 4 verified after the fact, its RED phase never observed; steps 14 and 15 went GREEN on their FIRST runs with NO
implementation change — the window already accumulates across pumped chunks and `Reset()` already clears the recognizer state). (TODO.md item 13.)

## Completed work
- [x] qor_jsondata parse-perf verification FINALIZED 22 Oct 2026 (TODO 14, round 18): exact harness/fixture identity
      (`test/components/formats/json/main.cpp` `[perf]` lines 284-305, fixture `test/data/data_2mb.json` 2,219,028 bytes),
      21 RelWithDebInfo runs over 7 builds in `cmakebuild_rel` + 1 Debug run, all exit 0, logs `tmp/json_final_rel_*.log`
      (raw logs deleted 22 Oct 2026 - the values below are the recorded readings);
      verdict NEUTRAL — no micro-edit earns credit (same-source build spread 13.5%); round-16 record marked unreproducible
      (its fixture `big_json.txt`, `tmp/pristine_qorjson` tree and `PARSE_TIME_MS` mode are absent).
      **COMPLETED**: user decision recorded — all three micro-edits KEPT (review grounds, no perf credit); TODO 15
      DEFERRED until greater resources are available; `tmp/` pruned 22 Oct 2026 (45 files / 300,687 B deleted,
      34 entries / 146.5 KB remain - only the round-18 evidence, see docs/agent/verification.md cleanup bullet)
- [x] Read docs/structure.md, docs/tutorial.md
- [x] Read root + sub CMakeLists.txt (4 levels)
- [x] Full scan: all set(target qor_*) + target_link_libraries across src
"- [x] Wrote docs/agent/repo_map.md (174 lines; 60 QOR libs + deps + layering)
"- [x] Scanned src/qor dirs; resolved D6 (header-only rule: no CMakeLists.txt = header-only)
- [x] Resolved D5 (user: protocols/http_old NOT built, ignore)
- [x] Generated docs/agent/ctags/qor_module.tags "(hand-emitted ctags format; 26 tag lines)
- [x] Re-checked ctags: Universal Ctags 6.1.0 IS available (ctags.exe via WinGet) -> D7 updated; will ctags every lib
"- [x] Generated docs/agent/ctags/qor_host.tags (Universal Ctags 6.1.0; framework/app/host, 11 symbols)
- [x] Verified build: cmake configure (MSVC 19.50, VS2026, Windows) + cmake --build --target qor_module -> PASS (qor_module.lib produced, no errors/warnings)"
- [x] Wrote docs/agent/purposes.md — one-line purpose summary per QOR lib, all ~60 libs, derived from consolidated src/**/purpose.md (docs/agent/_tmp_purpose_all_out.md) + repo_map.md; header-only libs noted (D6)
"- [x] Wrote docs/agent/deps.dot — DOT/digraph of every QOR lib dependency (edge A->B = A target_links_libraries against B); clustered by subsystem; http_old excluded per D5; non-QOR platform adapters noted as leaves
- [x] Wrote docs/agent/parsers.md — skill file for creating a QOR parser module, learned by analysis of qor_jsondata (src/components/data/formats/json): two-layer design (general qor::data::parser framework + per-format qor_Xdata lib), ABNF->combinator table (Sequence/Optional/ZeroOrMore/OneOrMore/Specific), ParserState Prepare/Emit/Fail + node stack, tokens.h enum + nodes NodeAdapter + model/ types, BufferContext input feeding, build/test checklist"

## Open questions
- platform adapter libs referenced: winqor_*, linqor_* -- are these built in this tree or external?
- src/qor has no "top-level" dir; all libs live under essentials/flyers/function/memory/module/property.

## Decisions
- (D1) Library unit of work = one set(target qor_X) CMakeLists.txt.
- (D2) Symbol index granularity = per-QOR-lib ctags (per repo map 60 libs + adapters).
- (D3) Verification = compiler-as-judge; incremental rebuild per lib; record in verification.md.

## Task state
- qor_jsondata parse-perf verification (TODO 14): **COMPLETED** 22 Oct 2026 (round 18) — reproducible record + NEUTRAL
  verdict; logs tmp/json_final_rel_*.log (21 runs) and tmp/json_perf_debug_now.log (Debug) - raw logs deleted 22 Oct 2026;
  D9 rewritten; TODO 14 marked [x]; all three micro-edits KEPT by user decision; tmp/ emptied of JSON-perf logs (3 dirs left)
- qor_jsondata structural parse-perf refactoring (TODO 15): **DEFERRED** 22 Oct 2026 until greater resources are available
  — inventory parked, nothing started
- repo_map.md: DONE
- ctags/qor_module.tags: DONE (hand-emitted)
"- ctags/qor_host.tags: DONE (ctags 6.1.0)
"- ctags/qor_{current,injection,sync,thread}.tags: DONE (ctags 6.1.0)
- ctags/qor_*.tags for all 45 leaf libs (src/components): DONE (ctags 6.1.0; http_old skipped per D5)
"- verification.md: DONE (template written; enhanced existing file with D3/D5/D6 notes + per-lib entry template)
"- build verify qor_module: DONE (PASS — MSVC 19.50.35721.0, VS2026, Windows; qor_module.lib produced; logged in verification.md)
- add_module skill file docs/agent/add_module.md: DONE (qor_yaml experiment; facts to add future modules)
- qor_yaml experiment: DONE (src/components/protocols/yaml CMakeLists.txt + qor_yaml.cpp + yaml.cpp -> qor_yaml.dll; registered add_subdirectory(yaml) in src/components/protocols/CMakeLists.txt)
- QORTestYAML test: DONE (test/components/protocols/yaml CMakeLists.txt + test_main.cpp + test_yaml.cpp; added add_subdirectory(components/protocols/yaml) in test/CMakeLists.txt; QORTestYAML.exe builds, links qor_yaml.lib, test yaml_module_loads OK(1/1) exit=0)""
- purposes.md: DONE (one-line purpose summary per QOR lib, ~60 libs + subsystem context)
"- deps.dot: DONE (DOT/digraph; edges A->B = A target_links_libraries against B; clustered by subsystem; http_old excluded per D5)
- parsers.md skill file: DONE (docs/agent/parsers.md; learned from qor_jsondata analysis; ABNF->combinator mapping, module skeleton, production-class patterns, build/test checklist)"
- JSON generator increment (docs/archive/jsonGeneratorPlan.md — retired 20 Oct 2026, task COMPLETE): DONE — states OK(36/36) in QORTestJSONGenerator, roundtrip green in QORTestJSON (exit 0), and full QORTest regression run green at OK(678/678) (process exit 0)
- task 12 YAML parser: IN PROGRESS — resume point in 'Task 12 (YAML parser) — detailed progress' below (string.h/.cpp + CMakeLists add_library landed this session; remaining productions + link deps + CharacterNode + tests still pending)


## Task 12 (YAML parser) — detailed progress

### Done
- Model headers: model/valuemodel.h, model/value.h, model/string.h, model/sequence.h, model/pair.h, model/mapping.h, model/character.h, model/_true.h, model/_false.h, model/_null.h
- Parser headers: parser/tokens.h (yamlToken enum starting at eToken::Max+1), parser/helpers.h (GetTokenName, StandardPrepare, StandardFail, TrivialEmit, SimpleEmit — mirrors json/helpers.h)
- Node adapter headers: parser/nodes/value.h, string.h, sequence.h, mapping.h, pair.h, _true.h, _false.h, _null.h (each NodeAdapter<model::yaml::X> tagged with yamlToken::...)
- State headers (flow subset):
  - _4/: ws.h, unescaped.h, quotation_mark.h, value_separator.h, character.h (AnyOneOf_t<unescaped,unescaped> — escapes deferred per D8)
  - _1/: begin_sequence.h, begin_mapping.h, _true.h/_true.cpp, _false.h/_false.cpp, _null.h/_null.cpp
  - _2/: end_sequence.h, end_mapping.h
  - _3/: name_separator.h
- Bug fix: parser/nodes/_true.h was mangled with literal \n escapes; corrected via single_find_and_replace.

### Read for reference
- Framework: parser.h/.cpp, state.h, node.h, result.h, context.h, tokens.h, sequence.h, optional.h, zeroormore.h, specific.h, anyoneof.h, anyoneofset.h/.cpp, oneormore.h, oneofarange.h, deferred.h, rfc5234.h, podbuffer.h, buffercontext.h, parsersink.h, fastflow.h
- JSON model/states: value.h, valuemodel.h, array.h, object.h, member.h, string.h, _true.h, _false.h, _null.h; array.cpp, string.cpp/.h, ws.h, value.cpp/.h, member.cpp, _false.cpp, _true.h, begin_array.h, end_array.h, value_separator.h, name_separator.h, quotation_mark.h, unescaped.h, character.h/.cpp
- Grep confirmed UTF8CodePage lives at src/qor/essentials/text/codepage/codepages/utf8.h (module QOR_TEXT; needs qor_text link dep).
- AnyOneOf_t requires TWO alternatives; no single-arg form exists (character.h uses unescaped twice as stub).

### Still to do
1. Create state productions (headers + .cpp under src/components/protocols/yaml/parser/states/):
   - _4/string.h + .cpp (Sequence_t<quotation_mark, Sequence_t<ZeroOrMore_t<character>, quotation_mark>>; Prepare pushes StringNode; Emit collects chars via UTF8CodePage.Encode → SetValue; Fail pops)
   - _4/value.h + .cpp (AnyOneOf nested chain over _false/_null/_true/number/sequence/string + deferred<mapping>/deferred<sequence>; Prepare pushes ValueNode; Emit copies child ValueModel into Value; Fail pops)
   - _1/sequence.h + .cpp (Sequence(begin_sequence, Sequence(Optional(Sequence(value, ZeroOrMore(Sequence(value_separator,value)))), end_sequence)); Prepare pushes SequenceNode; Emit collects value nodes into m_values)
   - _3/mapping.h + .cpp (analogous with pair; Prepare pushes MappingNode; Emit collects pair nodes into m_pairs)
   - _3/pair.h + .cpp (Sequence_t<string, Sequence_t<name_separator, value>>; Prepare pushes PairNode; Emit sets key from string + value)
   - _1/document.h + .cpp (root = value)
   - parser/yamlparser.h umbrella include
   - _3/number.h + .cpp if scalars include numeric YAML (or defer per D8)
2. Update src/components/protocols/yaml/CMakeLists.txt: add all new .cpp sources to add_library; add link deps qor_pipeline qor_workflow qor_issue qor_error qor_log qor_text to target_link_libraries (current: qor_module qor_host qor_current qor_sync qor_injection qor_parser).
3. Build: cmake --build build --target qor_yaml; record PASS in docs/agent/verification.md (per D3).
4. Tests: replace placeholder test_yaml.cpp with real cases feeding YAML strings ([1, 2, 3], {a: 1, b: 2}, "hello", true, null) via PODBuffer<uint32_t>+CodePointContext+Parser::SetContext/SetInitialStep/Parse()/FinalParse()/PopNode, asserting model types/values; rebuild QORTestYAML.exe and run.
5. Update docs/agent/STATE.MD and docs/agent/TODO.md (mark item 12 [x] when verified).

### Reference skill
docs/agent/parsers.md (ABNF→combinator table, module skeleton, production-class patterns, build/test checklist).

## Task state — #13 qor_mime (20 Oct 2026)
- Step R research (how QOR pipeline filters are created + tested): DONE — findings in
  `memory-bank/qor_mime_plan.md` (filter.h/filter.cpp/base64encodefilter/linereader/QORTestYAML patterns).
- Module scaffold `src/framework/data/mime`: DONE — `CMakeLists.txt` (`qor_mime` SHARED, `QOR_MIME`),
  `qor_mime.cpp` (singleton), `mime.h` (`qor::mime::MIMERecognizerFilter` outline API + `MIMEType` enum:
  10 targets + OctetStream fallback + Unresolved), `mime.cpp` (`init_mime` export + placeholder `Inspect()`),
  `purpose.md`, `status.md`. Registered `add_subdirectory(mime)` in `src/framework/data/CMakeLists.txt`.
- Test target `test/framework/data/mime`: DONE — `QORTestMIME` exe (`test_main.cpp` + placeholder `test_mime.cpp`
  case `mime_module_loads`), registered `add_subdirectory(framework/data/mime)` in `test/CMakeLists.txt`.
- Verify #13 outline: DONE — `qor_mime.dll` + import lib built clean, `QORTestMIME.exe` `[OK](1/1)` exit 0
  (logged `docs/agent/verification.md`).
- Plan step 1 (`mime_filter_passes_octets_through`): DONE 20 Oct 2026 — test added to `test_mime.cpp` FIRST
  and seen RED (`[FAIL] #1`, exit 1, "value was : 0" — base `Filter` does not move octets by itself), then
  `mime.h`/`mime.cpp` gained `TransferOctets()` (buffer→buffer move like `CopyFilter::Copy`, called from
  `ReadFilter`/`WriteFilter`); rebuilt qor_mime + QORTestMIME → `[TOTAL](2/2) [OK](2/2)` exit 0
  (logged `docs/agent/verification.md`, run from `build\test\framework\data\mime\Debug`).
  Key plumbing finding: a filter pumps in Push mode under a `Pipeline` parent (Element::SetFlowMode only
  forwards to a parent); Pull mode hits `Filter::Read` → `ReadFilter(unitsRead, unitsRead)` (0 units at entry).
- Plan step 2 (`mime_captures_leading_octets`): DONE 20 Oct 2026 — test written FIRST and seen RED
  (`[FAIL] #2`, `[OK](2/3)`, exit 1), then `Inspect()` in `mime.cpp` captures the leading octets via the
  buffer three-step read protocol — `ReadRequest` → `memcpy` into `m_signature` → **`ReadReject`** (gives
  the reservation back, source NOT consumed; `buffer.cpp` `ReadReject` restores `m_readEnd`) — capped by
  the 16-byte `SIGNATURE_WINDOW_BYTES` and `ReadCapacity`; `ReadFilter`/`WriteFilter` call `Inspect()`
  BEFORE `TransferOctets()`; `mime.h` needed no change. Rebuilt → `QORTestMIME` `[TOTAL](3/3) [OK](3/3)`
  exit 0 (logged `docs/agent/verification.md`). NOTE: a run issued in the same command batch as the build
  used the stale previous exe — build and run must be separate sequential commands; and `log::debug`
  output is not visible in `QORTestMIME` (log flyer not wired there), so it is not a usable probe.
- Plan step 3 (`mime_recognises_png_signature`): DONE 20 Oct 2026 — test written FIRST and seen RED
  (`[FAIL] #3`, `[OK](3/4)`, exit 1), then a fixed-order guard in `Inspect()` (`m_detectedType ==
  Unresolved && m_signatureCount >= 8`, `memcmp` the window vs `89 50 4E 47 0D 0A 1A 0A` → `PNG`)
  plus the complete registered-name switch in `GetTypeName()`; `mime.h` unchanged →
  `QORTestMIME` `[TOTAL](4/4) [OK](4/4)` exit 0.
- Plan step 4 (`mime_recognises_jpeg_signature`): **VERIFIED DONE 20 Oct 2026, NOT test-driven** — the
  test and the jpeg guard (`m_signatureCount >= 3`, `memcmp` vs `FF D8 FF`) were already in the working
  tree as uncommitted edits to `mime.cpp`/`test_mime.cpp` with no build, no run and no record (no
  step-4 commit in `git log`). Verified after the fact: built `qor_mime` + `QORTestMIME`, ran the exe
  from `build\test\framework\data\mime\Debug` → `[TOTAL](5/5) [OK](5/5)` exit 0. The RED phase was
  never observed — recorded honestly, not assumed. LESSON: unverified working-tree code is not done.
- Plan step 5 (`mime_recognises_gif_signature`): DONE 20 Oct 2026 test-first — test written FIRST
  (two streams in the case, fresh elements for the second, `GIF89a` and `GIF87a` six-byte headers,
  pass-through + `== GIF` + `"image/gif"` per stream) and seen RED (`[FAIL] #5`, `[TOTAL](6/6)
  [OK](5/6) [FAIL](1/6)`, exit 1), then a third `Inspect()` guard (`m_signatureCount >= 6`,
  `memcmp` vs GIF89a OR GIF87a headers) → `[TOTAL](6/6) [OK](6/6)` exit 0; `mime.h` unchanged.
- Plan step 6 (`mime_recognises_pdf_signature`): DONE 20 Oct 2026 test-first — test written FIRST
  (one stream, `%PDF-1.4\n` four-byte magic + version marker + short body; asserts pass-through,
  `== PDF`, `"application/pdf"`) and seen RED (`[FAIL] #6`, `[TOTAL](7/7) [OK](6/7) [FAIL](1/7)`,
  exit 1), then a fourth `Inspect()` guard (`m_signatureCount >= 4`, `memcmp` vs `{0x25,0x50,0x44,0x46}`
  = `%PDF` → `MIMEType::PDF`) → `[TOTAL](7/7) [OK](7/7)` exit 0; `mime.h` unchanged. No new gotchas.
- NEW build gotcha: two `cmake --build` invocations issued in the same command batch run concurrently
  over one build tree and race on the shared `.pdb` — `error C1090: PDB API call failed` (unrelated
  dependency) and `error C1041: cannot open program database '…vc145.pdb'; if multiple CL.EXE write to
  the same .PDB file, please use /FS`. Transient but real; exactly one cmake build per command.
- Plan step 7 (`mime_recognises_zip_signature`): DONE 20 Oct 2026 test-first — test written FIRST
  (one stream, raw `PK 03 04` magic `"PK\x03\x04"` + body) and seen RED (`[FAIL] #7`, `[TOTAL](8/8)
  [OK](7/8) [FAIL](1/8)`, exit 1), then a fifth `Inspect()` guard (`m_detectedType == Unresolved &&
  m_signatureCount >= 4`, `memcmp` vs `{0x50,0x4B,0x03,0x04}` = `PK 03 04` → `MIMEType::ZIP`) →
  `[TOTAL](8/8) [OK](8/8)` exit 0; `mime.h` unchanged. No new gotchas.
- Plan steps 8-14: DONE 20-22 Oct 2026 test-first (XML, HTML, JSON, JavaScript, plain text, the octet-stream
  fallback, the signature split across pumped chunks) — details in `src/framework/data/mime/status.md` and
  `docs/agent/verification.md`; all 10 target types recognised and the recognizer made total.
- Plan step 15 (`mime_recognizer_resets_between_streams`): DONE 22 Oct 2026 test-first — ONE `MIMERecognizerFilter`
  instance serving TWO streams (the PNG stream of step 3, then `Reset()`, then the PDF stream of step 6 through
  FRESH elements re-linked with `Filter::SetSource`/`Filter::SetSink`); asserts the cleared state after `Reset()`
  (`GetDetectedType() == MIMEType::Unresolved`, `GetTypeName() == ""`, `GetSignatureByteCount() == 0`) and that
  stream 2 recognises `== MIMEType::PDF` / `"application/pdf"` with pass-through and a 16-octet window. GREEN on
  the FIRST run `[TOTAL](16/16) [OK](16/16)` exit 0 — NO implementation change needed (`Reset()` already zeroes
  `m_signatureCount` and restores `m_detectedType`); `mime.h`/`mime.cpp` untouched, only `test_mime.cpp` recompiled
  (logs `tmp/qortestmime_step15_build_exe.log`, `tmp/qortestmime_step15_green.log`). LESSON: a filter is reused by
  re-wiring the FILTER — `Element::SetSink`/`SetSource` return false on an element that already has a connection.
- Plan step 16 (`mime_recogniser_chained_with_qor_base64encodefilter`): DONE 22 Oct 2026 test-first — TWO pipeline
  filters in series `StringSource -> MIMERecognizerFilter -> Base64EncodeFilter -> StringSink`. Anatomy FIRST: the
  encode filter is an `InlineFilter<byte>` — a Buffer pair, not an Element — so the framework inserts it as the buffer
  shared by the elements either side of it (`Pipeline::InsertInlineFilter`, BeforeSink position); here it is the SINK's
  buffer, built with an explicit capacity (`Base64EncodeFilter encodeFilter(128)`) because its default ctor holds zero
  items. Stream: the 8-byte PNG magic as explicit octets + a 10-byte payload = 18 octets (a multiple of 3, so no base64
  padding), pumped for the 24 ENCODED units; asserts the sink text is exactly `"iVBORw0KGgpJSERSIHFvcjAx"` and that the
  recognizer still reports `MIMEType::PNG` / `"image/png"` with a 16-octet window. Seen RED FIRST (`[FAIL] #16`,
  `[OK](16/17)`, exit 1 — the sink got only the first 18 of the 24 encoded characters), then fixed in
  `mime.cpp::TransferOctets`: the consumed count (`unitsConsumed`, drives the memcpy and the source `ReadAcknowledge`)
  must be kept apart from the produced count (`unitsToTransfer`, rewritten by the sink's `InlineFilter::WriteAcknowledge`
  and returned in `unitsTransferred`) — sharing one variable let `ReadAcknowledge` clamp the produced count back to the
  raw one. `mime.h` contract note extended; `test/framework/data/mime/CMakeLists.txt` gained `qor_base64encodefilter`.
  Rebuilt `qor_mime` then `QORTestMIME` (separate commands) → `[TOTAL](17/17) [OK](17/17)` exit 0 (logs
  `tmp/qor_mime_step16_dll.log`, `tmp/qortestmime_step16_exe.log`, `tmp/qortestmime_step16_green_run.log`). LESSON: a
  recognising filter inspects the stream BEFORE any downstream transform, so it classifies the PRE-encoded octets and is
  valid in front of an encoding filter; a pass-through filter must propagate the count its sink buffer PRODUCED, not the
  count it CONSUMED.
- NOT done: the optional `mime_type_name_drives_a_sink` scope, the full `QORTest` regression run over the changed
  `mime.cpp`, and consumers (HTTP Content-Type TODO, UI renderer loader, `Resource::Type()`).
- Note: TODO.md/STATE.MD on disk do not contain the earlier `qor_workflow` pipeline-integration item that
  STATE.MD's "Current task" previously referenced; the queue files appear trimmed. Item numbers on disk now
  run 1..13 with 13 = this task (item 14 = this JSON parse-performance task, added 22 Oct 2026). Not invented — just recorded so the numbering is not read as a gap.

## Task state — qor_jsondata parse-performance verification (FINAL round 18, 22 Oct 2026 — COMPLETED, TODO 14 [x])
- Harness identity verified on disk: fixture `test/data/data_2mb.json` = **2,219,028 bytes**; the timer is the
  `[perf] 2MB JSON parse (cold|warm): N ms` pair printed by `test/components/formats/json/main.cpp` lines 284-305.
  There is NO `PARSE_TIME_MS` env mode and no `[JSON parse time]` line in that file (grep: no match), and the
  handler is `log::Level::Informative` (main.cpp line 82), so run logs are 7 non-blank lines / 1332-1336 bytes in Debug and
  RelWithDebInfo — no log flood. This exe prints no `[OK](n/n)` summary; judge it on exit `0` + no
  `Test failed`/`Failure:` text.
- FINAL measurement (reproducible, 21 runs over 7 builds in `cmakebuild_rel`, logs
  `tmp/json_final_rel_{A,A2,E,F,D,D2,E2}_run{1,2,3}.log` (raw logs deleted 22 Oct 2026), all exit `0`): git-HEAD sources **2110 ms mean**
  (per-build means 1959.6/2182.5/2188.6 -> **11.8% spread for identical sources**), current tree (all three edits)
  **1879 ms mean** (builds 1875.4/1881.9; a third build of the same sources, `json_perf_rel_now.log` since deleted,
  measured 2128.5 ms mean -> **13.5% spread**), mask-only 2259 ms, mask-less 2255 ms; Debug current tree 54.6-54.8 s
  (`json_perf_debug_now.log`, since deleted). Verdict: **performance-neutral within the noise floor** — the -11.0% A->D delta
  is smaller than the same-source build spread, so no micro-edit earns credit.
- The round-16 record (2,102,067-byte `big_json.txt`, `tmp/pristine_qorjson` tree, `PARSE_TIME_MS` mode,
  1.93 s / 2.14 s / 53.2 s / 55.1 s, ~6.7M `log::debug` lines, `[OK](42/42)`) is UNREPRODUCIBLE — those artifacts
  are absent from this tree; it is kept in `docs/agent/verification.md` only as marked history. The "2MB ~56 s ->
  ~18 s" claim does not exist in either config.
- Working-tree state: all three micro-edits remain applied (variant D) and are now **committed at HEAD** (`dea24af2`)
  — `helpers.h`/`state.cpp` `#ifndef NDEBUG`
  guards, `ParserState::Workflow()`/`Specific::Enter` RTTI removal, `src/framework/data/pipeline/podbuffer.h`
  power-of-2 masking. RESOLVED: keep the mask (bounds every index read, `assert` removal stays) — kept for review,
  not for performance. **User decision 22 Oct 2026: all three micro-edits are KEPT** — the task is closed with the
  edits in place and zero performance credit claimed.
- Cleanup 22 Oct 2026 (two passes): pass 1 deleted 45 files / 300,687 B of superseded round-16/17 artifacts and scratch
  scripts (`tmp/dbg_*`, `tmp/qortestjson_*`, `tmp/rel_{B,C,pre,post,pre2,config,fixed,instr,json,pristine,full}_*`,
  `tmp/rel_D_run_{1,2,3}.txt`, `tmp/json_perf.patch`, `tmp/_*.js` / `tmp/_*.txt`); pass 2 deleted the 31 round-18 logs
  (146.5 KB — the 28 `tmp/json_final_rel_*_{build,run1,run2,run3}.log`, `tmp/json_perf_rel_now.log`,
  `tmp/json_perf_debug_now.log`, `tmp/rel_D_build.log`). **`tmp/` now holds only the 3 dirs `tmp/dcast`,
  `tmp/edited`, `tmp/jsonread`** — no JSON-perf log survives anywhere, so the `docs/agent/verification.md` FINAL entry's
  table is the sole durable record and every `tmp/...` name in it (and in the SUPERSEDED entries) is provenance only.
- DEFERRED (TODO 15, structural refactoring) 22 Oct 2026 — parked until greater resources are available, nothing
  started; when resumed, measure each with the same `[perf]` timer and believe only deltas over ~14%: (1) bulk scanning
  of string/number runs in `parser/states/_2/character.cpp` (+`_4/string.cpp`,
  `_3/integer.cpp`, `_3/fraction.cpp`); (2) devirtualized/table-driven machine for the `ParserState` hierarchy
  (`src/components/data/parser/{state.cpp,states/*}`, 48 JSON state files / 17 .cpp under
  `src/components/data/formats/json/parser/states/_1.._4`); (3) model-node pooling after removing the
  `Parser::FinalParse()` -> `AnyObject::Cast<Fastflow>` hop (`typedany.h:158` `CastToBase_` live `dynamic_cast`).

