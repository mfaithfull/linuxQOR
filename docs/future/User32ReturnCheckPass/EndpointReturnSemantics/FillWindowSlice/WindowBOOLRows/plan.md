---
Type: Plan
Title: "window BOOL rows"
Parent: "../plan.md"
Level: 3
Items:
  - Title: window BOOL slice 1
    Status: Atomic
    Content:
  - Title: window BOOL slice 2
    Status: Atomic
    Content:
  - Title: window BOOL slice 3
    Status: Atomic
    Content:
  - Title: window BOOL slice 4
    Status: Atomic
    Content:
  - Title: window BOOL slice 5
    Status: Atomic
    Content:
  - Title: window BOOL slice 6
    Status: Atomic
    Content:
---

# window BOOL rows

Slug: `WindowBOOLRows` - from title "window BOOL rows" (no drop-words, PascalCased, 14 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **55** `BOOL` rows of `window.cpp`, in 6 slices of at most 10 URLs, so each item is one fetch batch and one write pass.

## Resume

Cursor: **terminal** - all 6 items `Atomic`, all 6 closed 12 Nov 2026. All **55** `BOOL` rows of `window.cpp` carry the verdict triple: `checked 37 / unchecked 18 / void 0`, measured off the table (`window.cpp` + `pubRetType BOOL` → `rows=55 checked=37 unchecked=18 void=0 blank=0`); the folder single-pass audit `tmp/_tmp_u2_folder_audit.ps1` `rows=597 filled=561 checked=335 unchecked=217 void=8 class-shape violations=0` with `window.cpp blank=31` (the HWND and other leaves, still open).

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the parent of the parent (`../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: window HWND rows, window other rows.

Slice boundaries are fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM window-BOOL`: 55 rows in slices of 10/10/10/10/10/5 in table order. Expect most rows here to end `unchecked` with the page's wording - the answer-shaped `BOOL` families (`IsWindow`, `IsChild`, `IsIconic`, `ShowOwnedPopups`, `AnimateWindow`) answer FALSE legitimately.

## window BOOL slice 1

Rows 1-10: `AdjustWindowRect`, `AdjustWindowRectEx`, `AdjustWindowRectExForDpi`, `AllowSetForegroundWindow`, `AnimateWindow`, `AnyPopup`, `BringWindowToTop`, `ChangeWindowMessageFilter`, `CloseWindow`, `DeregisterShellHookWindow`. One fetch batch of 10, then write those 10 rows.

## window BOOL slice 2

Rows 11-20: `DestroyWindow`, `EndDeferWindowPos`, `EndTask`, `EnumChildWindows`, `EnumThreadWindows`, `EnumWindows`, `GetAltTabInfo`, `GetClientRect`, `GetGUIThreadInfo`, `GetLayeredWindowAttributes`. One fetch batch of 10, then write.

## window BOOL slice 3

Rows 21-30: `GetProcessDefaultLayout`, `GetTitleBarInfo`, `GetWindowInfo`, `GetWindowPlacement`, `GetWindowRect`, `IsChild`, `IsGUIThread`, `IsHungAppWindow`, `IsIconic`, `IsProcessDPIAware`. One fetch batch of 10, then write.

## window BOOL slice 4

Rows 31-40: `IsWindow`, `IsWindowUnicode`, `IsWindowVisible`, `IsZoomed`, `LockSetForegroundWindow`, `LogicalToPhysicalPoint`, `LogicalToPhysicalPointForPerMonitorDPI`, `MoveWindow`, `OpenIcon`, `PhysicalToLogicalPoint`. One fetch batch of 10, then write.

## window BOOL slice 5

Rows 41-50: `PhysicalToLogicalPointForPerMonitorDPI`, `RegisterShellHookWindow`, `SetForegroundWindow`, `SetLayeredWindowAttributes`, `SetProcessDefaultLayout`, `SetProcessDPIAware`, `SetWindowPlacement`, `SetWindowPos`, `SetWindowText`, `ShowOwnedPopups`. One fetch batch of 10, then write.

## window BOOL slice 6

Rows 51-55: `ShowWindow`, `ShowWindowAsync`, `SoundSentry`, `UpdateLayeredWindow`, `UpdateLayeredWindowIndirect`. One fetch batch of 5, then write. `ShowWindow` is a previous-state return - expected `unchecked` with the page's wording.

## Cursors

window BOOL slice 1: {"tu":"window.cpp","pubType":"BOOL","slice_start":0,"slice_len":10,"done":true}
window BOOL slice 2: {"tu":"window.cpp","pubType":"BOOL","slice_start":10,"slice_len":10,"done":true}
window BOOL slice 3: {"tu":"window.cpp","pubType":"BOOL","slice_start":20,"slice_len":10,"done":true}
window BOOL slice 4: {"tu":"window.cpp","pubType":"BOOL","slice_start":30,"slice_len":10,"done":true}
window BOOL slice 5: {"tu":"window.cpp","pubType":"BOOL","slice_start":40,"slice_len":10,"done":true}
window BOOL slice 6: {"tu":"window.cpp","pubType":"BOOL","slice_start":50,"slice_len":5,"done":true}

## Closed 12 Nov 2026 - evidence

- Six fetch batches, all `bad=0 VERDICT fail=0`: `tmp/_tmp_u2_wb_bool_fetch.out.txt` (10), `_s2` (10), `_s3` (10), `_s4` (10), `_s5` (10), `_s6` (4 fetched + 1 `NO-PAGE`). New instrument `tmp/_tmp_u2_wb_fetch.ps1`, cloned from the strings one with `want=window.cpp` plus a **`pubType` third argument** (`BOOL` / `HWND` / the other families) - the 86 `window.cpp` rows interleave by type in table order, so a bare `skip`/`take` walk does not reproduce the `ITEM window-BOOL` slice boundaries in `tmp/_tmp_u2_slice_stems.txt`; the reports carry the type in their name for the same reason.
- 55 verdict lines appended to `tmp/_tmp_u2_decisions.txt` and written by `tmp/_tmp_u2_patch.ps1` (`decisions with no table row = 0`, `class-shape violations = 0`).
- Every one of the 37 `checked` rows takes `BoolCheck` (`returncheck.h:94`) - all 37 have `sdkRetType BOOL`, so no row here needed a class the vocabulary does not already have.
- The 18 `unchecked` rows split by kind: the question/answer BOOLs `AnyPopup`, `IsChild`, `IsGUIThread`, `IsHungAppWindow`, `IsIconic`, `IsProcessDPIAware`, `IsWindow`, `IsWindowUnicode`, `IsWindowVisible`, `IsZoomed`, `ShowWindow`, `SetForegroundWindow`; the callback-shaped enumerations `EnumChildWindows` ("The return value is not used."), `EnumThreadWindows`, `EnumWindows`; and the pages that document no return at all - `LogicalToPhysicalPoint`, `PhysicalToLogicalPoint` (Return value section literally "None" against a `BOOL` row) and `UpdateLayeredWindowIndirect` (probe `no-page`, tag `no-return-value-section`, docQuote stays `-`).
- Deviations from this leaf's `## Context`, which named `IsWindow`, `IsChild`, `IsIconic`, `ShowOwnedPopups`, `AnimateWindow` as the rejections: measured, `IsWindow`/`IsChild`/`IsIconic` are unchecked as predicted and `ShowWindow` joins them on the previous-state wording ("If the window was previously visible, the return value is nonzero. If the window was previously hidden, the return value is zero."), but `AnimateWindow` and `ShowOwnedPopups` are **checked** - both pages carry the status boilerplate with a `GetLastError` route, and `AnimateWindow` additionally enumerates its failure situations ("If the window is already visible and you are trying to show the window ... If the thread does not own the window. Note that, in this case, AnimateWindow fails but GetLastError returns ERROR_SUCCESS"), which is a documented failure rather than a legal answer. The `ERROR_SUCCESS` caveat is recorded for the code-side leaf since it means a landed check cannot fall back on the error code there.
- `SetForegroundWindow` is unchecked despite being a `Set*` call: its page words the return as the window's state ("If the window was brought to the foreground ... If the window was not brought to the foreground, the return value is zero") and offers no `GetLastError` route at all, unlike every status row here - the pair with the checked `LockSetForegroundWindow` row (status wording) shows the arbitration.
- `EnumWindows` is unchecked on the conflation reading, not the whole-return-is-the-answer reading: "If EnumWindowsProc returns zero, the return value is also zero. In this case, the callback function should call SetLastError to obtain a meaningful error code" - the callback's stop-enumeration zero is a legal answer indistinguishable from the failure zero, the same shape as the `DrawText` and `GetTabbedTextExtent` rows.
- Two `CORRECTED` legs of the slice contract fired: `GetAltTabInfo` (`nf-winuser-getalttabinfoa`) and `SetWindowText` (`nf-winuser-setwindowtexta`), both `useswinapiAW` rows with `sdkRetType NO-PROTOTYPE`; their class base is the published `BOOL`.
- Caller counts (from the table's `inTreeCallers` column, echoed in each row's `notes`): 33 of the 55 rows have nonzero callers, `SetWindowPos` the densest at 5, so the audit-before-landing marker is owed on those rows by the code-side leaf; the 22 zero-caller rows are booked change-safe.
- No code change is proposed here; only the verdict triple.

