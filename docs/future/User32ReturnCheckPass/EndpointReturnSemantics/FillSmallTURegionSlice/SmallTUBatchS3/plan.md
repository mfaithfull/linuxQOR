---
Type: Plan
Title: "Small TU batch S3"
Parent: "../plan.md"
Level: 3
Items:
  - Title: listbox rows
    Status: Atomic
    Content:
  - Title: accessibility rows
    Status: Atomic
    Content:
  - Title: errorhandling rows
    Status: Atomic
    Content:
  - Title: hook rows
    Status: Atomic
    Content:
  - Title: legacy rows
    Status: Atomic
    Content:
  - Title: mdi rows
    Status: Atomic
    Content:
  - Title: powermanagement rows
    Status: Atomic
    Content:
---

# Small TU batch S3

Slug: `SmallTUBatchS3` - from title "Small TU batch S3" (no drop-words, PascalCased, 14 chars, ASCII); the sibling U1 tree's name. Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **27** table rows of these 7 small TUs, one item per TU, each item one fetch batch.

## Resume

Cursor: **all 7 items done** - this plan reached its terminal state on 12 Nov 2026. Measured at that write: all **27** rows carry the verdict triple (`tmp/_tmp_u2_leaf_verify.txt`: the 7 TUs read `slice_rows=n table_rows=n filled=n ok`, `tus=7 slice_total=27 table_total=27 filled_total=27 fail=0`, `class-base violations=0`), verdicts `checked 12 / unchecked 13 / void 2` (`tmp/_tmp_u2_batch_counts.txt` S3 TOTAL), every quote verbatim from `tmp/_tmp_u2_probe_cache.txt` and appended to `tmp/_tmp_u2_docquotes.txt`. Next in the parent: item 4 **Small TU batch S4**.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the grandparent's (`../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: Small TU batch S1, Small TU batch S2, Small TU batch S4, Small TU batch S5.

Rows and stems per item are listed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM <tu> slice 1`.

Measured correction to the hook-handle caution this batch wrote: at `SetWinEventHook` the page reserves zero ("If unsuccessful, returns zero") and no legal success answer reaches it, so the session-27 test passes and the row takes `CheckReturn< HWINEVENTHOOK, CheckNonZeroHandle< HWINEVENTHOOK > >`. The NULL-is-a-legal-value wording belongs to `UnhookWinEvent`'s **parameter** (`"The hWinEventHook parameter is NULL or not valid"` is a documented failure of that stem), not to the setter's return, so it bars no check here.

## listbox rows

3 rows: `DlgDirList`, `DlgDirSelectEx`, `GetListBoxInfo`. One fetch batch, then write.

## accessibility rows

4 rows: `SetWinEventHook`, `UnhookWinEvent`, `IsWinEventHookInstalled`, `NotifyWinEvent`. One fetch batch, then write. The hook-handle rows here are where NULL is a documented failure **and** a legal setter answer - those end `unchecked` with the page's wording.

## errorhandling rows

4 rows: `FlashWindow`, `FlashWindowEx`, `MessageBeep`, `SetLastErrorEx`. `MessageBeep` is decided in source (`errorhandling.cpp:85`, `CheckReturn< BOOL, BoolCheck >`) - fill its row from the source precedent and D51, no fetch needed; the other 3 take one fetch batch; then write the triple for all 4 rows.

## hook rows

4 rows: `CallMsgFilter`, `CallNextHookEx`, `SetWindowsHookEx`, `UnhookWindowsHookEx`. One fetch batch, then write.

## legacy rows

4 rows: `DragObject`, `SetMessageQueue`, `GetWindowWord`, `SetWindowWord`. One fetch batch, then write.

## mdi rows

4 rows: `CreateMDIWindow`, `DefFrameProc`, `DefMDIChildProc`, `TranslateMDISysAccel`. One fetch batch, then write.

## powermanagement rows

4 rows: `RegisterPowerSettingNotification`, `UnregisterPowerSettingNotification`, `RegisterSuspendResumeNotification`, `UnregisterSuspendResumeNotification`. One fetch batch, then write.
