---
Type: WikiPage
PageType: module
Title: "qor_jpegdecodefilter"
Target: qor_jpegdecodefilter
Status: current
LastVerified: 2026-11-18
Claims: 14
Summary: "A vendored stb_image JPEG core in a pipeline Filter shell that decodes only the header: Decode parses the header then discards the result, ReadFilter reads nothing and claims one unit, and the single consumer example is commented out of the build with a wrong include path."
Anchors:
  - { kind: in-tree, path: src/components/data/pipeline/filters/jpegdecodefilter, symbol: qor_jpegdecodefilter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/filters/jpegdecodefilter/CMakeLists.txt, symbol: qor_jpegdecodefilter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/filters/jpegdecodefilter/jpegdecodefilter.h, symbol: JPEGDecodeFilter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/filters/jpegdecodefilter/jpegdecodefilter.cpp, symbol: JPEGDecodeFilter::Decode, verified: 2026-11-18 }
  - { kind: in-tree, path: examples/experimental/images/CMakeLists.txt, symbol: qor_jpegdecodefilter, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_jpegdecodefilter --parallel 8
---

# qor_jpegdecodefilter

`FEATURES.md:102` badges it "Experimental, Incomplete" - the most honest status in the pipeline
filter family, whose base64 twins claim "Stable" at `:49-50`. It shares the purposes row
"pipeline filters (inline buffer+function)" with all four filter siblings
(`docs/agent/purposes.md:90`) and is configured at `src/components/data/pipeline/filters/CMakeLists.txt:4`.
The target is 4 files, 1,009 lines: 13-line `CMakeLists.txt`, 152-line header, 808-line filter,
36-line `ThisModule()` bootstrap.

- The 808-line `.cpp` is a vendored subset of the public-domain stb_image JPEG decoder: `stbi_*`
  types and helpers throughout (`stbi__context` at `jpegdecodefilter.h:50-65`, `stbi__jpeg` at
  `:78-130`, `stbi__jpeg_dezigzag` at `jpegdecodefilter.cpp:104`, `stbi__idct_block` at `:159`,
  `stbi__build_huffman` at `:338`). The stb entry points that would drive a full decode -
  `stbi__jpeg_test`, `stbi__jpeg_load`, `stbi__jpeg_info` - appear only as prototypes inside a
  comment block (`:91-93`) and have no bodies anywhere in the file.
- The filter enters through `WriteFilter`, which delegates straight to `Decode`
  (`jpegdecodefilter.h:141-144`). `Decode` (`jpegdecodefilter.cpp:785-799`) builds a
  stack-local `stbi__jpeg`, parses only the header via
  `stbi__decode_jpeg_header(&j, STBI__SCAN_header)` (`:795`), stores the outcome in `r`, never
  inspects it, and returns `true` unconditionally (`:798`). The lines that would attach pipeline
  bytes to the decoder buffer are commented out (`:790-791`).
- `ReadFilter` (`:801-806`) consumes nothing: it sets `unitsProcessed = 1` under the comment
  "We read 1 JPEG" (`:804`) and returns true.
- One class, one marker: `qor_pp_module_interface(QOR_JPEGDECODE) JPEGDecodeFilter : public
  qor::pipeline::Filter` at `jpegdecodefilter.h:132`; no gcc/inline marker variants anywhere (the
  encode twin has 5 marker sites). `QOR_JPEGDECODE` is `PRIVATE` in the build file (`:8`) and does
  not match the target name.
- Build file shape matches the base64 twins: SHARED library naming 2 `.cpp` (`:3`), version-macro
  definitions (`:8-12`), and exactly 3 link libraries `qor_module qor_signals qor_pipeline`
  (`:13`) - matching `docs/agent/repo_map.md:87` and the `deps.dot` node and edge (`:40`, `:108`).
- The only target that links it is the `images` example (`examples/experimental/images/CMakeLists.txt:16`,
  Windows branch `:24`) - and that subdirectory is commented out of the tree
  (`examples/experimental/CMakeLists.txt:9`), so as of 2026-11-18 no built artifact links or
  exercises this library; there is no test anywhere.
- The example would not compile if re-enabled: it includes
  `src/components/io/pipeline/filters/jpegdecodefilter/jpegdecodefilter.h`
  (`examples/experimental/images/images.cpp:37`) but the library lives under
  `src/components/data/...`.
- The judge build compiles 2 TUs fresh and emits exactly 3 `C4100` unreferenced-parameter warnings
  (`jpegdecodefilter.cpp(785,43)`, `(785,66)`, `(801,70)`) - the compiler is reporting the stubs:
  `Decode` ignores both flow-control parameters and `ReadFilter` ignores `unitsToProcess`.
- The existing `qor_image` page already leans on this library as "the decoder its one consumer
  wants" (`docs/wiki/modules/qor_image.md:53-54`, citing the example's link line) - but that
  consumer is itself disabled, so the image pipeline has no reachable decoder either.

## Failure modes

- Corrupt input passes: `Decode` calls `stbi__decode_jpeg_header`, which returns 0 through
  `stbi__err("no SOI Corrupt JPEG")` (`:766`) or "no SOF Corrupt JPEG" (`:776`), but the return
  value `r` is discarded - the filter reports success on any stream, good or broken.
- No pixel path exists: past the header, `stbi__process_marker` (`:453`) and
  `stbi__process_frame_header` (`:655`) run only during header walk; no scan/entropy decoding
  loop (`stbi__decode_jpeg_image`, `stbi__jpeg_load`) is present, so ~650 lines of the vendored
  decoder - IDCT, Huffman, resampling - are dead weight reachable by nothing.
- A pipeline pump that trusts `ReadFilter`'s `unitsProcessed = 1` advances the stream accounting
  over bytes that were never moved (`:804`).
- The commented-out buffer wiring (`:790-791`) means the `stbi__context` carries no data
  (`{0}` at `:789`); any future header parse beyond marker init reads a null buffer pair -
  the current code only dodges it because `STBI__SCAN_header` plus empty callbacks keep
  `stbi__get_marker` at EOF.
- "Experimental, Incomplete" is the only warning label: the library builds, exports the class, and
  ships in the tree with zero consumers, zero tests, and a disabled example whose include path is
  wrong (`images.cpp:37`) - nothing in the repo can notice it regress.

## See also

[[qor_module]] [[qor_signals]] [[qor_pipeline]] [[qor_image]] [[qor_base64decodefilter]]

Not yet paged: the `qor::pipeline::Filter` base it derives from
(`src/framework/data/pipeline/filter.h`, paged in scope of `qor_pipeline`), the disabled `images`
example, and the sibling `unicodetextfilter` configured at
`src/components/data/pipeline/filters/CMakeLists.txt:5`.
