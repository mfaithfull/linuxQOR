---
Type: WikiPage
PageType: invariant
Title: "test"
Status: current
LastVerified: 2026-11-18
Claims: 6
Summary: "Tests run as standalone binaries, not ctest - but only 3 QORTest exes are configured, so the green ladder covers three binaries, not the test tree, and the rule's own example is one of the missing ones."
Anchors:
  - { kind: in-tree, path: docs/agent/verification.md, symbol: Test, verified: 2026-11-18 }
Judges:
  - & build\test\uri\Debug\QORTestURI.exe
---

# test

The assertion, from `docs/agent/verification.md` section "Test": verification binaries are run
**directly** - `& build\test\<path>\<exe>` - and the file states it outright: "The QOR does not use
ctest." If the rule fails, a "tested" claim in the wiki names a suite the repo's own ladder never
runs, and ctest-shaped expectations (`ctest --test-dir`, labels, fixtures) get imported into a tree
that has none of that machinery.

- The judge run 18 Nov 2026: `& build\test\uri\Debug\QORTestURI.exe` printed `[TOTAL](1/1)`,
  `[OK](1/1)`, exit 0 - the binary is run as a process, and the verdict is read from its stdout and
  its exit code, exactly as the section prescribes.
- The section's own example is stale against the disk: `build\test\framework\data\mime\Debug\QORTestMIME.exe`
  does **not** exist. Measured 18 Nov 2026, `build/test` holds exactly **3** configured `QORTest*.exe`
  - `QORTestJSONGenerator.exe` (components/formats/json/generator), `QORTestYAML.exe`
  (components/protocols/yaml), `QORTestURI.exe` (test/uri).
- Its present cost: the "compiler-as-judge per D3" ladder can only ever be green over three
  binaries, while the test sources under `test/` are far more numerous - a lib with no configured
  test exe has, under this rule, no falsifiable behavior claim at all. `wiki-lint freshness` dates
  this page from the exe's write time, which is also the only instrument-side fact about the test
  tree's freshness.
- Nothing enforces the "no ctest" half: no instrument counts a `ctest` invocation. The rule is kept
  by convention and by the absence of any `CTest` configure in the CMake tree.

## See also

[[build-commands]] [[lint]] [[green-is-ok-and-exit-zero]] - the exe's exit code and `[OK]` pair is
the next rule. The three test pages that judge against these binaries sit in `modules/`.
