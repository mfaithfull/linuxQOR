---
Type: Plan
Title: "Components pages group 2"
Parent: "../plan.md"
Level: 3
Items:
  - Title: Write the qor_unicodetextfilter page
    Status: Atomic
    Content:
  - Title: Write the qor_datasink page
    Status: Atomic
    Content:
  - Title: Write the qor_deserializersink page
    Status: Atomic
    Content:
  - Title: Write the qor_parsersink page
    Status: Atomic
    Content:
  - Title: Write the qor_stringsink page
    Status: Atomic
    Content:
  - Title: Write the qor_aggregatesource page
    Status: Atomic
    Content:
  - Title: Write the qor_datasource page
    Status: Atomic
    Content:
---

# Components pages group 2

## Goal

Seven steps of **one page each** over the pipeline's filter, sink and source families:
`qor_unicodetextfilter`, `qor_datasink`, `qor_deserializersink`, `qor_parsersink`,
`qor_stringsink`, `qor_aggregatesource`, `qor_datasource`.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- This plan is **CLOSED 18 Nov 2026** - all 7 one-page leaves landed (`qor_unicodetextfilter`,
  `qor_datasink`, `qor_deserializersink`, `qor_parsersink`, `qor_stringsink`,
  `qor_aggregatesource`, `qor_datasource`). Leaf 7 printed the close pair:
  `coverage = 57 of 90`, `uncovered_targets = 33`, `illegal_fields = 0`
  (`pages = 57 of 63`, `strays = 0`), `dead = 0` (`anchors = 268 of 268`), each
  `VERDICT fail=0`; `wiki-lint provenance` unchanged at `violations = 2`, both pre-existing.
  Judges green for all seven: the last three built fresh 0-warning dlls (stringsink
  105,472 B, aggregatesource 73,216 B, datasource 86,016 B); leaves 1-4 stamp-checked
  (dll newer than newest source) and judge re-ran `exit=0`.
- **Leaves 1-4 landed without a slot rotation, and the disk outranked the slot.** This slot
  read "leaf 1 not started" while four intact pages sat in `docs/wiki/modules/` (written by
  a session that died before running the stop pair). The re-derivation rule caught it:
  bodies read whole (no truncation), `Claims:` equalled body bullets, then the instruments
  closed all four in one run (`coverage = 54 of 90`, `dead = 0`). Lesson kept: an unrotated
  slot is not an unlanded leaf - re-derive from disk, verify, then rotate.
- **Grep the line numbers before drafting.** `qor_datasource` was drafted from an
  unnumbered `Get-Content` read and carried ten citations drifted 2-3 lines; caught by the
  read-back audit and fixed before the stop pair ran. The numbers in a claim are claims.
- These seven were the cheapest pages in the region (small folders, few symbols) - the
  ordering rule working; the densest five are `ComponentsPagesGroup7`.

## Context

Inherited from the parent (`../plan.md`, Level 2) and grandparent (`../../plan.md`, Level 1): one
page per build target in `docs/wiki/modules/`; `Judges:` is that target's own build command; the
target list was verified once by `Reconcile the target list`. **`../plan.md` §Decisions holds the
page contract - shape, read budget, judge rule, stop pair - and binds every leaf here.** Sibling
titles: `ComponentsPagesGroup1`, `ComponentsPagesGroup3` .. `ComponentsPagesGroup7`.

## Decisions

- **One family per leaf, not one family per plan** - the sink and source folders hold 4 and 5
  targets apiece, and a folder-shaped leaf is the 10-page mistake again in disguise. The leaf's
  unit is a target; the folder is only where its `CMakeLists.txt` lives.

## Write the qor_unicodetextfilter page

Input verified: `ComponentsPagesGroup1` closed on `coverage = 50 of 90`.

One page: `docs/wiki/modules/qor_unicodetextfilter.md` for target `qor_unicodetextfilter`
(`src/components/data/pipeline/filters/unicodetextfilter/CMakeLists.txt`), under the page contract
in [`../plan.md`](../plan.md).

Stop condition: `wiki-census coverage` re-reads `coverage = 51 of 90` with
`uncovered_targets = 39`, `wiki-lint schema` prints `illegal_fields = 0`,
`wiki-ref-check anchors` prints `dead = 0`.

## Write the qor_datasink page

Input verified: `coverage = 51 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_datasink.md` for target `qor_datasink`
(`src/components/data/pipeline/sinks/datasink/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 52 of 90`, `uncovered_targets = 38`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_deserializersink page

Input verified: `coverage = 52 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_deserializersink.md` for target `qor_deserializersink`
(`src/components/data/pipeline/sinks/deserializersink/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 53 of 90`, `uncovered_targets = 37`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_parsersink page

Input verified: `coverage = 53 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_parsersink.md` for target `qor_parsersink`
(`src/components/data/pipeline/sinks/parsersink/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 54 of 90`, `uncovered_targets = 36`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_stringsink page

Input verified: `coverage = 54 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_stringsink.md` for target `qor_stringsink`
(`src/components/data/pipeline/sinks/stringsink/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 55 of 90`, `uncovered_targets = 35`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_aggregatesource page

Input verified: `coverage = 55 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_aggregatesource.md` for target `qor_aggregatesource`
(`src/components/data/pipeline/sources/aggregatesource/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 56 of 90`, `uncovered_targets = 34`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_datasource page

Input verified: `coverage = 56 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_datasource.md` for target `qor_datasource`
(`src/components/data/pipeline/sources/datasource/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 57 of 90`, `uncovered_targets = 33`, `illegal_fields = 0`, `dead = 0` -
this plan's close line, and the number `ComponentsPagesGroup3` opens on.
