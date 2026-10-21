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

## Next increment
- [ ] Step 9 `mime_recognises_html_signature` — `<!DOCTYPE HTML` case-insensitive, or the first `<`
      starts a tag in {html,head,body,title} → `text/html`; write the test FIRST, then the html guard
      in `Inspect()` (name table already complete from step 3).

