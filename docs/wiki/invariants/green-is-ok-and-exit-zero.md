---
Type: WikiPage
PageType: invariant
Title: "green-is-ok-and-exit-zero"
Status: current
LastVerified: 2026-11-18
Claims: 5
Summary: "Green means [OK](n/n) in the output AND shell exit code 0 - the only definition of green in the repo, and no shipped instrument checks the pair; a 0-exit run with no [OK] is not green."
Anchors:
  - { kind: in-tree, path: docs/agent/verification.md, symbol: green, verified: 2026-11-18 }
Judges:
  - & build\test\uri\Debug\QORTestURI.exe
---

# green-is-ok-and-exit-zero

The assertion, from `docs/agent/verification.md` L15 (the green definition closing the "Test"
section): a run is **green** if and only if the output contains `[OK](n/n)` **and** the shell exit
code is 0. It is the repo's only definition of green - every "VERIFIED (green)" entry in the Results
log below it is a claim about this pair.

- The two conditions are independent, and the rule is the **conjunction**: an exe that exits 0 with
  no `[OK](n/n)` line is not green, and an `[OK](n/n)` printed under a non-zero exit is not green.
  A wiki page that records one half as evidence has mis-stamped the other half.
- Measured 18 Nov 2026 against the judge run: `[TOTAL](1/1)`, `[OK](1/1)`, exit 0 - the pair, not
  either half. The `[TOTAL]` line preceding `[OK]` with equal counts is what makes `n/n` carry
  content rather than being an echo.
- No shipped instrument enforces the pair. `wiki-lint freshness` reads `LastVerified:` against a
  judge artifact's write date; `Get-WikiJudgeEvidence -RunJudges` (shared shape code, exercised by
  `wiki-reconcile verify`) checks exit 0 only - the `[OK](n/n)` half is checked by nobody outside
  the run that reads stdout. A green stamp in the wiki is therefore only as honest as the agent who
  read the output.
- Its present cost, named by the folder's own rule: the Results log quotes this definition in every
  entry, so a page citing a log line as evidence inherits an unenforced claim - which is why the
  judge vocabulary for invariant pages stops at L15 and treats the log below it as evidence only.

## See also

[[test]] - the exe this judge runs, and the three-binary coverage that bounds it.
[[build-commands]] [[lint]] - the other two sections' rules.
