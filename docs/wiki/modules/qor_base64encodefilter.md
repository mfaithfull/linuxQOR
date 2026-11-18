---
Type: WikiPage
PageType: module
Title: "qor_base64encodefilter"
Target: qor_base64encodefilter
Status: current
LastVerified: 2026-11-18
Claims: 14
Summary: "The encode twin of the decode filter is a different animal: an InlineFilter buffer, not a pipeline Element, with a bit-stuffing encoder that pads per call; it is the only one of the two with a live executable test - the MIME chain case - whose RED phase was a 18-of-24 truncation."
Anchors:
  - { kind: in-tree, path: src/components/data/pipeline/filters/base64encodefilter, symbol: qor_base64encodefilter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/filters/base64encodefilter/CMakeLists.txt, symbol: qor_base64encodefilter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/filters/base64encodefilter/base64encodefilter.h, symbol: Base64EncodeFilter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/filters/base64encodefilter/base64encodefilter.cpp, symbol: base64_chars, verified: 2026-11-18 }
  - { kind: in-tree, path: test/framework/data/mime/test_mime.cpp, symbol: mime_recogniser_chained_with_qor_base64encodefilter, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_base64encodefilter --parallel 8
---

# qor_base64encodefilter

"Base64 encode filter for Pipeline" with status column "Stable" (`FEATURES.md:50`); like its twin
it shares the purposes row "pipeline filters (inline buffer+function)"
(`docs/agent/purposes.md:90`). The whole target is 4 files and 109 lines - 13-line
`CMakeLists.txt`, 24-line header, 57-line filter, 15-line registration - configured at
`src/components/data/pipeline/filters/CMakeLists.txt:2`.

- The build file is the decode leaf's twin down to the byte: `add_library` SHARED names 2 `.cpp`
  (`:3`), `QOR_BASE64ENCODE` is PRIVATE (`:8`), and the link line is the same 3 libraries
  (`:13` `qor_module qor_signals qor_pipeline`, identical to the decode filter's `:13`).
  `repo_map.md:86`, `deps.dot:107` and `FEATURES.md:50` all agree with it.
- The class is not a pipeline Element: it derives from `InlineFilter<byte>`
  (`base64encodefilter.h:11`, base from `src/framework/data/pipeline/inlinefilter.h` included at
  `:7`) - in a Pipeline it acts as the Buffer shared by the elements either side of it
  (`test/framework/data/mime/test_mime.cpp:1277-1279`), where the decode filter is a `Filter`
  element.
- The export style differs too: the class carries `qor_pp_module_interface_gcc`
  (header `:11`) and all four members carry `qor_pp_module_interface` individually (`:15-18`) -
  five marker sites against the decode filter's one.
- Consumers: 9 targets link it - `QORTest` (`test/CMakeLists.txt:95`, `:106`), `QOROpTests`
  Windows branch only (`test/operational/CMakeLists.txt:34`), `QORTestMIME`
  (`test/framework/data/mime/CMakeLists.txt:1`, link at `:15`) and six examples - including
  `examples/basic/plumbing/CMakeLists.txt:22`, which links this filter and not the decode one.
  No library under `src/` links it.
- The encoder is bit-stuffing, not table math: a static alphabet (`base64encodefilter.cpp:21-24`),
  a shifting accumulator (`:30-43`) capped to `(unitsToProcess + 2) / 3 * 4` and then to the sink's
  `writeCount` (`:28-29`), and a per-call `=` pad to a 4-multiple (`:49-52`). The default
  constructor builds a zero-item filter (`:9-11`) - "its default ctor holds zero items and encodes
  nothing" (`docs/agent/verification.md:126`) - and a capacity constructor exists (`:13-15`).
- It has the one live executable test among the two Base64 filters:
  `mime_recogniser_chained_with_qor_base64encodefilter` (`test_mime.cpp:1292`) pumps 18 raw
  octets and asserts the exact 24-character base64 string (`:1339`) plus the recognizer's
  pre-encoding classification (`:1343-1345`).
- The test carries the filter's contract as prose (`test_mime.cpp:1275-1291`): the framework
  inserts it at the `InsertInlineFilter` BeforeSink position (`:1279-1282`),
  `InlineFilter::WriteAcknowledge` rewrites the item count to the encoded count (`:1283-1285`),
  and "it pads its output per call, so the stream must arrive in whole chunks whose length is a
  multiple of three" (`:1286-1287`).
- That contract was earned by a failure: `docs/agent/verification.md:120-135` records the
  22 Oct 2026 RED→GREEN for exactly this case - RED: "the sink received only the FIRST 18 of the
  24 encoded characters, i.e. the stream was truncated at the raw input length" (`:135`), GREEN:
  17/17 (`:134`); the same doc logs an earlier `LNK2019` for `Base64EncodeFilter` against a stale
  build directory (`:119`).
- The judge build is a fresh compile with nothing to complain about: exit 0, both `.cpp`
  compiled, zero warnings - unlike the decode filter's standing C4244 - and the dll (84,480
  bytes) and `.exp` (1,771) re-linked at 18 Nov 2026 09:00:14 over a `.lib` still stamped
  24 Oct 2026.

## Failure modes

- Per-call padding (`base64encodefilter.cpp:49-52`) makes chunking part of the contract: any
  pumped chunk whose length is not a multiple of 3 injects `=` mid-stream, and the pipeline
  enforces nothing - the only guard is prose in the test comment (`test_mime.cpp:1286-1287`),
  and `QORTestMIME` exercises just one aligned chunk (`:1318-1336`).
- The default constructor is a silent no-op (`base64encodefilter.cpp:9-11`): a filter
  default-constructed into a pipeline encodes nothing and acknowledges success; only the test's
  explicit capacity (`test_mime.cpp:1310`) avoids it.
- Both pipeline test cases for the Base64 pair are commented out - encode inside
  `/* :244 ... */ :278` in `test/framework/test_pipeline.cpp`, decode inside `:306-352` - so the
  filter's whole executable coverage is the single MIME chain case; its RED phase
  (`verification.md:135`) shows the truncation failure mode the commented cases were meant to
  catch.
- The truncation fix lives outside this dll: the cap `output_space = std::min(output_space,
  writeCount)` (`:29`) can emit a partial quartet when the sink buffer is short, and what makes
  the chain work is the framework's `InlineFilter::WriteAcknowledge` count rewrite
  (`test_mime.cpp:1283-1285`) - a future change in `qor_pipeline` can silently undo this
  target's only passing test.
- `FEATURES.md:50` again badges "Stable" over a library with zero `src/` consumers, a dead
  pipeline test and a Windows-only `QOROpTests` link (`test/operational/CMakeLists.txt:34`) -
  the same unreconciled vocabulary as the decode twin's row.

## See also

[[qor_module]] [[qor_signals]] [[qor_pipeline]] [[qor_base64decodefilter]] [[qor_jsondata]]

Not yet paged: the `InlineFilter<byte>` base it derives from
(`src/framework/data/pipeline/inlinefilter.h`, paged in scope of `qor_pipeline`), the MIME test
executable `QORTestMIME` and its subject `qor_mime` (named uncovered in `ComponentsLibPages`),
and the sibling `unicodetextfilter`.
