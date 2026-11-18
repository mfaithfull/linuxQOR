---
Type: WikiPage
PageType: module
Title: "qor_stdoutsink"
Target: qor_stdoutsink
Status: current
LastVerified: 2026-11-18
Claims: 18
Summary: "The most-consumed pipeline sink in the tree (seven examples, three test rows, a real test file): a configured, judge-green stdout sink whose Push divides fwrite's already-unit count by the unit size a second time, silently under-acknowledging any wide-unit pipeline."
Anchors:
  - { kind: in-tree, path: src/components/io/pipeline/sinks/stdoutsink, symbol: qor_stdoutsink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sinks/stdoutsink/CMakeLists.txt, symbol: qor_stdoutsink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sinks/stdoutsink/stdoutsink.h, symbol: StdOutSink, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sinks/stdoutsink/stdoutsink.cpp, symbol: StdOutSink::Push, verified: 2026-11-18 }
  - { kind: in-tree, path: src/components/io/pipeline/sinks/CMakeLists.txt, symbol: stdoutsink, verified: 2026-11-18 }
  - { kind: in-tree, path: test/CMakeLists.txt, symbol: qor_stdoutsink, verified: 2026-11-18 }
Judges:
  - cmake --build cmakebuild --target qor_stdoutsink --parallel 8
---

# qor_stdoutsink

Configured at `sinks/CMakeLists.txt:2`, judge-green on 18 Nov 2026 (exit 0,
both declared TUs compiled fresh, import library generated, 84,480-byte dll
stamped 13:08, 38 `*stdoutsink*` artifacts under `cmakebuild/`). Where its ofstream
sibling is the group's only test-link target, this is the family's
workhorse: its own test file at `test/CMakeLists.txt:64`, linked at
`test/CMakeLists.txt:91,102`, `test/operational/CMakeLists.txt:22,31` and
`test/ui/terminal/CMakeLists.txt:21`, and nine link lines across seven
example CMakeLists (`intermediate/client:13`, `experimental/resourced:23`,
`eventer:22`, `readlinebyline:17,24`, `curlike:15`, `echoclient:17,25`,
`hydra:19`). `repo_map.md:110`, `deps.dot:131` and `FEATURES.md:81` index
it; `purposes.md:89` lumps it with four other std/console pipeline parts.

- One class: `StdOutSink : public pipeline::Sink` (`stdoutsink.h:11`) in
  `qor::io::components` (`:9`) - this folder keeps the PascalCase
  `StdOutSink` spelling its ofstream sibling dropped (`stdofstreamWriter`).
- The guard here is clean: `QOR_PP_H_COMPONENTS_PIPELINE_SINKS_STDOUT`
  opens and closes `stdoutsink.h:4,28` and a live tree grep finds those
  three sites only - the misplaced-`#endif` fossil the socketconnector
  page records for `SINKS_STDOFSTREAM` does not repeat for `STDOUT`.
- The whole product is one method: `Push` is declared **private**
  (`stdoutsink.h:22`) while the base declares it virtual at
  `sink.h:47` - legal for a virtual, but a holder of `StdOutSink&` cannot
  call it directly, only through the base. `Pull` (`sink.h:48`) and
  `WriteBytes` (`sink.h:49`) are not overridden at all.
- The pump body is thirteen lines: `GetBuffer()` (`stdoutsink.cpp:29`),
  `ReadRequest` (`:32`), `fwrite(pData, unitSize, unitsToWrite, stdout)`
  (`:34`); on non-zero return it acknowledges and reports success
  (`:36-40`), otherwise `return false` (`:43`) - like the ofstream sink,
  no continuable flyer on the failure branch, silence.
- The unit arithmetic is the defect: `fwrite` already returns whole
  **items**, so `unitsWritten = bytesWritten / unitSize` (`:37`) divides
  by the unit size a second time - correct only when `unitSize == 1`;
  at unit size 2 a 10-unit push acknowledges 5 units and the pipeline's
  accounting of the push is wrong by the unit factor. The variable name
  `bytesWritten` (`:34`) is the fossil of the misreading.
