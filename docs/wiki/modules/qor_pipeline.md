---
Type: WikiPage
PageType: module
Title: "qor_pipeline"
Target: qor_pipeline
Status: current
LastVerified: 2026-11-19
Claims: 9
Summary: "Sink/Source data pipeline whose Pipeline is itself a Filter; widest gate-token surface in the tree and an Element copy path that reads an uninitialised pointer."
Anchors:
  - { kind: in-tree, path: src/framework/data/pipeline, symbol: qor_pipeline, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/data/pipeline/CMakeLists.txt, symbol: qor_pipeline, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/data/pipeline/pipeline.h, symbol: Pipeline, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/data/pipeline/element.h, symbol: Element, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/data/pipeline/element.cpp, symbol: GetFlowMode, verified: 2026-11-16 }
  - { kind: in-tree, path: src/framework/data/pipeline/purpose.md, symbol: Sink, verified: 2026-11-16 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/stringsource/stringsource.h, symbol: StringSource, verified: 2026-11-19 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/stringsource/CMakeLists.txt, anchor: commit:fd3b46ba9f00f35aa83c49fb72df0e2aebb8e0ba, verified: 2026-11-19 }
Judges:
  - cmake --build cmakebuild --target qor_pipeline --parallel 8
---

# qor_pipeline

"Data pipeline framework (Sink/Source). Chain source->sink." (`docs/agent/purposes.md`). Built SHARED
from eleven translation units - `qor_pipeline.cpp buffer.cpp element.cpp source.cpp iosource.cpp
sink.cpp filter.cpp pipeline.cpp plug.cpp iosink.cpp copyfilter.cpp` - at version 0.5.0
(`src/framework/data/pipeline/CMakeLists.txt`).

- 62 of the 281 `CMakeLists.txt` under `src`, `test` and `examples` name it, behind only
  [[qor_module]] (222) and [[qor_host]] (172). Its link line is
  `qor_module qor_thread qor_signals qor_sync qor_interception qor_error qor_log`, which
  `docs/agent/repo_map.md` copies token for token and places at layer 8 of the build order.
- `QOR_PIPELINE` has 165 hits in 11 files in `src` - the widest gate-token surface measured in
  this slice, against 43 for [[qor_workflow]], 1 for [[qor_role]] and 1 for [[qor_optionparser]].
  The concentration is in the members: `pipeline.h` 34, `source.h` 31, `sink.h` 29, `filter.h`
  24, `plug.h` 24.
- A `Pipeline` is a `Filter`: `class qor_pp_module_interface_gcc(QOR_PIPELINE) Pipeline : public Filter`
  (`src/framework/data/pipeline/pipeline.h`), which is what lets `InsertFilter(Filter*, FilterPos)`
  splice a filter in at `BeforeSink` or `AfterSource` and let a pipeline be inserted into a pipeline.
  It is also the only class in the folder that stores a flow mode (`FlowMode m_flowmode;`).
- Below the pipeline the flow mode is inherited, never stored. `Element::GetFlowMode()` returns
  `m_parent->GetFlowMode()` with a parent and `Pull` without one, and `Element::SetFlowMode(flowmode)`
  forwards to the parent and does nothing at all when `m_parent` is null
  (`src/framework/data/pipeline/element.cpp`) - so an `Element` adopts its pipeline's direction and
  refuses to be pointed the other way until it is attached.
- The five predicates `IsBuffered/IsSource/IsSink/IsPlug/HasSource` are virtual with bodies, not pure
  (`src/framework/data/pipeline/element.h`); `HasSink()` returns `false` unconditionally and
  `GetSource()` returns `nullptr` (`src/framework/data/pipeline/element.cpp`). Only
  `SetSource(Element*)` and `SetSink(Element*)` are `= 0`, so a derived element that never says which
  end it is reports "no source, no sink" instead of failing to compile.
- `purpose.md` is the largest in the slice by an order of magnitude - 60 lines, 392 words, with ASCII
  diagrams walking the semi-circular buffer's read and write heads through a 4K buffer in five steps.
  `docs/agent/purposes.md` compresses all of it to one line ending "semicircular.", a word the source
  writes as two ("Buffers a semi circular").

## Failure modes

- The copy constructor reads `m_buffer` before it holds anything. `Element::Element() : m_parent(nullptr),
  m_buffer(nullptr) { }` initialises both, but the copy constructor has no member-initialiser list and
  no delegating call - `Element::Element( const Element& src ) { *this = src; }`
  (`src/framework/data/pipeline/element.cpp`) - and the `operator=` it forwards to tests the
  not-yet-initialised member: `if(m_buffer) { *m_buffer = *src.m_buffer; }`. `m_buffer` is a raw
  `Buffer*` with no default member initialiser (`src/framework/data/pipeline/element.h`), so every
  `Element` copy branches on an indeterminate pointer, and taking the branch writes through one. The
  same lines carry the author's own note, "NOTE: It's far from obvious that this behaviour will be
  correct in all cases" (`src/framework/data/pipeline/element.cpp`), and the operator copies `m_parent`
  raw, so a copied element claims its original's owner.
- `Protocol` lives in this target's include path but belongs to another library. Its five virtuals
  return `qor::io::network::sockets::eAddressFamily / eType / eProtocol`
  (`src/framework/data/pipeline/protocol.h`), reached through
  `#include "src/framework/io/network/socketdefs.h"` - a header owned by `qor_sockets`
  (`src/framework/io/network/CMakeLists.txt`), which this link line does not name and
  `docs/agent/repo_map.md` places at layer 7. `protocol.h` is in no source list, `Protocol` carries no
  gate token, and eight files across `src/components` include it
  (`src/components/io/network/server/netserver.h`, `src/components/protocols/http/protocol.h`, six
  more), so the class is compiled only inside its consumers.
- No use of `QOR_PIPELINE` survives outside the folder: all 11 files carrying the token in
  `src` sit under `src/framework/data/pipeline`. The class and the four members of
  `src/components/data/pipeline/sources/stringsource/stringsource.h` (`:12`, `:17-19,23`) all
  name `QOR_STRINGSOURCE`, the only token that target's `CMakeLists.txt` stamps, so no class in
  the tree takes its export direction from two targets' tokens - the state of the tree since
  `commit:fd3b46ba9f00f35aa83c49fb72df0e2aebb8e0ba`.

## See also

[[qor_workflow]] [[qor_interception]] [[qor_error]] [[qor_log]] [[qor_module]]
