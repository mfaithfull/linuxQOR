---
Type: Plan
Title: "Framework lib pages"
Parent: "../plan.md"
Level: 2
Items:
  - Title: Framework pages group 1
    Status: Refined
    Content: ./FrameworkPagesGroup1/plan.md
  - Title: Framework pages group 2
    Status: Refined
    Content: ./FrameworkPagesGroup2/plan.md
  - Title: Framework pages group 3
    Status: Refined
    Content: ./FrameworkPagesGroup3/plan.md
  - Title: Framework pages group 4
    Status: Refined
    Content: ./FrameworkPagesGroup4/plan.md
---

# Framework lib pages

## Goal

One page per `src/framework` QOR lib target - 26 targets, and one target per `Atomic` leaf.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor:` this plan is **CLOSED 17 Nov 2026** - all 4 groups landed their 26 pages.
  **Group 4 CLOSED 17 Nov 2026** ([`./FrameworkPagesGroup4/plan.md`](./FrameworkPagesGroup4/plan.md)):
  all 5 pages landed (`qor_uilayout`, `qor_uiselector`, `qor_sercon`, `qor_terminal`,
  `qor_uiwidgets`); leaf 5 printed this plan's close line: `coverage = 43 of 90`,
  `uncovered_targets = 47`, `dangling_module_pages = 0`, `illegal_fields = 0`
  (`pages = 43 of 49`, `strays = 0`), `dead = 0` (`anchors = 190 of 190`), each `VERDICT fail=0`;
  provenance unchanged at `violations = 2`. Two notes the last leaves add: an already-built target
  makes the judge an msbuild no-op - green, 0 compile lines - so the honest claim is a stamp-check
  (dll timestamp vs newest source mtime), and the export table is a cheap provenance probe: the
  `qor_uiwidgets` dll re-publishes 57 of its 78 exports as other modules' inline definitions.
  **Group 4 leaf 1 CLOSED 17
  Nov 2026** - `docs/wiki/modules/qor_uilayout.md` (153 lines, `Claims: 10`, 7 anchors, judge green)
  printed `coverage = 39 of 90`, `uncovered_targets = 51`, `illegal_fields = 0`
  (`pages = 39 of 45`), `dead = 0` (`anchors = 168 of 168`), each `VERDICT fail=0`. Two habits this
  leaf needs: check the enclosing region before calling commented-out code a compile landmine
  (`focused` is inside `/* */` at all 12 of its sites), and diff a folder inventory against the
  `add_library` source list first - that is what exposed 1,212 uncompiled `.cpp` lines here.
  **Group 3 CLOSED 17 Nov
  2026** - all 7 pages landed (`qor_sync`, `qor_task`, `qor_thread`, `qor_workitem`, `qor_uicommon`,
  `qor_desktopui`, `qor_gl`) and its close line printed `coverage = 38 of 90`,
  `uncovered_targets = 52`, `dangling_module_pages = 0`, `illegal_fields = 0` (`pages = 38 of 44`),
  `dead = 0` (`anchors = 161 of 161`), each `VERDICT fail=0`.
  **A page body can be intact and its numbers wrong.** Leaf 7's body had no seam, and three of its
  counts were wrong: two file sizes were `Measure-Object -Line` counts - that cmdlet skips blank
  lines - sitting next to a physical-line total, one sentence named `.cpp` files a sibling
  `CMakeLists.txt` does not compile, and `symbol: Create` returned `DEAD` because the shape's symbol
  match rejects a symbol preceded by `::`. Measure sizes with `(Get-Content).Count`, and qualify an
  anchor symbol with its class (`WindowPimpl::Create`) - same rule `Font::Claim` taught in group 1.
  **Group 2 CLOSED 17 Nov 2026** - all 7 pages landed (`qor_resources`, `qor_image`, `qor_events`,
  `qor_signals`, `qor_asyncioservice`, `qor_filesystem`, `qor_sockets`) and its close line printed
  `coverage = 31 of 90`, `uncovered_targets = 59`, `dangling_module_pages = 0`, `illegal_fields = 0`
  (`pages = 31 of 37`), `dead = 0` (`anchors = 118 of 118`), each `VERDICT fail=0`. This plan closes
  at `coverage = 43 of 90` / `uncovered_targets = 47` (the pre-refinement slices promised the same
  two numbers).
- **Read every page body before its stop pair runs.** 3 of group 2's 7 pages landed truncated - a claim
  bullet cut mid-sentence with `## Failure modes` written over its tail and the orphan parked after
  `## See also` (`qor_asyncioservice`, `qor_filesystem`, `qor_sockets`; `qor_image` the same on 16
  Nov). All three were invisible to the instruments and only reading found them. Group 2's resume slot
  owns the detail; the rule binds groups 3-4.
