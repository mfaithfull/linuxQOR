# qor_mime plan — test-first MIME recognition pipeline filter

Task: add a `qor_mime` module under `src/framework/data` (own folder) providing a
pipeline filter that recognises ten popular MIME types. Approach: **test first** —
each increment is a failing test made green by the smallest piece of code, one at a
time. Outline created 20 Oct 2026, verified green.

## Step R — research: how QOR pipeline filters are built and tested (DONE, before any code)

Read for the pattern (`docs/agent/add_module.md` + these files):
- `src/framework/data/pipeline/purpose.md` — pipeline = `source -> (filter)* -> sink`;
  data flows source to sink. A **filter is both a Sink and a Source** so it can both
  accept and pass on data. This is why the recognizer can sit anywhere in the chain.
- `src/framework/data/pipeline/filter.h`/`filter.cpp` — framework base
  `qor::pipeline::Filter : Sink, Source`. Ctor order matters: `Sink(source), Source(sink)`
  — the sink is built from the *source* argument. Overrides Read/Write/ReadFilter/
  WriteFilter/Name (Name() returns "Filter").
- `src/framework/data/pipeline/direct.h` — `DirectConnectFilter<B,Derived>` — only for
  zero-copy type identity; not used here and discouraged ("not to be used lightly").
- `inlinefilter.h` — `InlineFilter<B,Derived>` — buffer transform helper; base64decodefilter
  and unicodetextfilter derive from it. Not needed for a read-only recognizer.
- Existing filters to copy from:
  `src/components/data/pipeline/filters/base64encodefilter/{base64encodefilter.h,base64encodefilter.cpp,CMakeLists.txt,purpose.md}`
  (SHARED `qor_base64encodefilter`, `QOR_BASE64ENCODEFILTER`, ctor/dtor/operator=
  pattern, `qor_pp_module_interface(QOR_X)` on every exported member)
  `.../filters/linereader/{linereader.h,linereader.cpp}` — the `= default` ctor/dtor/
  operator= style and ReadFilter/WriteFilter overrides.
  `.../filters/jpegdecodefilter` and `unicodetextfilter` — same shape.
- How filters are tested: `test/framework/test_pipeline.cpp` (in the QORTest exe, uses
  `StringSource -> Base64EncodeFilter -> StringSink` and asserts round trip) and
  `test/components/data/pipeline/{test_main.cpp,test_bufferedsource.cpp}`.
- Dedicated per-module test exe to copy: `test/components/protocols/yaml/{CMakeLists.txt,test_main.cpp}`.

Findings that shape the module:
1. A filter is created by deriving from `qor::pipeline::Filter` and overriding
   `ReadFilter`/`WriteFilter` (`bool f(size_t& unitsProcessed, size_t unitsToProcess)`);
   returning false stops the pump. `Read()`/`Write()` handle "pull" mode.
2. A filter must not consume/ack source data it does not pass on, or the sink loses bytes.
3. A filter is compiled as its own SHARED lib (`set(target qor_X)`) and is registered in
   the parent CMakeLists; the test exe links the libs it needs and copies DLLs
   `$<TARGET_RUNTIME_DLLS:...>` into its own output folder (POST_BUILD copy, Windows).
4. Test exes run standalone from their build folder: output
   `<file>(<line>) : <case_name>` then `[TOTAL](n/n)` / `[OK](n/n)` and shell exit code
   is the pass/fail indicator (0 = all passed). The QOR does not use ctest (D8 in DECISIONS.md):
   no `add_test()`/`enable_testing()` is added to a QOR test CMakeLists.
   Test judge = the exe's [OK](n/n) output and its exit code.

## Design decisions taken with the outline

- Folder `src/framework/data/mime` (framework layer, as the user specified). Note the
  convention deviation: existing filters live in `src/components/data/pipeline/filters`
  (components layer). Kept framework layer per instruction; the class derives from the
  framework `qor::pipeline::Filter`, so this is defensible as the default filter for any
  pipeline. Recorded here so the choice is not accidental.
- Library `qor_mime`, define `QOR_MIME`, namespace `qor::mime` (the `framework/data` path
  is elided, as `qor::pipeline` and `qor::dio` do).
