# TODO (queue)

## Now
1. [x] Build repo map
2. [x] Generate ctags for qor_module (static core, smallest) -> docs/agent/ctags/qor_module.tags
3. [x] Generate ctags for qor_host -> docs/agent/ctags/qor_host.tags (Universal Ctags, 11 symbols)
4. [x] Generate ctags for qor_current qor_injection qor_sync qor_thread -> docs/agent/ctags/qor_{current,injection,sync,thread}.tags (Universal Ctags; symtags 92/51/267/121)
5. [x] Generate ctags for leaf libs (highest churn) -> 45 libs indexed under src/components (http_old skipped per D5); largest: qor_renderer 3682, qor_tui 1937, qor_http 1170
6. [x] Write docs/agent/verification.md template (build/test/lint commands) — enhanced existing file with D3/D5/D6 notes + per-lib entry template
7. [x] Verify build works: cmake -S . -B build; cmake --build build --target qor_module (PASS — MSVC/Windows, qor_module.lib produced)
8. [x] Purpose summaries per lib (from purpose.md + headers) — consolidated from src/**/purpose.md into docs/agent/_tmp_purpose_all_out.md; summaries captured in STATE.MD
9. [x] Dependency graph as DOT -> docs/agent/deps.dot (digraph; edges A->B = A target_links_libraries against B; clustered by subsystem)
10. [x] Learn how to add a new module + write skill file docs/agent/add_module.md (qor_yaml experiment: src/components/protocols/yaml -> qor_yaml.dll + test/components/protocols/yaml -> QORTestYAML.exe, test OK(1/1) exit=0)
11. [x] Learn how to create a QOR parser + write skill file docs/agent/parsers.md Learn by analysis of the qor_jsondata module in C:\RAID0\Develop\linuxQOR\src\components\data\formats\json
12. [x] Continue the JSON generator increment: pipeline integration (`GeneratorSource` buffer/context fix + `JSONWriter` test helper) and roundtrip tests in QORTestJSON; DONE and verified 20 Oct 2026 — QORTestJSONGenerator `[OK](36/36)`, QORTestJSON roundtrip exit 0, and the full QORTest regression run GREEN at `[OK](678/678)` (process exit 0); context + ordered plan in docs/archive/jsonGeneratorPlan.md, state in src/components/data/formats/json/generator/status.md
13. [~] **qor_mime — MIME recognition pipeline filter** (steps 0-16 DONE 20-22 Oct 2026: outline + pass-through + signature window + png/jpeg/gif/pdf/zip/xml/html/json/javascript/plain text detection; ALL 10 of 10 types recognised + octet-stream fallback + recognition across pumped chunk boundaries + recognizer reuse across `Reset()` + chaining with `qor_base64encodefilter`) →
    new module `src/framework/data/mime` (lib `qor_mime`, define `QOR_MIME`, namespace `qor::mime`,
    class `qor::mime::MIMERecognizerFilter : qor::pipeline::Filter`) + test target `test/framework/data/mime`
    (exe `QORTestMIME`). Purpose: a pipeline filter that recognises **10 popular MIME
    types** (text/plain, text/html, application/json, application/xml, application/javascript, image/png,
    image/jpeg, image/gif, application/pdf, application/zip) with application/octet-stream fallback, without
    disturbing the octet stream. Test-first plan: `memory-bank/qor_mime_plan.md` (step R research → 17 increments).
    Done: research, scaffold (singleton + `MIMEType` enum + API outline + placeholder `Inspect()`),
    `add_subdirectory(mime)` in `src/framework/data/CMakeLists.txt`, `add_subdirectory(framework/data/mime)`
    in `test/CMakeLists.txt`, `qor_mime.dll` + `QORTestMIME.exe` built, run green `[OK](1/1)` exit 0.
    Step 1 DONE 20 Oct 2026: test `mime_filter_passes_octets_through` written FIRST (red `[FAIL] #1` exit 1),
    then `TransferOctets()` (declared in `mime.h`, called from `ReadFilter`/`WriteFilter`) → `[OK](2/2)` exit 0.
    Step 2 DONE 20 Oct 2026: test `mime_captures_leading_octets` written FIRST (red `[FAIL] #2` exit 1),
    then `Inspect()` capture (`ReadRequest` → `memcpy` → `ReadReject` = source not consumed, 16-byte window) →
    `[TOTAL](3/3) [OK](3/3)` exit 0.
    Step 3 DONE 20 Oct 2026: test `mime_recognises_png_signature` written FIRST (red `[FAIL] #3`, `[OK](3/4)`
    exit 1), then PNG magic guard (`m_signatureCount >= 8`, `memcmp` vs `89 50 4E 47 0D 0A 1A 0A`) in `Inspect()`
    + complete name table in `GetTypeName()` → `[TOTAL](4/4) [OK](4/4)` exit 0.
    Step 4 VERIFIED DONE 20 Oct 2026 (NOT test-driven): the `mime_recognises_jpeg_signature` test AND the
    jpeg guard were already in the working tree, uncommitted and never built or run — verified after the fact
    (`[TOTAL](5/5) [OK](5/5)` exit 0); its RED phase was never observed.
    Step 5 DONE 20 Oct 2026 test-first: `mime_recognises_gif_signature` written FIRST (red `[FAIL] #5`,
    `[OK](5/6)` `[FAIL](1/6)` exit 1), then a third `Inspect()` guard (`m_signatureCount >= 6`, GIF89a OR GIF87a
    six-byte `memcmp`) → `[TOTAL](6/6) [OK](6/6)` exit 0.
    Gotchas: two `cmake --build` calls batched together race on the shared `.pdb` (`C1090`/`C1041 … use /FS`) —
    one cmake build per command, strictly sequential; and the build-and-run-must-be-separate rule bit again.
    Step 6 DONE 20 Oct 2026 test-first: `mime_recognises_pdf_signature` written FIRST (one stream,
    `%PDF-1.4\n` + body) and seen RED (`[FAIL] #6`, `[OK](6/7)` `[FAIL](1/7)` exit 1), then a fourth
    `Inspect()` guard (`m_signatureCount >= 4`, `memcmp` vs `{0x25,0x50,0x44,0x46}` = `%PDF`) →
    `[TOTAL](7/7) [OK](7/7)` exit 0.
    Step 7 DONE 20 Oct 2026 test-first: `mime_recognises_zip_signature` written FIRST (one stream,
    4-byte `PK 03 04` local file header magic as raw octets `"PK\x03\x04"` + body) and seen RED
    (`[FAIL] #7`, `[OK](7/8)` `[FAIL](1/8)` exit 1), then a fifth `Inspect()` guard (`m_signatureCount
    >= 4`, `memcmp` vs `{0x50,0x4B,0x03,0x04}` = `PK 03 04`) → `[TOTAL](8/8) [OK](8/8)` exit 0.
    Steps 8/9/10/11 DONE 20-22 Oct 2026 test-first (xml `<?xml` marker → `[TOTAL](9/9) [OK](9/9)`; html doctype
    + document-level first tag via file-local `markupMatch()` case-fold helper → `[TOTAL](10/10) [OK](10/10)`;
    json first non-JSON-whitespace octet `{`/`[` → `[TOTAL](11/11) [OK](11/11)`; javascript shebang `#!` + top-level
    `function ` (plain `memcmp`, NOT case-folded markupMatch — JS is case-sensitive) + leading `//`/`/*` comment →
    `[TOTAL](12/12) [OK](12/12)`, all exit 0, `mime.h` unchanged throughout). Step 12 DONE 22 Oct 2026
    test-first (plain text = the whole 16-byte window captured and every octet printable 0x80-0xFF | 0x20-0x7E |
    tab/LF/CR → `[TOTAL](13/13) [OK](13/13)`; 10 of 10 target types recognised). Step 13 DONE 22 Oct 2026
    test-first: `mime_falls_back_to_octet_stream` written FIRST (two Push streams: a NUL-led binary blob with
    explicit C0/DEL octets, and a Windows executable image prefix — printable 'MZ' betrayed by the NUL/0x90
    octets inside the window) seen RED (`[FAIL] #13`, `[TOTAL](14/14) [OK](13/14)`, exit 1), then the eleventh
    and LAST fixed-order `Inspect()` guard after the plain-text guard (`m_detectedType == Unresolved &&
    m_signatureCount >= SIGNATURE_WINDOW_BYTES` → `MIMEType::OctetStream`, `"application/octet-stream"`; the
    only guard with no signature of its own, insists on a full window like the plain-text guard) → GREEN
    `[TOTAL](14/14) [OK](14/14)` exit 0 — **the recognizer is now total**. Plan step 14
    `mime_recognises_type_split_across_chunks` DONE 22 Oct 2026 (test written FIRST: the PNG stream pumped
    three octets at a time, so the eight byte PNG magic is split across three pumped chunks; asserts pass-through,
    `GetSignatureByteCount() == 16`, `== MIMEType::PNG`, `"image/png"`; GREEN on the FIRST run `[TOTAL](15/15)`
    `[OK](15/15)` exit 0 with NO implementation change — the window already accumulates across chunks because
    `m_signatureCount` is a member and `Inspect()` runs again on every pumped chunk, so the case proves
    existing behaviour rather than driving new code; logs `tmp/qortestmime_step14_build_exe.log`,
    `tmp/qortestmime_step14_green.log`). Plan step 15 `mime_recognizer_resets_between_streams` DONE and
    verified PASS 22 Oct 2026 test-first: test written FIRST and GREEN on the FIRST run — ONE
    `qor::mime::MIMERecognizerFilter` instance serves TWO streams (the PNG stream of step 3, then `Reset()`,
    then the PDF stream of step 6 through FRESH elements re-linked with `Filter::SetSource`/`Filter::SetSink`);
    after `Reset()` it asserts the state IS cleared (`GetDetectedType() == MIMEType::Unresolved`,
    `GetTypeName() == ""`, `GetSignatureByteCount() == 0`) and stream 2 re-recognises `== MIMEType::PDF` /
    `"application/pdf"` with pass-through and a 16-octet window → `[TOTAL](16/16)` `[OK](16/16)` exit 0 with
    NO implementation change (`mime.h`/`mime.cpp` untouched, only `test_mime.cpp` recompiled; logs
    `tmp/qortestmime_step15_build_exe.log`, `tmp/qortestmime_step15_green.log`). LESSON: a filter is reused
    by re-wiring the FILTER, not its plumbing — `Element::SetSink`/`SetSource` return false on an element
    that already has a connection. Plan step 16 `mime_recogniser_chained_with_qor_base64encodefilter` DONE and verified PASS 22 Oct 2026 test-first
    (TWO pipeline filters in series — `StringSource -> MIMERecognizerFilter -> Base64EncodeFilter -> StringSink`).
    Anatomy read FIRST: `Base64EncodeFilter` is an `InlineFilter<byte>` — a Buffer pair, not an Element — so it is
    inserted as the buffer shared by the elements either side of it (`Pipeline::InsertInlineFilter`, BeforeSink
    position: the sink's buffer IS the filter and the sink's source element also buffers it); wired by hand here
    with `Base64EncodeFilter encodeFilter(128)` because its default ctor holds zero items. Stream = the 8-byte
    PNG magic as explicit octets + a 10-byte payload = 18 octets (a multiple of 3, so base64 pads nothing; longer
    than the 16-byte window; one pumped chunk), pumped for the 24 ENCODED units; asserts the sink text is exactly
    the base64 of the stream (`"iVBORw0KGgpJSERSIHFvcjAx"`, computed independently) and that the recognizer still
    reports `== MIMEType::PNG` / `"image/png"` with a 16-octet window. Seen RED FIRST — the sink received only the
    FIRST 18 of the 24 encoded characters (`[FAIL] #16`, `[TOTAL](17/17) [OK](16/17) [FAIL](1/17)`, exit 1; logs
    `tmp/qortestmime_step16_run.log`, `tmp/qortestmime_step16_diag_run.log`, `tmp/qortestmime_step16_diag2_run.log`),
    caused by `mime.cpp::TransferOctets` passing ONE variable to both buffer acknowledgements — `InlineFilter::WriteAcknowledge`
    rewrites the count it is handed to the number of units it PRODUCED (base64 expands 4 per 3) while `Buffer::ReadAcknowledge`
    clamps its count back to what the source holds, so the expansion was lost before it reached the caller. Fix: keep
    `unitsConsumed` (the raw count, for the memcpy and the source acknowledgement) apart from `unitsToTransfer` (rewritten
    by the sink's `WriteAcknowledge` to the produced count, returned in `unitsTransferred`); for a plain buffer the two
    are equal, so steps 1-15 keep their behaviour. `mime.h` contract note extended accordingly; `test/framework/data/mime/CMakeLists.txt`
    gained `qor_base64encodefilter` in `qor_libraries`. Rebuilt `qor_mime` then `QORTestMIME` (separate commands) ->
    `[TOTAL](17/17) [OK](17/17)`, no `[FAIL`, exit 0 (logs `tmp/qor_mime_step16_dll.log`, `tmp/qortestmime_step16_exe.log`,
    `tmp/qortestmime_step16_green_run.log`). LESSON: a recognising filter inspects the stream BEFORE any downstream
    transform, so it classifies the pre-encoded octets and is valid in front of an encoding filter; a pass-through
    filter must propagate the count its sink buffer PRODUCED, not the count it CONSUMED. Next: optional scope only —
    `mime_type_name_drives_a_sink` (the recognised type name driving a sink).


14. [x] **qor_jsondata parse performance — verification FINALIZED 22 Oct 2026 (round 18)** (D9 + `docs/agent/verification.md`
    "qor_jsondata parse-perf FINALIZATION (round 18)" — the ONLY reproducible record; the round-16 figures that
    used to be quoted here (2,102,067-byte `test/data/big_json.txt`, a `PARSE_TIME_MS` env mode, a
    `tmp/pristine_qorjson` build tree, `[OK](42/42)`, ~6.7M `log::debug` lines) do not exist in this tree and
    must not be cited): the three micro-edits (RTTI removal, `#ifndef NDEBUG`
    logging guards, podbuffer power-of-2 buffer masking) measured **performance-neutral within the noise floor**.
    FINAL measurement (round 18, reproducible on this tree): fixture `test/data/data_2mb.json` = **2,219,028 bytes**;
    harness `test/components/formats/json/main.cpp` lines 284-305 prints `[perf] 2MB JSON parse (cold|warm): N ms`
    (there is NO `PARSE_TIME_MS` env mode in the committed harness); handler is `log::Level::Informative` so run logs
    are 7 non-blank lines - no log flood in either config. 21 RelWithDebInfo runs over 7 builds in `cmakebuild_rel`
    (logs `tmp/json_final_rel_{A,A2,E,F,D,D2,E2}_run{1,2,3}.log`, deleted from `tmp/` 22 Oct 2026 - the values below are
    the recorded readings, all exit `0`): git-HEAD sources **2110 ms mean**
    (per-build 1959.6 / 2182.5 / 2188.6 -> 11.8% spread for identical sources), current tree with all three edits
    **1879 ms mean** (builds 1875.4 / 1881.9; a third build of the same sources, `json_perf_rel_now.log` since deleted,
    measured 2128.5 ms mean -> 13.5% spread), mask-only 2259 ms, mask-less 2255 ms; Debug current tree 54.6-54.8 s.
    VERDICT: **performance-neutral within the noise floor** (the -11.0% A->D delta is smaller than the 13.5%
    same-source build spread and single-edit attribution is self-contradictory); the "2MB ~56 s -> ~18 s" claim
    does not exist in either
    config and the round-16 figures (2,102,067-byte `big_json.txt`, `tmp/pristine_qorjson` tree, `PARSE_TIME_MS` mode)
    are unreproducible because those artifacts are gone.
    The structural follow-up work is tracked as item 15 (DEFERRED) below.
    RESOLVED (no perf credit either way): `% m_size` -> `& m_wrapMask` in `PODBuffer::AddressOf` is neither a win nor a
    measured regression on this tree; KEEP it (it bounds every index read and the `assert` removal stays) unless the
    user objects on review grounds.
    **COMPLETED + user decision 22 Oct 2026**: all three micro-edits are KEPT in the tree and now committed at HEAD
    (`dea24af2 "JSON parser performance review concluded"`, 24 `src/` paths +105/-79 plus the harness +233/-192; kept on
    review grounds, zero performance credit claimed). `tmp/` pruned in two passes after the run: pass 1 deleted 45 files /
    300,687 B of superseded round-16/17 artifacts and scratch scripts; pass 2 deleted the 31 round-18 logs (146.5 KB - the
    28 `tmp/json_final_rel_*_{build,run1,run2,run3}.log`, `tmp/json_perf_rel_now.log`, `tmp/json_perf_debug_now.log`,
    `tmp/rel_D_build.log`). **`tmp/` now holds only the 3 dirs `tmp/dcast` / `tmp/edited` / `tmp/jsonread`** - no JSON-perf
    log survives, so the `docs/agent/verification.md` FINAL entry's table is the sole durable record and every `tmp/...`
    name in it (and in the SUPERSEDED entries) is provenance only.

15. [ ] **qor_jsondata STRUCTURAL parse-performance refactoring — DEFERRED 22 Oct 2026** (parked until greater
    resources are available; nothing started). Prep inventory — measure each under the same `[perf]` timer, believe
    only deltas over ~14%:
    (a) bulk scanning of string/number runs in `character` instead of per-character transitions
        (`src/components/data/formats/json/parser/states/_2/character.cpp`, `_4/string.cpp`, `_3/integer.cpp`, `_3/fraction.cpp`),
    (b) devirtualized / table-driven state machine replacing the polymorphic `ParserState` hierarchy
        (`src/components/data/parser/state.cpp` + `src/components/data/parser/states/{anyoneof,anyoneofset,oneofarange,oneormore,optional,sequence,specific,zeroormore}.{h,cpp}`,
        48 JSON state files (17 .cpp) under `parser/states/_1.._4`),
    (c) model-node pooling - prerequisite: remove the `Parser::FinalParse()` -> `AnyObject::Cast<Fastflow>` hop
        (`typedany.h:158` `CastToBase_` does a live `dynamic_cast` per node).

16. [x] **winqor_api_shell — 60 stateless shell32.dll wrappers (26 Oct 2026)** — DONE and verified: `shell.h`
    declares `qor::win::api::Shell` with 60 static prototypes, seven .cpp files define all 60 in the kernel
    pattern (`qor_pp_useswinapi(shell32, Name)` + `Library::Call`, `CheckReturn<HRESULT>::TType` /
    `CheckReturn<BOOL>::TType` where the SDK returns HRESULT/BOOL). Build green, audit 60/60/60 with zero
    missing and zero non-exported names. Open decision left for review: the 4 live-but-out-of-scope
    `SH*UnreadMail*W` / `SHGetDriveMedia` entry points (see DECISIONS.md D12 and shell/purpose.md).


## Later
