---
Type: Plan
Title: "painting rows"
Parent: "../plan.md"
Level: 4
Items:
  - Title: painting slice 1
    Status: Atomic
    Content:
  - Title: painting slice 2
    Status: Atomic
    Content:
  - Title: painting slice 3
    Status: Atomic
    Content:
---

# painting rows

Slug: `PaintingRows` - from title "painting rows" (no drop-words, PascalCased, 12 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **25** `painting.cpp` rows in 3 slices of at most 10 URLs.

## Resume

Cursor: **terminal** - all 3 items `Atomic`, all 3 closed 12 Nov 2026. All **25** rows of `painting.cpp` carry the verdict triple: `checked 15 / unchecked 10 / void 0`, `tmp/_tmp_u2_tu_state.ps1 painting.cpp` `rows=25 checked=15 unchecked=10 void=0 blank=0 shape_bad=0`; the folder single-pass audit `tmp/_tmp_u2_folder_audit.ps1` `rows=597 filled=367 checked=220 unchecked=141 void=6 sum=367 class-shape violations=0 filled rows with no docQuote=0`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: desktop rows, clipboard rows, dpi rows, dialog rows, misc rows, message rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM painting`: 25 rows in slices of 10/10/5 in table order. 8 of 25 rows have 0 in-tree callers - the other 17 need the caller count in `notes` before a check lands. `ValidateRect`/`ValidateRgn`/`InvalidateRect`-shaped rows answer the previous state - expected `unchecked` with the page's wording.

## painting slice 1

Rows 1-10: `BeginPaint`, `DrawAnimatedRects`, `DrawCaption`, `DrawEdge`, `DrawFocusRect`, `DrawFrameControl`, `DrawState`, `EndPaint`, `ExcludeUpdateRgn`, `GetUpdateRect`. One fetch batch of 10, then write those 10 rows.

## painting slice 2

Rows 11-20: `GetUpdateRgn`, `GetWindowDC`, `GetWindowRgn`, `GetWindowRgnBox`, `GrayString`, `InvalidateRect`, `InvalidateRgn`, `LockWindowUpdate`, `PaintDesktop`, `RedrawWindow`. One fetch batch of 10, then write those 10 rows.

## painting slice 3

Rows 21-25: `SetWindowRgn`, `UpdateWindow`, `ValidateRect`, `ValidateRgn`, `WindowFromDC`. One fetch batch of 5, then write those 5 rows.

## Cursors

painting slice 1: {"tu":"painting.cpp","slice_start":0,"slice_len":10,"done":true}
painting slice 2: {"tu":"painting.cpp","slice_start":10,"slice_len":10,"done":true}
painting slice 3: {"tu":"painting.cpp","slice_start":20,"slice_len":5,"done":true}

## Closed 12 Nov 2026 - evidence

- Slice 1 (rows 1-10) one batch: `tmp/_tmp_u2_paint_fetch.out.txt` - 10 `FETCH ... match-cache http=200` (1 `CORRECTED`), `bad=0 no-page=0`, `VERDICT fail=0`.
- Slice 2 (rows 11-20) one batch: `tmp/_tmp_u2_paint_fetch_s2.out.txt` - 10 `match-cache` at HTTP 200 (1 `CORRECTED`), `bad=0`, `VERDICT fail=0`.
- Slice 3 (rows 21-25) one batch: `tmp/_tmp_u2_paint_fetch_s3.out.txt` - 5 `match-cache` at HTTP 200, `bad=0`, `VERDICT fail=0`.
- Instrument `tmp/_tmp_u2_paint_fetch.ps1` carries the fixed slice-tag math `[Math]::Floor($skip / $take) + 1` (the misc leaf's `[int]` rounding mislabelled its third batch).
- 25 verdict lines appended to `tmp/_tmp_u2_decisions.txt` (lines 349-393) and written by `tmp/_tmp_u2_patch.ps1` (`decisions with no table row = 0`, `class-shape violations = 0`); 17 rows carry nonzero in-tree callers (1 each, `GetWindowDC` 2) with the count in `notes` as the audit-before-landing marker, 8 rows change-safe.
- Deviation from this leaf's `## Context`, which predicted the `ValidateRect`/`ValidateRgn`/`InvalidateRect` trio as unchecked "answering the previous state": measured, all three pages state status only ("If the function succeeds, the return value is nonzero. If the function fails, the return value is zero.") with no answer, so all three are **checked** `BoolCheck`. The rows that actually answer a question here are `GetUpdateRect` (zero = an empty update region) and the four region-complexity rows `ExcludeUpdateRgn`/`GetUpdateRgn`/`GetWindowRgn`/`GetWindowRgnBox` (int return that is the enumeration `NULLREGION`/`SIMPLEREGION`/`COMPLEXREGION`/`ERROR` the caller branches on, `ERROR` naming a window with no region set - a legal state), all **unchecked**.
- Two rows are unchecked on "the page reserves nothing": `EndPaint` and `InvalidateRgn`, both "The return value is always nonzero." - landing a check could never fire, so they stay unchecked rather than being given a dead check.
- `LockWindowUpdate` unchecked because its own page names a legal state behind the zero ("zero, indicating that an error occurred or another window was already locked"); `GrayString` unchecked because the zero folds in the caller's own output-function answer.
- `WindowFromDC` unchecked: "If no window is associated with the specified DC, the return value is NULL" - a printer DC or a context-less DC is a legal state, so `CheckNonZeroHandle< HWND >` would flag a legal lookup.
- Two `HDC` rows are checked `CheckNonZeroHandle< HDC >` (`BeginPaint`, `GetWindowDC`) - the class the HBITMAP/HCURSOR/HDESK/HWINSTA rows already take, not `HandleCheck` (keys on `INVALID_HANDLE_VALUE`, not NULL). `SetWindowRgn` is checked `CheckNonZero< int >` because the row's type is int while its page states BOOL-style status.
- No code change proposed; verdict triple only. `painting.cpp`'s existing live `CheckReturn` count is reconciled at the `PaintingSites` code-side leaf, not here.