- The failure branch leaves the by-reference `unitsWritten` untouched
  (`stdoutsink.cpp:43`) - the caller keeps whatever the variable held
  before the call.
- Flush discipline is destructor-only: `~StdOutSink` calls
  `fflush(stdout)` (`stdoutsink.cpp:23`), `Push` never flushes - output
  from this sink interleaves with any other C-stdio writer in the host
  process (`fwrite` to `stdout`, `:34`).
- Fossil include: the TU pulls `pipeline/source.h` (`stdoutsink.cpp:7`)
  and names no `Source` symbol anywhere in the file.
- Copyable by design: copy ctor and assignment (`stdoutsink.h:16-17`,
  `stdoutsink.cpp:13,15-19`) copy only the `Sink` base - a copy is a
  second independent writer to the one process-global `stdout`.
- Index and link agree exactly here, unlike the ofstream sibling: three
  libraries in the link line (`CMakeLists.txt:13`) match `deps.dot:131`
  and `repo_map.md:110` row for row; `qor_signals` is named by no source
  in the folder but is genuinely needed through the base's `SignalBase`
  (`sink.h:16`).
- Bootstrap and module name: 15-line bootstrap naming "Querysoft Open
  Runtime: Standard Output Sink Module" (`qor_stdoutsink.cpp:12`); the
  export macro rides the class and every member (`stdoutsink.h:11,15-18`).
- `FEATURES.md:81` calls it "Stable - std::cout Sink for Pipeline" while
  the implementation is pure C stdio (`fwrite` at `stdoutsink.cpp:34`,
  `fflush` at `:23`) and never touches `std::cout` - the feature column
  describes intent, not code.
- Test surface, counted not read: `test/CMakeLists.txt:64` compiles
  `components/pipeline/test_stdoutsink.cpp`, whose suite (`:45`) carries
  three cases - `testCompilesWithqor_stdoutsinkLinked` (`:47`),
  `canInstancestdoutsink` (`:52`), `canWriteToStdOutSink` (`:59`);
  `test/framework/test_pipeline.cpp` drives `StdOutSink` at four sites
  (`:175,198,226,257`).
- A consumer's namespace fossil: `echoclient/echoresponsepipeline.h:53`
  spells the member `qor::components::StdOutSink` - one `io` level short
  of the class's own namespace (`stdoutsink.h:9`) - while
  `hydra/responsepipeline.h:42` spells it correctly.
- Folder: 4 files, 102 physical lines - TUs 61 (bootstrap 15,
  `stdoutsink.cpp` 46), header 28, `CMakeLists.txt` 13; no `purpose.md`
  in the folder.
- Judge evidence: `cmake --build cmakebuild --target qor_stdoutsink
  --parallel 8` exit 0 on 18 Nov 2026, both TUs compiled and dll linked
  fresh - green again certifies compilation only: the unit-arithmetic
  defect above compiles clean.

## Failure modes

- Wide-unit under-acknowledgement: at `unitSize > 1` the sink
  acknowledges `bytesWritten / unitSize` units (`stdoutsink.cpp:37-38`)
  for an `fwrite` that wrote `unitsToWrite` whole units (`:34`) - the
  pipeline believes a wide-unit push landed a `1/unitSize` fraction of
  what stdout received, and repeated pushes desynchronize buffer state
  against the output stream; the byte-unit case (`unitSize == 1`) is the
  only arithmetic that is right, which is why the test rows stay green.
- Broken sink vs empty buffer are indistinguishable: null buffer and
  zero-byte `fwrite` both fall to `return false` without touching
  `unitsWritten` or firing an error flyer (`stdoutsink.cpp:41-43`) - an
  upstream element cannot tell "nothing to write" from "the sink is
  dead", and no flyer reaches the error plumbing.

## See also

[[qor_stdofstreamsink]] [[qor_stdfstreamconnector]] [[qor_pipeline]]
[[qor_signals]] [[qor_module]] [[qor_linereader]] [[qor_socketconnector]]

Not yet paged: `qor_stdifstreamsource` and `qor_stdinsource` - the next
two leaves of this group.
