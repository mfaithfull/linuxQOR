"# Verification log (compiler-as-judge, per D3)

Unit of verification = one QOR lib (one `set(target qor_X)`). Incremental rebuild only the touched lib.
Record one entry per verification run. Newest at top."

## Build commands (incremental)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target qor_<NAME> -- -j4

## Lint
clang-tidy -p build docs/.../file.cpp

## Test
& build\test\<path>\<exe>   # e.g. build\test\framework\data\mime\Debug\QORTestMIME.exe
# green = "[OK](n/n)" in the output AND shell exit code 0. The QOR does not use ctest.

## Results
### 2026-10-22 — qor_mime plan step 14 / QORTestMIME (signature split across chunks) — PASS

- Command (build the test exe only — `qor_mime` lib sources unchanged, only `test_mime.cpp` recompiled): `cmake --build C:\RAID0\Develop\linuxQOR\build --target QORTestMIME --config Debug` → exit `0`, log `tmp/qortestmime_step14_build_exe.log` (compiles `test_mime.cpp` only, links `qor_mime.dll` / `qor_pipeline.dll` / `qor_qor.dll` / `qor_error.dll`; no `error C`/`warning C`/`LNK`).
- Command (run the exe from its own build dir, as required by the `qor.dll` relative data path): `cd C:\RAID0\Develop\linuxQOR\build\test\framework\data\mime\Debug ; ./QORTestMIME.exe` → exit `0`, log `tmp/qortestmime_step14_green.log`.
- Result: `test_mime.cpp(1071) : mime_recognises_type_split_across_chunks` then `[TOTAL](15/15)`, `[OK](15/15)`, NO `[FAIL](` line, exit `0`.
- Note: the case went **GREEN on the first run** — the plan expected NO implementation change and the step is a behaviour proof, not new code. The 8-byte PNG magic split across three 3-byte pumped chunks still classifies `== MIMEType::PNG` / `"image/png"`, `GetSignatureByteCount()` still caps at 16, and pass-through is intact. The case is meaningful: had the window NOT accumulated across pumped chunks, the 16-byte count assert would fail.
- No build gotchas: one `cmake --build` per command and build-then-run-separately both held; the exe run needed no `qor.dll` payload fix (15th run clean).

### 2026-10-22 — qor_mime plan step 13 / QORTestMIME (application/octet-stream fallback) — PASS
- **Test first**: added `mime_falls_back_to_octet_stream` to `test/framework/data/mime/test_mime.cpp`
  BEFORE touching `mime.cpp`/`mime.h` (TWO Push-mode streams in the one case, fresh elements for the
  second — the shape of steps 5/9/10/11/12: stream 1 a raw binary blob opening with a NUL and carrying
  the C0 controls and DEL written as explicit octets, then a printable payload tail; stream 2 the leading
  octets of a Windows executable image — the printable 'M' 'Z' DOS prefix followed by the 0x90 padding
  byte and NUL offset fields, then a payload tail; per stream asserts `sink.GetData() == data` pass-through,
  `GetSignatureByteCount() == 16`, `GetDetectedType() == MIMEType::OctetStream`,
  `std::string(GetTypeName()) == "application/octet-stream"`).
- **Run (red)**: `cmake --build build --target QORTestMIME` (lib unchanged) then the exe run in its OWN
  command from `build\test\framework\data\mime\Debug` → `test_mime.cpp(955) : mime_falls_back_to_octet_stream
  [FAIL], #13`, `[TOTAL](14/14) [OK](13/14) [FAIL](1/14)`, exit 1, reason `expected boolean to have the value
  of true / value was : 0` → the test is meaningful (no fallback exists, so an unmatched stream stays
  `Unresolved` with an empty name). Log `tmp/qortestmime_step13_red.log`.
- **Code**: `mime.cpp` only — an eleventh and LAST fixed-order guard in `Inspect()` AFTER the plain text
  guard: `m_detectedType == MIMEType::Unresolved && m_signatureCount >= SIGNATURE_WINDOW_BYTES` →
  `MIMEType::OctetStream`. It is the only guard with no signature of its own — it fires when every guard
  above has declined — and like the plain text guard it insists on a FULL window, because a partially
  captured window is not evidence that a stream carries no signature; a stream shorter than the window
  stays `Unresolved` until the octets that decide it arrive (the existing 12-byte truncation test still
  passes with `GetSignatureByteCount() == 12`). `mime.h` unchanged (the name table has carried
  `application/octet-stream` since step 3). The guard makes the recognizer total: any stream pumped in
  full ends with a type and a non-empty name.
- **GREEN**: rebuilt `qor_mime` (log `tmp/qortestmime_step13_build_lib.log`, no errors/warnings) then
  `QORTestMIME` (log `tmp/qortestmime_step13_build_exe.log`) in SEPARATE commands, then the exe run from
  its build dir → `[TOTAL](14/14) [OK](14/14)`, zero `FAIL` lines, exit 0 (`tmp/qortestmime_step13_green.log`).
  **All 10 target types plus the fallback recognised.** No new build gotchas.

