---
Type: Plan
Title: "Decision pages D50 to 56"
Parent: "../plan.md"
Level: 5
Items:
  - Title: Write the D50 page
    Status: Atomic
    Content:
  - Title: Write the D51 page
    Status: Atomic
    Content:
  - Title: Write the D52 page
    Status: Atomic
    Content:
  - Title: Write the D53 page
    Status: Atomic
    Content:
  - Title: Write the D54 page
    Status: Atomic
    Content:
  - Title: Write the D55 page
    Status: Atomic
    Content:
  - Title: Write the D56 page
    Status: Atomic
    Content:
---

# Decision pages D50 to 56

## Goal

Seven leaves, each writing exactly one page: `docs/wiki/decisions/D<n>.md` for
`n = 50..56`, each anchored to the commit that made its D-entry.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `CLOSED 18 Nov 2026 - seven pages landed, D50..D56; close-line census `  decisions = 56`,
  schema `illegal_fields = 0`, ref-check anchors `516 of 516, dead = 0`. Judges: D50/D55 the folder
  gate cl leg, D51 the cl leg (numstat provenance), D53 the standalone /W4 batch legs, D54/D52 the
  D56 sentinel (tmp ledger/census/table provenance), D56 the schema-lint instrument itself. The D56
  page landed with NO Anchors block (entry uncommitted, anchor = NO-COMMIT - fill after the root
  Records and commit).`

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

## Write the D50 page

Input verified: probe row `D50` (sha green, judge named or reasoned)

One page: `docs/wiki/decisions/D50.md`, same shape.
The probe row names the 29 Oct entry D50 reverses: that older page - written earlier in this pass,
per its probe row - carries the legal `SupersededBy:` naming D50; this page states the reversal in
prose and adds no undeclared field.

Stop condition: `  decisions = 50`, `illegal_fields = 0`, `dead = 0`.
## Write the D51 page

Input verified: probe row `D51`; through `D50` landed

One page: `docs/wiki/decisions/D51.md`, same shape.

Stop condition: `  decisions = 51`, `illegal_fields = 0`, `dead = 0`.
## Write the D52 page

Input verified: probe row `D52`; through `D51` landed

One page: `docs/wiki/decisions/D52.md`, same shape.

Stop condition: `  decisions = 52`, `illegal_fields = 0`, `dead = 0`.
## Write the D53 page

Input verified: probe row `D53`; through `D52` landed

One page: `docs/wiki/decisions/D53.md`, same shape.

Stop condition: `  decisions = 53`, `illegal_fields = 0`, `dead = 0`.
## Write the D54 page

Input verified: probe row `D54`; through `D53` landed

One page: `docs/wiki/decisions/D54.md`, same shape.

Stop condition: `  decisions = 54`, `illegal_fields = 0`, `dead = 0`.
## Write the D55 page

Input verified: probe row `D55`; through `D54` landed

One page: `docs/wiki/decisions/D55.md`, same shape.

Stop condition: `  decisions = 55`, `illegal_fields = 0`, `dead = 0`.
## Write the D56 page

Input verified: probe row `D56` (entry added 18 Nov 2026, sha pending)

One page: `docs/wiki/decisions/D56.md`, same shape.
The entry is the sentinel-judge rule itself, so it needs no sentinel: its judge is the schema
instrument - `pwsh -NoProfile -File tools/wiki-lint/wiki-lint.ps1 -Command schema -WikiRoot
docs/wiki -Report tmp/_tmp_dp_lint.txt`. The commit anchor is written only if
`git --no-pager log -1 --format=%H -- docs/agent/decisions/D56.md` returns a sha; while the entry
is uncommitted the leaf prints `anchor = NO-COMMIT`, lands the page without `Anchors:`, and the
anchor is filled in after the root `Records and commit`.

Stop condition: `  decisions = 56` - the leaf-plan close - with `illegal_fields = 0`, `dead = 0`.