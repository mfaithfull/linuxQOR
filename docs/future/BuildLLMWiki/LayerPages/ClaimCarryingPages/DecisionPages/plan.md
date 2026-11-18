---
Type: Plan
Title: "Decision pages"
Parent: "../plan.md"
Level: 3
Items:
  - Title: Decision pages slice 1
    Status: Atomic
    Content:
  - Title: Decision pages slice 2
    Status: Atomic
    Content:
  - Title: Decision pages slice 3
    Status: Atomic
    Content:
  - Title: Decision pages slice 4
    Status: Atomic
    Content:
  - Title: Decision pages slice 5
    Status: Atomic
    Content:
  - Title: Decision pages slice 6
    Status: Atomic
    Content:
---

# Decision pages

## Goal

One wiki page per recorded decision, each carrying the commit that made it - the only wiki pages
whose anchor is a commit rather than a path.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: not started` - 6 `Atomic`, in D-id order. Measured source: `docs/agent/decisions/`
  holds **55** files, `D1.md`..`D55.md`.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: write the pages that carry a falsification handle.
- **Parent locked decisions**: `docs/learning/` and `docs/wiki/` never sync; a page's `Judges:` are
  drawn from `docs/agent/verification.md`'s four rule sections, not its Results log.
- **Sibling plan titles**: `Gotcha pages`, `Invariant pages`, `Concept pages` (the three `Atomic`
  items in the parent).
- **Measured**: `docs/agent/DECISIONS.md` is the index; `docs/agent/decisions/` holds 55 files.

## Decisions

- **Slice by D-id, not by topic** - the ids are the artifact's only stable naming, and a topic cut
  would need a second instrument to prove it covered all 55.
- **One page per D-entry is the literal reading of §14 item 10** - flagged, not assumed. If the
  wiki should carry decision **clusters** instead, this plan is re-cut smaller; the cut is fixed by
  the same instrument either way, so the choice changes the leaf count and not the guarantee.
- **The anchor is a commit, so the pin is a ref, not a path pin** - §6.2. A decision page is stale
  when the code it decided on has moved, and the thing that detects that is `wiki-reconcile`'s
  `history` subcommand, not its `anchors` subcommand.

## Decision pages slice 1

Ten pages, one per D-entry, enumerated by id: `D1`, `D2`, `D3`, `D4`, `D5`, `D6`, `D7`, `D8`,
`D9`, `D10`. Each page: `Type: wiki-decision`, `Decision: D<n>`, `Anchors:` carrying
`commit:` (the hash recorded in the D-entry) and `path:` for the file the decision governs,
`Judges:` only where the decision names a buildable artifact, `SupersededBy:` where the D-entry
says so; body the decision's own words plus the code anchor.

Input verified: the 55 files measured in the Resume slot.

Stop condition: `wiki-census pages` re-reads `decision = 10` and `wiki-ref-check anchors` prints
`dead = 0`; a D-entry whose commit hash is absent prints rather than invents.

## Decision pages slice 2

Ten pages, one per D-entry, enumerated by id: `D11`, `D12`, `D13`, `D14`, `D15`, `D16`, `D17`,
`D18`, `D19`, `D20`. Same shape as slice 1.

Input verified: the 55 files measured in the Resume slot, and the 10 pages of slice 1.

Stop condition: `wiki-census pages` re-reads `decision = 20 of 20`.

## Decision pages slice 3

Ten pages, one per D-entry, enumerated by id: `D21`, `D22`, `D23`, `D24`, `D25`, `D26`, `D27`,
`D28`, `D29`, `D30`. Same shape as slice 1.

Input verified: the 55 files, and the 20 pages written so far.

Stop condition: `decision = 30 of 30`.

## Decision pages slice 4

Ten pages, one per D-entry, enumerated by id: `D31`, `D32`, `D33`, `D34`, `D35`, `D36`, `D37`,
`D38`, `D39`, `D40`. Same shape as slice 1.

Input verified: the 55 files, and the 30 pages written so far.

Stop condition: `decision = 40 of 40`.

## Decision pages slice 5

Ten pages, one per D-entry, enumerated by id: `D41`, `D42`, `D43`, `D44`, `D45`, `D46`, `D47`,
`D48`, `D49`, `D50`. Same shape as slice 1. Note: `D50` is the decision that reversed the settled
"leave as-is" for the handler half only - its page must carry `SupersededBy:` nowhere and
`Supersedes:` the earlier entry, which is the one place a supersession chain is legible.

Input verified: the 55 files, and the 40 pages written so far.

Stop condition: `decision = 50 of 50`.

## Decision pages slice 6

Five pages, one per D-entry, enumerated by id: `D51`, `D52`, `D53`, `D54`, `D55`. Same shape as
slice 1. These are the newest decisions and the ones whose code is most likely already moved, so
their `LastVerified:` stamps are the ones most likely to be stale on the first read - which is the
point of seeding them.

Input verified: the 55 files, and the 50 pages written so far.

Stop condition: `decision = 55 of 55` - the full census of `docs/agent/decisions/`, printed
against the disk listing rather than against the prose count.
