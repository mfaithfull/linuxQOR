---
Type: Plan
Title: "Decision pages group 1"
Parent: "../plan.md"
Level: 4
Items:
  - Title: Decision pages D1 to D7
    Status: Refined
    Content: ./DecisionPagesGroup1A/plan.md
  - Title: Decision pages D8 to D14
    Status: Refined
    Content: ./DecisionPagesGroup1B/plan.md
---

# Decision pages group 1

## Goal

Fourteen decision pages, `D1`..`D14`, one per leaf, in D-id order - the oldest decisions, whose code
is the most likely to have already moved, which is exactly what a commit anchor is for.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: CLOSED 18 Nov 2026` - both leaf-plans landed: `Group1A` = `D1`..`D7`, `Group1B` =
  `D8`..`D14`; group close pair printed `  decisions = 14`, `illegal_fields = 0`, `dead = 0`, each
  `VERDICT fail=0` (see the leaf-plan slots).

## Context

Inherited from `../plan.md` (Level 3) - re-read rather than trusted. The page contract is the shape
of a shipped wiki page (worked example: `docs/wiki/invariants/lint.md`): `Type: WikiPage`,
`PageType: decision`, required `Status`, `LastVerified`, non-empty `Judges`, and the commit anchor
`{ kind: in-tree, path: docs/agent/decisions/D<n>.md, anchor: commit:<sha>, verified: <date> }`.
Siblings: `DecisionPagesGroup2..4`.

## Decisions

- **These plans are plumbing for the 7-item breadth cap** - `Group1A`/`1B` split D-id ranges only;
  the guarantee (one page per leaf, stop line advancing one id) is identical across groups.
- **No page carries `Supersedes:` or `Decision:`** - measured 18 Nov 2026: neither is a schema field;
  `SupersededBy:` is, and the probe slot says which page carries it.

## Decision pages D1 to D7

See [`./DecisionPagesGroup1A/plan.md`](./DecisionPagesGroup1A/plan.md) - seven one-page leaves.

## Decision pages D8 to D14

See [`./DecisionPagesGroup1B/plan.md`](./DecisionPagesGroup1B/plan.md) - seven one-page leaves.
