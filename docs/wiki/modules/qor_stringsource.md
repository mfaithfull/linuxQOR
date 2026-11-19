---
Type: WikiPage
PageType: module
Title: "qor_stringsource"
Target: qor_stringsource
Status: current
LastVerified: 2026-11-19
Claims: 15
Summary: "The pipeline's string-backed source, the only one of the five with a Working status and by far the most exercised - 43 StringSource mentions across the MIME and pipeline suites - its export marks all carry its own token since fd3b46ba9f, and one of two targets that hardcode their own name in add_library."
Sources:
  - hook-fd3b46ba9f-qor_stringsource
Anchors:
  - { kind: in-tree, path: src/components/data/pipeline/sources/stringsource, symbol: qor_stringsource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/stringsource/CMakeLists.txt, symbol: qor_stringsource, verified: 2026-11-19 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/stringsource/CMakeLists.txt, anchor: commit:fd3b46ba9f00f35aa83c49fb72df0e2aebb8e0ba, verified: 2026-11-19 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/stringsource/stringsource.h, symbol: StringSource, verified: 2026-11-19 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/stringsource/stringsource.cpp, symbol: StringSource::ReadBytes, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/stringsource/qor_stringsource.cpp, symbol: ThisModule, verified: 2026-11-18 }
  - { kind: in-tree, path: src/configuration/configuration.h, symbol: qor_pp_module_interface, verified: 2026-11-19 }
  - { kind: in-tree, path: test/framework/test_pipeline.cpp, symbol: validateStringSource, verified: 2026-11-19 }
Judges:
  - cmake --build cmakebuild --target qor_stringsource --parallel 8
---

# qor_stringsource

Configured first at `src/components/data/pipeline/sources/CMakeLists.txt:1`; the folder
is 4 files, 94 lines: 13-line `CMakeLists.txt`, 31-line `stringsource.h`, 35-line
`stringsource.cpp`, 15-line bootstrap. `FEATURES.md:95` carries it as "Working" - the
odd status row among the five sources, which otherwise split Stable/Experimental - and
`docs/agent/purposes.md:92` lumps it with the other four.

- The smallest public surface in the family: one class `StringSource : public
  pipeline::Source` (`stringsource.h:12`), public ctor/dtor/`SetData` (`:17-19`), one
  protected `ReadBytes` override (`:23`), and two privates - `std::string m_data` and
  `std::string::iterator m_it` (`:25-26`); no template, no helper, no writer wrapper.
- The build hardcodes itself: `set(target qor_stringsource)` (`CMakeLists.txt:1`) is
  ignored by `add_library(qor_stringsource SHARED ...)` (`:3`), which spells the
  literal instead of `${target}` - grep for `add_library(qor_` across all of
  `src/components/data/pipeline` hits exactly this line and `aggregatesource/
  CMakeLists.txt:3`, the only two such lines in the tree.
- The link line is the family's only trio: `qor_module qor_signals qor_pipeline`
  (`CMakeLists.txt:13`), matching `docs/agent/repo_map.md:98` and `docs/agent/deps.dot:119`
  exactly - the other four sources all link a fourth library (`qor_error`), and this TU
  includes and fires none.
- The export marking follows the family pattern: the class carries the
  gcc-only variant `qor_pp_module_interface_gcc(QOR_STRINGSOURCE)` (`stringsource.h:12`),
  which expands to nothing under MSVC (`src/configuration/configuration.h:43`) and to an
  export under gcc/clang (`:40`), and all four member marks carry the target's own token
  (`stringsource.h:17-19,23`) - the same class-plus-members pairing `datasource.h:18,23-40`
  shows, where `generatorsource.h:16`, `randomsource.h:13` and `aggregatesource.h:55` mark
  only their class. Since `commit:fd3b46ba9f` no header under the five source folders names
  another target's token, and this class and its members take their export direction from one.
