---
Type: Plan
Title: "Small TU batch region checks"
Parent: "../plan.md"
Level: 1
Items:
  - Title: Small TU batch S1
    Status: Atomic
    Content:
  - Title: Small TU batch S2
    Status: Atomic
    Content:
  - Title: Small TU batch S3
    Status: Atomic
    Content:
  - Title: Small TU batch S4
    Status: Atomic
    Content:
  - Title: Small TU batch S5
    Status: Atomic
    Content:
---

# Small TU batch region checks

Slug: `SmallTUBatchRegionChecks` - from title "Small TU batch region checks" (no drop-words, PascalCased, exactly 24 chars, at the cap). It mirrors the sibling tree's `BatchSmallTUs` boundaries: the same 33 TUs, the same S1-S5 order, the same site counts, so the two passes re-census against one another. Uniqueness is stored here, not recomputed.

## Goal

Land `CheckReturn< sdkRetType, Check >::TType` at exactly the `checked` verdicts of the **124 sites over the 33 smallest TUs** of `src/platform/os/windows/api_layer/user`, smallest-first, and record the `unchecked` verdicts as written reasons - so the small-TU half of U2 resolves against the table instead of against judgement.

## Resume

Cursor: item 1 **Small TU batch S1** (`Atomic`) - the plan's first action. **Hard precondition before any item here runs: the parent's item 2 (Endpoint return semantics) must have filled the verdict columns for these rows** - a site whose table row has no verdict gets no token, so this plan is a no-op until the table says otherwise.

## Context

Inherited from the parent (`../plan.md`): the rule is D51's decision process gated by the session-27 soundness test; every site ends **checked with a named class**, **unchecked with a written reason tied to its documented SDK failure value**, or `void`; G6 makes every landed check live on every call; the TU's site comment carries the doc quote and the class, and the TU banner records the verdict counts. Sibling titles: Site census tooling, Endpoint return semantics, Check class vocabulary, Large TU batch region checks, Folder gate and census, Records and commit.

Per-TU site counts on disk (measured by `tmp/_tmp_u2_tree_verify.ps1` -> `tmp/_tmp_u2_tree_verify.txt`, ledger total 597 over 53 TUs):

- S1 10: `printing` 1, `security` 2, `devicemanagement` 2, `brush` 1, `bitmap` 1, `resource` 2, `console` 1
- S2 18: `filledshapes` 3, `buttoncontrol` 3, `synchronization` 2, `combobox` 3, `timer` 2, `windowprocedure` 2, `coordtransform` 3
- S3 27: `legacy` 4, `mdi` 4, `listbox` 3, `powermanagement` 4, `errorhandling` 4, `accessibility` 4, `hook` 4
- S4 33: `monitor` 5, `windowproperties` 5, `processthread` 5, `systeminfo` 4, `ime` 5, `help` 5, `text` 4
- S5 36: `displayconfig` 7, `systemshutdown` 6, `caret` 8, `rawinput` 7, `devicecontext` 8

Already-decided sites inside this region that these batches must **not** re-edit: `errorhandling.cpp:85` (`MessageBeep`), `processthread.cpp:87`/`:97`/`:115` (`GetGuiResources` + 2), `systemshutdown.cpp:134` (D51's `ShutdownBlockReasonDestroy`) - 5 of the 124.

Every batch ends at its own per-TU re-census (the U17 lesson: tick the moment the batch's re-census verifies, never at the close), and the per-TU invariant stays `qor_pp_fcontext; == qor_pp_useswinapi* == Library::*Call< == Win32ErrorHandler _; == n` with `CheckReturn<` code sites counted separately against the table's `checked` rows for that TU.

## Small TU batch S1

10 sites over `printing`, `security`, `devicemanagement`, `brush`, `bitmap`, `resource`, `console`. Read those rows from `tmp/_tmp_u2_endpoint_table.csv`, land only the `checked` verdicts, write the site comment + class for each, add one `../returncheck.h` include per TU that gains its first check, then re-census those 7 TUs and tick the `registers/u2.md` row in the same session.

## Small TU batch S2

18 sites over `filledshapes`, `buttoncontrol`, `synchronization`, `combobox`, `timer`, `windowprocedure`, `coordtransform`. Same procedure. Note: `windowprocedure`'s `DefWindowProcT`/`GetWindowProc`/`SetWindowProc` family and the `Hook` callbacks take `LPARAM`/`LRESULT` - the `LRESULT` rows (13 library-wide) are the ones whose published spelling differs from the SDK type, so the class base must be the row's `sdkRetType`.

## Small TU batch S3

27 sites over `legacy`, `mdi`, `listbox`, `powermanagement`, `errorhandling`, `accessibility`, `hook`. `errorhandling.cpp:85` is already checked - skip it; its include already exists. `accessibility`/`hook` carry the `HWINEVENTHOOK`/`HHOOK` handle rows where NULL is a documented failure but also a legal answer for the setter APIs - those end `unchecked` with the reason.

## Small TU batch S4

33 sites over `monitor`, `windowproperties`, `processthread`, `systeminfo`, `ime`, `help`, `text`. `processthread.cpp` already carries 3 checks and the include - only its remaining rows are in play; the `GetProcessList`/`GetGuiResources` numeric-count rows are the `DWORD`/`BOOL` precedent for `CheckNonZero< DWORD >`.

## Small TU batch S5

36 sites over `displayconfig`, `systemshutdown`, `caret`, `rawinput`, `devicecontext`. `systemshutdown.cpp:134` is already checked and its include exists - the remaining 5 sites of that TU decide against their rows (`LockWorkStation` and `ShutdownBlockReasonCreate`/`Query` are the ones whose FALSE is a documented failure and whose BOOL carries no answer meaning, so they are the expected `checked` rows here).
