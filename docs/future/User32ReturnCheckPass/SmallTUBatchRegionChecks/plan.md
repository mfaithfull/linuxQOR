---
Type: Plan
Title: "Small TU batch region checks"
Parent: "../plan.md"
Level: 1
Items:
  - Title: Small TU batch S1
    Status: Refined
    Content: ./SmallTUBatchS1/plan.md
  - Title: Small TU batch S2
    Status: Refined
    Content: ./SmallTUBatchS2/plan.md
  - Title: Small TU batch S3
    Status: Refined
    Content: ./SmallTUBatchS3/plan.md
  - Title: Small TU batch S4
    Status: Refined
    Content: ./SmallTUBatchS4/plan.md
  - Title: Small TU batch S5
    Status: Refined
    Content: ./SmallTUBatchS5/plan.md
---

# Small TU batch region checks

Slug: `SmallTUBatchRegionChecks` - from title "Small TU batch region checks" (no drop-words, PascalCased, exactly 24 chars, at the cap). It mirrors the sibling tree's `BatchSmallTUs` boundaries: the same 33 TUs, the same S1-S5 order, the same site counts, so the two passes re-census against one another. Uniqueness is stored here, not recomputed.

## Goal

Land `CheckReturn< sdkRetType, Check >::TType` at exactly the `checked` verdicts of the **124 sites over the 33 smallest TUs** of `src/platform/os/windows/api_layer/user`, smallest-first, and record the `unchecked` verdicts as written reasons - so the small-TU half of U2 resolves against the table instead of against judgement.

## Resume

Cursor: terminal - all 5 items closed. Terminal: all 5 items closed. **Hard precondition: these rows must already carry verdicts.** Items 1 **Small TU batch S1**, 2 **Small TU batch S2**, 3 **Small TU batch S3**, 4 **Small TU batch S4** and 5 **Small TU batch S5** are **CLOSED 12 Nov 2026**. Item 5 **Small TU batch S5** is **CLOSED 12 Nov 2026** (36 sites over 5 TUs - `displayconfig` 7, `systemshutdown` 6, `caret` 8, `rawinput` 7, `devicecontext` 8: **33 carry a `CheckReturn`** (32 newly landed - 7 `displayconfig`, 4 `systemshutdown`, 8 `caret`, 7 `rawinput`, 7 `devicecontext`) and the `systemshutdown.cpp:134` seed already landed), **2 stayed unchecked with the written reason at the site** (`CancelShutdown`, `ReleaseDC`), 0 rows are `void`; per-TU census `tus=1 fail=0` for each TU, standalone `/W4` leg `tus=1 fail=0` each at exit 0 / 0 diagnostics / obj newer, `../returncheck.h` includes went 0 -> 1 in 4 of the 5 TUs (`systemshutdown.cpp` already carried its 1 from the seed); layer-wide live `CheckReturn<` code sites now **79 over 28 TUs** with **28** `../returncheck.h` includes, `tmp/_tmp_u2_live_checks.txt`). Item 1 **Small TU batch S1** is **CLOSED 12 Nov 2026** (10 sites decided, 8 tokens landed, 2 written-reason `unchecked`, `tmp/_tmp_u2_code_census.txt` `tus=7 fail=0`, `tmp/_tmp_u2_gate_tus.out.txt` `tus=7 fail=0` at exit 0 / 0 diagnostics, `registers/u2.md` ticked the same session). Item 2 **Small TU batch S2** is **CLOSED 12 Nov 2026** (18 sites decided, 11 tokens landed, 5 written-reason `unchecked`, per-TU census `tus=7 fail=0`, standalone `/W4` leg `tus=7 fail=0`, `registers/u2.md` ticked the same session). Item 3 **Small TU batch S3** is **CLOSED 12 Nov 2026** (27 sites decided: 11 tokens landed + the `errorhandling.cpp:85` seed already landed, 13 written-reason `unchecked`, 2 `void`; census `tmp/_tmp_u2_code_census.txt` `tus=7 fail=0`, `tmp/_tmp_u2_gate_tus.out.txt` `tus=7 fail=0` at exit 0 / 0 diagnostics / objs newer, 0 of the 5 seeds re-edited, `registers/u2.md` ticked the same session; layer-wide live `CheckReturn<` code sites now **37 over 20 TUs**, `tmp/_tmp_u2_live_checks.txt`). Item 4 **Small TU batch S4** is **CLOSED 12 Nov 2026** (33 sites decided over 7 TUs - `monitor` 5, `windowproperties` 5, `processthread` 5, `systeminfo` 4, `ime` 5, `help` 5, `text` 4: **12 carry a `CheckReturn`** (9 newly landed - `EnumDisplayMonitors`/`GetMonitorInfo`/`SetProp`/`SetMenuContextHelpId`/`SetWindowContextHelpId`/`WinHelp` `BoolCheck`, `GetSystemMetrics` `CheckNonZero< int >`, `SetSysColors`/`SystemParametersInfo` `BoolCheck`) and the 3 `processthread.cpp` seeds never re-edited), **16 stayed unchecked with the written reason at the site**, 1 row is `void` (`DisableProcessWindowsGhosting`); per-TU census `tus=1 fail=0` for each TU, standalone `/W4` leg `tus=1 fail=0` each at exit 0 / 0 diagnostics / obj newer, `../returncheck.h` includes went 0 -> 1 in 5 of the 7 TUs (`ime.cpp` and `text.cpp` carry 0 checks and keep 0 includes); layer-wide live `CheckReturn<` code sites now **46 over 24 TUs** with **24** includes (`tmp/_tmp_u2_live_checks.txt`), `registers/u2.md` ticked each TU the same session). **Hard precondition per batch: that batch's rows must already carry verdicts** - S1-S5's 124 rows now all do (`tmp/_tmp_u2_filled.txt`: `tus=53 rows=597 filled=124 full-tus=33 partial=0 empty=0`).

