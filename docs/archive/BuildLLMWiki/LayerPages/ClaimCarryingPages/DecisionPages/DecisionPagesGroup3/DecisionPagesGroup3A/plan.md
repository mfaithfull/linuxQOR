---
Type: Plan
Title: "Decision pages D29 to 35"
Parent: "../plan.md"
Level: 5
Items:
  - Title: Write the D29 page
    Status: Atomic
    Content:
  - Title: Write the D30 page
    Status: Atomic
    Content:
  - Title: Write the D31 page
    Status: Atomic
    Content:
  - Title: Write the D32 page
    Status: Atomic
    Content:
  - Title: Write the D33 page
    Status: Atomic
    Content:
  - Title: Write the D34 page
    Status: Atomic
    Content:
  - Title: Write the D35 page
    Status: Atomic
    Content:
---

# Decision pages D29 to 35

## Goal

Seven leaves, each writing exactly one page: `docs/wiki/decisions/D<n>.md` for
`n = 29..35`, each anchored to the commit that made its D-entry.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: CLOSED 18 Nov 2026 - all seven pages landed` (`D29`..`D35`). Stop counts met: census
  `  decisions = 35`, schema `illegal_fields = 0`, ref-check `anchors = 496 of 496`, `dead = 0`,
  `VERDICT fail=0`. Judges: folder `/W4` gate recorded-green on `D29`-`D34` (each entry names the
  gate leg itself; `D33`'s probe row reads none because its gate is spelled in prose - the entry's
  own gate section is the recorded command); `D35` carries the D56 sentinel (policy, no code change,
  proof is source-read). All seven probe shas re-verified green at leaf time.

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

## Write the D29 page

Input verified: probe row `D29` (sha green, judge named or reasoned)

One page: `docs/wiki/decisions/D29.md`, same shape.

Stop condition: `  decisions = 29`, `illegal_fields = 0`, `dead = 0`.
## Write the D30 page

Input verified: probe row `D30`; through `D29` landed

One page: `docs/wiki/decisions/D30.md`, same shape.

Stop condition: `  decisions = 30`, `illegal_fields = 0`, `dead = 0`.
## Write the D31 page

Input verified: probe row `D31`; through `D30` landed

One page: `docs/wiki/decisions/D31.md`, same shape.

Stop condition: `  decisions = 31`, `illegal_fields = 0`, `dead = 0`.
## Write the D32 page

Input verified: probe row `D32`; through `D31` landed

One page: `docs/wiki/decisions/D32.md`, same shape.

Stop condition: `  decisions = 32`, `illegal_fields = 0`, `dead = 0`.
## Write the D33 page

Input verified: probe row `D33`; through `D32` landed

One page: `docs/wiki/decisions/D33.md`, same shape.

Stop condition: `  decisions = 33`, `illegal_fields = 0`, `dead = 0`.
## Write the D34 page

Input verified: probe row `D34`; through `D33` landed

One page: `docs/wiki/decisions/D34.md`, same shape.

Stop condition: `  decisions = 34`, `illegal_fields = 0`, `dead = 0`.
## Write the D35 page

Input verified: probe row `D35`; through `D34` landed

One page: `docs/wiki/decisions/D35.md`, same shape.

Stop condition: `  decisions = 35` - the leaf-plan close - with `illegal_fields = 0`, `dead = 0`.