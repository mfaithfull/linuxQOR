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

- `Cursor: complete - leaves 1-6 landed 18 Nov 2026, all six judges green:
  `qor_uri` (150 lines, `Claims: 26`, closed at `coverage = 78 of 90`, `anchors = 394 of 394`),
  `qor_logaggregator` (195 lines, `Claims: 32`, `coverage = 79 of 90`, `anchors = 400 of 400`),
  `qor_loghandler` (159 lines, `Claims: 24`, `coverage = 80 of 90`, `anchors = 406 of 406`),
  `qor_canvas` (168 lines, `Claims: 27`, `coverage = 81 of 90`, `uncovered_targets = 9`,
  `anchors = 412 of 412`, `illegal_fields = 0`, `dead = 0`, `over_budget = 0`),
  `qor_console` (172 lines, `Claims: 34`, `coverage = 82 of 90`, `uncovered_targets = 8`,
  `anchors = 418 of 418`, `illegal_fields = 0`, `dead = 0`, `over_budget = 0`), and
  `qor_egl` (140 lines, `Claims: 30`, `coverage = 83 of 90`, `uncovered_targets = 7`,
  `anchors = 424 of 424`, `illegal_fields = 0`, `dead = 0`, `over_budget = 0`);
  `wiki-lint provenance` unchanged at `violations = 2` (`modules/qor_task.md:45 [url]`,
  `modules/qor_signals.md:52 [secret]` - both pre-existing and false-positive-shaped: a quoted URL,
  and an enum-label word followed by a colon).
  This plan closes at `coverage = 83 of 90`, `uncovered_targets = 7`;
  `ComponentsPagesGroup7` opens on that line.
- `qor_logaggregator` and `qor_loghandler` live under `src/components/qor/`, not under `src/qor/`.
  The path in each leaf is the one that resolves; `src/qor/` holds the core region
  (`SeedPages`), which is closed.
- Lesson carried into leaf 4: count `Claims:` **after** drafting (leaf 3's front matter was off by
  one until recounted) and read the page body back before its stop pair runs. `FEATURES.md` rows
  for the three UI targets here are `:108` `qor_canvas`, `:109` `qor_console`, `:110` `qor_egl`,
  each "Experimental UI"; the `winqor_*` (`:149`, `:151`) and `linqor_*` (`:166`, `:167`) variants
  are separate targets and are not this leaf's page.

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
