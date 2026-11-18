---
Type: Plan
Title: "Components pages group 3"
Parent: "../plan.md"
Level: 3
Items:
  - Title: Write the qor_generatorsource page
    Status: Atomic
    Content:
  - Title: Write the qor_randomsource page
    Status: Atomic
    Content:
  - Title: Write the qor_stringsource page
    Status: Atomic
    Content:
  - Title: Write the qor_serialize page
    Status: Atomic
    Content:
  - Title: Write the qor_generator page
    Status: Atomic
    Content:
  - Title: Write the qor_netclient page
    Status: Atomic
    Content:
  - Title: Write the qor_httpclient page
    Status: Atomic
    Content:
---

# Components pages group 3

## Goal

Seven steps of **one page each** across the tail of `pipeline/sources` and into `io`:
`qor_generatorsource`, `qor_randomsource`, `qor_stringsource`, `qor_serialize`, `qor_generator`,
`qor_netclient`, `qor_httpclient`.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: leaf 1 - Write the qor_generatorsource page` - not started. Opens at
  `coverage = 57 of 90` (the line `ComponentsPagesGroup2` closes on) and closes at
  `coverage = 64 of 90`, `uncovered_targets = 26`.
- **Two of these seven have folder names that do not match the target**: `qor_serialize` is
  `src/components/io/deserializer`, `qor_httpclient` is `src/components/io/network/clients/http`.
  The leaf cites the CMakeLists it read, which is the only way this class of mistake is caught.

## Context

Inherited from the parent (`../plan.md`, Level 2) and grandparent (`../../plan.md`, Level 1): one
page per build target in `docs/wiki/modules/`; `Judges:` is that target's own build command; the
target list was verified once by `Reconcile the target list`. **`../plan.md` §Decisions holds the
page contract - shape, read budget, judge rule, stop pair - and binds every leaf here.** Sibling
titles: `ComponentsPagesGroup1`, `ComponentsPagesGroup2`, `ComponentsPagesGroup4` ..
`ComponentsPagesGroup7`.

## Decisions

- **`qor_serialize` is page-named by its target, not its folder** - `docs/wiki/modules/` is keyed by
  target so the hook can map a diff to a page without an alias index (the parent's locked decision).
  A page called `deserializer.md` would be invisible to `wiki-census coverage`.

## Write the qor_generatorsource page

Input verified: `ComponentsPagesGroup2` closed on `coverage = 57 of 90`.

One page: `docs/wiki/modules/qor_generatorsource.md` for target `qor_generatorsource`
(`src/components/data/pipeline/sources/generatorsource/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `wiki-census coverage` re-reads `coverage = 58 of 90` with
`uncovered_targets = 32`, `wiki-lint schema` prints `illegal_fields = 0`,
`wiki-ref-check anchors` prints `dead = 0`.

## Write the qor_randomsource page

Input verified: `coverage = 58 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_randomsource.md` for target `qor_randomsource`
(`src/components/data/pipeline/sources/randomsource/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 59 of 90`, `uncovered_targets = 31`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_stringsource page

Input verified: `coverage = 59 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_stringsource.md` for target `qor_stringsource`
(`src/components/data/pipeline/sources/stringsource/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 60 of 90`, `uncovered_targets = 30`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_serialize page

Input verified: `coverage = 60 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_serialize.md` for target `qor_serialize`
(`src/components/io/deserializer/CMakeLists.txt` - the folder says `deserializer`), under the page
contract in [`../plan.md`](../plan.md).

Stop condition: `coverage = 61 of 90`, `uncovered_targets = 29`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_generator page

Input verified: `coverage = 61 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_generator.md` for target `qor_generator`
(`src/components/io/generator/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md). It is not `qor_generatorsource` - say which is which once.

Stop condition: `coverage = 62 of 90`, `uncovered_targets = 28`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_netclient page

Input verified: `coverage = 62 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_netclient.md` for target `qor_netclient`
(`src/components/io/network/client/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 63 of 90`, `uncovered_targets = 27`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_httpclient page

Input verified: `coverage = 63 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_httpclient.md` for target `qor_httpclient`
(`src/components/io/network/clients/http/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md). `qor_http` (`src/components/protocols/http`, D5) is a different
target and a different page - group 5's.

Stop condition: `coverage = 64 of 90`, `uncovered_targets = 26`, `illegal_fields = 0`, `dead = 0` -
this plan's close line, and the number `ComponentsPagesGroup4` opens on.
