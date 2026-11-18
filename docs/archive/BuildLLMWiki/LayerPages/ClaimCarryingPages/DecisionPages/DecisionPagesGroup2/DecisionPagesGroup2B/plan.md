---
Type: Plan
Title: "Decision pages D22 to 28"
Parent: "../plan.md"
Level: 5
Items:
  - Title: Write the D22 page
    Status: Atomic
    Content:
  - Title: Write the D23 page
    Status: Atomic
    Content:
  - Title: Write the D24 page
    Status: Atomic
    Content:
  - Title: Write the D25 page
    Status: Atomic
    Content:
  - Title: Write the D26 page
    Status: Atomic
    Content:
  - Title: Write the D27 page
    Status: Atomic
    Content:
  - Title: Write the D28 page
    Status: Atomic
    Content:
---

# Decision pages D22 to 28

## Goal

Seven leaves, each writing exactly one page: `docs/wiki/decisions/D<n>.md` for
`n = 22..28`, each anchored to the commit that made its D-entry.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: CLOSED 18 Nov 2026 - all seven pages landed` (`D22`..`D28`). Stop counts met:
  census `  decisions = 28`, schema `illegal_fields = 0`, ref-check `anchors = 489 of 489`,
  `dead = 0`, `VERDICT fail=0`. Judges: folder `/W4` gate recorded-green on `D22`/`D23`/`D24`/`D25`/`D27`/`D28`;
  `D26` carries the D56 sentinel (policy, touches no source; census instruments throwaway). All four
  probe shas re-verified green at leaf time (`git cat-file -e`): `8a538e49...9b87` (D22-D25),
  `fd6e761b...0d31` (D26), `10b59ad7...1597` (D27), `4dc465c0...5145` (D28).

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

## Write the D22 page

Input verified: probe row `D22` (sha green, judge named or reasoned)

One page: `docs/wiki/decisions/D22.md`, same shape.

Stop condition: `  decisions = 22`, `illegal_fields = 0`, `dead = 0`.
## Write the D23 page

Input verified: probe row `D23`; through `D22` landed

One page: `docs/wiki/decisions/D23.md`, same shape.

Stop condition: `  decisions = 23`, `illegal_fields = 0`, `dead = 0`.
## Write the D24 page

Input verified: probe row `D24`; through `D23` landed

One page: `docs/wiki/decisions/D24.md`, same shape.

Stop condition: `  decisions = 24`, `illegal_fields = 0`, `dead = 0`.
## Write the D25 page

Input verified: probe row `D25`; through `D24` landed

One page: `docs/wiki/decisions/D25.md`, same shape.

Stop condition: `  decisions = 25`, `illegal_fields = 0`, `dead = 0`.
## Write the D26 page

Input verified: probe row `D26`; through `D25` landed

One page: `docs/wiki/decisions/D26.md`, same shape.

Stop condition: `  decisions = 26`, `illegal_fields = 0`, `dead = 0`.
## Write the D27 page

Input verified: probe row `D27`; through `D26` landed

One page: `docs/wiki/decisions/D27.md`, same shape.

Stop condition: `  decisions = 27`, `illegal_fields = 0`, `dead = 0`.
## Write the D28 page

Input verified: probe row `D28`; through `D27` landed

One page: `docs/wiki/decisions/D28.md`, same shape.

Stop condition: `  decisions = 28` - the leaf-plan close - with `illegal_fields = 0`, `dead = 0`.