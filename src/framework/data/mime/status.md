# status.md — qor_mime (volatile development state)

## Outline created 20 Oct 2026
- `CMakeLists.txt` — `qor_mime` SHARED, `QOR_MIME`, links `qor_module qor_host qor_current qor_sync qor_signals qor_pipeline`
- `qor_mime.cpp` — mandatory module singleton (`ThisModule()`)
- `mime.h` — `qor::mime::MIMERecognizerFilter : qor::pipeline::Filter` outline API + `MIMEType` enum (ten types + `OctetStream` fallback + `Unresolved`)
- `mime.cpp` — `qor_pp_export void init_mime(void)` (Windows import lib guard) + placeholder `Inspect()` (pass through, no signature capture yet)
- test target `QORTestMIME` in `test/framework/data/mime` — placeholder case `mime_module_loads`

## Increments done (see memory-bank/qor_mime_plan.md for the full plan)
- [x] Step 0 scaffold: `qor_mime` builds, `QORTestMIME` builds and runs green `[OK](1/1)` exit 0
- [x] Step 1 `mime_filter_passes_octets_through`: test written FIRST and seen RED
      (`[FAIL] #1`, exit 1 — the base `Filter` does not pass octets through by itself), then
      `TransferOctets()` (buffer→buffer move like `CopyFilter::Copy`, declared in `mime.h`,
      called from `ReadFilter`/`WriteFilter`) made it GREEN. Verified 20 Oct 2026:
      `QORTestMIME.exe` `[TOTAL](2/2) [OK](2/2)` exit 0 (logged `docs/agent/verification.md`).
      Wiring note: the test pumps in `Element::FlowMode::Push` under a `Pipeline` parent —
      Push is the working pump path for a filter; Pull mode hits a base-framework bug
      (`Filter::Read` calls `ReadFilter(unitsRead, unitsRead)`, i.e. 0 units at entry).
- [x] Step 2 `mime_captures_leading_octets`: test written FIRST and seen RED
      (`[FAIL] #2`, `[OK](2/3)`, exit 1 — placeholder `Inspect()` captured nothing), then
      `Inspect()` implemented in `mime.cpp` using the pipeline buffer three-step read protocol
      (`buffer.h`/`buffer.cpp`): `ReadRequest` on the source buffer → `memcpy` into `m_signature`
      → **`ReadReject`** to give the reservation back, so the source is never consumed; capture
      capped by `SIGNATURE_WINDOW_BYTES` (16) and `ReadCapacity`. `ReadFilter`/`WriteFilter`
      call `Inspect()` BEFORE `TransferOctets()`. `mime.h` unchanged. Verified 20 Oct 2026:
      `QORTestMIME.exe` `[TOTAL](3/3) [OK](3/3)` exit 0 from
      `build\test\framework\data\mime\Debug` (logs `tmp/qortestmime_step2_red.log`,
      `tmp/qortestmime_step2_green.log`). `GetDetectedType()`/`GetTypeName()` still untouched.
- [x] Step 3 `mime_recognises_png_signature`: test written FIRST and seen RED
      (`[FAIL] #3`, `[OK](3/4)`, exit 1 — pass-through assert passed, `GetDetectedType()` still
      `Unresolved`), then in `mime.cpp` a fixed-order signature table inside `Inspect()` (after each
      capture, guard `m_detectedType == Unresolved && m_signatureCount >= 8`, `memcmp` first 8 of
      `m_signature` vs the PNG header `89 50 4E 47 0D 0A 1A 0A` → `m_detectedType = MIMEType::PNG`)
      plus the complete registered-name switch in `GetTypeName()` (`mime.h` unchanged). Detection lives
      in `Inspect()` (the non-consuming capture hook) so it re-evaluates per chunk. Verified 20 Oct 2026:
      `QORTestMIME.exe` `[TOTAL](4/4) [OK](4/4)` exit 0 from `build\test\framework\data\mime\Debug`.

- [x] Step 4 `mime_recognises_jpeg_signature`: **verified 20 Oct 2026** — the test AND the jpeg
      detector were already in the working tree (uncommitted) but had never been built or run, so
      this increment was verified after the fact rather than driven red-then-green; its RED phase
      was NOT observed. `cmake --build build --target qor_mime` + `--target QORTestMIME` → SUCCESS,
      `QORTestMIME.exe` from `build\test\framework\data\mime\Debug` → `[TOTAL](5/5) [OK](5/5)` exit 0
      (log `tmp/qortestmime_step4_green.log`). Detector = guard `m_signatureCount >= 3` + `memcmp`
      vs `FF D8 FF` in `Inspect()`; `mime.h` unchanged.
