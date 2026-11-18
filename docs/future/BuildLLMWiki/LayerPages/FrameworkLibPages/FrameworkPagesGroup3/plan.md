---
Type: Plan
Title: "Framework pages group 3"
Parent: "../plan.md"
Level: 3
Items:
  - Title: Write the qor_sync page
    Status: Atomic
    Content:
  - Title: Write the qor_task page
    Status: Atomic
    Content:
  - Title: Write the qor_thread page
    Status: Atomic
    Content:
  - Title: Write the qor_workitem page
    Status: Atomic
    Content:
  - Title: Write the qor_uicommon page
    Status: Atomic
    Content:
  - Title: Write the qor_desktopui page
    Status: Atomic
    Content:
  - Title: Write the qor_gl page
    Status: Atomic
    Content:
---

# Framework pages group 3

## Goal

Seven steps of **one page each**: `qor_sync`, `qor_task`, `qor_thread`, `qor_workitem`,
`qor_uicommon`, `qor_desktopui`, `qor_gl`.

## Resume (state slot - the only mutable section of this plan)

Cache, not record. Read the front matter and this slot, re-derive from disk, rewrite if they differ.

- `Cursor: group 3 CLOSED 17 Nov 2026` - all 7 pages landed (`qor_sync`, `qor_task`, `qor_thread`,
  `qor_workitem`, `qor_uicommon`, `qor_desktopui`, `qor_gl`). Next is `FrameworkPagesGroup4` leaf 1,
  `qor_uilayout` ([`../FrameworkPagesGroup4/plan.md`](../FrameworkPagesGroup4/plan.md)), opening on
  this group's printed close line, `coverage = 38 of 90`. Leaf 7 closed it with
  `docs/wiki/modules/qor_gl.md` (147 lines, `Claims: 11`, 7 anchors) at `coverage = 38 of 90`,
  `uncovered_targets = 52`, `dangling_module_pages = 0`, `illegal_fields = 0`
  (`pages = 38 of 44`) and `dead = 0` (`anchors = 161 of 161`), each `VERDICT fail=0`. Its judge is
  the group's only red one and it is a finding, not a slip:
  `cmake --build cmakebuild --target qor_gl --parallel 8` -> `MSBUILD : error MSB1009: Project file
  does not exist. Switch: qor_gl.vcxproj` at exit 1, because no `CMakeLists.txt` descends into
  `src/framework/ui/gl` (`cmakebuild` holds 306 `.vcxproj`, 0 of them `qor_gl.vcxproj`).
  Leaf 7 is also the group's first **instrument-caught** defect: the body was intact and the numbers
  were not. `Measure-Object -Line` skips blank lines, so the two file sizes the page published
  (1,297 / 1,003) were blank-excluded while the total it published (5,007) was physical; the sibling
  sentence named five `.cpp` that `src/platform/os/windows/ui/gl/CMakeLists.txt:3-7` does not
  compile (it compiles `winqor_gl.cpp`, `cursorimpl.cpp`, `windowimpl.cpp`, `wgl.cpp`); and
  `symbol: Create` came back `DEAD` because the shape's symbol match rejects a symbol preceded by
  `::` - it has to be `WindowPimpl::Create`. Re-measured with `(Get-Content).Count` by
  `tmp/_tmp_lp_g3_l7_verify.ps1` before the stop pair, which is what makes `dead = 0` mean anything.
  Leaf 6 closed the same day with `docs/wiki/modules/qor_desktopui.md` (141 lines, `Claims: 10`,
  judge green) at `coverage = 37 of 90`; detail under "Write the qor_desktopui page" below.
- Opens at `coverage = 31 of 90` (the line `FrameworkPagesGroup2` closes on) and closes at
  `coverage = 38 of 90`, `uncovered_targets = 52`.
- The four `parallel` leaves are the ones the per-OS split bites on: `qor_thread` has
  `linqor_thread` and `winqor_thread` siblings under `src/platform/os/<os>/app/thread/`, and the
  page is about `qor_thread` only.

## Context

Inherited from the parent (`../plan.md`, Level 2) and grandparent (`../../plan.md`, Level 1): one
page per build target in `docs/wiki/modules/`; `Judges:` is that target's own build command; the
target list was verified once by `Reconcile the target list`. **`../plan.md` §Decisions holds the
page contract - shape, read budget, judge rule, stop pair - and binds every leaf here.** Sibling
titles: `FrameworkPagesGroup1`, `FrameworkPagesGroup2`, `FrameworkPagesGroup4`.

## Decisions

- **The judge of a parallel or UI target may be red for a platform reason** - a GL or desktop-UI
  target can fail to configure on a box without the SDK. That is written into the page's
  `Failure modes` with the printed line, exactly as `SeedPages` recorded for `qor_textshaping`
  (declared, but its `add_subdirectory` is commented out). A red judge is evidence, never a
  reason to skip the leaf or to hand-set `Status:`.

