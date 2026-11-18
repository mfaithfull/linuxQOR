---
Type: Plan
Title: "Components pages group 6"
Parent: "../plan.md"
Level: 3
Items:
  - Title: Write the qor_uri page
    Status: Atomic
    Content:
  - Title: Write the qor_logaggregator page
    Status: Atomic
    Content:
  - Title: Write the qor_loghandler page
    Status: Atomic
    Content:
  - Title: Write the qor_canvas page
    Status: Atomic
    Content:
  - Title: Write the qor_console page
    Status: Atomic
    Content:
  - Title: Write the qor_egl page
    Status: Atomic
    Content:
---

# Components pages group 6

## Goal

Six steps of **one page each** over the last of the protocols, the two `qor/` libs and the first of
the UI components: `qor_uri`, `qor_logaggregator`, `qor_loghandler`, `qor_canvas`, `qor_console`,
`qor_egl`.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: leaf 1 - Write the qor_uri page` - not started. Opens at `coverage = 77 of 90`
  (the line `ComponentsPagesGroup5` closes on) and closes at `coverage = 83 of 90`,
  `uncovered_targets = 7`.
- `qor_logaggregator` and `qor_loghandler` live under `src/components/qor/`, not under `src/qor/`.
  The path in each leaf is the one that resolves; `src/qor/` holds the core region
  (`SeedPages`), which is closed.

## Context

Inherited from the parent (`../plan.md`, Level 2) and grandparent (`../../plan.md`, Level 1): one
page per build target in `docs/wiki/modules/`; `Judges:` is that target's own build command; the
target list was verified once by `Reconcile the target list`. **`../plan.md` §Decisions holds the
page contract - shape, read budget, judge rule, stop pair - and binds every leaf here.** Sibling
titles: `ComponentsPagesGroup1` .. `ComponentsPagesGroup5`, `ComponentsPagesGroup7`.

## Decisions

- **The UI components are split across two plans (6 here without the densest, 5 there) so the five
  densest libs keep a plan to themselves** - the parent's densest-last decision is only worth
  anything if the dense set can be stopped in front of without stopping a family.

## Write the qor_uri page

Input verified: `ComponentsPagesGroup5` closed on `coverage = 77 of 90`.

One page: `docs/wiki/modules/qor_uri.md` for target `qor_uri`
(`src/components/protocols/uri/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `wiki-census coverage` re-reads `coverage = 78 of 90` with
`uncovered_targets = 12`, `wiki-lint schema` prints `illegal_fields = 0`,
`wiki-ref-check anchors` prints `dead = 0`.

## Write the qor_logaggregator page

Input verified: `coverage = 78 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_logaggregator.md` for target `qor_logaggregator`
(`src/components/qor/logaggregator/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 79 of 90`, `uncovered_targets = 11`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_loghandler page

Input verified: `coverage = 79 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_loghandler.md` for target `qor_loghandler`
(`src/components/qor/loghandler/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md). Its aggregator is the page above - link it.

Stop condition: `coverage = 80 of 90`, `uncovered_targets = 10`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_canvas page

Input verified: `coverage = 80 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_canvas.md` for target `qor_canvas`
(`src/components/ui/canvas/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 81 of 90`, `uncovered_targets = 9`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_console page

Input verified: `coverage = 81 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_console.md` for target `qor_console`
(`src/components/ui/console/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 82 of 90`, `uncovered_targets = 8`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_egl page

Input verified: `coverage = 82 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_egl.md` for target `qor_egl`
(`src/components/ui/egl/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 83 of 90`, `uncovered_targets = 7`, `illegal_fields = 0`, `dead = 0` -
this plan's close line, and the number `ComponentsPagesGroup7` (the dense five) opens on.
