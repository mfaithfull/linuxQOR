---
Type: Plan
Title: "Claim-carrying pages"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Gotcha pages
    Status: Refined
    Content: ./GotchaPages/plan.md
  - Title: Invariant pages
    Status: Refined
    Content: ./InvariantPages/plan.md
  - Title: Concept pages
    Status: Refined
    Content: ./ConceptPages/plan.md
  - Title: Decision pages
    Status: Refined
    Content: ./DecisionPages/plan.md
---

# Claim-carrying pages

## Goal

Write the pages that carry a falsification handle rather than a description - the pages whose
whole point is that they can be made red.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `PASS CLOSED 18 Nov 2026 - all four items closed. Item 3 `Concept pages` closed last: cut to
  [`./ConceptPages/plan.md`](./ConceptPages/plan.md) (7 Atomic leaves) the same day, purpose page
  + five concept pages landed, close line `  concepts = 5`, `purpose_files = 7`, index
  `index_rows = 156 of 156 / groups = 5`, anchors `516 of 516, dead = 0`, schema
  `illegal_fields = 0`, freshness `stale = 0` (refs close-run prints the pre-existing
  `broken = 1` at `modules/qor_netserver.md:73`, unchanged by this pass).` Item 4 `Decision pages` **CLOSED 18 Nov 2026** - all 56 pages landed
  (`docs/wiki/decisions/D1.md..D56.md` + `purpose.md`) across `Group1..Group4`; close line: census
  `  decisions = 56`, `wiki-index build` `index_rows = 151 of 151`, `unindexed = 0`, `groups = 4`;
  ref-check anchors `516 of 516`, `dead = 0`; schema `illegal_fields = 0`; freshness `stale = 0`,
  `undated = 143` informational; every `VERDICT fail=0` (reports `tmp/_tmp_dp_*.txt`). The D56 page
  carries **no anchor** (`anchor = NO-COMMIT` - `docs/agent/decisions/D56.md` is uncommitted; fill
  after the root `Records and commit`). Judges: each page from its entry's own recorded command; the
  20 command-less pages carry the D56 sentinel; `SupersededBy:` carried by no page (measured).
  Item 1 `Gotcha pages` and item 2 `Invariant pages` close lines stand as previously recorded.
