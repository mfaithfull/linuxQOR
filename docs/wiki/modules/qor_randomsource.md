---
Type: WikiPage
PageType: module
Title: "qor_randomsource"
Target: qor_randomsource
Status: current
LastVerified: 2026-11-18
Claims: 13
Summary: "The pipeline's 'Noise Source' (FEATURES.md calls it Stable): a 33-line header, one override whose ReadBytes re-emits a single random_device sample taken once at construction - a constant byte stream that never ends, whose only in-tree consumer is an unconfigured example with a stale include."
Anchors:
  - { kind: in-tree, path: src/components/data/pipeline/sources/randomsource, symbol: qor_randomsource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/randomsource/CMakeLists.txt, symbol: qor_randomsource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/randomsource/randomsource.h, symbol: RandomSource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/randomsource/randomsource.cpp, symbol: RandomSource::ReadBytes, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/data/pipeline/sources/randomsource/randomsource.cpp, symbol: RandomSource::Next, verified: 2026-11-18 }
  - { kind: in-tree, path: examples/experimental/hydra/CMakeLists.txt, symbol: qor_randomsource, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_randomsource --parallel 8
---

# qor_randomsource

Configured at `src/components/data/pipeline/sources/CMakeLists.txt:2`; the folder is 4
files, 136 lines: 13-line `CMakeLists.txt`, 33-line `randomsource.h`, 54-line
`randomsource.cpp`, 36-line bootstrap. `FEATURES.md:71` carries it as "Stable - Noise
Source for Pipeline" - one of the two Stable rows among the five pipeline sources - and
`docs/agent/purposes.md:92` lumps it with the other four.

- The smallest unit of its family: one class `RandomSource : public pipeline::Source`
  (`randomsource.h:13`), one override `ReadBytes` (`:22`), one helper `Next` (`:23`),
  two privates - `std::random_device m_rnd` and `int m_start` (`:27-28`) - with no
  template and no writer wrapper beside it (datasource ships an 85-line `writer.h`,
  generatorsource a 93-line template).
- Family shape holds: namespace `qor::pipeline::components` (`:11`), MSVC marking
  `qor_pp_module_interface(QOR_RANDOMSOURCE)` (`:13`) paired with the compile definition
  `QOR_RANDOMSOURCE` (`CMakeLists.txt:8`), and a tree-unique include guard naming
  `RANDOM` (`:4`, 3 matches repo-wide, all this file).
- The noise product is one sample: the ctor draws `m_start = m_rnd()` once
  (`randomsource.cpp:33-36`); `Next()` returns `m_start % 128` (`:40-43`); `ReadBytes`
  fills every requested byte with `Next() % 256` (`:47-50`) and returns `bytesToRead`
  (`:51`) - the stream is one constant byte repeated, never reseeded.
- It never ends: `ReadBytes` always returns the full count (`:51`), so the framework's
  zero-return path into `OnEndOfData` is unreachable from this source and pipeline
  termination belongs to whatever consumes it.
- Link line, index and graph agree on the quartet `qor_module qor_signals qor_pipeline
  qor_error` (`CMakeLists.txt:13` = `docs/agent/repo_map.md:97` = `docs/agent/deps.dot:118`)
  - but neither of its two distinctive edges survives the read: `qor_error` is included
  (`randomsource.cpp:29`) and no flyer fires in the 54-line TU, and `qor_signals` is
  named by no source in the folder; `src/framework/data/pipeline/sink.h` (`:28`) is a
  dead include with no sink named anywhere in the file.
- Its one in-tree consumer is out of the build: `hydra` links it
  (`examples/experimental/hydra/CMakeLists.txt:19`) but
  `examples/experimental/CMakeLists.txt` configures only `flow`, `osspecific` and
  `resourced` - hydra is not in the list and no hydra project exists in `cmakebuild`;
  no test target links this library either.
- The dead consumer carries a fossil: `requestpipeline.h:31` includes
  `src/components/io/pipeline/sources/randomsource/randomsource.h` - a path that no
  longer exists (`src/components/io/pipeline` today holds connectors, sinks and the two
  stdio sources) - so the include records where the lib used to live, and re-enabling
  hydra would build red on it.
- License split follows the family: BSL-1.0 SPDX "2008 - Present" on the header
  (`:1-2`), full MIT blocks "2008 - 2025" on both `.cpp` (`randomsource.cpp:1-23`,
  `qor_randomsource.cpp:1-23`); the bootstrap's `ThisModule` names "Querysoft Open
  Runtime: Random Source Module" (`qor_randomsource.cpp:33`).
- The judge compiled both named `.cpp` fresh (exit 0, run 18 Nov 2026) into a
  988,672-byte dll stamped 11:13, newer than every folder file (newest source 15 Oct
  2026).

## Failure modes

- The randomness is a construction accident: `m_rnd` is sampled exactly once (`:35`)
  and `Next()` re-derives `m_start % 128` forever (`:42`) - every pull emits the same
  byte, so the "Noise Source" (FEATURES.md:71) is a constant stream per object, and
  nothing in the test tree exercises it (no consumer compiles).
- The value range is silently halved: `Next() % 256` (`:49`) can never set the high
  bit - `Next()` is already `% 128` (`:42`) - so a byte consumer expecting 0-255 sees
  0-127.
- No end condition at all: the return is unconditional `bytesToRead` (`:51`) - the
  source cannot report end-of-data, and a pipeline that assumes a finite source
  hangs on this one.
- Judged by build only: with hydra unconfigured and its include stale
  (`requestpipeline.h:31`), no compiled TU instantiates the class anywhere in the
  tree, so a green judge proves the syntax and link line, not any behaviour.

## See also

[[qor_datasource]] [[qor_aggregatesource]] [[qor_generatorsource]] [[qor_pipeline]]
[[qor_module]] [[qor_signals]] [[qor_error]]

Not yet paged: `qor_stringsource` (group 3 leaf 3, the last source of the five), the
`hydra` example target (unconfigured), and `qor_socketconnector` / `qor_stdoutsink`
(hydra's other pipeline libraries - group 4 and 5).