- **Re-cut 16 Nov 2026: the 10-page leaf was never one step.** The three `Atomic` slices of
  10 + 10 + 6 pages are replaced by **26 leaves of one page each** in 4 `Refined` groups of
  7 / 7 / 7 / 5 - seven because the breadth cap in `docs/future/PLAN-FORMAT.md` is 7 items per
  plan, not because a page costs ten. The measured cost of the old shape, all of it slice 1: 35
  `tmp/_tmp_fw*` probe files between 12:56 and 18:08, 2 of 10 pages landed, newest probe
  (`tmp/_tmp_fw1_fact4.txt`) still reading whole `.cpp` files for `qor_host` when the context
  cycled. The cut is fixed by `tmp/_tmp_llmwiki_page_cut.ps1` -> `.txt` (recreate by slug):
  `framework claimed = 26 of 26  owned_once = 26  landed = 2  VERDICT fail=0`.
- `Remaining` - none: all 26 framework leaves have landed. The next sibling in the parent's item
  order is `ComponentsLibPages` (45 one-page leaves), then `ClaimCarryingPages` (untouched: 3
  `Atomic` + 1 `Refined`). `ComponentsPagesGroup1` opens on this plan's close line,
  `coverage = 43 of 90`.

## Context

Inherited at creation - re-read rather than trusted, the tree has moved since:

- **Parent goal**: populate the wiki bounded by 90 distinct build targets.
- **Parent locked decisions**: one page per target in `docs/wiki/modules/`; `Judges:` is the target's
  build command; the target list is verified before pages are written.
- **Sibling plan titles**: `SeedPages` (runs first, 17 pages), `ComponentsLibPages`,
  `ClaimCarryingPages`.
- **Measured**: 26 `src/framework` targets. 27 folders under `src/framework` declare a `qor_`
  target; `qor_mime` (`src/framework/data/mime`) is the one that is not a unit of this plan - it is
  one of the two targets with no `repo_map.md` row, named by `Reconcile the target list`.

## Decisions

- **A page is not a unit, so a leaf is one page** - `docs/future/PLAN-FORMAT.md`: "A **unit** is work
  an agent performs by hand - a fetch, an edit, a lookup, a written row." One module page is several
  lookups (its `CMakeLists.txt`, its headers, its `repo_map.md` row, its `purposes.md` line), one
  write and one judge build, so ten pages is tens of units and no slice size makes that leaf atomic.
  The re-cut keeps every target id and the entire target order; only the group boundaries move, and
  they move to the breadth cap of 7.
- **The page contract, stated once here; every leaf in the four groups is bound by it.**
  1. *Shape*: `docs/wiki/modules/<target>.md` with `Type: WikiPage`, `PageType: module`, `Title:`,
     `Target:`, `Status:`, `LastVerified:`, `Claims:`, `Summary:`, `Anchors:` (3-6, each
     `kind: in-tree` with `path:` / `symbol:` / `verified:`) and `Judges:` - the 13 fields of
     `docs/wiki/schema.md`. `VerifiedAgainst:` is **not** one of them; `SeedPages` measured that
     writing it prints `illegal_fields > 0`. Body: purpose line, claims each naming the file it was
     read from, `## Failure modes`, `## See also` links to pages that exist. 200 lines is the budget.
  2. *Read budget - scoped 17 Nov 2026 after group 1 broke it and group 2 proved the scoping right*:
     the target's own `CMakeLists.txt`, the public headers in its own folder, its
     `docs/agent/repo_map.md` row, its `docs/agent/purposes.md` line, and tree-wide **counts**.
     Forbidden, and this is what the rule exists to stop: dumping a whole `.cpp` into a `tmp/` report
     to be read back, and reading a `.cpp` outside the target's own folder. Permitted: a bounded read
     of one file in the target's own folder - whole file if it is a few dozen lines, otherwise a
     stated line range - taken once, for one named claim. A defect that lives in a `.cpp` body is a
     real defect, and a rule that leaves it unrecorded is wrong, not the agent. A claim that needs a
     symbol list emits the per-lib ctags index (D7) and reads its count, not its rows.
  3. *Judge*: exactly one build per leaf, `cmake --build cmakebuild --target <target> --parallel 8` -
     never `build`, which `SeedPages` measured red for want of a `BUILD_NUMBER` cache entry. A red
     judge goes into `## Failure modes` with its printed line; `Status:` is never hand-set, because
     `schema.md` gives that power to `wiki-reconcile` alone.
  4. *Stop pair*: the leaf's own `coverage = N of 90`, plus `illegal_fields = 0` and `dead = 0`. One
     census per leaf, and no second unit of the same kind after it.
  5. *Three spellings the instruments enforce and prose does not* - each cost a red run in group 1 or
     group 2 leaf 1, so they are contract, not taste.
     (a) An `Anchors:` `symbol:` must be written as the file writes it. `Test-WikiSymbolInFile`
     (`tools/wiki-census/wiki-shape.ps1`) matches `(?<![A-Za-z0-9_:])symbol`, so a lookbehind rejects a
     symbol preceded by `:` - `Claim` is dead against `void Font::Claim()` and `Font::Claim` resolves.
     (b) `Claims:` is informational to `wiki-lint`, so nothing checks it: set it to the count of
     `^- ` bullets in the body, which is what a reader counts.
     (c) A `[[wikilink]]` to a page that does not exist yet is not caught by any instrument
     (`dangling_module_pages` is the opposite direction - a page with no target). Link only the 25
     pages on disk; `See also` the dependency's own page when it has one, otherwise the folder path.
