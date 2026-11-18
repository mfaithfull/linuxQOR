---
Type: WikiPage
PageType: module
Title: "qor_aggregatesource"
Target: qor_aggregatesource
Status: current
LastVerified: 2026-11-18
Claims: 17
Summary: "An Experimental island of a source: an exported class template whose reflection walk extracts every field of a stored struct and drops it - the buffer write is commented out, EOD fires on every pull, and nothing in the tree links or instantiates it."
Anchors:
  - { kind: in-tree, path: src/components/data/pipeline/sources/aggregatesource, symbol: qor_aggregatesource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/aggregatesource/CMakeLists.txt, symbol: qor_aggregatesource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/aggregatesource/aggregatesource.h, symbol: AggregateSource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/aggregatesource/aggregatesource.h, symbol: ElementWriter, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/aggregatesource/aggregatesource.h, symbol: Pull, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_aggregatesource --parallel 8
---

# qor_aggregatesource

Configured at `src/components/data/pipeline/sources/CMakeLists.txt:4`; the folder is 4 files,
180 lines: 13-line `CMakeLists.txt`, 98-line header, 33-line `.cpp`, 36-line bootstrap. The
header is the whole library and the `.cpp` declares nothing - the namespace opens at
`aggregatesource.cpp:30` and closes empty. `FEATURES.md:107` carries it as bare
"Experimental", no description sentence.

- The whole product is the exported class template `AggregateSource<T> : public
  pipeline::Source` (`aggregatesource.h:55`) with the macro `QOR_AGGREGATESOURCE` on a
  template; the folder instantiates nothing, so the dll carries only the module bootstrap.
- `add_library(qor_aggregatesource SHARED ...)` (`CMakeLists.txt:3`) hard-codes the literal
  name while `:1` sets `${target}` and `:8`/`:13` consume it - of the 14 `add_library` lines
  in `src/components/data/pipeline/` only this and `stringsource` hard-code; 12 use
  `${target}`.
- Reflection is used only from its guts: `fields_count<T>` (`aggregatesource.h:70`),
  `sequence_tuple::get<I>` (`:41`) and `tie_as_tuple` (`:74`) all sit under
  `qor::reflection::detail::`, and reflection is not in the link trio (`CMakeLists.txt:13` -
  trio matches `docs/agent/repo_map.md:94` and `docs/agent/deps.dot:115`).
- `ElementWriter<I,N>::Write` (`aggregatesource.h:36-45`) walks the tied tuple field by
  field but never writes: the buffer line is commented out (`:42`), the terminating
  specialization is a `noexcept {}` no-op (`:48-50`), and `org_val` is unused at every step.
- `Read` (`:68-76`) null-checks `GetBuffer()` (`:71`) and runs the walk (`:74`) - zero bytes
  reach the buffer per call.
- `Pull` (`:81-91`): counts 1 unit (`:86`), acknowledges through `OnReadSuccess(unitsRead)`
  (`:87`), and calls `OnEndOfData()` on every pull (`:88`); it returns true always (`:90`).
- The source pumps one stored struct: `SetData` copies into `m_value` (`:63-65`, member
  `:93`) and each successful pull re-reads that same value.
- Dead includes on both sides: the header carries `<string>` (`:28`) and `sink.h` (`:31`)
  without naming `std::string` or `Sink`; the `.cpp` carries `<stdio.h>` (`:26`) and
  `sink.h` (`:28`) into an empty namespace.
- License is fully pre-SPDX: 23-line MIT blocks dated "2008 - 2025" on `.h` and `.cpp` -
  the same family as the deserializersink's badge - and the bootstrap names the module
  "Querysoft Open Runtime: Aggregate Source Module" (`qor_aggregatesource.cpp:33`).
- Tree-wide it is an island: `qor_aggregatesource` appears only in its own build file
  (`CMakeLists.txt:1`, `:3`), no src/ or test/ target links it, and the template name
  `AggregateSource` occurs nowhere outside its own header (`:55`, ctor `:60`).
- The judge compiles both named `.cpp` (`:3`) into a 73,216-byte dll with 0 warnings
  (run 18 Nov 2026).

## Failure modes

- The source emits nothing: `//buffer.Write(val);` (`aggregatesource.h:42`) - every field is
  extracted through reflection and dropped, while `Pull` still acknowledges a unit (`:87`)
  for a walk that touched zero buffer bytes.
- EOD every pull: `OnEndOfData()` (`:88`) fires after each successful pull, not on
  exhaustion - a consumer pumping twice is told the source ended while it still holds the
  same `m_value`.
- Unit fiction: `unitsRead++` (`:86`) reports one unit whatever the struct size, so a buffer
  metered in units cannot size or throttle this source.
- Vacuous true: with `unitsToRead == 0` the branch is skipped and the call returns true
  (`:90`) leaving `unitsRead` untouched - "nothing asked" is indistinguishable from "data
  supplied".
- Nothing tests it: no target links the lib and no code instantiates the template, so the
  first consumer compile is the class's first real test - today's green judge proves only
  that the bootstrap links.
- The `qor::reflection::detail::` calls (`:41`, `:70`, `:74`) lean on an implementation
  namespace with no link edge and no API promise - reflection internals moving breaks this
  header and no consumer in the tree would fail first.

## See also

[[qor_module]] [[qor_signals]] [[qor_pipeline]]
