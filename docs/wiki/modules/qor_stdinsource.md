---
Type: WikiPage
PageType: module
Title: "qor_stdinsource"
Target: qor_stdinsource
Status: current
LastVerified: 2026-11-18
Claims: 25
Summary: "The stdin half of the console pair: the only std pipeline part whose index matches its link line (qor_error included, and genuinely used) - but LineMode()'s defaulted-true getter silently re-arms line mode every Pull, a normal EOF fires a serious flyer, and the one read test is commented out as a TODO."
Anchors:
  - { kind: in-tree, path: src/components/io/pipeline/sources/stdinsource, symbol: qor_stdinsource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sources/stdinsource/CMakeLists.txt, symbol: qor_stdinsource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sources/stdinsource/stdinsource.h, symbol: StdInSource, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sources/stdinsource/stdinsource.cpp, symbol: StdInSource::Pull, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sources/CMakeLists.txt, symbol: stdinsource, verified: 2026-11-18 }
  - { kind: in-tree, path: test/components/pipeline/test_stdinsource.cpp, symbol: StdInSourceTestSuite, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_stdinsource --parallel 8
---

# qor_stdinsource

Configured at `sources/CMakeLists.txt:1`, judge-green on 18 Nov 2026
(exit 0, both declared TUs compiled fresh, import library generated,
1,019,392-byte dll stamped 13:20). The inverse of its stdout sibling's
test luck: it is the family's least-tested std part, because the only
read test is commented out (see below). `repo_map.md:112`, `deps.dot:133`
and `FEATURES.md:79` index it; `purposes.md:89` lumps it with four other
std/console pipeline parts.

- One class: `StdInSource : public pipeline::Source`
  (`stdinsource.h:11`) in `qor::io::components` (`:9`) - the plain
  `Source` base, not the `iosource<Connector>` template the ifstream
  source derives from, matching the PascalCase console naming of
  `StdOutSink`.
- `LineMode` is an accident waiting: `bool LineMode(bool newMode =
  true)` (`stdinsource.h:19`) returns the old value **and assigns the
  defaulted `true`** (`stdinsource.cpp:25-30`) - every call-as-getter
  silently switches line mode on.
- Default is line mode: the ctor init-list sets `m_lineMode(true)`
  (`stdinsource.cpp:13`).
- `ReadLine` is char-at-a-time: `std::cin.read(&c, 1)` in a do-loop
  (`stdinsource.cpp:40`) stopping at `0x0A` or the units budget
  (`:50`); the trailing LF is stripped from the result (`:52-54`) and
  `unitsRead = result.size()` (`:56`).
- A clean EOF fires the error severity: `if (!std::cin.good())` calls
  `serious("std::cin bad status.")` (`stdinsource.cpp:41-44`) - `good()`
  goes false at plain end-of-input, so a user pressing Ctrl+Z produces
  the flyer the design reserves for genuine faults.
- Two stream disciplines on one console: line mode reads `std::cin`
  (`:40`) while the raw branch reads C `stdin` with `fread(space,
  GetBuffer()->GetUnitSize(), unitsToRead, stdin)` (`:84`).
- Stall is reported, not silent: a null `WriteRequest` fires
  `continuable("Pipeline stall. No space in source buffer.")` and
  returns false (`stdinsource.cpp:70-75`) - stdoutsink's null-buffer
  branch just returns false.
- Zero request short-circuits: `if(!unitsToRead) return true;`
  (`stdinsource.cpp:63-66`) touches nothing.
- End of data is a flyer: a zero `unitsRead` calls `OnEndOfData()`
  (`stdinsource.cpp:87-95`) instead of returning a status the caller
  could switch on.
- The private re-access pattern repeats: `Pull` is declared **private**
  (`stdinsource.h:25`) while the base declares it virtual at
  `source.h:49` - the same shape as stdoutsink's private `Push`.
- The copy semantics drop state: copy ctor and assignment copy only the
  `Source` base (`stdinsource.cpp:15-21`) and never initialize or copy
  `m_lineMode` - a copy starts with an indeterminate mode, which the
  next `Pull`'s getter-call at `:76` coerces back to `true`.
- Index and link agree here, uniquely among the five std parts: the
  link line names four libraries (`CMakeLists.txt:13`: `qor_module
  qor_signals qor_pipeline qor_error`) and `repo_map.md:112` and
  `deps.dot:133` show the same four - and the extra `qor_error` edge is
  real, this is the TU that fires `serious` (`:43`) and `continuable`
  (`:73`) through `error.h` (`:9`).
- Fossil include: `pipeline/sink.h` (`stdinsource.cpp:8`) - the TU
  names no `Sink` symbol.
- `FEATURES.md:79` calls it "Stable - std::cin Source for Pipeline":
  true of the default line mode only, the raw branch is C stdio
  (`:84`).
- Test surface, read to the boundary: `test/CMakeLists.txt:63` compiles
  `test_stdinsource.cpp`, whose live suite carries the link
  (`testCompilesWithqor_stdinsourceLinked`, `:46`) and instance
  (`canInstancestdinsource`, `:51`) cases only - the single read case,
  `canReadFromstdinsource`, sits inside a `/*TODO:` block
  (`:57` opening, case at `:58`, `*/` at `:70`), parked because stdin
  needs interactive input.
- Cross-use: `test_stdoutsink.cpp:37` includes this header and builds a
  `StdInSource source;` at `:63` inside the stdout sink's test.
- Consumers, counted not read: three examples take the header
  (`intermediate/client/workflow.h:9` member `:36`,
  `experimental/curlike/workflow.h:30`, `echoclient` request/response
  headers `:35-36` with member at `echorequestpipeline.h:49`), on top
  of link lines `client:13`, `curlike:15`, `echoclient:17,25`, and the
  test rows `test/CMakeLists.txt:91,102`, `test/operational:22,31`,
  `test/ui/terminal:21`.
- The namespace fossil repeats: `echorequestpipeline.h:49` spells the
  member `qor::components::StdInSource` - one `io` level short of
  `stdinsource.h:9` - the same miss the stdoutsink page records on the
  sibling response pipeline.
- Guard is clean: `QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_STDIN` appears
  at its own `stdinsource.h:4,5,30` only.
- Bootstrap and export: 15-line bootstrap naming "Querysoft Open
  Runtime: Standard Input Source Module" (`qor_stdinsource.cpp:12`);
  every public member carries the export macro (`stdinsource.h:15-20`).
- Folder: 4 files, 159 physical lines - TUs 116 (bootstrap 15,
  `stdinsource.cpp` 101), header 30, `CMakeLists.txt` 13; no
  `purpose.md` in the folder.
- Judge evidence: `cmake --build cmakebuild --target qor_stdinsource
  --parallel 8` exit 0 on 18 Nov 2026, both TUs compiled and dll linked
  fresh - green certifies compilation only: the `LineMode` re-arming
  below compiles clean.

## Failure modes

- Raw mode does not stick: `Pull` tests the mode with `if(LineMode())`
  (`stdinsource.cpp:76`), and the defaulted `true` parameter means that
  call **sets line mode back on** while returning the old value
  (`:25-30`) - a caller that did `LineMode(false)` gets exactly one raw
  `fread` pull, then the next pull silently returns to char-at-a-time
  `std::cin` reads, mid-stream, with nothing reported.
- EOF dressed as a serious fault: end of input takes the
  `!std::cin.good()` branch and fires `serious("std::cin bad status.")`
  (`stdinsource.cpp:41-44`) - the severity that marks a broken stream
  fires on every normal end of keyboard input, and the line merely
  `break`s so the partial line is still returned.
- Copied sources run in an indeterminate mode: neither copy path
  touches `m_lineMode` (`stdinsource.cpp:15-21`), so the first `Pull`
  on a copy reads an uninitialized `bool` at `:76` - and because that
  same call re-arms `true`, the bug hides itself after one round trip,
  changing behavior once in a way no test can catch (the read case is
  commented out, `test_stdinsource.cpp:57-70`).

## See also

[[qor_stdoutsink]] [[qor_stdifstreamsource]] [[qor_stdfstreamconnector]]
[[qor_pipeline]] [[qor_signals]] [[qor_module]] [[qor_error]]