### 2026-10-22 — qor_mime plan step 12 / QORTestMIME (plain text recognition) — PASS
- **Test first**: added `mime_recognises_plain_text_signature` to `test/framework/data/mime/test_mime.cpp`
  BEFORE touching `mime.cpp`/`mime.h` (TWO Push-mode streams in the one case, fresh elements for the
  second — the shape of steps 5, 9, 10 and 11: stream 1 an ASCII-only prose document longer than the
  sixteen byte signature window (letters, digits, punctuation, spaces, a tab, line feeds), stream 2 a
  document carrying UTF-8 multi-byte octets written as explicit octets (a two byte sequence and a three
  byte sequence) so the stream does not depend on the encoding of the source file; per stream asserts
  `sink.GetData() == data` pass-through, `GetDetectedType() == MIMEType::PlainText`,
  `std::string(GetTypeName()) == "text/plain"`).
- **Run (red)**: `cmake --build build --target QORTestMIME` (lib unchanged) then the exe run in its OWN
  command from `build\test\framework\data\mime\Debug` → `test_mime.cpp(841) : mime_recognises_plain_text_signature
  [FAIL], #12`, `[TOTAL](13/13) [OK](12/13) [FAIL](1/13)`, exit 1, reason `expected boolean to have the value of true / value was : 0`
  → the test is meaningful (no plain text detector exists). Log `tmp/qortestmime_step12_red.log`,
  build log `tmp/qortestmime_step12_red_build.log`.
- **Code**: `mime.cpp` only — a tenth fixed-order guard in `Inspect()` AFTER the JavaScript guards: it
  requires `m_detectedType == Unresolved && m_signatureCount >= SIGNATURE_WINDOW_BYTES`, i.e. the WHOLE
  window captured (unlike the shorter signatures above), so a binary stream cannot look like text before
  the control octets that betray it have arrived; it then scans every captured octet and accepts only
  0x80-0xFF (UTF-8 encoded text), 0x20-0x7E (printable ASCII) and 0x09/0x0A/0x0D (tab, line feed, carriage
  return) — any other control octet (NUL, the other C0 controls, DEL) rejects the stream and leaves it
  unresolved for the octet-stream fallback (step 13). Well formed UTF-8 is deliberately NOT validated here:
  that belongs to the Unicode layer, which a framework layer module must not depend on. `mime.h` unchanged
  (name table complete since step 3); the running comment in `Inspect()` corrected from "remaining type
  (plain text) and the octet-stream fallback" to "the octet-stream fallback".
- **Build**: `cmake --build build --target qor_mime` → exit 0 (`qor_mime.dll`), then
  `cmake --build build --target QORTestMIME` → exit 0, no `error C`/`warning C`/`LNK` in either log
  (`tmp/qortestmime_step12_build_lib.log`, `tmp/qortestmime_step12_build_exe.log`).
- **Run (green)**: from `build\test\framework\data\mime\Debug` → `[TOTAL](13/13) [OK](13/13)`, no
  `[FAIL` in the log, `$LASTEXITCODE = 0` → PASS. Log `tmp/qortestmime_step12_green.log`. **10 of 10
  target types now recognised** (PNG, JPEG, GIF, PDF, ZIP, XML, HTML, JSON, JavaScript, plain text).
- No new gotchas: the one-cmake-build-per-command and build-then-run-separately rules both held. **Learning:**
  the weakest heuristic must demand a FULL signature window — a partially captured window is not evidence
  of text, and requiring the full window is what keeps this guard from stealing the binary, markup and
  container streams whose definite signatures are matched earlier in the fixed order.


```
### 2026-10-22 — qor_mime plan step 11 / QORTestMIME (JavaScript shebang + function + comment recognition) — PASS
- **Test first**: added `mime_recognises_javascript_signature` to `test/framework/data/mime/test_mime.cpp`
  BEFORE touching `mime.cpp`/`mime.h` (THREE Push-mode streams in the one case, fresh elements per stream —
  the multi-stream shape of steps 5/9/10: stream 1 a standalone script leading with the shebang `#!` (0x23 0x21),
  stream 2 a source opening with a top-level `function ` declaration, stream 3 a source opening with a block
  comment `/*`; per stream asserts `sink.GetData() == data` pass-through, `GetDetectedType() == MIMEType::JavaScript`,
  `std::string(GetTypeName()) == "application/javascript"`).
