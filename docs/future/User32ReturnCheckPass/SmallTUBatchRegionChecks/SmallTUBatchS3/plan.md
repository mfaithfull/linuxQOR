---
Type: Plan
Title: "Small TU batch S3"
Parent: "../plan.md"
Level: 2
Items:
  - Title: legacy sites
    Status: Atomic
    Content:
  - Title: mdi sites
    Status: Atomic
    Content:
  - Title: listbox sites
    Status: Atomic
    Content:
  - Title: powermanagement sites
    Status: Atomic
    Content:
  - Title: errorhandling sites
    Status: Atomic
    Content:
  - Title: accessibility sites
    Status: Atomic
    Content:
  - Title: hook sites
    Status: Atomic
    Content:
---

# Small TU batch S3

Slug: `SmallTUBatchS3` - from title "Small TU batch S3" (no drop-words, PascalCased, 14 chars, ASCII); the same folder name the verdict-side tree uses, so the code batch and the fetch batch cover identical row sets. Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **27 sites** of these 7 small TUs, one item per TU, each item closing with that TU's re-census.

## Resume

Cursor: **CLOSED 12 Nov 2026** - all 7 items `Atomic` and landed. 27 sites decided from the table: **11 tokens landed** (`GetWindowWord` `CheckNonZero< WORD >`, `CreateMDIWindow` `CheckNonZeroHandle< HWND >`, `DlgDirList` `CheckNonZero< int >`, the 2 `Register*Notification` rows `CheckNonZeroHandle< HPOWERNOTIFY >` and the 2 `Unregister*` rows `BoolCheck`, `SetWinEventHook` `CheckNonZeroHandle< HWINEVENTHOOK >` + `UnhookWinEvent` `BoolCheck`, `SetWindowsHookEx` `CheckNonZeroHandle< HHOOK >` + `UnhookWindowsHookEx` `BoolCheck`) - the batch's 12th `checked` row is the untouched `errorhandling.cpp:85` (`MessageBeep`) seed - plus **13 written-reason `unchecked`** and **2 `void`** (`NotifyWinEvent` `accessibility.cpp:51`, `SetLastErrorEx` `errorhandling.cpp:94`). Census `tmp/_tmp_u2_code_census.txt` `tus=7 fail=0` (one `../returncheck.h` include per TU, `CheckReturn-code` == that TU's `checked` rows), standalone `/W4` leg `tmp/_tmp_u2_gate_tus.out.txt` `tus=7 fail=0` at exit 0 / 0 diagnostics / objs newer, `registers/u2.md` experiment line + S3 entry ticked in the same session. Terminal: all 7 items `Atomic`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the parent's (`../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, verbatim quote in the site comment, one include per TU, the 5 seeds never re-edited, the per-TU re-census stop condition. Sibling titles in the parent: Small TU batch S1, Small TU batch S2, Small TU batch S4, Small TU batch S5.

Row sets per TU are the same rows the verdict-side `SmallTUBatchS3` filled (`tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt`, `ITEM <tu> slice 1`). One seed sits in this batch: `errorhandling.cpp:85` - it is counted in the TU's rows but must not be re-edited.

## legacy sites

4 sites: `DragObject`, `SetMessageQueue`, `GetWindowWord`, `SetWindowWord` (`legacy.cpp`). Same procedure, then re-census that TU.

## mdi sites

4 sites: `CreateMDIWindow`, `DefFrameProc`, `DefMDIChildProc`, `TranslateMDISysAccel` (`mdi.cpp`). Same procedure, then re-census that TU.

## listbox sites

3 sites: `DlgDirList`, `DlgDirSelectEx`, `GetListBoxInfo` (`listbox.cpp`). Same procedure, then re-census that TU.

## powermanagement sites

4 sites: `RegisterPowerSettingNotification`, `UnregisterPowerSettingNotification`, `RegisterSuspendResumeNotification`, `UnregisterSuspendResumeNotification` (`powermanagement.cpp`). The register rows' documented failure is FALSE with no answer meaning - the expected `checked` set here. Re-census after.

## errorhandling sites

4 rows, **3 in play**: `FlashWindow`, `FlashWindowEx`, `SetLastErrorEx`, plus the already-landed `MessageBeep` at `errorhandling.cpp:85` (skip it, its include already exists). Re-census `errorhandling.cpp` after - its `CheckReturn<` count must already include the seed.

## accessibility sites

4 sites: `SetWinEventHook`, `UnhookWinEvent`, `IsWinEventHookInstalled`, `NotifyWinEvent` (`accessibility.cpp`). The `HWINEVENTHOOK` rows answer NULL both as failure and as a legal setter value - those stay `unchecked` with the reason. Re-census after.

## hook sites

4 sites: `CallMsgFilter`, `CallNextHookEx`, `SetWindowsHookEx`, `UnhookWindowsHookEx` (`hook.cpp`). `HHOOK` rows - class base is the row's `sdkRetType`. Same procedure, then re-census that TU.