- Class `MIMERecognizerFilter : public qor::pipeline::Filter`. `qor::pipeline` is NOT
  re-entered inside the class body.
- `enum class MIMEType { Unresolved, OctetStream, PlainText, HTML, JSON, XML, JavaScript,
  PNG, JPEG, GIF, PDF, ZIP }` — the ten targets + the `application/octet-stream` fallback +
  the "nothing inspected yet" state.
- Recognition is exposed on the filter (`GetDetectedType()`, `GetTypeName()`,
  `GetSignatureByteCount()`, `Reset()`); the stream itself is never altered.
- Signature window: 16 leading bytes captured into the filter (`SIGNATURE_WINDOW_BYTES`).

## Signature / decision table for the ten types

| # | MIME type | how recognised | strength |
|---|---|---|---|
| 1 | `image/png` | magic `89 50 4E 47 0D 0A 1A 0A` (8 bytes) | definite |
| 2 | `image/jpeg` | magic `FF D8 FF` (3 bytes) | definite |
| 3 | `image/gif` | magic `GIF87a` or `GIF89a` (6 bytes) | definite |
| 4 | `application/pdf` | magic `%PDF` (4 bytes) | definite |
| 5 | `application/zip` | magic `PK 03 04` (4 bytes) | definite |
| 6 | `application/xml` | leading text `<?xml` (5 chars) | strong heuristic |
| 7 | `text/html` | `<!DOCTYPE HTML` case-insensitive, or first `<` starts tag in {html,head,body,title} | heuristic |
| 8 | `application/json` | first non-space char is `{` or `[` | heuristic |
| 9 | `application/javascript` | shebang `#!` or `/*`/`//` comment plus ECMA keywords, or `function `/`=>` | weak heuristic |
| 10 | `text/plain` | all inspected bytes printable ASCII/UTF-8, no markup markers | default for text |
| — | `application/octet-stream` | fallback when nothing above matches | fallback |

Fixed evaluation order (most definite first): png, jpeg, gif, pdf, zip, xml, html, json,
javascript, plain text, else octet-stream. Decided up front so results are deterministic
and each test is independent of the order the tests are written.

## Test-first increments — write the test, watch it fail, then write the code

Each increment = (a) new test case in `test/framework/data/mime/test_mime.cpp`, (b) build
`QORTestMIME` and see the new case FAIL (red), (c) smallest implementation change in
`mime.h`/`mime.cpp`, (d) rebuild + run, expect `[OK](n/n)` exit 0, (e) tick `status.md`.

| Step | Test case name | Asserts | Code added |
|---|---|---|---|
| 0 ✅ | `mime_module_loads` | filter constructs, type is `Unresolved` | scaffold — DONE, verified green |
| 1 ✅ | `mime_filter_passes_octets_through` | `StringSource -> MIMERecognizerFilter -> StringSink` output string == input string | DONE 20 Oct 2026 — the base `Filter` does NOT pass octets through by itself (placeholder `Inspect()` moved nothing, new case went `[FAIL] #1` exit 1), so the smallest fix is `TransferOctets()` (CopyFilter::Copy style buffer-to-buffer move) called from `ReadFilter`/`WriteFilter`. Verified `[OK](2/2)` exit 0. NOTE: Push mode is the working pump path for a filter; Pull mode hits a base-framework bug (`Filter::Read` calls `ReadFilter(unitsRead, unitsRead)` — 0 units at entry) |
| 2 | `mime_captures_leading_octets` | `GetSignatureByteCount() == 16` for a 40 byte input; `== 4` for a 4 byte input | `Inspect()` copies available bytes into `m_signature` without acking/consuming source |
| 3 | `mime_recognises_png_signature` | `GetDetectedType() == MIMEType::PNG`, `GetTypeName() == "image/png"` | png magic compare + name table |
| 4 | `mime_recognises_jpeg_signature` | `== JPEG`, `"image/jpeg"` | jpeg magic compare |
| 5 | `mime_recognises_gif_signature` | `== GIF`, `"image/gif"` for both `GIF89a` and `GIF87a` | gif magic compare (two variants) |
| 6 | `mime_recognises_pdf_signature` | `== PDF`, `"application/pdf"` | pdf magic compare |
| 7 | `mime_recognises_zip_signature` | `== ZIP`, `"application/zip"` | zip magic compare |
| 8 | `mime_recognises_xml_from_markup` | `== XML`, `"application/xml"` | `<?xml` text check |
| 9 | `mime_recognises_html_from_markup` | `== HTML`, `"text/html"` for `<!DOCTYPE html>...` and `<html>` | doctype/tag scan + case-insensitive compare helper |
| 10 | `mime_recognises_json_from_markup` | `== JSON`, `"application/json"` | first non-space `{`/`[` check |
| 11 | `mime_recognises_javascript_from_markup` | `== JavaScript`, `"application/javascript"` | shebang/comment + keyword heuristic |
| 12 | `mime_recognises_plain_text` | `== PlainText`, `"text/plain"` | printable-byte scan default |
| 13 | `mime_falls_back_to_octet_stream` | `== OctetStream`, `"application/octet-stream"` | fallback branch |
| 14 | `mime_recognises_type_split_across_chunks` | 3-byte pumped chunks, png header still `== PNG` | signature window already handles it — proves chunk boundary handling |
| 15 | `mime_recognizer_resets_between_streams` | after `Reset()` back to `Unresolved` and count 0, then re-recognise `== PDF` | `Reset()` (already drafted — verify) |
| 16 | `mime_recognizer_chained_with_base64_filter` | `StringSource -> MIMERecognizerFilter -> Base64EncodeFilter -> StringSink` output decodes to input and type still `image/png` | nothing in qor_mime — proves it composes as a real pipeline filter |
| 17 | `mime_type_name_drives_a_sink` | the recognized name selects downstream behaviour | application-level glue (optional scope) |