- `Concept pages inputs (measured 18 Nov 2026): five rows in docs/learning/RCU/RESOURCES.md
  (L5 Wikipedia RCU, L10 cs_libguarded, L16 Doxygen, L35 CopperSpice forum, L37 r/cpp); shape per
  D56 = Type: WikiPage, PageType: concept, sentinel Judges, no Anchors, the domain-not-tree
  sentence in the body; census must reach `  concepts = 5` and wiki-index close groups = 5.`
  Item 1 `Gotcha pages` **CLOSED 18 Nov 2026** (6 leaves: purpose, 3 pages, index rebuild;
  close line `index_rows = 91 of 91` `groups = 2`, `pages = 91`, `  gotchas = 3`,
  `anchors = 457 of 457`, `dead = 0`, `missing_purpose = 0 of 5`, each `VERDICT fail=0`, reports
  `tmp/_tmp_cg_*.txt`; it found the pre-existing `broken = 1` at `modules/qor_netserver.md:73` and
  the all-`undated` freshness state, both recorded not fixed). Item 2 `Invariant pages` **CLOSED
  18 Nov 2026** - cut from `Atomic` the same day (four page
  writes past 1:1, wrong inputs) into
  [`./InvariantPages/plan.md`](./InvariantPages/plan.md), 6 `Atomic` leaves: probe, one page per
  rule section, index close. Close line: `index_rows = 95 of 95` `unindexed = 0` `groups = 3`;
  `pages = 95`, `  invariants = 4`, `strays = 0`; `illegal_fields = 0`;
  `anchors = 461 of 461`, `dead = 0`, `unpinned = 0`; each `VERDICT fail=0`
  (reports `tmp/_tmp_iv_*.txt`). Two live findings sit on those pages by design: the `lint` page's
  judge is **red by configuration** (no `compile_commands.json` in either configured tree), and the
  `## Test` rule's example exe is not configured - only 3 `QORTest*.exe` exist. `wiki-lint
  freshness` now prints `evidenced = 3` (the wiki's first dated pages: the exe and .cpp artifacts
  the judges name) - it moved off 0 because the judges name real files. `wiki-ref-check refs`
  unchanged: one pre-existing BROKEN (`modules/qor_netserver.md:73`).
- `Baseline measured 18 Nov 2026` - `wiki-census pages` prints `files = 94`, `pages = 88`,
  `strays = 0`, `purpose_files = 4`, `  gotchas = 0`; `docs/wiki/index.md` is stale at **10** `- [[`
  rows over 88 pages, so the index rebuild is part of the gotcha pass, not a later chore.
- `Remaining` - none; all four items closed. Baseline now: census `files = 165`, `pages = 156`,
   `  concepts = 5` `  decisions = 56` `  gotchas = 3` `  invariants = 4`, `index_rows = 156 of
   156` `groups = 5`.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: populate the wiki bounded by 90 distinct build targets.
- **Parent locked decisions**: two catalogs that never sync (`docs/learning/` keeps its own index
  and vocabulary); a page's `Judges:` are its target's build commands; the qmd rung is not built.
- **Sibling plan titles**: `SeedPages`, `FrameworkLibPages`, `ComponentsLibPages`.
- **Measured sources**: `memory-bank/gotchas/` holds **3** files (`git.md`, `powershell.md`,
  `windows.md`); `docs/agent/verification.md` is **529** lines with **129** `- ` bullets and
  **27** headings, of which the **4** rule sections (`## Build commands (incremental)` L6,
  `## Lint` L10, `## Test` L13, the green rule L15) sit above the `## Results` log at L17;
  `docs/learning/RCU/RESOURCES.md` has **5** `- [` rows; `docs/agent/decisions/` holds **55**
  files, `D1.md`..`D55.md`.

## Decisions

- **Concept pages carry no `Anchors:` and no `Judges:`** - §13.1's fourth answer. A concept page
  describes a domain, not this tree; the absence is deliberate and is what keeps `docs/learning/`
  from being mistaken for claims. A page that judges nothing must say so, because `wiki-lint
  freshness` treats a missing `Judges:` on a **claim** page as a failure.
- **The judge vocabulary is the four rule sections, not the 129 bullets** - measured. The Results
  log is evidence, not a rule; treating a log line as an invariant would make the wiki assert
  yesterday's run as today's rule.

## Gotcha pages

See [`./GotchaPages/plan.md`](./GotchaPages/plan.md) - 6 `Atomic`: probe the three sources for the
symbol / reproduction-command / present-cost triple, write `docs/wiki/gotchas/purpose.md`, one page
per source (`git`, `powershell`, `windows`), then `wiki-index build` as the close.

Two measured corrections to what this section asserted: the ids are `memory-bank/gotchas/git.md`,
`memory-bank/gotchas/powershell.md`, `memory-bank/gotchas/windows.md` (the prose above dropped the
folder from the last two), and the stop line is `  gotchas = 3` as printed by `wiki-census pages`,
not `gotcha = 3 of 3`.

## Invariant pages

See [`./InvariantPages/plan.md`](./InvariantPages/plan.md) - 6 `Atomic` (probe, one leaf per page,
index close), **CLOSED 18 Nov 2026**. Four pages, one per rule section: `build-commands` (L6),
`lint` (L10), `test` (L13), `green-is-ok-and-exit-zero` (L15). Two measured corrections to the
`Atomic` prose above: the item said "with its `pin:` at the current commit" - pins bind only to
`out-of-tree` (`Resolve-WikiAnchor` / `wiki-ref-check anchors` counts `unpinned` from out-of-tree
only), so these in-tree anchors carry `symbol:` and no pin, head `2026-10-26 54116800` in the
child's slot; and the item's canonical test exe (`QORTestMIME.exe`) is not configured - the child's
probe found 3 configured exes and re-pointed the judges at a real one. The folder
`docs/wiki/invariants/purpose.md` exists from the scaffolding pass and was not touched here; its
L13 lint assertion is **measured false** (`Test-WikiJudgeInVocabulary` called by no shipped
instrument) - fixing that assertion is a governance edit, recorded in the child's Context.

## Concept pages

Five pages, one per `docs/learning/RCU/RESOURCES.md` row, enumerated by id: the Wikipedia RCU
row (L5), the `cs_libguarded` row (L10), the CsLibGuarded Doxygen row (L16), the CopperSpice
forum row (L35), the r/cpp row (L37). Each carries **no** `Anchors:` and no `Judges:`; it carries
`Type: wiki-concept`, a `See also` block linking the RCU learning page ids, and the line that says
so - "this page describes a domain, not this tree; it is not falsifiable against a build".

Input verified: the five rows are measured above; `docs/learning/` is read for ids only, and its
index is never copied (§13.1 - two catalogs that never sync).

Stop condition: `wiki-census pages` re-reads `  concepts = 5`, and `wiki-lint freshness`
reports these five as **deliberately unstamped** rather than as failures - a lint that cannot
tell a concept page from a broken claim page is itself a finding.

Measured 18 Nov 2026, for whoever refines this item: two of its phrases are not instrument-legal as
written. `wiki-lint schema` requires `Judges` non-empty and an empty block list casts to `''`
(`wiki-lint.ps1`:184, `[string]@()` measured `eqempty=True`), so a concept page with a bare
`Judges:` prints `MISSING-FIELD Judges`; and `Type: wiki-concept` is not a page at all, because
`Get-WikiPageClass` accepts only `Type: WikiPage` and files with any other `Type` come back as
strays and fail the schema run. Resolving this is a schema question with a D-entry, not a leaf.

**RESOLVED 18 Nov 2026 by D56** (`docs/agent/decisions/D56.md`): a concept page is
`Type: WikiPage`, `PageType: concept` (shipped vocabulary), with the sentinel
`Judges: none - this page describes a domain, not this tree; it is not falsifiable against a build`
(non-empty passes the required-field rule; the sentence above stays, in the body) and no `Anchors:`;
freshness leaves them `undated`, informational. The item is unblocked; the cut is the next act, and
the pass close line must expect `wiki-index build` `groups = 5` once `concept` joins the other four.

## Decision pages

One wiki page per recorded decision, each carrying the commit that made it - the only wiki pages
whose anchor is a commit rather than a path, which is why they are the only wiki pages that can be
stale about **history** rather than about the tree. In
[`./DecisionPages/plan.md`](./DecisionPages/plan.md).

**Re-cut charted 18 Nov 2026.** The six 10-page slices are gone: they broke the 16 Nov 2026
one-page-per-leaf rule, and the flat 55-leaf alternative breaks the 7-item breadth cap
(`tmp/_tmp_llmwiki_tree_verify.ps1` `BREADTH-FAIL` L40; 7 x 7 = 49 < 55 + probe + purpose + close).
The child now holds `Probe`, `Write the decisions folder purpose page` and `Close` as `Atomic` -
the purpose leaf is in the child, not a group, because `docs/wiki/decisions/` does not exist at all
(measured) - over four `Refined` groups of 11 (`DecisionPagesGroup1..4`, Level 4), each holding two
`Level 5` leaf-plans of 7 pages (`..Group1A`/`1B` ... `4A`/`4B`): 55 `Atomic` one-page leaves, ids
enumerated, in D-id order, running stop counts `  decisions = 1..55`. Shape facts carried into every
leaf: `PageType: decision` is in the shipped vocabulary (`tools/wiki-census/wiki-shape.ps1`:33);
`Decision:` and `Supersedes:` are **not** schema fields (`WikiPageFields`, wiki-shape.ps1:40-41)
while `SupersededBy:` is; `Judges:` is required non-empty (wiki-shape.ps1:43), so the probe supplies
each page's judge from the entry's own recorded command; and the anchor
`{ kind: in-tree, path: docs/agent/decisions/D<n>.md, anchor: commit:<sha>, verified: <date> }`
resolves through `Resolve-WikiAnchor` (wiki-shape.ps1:423) via `git cat-file -e <sha>^{commit}`,
printing `dead` on a moved path - the history detector, no schema change needed. Re-verify after
the re-cut: `plans=41 levels=0,1,2,3,4,5 atomic=170 refined=40 planning=0 VERDICT fail=0`.
Nothing is written yet - the probe leaf is first.