- **Order follows the dependency direction, and one claim in the old version of this list was
  wrong** - `qor_module` is seeded by `SeedPages`; **`qor_host` is not** - no
  `docs/wiki/modules/qor_host.md` existed on 16 Nov and it is one of the 26
  (`src/framework/app/host/CMakeLists.txt`). It is leaf 3 of group 1. Order still earns its keep
  through linking: a page whose dependency already has a page can cite it, which is what gives
  `wiki-ref-check refs` something to check.
- **The 24/26 count is measured, not inherited from the design** - §13.1's prose said 24; the disk
  says 26. The instrument is the authority and the leaves are cut from the disk list.

## Framework pages group 1

**CLOSED 16 Nov 2026.** See [`./FrameworkPagesGroup1/plan.md`](./FrameworkPagesGroup1/plan.md) -
7 `Atomic` leaves of **one page each**, in the inherited order, all landed: `qor_application`,
`qor_dynamiclibrary`, `qor_host`, `qor_optionparser`, `qor_role`, `qor_workflow`, `qor_pipeline`.

Input verified: the target list from the parent's reconciling item, and the 17 core pages of
`SeedPages` (which is what the `See also` lines resolve against).

Stop condition printed by leaf 7 (the group's close line): `coverage = 24 of 90`,
`uncovered_targets = 66`.

**Read-budget note, resolved.** The 2. *Read budget* rule was held only partly in leaves 5-7: those
leaves read whole `.cpp` files (`role.cpp`, `workflow.cpp`, `element.cpp`) to find their strongest
failure mode, and produced the group's three best claims. The rule has been scoped in Decisions
§2 rather than re-asserted - the thing it forbids is a `.cpp` dumped into `tmp/` and re-read, not a
bounded read of the target's own file. Groups 2-4 run under that scope.

## Framework pages group 2

**CLOSED 17 Nov 2026.** See [`./FrameworkPagesGroup2/plan.md`](./FrameworkPagesGroup2/plan.md) -
7 `Atomic` leaves of one page each, all landed: `qor_resources`, `qor_image`, `qor_events`,
`qor_signals`, `qor_asyncioservice`, `qor_filesystem`, `qor_sockets`.

Input verified: group 1's close line, `coverage = 24 of 90`.

Stop condition printed by leaf 7: `coverage = 31 of 90`, `uncovered_targets = 59`.

## Framework pages group 3

**CLOSED 17 Nov 2026.** See [`./FrameworkPagesGroup3/plan.md`](./FrameworkPagesGroup3/plan.md) -
7 `Atomic` leaves of one page each, all landed: `qor_sync`, `qor_task`, `qor_thread`,
`qor_workitem`, `qor_uicommon`, `qor_desktopui`, `qor_gl`.

Input verified: group 2's close line, `coverage = 31 of 90`.

Stop condition printed by leaf 7 (this group's close line): `coverage = 38 of 90`,
`uncovered_targets = 52`.

## Framework pages group 4

See [`./FrameworkPagesGroup4/plan.md`](./FrameworkPagesGroup4/plan.md) - 5 `Atomic` leaves of one
page each: `qor_uilayout`, `qor_uiselector`, `qor_sercon`, `qor_terminal`, `qor_uiwidgets`.

Input verified: group 3's close line, `coverage = 38 of 90`.

Stop condition (printed by leaf 5): `coverage = 43 of 90`, `uncovered_targets = 47` - this plan's
close line, and the number `ComponentsLibPages` opens on.
