---
Type: WikiPage
PageType: module
Title: "qor_jsondata"
Target: qor_jsondata
Status: current
LastVerified: 2026-11-18
Claims: 13
Summary: "The RFC 8259 grammar in one dll - 17 parser states, 11 generator states and a header-only model; all 29 sources compile, but the index row prints 5 of the 12 link libraries, the umbrella header has zero consumers and a 0-byte object.h sits in the root."
Anchors:
  - { kind: in-tree, path: src/components/data/formats/json, symbol: qor_jsondata, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/formats/json/CMakeLists.txt, symbol: qor_jsondata, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/formats/json/qor_json.cpp, symbol: ThisModule, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/formats/json/parser/tokens.h, symbol: jsonToken, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/formats/json/parser/helpers.h, symbol: GetTokenName, verified: 2026-11-18 }
  - { kind: in-tree, path: src/framework/data/resources/CMakeLists.txt, symbol: qor_jsondata, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_jsondata --parallel 8
---

# qor_jsondata

"JSON data handling (formats/json)." (`docs/agent/purposes.md:75`), and the writing half describes
itself: "The writing half of the JSON format, mirroring `parser/`" with "Grammar per RFC 8259:
value = string / number / true / false / null / object / array"
(`src/components/data/formats/json/generator/purpose.md:1-3`, `:13-15`). The folder is 121 files -
a 42-line `CMakeLists.txt`, 29 `.cpp` (2,318 lines), 89 `.h` (2,668 lines), a 15-line generator
purpose and a 95-line generator `status.md`; it builds SHARED and is configured by
`add_subdirectory(json)` at `src/components/data/formats/CMakeLists.txt:1`.

- 4 of the 281 `CMakeLists.txt` under `src`, `test` and `examples` name the target: its own
  declaration (`src/components/data/formats/json/CMakeLists.txt:1`, `:3`), the framework consumer
  `qor_resources` (`src/framework/data/resources/CMakeLists.txt:22`) and the two test executables
  (`test/components/formats/json/CMakeLists.txt:17` and `:24`,
  `test/components/formats/json/generator/CMakeLists.txt:18`). The index row
  (`docs/agent/repo_map.md:82`) prints only the first 5 of the 12 libraries on the real link line
  (`CMakeLists.txt:41-42`); `qor_workflow qor_pipeline qor_parser qor_issue qor_error qor_log
  qor_generator` - the data-flow half of the closure - are absent from the row.
- Every source in the folder compiles: `add_library` (`:3-31`) names 29 `.cpp` and the folder holds
  exactly 29, and `cmakebuild/src/components/data/formats/json` holds 29 `.obj`. The other 89 files
  are headers, and two of the three sub-trees are header-only: `model/` is 21 `.h` and 0 `.cpp`
  (`model/valuemodel.h:21` `class ValueModel`, `model/object.h:12` `class Object`), and
  `parser/nodes/` is 21 `.h` and 0 `.cpp`.
- Two namespaces and one shared token marker - the parser states live in
  `qor::data::parser::json`
  (`parser/tokens.h:17`) and the generator states in `qor::data::generator::json`
  (`generator/generator.h:7`); 17 + 11 = 28 state classes each write
  `qor_pp_module_interface(QOR_JSON)` (`parser/states/_4/value.h:12`,
  `generator/states/_value.h:18`), and `QOR_JSON` is defined PRIVATE by exactly this
  `CMakeLists.txt:36`. The `model/` classes carry no marker at all.
- The folder holds four files named `object.h`, and the root one is 0 bytes:
  `src/components/data/formats/json/object.h` (0 lines) is included by 0 files in the tree, while
  `model/object.h` (29 lines), `parser/nodes/object.h` (23) and `parser/states/_3/object.h` (25)
  all have consumers - e.g. `src/framework/data/resources/types/json/jsonresource.h:12` and
  `test/components/formats/json/jsoncompare.h:21`.
- `jsonparser.h` (42 lines) is an umbrella that includes 29 state headers (`:7-35`) and is included
  by 0 files in `src`, `test` or `examples`: every consumer names states by hand instead -
  `src/framework/data/resources/types/json/reader.h:7-10` takes `parser/tokens.h`,
  `parser/states/_3/object.h`, `parser/nodes/object.h` and `model/object.h` separately.
- `parser/helpers.h` (99 lines) is included by exactly one compilation unit,
  `parser/states/_1/array.cpp:13` (`#include "../../helpers.h"`) - and `GetTokenName` (`helpers.h:12`)
  is a non-static, non-inline free function in that header.
- The token vocabulary is one enum plus one map: `jsonToken` (`parser/tokens.h:19-52`) starts at
  `eToken::Max + 1ull` (`:21`, comment `//19`) and carries 31 members; `jsonTokenNames`
  (`:54-86`) is a `static const std::map` with 31 matching entries, in a header - a per-TU copy.
- The dll is a module by bootstrap, not by content: `qor_json.cpp:10-14` is 15 lines whose body is
  `ThisModule()` returning a static `qor::Module("Querysoft Open Runtime: JSON Module", ...)`, and
  `<buildnumber.h>` is pulled only `#ifdef NDEBUG` (`:5-7`). The tests exist and are configured:
  `test/CMakeLists.txt:123` (`add_subdirectory(components/formats)`) brings `QORTestJSON`
  (`main.cpp` 310 lines, plus `jsoncompare.h` 193, `jsonreader.h` 89, `jsonwriter.h` 92) and
  `QORTestJSONGenerator` (`test_jsongenerator.cpp` 811 lines); both vcxproj exist under
  `cmakebuild/test/components/formats/json`.

## Failure modes

- The 18 Nov 2026 judge is a stamp, not a compile: the build log is 24 lines with 0 compile lines
  for this target (only `qor_signals.cpp` of a dependency recompiled) and `qor_jsondata.dll`
  (2,916,864 bytes, `.exp` 59,297) is newer (08:33) than the newest source in the folder
  (`generator.h`, 25 Oct 2026 12:30). Green here says nothing about the grammar; the 29 `.obj` are
  from an earlier build.
- A wrong grammar production still compiles: the state classes are template expressions over the
  [[qor_parser]] combinators - `member final : public Sequence_t< string, Sequence_t<name_separator,
  value> >` (`parser/states/_3/member.h:15`) - and no judge runs the parse; the only executable
  check is `QORTestJSON`, which this leaf's single judge does not run.
- The 0-byte `object.h` is invisible to every instrument: no include cites it, no line counts it,
  and deleting it would break nothing - a field nothing checks, the F1 shape, sitting in the
  target's root.
- The single `helpers.h` inclusion is load-bearing: a second state `.cpp` that included it would
  give `GetTokenName` two external definitions (`helpers.h:12`) and only the link step of some
  future build would say so; nothing in the tree records that `array.cpp:13` is load-bearing.
- `docs/agent/repo_map.md:82` is not the dependency list: acting on its 5 libraries instead of the
  12 at `CMakeLists.txt:41-42` drops the page's own `Judges` dependency direction - the instrument
  (`wiki-census targets`) reads the CMakeLists, so the index drift is quiet.

## See also

[[qor_module]] [[qor_host]] [[qor_current]] [[qor_sync]] [[qor_injection]] [[qor_workflow]]
[[qor_pipeline]] [[qor_resources]] [[qor_text]]

Not yet paged: `src/components/data/parser` (target `qor_parser` - the combinator base every state
here derives from), `src/components/io/generator` (`qor_generator`), and
`src/components/data/formats/ttf` (`qor_ttf`, the sibling folder configured at
`src/components/data/formats/CMakeLists.txt:2`).