- **Run (red)**: `cmake --build build --target QORTestMIME` (lib unchanged) then the exe run in its OWN
  command from `build\test\framework\data\mime\Debug` → `test_mime.cpp(708) : mime_recognises_javascript_signature
  [FAIL], #11`, `[TOTAL](12/12) [OK](11/12) [FAIL](1/12)`, exit 1 → the test is meaningful (no JavaScript detector
  exists). Log `tmp/qortestmime_step11_red.log`, build log `tmp/qortestmime_step11_red_build.log`.
- **Code**: `mime.cpp` only — a ninth fixed-order guard in `Inspect()` AFTER the JSON container guard: (1) shebang
  (`m_signature[0]==0x23 && m_signature[1]==0x21`), (2) a top-level `function ` declaration via a plain `memcmp`
  vs `{66 75 6E 63 74 69 6F 6E 20}` (JavaScript is case-sensitive, so `markupMatch()` is NOT used — a case-folded
  compare would wrongly accept `FUNCTION`), (3) a leading source comment `//` (0x2F 0x2F) or `/*` (0x2F 0x2A).
  JavaScript has no magic number, so this is a weak text heuristic evaluated after JSON in the fixed order (png,
  jpeg, gif, pdf, zip, xml, html, json, javascript, ...). The arrow form `=>` is contemplated as a further weak
  signature but is not yet exercised by a test, so it is not implemented. `mime.h` unchanged (name table complete
  since step 3); the running comment in `Inspect()` corrected from "remaining two types" to "remaining type ... and
  the octet-stream fallback".
- **Build**: `cmake --build build --target qor_mime` → exit 0 (`qor_mime.dll`), then
  `cmake --build build --target QORTestMIME` → exit 0, no `error C`/`warning C`/`LNK` in either log
  (`tmp/qortestmime_step11_build_lib.log`, `tmp/qortestmime_step11_build_exe.log`).
- **Run (green)**: from `build\test\framework\data\mime\Debug` → `[TOTAL](12/12) [OK](12/12)`, no `[FAIL` in the
  log, `$LASTEXITCODE = 0` → PASS. Log `tmp/qortestmime_step11_green.log`. **9 of 10 target types now recognised**
  (PNG, JPEG, GIF, PDF, ZIP, XML, HTML, JSON, JavaScript).
- No new gotchas: the one-cmake-build-per-command and build-then-run-separately rules both held. **Learning:** a
  case-sensitive language keyword (JS `function`) needs a plain byte compare, not the case-folded `markupMatch` used
  for the case-insensitive HTML markup — reusing a case-folded helper across differently-cased languages is a false-positive
  hazard.



### 2026-10-22 — qor_mime plan step 10 / QORTestMIME (JSON container recognition) — PASS
- **Test first**: added `mime_recognises_json_signature` to `test/framework/data/mime/test_mime.cpp`
  BEFORE touching `mime.cpp`/`mime.h` (TWO Push-mode streams in the one case, fresh elements for the
  second — the shape of steps 5 and 9: stream 1 the JSON object `{ "qor" : "mime test", "step" : 10 }`
  with no leading whitespace and a longer body, stream 2 a JSON array preceded by JSON whitespace
  (three spaces, a line feed, a tab) so the skip-past-whitespace half of the heuristic is exercised;
  per stream asserts `sink.GetData() == data` pass-through, `GetDetectedType() == MIMEType::JSON`,
  `std::string(GetTypeName()) == "application/json"`).
- **Run (red)**: `cmake --build build --target QORTestMIME` (lib unchanged) then the exe run in its OWN
  command from `build\test\framework\data\mime\Debug` → `test_mime.cpp(610) : mime_recognises_json_signature
  [FAIL], #10`, `[TOTAL](11/11) [OK](10/11) [FAIL](1/11)`, exit 1, reason `expected boolean to have the value of true / value was : 0`
  → the test is meaningful (no JSON detector exists). Log `tmp/qortestmime_step10_red.log`,
  build log `tmp/qortestmime_step10_build_red.log`.
- **Code**: `mime.cpp` only — an eighth fixed-order guard in `Inspect()` after the HTML markup guards:
  scan the captured window for the FIRST octet that is not JSON white space (0x20 space, 0x09 tab,
  0x0A line feed, 0x0D carriage return — the only four characters JSON allows as whitespace), then
  set `m_detectedType = MIMEType::JSON` when that octet is `{` (0x7B) or `[` (0x5B) and stop the scan.
  JSON has no magic number, so this is a text heuristic evaluated after the HTML checks in the fixed
  order (png, jpeg, gif, pdf, zip, xml, html, json, ...). Unlike the markup compares above it is NOT
  case-relevant, so a plain octet compare is enough — `markupMatch()` is not used and no new helper
  was needed. The guard decides as soon as a single non-whitespace octet is captured, so it does not
  need the window to be full. `mime.h` unchanged (name table complete since step 3); the running
  comment in `Inspect()` was corrected from "remaining three types" to "remaining two types".
