---
Type: WikiPage
PageType: module
Title: "qor_linereader"
Target: qor_linereader
Status: current
LastVerified: 2026-11-18
Claims: 14
Summary: "The only pipeline filter in this group with real logic and no consumer: a 43-line CR/LF splitter that treats ReadFilter and WriteFilter as one function, counts bytes not lines, can stall on an empty line, and is registered by a zero-byte bootstrap file."
Anchors:
  - { kind: in-tree, path: src/components/data/pipeline/filters/linereader, symbol: qor_linereader, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/filters/linereader/CMakeLists.txt, symbol: qor_linereader, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/filters/linereader/linereader.h, symbol: LineReader, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/filters/linereader/linereader.cpp, symbol: ReadUpToLineEnd, verified: 2026-11-18 }
  - { kind: in-tree, path: examples/experimental/readlinebyline/CMakeLists.txt, symbol: qor_linereader, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_linereader --parallel 8
---

# qor_linereader

`FEATURES.md:62` carries "Stable" plus the purpose text in the status column itself -
"A Line At A Time reader for Pipeline" - the only filter row in this group with a description.
Configured at `src/components/data/pipeline/filters/CMakeLists.txt:3`; the folder is 4 files,
107 lines: 13-line `CMakeLists.txt`, 27-line header, 67-line filter, and
`qor_linereader.cpp` at 0 bytes - a literally empty translation unit with no `ThisModule()`
bootstrap (the jpeg sibling's is 36 lines).

- The empty registration file compiles anyway: the judge build takes both named `.cpp`
  (`CMakeLists.txt:3`) and links a working 71,168-byte `qor_linereader.dll` with zero warnings -
  0 warnings is unique in this group (jpeg: 3 C4100, base64-decode: 1 C4244).
- Markers invert the jpeg style: the class carries the gcc form
  `qor_pp_module_interface_gcc(QOR_LINEREADER)` (`linereader.h:11`) and all six members are
  individually marked `qor_pp_module_interface(QOR_LINEREADER)` (`:15-21`) - 7 sites; the macro
  is `PRIVATE` in the build file (`CMakeLists.txt:8`).
- Link line is the filter-family trio `qor_module qor_signals qor_pipeline` (`:13`), matching
  `docs/agent/repo_map.md:88` and `docs/agent/deps.dot:109`.
- `LineReader : public pipeline::Filter` (`linereader.h:11`) has exactly one behavior:
  `ReadFilter` and `WriteFilter` are character-for-character the same delegation to
  `ReadUpToLineEnd` (`linereader.cpp:13-21`) - direction through the filter does not matter.
- `ReadUpToLineEnd` (`:23-64`) is the whole product: `ReadRequest` up to `unitsToProcess`
  source bytes (`:30`), scan for `0x0D`/`0x0A` (`:35-38`), `memcpy` the line body into the sink
  (`:55-56`), then acknowledge - `WriteAcknowledge` of the body (`:57`) and
  `ReadAcknowledge` of body plus consumed terminator (`:58`), with one CR then one LF skipped so
  CRLF costs one line (`:44-51`).
- `unitsProcessed` is set to the byte count of the line body (`:59`), not 1 - the "Line At A
  Time" contract reports bytes.
- No `src/` target links it and no test mentions it; the only linking target is the
  `readlinebyline` example (`examples/experimental/readlinebyline/CMakeLists.txt:17`,
  Windows branch `:24`, class used at `linebylineworkflow.cpp:48`) - and that directory is
  configured by nobody: `examples/experimental/CMakeLists.txt` lists 12 subdirectories and
  `readlinebyline` is not among them, not even commented out (unlike `images` at `:9`).
- Its own `purpose.md` is absent; the row at `docs/agent/purposes.md:90` groups it with the four
  filter siblings under "pipeline filters (inline buffer+function)" - the description lives only
  in `FEATURES.md:62`.

## Failure modes

- Empty-line stall: when the first buffered byte is a terminator, `output_space` is 0
  (`:40`) and the entire `if(output_space > 0)` block is skipped (`:42`) - the terminator is
  never consumed and never acknowledged; the call returns true with `unitsProcessed = 0`
  (`:27`) forever, so a run of blank lines wedges a pump that waits on progress.
- Scan bound `data + unitsToProcess - 1` (`:35`) leaves the final window byte unexamined, and
  the terminator peek after `scan++` (`:44-51`) can read one byte past the `ReadRequest` grant.
- `memcpy(space, data, output_space)` (`:56`) has no null check on the result of
  `WriteRequest(output_space)` (`:55`) - a full sink turns into a write through a null pointer.
- `ReadFilter` == `WriteFilter` (`:13-21`): a pipeline stage that invokes both directions per
  pump copies the same line twice, because the filter has no state distinguishing direction.
- The "Stable" badge (`FEATURES.md:62`) sits over a library with a 0-byte registration TU, zero
  consumers in any built target, and zero tests - the 0-warning, 71 KB dll is all the evidence
  of stability the repo has, and nothing can observe a regression.

## See also

[[qor_module]] [[qor_signals]] [[qor_pipeline]] [[qor_base64decodefilter]] [[qor_jpegdecodefilter]]

Not yet paged: the `pipeline::Filter` base it derives from
(`src/framework/data/pipeline/filter.h`, paged in scope of `qor_pipeline`), the never-configured
`readlinebyline` example, and the sibling `unicodetextfilter` configured at
`src/components/data/pipeline/filters/CMakeLists.txt:5`.
