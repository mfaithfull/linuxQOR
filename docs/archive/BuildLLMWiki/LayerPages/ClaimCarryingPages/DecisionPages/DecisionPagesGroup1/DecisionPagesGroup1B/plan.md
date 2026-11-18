---
Type: Plan
Title: "Decision pages D8 to 14"
Parent: "../plan.md"
Level: 5
Items:
  - Title: Write the D8 page
    Status: Atomic
    Content:
  - Title: Write the D9 page
    Status: Atomic
    Content:
  - Title: Write the D10 page
    Status: Atomic
    Content:
  - Title: Write the D11 page
    Status: Atomic
    Content:
  - Title: Write the D12 page
    Status: Atomic
    Content:
  - Title: Write the D13 page
    Status: Atomic
    Content:
  - Title: Write the D14 page
    Status: Atomic
    Content:
---

# Decision pages D8 to 14

## Goal

Seven leaves, each writing exactly one page: `docs/wiki/decisions/D<n>.md` for
`n = 8..14`, each anchored to the commit that made its D-entry.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: CLOSED 18 Nov 2026` - all 7 pages landed (`D8`..`D14`), running stop counts printed
  `  decisions = 8..14` (each `VERDICT fail=0`). Leaf-plan close: `wiki-census pages` `pages = 109`,
  `  decisions = 14`; `wiki-lint schema` `illegal_fields = 0`; `wiki-ref-check anchors` `dead = 0`,
  each `VERDICT fail=0`. D8/D9/D10/D11 carry the D56 sentinel (no re-runnable tree command: D9's
  perf logs and D10/D11's `cl`/`dumpbin` probes were throwaway `tmp/`); D12 is judged by its target
  build, D13 by `cl /EP`, D14 by the folder `/W4` gate line it declares unaffected (now 54/54).

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

## Write the D8 page

Input verified: probe row `D8` (sha green, judge named or reasoned)

One page: `docs/wiki/decisions/D8.md`, same shape.

Stop condition: `  decisions = 8`, `illegal_fields = 0`, `dead = 0`.
## Write the D9 page

Input verified: probe row `D9`; through `D8` landed

One page: `docs/wiki/decisions/D9.md`, same shape.

Stop condition: `  decisions = 9`, `illegal_fields = 0`, `dead = 0`.
## Write the D10 page

Input verified: probe row `D10`; through `D9` landed

One page: `docs/wiki/decisions/D10.md`, same shape.

Stop condition: `  decisions = 10`, `illegal_fields = 0`, `dead = 0`.
## Write the D11 page

Input verified: probe row `D11`; through `D10` landed

One page: `docs/wiki/decisions/D11.md`, same shape.

Stop condition: `  decisions = 11`, `illegal_fields = 0`, `dead = 0`.
## Write the D12 page

Input verified: probe row `D12`; through `D11` landed

One page: `docs/wiki/decisions/D12.md`, same shape.

Stop condition: `  decisions = 12`, `illegal_fields = 0`, `dead = 0`.
## Write the D13 page

Input verified: probe row `D13`; through `D12` landed

One page: `docs/wiki/decisions/D13.md`, same shape.

Stop condition: `  decisions = 13`, `illegal_fields = 0`, `dead = 0`.
## Write the D14 page

Input verified: probe row `D14`; through `D13` landed

One page: `docs/wiki/decisions/D14.md`, same shape.

Stop condition: `  decisions = 14` - the leaf-plan close - with `illegal_fields = 0`, `dead = 0`.