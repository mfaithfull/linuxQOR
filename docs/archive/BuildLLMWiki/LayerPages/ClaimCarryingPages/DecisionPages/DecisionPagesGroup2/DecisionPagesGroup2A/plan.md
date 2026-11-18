---
Type: Plan
Title: "Decision pages D15 to 21"
Parent: "../plan.md"
Level: 5
Items:
  - Title: Write the D15 page
    Status: Atomic
    Content:
  - Title: Write the D16 page
    Status: Atomic
    Content:
  - Title: Write the D17 page
    Status: Atomic
    Content:
  - Title: Write the D18 page
    Status: Atomic
    Content:
  - Title: Write the D19 page
    Status: Atomic
    Content:
  - Title: Write the D20 page
    Status: Atomic
    Content:
  - Title: Write the D21 page
    Status: Atomic
    Content:
---

# Decision pages D15 to 21

## Goal

Seven leaves, each writing exactly one page: `docs/wiki/decisions/D<n>.md` for
`n = 15..21`, each anchored to the commit that made its D-entry.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: CLOSED 18 Nov 2026` - all 7 pages landed (`D15`..`D21`), running stop counts printed
  `  decisions = 15..21` (each `VERDICT fail=0`). Leaf-plan close: `wiki-census pages` `pages =
  116`, `  decisions = 21`; `wiki-lint schema` `illegal_fields = 0`; `wiki-ref-check anchors`
  `dead = 0`, each `VERDICT fail=0`. Judges: D15/D16/D20/D21 the folder `cl /W4` gate line;
  D17/D18/D19 the D56 sentinel (scope/purpose decisions touching no source, evidence in throwaway
  `tmp/` instruments).

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

## Write the D15 page

Input verified: probe row `D15` (sha green, judge named or reasoned)

One page: `docs/wiki/decisions/D15.md`, same shape.

Stop condition: `  decisions = 15`, `illegal_fields = 0`, `dead = 0`.
## Write the D16 page

Input verified: probe row `D16`; through `D15` landed

One page: `docs/wiki/decisions/D16.md`, same shape.

Stop condition: `  decisions = 16`, `illegal_fields = 0`, `dead = 0`.
## Write the D17 page

Input verified: probe row `D17`; through `D16` landed

One page: `docs/wiki/decisions/D17.md`, same shape.

Stop condition: `  decisions = 17`, `illegal_fields = 0`, `dead = 0`.
## Write the D18 page

Input verified: probe row `D18`; through `D17` landed

One page: `docs/wiki/decisions/D18.md`, same shape.

Stop condition: `  decisions = 18`, `illegal_fields = 0`, `dead = 0`.
## Write the D19 page

Input verified: probe row `D19`; through `D18` landed

One page: `docs/wiki/decisions/D19.md`, same shape.

Stop condition: `  decisions = 19`, `illegal_fields = 0`, `dead = 0`.
## Write the D20 page

Input verified: probe row `D20`; through `D19` landed

One page: `docs/wiki/decisions/D20.md`, same shape.

Stop condition: `  decisions = 20`, `illegal_fields = 0`, `dead = 0`.
## Write the D21 page

Input verified: probe row `D21`; through `D20` landed

One page: `docs/wiki/decisions/D21.md`, same shape.

Stop condition: `  decisions = 21` - the leaf-plan close - with `illegal_fields = 0`, `dead = 0`.