- **Build**: `cmake --build build --target qor_mime` → exit 0 (`qor_mime.dll`), then
  `cmake --build build --target QORTestMIME` → exit 0, no `error C`/`warning C`/`LNK` in either log
  (`tmp/qortestmime_step10_build_lib.log`, `tmp/qortestmime_step10_build_exe.log`).
- **Run (green)**: from `build\test\framework\data\mime\Debug` → `[TOTAL](11/11) [OK](11/11)`, no
  `[FAIL` in the log, `$LASTEXITCODE = 0` → PASS. Log `tmp/qortestmime_step10_green.log`. **8 of 10
  target types now recognised** (PNG, JPEG, GIF, PDF, ZIP, XML, HTML, JSON).
- No new gotchas: the one-cmake-build-per-command and build-then-run-separately rules both held.
  **Learning:** a heuristic that must skip a variable-length prefix (JSON whitespace before the
  container) is a scan of the window, not a fixed-offset compare — and it must stop at the first
  non-skipped octet so that a later octet of the same kind cannot produce a false positive.


### 2026-10-22 — qor_mime plan step 9 / QORTestMIME (HTML doctype + document-level first-tag recognition) — PASS
- **Test first**: added `mime_recognises_html_signature` to `test/framework/data/mime/test_mime.cpp`
  BEFORE touching `mime.cpp`/`mime.h` (TWO Push-mode streams in the one case, fresh elements for the
  second — mirroring step 5's two GIF variants: stream 1 the lower case HTML5 doctype
  `<!DOCTYPE html>\n<html><head><title>...` and stream 2 no doctype at all, opening with the upper case
  document-level tag `<HTML>\n<head>...`; per stream asserts `sink.GetData() == data` pass-through,
  `GetDetectedType() == MIMEType::HTML`, `std::string(GetTypeName()) == "text/html"`).
- **Run (red)**: `cmake --build build --target QORTestMIME` (lib unchanged) then the exe run in its OWN
  command from `build\test\framework\data\mime\Debug` → `test_mime.cpp(514) : mime_recognises_html_signature
  [FAIL], #9`, `[TOTAL](10/10) [OK](9/10) [FAIL](1/10)`, exit 1, reason `expected boolean to have the value of true / value was : 0`
  → the test is meaningful (no HTML detector exists). Log `tmp/qortestmime_step9_red.log`,
  build log `tmp/qortestmime_step9_build_red.log`.
- **Code**: `mime.cpp` only — a seventh fixed-order guard in `Inspect()` after the XML marker: a 14-octet
  `<!DOCTYPE HTML` doctype compare (the HTML5 form) and, while `m_detectedType` is still `Unresolved`, a
  document-level first-tag compare (`m_signature[0] == 0x3C` `'<'` then case-folded `html`/`head`/`body`/`title`).
  HTML has no binary magic number, so both are text heuristics evaluated after the XML declaration marker
  in the fixed order. Unlike the magics above these compares CANNOT use `memcmp` — HTML markup spelling is
  case-insensitive — so `mime.cpp` gained a file-local `static bool markupMatch(const qor::byte*, size_t,
  const qor::byte*, size_t)` ASCII case-fold compare (folds ASCII a-z/A-Z to upper only, no locale, no new
  dependency; internal helper, no `qor_pp_module_interface`, not part of the module interface). `markupMatch`
  also checks the window length, so each compare decides as soon as its own octets are captured. `mime.h`
  unchanged (name table complete since step 3).
- **Build**: `cmake --build build --target qor_mime` → exit 0 (`qor_mime.dll`), then
  `cmake --build build --target QORTestMIME` → exit 0, no `error C`/`warning C`/`LNK` in either log
  (`tmp/qortestmime_step9_build_lib.log`, `tmp/qortestmime_step9_build_exe.log`).
- **Run (green)**: from `build\test\framework\data\mime\Debug` → `[TOTAL](10/10) [OK](10/10)`, no
  `[FAIL`, `$LASTEXITCODE = 0` → PASS. Log `tmp/qortestmime_step9_green.log`. **7 of 10 target types
  now recognised** (PNG, JPEG, GIF, PDF, ZIP, XML, HTML).
- No new gotchas: the one-cmake-build-per-command and build-then-run-separately rules both held.
  **Learning:** a format with two recognition paths (doctype OR first tag) is still ONE increment —
  the test asserts every path, the code gets one guard with one compare per path (same shape as step 5's
  GIF87a/GIF89a pair, but with case-folded text compares instead of `memcmp`).

### 2026-10-20 — qor_mime plan step 8 / QORTestMIME (XML `<?xml` declaration recognition) — PASS
- **Test first**: added `mime_recognises_xml_signature` to `test/framework/data/mime/test_mime.cpp`
  BEFORE touching `mime.cpp`/`mime.h` (one Push-mode stream: the five character XML declaration
  marker `<?xml version="1.0"?>\n` + short body; asserts `sink.GetData() == xmlData`
  pass-through, `GetDetectedType() == MIMEType::XML`, `std::string(GetTypeName()) == "application/xml"`).
