---
Type: Plan
Title: "Components pages group 7"
Parent: "../plan.md"
Level: 3
Items:
  - Title: Write the qor_layout page
    Status: Atomic
    Content:
  - Title: Write the qor_opengles page
    Status: Atomic
    Content:
  - Title: Write the qor_renderer page
    Status: Atomic
    Content:
  - Title: Write the qor_tui page
    Status: Atomic
    Content:
  - Title: Write the qor_widgets page
    Status: Atomic
    Content:
---

# Components pages group 7

## Goal

The five densest libs in the tree, one page per leaf, and each of the five is stopped in front of
by its own stop line: `qor_layout`, `qor_opengles`, `qor_renderer`, `qor_tui`, `qor_widgets`.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `CLOSED 18 Nov 2026` - all 5 pages landed (`qor_layout`, `qor_opengles`, `qor_renderer`,
  `qor_tui`, `qor_widgets`); this group's close-line stop pair printed `coverage = 88 of 90`,
  `uncovered_targets = 2` (`qor_mime`, `qor_yaml` - the unindexed pair, the queue's to resolve, not
  this plan's), `illegal_fields = 0`, `dead = 0` (`anchors = 454 of 454`), each `VERDICT fail=0`.
  All five judges are green and link their own `.dll`. Group 7 lessons: `qor_renderer` is a vendored
  ThorVG fork whose `config.h` is dead (`//#include "config.h"` at `common/common.h:31`), so the
  `#ifdef THORVG_*_LOADER_SUPPORT` factories in `loader.cpp` preprocess out and only `RawLoader` is
  live - a green judge there proves the fork links, not that it decodes; `qor_renderer`'s `repo_map`
  row (`:125`) omits one of the seven linked libs (`qor_opengles`), the first index gap seen in this
  region; `qor_widgets` is in the "densest" group but is the group's floor (658 lines, 2 concrete
  types, no `src/` consumer - only `examples/experimental/canvastest` links it), so the group label
  names the queue backlog, not each lib. `qor_tui`/`qor_widgets` index rows are honest (6=6, 5=5).
- **Closed at the number `ClaimCarryingPages` opens on** - `coverage = 88 of 90`, the whole
  `LayerPages` lib-page pass complete.
- **This is the leaf the density rule exists for.** Five heavy targets that used to be one leaf, and
  each one is a session: read the target's own folder, emit its own ctags index if the claim needs
  symbols, write 200 lines, run one judge, print one line, stop.

## Context

Inherited from the parent (`../plan.md`, Level 2) and grandparent (`../../plan.md`, Level 1): one
page per build target in `docs/wiki/modules/`; `Judges:` is that target's own build command; the
target list was verified once by `Reconcile the target list`. **`../plan.md` §Decisions holds the
page contract - shape, read budget, judge rule, stop pair - and binds every leaf here**, and its
read budget is what makes a dense lib affordable at all. Sibling titles:
`ComponentsPagesGroup1` .. `ComponentsPagesGroup6`.

## Decisions

- **A dense page is short, not thin** - the 200-line budget (`docs/wiki/schema.md` §Budget) is met
  by naming anchors and counts, never by summarising symbol lists: `qor_renderer` is an anchor into
  `docs/agent/ctags/qor_renderer.tags` plus its symbol count, not a transcription of it (F1).
- **`qor_tui` is `src/components/ui/terminal`** and `qor_terminal`
  (`src/framework/ui/terminal`, `FrameworkPagesGroup4`) is not this page. Both exist; the folder
  names are identical.

## Write the qor_layout page

Input verified: `ComponentsPagesGroup6` closed on `coverage = 83 of 90`.

One page: `docs/wiki/modules/qor_layout.md` for target `qor_layout`
(`src/components/ui/layout/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md). It is not `qor_uilayout` (`src/framework/ui/layout`).

Stop condition: `wiki-census coverage` re-reads `coverage = 84 of 90` with
`uncovered_targets = 6`, `wiki-lint schema` prints `illegal_fields = 0`,
`wiki-ref-check anchors` prints `dead = 0`.

## Write the qor_opengles page

Input verified: `coverage = 84 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_opengles.md` for target `qor_opengles`
(`src/components/ui/opengles/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 85 of 90`, `uncovered_targets = 5`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_renderer page

Input verified: `coverage = 85 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_renderer.md` for target `qor_renderer`
(`src/components/ui/renderer/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md). The densest target in the tree - the read budget is not negotiable here,
and the ctags index is read for its count, never its rows.

Stop condition: `coverage = 86 of 90`, `uncovered_targets = 4`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_tui page

Input verified: `coverage = 86 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_tui.md` for target `qor_tui`
(`src/components/ui/terminal/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md). `qor_terminal` is the framework neighbour - different target, same
folder name.

Stop condition: `coverage = 87 of 90`, `uncovered_targets = 3`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_widgets page

Input verified: `coverage = 87 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_widgets.md` for target `qor_widgets`
(`src/components/ui/widgets/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 88 of 90` with `uncovered_targets = 2` and the two named
(`qor_mime`, `qor_yaml`), `illegal_fields = 0`, `dead = 0` - the close line of the whole
`LayerPages` lib-page pass, and the number `Claim-carrying pages` opens on.
