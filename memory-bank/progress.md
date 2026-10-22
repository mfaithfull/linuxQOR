# Progress

**Last updated:** 22 Oct 2026 — **qor_mime plan step 13 (the octet-stream fallback — a full window that is neither printable text nor a known signature) DONE and verified green, driven test-first** (RED
`[FAIL] #13`, `[TOTAL](14/14) [OK](13/14) [FAIL](1/14)` exit 1 → GREEN `[TOTAL](14/14) [OK](14/14)` exit 0; logs `tmp/qortestmime_step13_red.log`,
`tmp/qortestmime_step13_green.log`). **The qor_mime recognizer is now complete on its targets: ALL 10 of the 10 target MIME types recognised** (PNG, JPEG, GIF,
PDF, ZIP, XML, HTML, JSON, JavaScript, plain text) **AND the application/octet-stream fallback proven** — every stream that fills the 16-byte signature window
now ends with a type and a non-empty name. Step 12 (recognise plain text — the whole signature window printable) was likewise DONE and verified green test-first
(RED `[FAIL] #12` → GREEN `[TOTAL](13/13) [OK](13/13)`). Earlier the same day: qor_mime plan step 11 (recognise JavaScript — shebang `#!`, leading `function `, leading `//`/`/*` comment) DONE, qor_mime plan step 10 (recognise JSON — first non-JSON-whitespace
octet is `{` or `[`) was DONE, qor_mime plan step 9 (recognise HTML — the `<!DOCTYPE html`
doctype and the document-level first tag) was DONE and verified green test-first (RED `[FAIL] #9` → GREEN
`[TOTAL](10/10) [OK](10/10)`), qor_mime plan step 8 (recognise the XML `<?xml` declaration marker)
was DONE and verified green test-first, qor_mime plan step 7 (recognise the ZIP `PK 03 04` magic) was likewise
DONE, and qor_mime plan steps 0-6 were done (step 4 verified after the fact — its
RED phase never observed), and
the JSON generator increment is **COMPLETE**: the full `QORTest` regression run (the
base framework suite) is **green at `[TOTAL](678/678)` / `[OK](678/678)`, process exit code 0**, on
top of the parse -> generate -> parse **roundtrip** being green in `QORTestJSON` (process exit code
0) and the **pipeline integration** step (a JSON model written to a real file through a real
pipeline: `GeneratorSource` -> `FileConnector`) **DONE and verified at
## MIME Recognition — qor_mime (20-22 Oct 2026, steps 0-12: outline + pass-through + signature window + PNG + JPEG + GIF + PDF + ZIP + XML + HTML + JSON + JavaScript + plain text)

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
- ✅ **Plan step 1** `mime_filter_passes_octets_through`: RED first (`[FAIL] #1`, exit 1 — the base
  `Filter` does not move octets by itself, so the plan's "no code expected" assumption was wrong),
  then `MIMERecognizerFilter::TransferOctets()` (declared in `mime.h`, called from
  `ReadFilter`/`WriteFilter`, modelled on `CopyFilter::Copy`) → `QORTestMIME` `[TOTAL](2/2) [OK](2/2)`
  exit 0. Filter now provably sits between a source and a sink and leaves the octet stream untouched.
  Push mode + `Pipeline` parent is the working filter-pump path; Pull mode hits
  `Filter::Read` → `ReadFilter(unitsRead, unitsRead)` (0 units at entry) in the base framework.
- ✅ **Plan step 2** `mime_captures_leading_octets`: RED first (`[FAIL] #2`, `[OK](2/3)`, exit 1 — the
  placeholder `Inspect()` captured nothing and was not even called), then `Inspect()` in `mime.cpp`
  captures the leading octets through the pipeline buffer three-step read protocol — `ReadRequest`
  → `memcpy` into `m_signature` → **`ReadReject`** to give the reservation back, so the source is
  NOT consumed and the same octets still move on through `TransferOctets` — capped by
  `SIGNATURE_WINDOW_BYTES` (16); `ReadFilter`/`WriteFilter` call `Inspect()` before `TransferOctets`;
  `mime.h` needed no change. `QORTestMIME` `[TOTAL](3/3) [OK](3/3)` exit 0. New gotchas recorded: a
  run issued in the same command batch as the build ran the **stale previous exe** (build and run
  must be separate sequential commands), and `log::debug` in `qor_mime` produces no output in
  `QORTestMIME` (log flyer not wired there). `GetDetectedType()`/`GetTypeName()` still placeholder
  until step 3.
- ✅ **Plan step 3** `mime_recognises_png_signature`: RED first (`[FAIL] #3`, `[OK](3/4)`, exit 1 —
  pass-through assert passed, `GetDetectedType()` still `Unresolved`), then in `mime.cpp` the fixed-order
  signature table inside `Inspect()` (after each capture, `memcmp` first 8 of `m_signature` vs the PNG
  header `89 50 4E 47 0D 0A 1A 0A` → `m_detectedType = PNG`) + the complete registered-name switch in
  `GetTypeName()` (`mime.h` unchanged). Verified 20 Oct 2026: `QORTestMIME.exe` `[TOTAL](4/4) [OK](4/4)`
  exit 0 from `build\test\framework\data\mime\Debug`. Name table is data (enum→name) so it is complete
  now; only the detectors stay incremental.
- ✅ **Plan step 4** `mime_recognises_jpeg_signature`: **VERIFIED 20 Oct 2026 but NOT test-driven** — the
  test and the jpeg detector (`m_signatureCount >= 3` + `memcmp` vs `FF D8 FF` in `Inspect()`) were
  already in the working tree as uncommitted edits to `mime.cpp`/`test_mime.cpp` with no build, no run
  and no record anywhere (no step-4 commit in `git log`). Built `qor_mime` + `QORTestMIME` and ran the
  exe from `build\test\framework\data\mime\Debug` → `[TOTAL](5/5) [OK](5/5)`, no `[FAIL`, exit 0
  (`tmp/qortestmime_step4_green.log`). The RED phase was never observed — recorded as such rather than
  assumed. **Lesson: working-tree code that has never been built/run is not "done"; verify first.**
- ✅ **Plan step 5** `mime_recognises_gif_signature`: test written FIRST (two streams in the case with
  fresh elements for the second — `GIF89a` and `GIF87a` six-byte headers, pass-through +
  `GetDetectedType() == MIMEType::GIF` + `GetTypeName() == "image/gif"` per stream) and seen RED
  (`mime_recognises_gif_signature [FAIL], #5`, `[TOTAL](6/6) [OK](5/6) [FAIL](1/6)`, exit 1,
  `tmp/qortestmime_step5_red.log`), then `Inspect()` gained a third fixed-order guard
  (`m_signatureCount >= 6`, `memcmp` vs `{47 49 46 38 39 61}` OR `{47 49 46 38 37 61}` → `GIF`);
  `mime.h` unchanged. Rebuilt → `[TOTAL](6/6) [OK](6/6)`, exit 0
  (`tmp/qortestmime_step5_green.log`).
- ✅ **Plan step 6** `mime_recognises_pdf_signature`: test written FIRST (one Push-mode stream,
  `%PDF-1.4\n` — the 4-byte `%PDF` magic plus version marker — plus a short body; asserts
  `sink.GetData() == pdfData` pass-through, `GetDetectedType() == MIMEType::PDF`,
  `std::string(GetTypeName()) == "application/pdf"`) and seen RED (`mime_recognises_pdf_signature [FAIL],
  #6`, `[TOTAL](7/7) [OK](6/7) [FAIL](1/7)`, exit 1, `tmp/qortestmime_step6_red.log`), then `Inspect()`
  gained a fourth fixed-order guard (`m_detectedType == Unresolved && m_signatureCount >= 4`,
  `memcmp` vs `{0x25,0x50,0x44,0x46}` = `%PDF` → `MIMEType::PDF`); `mime.h` unchanged. Rebuilt
  `qor_mime` then `QORTestMIME` (separate commands) → `[TOTAL](7/7) [OK](7/7)`, exit 0
  (`tmp/qortestmime_step6_green.log`). No new build gotchas this increment.
- ✅ **Plan step 7** `mime_recognises_zip_signature`: test written FIRST (one Push-mode stream,
  4-byte `PK 03 04` local file header magic as raw octets (`"PK\x03\x04"` hex escapes) plus a short
  body; asserts `sink.GetData() == zipData` pass-through, `GetDetectedType() == MIMEType::ZIP`,
  `std::string(GetTypeName()) == "application/zip"`) and seen RED (`mime_recognises_zip_signature [FAIL],
  #7`, `[TOTAL](8/8) [OK](7/8) [FAIL](1/8)`, exit 1, `tmp/qortestmime_step7_red.log`), then `Inspect()`
  gained a fifth fixed-order guard (`m_detectedType == Unresolved && m_signatureCount >= 4`,
  `memcmp` vs `{0x50,0x4B,0x03,0x04}` = `PK 03 04` → `MIMEType::ZIP`); `mime.h` unchanged. Rebuilt
  `qor_mime` then `QORTestMIME` (separate commands) → `[TOTAL](8/8) [OK](8/8)`, exit 0
  (`tmp/qortestmime_step7_green.log`). No new build gotchas this increment. 5 of 10 target types
  recognised.
- ✅ **Plan step 8** `mime_recognises_xml_signature`: test written FIRST (one Push-mode stream, the five
  character XML declaration marker `<?xml version="1.0"?>\n` — the `<?xml` marker plus the rest of a
  declaration — plus a short body; asserts `sink.GetData() == xmlData` pass-through, `GetDetectedType() ==
  MIMEType::XML`, `std::string(GetTypeName()) == "application/xml"`) and seen RED (`mime_recognises_xml_signature
  [FAIL], #8`, `[TOTAL](9/9) [OK](8/9) [FAIL](1/9)`, exit 1, `tmp/qortestmime_step8_red.log`), then `Inspect()`
  gained a sixth fixed-order guard (`m_detectedType == Unresolved && m_signatureCount >= 5`, `memcmp` vs
  `{0x3C,0x3F,0x78,0x6D,0x6C}` = `<?xml` → `MIMEType::XML`); `mime.h` unchanged. Rebuilt `qor_mime` then
  `QORTestMIME` (separate commands) → `[TOTAL](9/9) [OK](9/9)`, exit 0 (`tmp/qortestmime_step8_green.log`).
  No new build gotchas this increment. **6 of 10 target types recognised.** Note: XML has no binary magic
  number, so this is the first strong text heuristic (the `<?xml` declaration marker), evaluated after the
  definite binary signatures in the fixed order.
- ✅ **Plan step 9** `mime_recognises_html_signature`: test written FIRST (TWO streams in the one case,
  fresh elements for the second — mirroring step 5's two GIF variants: the lower case HTML5 doctype
  `<!DOCTYPE html>\n<html><head>...` and the no-doctype upper case document-level tag `<HTML>\n<head>...`;
  per stream asserts `sink.GetData() == data` pass-through, `GetDetectedType() == MIMEType::HTML`,
  `std::string(GetTypeName()) == "text/html"`) and seen RED (`mime_recognises_html_signature [FAIL],
  #9`, `[TOTAL](10/10) [OK](9/10) [FAIL](1/10)`, exit 1, `tmp/qortestmime_step9_red.log`, reason
  `expected boolean to have the value of true / value was : 0`), then `Inspect()` gained a seventh
  fixed-order guard after the XML marker: a case-folded 14-octet `<!DOCTYPE HTML` compare and, while
  still unresolved, a document-level first-tag compare (`m_signature[0] == '<'` then case-folded
  `html`/`head`/`body`/`title`). HTML has no binary magic, so both are text heuristics; unlike the fixed
  magics above they cannot use `memcmp` (HTML markup spelling is case-insensitive), so `mime.cpp` gained a
  file-local `markupMatch()` ASCII case-fold compare helper — internal, no `qor_pp_module_interface`,
  no locale, no new dependency. `mime.h` unchanged. Rebuilt `qor_mime` then `QORTestMIME` (separate
  commands) → `[TOTAL](10/10) [OK](10/10)`, exit 0 (`tmp/qortestmime_step9_green.log`; build logs
  `tmp/qortestmime_step9_build_lib.log`/`tmp/qortestmime_step9_build_exe.log` show no
  `error C`/`warning C`/`LNK`). No new build gotchas. **7 of 10 target types recognised.**
- ✅ **Plan step 10** `mime_recognises_json_signature`: test written FIRST (TWO streams in the one case,
  fresh elements for the second — the shape of steps 5 and 9: a JSON object `{ "qor" : "mime test", "step" : 10 }`
  with no leading whitespace, and a JSON array preceded by JSON whitespace (three spaces, a line feed, a tab)
  so the skip-past-whitespace half of the heuristic is exercised; per stream asserts `sink.GetData() == data`
  pass-through, `GetDetectedType() == MIMEType::JSON`, `std::string(GetTypeName()) == "application/json"`) and
  seen RED (`mime_recognises_json_signature [FAIL], #10`, `[TOTAL](11/11) [OK](10/11) [FAIL](1/11)`, exit 1,
  `tmp/qortestmime_step10_red.log`, reason `expected boolean to have the value of true / value was : 0`), then
  `Inspect()` gained an eighth fixed-order guard after the HTML guards: scan the captured window for the FIRST
  octet that is not JSON whitespace (0x20 space, 0x09 tab, 0x0A line feed, 0x0D carriage return — the only four
  characters JSON allows as whitespace) and set `MIMEType::JSON` when that octet is `{` or `[`, then stop the
  scan. JSON has no magic number, so this is a text heuristic; unlike the markup compares above it is NOT case-relevant,
  so a plain octet compare is enough and `markupMatch()` is not used. It decides as soon as one non-whitespace
  octet is captured, so it does not need the window to be full. `mime.h` unchanged. Rebuilt `qor_mime`
  then `QORTestMIME` (separate commands) → `[TOTAL](11/11) [OK](11/11)`, exit 0 (`tmp/qortestmime_step10_green.log`;
  build logs `tmp/qortestmime_step10_build_lib.log`/`tmp/qortestmime_step10_build_exe.log` show no
  `error C`/`warning C`/`LNK`). No new build gotchas. **8 of 10 target types recognised.**
- (Plan step 11 JavaScript — recorded in the header above, in `docs/agent/verification.md` and in
  `src/framework/data/mime/status.md`: RED `[FAIL] #11` → GREEN `[TOTAL](12/12) [OK](12/12)`, 9 of 10.)
- ✅ **Plan step 12** `mime_recognises_plain_text_signature` (the plan's `mime_recognises_plain_text`, renamed
  to the `_signature` convention): test written FIRST (TWO Push-mode streams in the one case, fresh elements
  for the second — the shape of steps 5/9/10/11: an ASCII-only prose document longer than the 16-byte window,
  and a document carrying explicit UTF-8 octets `0xC3,0xA9` (two byte sequence) and `0xE2,0x82,0xAC` (three
  byte sequence) so the stream does not depend on the test file's encoding; per stream asserts pass-through,
  `== MIMEType::PlainText`, `"text/plain"`) and seen RED (`mime_recognises_plain_text_signature [FAIL], #12`,
  `[TOTAL](13/13) [OK](12/13) [FAIL](1/13)`, exit 1, `tmp/qortestmime_step12_red.log`), then `Inspect()` gained
  a tenth fixed-order guard after the JavaScript guards: requires `m_signatureCount >= SIGNATURE_WINDOW_BYTES`
  (the WHOLE window — the only guard that demands a full window, because a partially captured window is not
  evidence of text) and every captured octet printable — 0x80-0xFF (UTF-8 encoded text), 0x20-0x7E (printable
  ASCII) or 0x09/0x0A/0x0D (tab, line feed, carriage return); any other control octet (NUL, the other C0
  controls, DEL) leaves the stream unresolved for the step 13 fallback. UTF-8 well-formedness deliberately
  NOT validated — that is the Unicode layer's job and this framework layer module must not depend on it.
  `mime.h` unchanged. Rebuilt `qor_mime` then `QORTestMIME` (separate commands) → `[TOTAL](13/13) [OK](13/13)`,
  no `[FAIL`, exit 0 (`tmp/qortestmime_step12_green.log`). **10 of 10 target types recognised** — the plan's
  "10 of 10 target types recognised in steps 3-12" stop condition is met.


- ⚠️ **NEW build gotcha (20 Oct 2026): `cmake --build` calls must be sequential, one per command.**
  Two `cmake --build` invocations issued in the same command batch run concurrently over one build
  tree and race on the shared `.pdb` — `error C1090: PDB API call failed` on an unrelated dependency
  and `error C1041: cannot open program database '…vc145.pdb'; if multiple CL.EXE write to the same
  .PDB file, please use /FS`. Both were transient (a retry alone rebuilt clean), but they are real
  and were hit twice in this increment.
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
- ✅ Detection is now COMPLETE: all 10 target types recognised (PNG, JPEG, GIF, PDF, ZIP, XML, HTML,
  JSON, JavaScript, plain text) AND the `application/octet-stream` fallback proven (plan step 13, 22 Oct
  2026) — every stream that fills the 16-byte signature window ends with a type and a non-empty name.
- ⏳ Pipeline behaviour still unproven (plan steps 14-17): a signature split across pumped chunks,
  `Reset()` reuse, chaining with `qor_base64encodefilter`, and the full `QORTest` regression run.
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