- The gate test is emptiness, not identity: `qor_pp_module_interface(_X)` expands by
  `qor_pp_if(qor_pp_is_empty(_X), qor_pp_import, qor_pp_export)` (`configuration.h:33`), never
  matching the named token against the `target_compile_definitions` entry (`CMakeLists.txt:8`),
  so the foreign `QOR_PIPELINE` marks this header carried until `commit:fd3b46ba9f` never
  failed to compile - a token mismatch in this family is latent, not loud.
- The copy is byte-at-a-time: `ReadBytes` loops while `m_it != m_data.end()` and the
  request is unfilled, writing `*m_it++` per pass (`stringsource.cpp:25-33`) and
  returning the actual count - a short count then a zero, with end-of-data signalled by
  the framework: the TU never touches `OnEndOfData` (unlike generatorsource).
- It replays: the default ctor starts empty (`:11-15`) and `SetData` copies the new
  string then re-primes `m_it` to `begin()` (`:19-23`), so the object can be refilled
  and pumped again, mid-stream or after exhaustion.
- The include guard names the folder's old home: `QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STRING`
  (`stringsource.h:4,31`) omits the `data/` segment the path has today
  (`docs/agent/repo_map.md:98`), the same fossil that hydra's `requestpipeline.h:31`
  include still carries from the pre-move layout.
- License shape breaks with its `.cpp` siblings: all four files carry the 2-line SPDX
  BSL-1.0 "2008 - Present" header (`:1-2` in each) - no 23-line MIT block on the TUs as
  generatorsource and randomsource have - and the bootstrap is the 15-line SPDX shape
  naming "Querysoft Open Runtime: String Source Module" (`qor_stringsource.cpp:12`).
- It is the family's test workhorse: `StringSource` appears 31 times in
  `test/framework/data/mime/test_mime.cpp` (QORTestMIME links the lib,
  `test/framework/data/mime/CMakeLists.txt:15`) and 12 times in
  `test/framework/test_pipeline.cpp`, where `validateStringSource` (`:217`) builds a
  push-mode pipeline `StringSource -> sink` and pumps `testData.length()` (`:242`);
  `validateStringSink` (`:280`) wires its own `StringSource` (`:293`), while the two base64
  cases that also construct one (`:261`, `:320`) sit inside commented-out blocks
  (`:244-278`, `:306-352`); QORTest links the lib on both OS branches
  (`test/CMakeLists.txt:95,106`).

## Failure modes

- A declared link nobody uses: `QOROpTests` carries `qor_stringsource` on its link line
  (`test/operational/CMakeLists.txt:34`) but its three sources (`:6-9`) name
  StringSource nowhere in the directory.
- No dedicated suite: the stdio siblings get their own test files
  (`test/CMakeLists.txt:63-67`), but no `test_stringsource.cpp` exists anywhere - the
  class is exercised only through the shared `test_pipeline.cpp` case and as MIME
  plumbing.
- The iterator lives inside the buffer: `m_it` is a `std::string::iterator` into
  `m_data` (`stringsource.h:25-26`) and the class declares no copy control anywhere, so
  a copy taken mid-stream holds an iterator into the original string's storage, even
  though `SetData` itself keeps the pair coherent by assigning first (`stringsource.cpp:21-22`).
- Zero is ambiguous: `ReadBytes` returns 0 both for a never-filled and an exhausted
  source (`:28`), and nothing - header (`:23`) or docs - records that the zero means
  end-of-data to the framework.
- The judge compiled both named `.cpp` fresh (exit 0, run 18 Nov 2026) into a
  106,496-byte dll stamped 11:26, newer than every folder file (newest source
  `stringsource.cpp`, 15 Oct 2026).

## See also

[[qor_datasource]] [[qor_aggregatesource]] [[qor_generatorsource]] [[qor_randomsource]]
[[qor_pipeline]] [[qor_module]] [[qor_signals]] [[qor_stringsink]]

Not yet paged: `QORTestMIME` / `QORTest` (test executables, not lib targets, outside the
90-target census), and `qor_serialize` (group 3 leaf 4, folder
`src/components/io/deserializer`).
