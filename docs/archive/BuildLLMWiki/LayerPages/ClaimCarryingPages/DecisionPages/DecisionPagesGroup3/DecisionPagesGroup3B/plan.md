---
Type: Plan
Title: "Decision pages D36 to 42"
Parent: "../plan.md"
Level: 5
Items:
  - Title: Write the D36 page
    Status: Atomic
    Content:
  - Title: Write the D37 page
    Status: Atomic
    Content:
  - Title: Write the D38 page
    Status: Atomic
    Content:
  - Title: Write the D39 page
    Status: Atomic
    Content:
  - Title: Write the D40 page
    Status: Atomic
    Content:
  - Title: Write the D41 page
    Status: Atomic
    Content:
  - Title: Write the D42 page
    Status: Atomic
    Content:
---

# Decision pages D36 to 42

## Goal

Seven leaves, each writing exactly one page: `docs/wiki/decisions/D<n>.md` for
`n = 36..42`, each anchored to the commit that made its D-entry.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: CLOSED 18 Nov 2026` - all 7 leaves landed (`D36`..`D42`), stop pair at the close printed
  `  decisions = 42`, `illegal_fields = 0`, `dead = 0` (`anchors = 503 of 503`), each
  `VERDICT fail=0` (reports `tmp/_tmp_dp_{pages,schema,anchors}.txt`). The first two leaves
  (`D36`, `D37`) landed 20:33/20:34 18 Nov 2026 by the pass that pre-empted this slot's re-derivation;
  `D38`..`D42` landed this session. Judges as probed: `D36` sentinel (policy, D56); `D37`/`D38` the
  folder-gate `cl` command the entries record (the D38 page states the exit-0 fixed run and the
  zero-diff all-libraries leg); `D39` `dumpbin /exports C:\Windows\System32\user32.dll` (the entry's
  own recorded run, one `ConsoleControl` row); `D40` the folder-gate `cl` line (post-rename exit 0,
  C2039/C2065/C2006 zero-diff signatures); `D41`/`D42` sentinels (no-edit closes, tmp scans are
  provenance).

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

## Write the D36 page

Input verified: probe row `D36` (sha green, judge named or reasoned)

One page: `docs/wiki/decisions/D36.md`, same shape.

Stop condition: `  decisions = 36`, `illegal_fields = 0`, `dead = 0`.
## Write the D37 page

Input verified: probe row `D37`; through `D36` landed

One page: `docs/wiki/decisions/D37.md`, same shape.

Stop condition: `  decisions = 37`, `illegal_fields = 0`, `dead = 0`.
## Write the D38 page

Input verified: probe row `D38`; through `D37` landed

One page: `docs/wiki/decisions/D38.md`, same shape.

Stop condition: `  decisions = 38`, `illegal_fields = 0`, `dead = 0`.
## Write the D39 page

Input verified: probe row `D39`; through `D38` landed

One page: `docs/wiki/decisions/D39.md`, same shape.

Stop condition: `  decisions = 39`, `illegal_fields = 0`, `dead = 0`.
## Write the D40 page

Input verified: probe row `D40`; through `D39` landed

One page: `docs/wiki/decisions/D40.md`, same shape.

Stop condition: `  decisions = 40`, `illegal_fields = 0`, `dead = 0`.
## Write the D41 page

Input verified: probe row `D41`; through `D40` landed

One page: `docs/wiki/decisions/D41.md`, same shape.

Stop condition: `  decisions = 41`, `illegal_fields = 0`, `dead = 0`.
## Write the D42 page

Input verified: probe row `D42`; through `D41` landed

One page: `docs/wiki/decisions/D42.md`, same shape.

Stop condition: `  decisions = 42` - the leaf-plan close - with `illegal_fields = 0`, `dead = 0`.