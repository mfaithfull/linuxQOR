---
Type: Plan
Title: "Window TU batch"
Parent: "../plan.md"
Level: 2
Items:
  - Title: window BOOL sites
    Status: Refined
    Content: ./WindowBOOLSites/plan.md
  - Title: window HWND sites
    Status: Refined
    Content: ./WindowHWNDSites/plan.md
  - Title: window other sites
    Status: Refined
    Content: ./WindowOtherSites/plan.md
---

# Window TU batch

Slug: `WindowTUBatch` - from title "Window TU batch" (no drop-words, PascalCased, 13 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **86** `window.cpp` sites, cut by documented return family then by the leaf budget, so every leaf is one edit pass of at most 10 sites.

## Resume

Cursor: **terminal** - all 3 items `Refined` with children on disk, all 3 closed. Item 3 **window other sites** is **CLOSED 13 Nov 2026** - `./WindowOtherSites/plan.md` is terminal at 2 closed `Atomic` items, and all **14** other-family rows sit in their end state (`token=8 bare=5 void=1`, `tmp/_tmp_u2_wo_sites.ps1`): the leaf's 8 `checked` rows landed `CheckReturn< >` at their own published type (`CheckNonZero< UINT >` x3 - `ArrangeIconicWindows`, `RealGetWindowClassT`, `GetDpiForWindow`; `CheckNonZero< WORD >` x2 - `CascadeWindows`, `TileWindows`; `CheckNonZeroHandle< HDWP >` x2 - `BeginDeferWindowPos`, `DeferWindowPos`; `CheckNonZero< DWORD >` x1 - `GetWindowThreadProcessId`), the 5 `unchecked` rows stayed bare, and `SwitchToThisWindow` (not `CascadeWindows`, as this leaf's `## Context` had it) is the `void` row and took no token; caller audits for `TileWindows` (windowcontroller.cpp:443) and `GetDpiForWindow` (windowcontroller.cpp:792, inside `WindowController::GetDPI()`) were read before landing, both untested pass-throughs that leave the value identical. The per-TU re-census `tmp/_tmp_u2_site_census_win.ps1` reads `rows=86 bare-checked=0 wrapped-unchecked=0 multi-or-void-wrap=0 VERDICT fail=0` and `window.cpp` `CheckReturn<` = **50** = the table's 50 `checked` rows across all 10 `window.cpp` slices (BOOL 37 + HWND 5 + other 8); gates `window.cpp exit=TU_EXIT=0 diags=0 dating=obj-newer ok` and folder `srcs=54 not-in-log=0 stale=0 real-diag-lines=0`. Item 2 **window HWND sites** is **CLOSED 12 Nov 2026** - `./WindowHWNDSites/plan.md` terminal at 2 closed `Atomic` items, census `CreateWindowEx cr=1` / `FindWindow cr=1` / `FindWindowEx cr=1` / `GetAncestor cr=1` / `SetParent cr=1` with `cr=0` for all 12 `unchecked` HWND rows, `window.cpp CheckReturn<` = 42 at that stop (the census instrument's `$l`/`$L` case collision and missing `T`-suffix stem mapping are recorded in that leaf). Item 1 **window BOOL sites** is **CLOSED 12 Nov 2026** - `./WindowBOOLSites/plan.md` terminal at 6 closed `Atomic` items, all **37** `checked` BOOL rows landed across 6 fetch-free slices, the 18 `unchecked` BOOL rows bare, one include. Terminal: all 3 items `Refined` with children on disk. **Hard precondition: these rows carry verdicts** (`EndpointReturnSemantics` subtree) - met: all 86 `window.cpp` rows carry a verdict triple.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the parent's (`../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, caller audit for nonzero counts, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: Large TU batch L1, Large TU batch L2, Large TU batch L3.

Measured by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt`: `window.cpp` = 86 rows, BOOL 55 / HWND 17 / other 14, cut into 6 + 2 + 2 = 10 slices. This TU holds the heaviest nonzero caller counts (`GetDlgCtrlID` 8, `SetWindowPos` 5, `GetDC` 4, `ReleaseDC` 3) - every `checked` row here carries its `registers/u19.md` audit note before the token moves.

## window BOOL sites

55 sites in `window.cpp` published `BOOL`, 6 slices. Child plan: `./WindowBOOLSites/plan.md` (Level 2 -> 3, 6 `Atomic` items, terminal).

## window HWND sites

17 sites published `HWND`, 2 slices. Child plan: `./WindowHWNDSites/plan.md` (2 `Atomic` items, terminal).

## window other sites

14 sites (`UINT` 4, `int` 3, `WORD` 3, `HDWP` 2, `DWORD` 1, `void` 1), 2 slices. Child plan: `./WindowOtherSites/plan.md` (2 `Atomic` items, terminal).