- **Run (red)**: `cmake --build build --target QORTestMIME` (lib unchanged) then the exe run in its OWN
  command from `build\test\framework\data\mime\Debug` → `test_mime.cpp(450) : mime_recognises_xml_signature
  [FAIL], #8`, `[TOTAL](9/9) [OK](8/9) [FAIL](1/9)`, exit 1 → the test is meaningful (no XML detector
  exists). Log `tmp/qortestmime_step8_red.log`, build log `tmp/qortestmime_step8_red_build.log`.
- **Code**: `mime.cpp` only — a sixth fixed-order guard in `Inspect()` after png/jpeg/gif/pdf/zip:
  `m_detectedType == Unresolved && m_signatureCount >= 5`, then `memcmp` the window against
  `xmlSignature` {0x3C,0x3F,0x78,0x6D,0x6C} (`<` `?` `x` `m` `l`) → `m_detectedType = MIMEType::XML`.
  `mime.h` unchanged (name table complete since step 3). No binary magic number exists for XML, so this
  is the first strong text heuristic, evaluated after the definite binary signatures in the fixed order.
- **Build**: `cmake --build build --target qor_mime` → exit 0 (`qor_mime.dll`), then
  `cmake --build build --target QORTestMIME` → exit 0, no `error C`/`warning C`/`LNK` in either log
  (`tmp/qortestmime_step8_build_lib.log`, `tmp/qortestmime_step8_build_exe.log`).
- **Run (green)**: from `build\test\framework\data\mime\Debug` → `[TOTAL](9/9) [OK](9/9)`, no
  `[FAIL`, `$LASTEXITCODE = 0` → PASS. Log `tmp/qortestmime_step8_green.log`. **6 of 10 target types
  now recognised** (PNG, JPEG, GIF, PDF, ZIP, XML).
- No new gotchas: the one-cmake-build-per-command and build-then-run-separately rules both held.

### 2026-10-20 — qor_mime plan step 7 / QORTestMIME (ZIP `PK 03 04` recognition) — PASS
- **Test first**: added `mime_recognises_zip_signature` to `test/framework/data/mime/test_mime.cpp`
  BEFORE touching `mime.cpp`/`mime.h` (one Push-mode stream: the 4-byte `PK 03 04` local file header
  magic written as raw octets `"PK\x03\x04"` + short body; asserts `sink.GetData() == zipData`
  pass-through, `GetDetectedType() == MIMEType::ZIP`, `std::string(GetTypeName()) == "application/zip"`).
- **Run (red)**: `cmake --build build --target QORTestMIME` (lib unchanged) then the exe run in its OWN
  command from `build\test\framework\data\mime\Debug` → `test_mime.cpp(394) : mime_recognises_zip_signature
  [FAIL], #7`, `[TOTAL](8/8) [OK](7/8) [FAIL](1/8)`, exit 1 → the test is meaningful (no ZIP detector
  exists). Log `tmp/qortestmime_step7_red.log`, build log `tmp/qortestmime_step7_red_build.log`.
- **Code**: `mime.cpp` only — a fifth fixed-order guard in `Inspect()` after png/jpeg/gif/pdf:
  `m_detectedType == Unresolved && m_signatureCount >= 4`, then `memcmp` the window against
  `zipSignature` {0x50,0x4B,0x03,0x04} (`P` `K` `03` `04`) → `m_detectedType = MIMEType::ZIP`.
  `mime.h` unchanged (name table complete since step 3).
- **Build**: `cmake --build build --target qor_mime` → exit 0 (`qor_mime.dll`), then
  `cmake --build build --target QORTestMIME` → exit 0, no `error C`/`warning C`/`LNK` in either log
  (`tmp/qortestmime_step7_build_lib.log`, `tmp/qortestmime_step7_build_exe.log`).
- **Run (green)**: from `build\test\framework\data\mime\Debug` → `[TOTAL](8/8) [OK](8/8)`, no
  `[FAIL`, `$LASTEXITCODE = 0` → PASS. Log `tmp/qortestmime_step7_green.log`. 5 of 10 target types
  now recognised (PNG, JPEG, GIF, PDF, ZIP).
- No new gotchas: the one-cmake-build-per-command and build-then-run-separately rules both held.

### 2026-10-20 — qor_mime plan step 6 / QORTestMIME (PDF header recognition) — PASS
- **Test first**: added `mime_recognises_pdf_signature` to `test/framework/data/mime/test_mime.cpp`
  BEFORE touching `mime.cpp`/`mime.h` (one stream: `%PDF-1.4\n` + short body — the `%PDF` magic
  followed by the version marker as real files have it; asserts `sink.GetData() == pdfData`
  pass-through, `GetDetectedType() == MIMEType::PDF`, `std::string(GetTypeName()) == "application/pdf"`).
