---
Type: Plan
Title: "Decision pages group 3"
Parent: "../plan.md"
Level: 4
Items:
  - Title: Decision pages D29 to 35
    Status: Refined
    Content: ./DecisionPagesGroup3A/plan.md
  - Title: Decision pages D36 to 42
    Status: Refined
    Content: ./DecisionPagesGroup3B/plan.md
---

# Decision pages group 3

## Goal

Fourteen decision pages, `D29`..`D42`, one per leaf, in D-id order.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: CLOSED 18 Nov 2026` - `Group3A` (`D29`..`D35`, stop `  decisions = 35`) and `Group3B`
  (`D36`..`D42`, stop `  decisions = 42`, `illegal_fields = 0`, `dead = 0`, `anchors = 503 of 503`)
  both closed; each `VERDICT fail=0`.

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

## Decision pages D29 to 35

See [`./DecisionPagesGroup3A/plan.md`](./DecisionPagesGroup3A/plan.md) - seven one-page
leaves.

## Decision pages D36 to 42

See [`./DecisionPagesGroup3B/plan.md`](./DecisionPagesGroup3B/plan.md) - seven one-page
leaves.