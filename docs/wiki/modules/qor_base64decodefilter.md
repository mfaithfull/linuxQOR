---
Type: WikiPage
PageType: module
Title: "qor_base64decodefilter"
Target: qor_base64decodefilter
Status: current
LastVerified: 2026-11-18
Claims: 14
Summary: "A Stable-badged Base64 decoder in 4 files and 129 lines that decodes in both flow directions with a home-grown tolerant alphabet; no src/ library consumes it, its only test case is commented out, and the judge's compile printed a standing C4244."
Anchors:
  - { kind: in-tree, path: src/components/data/pipeline/filters/base64decodefilter, symbol: qor_base64decodefilter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/filters/base64decodefilter/CMakeLists.txt, symbol: qor_base64decodefilter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/filters/base64decodefilter/base64decodefilter.h, symbol: Base64DecodeFilter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/filters/base64decodefilter/base64decodefilter.cpp, symbol: pos_of_char, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/filters/base64decodefilter/base64decodefilter.cpp, symbol: Decode, verified: 2026-11-18 }
  - { kind: in-tree, path: test/framework/test_pipeline.cpp, symbol: Base64DecodeFilter, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_base64decodefilter --parallel 8
---

# qor_base64decodefilter

"Base64 decode filter for Pipeline" with status column "Stable" (`FEATURES.md:49`); the purposes
index shares one row across five filters: "pipeline filters (inline buffer+function)"
(`docs/agent/purposes.md:90`). The whole target is 4 files and 129 lines - a 13-line
`CMakeLists.txt`, a 26-line header, a 76-line filter and a 14-line registration file - and it is
configured at `src/components/data/pipeline/filters/CMakeLists.txt:1` of the five filter
sub-directories.

- `add_library` (`:3`) names 2 `.cpp`, the folder holds exactly those 2, and `cmakebuild` holds 2
  `.obj`; it builds SHARED with `QOR_BASE64DECODE` defined PRIVATE (`:8`) and links exactly 3
  libraries (`:13`): `qor_module qor_signals qor_pipeline`.
- Only its own `CMakeLists.txt` (`:1`, `:3`) names the target, but 8 other targets link it: the
  `QORTest` executable (`test/CMakeLists.txt:95` Linux, `:106` Windows) and `QOROpTests`
  (`test/operational/CMakeLists.txt:34` - Windows branch only; the Linux list at `:19-24` omits
  it) plus five examples (`examples/intermediate/server/CMakeLists.txt:24`,
  `experimental/resourced:23`, `experimental/eventer:22`, `experimental/curlike:16`,
  `experimental/hydra:28`). No library under `src/` links it - it is leaf-level plumbing.
- The three dependency indexes agree with the build 3-for-3: `repo_map.md:85`, `deps.dot:106` and
  `deps.svg:1454-1468` all print exactly the 3 libraries of `CMakeLists.txt:13` - no drift like
  `qor_jsondata`'s.
- One exported class: `Base64DecodeFilter : public Filter`
  (`base64decodefilter.h:11`, base from `src/framework/data/pipeline/filter.h` included at `:7`),
  marked `qor_pp_module_interface(QOR_BASE64DECODE)` - a macro that is not the target name,
  defined only by this `CMakeLists.txt:8`. The registration TU is the new short form: 14 lines,
  SPDX header, `ThisModule()` at `qor_base64decodefilter.cpp:10-14` naming
  "Base64 Decode Filter Module" (`:12`).
- The filter decodes in both flow directions: `ReadFilter` (`base64decodefilter.cpp:12-15`) and
  `WriteFilter` (`:17-20`) are one-liners that both call `Decode`.
- The alphabet is home-grown and deliberately liberal: `pos_of_char` (`:22-34`) maps `-` to 62
  alongside `+` (`:27`) and `_` to 63 alongside `/` (`:28`), and an unrecognised byte falls
  through to `return 0` with the comment "TODO: Error not base64encoded" (`:29-33`).
- `Decode` (`:36-74`) emits 3 bytes per 4 input (`:43`), moving data through pipeline buffer
  `ReadRequest`/`ReadAcknowledge` (`:42`, `:69`) and `WriteRequest`/`WriteAcknowledge` (`:44`,
  `:68`); it returns false only when a buffer is null or `unitsToProcess` is 0 (`:40`, `:73`) -
  there is no error channel for bad content.
- The judge build is a fresh compile: exit 0, both `.cpp` compiled, the dll and `.exp`
  (89,088 and 4,191 bytes) re-linked at 18 Nov 2026 08:56, while the `.lib` stamp is still
  24 Oct 2026 - and the compile printed one standing warning, C4244 at
  `base64decodefilter.cpp:62`, `unsigned int` narrowed to `qor::byte`.
- The only test case named for this filter is dead code: `testBase64DecodeFilter`
  (`test/framework/test_pipeline.cpp:307`) sits inside a `/* :306 ... */ :352` comment block,
  assertion included (`:350`); the sibling encode case is equally dead in its own block
  (`/* :244 ... */ :278`), and the file still includes this filter's header (`:37`) - which is
  why `QORTest` links the dll at all.

## Failure modes

- Invalid input decodes silently as `A`-padding: every unrecognised byte maps to 0 (`:29-33`),
  the TODO at `:31` admits the gap, and `Decode`'s bool contract (`:40`, `:73`) cannot report
  content errors - downstream bytes are simply wrong.
- `.` as pad belongs to no Base64 variant: `:53-54` ends a triplet on both `=` and `.` while
  citing RFC 2045, yet the URL-safe variant it also claims is spelled `-`/`_` (`:27-28`) -
  a `.` inside a group truncates that triplet and misaligns everything after it.
- Chunk seams are unguarded by design: the loop only bounds reads to the current
  `ReadRequest` window (`:47`, `:53`, `:60`), so a 4-character group split across two pump calls
  decodes as two partial groups; the one test that could catch this is the commented-out case
  (`test_pipeline.cpp:306-352`).
- The C4244 at `:62` is permanent evidence of an unreviewed line: the first output byte is
  explicitly `static_cast<byte>` (`:50`) but the third (`:62`) is not, and the same build that
  linked the dll printed the warning.
- The `FEATURES.md:49` "Stable" badge is reconciled by nothing: the filter has zero `src/`
  consumers, its only test case is commented out, and `QOROpTests` links it only on Windows
  (`test/operational/CMakeLists.txt:34` vs `:19-24`) - no instrument in this wiki notices any of
  the three.

## See also

[[qor_module]] [[qor_signals]] [[qor_pipeline]] [[qor_jsondata]] [[qor_parser]]

Not yet paged: its encode twin `qor_base64encodefilter` (leaf 5 here), the other pipeline filters
under `src/components/data/pipeline/filters` - `jpegdecodefilter`, `linereader`,
`unicodetextfilter` - and the pipeline elements it derives from (`Filter` itself lives in
`src/framework/data/pipeline/filter.h`).