Stop conditions: 10 of 10 target types recognised in steps 3–12, fallback proven in 13,
pipeline contract proven in 1/2/14/15/16.

## Build + verify commands (Windows / MSVC / VS 2026 generator)

    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug          # reconfigure picks up the new subdirs
    cmake --build build --target qor_mime                 # lib = compiler as judge (D3)
    cmake --build build --target QORTestMIME              # test exe
    & build\test\framework\data\mime\Debug\QORTestMIME.exe ; $LASTEXITCODE   # expect [OK](n/n), exit 0
    # The QOR does not use ctest (D8). Exe output + exit code is the test judge; do not add
    # enable_testing()/add_test() to a QOR test CMakeLists.

Pitfalls confirmed while creating the outline:
- `cmake --build ... -- -j4` FAILS on this generator (`MSB1001: Unknown switch`). Use
  `cmake --build build --target qor_mime` (no `-j4` after `--`), or CMake's own `-j N`.
- A QOR module with no exported symbol makes a test exe fail with `LNK1104` (no import
  `.lib`). Guard: `qor_pp_export void init_mime(void)` in `mime.cpp`. Verified: the
  qor_mime link produced `qor_mime.lib` + `qor_mime.exp`.
- File-based test cases in this repo write relative to the **exe** (e.g. the JSON generator uses
  `constexpr const char* outputFolder = "../../../../../../../test/data";`). If a qor_mime step
  ever writes files, use the same convention and always run the exe from its build folder —
  running from the repo root gives false `[FAIL]`.
- `src/framework/data` and `src/components/data` layers are elided from namespace names:
  use `qor::mime`, and inside the class body write `qor::pipeline::...` not `qor::mime::pipeline::...`.
- `QOR_MIME` must be added as a `target_compile_definitions` PRIVATE define or every
  `qor_pp_module_interface(QOR_MIME)` in `mime.cpp` expands to nothing.
- The `test/framework/data/mime` test dir had to be registered in `test/CMakeLists.txt`
  (the whole `test/framework/data` tree was previously unbuilt — `qor_stringsource` and
  `qor_stringsink` had never been compiled for a test exe; they built fine when
  `QORTestMIME` pulled them in).

## Actual results so far

- Step 0 scaffold: `cmake --build build --target qor_mime` → `qor_mime.dll` + import lib,
  no errors/warnings. `QORTestMIME` → `QORTestMIME.exe`. Run →
  `test_mime.cpp(15) : mime_module_loads` `[TOTAL](1/1)` `[OK](1/1)` exit 0. PASS.

