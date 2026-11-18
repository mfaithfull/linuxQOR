---
Type: Tool
Scripts:
  - File: tools/wiki-census/wiki-census.ps1
    Usage: "-Command <pending|targets|coverage|pages> (required) -WikiRoot <dir> (required) -RawRoot <dir> (required for pending) -TreeRoot <dir> (required for targets and coverage; src is derived as <TreeRoot>/src and the resolved path is printed) -Report <file> (required). -Help prints the subcommands and touches no file."
    Output: "the -Report file and the same lines on stdout; the report path is printed as 'report = <path>'. Reports belong in tmp/."
  - File: tools/wiki-census/wiki-shape.ps1
    Usage: "dot-sourced, never invoked - the shared shape every wiki instrument reads: the parse, the page listing, the anchor resolver, the queue entry, the judge vocabulary, the run plumbing."
    Output: "nothing - defining functions is its whole effect; its version prints as 'shape=wiki-shape-N' on every instrument run."
---

# wiki-census

## What it does

Reports the wiki's state as counts, so "up to date" is a printed line and not a sentence.
Four subcommands, one per question the wiki is asked: `pending` (is anything un-compiled),
`targets` (which build units exist), `coverage` (which units have a page), `pages` (what exists
and where it sits).

The parse it counts is **the shared shape** for the other five wiki instruments
(`docs/WikiDesign.md` §5 "one shape", `LayerInstruments/RefIndexInstruments` "the parse and page
listing come from wiki-census"). It lives beside the dispatcher, in `wiki-shape.ps1`, so an
instrument never has to run the census to read it. Dot-source it and the shape is yours:

```powershell
. (Join-Path $repoRoot 'tools/wiki-census/wiki-shape.ps1')
```

What that defines: `Read-WikiLines` (one read path, BOM stripped), `Split-WikiDocument` +
`ConvertFrom-WikiFrontMatter` (the page parse), `Get-WikiPage` / `Get-WikiPageClass` (the page
listing and the page/nav/purpose/stray classification), `Get-WikiAnchorRecord` (a page's
`Anchors:` in the queue's words), `Get-WikiWikilink` / `Resolve-WikiWikilink`,
`Get-WikiQueueEntry` / `Test-WikiQueueEntryComplete` (the nine-field entry),
`Get-WikiTargetDeclaration` (`set(target qor_`), `Resolve-WikiAnchor` (every anchor kind, shared by
ref-check and reconcile so "dead" means one thing), `Get-WikiJudgeVocabulary` (the green commands
of `docs/agent/verification.md`), `Get-WikiProseAnchorCandidate`, and the run plumbing
(`Exit-WikiUsage`, `Write-WikiRun`, `Resolve-WikiRequiredRoot`, `Get-WikiDateOnly`). The dispatcher
at the tail runs only when the file is invoked directly, so dot-sourcing defines functions and
writes nothing.

## Contract

Required args, no defaults: `-Command`, `-WikiRoot`, `-Report`, plus `-RawRoot` for `pending` and
`-TreeRoot` for `targets`/`coverage`. A missing or unreadable one prints `USAGE`, then
`MISSING args=<name>`, then `VERDICT fail=1`, exits `1`, and touches no file. Nothing encodes a
task: the wiki root, the raw root and the tree root are all arguments.

Verdict lines, not exit codes, are the answer (`VERDICT fail=N`, and the exit code follows it):

```
queue = docs/raw/pending.md
pending = 0 of 0
consumed = 0
entry_fields = 9  [id added_by kind path anchor pin verified target why]
src_root = src
cmakelists = 208
scan_unreadable = 0 of 208
declarations_qor = 91
targets = 90 of 90
coverage = 0 of 90
uncovered_targets = 90
files = 4
pages = 0
strays = 0
```

Claimed and seen are printed separately: `0 of 0` is never a pass. `pending` prints a `vacuity:`
line over an empty queue, `pages` prints one over an empty wiki. A `targets = 0 of 0` over a tree
that has CMakeLists.txt is a **failure**, not an empty wiki - the scan has real input.
`coverage` reports and does not judge: the verdict pair is `pending = 0` plus
`wiki-reconcile`'s `VERDICT fail=0`.

`docs/raw/pending.md` holds no ids once ingested (an entry is *consumed*, §9(5)), so `consumed`
counts the `Sources:` ids held by pages and says so on the line.

## Provenance

Written 14 Nov 2026 for `docs/future/BuildLLMWiki/LayerInstruments/CensusInstrument/plan.md`; the
scan is the promoted form of `tmp/_tmp_llmwiki_scaffold_measure.ps1` from the scaffolding pass,
which measured 208 CMakeLists.txt, 91 `set(target qor_` declarations, 90 distinct.

Bugs it fixes, each found by running it against the real tree:

- `Split-Path -LiteralPath X -Parent` is **not a valid parameter set** in PowerShell 7.7 - it throws
  "Parameter set cannot be resolved", leaves the variable `$null`, and the next `Test-Path` then
  fails three lines later, far from the cause. Use `[IO.Path]::GetDirectoryName()` or `-Path`.
- `Get-ChildItem -LiteralPath` on a path whose type is unknown returns **nothing** rather than
  erroring, so an unreadable artifact looks like a clean zero. `targets` therefore lists every
  entry named `CMakeLists.txt` and prints `scan_unreadable = N of M`, which is the deliberate FAIL
  line used in the self-test (a folder named `CMakeLists.txt` is reported, not skipped).
- `Get-Content` keeps a UTF-8 BOM on line 1 unless the read is `-Encoding utf8`; one read path
  (`Read-WikiLines`) strips it so a front-matter fence is never missed.
- `Group-Object`'s `.Name` is a `GroupInfo` member, **not** a property of the grouped items:
  `$items | Group-Object -Property Name` does not bind. Group with `@()` around a `Where-Object`
  or index with `[0]` - a bare pipeline `[0]` returns element zero, not a one-element array.
- Parameters of a dot-sourceable script must not be `[Mandatory]`: a Mandatory parameter prompts
  when the file is dot-sourced with no arguments, which hangs the caller. Requiredness is enforced
  in the dispatcher instead. The same trap one layer down cost a **false green**: `Resolve-WikiRequiredRoot`
  had `[Parameter(Mandatory)]$Value`, so passing `''` threw a *non-terminating* bind error, the
  script printed the error and **ran on** with a `$null` root to `VERDICT fail=0`. Requiredness is
  now tested by the caller before the value is passed, and the self-tests assert `Cannot bind` is
  absent wherever `VERDICT fail=1` is asserted.
- `$MyInvocation.InvocationName -ne '.'` is the dispatcher guard (verified against the five real
  invocation forms: direct, `&` relative, `&` absolute, `pwsh -File`, dot-source). An unclosed
  brace anywhere above it silently swallows the dispatcher and the whole file still parses - the
  self-test exists to catch exactly that.