- [x] Step 5 `mime_recognises_gif_signature`: test written FIRST (two streams, fresh elements each,
      `GIF89a` and `GIF87a`) and seen RED — `[FAIL] #5`, `[TOTAL](6/6) [OK](5/6) [FAIL](1/6)`, exit 1
      (log `tmp/qortestmime_step5_red.log`), then `Inspect()` gained a third fixed-order guard
      (`m_signatureCount >= 6`, `memcmp` vs the six-byte GIF89a header OR the GIF87a header →
      `MIMEType::GIF`). Rebuilt `qor_mime` + `QORTestMIME` → `[TOTAL](6/6) [OK](6/6)`, no `[FAIL`,
      exit 0 (log `tmp/qortestmime_step5_green.log`). `mime.h` unchanged.
      Build gotcha hit while doing this: two `cmake --build` calls issued in the same batch run
      concurrently and race on the shared `.pdb` (`C1090` / `C1041 … use /FS`) — one cmake build
      per command, strictly sequential.

- [x] Step 6 `mime_recognises_pdf_signature`: test written FIRST (one stream, `%PDF-1.4\n` + body,
      asserts pass-through, `== MIMEType::PDF`, `"application/pdf"`) and seen RED — `[FAIL] #6`,
      `[TOTAL](7/7) [OK](6/7) [FAIL](1/7)`, exit 1 (log `tmp/qortestmime_step6_red.log`), then `Inspect()`
      gained a fourth fixed-order guard (`m_detectedType == Unresolved && m_signatureCount >= 4`,
      `memcmp` vs `{0x25,0x50,0x44,0x46}` = `%PDF` → `MIMEType::PDF`). `mime.h` unchanged. Rebuilt
      `qor_mime` then `QORTestMIME` (separate commands) → `[TOTAL](7/7) [OK](7/7)`, no `[FAIL`,
      exit 0 (log `tmp/qortestmime_step6_green.log`). 4 of 10 target types recognised.

- [x] Step 7 `mime_recognises_zip_signature`: test written FIRST (one stream, 4-byte `PK 03 04`
      local file header magic as raw octets (`"PK\x03\x04"`) + body, asserts pass-through,
      `== MIMEType::ZIP`, `"application/zip"`) and seen RED — `[FAIL] #7`, `[TOTAL](8/8) [OK](7/8)
      [FAIL](1/8)`, exit 1 (log `tmp/qortestmime_step7_red.log`), then `Inspect()` gained a fifth
      fixed-order guard (`m_detectedType == Unresolved && m_signatureCount >= 4`, `memcmp` vs
      `{0x50,0x4B,0x03,0x04}` = `PK 03 04` → `MIMEType::ZIP`). `mime.h` unchanged. Rebuilt
      `qor_mime` then `QORTestMIME` (separate commands) → `[TOTAL](8/8) [OK](8/8)`, no `[FAIL`,
      exit 0 (log `tmp/qortestmime_step7_green.log`). 5 of 10 target types recognised.

- [x] Step 8 `mime_recognises_xml_signature`: test written FIRST (one Push-mode stream, the five
      character XML declaration marker `<?xml version="1.0"?>\n` + short body, asserts pass-through,
      `== MIMEType::XML`, `"application/xml"`) and seen RED — `[FAIL] #8`, `[TOTAL](9/9) [OK](8/9)
      [FAIL](1/9)`, exit 1 (log `tmp/qortestmime_step8_red.log`), then `Inspect()` gained a sixth
      fixed-order guard (`m_detectedType == Unresolved && m_signatureCount >= 5`, `memcmp` vs
      `{0x3C,0x3F,0x78,0x6D,0x6C}` = `<?xml` → `MIMEType::XML`). No binary magic exists for XML, so
      this is the first strong text heuristic. `mime.h` unchanged. Rebuilt `qor_mime` then
      `QORTestMIME` (separate commands) → `[TOTAL](9/9) [OK](9/9)`, no `[FAIL`, exit 0 (log
      `tmp/qortestmime_step8_green.log`). 6 of 10 target types recognised.

