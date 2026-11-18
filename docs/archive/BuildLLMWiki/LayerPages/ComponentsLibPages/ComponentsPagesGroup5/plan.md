---
Type: Plan
Title: "Components pages group 5"
Parent: "../plan.md"
Level: 3
Items:
  - Title: Write the qor_stdoutsink page
    Status: Atomic
    Content:
  - Title: Write the qor_stdifstreamsource page
    Status: Atomic
    Content:
  - Title: Write the qor_stdinsource page
    Status: Atomic
    Content:
  - Title: Write the qor_echo page
    Status: Atomic
    Content:
  - Title: Write the qor_http page
    Status: Atomic
    Content:
  - Title: Write the qor_jip page
    Status: Atomic
    Content:
---

# Components pages group 5

## Goal

Six steps of **one page each** over the std stream ends and the protocols:
`qor_stdoutsink`, `qor_stdifstreamsource`, `qor_stdinsource`, `qor_echo`, `qor_http`, `qor_jip`.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: CLOSED` - **Group 5 CLOSED 18 Nov 2026**: all 6 pages landed (`qor_stdoutsink`,
  `qor_stdifstreamsource`, `qor_stdinsource`, `qor_echo`, `qor_http`, `qor_jip`); close-line stop
  pair printed `coverage = 77 of 90`, `uncovered_targets = 13`, `illegal_fields = 0`, `dead = 0`
  (`anchors = 388 of 388`), each `VERDICT fail=0`; `wiki-lint provenance` unchanged at
  `violations = 2`. Two judges went red and both pages document the red instead of skipping the
  page: leaf 5's `qor_http` fails in-source (C2512 at `sink.cpp:11` - `new_ref` of
  `BufferContext<uint32_t>` against a ctor that takes `const Buffer*`; only 6 of its 46 declared
  TUs are compiled, 41 source lines commented out) and leaf 6's `qor_jip` never reaches a compiler
  (`MSB1009` - no `add_subdirectory` anywhere adds the folder, and its `add_library` names two
  parser sources that do not exist on disk; the folder is echo's skeleton minus a deleted parser
  layer). Lesson: count `Claims:` after drafting, not before - leaves 5 and 6 each needed an edit
  pass, front matter off by 2-3 until recounted.
- Six leaves, not seven, because the group that follows also carries six and the densest five are
  still last - the bands are a file split, and the order they split is inherited. The band is fully
  paged now; `ComponentsPagesGroup6` opens on `coverage = 77 of 90`.

## Context

Inherited from the parent (`../plan.md`, Level 2) and grandparent (`../../plan.md`, Level 1): one
page per build target in `docs/wiki/modules/`; `Judges:` is that target's own build command; the
target list was verified once by `Reconcile the target list`. **`../plan.md` §Decisions holds the
page contract - shape, read budget, judge rule, stop pair - and binds every leaf here.** Sibling
titles: `ComponentsPagesGroup1` .. `ComponentsPagesGroup4`, `ComponentsPagesGroup6`,
`ComponentsPagesGroup7`.

## Decisions

- **`qor_http` is declared twice and the page is written once** - `wiki-census targets` prints
  `declared_twice = [qor_http]`: `src/components/protocols/http` and
  `src/components/protocols/http_old` both `set(target qor_http)`. D5 settles it - `http_old` is
  not built and is excluded from the repo map and ctags - so the leaf reads `protocols/http`,
  cites D5, and never opens `http_old`.

## Write the qor_stdoutsink page

Input verified: `ComponentsPagesGroup4` closed on `coverage = 71 of 90`.

One page: `docs/wiki/modules/qor_stdoutsink.md` for target `qor_stdoutsink`
(`src/components/io/pipeline/sinks/stdoutsink/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `wiki-census coverage` re-reads `coverage = 72 of 90` with
`uncovered_targets = 18`, `wiki-lint schema` prints `illegal_fields = 0`,
`wiki-ref-check anchors` prints `dead = 0`.

## Write the qor_stdifstreamsource page

Input verified: `coverage = 72 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_stdifstreamsource.md` for target `qor_stdifstreamsource`
(`src/components/io/pipeline/sources/stdifstreamsource/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 73 of 90`, `uncovered_targets = 17`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_stdinsource page

Input verified: `coverage = 73 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_stdinsource.md` for target `qor_stdinsource`
(`src/components/io/pipeline/sources/stdinsource/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 74 of 90`, `uncovered_targets = 16`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_echo page

Input verified: `coverage = 74 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_echo.md` for target `qor_echo`
(`src/components/protocols/echo/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 75 of 90`, `uncovered_targets = 15`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_http page

Input verified: `coverage = 75 of 90` printed by the leaf above, and D5 recorded in the Decisions
above (`http_old` is not built).

One page: `docs/wiki/modules/qor_http.md` for target `qor_http`
(`src/components/protocols/http/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md). The duplicate declaration and its D-entry are a claim for this page -
it is the one place a reader will look for why two folders name one target.

Stop condition: `coverage = 76 of 90`, `uncovered_targets = 14`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_jip page

Input verified: `coverage = 76 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_jip.md` for target `qor_jip`
(`src/components/protocols/jip/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 77 of 90`, `uncovered_targets = 13`, `illegal_fields = 0`, `dead = 0` -
this plan's close line, and the number `ComponentsPagesGroup6` opens on.