- **Run (red)**: `cmake --build build --target QORTestMIME` then the exe run in its OWN command from
  `build\test\framework\data\mime\Debug` → `test_mime.cpp(342) : mime_recognises_pdf_signature [FAIL],
  #6`, `[TOTAL](7/7) [OK](6/7) [FAIL](1/7)`, exit 1 → the test is meaningful (no PDF detector exists).
  Log `tmp/qortestmime_step6_red.log`.
- **Code**: `mime.cpp` only — a fourth fixed-order guard in `Inspect()` after png/jpeg/gif:
  `m_detectedType == Unresolved && m_signatureCount >= 4`, then `memcmp` the window against
  `pdfSignature` {0x25,0x50,0x44,0x46} (`%` `P` `D` `F`) → `m_detectedType = MIMEType::PDF`.
  `mime.h` unchanged (name table complete since step 3).
- **Build**: `cmake --build build --target qor_mime` → exit 0 (`qor_mime.dll`), then
  `cmake --build build --target QORTestMIME` → exit 0, no `error C`/`warning C`/`LNK` in either log.
- **Run (green)**: from `build\test\framework\data\mime\Debug` → `[TOTAL](7/7) [OK](7/7)`, no
  `[FAIL`, `$LASTEXITCODE = 0` → PASS. Log `tmp/qortestmime_step6_green.log`. 4 of 10 target types
  now recognised (PNG, JPEG, GIF, PDF).
- No new gotchas: the one-cmake-build-per-command and build-then-run-separately rules both held.

### 2026-10-20 — qor_mime plan step 5 / QORTestMIME (GIF header recognition) — PASS
- **Test first**: added `mime_recognises_gif_signature` to `test/framework/data/mime/test_mime.cpp`
  BEFORE touching `mime.cpp`/`mime.h` (two streams in the one case, fresh elements each: `GIF89a`
  and `GIF87a` six-byte headers + short bodies; asserts `sink.GetData() == data` pass-through,
  `GetDetectedType() == MIMEType::GIF`, `std::string(GetTypeName()) == "image/gif"`).
- **Run (red)**: `cmake --build build --target QORTestMIME --config Debug` then the exe run in its
  OWN command from `build\test\framework\data\mime\Debug` → `mime_recognises_gif_signature [FAIL],
  #5`, `[TOTAL](6/6) [OK](5/6) [FAIL](1/6)`, `expected boolean to have the value of true / value was
  : 0`, exit 1 → the test is meaningful (no GIF detector exists). Log `tmp/qortestmime_step5_red.log`.
- **Code**: `mime.cpp` only — a third fixed-order guard in `Inspect()` after png/jpeg:
  `m_detectedType == Unresolved && m_signatureCount >= 6`, then `memcmp` the window against
  `gif89Signature` {0x47,0x49,0x46,0x38,0x39,0x61} OR `gif87Signature` {…,0x37,…} →
  `m_detectedType = MIMEType::GIF`. `mime.h` unchanged (name table complete since step 3).
- **Build**: `cmake --build build --target qor_mime --config Debug` → SUCCESS (`qor_mime.dll`), then
  `cmake --build build --target QORTestMIME --config Debug` → SUCCESS, no `error C`/`warning C`/`LNK`.
- **Run (green)**: from `build\test\framework\data\mime\Debug` → `[TOTAL](6/6) [OK](6/6)`, no
  `[FAIL`, `$LASTEXITCODE = 0` → PASS. Log `tmp/qortestmime_step5_green.log`.
- **NEW GOTCHA**: issuing two `cmake --build` invocations in the SAME tool/command batch runs them
  concurrently against one build tree and they race on the shared `.pdb` files — symptoms
  `error C1090: PDB API call failed` (on an unrelated dependency) and
  `error C1041: cannot open program database … if multiple CL.EXE write to the same .PDB file,
  please use /FS`. The builds are NOT independent: one cmake build per call, strictly sequential.

### 2026-10-20 — qor_mime plan step 4 / QORTestMIME (JPEG magic recognition) — PASS (verified, not test-driven)
- **State found**: the step 4 test `mime_recognises_jpeg_signature` AND the JPEG detector were already
  in the working tree (uncommitted `git status`: `mime.cpp` + `test_mime.cpp` modified) but had never
  been built or run — no verification record, no status.md tick. So this increment was **verified
  after the fact**; its RED phase was NOT observed and cannot be honestly claimed.
- **Build**: `cmake --build build --target qor_mime --config Debug` (first attempt hit the transient
  `C1090` PDB race — see the gotcha above; retry succeeded) then
  `cmake --build build --target QORTestMIME --config Debug` → SUCCESS.
