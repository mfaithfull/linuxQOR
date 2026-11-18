---
Type: Plan
Title: "Claim-carrying pages"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Gotcha pages
    Status: Atomic
    Content:
  - Title: Invariant pages
    Status: Atomic
    Content:
  - Title: Concept pages
    Status: Atomic
    Content:
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

- `Cursor: not started` - 3 `Atomic` + 1 `Refined` (6 `Atomic` leaves inside).

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

Input verified: the three `memory-bank/gotchas/` files exist on disk, measured at 3 this session.

Three pages, one per file, enumerated by id: `memory-bank/gotchas/git.md`,
`memory-bank/powershell.md`, `memory-bank/windows.md`. Each wiki page states the gotcha as an
assertion and carries `Judges:` - the command that would reproduce it - plus `Anchors:` into the
file that recorded it, so the wiki's copy is falsifiable against its own source.

Stop condition: `wiki-census pages` re-reads `gotcha = 3 of 3` and `wiki-lint freshness` prints
`unstamped = 0` over them.

## Invariant pages

Input verified: `docs/agent/verification.md` exists and names the build/test commands; `build/`
artifacts exist from the repo's own builds.

Four pages, one per rule section, enumerated by id: `build-commands` (L6), `lint` (L10),
`test` (L13), `green-is-ok-and-exit-zero` (L15). Each states the rule as an assertion and carries
`Judges:` - the command that would show the rule violated - plus `Anchors:` into
`docs/agent/verification.md` with its `pin:` at the current commit.

Stop condition: `wiki-census pages` re-reads `invariants = 4 of 4`, and `wiki-ref-check anchors`
prints `dead = 0` over their anchors.

## Concept pages

Five pages, one per `docs/learning/RCU/RESOURCES.md` row, enumerated by id: the Wikipedia RCU
row (L5), the `cs_libguarded` row (L10), the CsLibGuarded Doxygen row (L16), the CopperSpice
forum row (L35), the r/cpp row (L37). Each carries **no** `Anchors:` and no `Judges:`; it carries
`Type: wiki-concept`, a `See also` block linking the RCU learning page ids, and the line that says
so - "this page describes a domain, not this tree; it is not falsifiable against a build".

Input verified: the five rows are measured above; `docs/learning/` is read for ids only, and its
index is never copied (§13.1 - two catalogs that never sync).

Stop condition: `wiki-census pages` re-reads `concepts = 5 of 5`, and `wiki-lint freshness`
reports these five as **deliberately unstamped** rather than as failures - a lint that cannot
tell a concept page from a broken claim page is itself a finding.

## Decision pages

Six `Atomic` leaves over 55 D-entries, in [`./DecisionPages/plan.md`](./DecisionPages/plan.md).
These are the pages that carry a commit anchor, which is why they are the only wiki pages that
can be stale about **history** rather than about the tree.
