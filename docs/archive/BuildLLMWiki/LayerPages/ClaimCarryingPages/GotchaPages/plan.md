---
Type: Plan
Title: "Gotcha pages"
Parent: "../plan.md"
Level: 3
Items:
  - Title: Probe the three gotcha sources
    Status: Atomic
    Content:
  - Title: Add the gotchas folder purpose
    Status: Atomic
    Content:
  - Title: Write the git gotcha page
    Status: Atomic
    Content:
  - Title: Write the powershell gotcha page
    Status: Atomic
    Content:
  - Title: Write the windows gotcha page
    Status: Atomic
    Content:
  - Title: Rebuild the index and close the pass
    Status: Atomic
    Content:
---

# Gotcha pages

## Goal

Three wiki pages, one per file in `memory-bank/gotchas/`, each an assertion plus the command that
would reproduce the trap, anchored to the file that recorded it - so the wiki's copy can be made red
against its own source.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: CLOSED 18 Nov 2026` - all 6 `Atomic` leaves landed: probe (`gotcha_sources = 3 of 3`,
  `symbol_anchors = 3 of 3`, `reproduction_commands = 3 of 3`), `docs/wiki/gotchas/purpose.md`,
  three pages (`git` anchored on `pager`, `powershell` on `parallel`, `windows` on `api_layer`,
  judges = the recorded reproduction commands), and the index rebuild. Per-leaf stops printed
  `  gotchas = 1`, `2`, `3` in sequence with `illegal_fields = 0` and `dead = 0` throughout.
- **Close line, 18 Nov 2026** (report files `tmp/_tmp_cg_pages|schema|anchors|refs|index|prov|
  fresh|size.txt`): `wiki-index build` -> `index_rows = 91 of 91`, `unindexed = 0`, `groups = 2`,
  index.md 103 lines (was the stale 10-row core-seed build); `wiki-census pages` -> `pages = 91`,
  `  gotchas = 3`, `strays = 0`; `wiki-lint schema` -> `illegal_fields = 0`;
  `wiki-lint purpose` -> `missing_purpose = 0 of 5`, `purpose_files = 5`;
  `wiki-ref-check anchors` -> `anchors = 457 of 457`, `dead = 0`; `wiki-lint size` ->
  `over_budget = 0`; each `VERDICT fail=0`.
- **Findings carried out of the pass** - (1) `wiki-ref-check refs` prints `broken = 1`
  (`modules/qor_netserver.md [wikilink] -> maybe_unused`, a C++ attribute written in wikilink form
  at that page's L73): **pre-existing** (497 of 498 before the pass, 501 of 502 after - the pass
  added 4 links, all resolving), recorded not fixed per the provenance-pair precedent; a one-page
  repair belongs to the first refs-closing op run, not to a page leaf. (2) `wiki-lint provenance`
  unchanged at `violations = 2` (the known pair: `qor_task.md:45` github URL, `qor_signals.md:52`
  `token:` false positive). (3) `wiki-lint freshness` prints all 91 pages `undated`
  (`judges = 91 evidenced = 0`): no shipped judge run names an existing file, informational by the
  tool's own wording - recorded in `docs/wiki/gotchas/purpose.md` as the rule as enforced. The
  `powershell.md` head did not move during the pass; the anchor never needed re-probing.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: write the pages that carry a falsification handle rather than a description.
- **Parent locked decisions**: the judge vocabulary is `docs/agent/verification.md`'s four rule
  sections and not its Results log; `docs/learning/` and `docs/wiki/` never sync; a page that judges
  nothing says so.
- **Sibling plan titles**: `DecisionPages` (`Refined`, 6 `Atomic` leaves) and the parent's two
  remaining `Atomic` items, `Invariant pages` and `Concept pages`.
- **Measured shape facts** (`tools/wiki-census/wiki-shape.ps1`, read 18 Nov 2026): a file is a page
  only when `Type` is exactly `WikiPage` (`Get-WikiPageClass`:188) - any other `Type` is a stray and
  fails `wiki-lint schema`; the required non-empty fields are `Type`, `PageType`, `Title`, `Status`,
  `LastVerified`, `Judges`; `PageType: gotcha` must sit in `gotchas/` or it is `WRONG-FOLDER`; a
  `symbol:` anchor resolves when the literal appears in the file (`Test-WikiSymbolInFile`:369 -
  `[regex]::Escape` behind `(?<![A-Za-z0-9_:])`), so a heading word resolves and a comma may never
  sit inside an inline map; a bare line anchor returns `derived_only`, not `resolved`;
  `wiki-census pages` prints folder counts as `  gotchas = N`; `wiki-lint freshness` fails on
  `unstamped + stale`.
- **Schema row for `gotchas/`** (`docs/wiki/schema.md`:56): "a pointer into `memory-bank/gotchas/`
  plus what the trap now costs" - the page points; the prose stays in `memory-bank/`.

## Decisions

- **Why the parent's `Atomic` line was cut (18 Nov 2026)** - three gate rows fail it. The leaf had to
  discover its own inputs: an anchor symbol nothing had checked, a reproduction command nothing had
  run, and a folder that does not exist. It mixed kinds past 1:1 - 3 page writes plus a folder
  purpose plus 3 symbol lookups plus 3 command lookups plus 4 instrument passes. And its stop
  condition quoted a line no instrument prints: `gotcha = 3 of 3`, where
  `wiki-census pages` prints `  gotchas = N`. The fix is the tree's usual one: a verification leaf,
  one page per leaf, one shared close.
- **One page per gotcha file** - the source count is 3 and the schema row names a pointer per file. A
  file holding several traps still gets one page that names them; the day the census count and the
  source count diverge is the day this decision is wrong, and leaf 1 is what prints it.
- **`in-tree` anchors with a `symbol:`, and no `pin:`** - `memory-bank/` is inside this tree, so the
  F11 pin obligation (which `Resolve-WikiAnchor` applies only to `out-of-tree`) does not attach. The
  heads are recorded in the slot anyway: the day a gotcha file moves, `wiki-ref-check anchors` says
  so through the symbol, not through a pin nothing compares.
- **A gotcha page's `Judges:` is its reproduction command, and no instrument enforces the vocabulary
  on a page** - measured, not assumed: `Test-WikiJudgeInVocabulary` (`wiki-shape.ps1`:533) is called
  by none of the five instruments, and `docs/wiki/invariants/purpose.md`:13 asserts a lint that does
  not exist. Recorded rather than hidden, because it has a consequence: a `git`-headed judge is legal
  to the instruments, and `wiki-lint freshness` prints the page `undated` (informational) when the
  judge names no file. If the governance layer wants gotchas judged inside the vocabulary, that is a
  schema change and a D-entry, not a leaf here.

## Probe the three gotcha sources

Input verified: the 3 files exist at the heads recorded in the slot; `docs/wiki/gotchas/` does not
exist; the field shape and the anchor resolver are measured in Context.

Three lookups, enumerated by id: `git.md`, `powershell.md`, `windows.md`. Read each file once and
write three values into the probe slot: the `symbol:` string taken from the file itself (a heading
word that will resolve, never invented); the command that reproduces the trap, with its head word
noted so the judge maps onto `Get-WikiJudgeHead` honestly; and the one clause saying what the trap
costs **now**, which is the half of the schema row that is not a pointer.

Stop condition: one run prints `gotcha_sources = 3 of 3`, `symbol_anchors = 3 of 3`,
`reproduction_commands = 3 of 3`, and the slot carries the nine values.

## Add the gotchas folder purpose

Input verified: the probe slot holds nine values; `wiki-lint purpose` currently reports the folder as
out of scope only because it does not exist.

One write: `docs/wiki/gotchas/purpose.md` - what belongs here (an assertion, the trap's present cost,
a pointer into `memory-bank/gotchas/`), what does not (the gotcha's own prose stays in
`memory-bank/`; a rule belongs in `invariants/`), and the judge rule as the instruments actually
enforce it rather than as `docs/wiki/invariants/purpose.md`:13 states it.

Stop condition: `wiki-lint purpose` prints `missing_purpose = 0` over `docs/wiki/`, and
`wiki-census pages` still prints `  gotchas = 0` with `purpose_files = 5` - a purpose file is not a
page.

## Write the git gotcha page

Input verified: the probe slot's `git.md` row and `docs/wiki/gotchas/purpose.md`.

One page: `docs/wiki/gotchas/git.md` - `Type: WikiPage`, `PageType: gotcha`, `Status: current`,
`LastVerified:` the run's date, one anchor
`{ kind: in-tree, path: memory-bank/gotchas/git.md, symbol: <slot symbol>, verified: <run date> }`,
`Judges:` the slot's reproduction command, and a body that asserts the trap, states its present cost,
and points at the source instead of restating it.

Stop condition: `wiki-census pages` prints `  gotchas = 1`; `wiki-lint schema` prints
`illegal_fields = 0` with `strays = 0`; `wiki-ref-check anchors` prints `dead = 0`.

## Write the powershell gotcha page

Input verified: `  gotchas = 1` printed by the leaf above, and the slot's `powershell.md` row.

One page: `docs/wiki/gotchas/powershell.md`, same shape as the leaf above, anchored to
`memory-bank/gotchas/powershell.md`. That source's head is today's date - it is the one input that
may move during this pass - so if its symbol stops resolving, `wiki-ref-check anchors` prints `dead`
and the leaf re-probes rather than inventing a string.

Stop condition: `  gotchas = 2`, `illegal_fields = 0`, `dead = 0`.

## Write the windows gotcha page

Input verified: `  gotchas = 2` printed by the leaf above, and the slot's `windows.md` row.

One page: `docs/wiki/gotchas/windows.md`, same shape, anchored to `memory-bank/gotchas/windows.md`.
That source forwards to `src/platform/os/windows/api_layer/user/gotchas.md`; the page carries that as
a prose pointer, and its `Anchors:` stay on the `memory-bank/` file, because a second anchor source
is a unit this leaf was not given.

Stop condition: `  gotchas = 3` - the census against the disk listing of `memory-bank/gotchas/`, not
against prose - with `illegal_fields = 0`, `strays = 0`, `dead = 0`.

## Rebuild the index and close the pass

Input verified: `  gotchas = 3`, `illegal_fields = 0`, `dead = 0` printed by the leaf above.

One generator run and one slot write: `wiki-index build` rebuilds `docs/wiki/index.md` from the pages
(it is generated, never edited - op 12), and this plan's close line goes into this slot and the
parent's. The measured baseline is 10 rows over 88 pages, so the expected print is
`index_rows = 91 of 91` with `groups = 2` (`gotcha`, `module`).

Stop condition: `wiki-index build` prints `index_rows = 91 of 91` with `unindexed = 0`, and
`wiki-census pages` prints `pages = 91` with `  gotchas = 3`.