- **Run (green)**: exe run in its OWN command from `build\test\framework\data\mime\Debug` →
  5 cases listed (`mime_module_loads`, `mime_filter_passes_octets_through`,
  `mime_captures_leading_octets`, `mime_recognises_png_signature`, `mime_recognises_jpeg_signature`),
  `[TOTAL](5/5) [OK](5/5)`, no `[FAIL`, `$LASTEXITCODE = 0` → PASS. Log `tmp/qortestmime_step4_green.log`.
- **Code as found**: `Inspect()` gained `m_detectedType == Unresolved && m_signatureCount >= 3`
  → `memcmp` vs `{0xFF,0xD8,0xFF}` → `m_detectedType = MIMEType::JPEG`; guards are ordered per
  signature length so a shorter signature can decide before the 16-byte window is full.

### 2026-10-20 — qor_mime plan step 3 / QORTestMIME (PNG signature recognition) — PASS
- **Test first**: added `mime_recognises_png_signature` to `test/framework/data/mime/test_mime.cpp` BEFORE
  touching `mime.cpp`/`mime.h` (single stream: StringSource → `qor::mime::MIMERecognizerFilter` → StringSink,
  Push-mode pumping as step 1; asserts `sink.GetData() == data` pass-through, `GetDetectedType() ==
  MIMEType::PNG`, `std::string(GetTypeName()) == "image/png"`). Built `QORTestMIME` BEFORE writing any
  implementation code.
- **Run (red)**: `mime_recognises_png_signature [FAIL], #3`, `[TOTAL](4/4) [OK](3/4) [FAIL](1/4)`, exit 1
  (`tmp/qortestmime_step3_red.log`) — the pass-through assert passed, the two detection asserts failed
  (`GetDetectedType()` still `Unresolved`) → the test is meaningful.
- **Code**: `mime.cpp` only — fixed-order guard inside `Inspect()` after each capture
  (`m_detectedType == MIMEType::Unresolved && m_signatureCount >= 8`), `static const qor::byte
  pngSignature[8] = {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A}`, `memcmp` → `m_detectedType =
  MIMEType::PNG`; `GetTypeName()` gained the complete registered-name switch (all ten types +
  `application/octet-stream`; `Unresolved` → `nullptr`), so the name table is data and complete now
  while only the detectors stay incremental. `mime.h` unchanged.
- **Build**: `cmake --build build --target qor_mime --config Debug` then
  `cmake --build build --target QORTestMIME --config Debug` → SUCCESS.
- **Run (green)**: exe run in its OWN command from `build\test\framework\data\mime\Debug` →
  `[TOTAL](4/4) [OK](4/4)`, exit 0 (log `tmp/qortestmime_step3_green.log`).
- **Note**: no `error C`/`warning C`/`LNK` in either build.

### 2026-10-20 — qor_mime plan step 2 / QORTestMIME (signature window capture) — PASS
- **Test first**: added `mime_captures_leading_octets` to `test/framework/data/mime/test_mime.cpp`
  (Push-mode `Pipeline` wiring as step 1; 40-byte input → `GetSignatureByteCount() == 16`,
  fresh elements + 4-byte input → `== 4`; no `Reset()` reuse — that is plan step 15).
  Built `QORTestMIME` BEFORE writing any implementation code.
- **Run (red)**: from `build\test\framework\data\mime\Debug` →
  `mime_captures_leading_octets [FAIL], #2`, `[TOTAL](3/3) [OK](2/3) [FAIL](1/3)`, exit 1
  → the test is meaningful (placeholder `Inspect()` captured nothing and was not even called).
- **Code**: `mime.cpp` only — `Inspect()` captures via the buffer three-step read protocol
  (`buffer.h`/`buffer.cpp`): `ReadRequest(least(unitsToInspect, ReadCapacity, windowSpace))` →
  `memcpy` into `m_signature` → **`ReadReject(unitsRequested)`** to give the reservation back
  (the source is never consumed; `ReadReject` restores `m_readEnd`). `ReadFilter`/`WriteFilter`
  call `Inspect()` BEFORE `TransferOctets()`. `mime.h` unchanged (the hook was already declared).
- **Regenerate + build**: `cmake --build build --target qor_mime --target QORTestMIME` → SUCCESS,
  `qor_mime.dll` + `QORTestMIME.exe`, no `error C`/`warning C`/`LNK` lines.
- **Run (green)**: exe run in its OWN command after the build command (stale-exe gotcha avoided)
  → `[TOTAL](3/3) [OK](3/3)`, no `[FAIL`, `$LASTEXITCODE = 0` → PASS. Logs:
  `tmp/qortestmime_step2_red.log`, `tmp/qortestmime_step2_green.log`,
  `tmp/qortestmime_step2_final.log` (run against the final, un-instrumented build — the
  authoritative one).
