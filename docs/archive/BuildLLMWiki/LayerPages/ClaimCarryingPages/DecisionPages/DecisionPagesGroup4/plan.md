---
Type: Plan
Title: "Decision pages group 4"
Parent: "../plan.md"
Level: 4
Items:
  - Title: Decision pages D43 to 49
    Status: Refined
    Content: ./DecisionPagesGroup4A/plan.md
  - Title: Decision pages D50 to 56
    Status: Refined
    Content: ./DecisionPagesGroup4B/plan.md
---

# Decision pages group 4

## Goal

Fourteen decision pages, `D43`..`D56`, one per leaf, in D-id order.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `CLOSED 18 Nov 2026 - both leaf-plans landed: Group4A (D43..D49) and Group4B (D50..D56),
  fourteen pages; Group4B closed on `  decisions = 56` with schema `illegal_fields = 0` and
  anchors `516 of 516, dead = 0` (D56 page carries no anchor - NO-COMMIT).`

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

## Decision pages D43 to 49

See [`./DecisionPagesGroup4A/plan.md`](./DecisionPagesGroup4A/plan.md) - seven one-page
leaves.

## Decision pages D50 to 56

See [`./DecisionPagesGroup4B/plan.md`](./DecisionPagesGroup4B/plan.md) - seven one-page
leaves.