- [x] Step 9 `mime_recognises_html_signature`: test written FIRST (TWO Push-mode streams in the one
      case, fresh elements for the second — mirroring step 5's two GIF variants: the lower case
      HTML5 doctype `<!DOCTYPE html>\n<html><head>...` and the no-doctype upper case document-level
      tag `<HTML>\n<head>...`; per stream asserts pass-through, `== MIMEType::HTML`,
      `"text/html"`) and seen RED — `[FAIL] #9`, `[TOTAL](10/10) [OK](9/10) [FAIL](1/10)`, exit 1
      (log `tmp/qortestmime_step9_red.log`, reason `expected boolean to have the value of true /
      value was : 0`), then `Inspect()` gained a seventh fixed-order guard after the XML marker:
      a case-folded 14-octet `<!DOCTYPE HTML` compare and, while still unresolved, a document-level
      first-tag compare (`m_signature[0] == '<'` then case-folded `html`/`head`/`body`/`title`).
      HTML has no binary magic, so both are text heuristics; unlike the fixed magics above they
      cannot use `memcmp` because HTML markup spelling is case-insensitive, so `mime.cpp` gained a
      file-local `markupMatch()` ASCII case-fold compare helper (internal — no
      `qor_pp_module_interface`, no locale, no new dependency). `mime.h` unchanged. Rebuilt
      `qor_mime` then `QORTestMIME` (separate commands) → `[TOTAL](10/10) [OK](10/10)`, no
      `[FAIL`, exit 0 (logs `tmp/qortestmime_step9_build_lib.log`,
      `tmp/qortestmime_step9_build_exe.log`, `tmp/qortestmime_step9_green.log`). **7 of 10
      target types recognised** (PNG, JPEG, GIF, PDF, ZIP, XML, HTML).

- [x] Step 10 `mime_recognises_json_signature`: test written FIRST (TWO Push-mode streams in the
      one case, fresh elements for the second — the shape of steps 5 and 9: stream 1 a JSON object
      `{ "qor" : "mime test", "step" : 10 }` with no leading whitespace, stream 2 a JSON array
      preceded by JSON whitespace (three spaces, a line feed, a tab) so the skip-past-whitespace
      half of the heuristic is exercised; per stream asserts pass-through,
      `== MIMEType::JSON`, `"application/json"`) and seen RED — `mime_recognises_json_signature
      [FAIL], #10`, `[TOTAL](11/11) [OK](10/11) [FAIL](1/11)`, exit 1 (log
      `tmp/qortestmime_step10_red.log`, build log `tmp/qortestmime_step10_build_red.log`), then
      `Inspect()` gained an eighth fixed-order guard after the HTML markup guards: scan the captured
      window for the FIRST octet that is not JSON white space (0x20 space, 0x09 tab, 0x0A line feed,
      0x0D carriage return — the only four characters JSON allows as whitespace) and set
      `MIMEType::JSON` when that octet is `{` (0x7B) or `[` (0x5B), then stop the scan. JSON has no
      magic number, so this is a text heuristic evaluated after the HTML checks in the fixed order;
      unlike the markup compares above it is not case-relevant, so a plain octet compare is enough
      and `markupMatch()` is not used. It decides as soon as one non-whitespace octet is captured, so
      it does not need the window to be full. `mime.h` unchanged (name table complete since step 3).
      Rebuilt `qor_mime` then `QORTestMIME` (separate commands) → `[TOTAL](11/11) [OK](11/11)`, no
      `[FAIL`, exit 0 (logs `tmp/qortestmime_step10_build_lib.log`,
      `tmp/qortestmime_step10_build_exe.log`, `tmp/qortestmime_step10_green.log`). **8 of 10
      target types recognised** (PNG, JPEG, GIF, PDF, ZIP, XML, HTML, JSON).

