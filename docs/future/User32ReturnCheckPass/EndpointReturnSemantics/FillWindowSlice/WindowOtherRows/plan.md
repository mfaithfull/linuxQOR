---
Type: Plan
Title: "window other rows"
Parent: "../plan.md"
Level: 3
Items:
  - Title: window other slice 1
    Status: Atomic
    Content:
  - Title: window other slice 2
    Status: Atomic
    Content:
---

# window other rows

Slug: `WindowOtherRows` - from title "window other rows" (no drop-words, PascalCased, 15 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **14** `window.cpp` rows that are neither `BOOL` nor `HWND` (`UINT` 4, `int` 3, `WORD` 3, `HDWP` 2, `DWORD` 1, `void` 1), in 2 slices of at most 10 URLs.

## Resume

Cursor: **terminal** - both items `Atomic`, both closed 12 Nov 2026. All **14** rows of this leaf carry the verdict triple: `checked 8 / unchecked 5 / void 1`, and `window.cpp` is complete at `tmp/_tmp_u2_tu_state.ps1 window.cpp` `rows=86 checked=50 unchecked=35 void=1 blank=0 shape_bad=0`; the folder single-pass audit `tmp/_tmp_u2_folder_audit.ps1` reads `rows=597 filled=592 checked=349 unchecked=234 void=9 sum=592 class-shape violations=0 filled rows with no docQuote=0`, with the only remaining blanks (5) in `accelerator.cpp`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the parent of the parent (`../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: window BOOL rows, window HWND rows.

Slice boundaries fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM window-other`: 14 rows in slices of 10 and 4 in table order. The `void` row (`CascadeWindows`) needs no fetch - D51's rule settles it. `GetWindowText`/`GetWindowTextLength`-shaped counts answer 0 legitimately, so each row needs its page's exact wording before it decides.

## window other slice 1

Rows 1-10: `ArrangeIconicWindows`, `BeginDeferWindowPos`, `CascadeChildWindows`, `CascadeWindows`, `DeferWindowPos`, `GetWindowModuleFileName`, `GetWindowText`, `GetWindowTextLength`, `GetWindowThreadProcessId`, `InternalGetWindowText`. One fetch batch of 10 (9 fetches plus the `void` row settled without one), then write those 10 rows.

## window other slice 2

Rows 11-14: `RealGetWindowClass`, `SwitchToThisWindow`, `TileWindows`, `GetDpiForWindow`. One fetch batch of 4, then write.

## Cursors

window other slice 1: {"tu":"window.cpp","slice_start":0,"slice_len":10,"done":true}
window other slice 2: {"tu":"window.cpp","slice_start":10,"slice_len":4,"done":true}

## Closed 12 Nov 2026 - evidence

- Slice 1 (rows 1-10) in one fetch batch: `tmp/_tmp_u2_wo_fetch.out.txt` - 9 `FETCH ... match-cache http=200` plus `NO-PAGE CascadeChildWindows` (probe `no-page`, tag `no-return-value-section`, so the row is decided from the probe line and its docQuote stays `-`), `fetched=9 of 10 bad=0 no-page=0`, `VERDICT fail=0`.
- Slice 2 (rows 11-14) in one fetch batch: `tmp/_tmp_u2_wo_fetch_s3.out.txt` - 4 `match-cache` at HTTP 200, `fetched=4 of 4 bad=0 no-page=0`, `VERDICT fail=0`. The report file is named `_s3` because the instrument tags batches `[Math]::Floor($skip/$take)+1` and this leaf drives stems from `tmp/_tmp_u2_wo_stems.txt` (the 14 unfilled rows only) with `take=10`, so the second batch's tag comes from the skip it was given, not from its position in this leaf.
- 14 verdict lines in `tmp/_tmp_u2_decisions.txt`, written into the table by `tmp/_tmp_u2_patch.ps1`; all 14 rows land at 5 fields, `decisions with no table row = 0`, `class-shape violations = 0`.
- Class shapes this leaf adds to the TU: `CheckNonZeroHandle< HDWP >` for the `BeginDeferWindowPos`/`DeferWindowPos` pair (the deferred-window handle, which the caller hands straight to `EndDeferWindowPos`), `CheckNonZero< WORD >` for the `CascadeWindows`/`TileWindows` pair, `CheckNonZero< UINT >` for `ArrangeIconicWindows`/`RealGetWindowClass`/`GetDpiForWindow`, `CheckNonZero< DWORD >` for `GetWindowThreadProcessId`.
- Deviation from this leaf's `## Context`, which named `CascadeWindows` as the `void` row needing no fetch: measured, `CascadeWindows` is a `WORD` row checked on "If the function succeeds, the return value is the number of windows arranged. If the function fails, the return value is zero." and the leaf's `void` row is `SwitchToThisWindow` (`pubRetType void`, `sdkRetType VOID`, page Return value section the one word `None`). `CascadeChildWindows` is the row that needed no fetch instead - the probe has no Learn page for it at all.
- `GetDpiForWindow` is `checked` on the Remarks table, not the Return value line: the Return value line only says "An invalid hwnd value will result in a return value of 0", and the Remarks enumerate every legal `DPI_AWARENESS` case as nonzero (`DPI_AWARENESS_UNAWARE` answers the base 96 `USER_DEFAULT_SCREEN_DPI`, `DPI_AWARENESS_SYSTEM_AWARE` the system DPI, `DPI_AWARENESS_PER_MONITOR_AWARE` the monitor's DPI), so 0 is never a legal answer for a valid window and the row takes the same reading as the `GetWindowDpiAwarenessContext` row - deliberately unlike the `GetSystemMetricsForDpi` and `GetWindowTextLength` rows, whose zero is a legal metric/length and which the reviewed `dpi.cpp` banner states at :40.
- `GetWindowText`/`GetWindowTextLength` stay `unchecked` on the count reading the leaf predicted (0 is a legal length - a window with no title answers 0), and `GetWindowModuleFileName`/`InternalGetWindowText`/`CascadeChildWindows` on the no-reserved-value reading.
- 7 of 14 rows carry nonzero in-tree callers (1 each: `ArrangeIconicWindows`, `CascadeChildWindows`, `CascadeWindows`, `GetWindowThreadProcessId`, `SwitchToThisWindow`, `TileWindows`, `GetDpiForWindow`), each carrying its audit-before-landing marker in `notes`; the other 7 are change-safe at 0 callers.
- No code change is proposed in this leaf; only the verdict triple. `window.cpp` is now complete (86 rows) across the three `FillWindowSlice` leaves: BOOL 55, HWND 17, other 14.

