---
Type: Plan
Title: "Decision pages D43 to 49"
Parent: "../plan.md"
Level: 5
Items:
  - Title: Write the D43 page
    Status: Atomic
    Content:
  - Title: Write the D44 page
    Status: Atomic
    Content:
  - Title: Write the D45 page
    Status: Atomic
    Content:
  - Title: Write the D46 page
    Status: Atomic
    Content:
  - Title: Write the D47 page
    Status: Atomic
    Content:
  - Title: Write the D48 page
    Status: Atomic
    Content:
  - Title: Write the D49 page
    Status: Atomic
    Content:
---

# Decision pages D43 to 49

## Goal

Seven leaves, each writing exactly one page: `docs/wiki/decisions/D<n>.md` for
`n = 43..49`, each anchored to the commit that made its D-entry.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: CLOSED 18 Nov 2026` - all 7 leaves landed (`D43`..`D49`), stop pair at the close printed
  `  decisions = 49`, `illegal_fields = 0`, `dead = 0` (`anchors = 510 of 510`), each
  `VERDICT fail=0` (reports `tmp/_tmp_dp_{pages,schema,anchors}.txt`). Judges as probed: `D43`,
  `D44`, `D45`, `D47` sentinels (note-only / single-point-with-no-gate closes, D56); `D46` sentinel
  (seven tmp/_tmp_u27_p*.cpp probes are recreate-by-slug fixtures); `D48`/`D49` carry
  `dumpbin /exports C:\Windows\System32\user32.dll` - both closes' own recorded instrument, 1014
  address + 4 forwarded rows. One correction caught before its stop pair: the D46 page's first write
  carried D43's sha - wrong-but-live commit anchors pass `dead = 0`, so a moved-path check is not
  enough: each leaf must copy its own probe row's sha.

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

## Write the D43 page

Input verified: probe row `D43` (sha green, judge named or reasoned)

One page: `docs/wiki/decisions/D43.md`, same shape.

Stop condition: `  decisions = 43`, `illegal_fields = 0`, `dead = 0`.
## Write the D44 page

Input verified: probe row `D44`; through `D43` landed

One page: `docs/wiki/decisions/D44.md`, same shape.

Stop condition: `  decisions = 44`, `illegal_fields = 0`, `dead = 0`.
## Write the D45 page

Input verified: probe row `D45`; through `D44` landed

One page: `docs/wiki/decisions/D45.md`, same shape.

Stop condition: `  decisions = 45`, `illegal_fields = 0`, `dead = 0`.
## Write the D46 page

Input verified: probe row `D46`; through `D45` landed

One page: `docs/wiki/decisions/D46.md`, same shape.

Stop condition: `  decisions = 46`, `illegal_fields = 0`, `dead = 0`.
## Write the D47 page

Input verified: probe row `D47`; through `D46` landed

One page: `docs/wiki/decisions/D47.md`, same shape.

Stop condition: `  decisions = 47`, `illegal_fields = 0`, `dead = 0`.
## Write the D48 page

Input verified: probe row `D48`; through `D47` landed

One page: `docs/wiki/decisions/D48.md`, same shape.

Stop condition: `  decisions = 48`, `illegal_fields = 0`, `dead = 0`.
## Write the D49 page

Input verified: probe row `D49`; through `D48` landed

One page: `docs/wiki/decisions/D49.md`, same shape.

Stop condition: `  decisions = 49` - the leaf-plan close - with `illegal_fields = 0`, `dead = 0`.