- [x] Step 11 `mime_recognises_javascript_signature`: test written FIRST (THREE Push-mode streams in the one
      case, fresh elements per stream — the multi-stream shape of steps 5/9/10: stream 1 a standalone script
      leading with the shebang `#!` (0x23 0x21), stream 2 a source opening with a top-level `function `
      declaration, stream 3 a source opening with a block comment `/*`; per stream asserts pass-through,
      `== MIMEType::JavaScript`, `"application/javascript"`) and seen RED — `mime_recognises_javascript_signature
      [FAIL], #11`, `[TOTAL](12/12) [OK](11/12) [FAIL](1/12)`, exit 1 (log `tmp/qortestmime_step11_red.log`), then
      `Inspect()` gained a ninth fixed-order guard AFTER the JSON guard: (1) shebang (`m_signature[0]==0x23 &&
      m_signature[1]==0x21`), (2) a top-level `function ` declaration via a plain `memcmp` vs `{66 75 6E 63 74 69
      6F 6E 20}` — JavaScript is case-sensitive so `markupMatch()` is NOT used (a case-folded compare would wrongly
      accept `FUNCTION`), (3) a leading source comment `//` (0x2F 0x2F) or `/*` (0x2F 0x2A). JavaScript has no
      magic number, so this is a weak text heuristic evaluated after JSON in the fixed order; the arrow form `=>`
      is contemplated but not yet exercised by a test, so it is not implemented. `mime.h` unchanged (name table
      complete since step 3). Rebuilt `qor_mime` then `QORTestMIME` (separate commands) → `[TOTAL](12/12) [OK](12/12)`,
      no `[FAIL`, exit 0 (logs `tmp/qortestmime_step11_build_lib.log`, `tmp/qortestmime_step11_build_exe.log`,
      `tmp/qortestmime_step11_green.log`). **9 of 10 target types recognised** (PNG, JPEG, GIF, PDF, ZIP, XML, HTML,
      JSON, JavaScript). No new gotchas — one-cmake-build-per-command and build-then-run-separately both held.

- [x] Step 12 `mime_recognises_plain_text_signature`: test written FIRST (TWO Push-mode streams in the one
      case, fresh elements for the second — the shape of steps 5/9/10/11: stream 1 an ASCII-only prose
      document longer than the sixteen byte window (letters, digits, punctuation, spaces, a tab, line feeds),
      stream 2 a document carrying explicit UTF-8 octets (a two byte sequence C3 A9 and a three byte sequence
      E2 82 AC) so the stream does not depend on this file's encoding; per stream asserts pass-through,
      `== MIMEType::PlainText`, `"text/plain"`) and seen RED — `mime_recognises_plain_text_signature
      [FAIL], #12`, `[TOTAL](13/13) [OK](12/13) [FAIL](1/13)`, exit 1 (log `tmp/qortestmime_step12_red.log`,
      reason `expected boolean to have the value of true / value was : 0`), then a tenth fixed-order guard in
      `Inspect()` AFTER the JavaScript guards: requires the WHOLE window captured (`m_signatureCount >=
      SIGNATURE_WINDOW_BYTES`) and every captured octet printable — 0x80-0xFF (UTF-8 encoded text), 0x20-0x7E
      (printable ASCII) or 0x09/0x0A/0x0D (tab, line feed, carriage return); any other control octet (NUL, the
      other C0 controls, DEL) leaves the stream unresolved for the fallback. Well formed UTF-8 is deliberately
      not validated — that belongs to the Unicode layer, which this framework layer module must not depend on.
      `mime.h` unchanged. Rebuilt `qor_mime` then `QORTestMIME` (separate commands) → `[TOTAL](13/13) [OK](13/13)`,
      no `[FAIL`, exit 0 (logs `tmp/qortestmime_step12_build_lib.log`, `tmp/qortestmime_step12_build_exe.log`,
      `tmp/qortestmime_step12_green.log`). **10 of 10 target types recognised** (PNG, JPEG, GIF, PDF, ZIP,
      XML, HTML, JSON, JavaScript, plain text).