## Context

Per-TU site counts on disk (measured by `tmp/_tmp_u2_tree_verify.ps1` -> `tmp/_tmp_u2_tree_verify.txt`, ledger total 597 over 53 TUs):

- S1 10: `printing` 1, `security` 2, `devicemanagement` 2, `brush` 1, `bitmap` 1, `resource` 2, `console` 1
- S2 18: `filledshapes` 3, `buttoncontrol` 3, `synchronization` 2, `combobox` 3, `timer` 2, `windowprocedure` 2, `coordtransform` 3
- S3 27: `legacy` 4, `mdi` 4, `listbox` 3, `powermanagement` 4, `errorhandling` 4, `accessibility` 4, `hook` 4
- S4 33: `monitor` 5, `windowproperties` 5, `processthread` 5, `systeminfo` 4, `ime` 5, `help` 5, `text` 4
- S5 36: `displayconfig` 7, `systemshutdown` 6, `caret` 8, `rawinput` 7, `devicecontext` 8

Already-decided sites inside this region that these batches must **not** re-edit: `errorhandling.cpp:85` (`MessageBeep`), `processthread.cpp:87`/`:97`/`:115` (`GetGuiResources` + 2), `systemshutdown.cpp:134` (D51's `ShutdownBlockReasonDestroy`) - 5 of the 124.

## Decisions

The landing contract every leaf obeys - children point here, they do not restate it.

- **Token**: land `CheckReturn< sdkRetType, Check >::TType` at exactly the `checked` rows; the class base is the row's `sdkRetType`, never the published spelling (`LRESULT`/`LPARAM`/`HWND`/`HDWP`/`AW` rows). `unchecked` rows get the written reason in the site comment and no token; `void` rows get neither.
- **Quote**: the site comment carries the row's verbatim `docQuote` and the class - the table is the authority, not the page re-read at edit time.
- **Include**: one `../returncheck.h` include per TU that gains its first check, never a second.
- **Do not re-edit the 5 seeds**: `errorhandling.cpp:85`, `processthread.cpp:87`/`:97`/`:115`, `systemshutdown.cpp:134` - they are inside this region's row counts and are already landed.
- **Stop condition per leaf**: re-read the edited sites, then run the per-TU re-census for that TU - `qor_pp_fcontext; == qor_pp_useswinapi* == Library::*Call< == Win32ErrorHandler _; == n`, with `CheckReturn<` code sites counted separately against that TU's `checked` rows in the table. Tick the `registers/u2.md` row in the same session (the U17 lesson), never at the close.
- **Size bound**: one leaf = one TU = at most **8** sites here, so a leaf is one edit pass plus one re-census. A batch (10-36 sites over 7 TUs) is a `Refined` item, not a step.

## Small TU batch S1

10 sites over `printing`, `security`, `devicemanagement`, `brush`, `bitmap`, `resource`, `console`. Read those rows from `tmp/_tmp_u2_endpoint_table.csv`, land only the `checked` verdicts, write the site comment + class for each, add one `../returncheck.h` include per TU that gains its first check, then re-census those 7 TUs and tick the `registers/u2.md` row in the same session. Child plan: `./SmallTUBatchS1/plan.md` (Level 1 -> 2, 7 `Atomic` items, one per TU, terminal).

## Small TU batch S2

18 sites over `filledshapes`, `buttoncontrol`, `synchronization`, `combobox`, `timer`, `windowprocedure`, `coordtransform`. Same procedure. Note: `windowprocedure`'s `DefWindowProcT`/`GetWindowProc`/`SetWindowProc` family and the `Hook` callbacks take `LPARAM`/`LRESULT` - the `LRESULT` rows (13 library-wide) are the ones whose published spelling differs from the SDK type, so the class base must be the row's `sdkRetType`. Child plan: `./SmallTUBatchS2/plan.md` (Level 1 -> 2, 7 `Atomic` items, one per TU, terminal).

## Small TU batch S3

27 sites over `legacy`, `mdi`, `listbox`, `powermanagement`, `errorhandling`, `accessibility`, `hook`. `errorhandling.cpp:85` is already checked - skip it; its include already exists. `accessibility`/`hook` carry the `HWINEVENTHOOK`/`HHOOK` handle rows where NULL is a documented failure but also a legal answer for the setter APIs - those end `unchecked` with the reason. Child plan: `./SmallTUBatchS3/plan.md` (Level 1 -> 2, 7 `Atomic` items, one per TU, terminal).

## Small TU batch S4

33 sites over `monitor`, `windowproperties`, `processthread`, `systeminfo`, `ime`, `help`, `text`. `processthread.cpp` already carries 3 checks and the include - only its remaining rows are in play; the `GetProcessList`/`GetGuiResources` numeric-count rows are the `DWORD`/`BOOL` precedent for `CheckNonZero< DWORD >`. Child plan: `./SmallTUBatchS4/plan.md` (Level 1 -> 2, 7 `Atomic` items, one per TU, terminal).

## Small TU batch S5

36 sites over `displayconfig`, `systemshutdown`, `caret`, `rawinput`, `devicecontext`. `systemshutdown.cpp:134` is already checked and its include exists - the remaining 5 sites of that TU decide against their rows (`LockWorkStation` and `ShutdownBlockReasonCreate`/`Query` are the ones whose FALSE is a documented failure and whose BOOL carries no answer meaning, so they are the expected `checked` rows here). Child plan: `./SmallTUBatchS5/plan.md` (Level 1 -> 2, 5 `Atomic` items, one per TU, terminal).
