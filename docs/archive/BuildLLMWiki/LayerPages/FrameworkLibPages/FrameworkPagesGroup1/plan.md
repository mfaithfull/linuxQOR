---
Type: Plan
Title: "Framework pages group 1"
Parent: "../plan.md"
Level: 3
Items:
  - Title: Write the qor_application page
    Status: Atomic
    Content:
  - Title: Write the qor_dynamiclibrary page
    Status: Atomic
    Content:
  - Title: Write the qor_host page
    Status: Atomic
    Content:
  - Title: Write the qor_optionparser page
    Status: Atomic
    Content:
  - Title: Write the qor_role page
    Status: Atomic
    Content:
  - Title: Write the qor_workflow page
    Status: Atomic
    Content:
  - Title: Write the qor_pipeline page
    Status: Atomic
    Content:
---

# Framework pages group 1

## Goal

Seven steps of **one page each** over the head of the `src/framework` target list:
`qor_application`, `qor_dynamiclibrary`, `qor_host`, `qor_optionparser`, `qor_role`,
`qor_workflow`, `qor_pipeline`.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- **PLAN CLOSED 16 Nov 2026.** `Cursor: closed - all 7 leaves landed`. All seven pages are on disk in
  `docs/wiki/modules/` (`qor_application`, `qor_dynamiclibrary`, `qor_host`, `qor_optionparser`,
  `qor_role`, `qor_workflow`, `qor_pipeline`) and the close line printed
  `coverage = 24 of 90`, `uncovered_targets = 66`, `dangling_module_pages = 0`,
  `illegal_fields = 0` (`pages = 24 of 30`), `dead = 0` (`anchors = 76 of 76`), each `VERDICT fail=0`.
  `FrameworkPagesGroup2` opens on `coverage = 24 of 90`.
- This plan exists because the 10-page leaf was never one step: 16 Nov 2026 left **35**
  `tmp/_tmp_fw*` probe files (12:56-18:08) and 2 of 10 pages, the newest probe
  (`tmp/_tmp_fw1_fact4.txt`) still on `qor_host` and reading whole `.cpp` files. The parent's read
  budget is what stops the repeat, not more willpower. The re-cut held: leaves 3-7 took one probe
  script each at most, and every leaf closed on its own printed number.
- Opens at `coverage = 19 of 90`; closes at `coverage = 24 of 90` / `uncovered_targets = 66`. **Both
  printed.**

## Context

Inherited from the parent (`../plan.md`, Level 2) and grandparent (`../../plan.md`, Level 1): the
unit of wiki work is a build target; one page per target in `docs/wiki/modules/`; `Judges:` is that
target's own build command; the target list was verified once by `Reconcile the target list`
(`targets = 90 of 90`, `unindexed = 2 [qor_mime qor_yaml]`). **`../plan.md` §Decisions holds the
page contract - shape, read budget, judge rule, stop pair - and it binds every leaf here; a leaf
restates none of it.** Sibling titles: `FrameworkPagesGroup2`, `FrameworkPagesGroup3`,
`FrameworkPagesGroup4`.

## Decisions

- **The two landed pages keep their own leaves** instead of dissolving into prose - the cut must
  stay provable: `tmp/_tmp_llmwiki_page_cut.ps1` -> `.txt` (recreate by slug) reads these front
  matters and prints `framework claimed = 26 of 26  owned_once = 26  landed = 2  VERDICT fail=0`.
- **Group boundaries fall inside the old 10-slices; the target order does not.** `qor_pipeline` was
  slice 1's tail. Order carries the dependency direction and the densest-last rule; a boundary is
  only a file split at the breadth cap of 7.

## Write the qor_application page

**CLOSED 16 Nov 2026.** One page for `qor_application`
(`src/framework/app/application/CMakeLists.txt`), 6 claims and 1 failure mode, judge
`cmake --build cmakebuild --target qor_application --parallel 8`.

Input verified then: `targets = 90 of 90`, and the 17 core pages of `SeedPages`.

Stop condition (printed then): the page exists and `wiki-census pages` counted it - `pages = 18`.

## Write the qor_dynamiclibrary page

**CLOSED 16 Nov 2026.** One page for `qor_dynamiclibrary`
(`src/framework/app/dynamiclibrary/CMakeLists.txt`), 6 claims and 2 failure modes, among them the
misspelled gate token `QOR_DYNAMICLIBARY`.

Input verified then: the same target list, and `qor_application` from the leaf above.

Stop condition (printed then): the page exists and `coverage = 19 of 90` counted it.

## Write the qor_host page

**CLOSED 16 Nov 2026.** `docs/wiki/modules/qor_host.md`, 6 claims and 4 failure modes, judge
`cmake --build cmakebuild --target qor_host --parallel 8` (green). Its strongest claim: the
`qor_host` <-> `qor_module` cycle is suppressed by `/ignore:4217`
(`src/framework/app/host/CMakeLists.txt`) rather than broken.

Input verified then: `targets = 90 of 90`, leaves 1-2 landed, `coverage = 19 of 90`.

Stop condition printed: `coverage = 20 of 90`, `uncovered_targets = 70`, `dangling_module_pages = 0`,
`illegal_fields = 0`, `dead = 0`.

## Write the qor_optionparser page

**CLOSED 16 Nov 2026.** `docs/wiki/modules/qor_optionparser.md`, 6 claims and 4 failure modes. The
target is declared but never configured - `src/framework/app/CMakeLists.txt` has no
`add_subdirectory(optionparser)`, so its judge cannot go green
(`MSBUILD : error MSB1009: Project file does not exist.`).

Input verified then: `coverage = 20 of 90` printed by the leaf above.

Stop condition printed: `coverage = 21 of 90`, `uncovered_targets = 69`, `illegal_fields = 0`,
`dead = 0`.

## Write the qor_role page

**CLOSED 16 Nov 2026.** `docs/wiki/modules/qor_role.md`, 6 claims and 4 failure modes, judge
`cmake --build cmakebuild --target qor_role --parallel 8` (green). The `Role` copy leaves each
feature's `m_Role` naming the original.

Input verified then: `coverage = 21 of 90` printed by the leaf above.

Stop condition printed: `coverage = 22 of 90`, `uncovered_targets = 68`, `illegal_fields = 0`,
`dead = 0`.

## Write the qor_workflow page

**CLOSED 16 Nov 2026.** `docs/wiki/modules/qor_workflow.md`, 6 claims and 4 failure modes, judge
`cmake --build cmakebuild --target qor_workflow --parallel 8` (green). It trades headers with
`qor_application` over no link edge in either direction.

Input verified then: `coverage = 22 of 90` printed by the leaf above.

Stop condition printed: `coverage = 23 of 90`, `uncovered_targets = 67`, `illegal_fields = 0`,
`dead = 0`.

## Write the qor_pipeline page

**CLOSED 16 Nov 2026.** `docs/wiki/modules/qor_pipeline.md`, 6 claims and 3 failure modes, judge
`cmake --build cmakebuild --target qor_pipeline --parallel 8` (green). `Element`'s copy constructor
forwards to `operator=` which branches on the uninitialised `m_buffer`.

Input verified then: `coverage = 23 of 90` printed by the leaf above.

Stop condition printed: `coverage = 24 of 90`, `uncovered_targets = 66`, `illegal_fields = 0`,
`dead = 0` - this plan's close line.

