---
Type: WikiPage
PageType: module
Title: "qor_ttf"
Target: qor_ttf
Status: current
LastVerified: 2026-11-18
Claims: 14
Summary: "A TrueType font reader in 16 files - 12 header-only blob, structure and table classes and 3 .cpp, one of which compiles nothing; its own umbrella needs qor_pipeline but the dll never links it, and the only consumer in the tree is one test file."
Anchors:
  - { kind: in-tree, path: src/components/data/formats/ttf, symbol: qor_ttf, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/formats/ttf/CMakeLists.txt, symbol: qor_ttf, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/formats/ttf/rawfont.h, symbol: RawFont, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/formats/ttf/rawfont.cpp, symbol: RawFont::Init, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/formats/ttf/reader.h, symbol: Reader, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/formats/ttf/blobbuffer.h, symbol: BlobBuffer, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_ttf --parallel 8
---

# qor_ttf

"TrueType font reader (experimental TTF reader; formats/ttf)." (`docs/agent/purposes.md:76`). The
folder is 16 files - a 14-line `CMakeLists.txt`, 3 `.cpp` (153 lines) and 12 `.h` (1,946 lines),
8 of the headers under `tables/`; it builds SHARED and is configured by `add_subdirectory(ttf)` at
`src/components/data/formats/CMakeLists.txt:2`.

- Exactly one `CMakeLists.txt` in the tree names the target - its own declaration (`:1`, `:3`) - so
  the folder has no test target of its own; its only consumer in the build graph is
  `QORTestPipelines` (`test/pipeline/CMakeLists.txt:1`), which compiles `testttfreader.cpp` (`:5`)
  and links `qor_ttf` among 24 libraries (`:15`). The index rows agree with the build here, unlike
  `qor_jsondata`: `docs/agent/repo_map.md:83` and `docs/agent/deps.dot:104` print the same 5
  libraries as the link line (`CMakeLists.txt:14`).
- The 3 `.cpp` are the 3 `add_library` sources (`:3-4`) and the 3 `.obj` under
  `cmakebuild/src/components/data/formats/ttf` - but one of them compiles nothing:
  `blobbuffer.cpp:29-31` is an empty `qor::res::ttf` namespace, because `BlobBuffer`
  (`blobbuffer.h:37`) is defined entirely in its 269-line header.
- One module-interface marker in the whole folder: `RawFont` carries
  `qor_pp_module_interface(QOR_TTF)` (`rawfont.h:46`) and `QOR_TTF` is defined PRIVATE by this
  target alone (`CMakeLists.txt:9`). The 8 table classes (`headtable.h:36` `HeadTable`,
  `cmaptable.h:36` `CmapTable`, `locatable.h:36` `LocaTable`, `glyphtable.h:39` `GlyfTable`,
  `hheatable.h:38` `HheaTable`, `hmtxtable.h:38` `HmtxTable`, `kerntable.h:38` `KernTable`,
  `gpostable.h:38` `GposTable`), `BlobBuffer` and `Reader` carry none - there are no table `.cpp`,
  so consumers take the tables through headers.
- `reader.h` (491 lines) has exactly one consumer in the tree, `test/pipeline/testttfreader.cpp:39`,
  and its `Reader : public qor::pipeline::Reader` (`reader.h:37`) includes a pipeline sink header
  (`:30`) - but none of the target's 3 `.cpp` include `reader.h`, and `CMakeLists.txt:14` does not
  link `qor_pipeline`: the dll builds because none of its pipeline-dependent code is compiled
  into it.
- `rawfont.h` is the tables hub: it includes `blobbuffer.h` plus all 8 `tables/*.h` (`:34-42`) and
  holds 8 `ref_of<>` table members (`:93-100`) and the tag-to-`BlobBuffer` map (`:91`);
  `RawFont::Init` (`rawfont.cpp:42-84`) instantiates the wrappers from that map, keyed by the 8
  `constexpr` tags built by `make_tag` (`ttfstructures.h:35-37`, `:46-53`).
- `Reader::ReadFont` (`reader.h:79-93`) reads the header then 9 table blobs, including
  `'svg '` (`:91`); `ReadHeader` (`:95-116`) recognises four scaler types and parses only TrueType
  (`:100`) - the OTTO (`:104`) and `typ1` (`:108`) branches are empty, so OpenType-CFF and Type 1
  fonts yield an empty table directory; `ReadTable` (`:127-142`) copies the raw bytes into a
  `BlobBuffer` with a byte-order swap (`:135-137`) and emplaces it into the font's map (`:138`).
- `qor_ttf.cpp` (36 lines) is 25 lines of licence plus `ThisModule()` (`:31-35`) registering
  "Querysoft Open Runtime: TrueType Font Module" (`:33`) - the same registration shape as every
  other module page.
- The judge build is a real compile, not a stamp: it compiled the 3 `.cpp` and linked
  `qor_ttf.dll` (1,095,680 bytes; `.lib` 6,406, `.exp` 3,624) with 7 warnings - three C4100 and
  four C4244, all inside `cmaptable.h` (`:122`, `:192-193`) and `glyphtable.h` (`:142`, `:170`,
  `:174`) - and every one of them is reported while compiling `rawfont.cpp`, the only TU that
  includes the tables. The `qor_ttf.dll` copy under `cmakebuild/test/pipeline/Debug` is dated
  19 Oct 2026, older than the one this judge just linked.
- The only executable exercise of this folder is outside the 90-target coverage set:
  `testttfreader.cpp:55` constructs `qor::res::ttf::Reader(connector, 65536)` and `:62` reads a
  `glyph_header` through `m_glyfTable`, but `QORTestPipelines` is a test executable, not a
  `qor_*` module target.

## Failure modes

- Two getters are unguarded where their neighbours are guarded: `GetGlyphCount` and `GetUnitsPerEm`
  (`rawfont.h:80-88`) dereference `m_locaTable` / `m_headTable` directly, while
  `GetGlyphIndex` / `GetGlyphOffset` (`:64-78`) check `IsNotNull()` first - a font without a
  `loca` or `head` table takes down the consumer, and no judge here runs it.
- `Init()` reads the `loca` wrapper from `m_headTable->IndexToLocFormat()` (`rawfont.cpp:57`)
  while its only guard is `blob.Size()` on the loca blob (`:54-58`): a font that has `loca` but no
  `head` dereferences a null ref at `:57`, and the compile-only judge cannot see it.
- The struct layout is the contract and nothing checks it: `offset_subtable` and `table_directory`
  (`ttfstructures.h:55-70`) are plain layouts read through `ReadStructure` (`reader.h:97`, `:122`),
  and the table classes carry no export marker - a packing or alignment slip compiles quietly.
- `BlobBuffer`'s destructor calls `std::terminate()` on `Overrun()` (`blobbuffer.h:90-97`): a
  malformed table length does not report, it kills the process from inside a map value's
  destructor.
- The target knows nothing about its own public half: adding a `.cpp` that includes `reader.h` to
  `add_library` (`:3-4`) breaks the link, because `qor_pipeline` is absent from the 5 libraries at
  `CMakeLists.txt:14` - today that code only links inside `QORTestPipelines`, which does link it
  (`test/pipeline/CMakeLists.txt:14`).

## See also

[[qor_module]] [[qor_host]] [[qor_current]] [[qor_reporting]] [[qor_text]] [[qor_pipeline]]
[[qor_jsondata]]

Not yet paged: `src/components/data/parser` (target `qor_parser`, leaf 3 here) and the pipeline
plumbing the test links beside this library - `qor_fileconnector`, `qor_datasource`,
`qor_datasink` (`test/pipeline/CMakeLists.txt:15`).
