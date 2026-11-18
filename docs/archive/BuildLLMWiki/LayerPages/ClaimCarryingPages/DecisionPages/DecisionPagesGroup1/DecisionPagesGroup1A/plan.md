---
Type: Plan
Title: "Decision pages D1 to D7"
Parent: "../plan.md"
Level: 5
Items:
  - Title: Write the D1 page
    Status: Atomic
    Content:
  - Title: Write the D2 page
    Status: Atomic
    Content:
  - Title: Write the D3 page
    Status: Atomic
    Content:
  - Title: Write the D4 page
    Status: Atomic
    Content:
  - Title: Write the D5 page
    Status: Atomic
    Content:
  - Title: Write the D6 page
    Status: Atomic
    Content:
  - Title: Write the D7 page
    Status: Atomic
    Content:
---

# Decision pages D1 to D7

## Goal

Seven leaves, each writing exactly one page: `docs/wiki/decisions/D<n>.md` for `n = 1..7`, each
anchored to the commit that made its D-entry.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: CLOSED 18 Nov 2026` - all 7 pages landed (`D1`..`D7`), running stop counts printed
  `  decisions = 1..7` (each `wiki-lint schema` `illegal_fields = 0`, `VERDICT fail=0`). Leaf-plan
  close: `wiki-census pages` `pages = 102`, `  decisions = 7`; `wiki-lint schema` `illegal_fields =
  0`; `wiki-ref-check anchors` `dead = 0`, `VERDICT fail=0`. All 7 carry the D56 sentinel judge (the
  probe names no command for D1-D7); D5 names the collision the `modules/qor_http.md` page already
  carries; D7's reversal is stated in prose (no ledger supersession, so no `SupersededBy:`).

## Context

Inherited - re-read rather than trusted. The page shape is the shipped shape (worked example
`docs/wiki/invariants/lint.md`): `Type: WikiPage`, `PageType: decision`, `Title: "D<n>"`,
`Status: current`, `LastVerified: <run date>`, `Claims: <body count>`, one-line `Summary`,
`Anchors:` `{ kind: in-tree, path: docs/agent/decisions/D<n>.md, anchor: commit:<probe sha>,
verified: <run date> }`, non-empty `Judges:` from the probe row; body = the decision in the entry's
own words, the artifact it governs, and what would make the page stale. No `Decision:`, no
`Supersedes:` - neither is a schema field.

## Decisions

- **The judge is the entry's own recorded command** - the probe already decided each one or named
  why there is none; a leaf never invents a command to satisfy the required-field rule.
- **A dead probe sha stops the leaf, not the wiki** - if `git cat-file -e <sha>` fails at leaf time,
  the leaf prints the failure and does not write an anchor it cannot verify.

## Write the D1 page

Input verified: probe row `D1` (sha green, judge named or reasoned).

One page: `docs/wiki/decisions/D1.md` per the shape above; the body names the D1 decision and the
code or rule it governs as of the anchor commit.

Stop condition: `wiki-census pages` prints `  decisions = 1`; `wiki-lint schema` prints
`illegal_fields = 0`; `wiki-ref-check anchors` prints `dead = 0`.

## Write the D2 page

Input verified: probe row `D2`; `D1` page landed.

One page: `docs/wiki/decisions/D2.md`, same shape.

Stop condition: `  decisions = 2`, `illegal_fields = 0`, `dead = 0`.

## Write the D3 page

Input verified: probe row `D3`; through `D2` landed.

One page: `docs/wiki/decisions/D3.md`, same shape.

Stop condition: `  decisions = 3`, `illegal_fields = 0`, `dead = 0`.

## Write the D4 page

Input verified: probe row `D4`; through `D3` landed.

One page: `docs/wiki/decisions/D4.md`, same shape.

Stop condition: `  decisions = 4`, `illegal_fields = 0`, `dead = 0`.

## Write the D5 page

Input verified: probe row `D5`; through `D4` landed.

One page: `docs/wiki/decisions/D5.md`, same shape.

Stop condition: `  decisions = 5`, `illegal_fields = 0`, `dead = 0`.

## Write the D6 page

Input verified: probe row `D6`; through `D5` landed.

One page: `docs/wiki/decisions/D6.md`, same shape.

Stop condition: `  decisions = 6`, `illegal_fields = 0`, `dead = 0`.

## Write the D7 page

Input verified: probe row `D7`; through `D6` landed.

One page: `docs/wiki/decisions/D7.md`, same shape.

Stop condition: `  decisions = 7` - the leaf-plan close - with `illegal_fields = 0`, `dead = 0`.