## Write the qor_sync page

**CLOSED 17 Nov 2026.** `docs/wiki/modules/qor_sync.md`, judge green.

Input verified then: `FrameworkPagesGroup2` closed on `coverage = 31 of 90`.

Target: `qor_sync` (`src/framework/parallel/sync/CMakeLists.txt`), page contract in
[`../plan.md`](../plan.md).

Stop condition printed: `coverage = 32 of 90`, `uncovered_targets = 58`, `illegal_fields = 0`,
`dead = 0`.

## Write the qor_task page

**CLOSED 17 Nov 2026.** `docs/wiki/modules/qor_task.md`, judge green. The target is cppcoro under a
QOR gate: `CPPCORO_COMPILER_MSVC` is defined nowhere, so its MSVC workaround never compiles on MSVC,
and `sync_wait` reaches into `qor_sync` over a link line that names `qor_host` instead.

Input verified then: `coverage = 32 of 90` printed by the leaf above.

Target: `qor_task` (`src/framework/parallel/task/CMakeLists.txt`), page contract in
[`../plan.md`](../plan.md).

Stop condition printed: `coverage = 33 of 90`, `uncovered_targets = 57`, `illegal_fields = 0`,
`dead = 0`.


## Write the qor_thread page

Input verified: `coverage = 33 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_thread.md` for target `qor_thread`
(`src/framework/parallel/thread/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md). Its two per-OS siblings are named in the page as siblings, not
absorbed into it.

Stop condition: `coverage = 34 of 90`, `uncovered_targets = 56`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_workitem page

Input verified: `coverage = 34 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_workitem.md` for target `qor_workitem`
(`src/framework/parallel/workitem/CMakeLists.txt`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 35 of 90`, `uncovered_targets = 55`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_uicommon page

**CLOSED 17 Nov 2026.** `docs/wiki/modules/qor_uicommon.md`, judge green. The target is 12 headers
and 2 translation units: only `Contained` carries the export marker, so the mixin behaviour is
recompiled into every consumer, and `QOR_PP_H_UI_COMPOUND` / `QOR_PP_H_COMPONENTS_UI_SELECTABLE` are
each shared with a `src/components/ui` header that no target builds.

Input verified then: `coverage = 35 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_uicommon.md` for target `qor_uicommon`
(`src/framework/ui/common/CMakeLists.txt` - folder `common`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 36 of 90`, `uncovered_targets = 54`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_desktopui page

**CLOSED 17 Nov 2026.** `docs/wiki/modules/qor_desktopui.md`, judge green. The target is five files
and two translation units whose only verb is `Run()` - and `Run()` is `return EXIT_FAILURE;`
(`src/framework/ui/desktop/desktopui.cpp:31`). `ImplementsDesktopUI()` is exported with this
target's token and defined nowhere in the target; the export token `QOR_DESKTOPUI` is written by 11
files in four folders and defined by exactly one `CMakeLists.txt`; and the only consumer,
`examples/experimental/visual`, is commented out of the build and includes a folder that does not
exist.

Input verified then: `coverage = 36 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_desktopui.md` for target `qor_desktopui`
(`src/framework/ui/desktop/CMakeLists.txt` - folder `desktop`), under the page contract in
[`../plan.md`](../plan.md).

Stop condition: `coverage = 37 of 90`, `uncovered_targets = 53`, `illegal_fields = 0`, `dead = 0`.

## Write the qor_gl page

**CLOSED 17 Nov 2026.** `docs/wiki/modules/qor_gl.md`, judge red by design. The biggest target in the
slice - 15 `.cpp` and 29 headers, 7,867 lines - and no `CMakeLists.txt` in the tree descends into
`src/framework/ui/gl`, so its `Judges:` row can only print `MSB1009: Project file does not exist`
and `cmakebuild` holds 306 `.vcxproj` with none named `qor_gl.vcxproj`. `TheSession()` has two
definitions (`session.cpp:241-244` and the Windows sibling's `session.cpp:201-204`), 11 of the
sibling's includes name a folder that does not exist (`src/framework/gl/`), 12 of the 29 headers are
named by none of the sources the target compiles, and 45 of the portable pimpls are
`"Override not implemented for the current platform"`.

Input verified: `coverage = 37 of 90` printed by the leaf above.

One page: `docs/wiki/modules/qor_gl.md` for target `qor_gl`
(`src/framework/ui/gl/CMakeLists.txt`), under the page contract in [`../plan.md`](../plan.md).

Stop condition: `coverage = 38 of 90`, `uncovered_targets = 52`, `illegal_fields = 0`, `dead = 0` -
this plan's close line, and the number `FrameworkPagesGroup4` opens on.