- **Findings**: (1) pipeline buffer reads are reserve/use/commit-or-**reject**; `ReadReject` is
  the "peek without consuming" primitive a read-only filter needs. (2) the run right after a
  build in the same command batch used the PREVIOUS exe (the recorded gotcha bit again) — build
  and run must be separate sequential commands. (3) `log::debug` calls in `qor_mime` produced
  no output in `QORTestMIME` — the log flyer is not wired in this test exe; do not use log
  output as a test probe there.

### 2026-10-20 — qor_mime plan step 1 / QORTestMIME (pass-through increment) — PASS
- **Test first**: added `mime_filter_passes_octets_through` to `test/framework/data/mime/test_mime.cpp`
  (StringSource → `MIMERecognizerFilter` → StringSink under a Push-mode `Pipeline`; assert sink output == input).
  Built `QORTestMIME` BEFORE writing any implementation code.
- **Run (red)**: `Set-Location build\\test\\framework\\data\\mime\\Debug; .\\QORTestMIME.exe` →
  `mime_filter_passes_octets_through [FAIL], #1`, `[TOTAL](2/2) [OK](1/2) [FAIL](1/2)`,
  `expected boolean to have the value of true / value was : 0`, exit 1 → the test is meaningful.
- **Code**: `mime.h` gained `virtual bool TransferOctets(size_t& unitsTransferred, size_t unitsToTransfer)`;
  `mime.cpp` implements it like `CopyFilter::Copy` (sink `WriteRequest` / source `ReadRequest` / memcpy /
  both acks) and `ReadFilter`/`WriteFilter` call it. `Inspect()` and detection untouched.
- **Regenerate + build**: `cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug` then
  `cmake --build build --target qor_mime --target QORTestMIME` → SUCCESS, `qor_mime.dll` + `QORTestMIME.exe`,
  no errors/warnings.
- **Run (green)**: from `build\\test\\framework\\data\\mime\\Debug` → `[TOTAL](2/2) [OK](2/2)`, no `[FAIL]`,
  `$LASTEXITCODE = 0` → PASS. Logs: `tmp/qortestmime_step1.log` (red), `tmp/qortestmime_step1_green.log`.
- **Finding**: a filter only pumps when in `Element::FlowMode::Push` (it needs a `Pipeline` parent to carry the
  mode — `Element::SetFlowMode` forwards to the parent and stores nothing). In Pull mode
  `Filter::Read` calls `ReadFilter(unitsRead, unitsRead)` with `unitsRead == 0` at entry, so base-class pull
  pumping through a filter is a dead end in the current framework.

### 2026-10-20 — qor_mime (configure + build + test) — PASS
- **Configure**: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug` → SUCCESS (0.3s configure, 5.6s generate;
  new dirs `src/framework/data/mime` + `test/framework/data/mime` picked up)
- **Build lib**: `cmake --build build --target qor_mime` → SUCCESS — compiled `qor_mime.cpp`, `mime.cpp`;
  produced `build\src\framework\data\mime\Debug\qor_mime.dll` + import lib `qor_mime.lib` + `qor_mime.exp`
  (deps rebuilt as needed: qor_module, qor_host, qor_current, qor_sync, qor_signals, qor_pipeline). No errors/warnings.
- **Build test exe**: `cmake --build build --target QORTestMIME` → SUCCESS — `test_main.cpp` + `test_mime.cpp`,
  links `qor_mime.lib`; first-time builds of `qor_stringsource`/`qor_stringsink` for a test exe.
  Output `build\test\framework\data\mime\Debug\QORTestMIME.exe`.
- **Run test**: `& QORTestMIME.exe` → `test_mime.cpp(15) : mime_module_loads`, `[TOTAL](1/1)`, `[OK](1/1)`,
  `[DURATION] 0 seconds`, `$LASTEXITCODE = 0` → PASS.
- **Note**: `cmake --build build --target X -- -j4` FAILS on this VS2026/MSBuild generator
  (`MSB1001: Unknown switch`); omit `-j4`. Module needed `qor_pp_export void init_mime(void)` so an
  import lib exists (else test exes fail LNK1104 — learned from qor_renderer/qor_workflow).

### 2026-01-XX — qor_module (configure + build) — PASS
- **Configure**: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug` → SUCCESS (5.4s configure, 2.9s generate)
  - Platform: Windows, MSVC 19.50.35721.0, VS 2026 (Visual Studio 18 2026 generator)
  - Threads: TRUE (pthread not found on Windows; CMake Threads detected)
  - Build dir: C:/RAID0/Develop/linuxQOR/build
- **Build**: `cmake --build build --target qor_module` → SUCCESS
  - Compiled: library.cpp, module.cpp
  - Output: `build\src\qor\module\Debug\qor_module.lib`
  - No errors, no warnings
- **Verdict**: PASS — qor_module builds clean on MSVC/Windows.
- **Note**: Windows/MSVC toolchain confirmed. pthread absent (expected on Windows).
