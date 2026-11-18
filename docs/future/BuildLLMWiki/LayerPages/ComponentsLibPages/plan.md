---
Type: Plan
Title: "Components lib pages"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Components pages group 1
    Status: Refined
    Content: ./ComponentsPagesGroup1/plan.md
  - Title: Components pages group 2
    Status: Refined
    Content: ./ComponentsPagesGroup2/plan.md
  - Title: Components pages group 3
    Status: Refined
    Content: ./ComponentsPagesGroup3/plan.md
  - Title: Components pages group 4
    Status: Refined
    Content: ./ComponentsPagesGroup4/plan.md
  - Title: Components pages group 5
    Status: Refined
    Content: ./ComponentsPagesGroup5/plan.md
  - Title: Components pages group 6
    Status: Refined
    Content: ./ComponentsPagesGroup6/plan.md
  - Title: Components pages group 7
    Status: Refined
    Content: ./ComponentsPagesGroup7/plan.md
---

# Components lib pages

## Goal

One page per `src/components` QOR lib target - 45 targets, the largest region, one target per
`Atomic` leaf and ordered densest-last.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: group 1, leaf 1 - Write the qor_jsondata page`
  ([`./ComponentsPagesGroup1/plan.md`](./ComponentsPagesGroup1/plan.md)) - not started. Opens at
  `coverage = 43 of 90` (the number `FrameworkLibPages` closes on) and closes at
  `coverage = 88 of 90`, `uncovered_targets = 2` with the two named (`qor_mime`, `qor_yaml`).
- **Re-cut 16 Nov 2026: the 10-page leaf was never one step**, and this plan is the same mistake
  waiting at 4.5x the size. The five `Atomic` slices of 10 + 10 + 10 + 10 + 5 pages are replaced by
  **45 leaves of one page each** in 7 `Refined` groups of 7 / 7 / 7 / 7 / 6 / 6 / 5 - seven because
  the breadth cap in `docs/future/PLAN-FORMAT.md` is 7 items per plan. The proof it was needed is
  next door: `FrameworkLibPages` slice 1 spent 35 `tmp/_tmp_fw*` probes to land 2 of 10 pages. Every
  target id and the whole densest-last order survive the re-cut; only the boundaries move. The cut
  is fixed by `tmp/_tmp_llmwiki_page_cut.ps1` -> `.txt` (recreate by slug):
  `components claimed = 45 of 45  owned_once = 45  landed = 0  VERDICT fail=0`.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: populate the wiki bounded by 90 distinct build targets.
- **Parent locked decisions**: one page per target in `docs/wiki/modules/`; `Judges:` is the target's
  build command; the target list is verified before pages are written.
- **Sibling plan titles**: `SeedPages`, `FrameworkLibPages` (26 one-page leaves; 2 landed,
  `coverage = 19 of 90`), `ClaimCarryingPages`.
- **Measured**: 45 `src/components` targets - the largest region, and the one where a leaf's cost
  varies most (a `qor_renderer` page carries a 3,682-symbol ctags index; a `qor_echo` page carries
  almost nothing).

## Decisions

- **A page is not a unit, so a leaf is one page** - the parent's `FrameworkLibPages` states the
  measurement; here it is the same rule at 45 targets. `docs/future/PLAN-FORMAT.md` defines a unit as
  "a fetch, an edit, a lookup, a written row", so the old ten-page leaf was tens of units and no
  ordering made it `Atomic`. Target ids and order are unchanged by the re-cut; the group boundaries
  are the breadth cap of 7.
- **Densest-last, not alphabetical** - the sibling `User32ReturnCheckPass` tree's rule, kept. What
  the re-cut changes is what the rule protects: with one page per leaf, "densest last" means the
  expensive pages each get a session of their own instead of sharing a leaf with nine cheap ones,
  which is why `qor_renderer` is now alone in front of a stop line.
- **Every leaf obeys the page contract in `../FrameworkLibPages/plan.md` §Decisions** - the shape (the
  13 fields of `docs/wiki/schema.md`, and `VerifiedAgainst:` is not among them), the read budget (own
  `CMakeLists.txt`, own public headers, own `repo_map.md` row, own `purposes.md` line, tree-wide
  counts; no whole `.cpp`), one judge build (`cmake --build cmakebuild --target <target> --parallel 8`),
  and the stop pair (`coverage = N of 90`, `illegal_fields = 0`, `dead = 0`). It is stated once, in
  the sibling that already ran, because two copies of a rule run the op two ways (F10).
- **The ctags index is an anchor, not a summary** - §5. A page links `docs/agent/ctags/<lib>.tags`
  by path and pin; it never restates the symbol list, because a restated list is prose that goes
  stale silently, which is F1. `docs/agent/ctags/` holds 0 files on 16 Nov, so the leaf that needs an
  index emits it for its own lib (D7) and cites its count.

## Components pages group 1

See [`./ComponentsPagesGroup1/plan.md`](./ComponentsPagesGroup1/plan.md) - 7 `Atomic` leaves of **one
page each**: `qor_jsondata`, `qor_ttf`, `qor_parser`, `qor_base64decodefilter`,
`qor_base64encodefilter`, `qor_jpegdecodefilter`, `qor_linereader`.

Input verified: the target list, and the 19 pages on disk at the re-cut (17 `SeedPages` + 2 landed
framework).

Stop condition (the group's close line, printed by leaf 7): `coverage = 50 of 90`,
`uncovered_targets = 40`.

## Components pages group 2

See [`./ComponentsPagesGroup2/plan.md`](./ComponentsPagesGroup2/plan.md) - 7 `Atomic` leaves of one
page each: `qor_unicodetextfilter`, `qor_datasink`, `qor_deserializersink`, `qor_parsersink`,
`qor_stringsink`, `qor_aggregatesource`, `qor_datasource`.

Input verified: group 1's close line, `coverage = 50 of 90`.

Stop condition (printed by leaf 7): `coverage = 57 of 90`, `uncovered_targets = 33`.

## Components pages group 3

See [`./ComponentsPagesGroup3/plan.md`](./ComponentsPagesGroup3/plan.md) - 7 `Atomic` leaves of one
page each: `qor_generatorsource`, `qor_randomsource`, `qor_stringsource`, `qor_serialize`,
`qor_generator`, `qor_netclient`, `qor_httpclient`.

Input verified: group 2's close line, `coverage = 57 of 90`.

Stop condition (printed by leaf 7): `coverage = 64 of 90`, `uncovered_targets = 26`.

## Components pages group 4

See [`./ComponentsPagesGroup4/plan.md`](./ComponentsPagesGroup4/plan.md) - 7 `Atomic` leaves of one
page each: `qor_socketclient`, `qor_netserver`, `qor_fileconnector`, `qor_pipeconnector`,
`qor_socketconnector`, `qor_stdfstreamconnector`, `qor_stdofstreamsink`.

Input verified: group 3's close line, `coverage = 64 of 90`.

Stop condition (printed by leaf 7): `coverage = 71 of 90`, `uncovered_targets = 19`.

## Components pages group 5

See [`./ComponentsPagesGroup5/plan.md`](./ComponentsPagesGroup5/plan.md) - 6 `Atomic` leaves of one
page each: `qor_stdoutsink`, `qor_stdifstreamsource`, `qor_stdinsource`, `qor_echo`, `qor_http`,
`qor_jip`. `qor_http` is the target declared twice (D5 excludes `http_old`), and that page carries
the reason.

Input verified: group 4's close line, `coverage = 71 of 90`.

Stop condition (printed by leaf 6): `coverage = 77 of 90`, `uncovered_targets = 13`.

## Components pages group 6

See [`./ComponentsPagesGroup6/plan.md`](./ComponentsPagesGroup6/plan.md) - 6 `Atomic` leaves of one
page each: `qor_uri`, `qor_logaggregator`, `qor_loghandler`, `qor_canvas`, `qor_console`,
`qor_egl`.

Input verified: group 5's close line, `coverage = 77 of 90`.

Stop condition (printed by leaf 6): `coverage = 83 of 90`, `uncovered_targets = 7`.

## Components pages group 7

See [`./ComponentsPagesGroup7/plan.md`](./ComponentsPagesGroup7/plan.md) - 5 `Atomic` leaves of one
page each: `qor_layout`, `qor_opengles`, `qor_renderer`, `qor_tui`, `qor_widgets`. These are the
densest libs in the tree (largest ctags indices), so they are the **last** group - and each is now a
leaf of one page, which is the only way the dense set is affordable at all.

Input verified: group 6's close line, `coverage = 83 of 90`.

Stop condition (printed by leaf 5): `coverage = 88 of 90` with `uncovered_targets = 2` and the two
named (`qor_mime`, `qor_yaml`) - this plan's close line, and the number `ClaimCarryingPages` opens
on. The unindexed pair is the only wiki state that may legitimately be behind at the end of this
plan, and it is the queue's, not this plan's, to resolve.
