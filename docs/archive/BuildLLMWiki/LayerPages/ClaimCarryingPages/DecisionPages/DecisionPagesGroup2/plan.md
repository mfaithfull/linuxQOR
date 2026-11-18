---
Type: Plan
Title: "Decision pages group 2"
Parent: "../plan.md"
Level: 4
Items:
  - Title: Decision pages D15 to 21
    Status: Refined
    Content: ./DecisionPagesGroup2A/plan.md
  - Title: Decision pages D22 to 28
    Status: Refined
    Content: ./DecisionPagesGroup2B/plan.md
---

# Decision pages group 2

## Goal

Fourteen decision pages, `D15`..`D28`, one per leaf, in D-id order.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: GROUP2 CLOSED 18 Nov 2026` - `Group2A` closed (`D15`..`D21`, stop `  decisions = 21`) and
  `Group2B` closed (`D22`..`D28`, stop `  decisions = 28`); group-wide stop met: census
  `  decisions = 28`, schema `illegal_fields = 0`, ref-check `anchors = 489 of 489`, `dead = 0`.

## Context

Inherited from `../plan.md` (Level 3) - re-read rather than trusted. The page contract is the shape
of a shipped wiki page (worked example: `docs/wiki/invariants/lint.md`): `Type: WikiPage`,
`PageType: decision`, required `Status`, `LastVerified`, non-empty `Judges`, and the commit anchor
`{ kind: in-tree, path: docs/agent/decisions/D<n>.md, anchor: commit:<sha>, verified: <date> }`.
Siblings: the other `DecisionPagesGroup*` plans.

## Decisions

- **These plans are plumbing for the 7-item breadth cap** - the A/B split is D-id ranges only;
  the guarantee (one page per leaf, stop line advancing one id) is identical across groups.
- **No page carries `Supersedes:` or `Decision:`** - measured 18 Nov 2026: neither is a schema
  field; `SupersededBy:` is, and the probe slot says which page carries it.

## Decision pages D15 to 21

See [`./DecisionPagesGroup2A/plan.md`](./DecisionPagesGroup2A/plan.md) - seven one-page
leaves.

## Decision pages D22 to 28

See [`./DecisionPagesGroup2B/plan.md`](./DecisionPagesGroup2B/plan.md) - seven one-page
leaves.