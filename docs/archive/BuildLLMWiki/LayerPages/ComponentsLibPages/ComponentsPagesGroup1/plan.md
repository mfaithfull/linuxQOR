---
Type: Plan
Title: "Components pages group 1"
Parent: "../plan.md"
Level: 3
Items:
  - Title: Write the qor_jsondata page
    Status: Atomic
    Content:
  - Title: Write the qor_ttf page
    Status: Atomic
    Content:
  - Title: Write the qor_parser page
    Status: Atomic
    Content:
  - Title: Write the qor_base64decodefilter page
    Status: Atomic
    Content:
  - Title: Write the qor_base64encodefilter page
    Status: Atomic
    Content:
  - Title: Write the qor_jpegdecodefilter page
    Status: Atomic
    Content:
  - Title: Write the qor_linereader page
    Status: Atomic
    Content:
---

# Components pages group 1

## Goal

Seven steps of **one page each** over the head of the `src/components` target list:
`qor_jsondata`, `qor_ttf`, `qor_parser`, `qor_base64decodefilter`, `qor_base64encodefilter`,
`qor_jpegdecodefilter`, `qor_linereader`.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- **CLOSED 18 Nov 2026** - all 7 leaves landed. Close-line stop pair printed
  `coverage = 50 of 90`, `uncovered_targets = 40`, `illegal_fields = 0`, `dead = 0`
  (229 of 229 anchors re-resolve). `coverage = 50 of 90` is the number
  `ComponentsPagesGroup2` opens on.
- `docs/wiki/modules/` holds 50 module pages + `purpose.md` as of this re-derivation.

## Context

Inherited from the parent (`../plan.md`, Level 2) and grandparent (`../../plan.md`, Level 1): one
page per build target in `docs/wiki/modules/`; `Judges:` is that target's own build command; the
target list was verified once by `Reconcile the target list`. **`../plan.md` §Decisions holds the
page contract - shape, read budget, judge rule, stop pair - and binds every leaf here.** Sibling
titles: `ComponentsPagesGroup2` .. `ComponentsPagesGroup7`.

## Decisions

- **The folder is not the target name, and the leaf cites the file** - `qor_jsondata` is
  `src/components/data/formats/json`. Every path in this plan was read off the
  `set(target qor_*)` scan on 16 Nov 2026 rather than derived from a folder name.

## Write the qor_jsondata page

Input verified: `FrameworkPagesGroup4` closed on `coverage = 43 of 90`.

One page: `docs/wiki/modules/qor_jsondata.md` for target `qor_jsondata`
(`src/components/data/formats/json/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `wiki-census coverage` re-reads `coverage = 44 of 90` with
`uncovered_targets = 46`, `wiki-lint schema` prints `illegal_fields = 0`,
`wiki-ref-check anchors` prints `dead = 0`.

## Write the qor_ttf page

Input verified: `coverage = 44 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_ttf.md` for target `qor_ttf`
(`src/components/data/formats/ttf/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 45 of 90`, `uncovered_targets = 45`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_parser page

Input verified: `coverage = 45 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_parser.md` for target `qor_parser`
(`src/components/data/parser/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 46 of 90`, `uncovered_targets = 44`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_base64decodefilter page

Input verified: `coverage = 46 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_base64decodefilter.md` for target `qor_base64decodefilter`
(`src/components/data/pipeline/filters/base64decodefilter/CMakeLists.txt`), under the page contract
in [`../plan.md`](../plan.md).

Stop condition: `coverage = 47 of 90`, `uncovered_targets = 43`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_base64encodefilter page

Input verified: `coverage = 47 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_base64encodefilter.md` for target `qor_base64encodefilter`
(`src/components/data/pipeline/filters/base64encodefilter/CMakeLists.txt`), under the page contract
in [`../plan.md`](../plan.md).

Stop condition: `coverage = 48 of 90`, `uncovered_targets = 42`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_jpegdecodefilter page

Input verified: `coverage = 48 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_jpegdecodefilter.md` for target `qor_jpegdecodefilter`
(`src/components/data/pipeline/filters/jpegdecodefilter/CMakeLists.txt`), under the page contract
in [`../plan.md`](../plan.md).

Stop condition: `coverage = 49 of 90`, `uncovered_targets = 41`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_linereader page

Input verified: `coverage = 49 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_linereader.md` for target `qor_linereader`
(`src/components/data/pipeline/filters/linereader/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 50 of 90`, `uncovered_targets = 40`, `illegal_fields = 0`, `dead = 0` -
this plan's close line, and the number `ComponentsPagesGroup2` opens on.