- [x] Step 13 `mime_falls_back_to_octet_stream`: test written FIRST (TWO Push-mode streams in the one
      case, fresh elements for the second — the shape of steps 5/9/10/11/12: stream 1 a raw binary blob
      opening with a NUL and carrying the C0 controls and DEL written as explicit octets, followed by a
      printable payload tail; stream 2 the leading octets of a Windows executable image — the printable
      'M' 'Z' DOS prefix, the padding byte 0x90 and NUL offset fields, then a payload tail; per stream
      asserts pass-through, `GetSignatureByteCount() == 16`, `== MIMEType::OctetStream`,
      `"application/octet-stream"`) and seen RED — `mime_falls_back_to_octet_stream [FAIL], #13`,
      `[TOTAL](14/14) [OK](13/14) [FAIL](1/14)`, exit 1 (log `tmp/qortestmime_step13_red.log`, reason
      `expected boolean to have the value of true / value was : 0`), then an eleventh and last fixed-order
      guard in `Inspect()` AFTER the plain text guard: `m_detectedType == MIMEType::Unresolved &&
      m_signatureCount >= SIGNATURE_WINDOW_BYTES` → `MIMEType::OctetStream`. It is the only guard with no
      signature of its own — it fires when every guard above has declined, and like the plain text guard
      it insists on a full window, so a stream shorter than the window stays `Unresolved` until the octets
      that decide it arrive. This makes the recognizer total: a stream pumped in full always ends with a
      type and a non-empty name. `mime.h` unchanged (the name table already carried
      `application/octet-stream`). Rebuilt `qor_mime` then `QORTestMIME` (separate commands) →
      `[TOTAL](14/14) [OK](14/14)`, no `[FAIL`, exit 0 (logs `tmp/qortestmime_step13_build_lib.log`,
      `tmp/qortestmime_step13_build_exe.log`, `tmp/qortestmime_step13_green.log`). **All 10 target types
      plus the fallback recognised.**

- [x] Step 14 `mime_recognises_type_split_across_chunks`: test written FIRST (the PNG stream of step 3
      pumped three octets at a time — the eight byte PNG magic is split across three pumped chunks and
      no chunk carries a complete signature; asserts pass-through, `GetSignatureByteCount() == 16`,
      `== MIMEType::PNG`, `"image/png"`) and GREEN on the FIRST run — `[TOTAL](15/15) [OK](15/15)`,
      exit 0 (logs `tmp/qortestmime_step14_build_exe.log`, `tmp/qortestmime_step14_green.log`). NO
      implementation change was needed or made: `m_signatureCount` is a member of the filter and
      `Inspect()` runs again on every pumped chunk, so the window accumulates across chunk
      boundaries by construction. The case proves that claim — had the window not accumulated across
      chunks, the 16-octet count assert would fail. `mime.h` and `mime.cpp` unchanged; only
      `test_mime.cpp` recompiled.

- [x] Step 15 `mime_recognizer_resets_between_streams`: test written FIRST (ONE recognizer serving
      TWO streams, fresh elements for the second — the shape of steps 5/9/10/11/12/13: stream 1 the
      PNG stream of step 3, so the state being cleared by the `Reset()` is a KNOWN type rather than the
      initial state; after it the cleared state is asserted — `GetDetectedType() == MIMEType::Unresolved`,
      `GetTypeName() == ""` and `GetSignatureByteCount() == 0`; stream 2 the PDF stream of step 6 pushed
      through the SAME recognizer re-wired to the fresh elements with `Filter::SetSource`/`Filter::SetSink`,
      asserting pass-through, `GetSignatureByteCount() == 16`, `== MIMEType::PDF`, `"application/pdf"`) and
      GREEN on the FIRST run — `[TOTAL](16/16) [OK](16/16)`, no `[FAIL`, exit 0 (logs
      `tmp/qortestmime_step15_build_exe.log`, `tmp/qortestmime_step15_green.log`). NO implementation change
      was needed or made: `Reset()` already zeroes `m_signatureCount` and restores `m_detectedType` to
      `MIMEType::Unresolved`, so the case verifies existing behaviour — and it is meaningful, because had
      `Reset()` not cleared the type, stream 2 would still report `image/png`, and had it not cleared the
      count, the 16-octet count assert after stream 2 would fail. `mime.h` and `mime.cpp` unchanged; only
      `test_mime.cpp` recompiled. **Learning:** re-wiring a filter to fresh elements must go through the
      filter's own `SetSource`/`SetSink` — the `Element` accessors refuse to re-link an element that already
      has a source or sink, so a filter is reusable by re-linking IT, not by re-linking its plumbing.

## Next increment
- [ ] Step 16 `mime_recogniser_chained_with_qor_base64encodefilter` (two filters in series — the recognizer
      classifies the stream and `qor_base64encodefilter` encodes it; see
      `src/components/data/pipeline/filters/base64encodefilter`). Optional scope: `mime_type_name_drives_a_sink`.

