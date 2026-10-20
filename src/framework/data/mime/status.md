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

## Next increment
- [ ] Step 2 `mime_captures_leading_octets` — `GetSignatureByteCount() == 16` for a 40 byte
      input, `== 4` for a 4 byte input; code added: `Inspect()` copies available octets into
      `m_signature` without consuming the source
