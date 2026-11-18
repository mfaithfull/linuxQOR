---
Type: Plan
Title: "Components pages group 4"
Parent: "../plan.md"
Level: 3
Items:
  - Title: Write the qor_socketclient page
    Status: Atomic
    Content:
  - Title: Write the qor_netserver page
    Status: Atomic
    Content:
  - Title: Write the qor_fileconnector page
    Status: Atomic
    Content:
  - Title: Write the qor_pipeconnector page
    Status: Atomic
    Content:
  - Title: Write the qor_socketconnector page
    Status: Atomic
    Content:
  - Title: Write the qor_stdfstreamconnector page
    Status: Atomic
    Content:
  - Title: Write the qor_stdofstreamsink page
    Status: Atomic
    Content:
---

# Components pages group 4

## Goal

Seven steps of **one page each** over the network clients/server and the connector family:
`qor_socketclient`, `qor_netserver`, `qor_fileconnector`, `qor_pipeconnector`,
`qor_socketconnector`, `qor_stdfstreamconnector`, `qor_stdofstreamsink`.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: leaf 1 - Write the qor_socketclient page` - not started. Opens at `coverage = 64 of 90`
  (the line `ComponentsPagesGroup3` closes on) and closes at `coverage = 71 of 90`,
  `uncovered_targets = 19`.
- The connector family is the one place in this region where four targets share a parent folder
  (`src/components/io/pipeline/connectors/`), and each still gets its own leaf: the shared parent is
  a reason to be careful, not a reason to batch.

## Context

Inherited from the parent (`../plan.md`, Level 2) and grandparent (`../../plan.md`, Level 1): one
page per build target in `docs/wiki/modules/`; `Judges:` is that target's own build command; the
target list was verified once by `Reconcile the target list`. **`../plan.md` §Decisions holds the
page contract - shape, read budget, judge rule, stop pair - and binds every leaf here.** Sibling
titles: `ComponentsPagesGroup1` .. `ComponentsPagesGroup3`, `ComponentsPagesGroup5` ..
`ComponentsPagesGroup7`.

## Decisions

- **Read the child's `CMakeLists.txt`, never the parent's, for a claim about a target** - the four
  connector folders are siblings; a claim read off `connectors/CMakeLists.txt` would be about four
  targets at once and falsifiable by none of them (F10's shape: one field, no owner).

## Write the qor_socketclient page

Input verified: `ComponentsPagesGroup3` closed on `coverage = 64 of 90`.

One page: `docs/wiki/modules/qor_socketclient.md` for target `qor_socketclient`
(`src/components/io/network/clients/socket/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `wiki-census coverage` re-reads `coverage = 65 of 90` with
`uncovered_targets = 25`, `wiki-lint schema` prints `illegal_fields = 0`,
`wiki-ref-check anchors` prints `dead = 0`.

## Write the qor_netserver page

Input verified: `coverage = 65 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_netserver.md` for target `qor_netserver`
(`src/components/io/network/server/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 66 of 90`, `uncovered_targets = 24`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_fileconnector page

Input verified: `coverage = 66 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_fileconnector.md` for target `qor_fileconnector`
(`src/components/io/pipeline/connectors/fileconnector/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 67 of 90`, `uncovered_targets = 23`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_pipeconnector page

Input verified: `coverage = 67 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_pipeconnector.md` for target `qor_pipeconnector`
(`src/components/io/pipeline/connectors/pipeconnector/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 68 of 90`, `uncovered_targets = 22`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_socketconnector page

Input verified: `coverage = 68 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_socketconnector.md` for target `qor_socketconnector`
(`src/components/io/pipeline/connectors/socketconnector/CMakeLists.txt`), under the page contract
in [`../plan.md`](../plan.md).

Stop condition: `coverage = 69 of 90`, `uncovered_targets = 21`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_stdfstreamconnector page

Input verified: `coverage = 69 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_stdfstreamconnector.md` for target `qor_stdfstreamconnector`
(`src/components/io/pipeline/connectors/stdfstreamconnector/CMakeLists.txt`), under the page
contract in [`../plan.md`](../plan.md).

Stop condition: `coverage = 70 of 90`, `uncovered_targets = 20`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_stdofstreamsink page

Input verified: `coverage = 70 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_stdofstreamsink.md` for target `qor_stdofstreamsink`
(`src/components/io/pipeline/sinks/stdofstreamsink/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 71 of 90`, `uncovered_targets = 19`, `illegal_fields = 0`, `dead = 0` -
this plan's close line, and the number `ComponentsPagesGroup5` opens on.
