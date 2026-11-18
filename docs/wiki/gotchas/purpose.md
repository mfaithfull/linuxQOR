# Purpose - docs/wiki/gotchas

One page per file in `memory-bank/gotchas/` - an assertion that the trap is real, what it costs
**now**, and a pointer into the file that recorded it (`docs/wiki/schema.md` section "Pages"). The
prose stays in `memory-bank/`; a page that restates it has forked the truth.

- The count is the source-file count, not a feeling: `wiki-census pages` prints `  gotchas = N` and
  the day it diverges from `Get-ChildItem memory-bank/gotchas -File` is a finding.
- `Anchors:` are `in-tree` with a `symbol:` taken from a heading of the source file - no `pin:`,
  because `memory-bank/` is inside this tree and F11 pins only out-of-tree anchors. A heading that
  stops resolving prints `dead` in `wiki-ref-check anchors`; the fix is to re-read the source, not
  to invent a string.
- `Judges:` is the command that reproduces the trap, and this is the rule **as the instruments
  enforce it**, not as `docs/wiki/invariants/purpose.md` states it: `wiki-lint schema` only requires
  `Judges:` present and non-empty; `Test-WikiJudgeInVocabulary` (`tools/wiki-census/wiki-shape.ps1`)
  is called by no shipped instrument, so a reproduction command outside the verification vocabulary
  is legal to the tools. A judge that names no file leaves the page `undated` in `wiki-lint
  freshness` - informational, not a failure - so re-running the recorded command is the human
  half of falsifying these pages.
- Not here: the gotcha's own prose and measurements (those stay in `memory-bank/gotchas/`), rules
  stated as assertions (`invariants/`), or a trap that is really a bug worth a D-entry
  (`docs/agent/decisions/`).
