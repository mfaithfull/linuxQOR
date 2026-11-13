---
Type: Plan
Title: "painting sites"
Parent: "../plan.md"
Level: 3
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

# painting sites

Slug: `PaintingSites` - from title "painting sites" (no drop-words, PascalCased, 13 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **25** `painting.cpp` sites in 3 slices of at most 10 sites.

## Resume

Cursor: **terminal** - all 3 items `Atomic`, all 3 closed 12 Nov 2026, and `painting.cpp` re-censuses `rows=25 fcontext=25 useswinapi=25 (23 bare + 2 AW) LibraryCall=25 Win32ErrorHandler=25 CheckReturn-code=15 table-checked=15 table-unchecked=10 returncheck-includes=1 ok` (15 tokens = the TU's 15 `checked` rows, 25 policy blocks = 15 checked + 10 unchecked, standalone compile `TU_EXIT=0`). Item 3 **painting slice 3** is **CLOSED 12 Nov 2026**: all 5 of its sites are in an end state - 4 carry the token (`SetWindowRgn` over `CheckNonZero< int >`, `UpdateWindow` / `ValidateRect` / `ValidateRgn` over `BoolCheck`) and `WindowFromDC` carries the written reason. Item 2 **painting slice 2** is **CLOSED 12 Nov 2026**: all 10 of its sites are in an end state - 4 carry the `CheckReturn< >` token (`GetWindowDC` over `CheckNonZeroHandle< HDC >`, `InvalidateRect` / `PaintDesktop` / `RedrawWindow` over `BoolCheck`) each with its policy comment, and 6 carry a written reason (`GetUpdateRgn`, `GetWindowRgn`, `GetWindowRgnBox`, `GrayString`, `InvalidateRgn`, `LockWindowUpdate`); the per-TU re-census reads `painting.cpp rows=25 fcontext=25 useswinapi=25 (23 bare + 2 AW) LibraryCall=25 Win32ErrorHandler=25 CheckReturn-code=11 table-checked=15 table-unchecked=10 returncheck-includes=1` (unchanged except `CheckReturn-code` 7 -> 11 = slice 1's 7 + slice 2's 4; `check-FAIL` is the 4 `checked` rows slice 3 owns) and the single-TU compile gate `tmp/_tmp_u2_one_tu.bat ...painting.cpp` `TU_EXIT=0` with a 1-line log. Item 1 **painting slice 1** is **CLOSED 12 Nov 2026**: all 10 of its sites are in an end state - 7 carry the `CheckReturn< >` token (`BeginPaint` over `CheckNonZeroHandle< HDC >`, and `DrawAnimatedRects` / `DrawCaption` / `DrawEdge` / `DrawFocusRect` / `DrawFrameControl` / `DrawState` over `BoolCheck`), 3 carry a written reason (`EndPaint`, `ExcludeUpdateRgn`, `GetUpdateRect`), the TU's one `#include "../returncheck.h"` is in place, and the per-TU re-census `tmp/_tmp_u2_code_census.txt` reads `painting.cpp rows=25 fcontext=25 useswinapi=25 (23 bare + 2 AW) LibraryCall=25 Win32ErrorHandler=25 CheckReturn-code=7 table-checked=15 table-unchecked=10 returncheck-includes=1` (code=7 = exactly slice 1's 7 `checked` rows; the `check-FAIL` verdict is the TU's remaining 8 `checked` rows, which slices 2-3 own), and the single-TU compile gate `tmp/_tmp_u2_one_tu.bat ...painting.cpp` `TU_EXIT=0` with a 1-line log. Terminal: all 3 items `Atomic`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, caller audit for nonzero counts, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: desktop sites, clipboard sites, dpi sites, dialog sites, misc sites, message sites. 17 of 25 rows have nonzero in-tree callers. `ValidateRect`/`ValidateRgn` answer the previous state - expected `unchecked` with the reason.

## painting slice 1

10 sites in `painting.cpp`: `BeginPaint`, `DrawAnimatedRects`, `DrawCaption`, `DrawEdge`, `DrawFocusRect`, `DrawFrameControl`, `DrawState`, `EndPaint`, `ExcludeUpdateRgn`, `GetUpdateRect`. Land only the `checked` rows. Stop condition: re-read the sites, then re-census `painting.cpp` against its `checked` row count.

## painting slice 2

10 sites in `painting.cpp`: `GetUpdateRgn`, `GetWindowDC`, `GetWindowRgn`, `GetWindowRgnBox`, `GrayString`, `InvalidateRect`, `InvalidateRgn`, `LockWindowUpdate`, `PaintDesktop`, `RedrawWindow`. Stop condition: re-read the sites, then re-census `painting.cpp` - its counts must already match the slice-1 census.

## painting slice 3

5 sites in `painting.cpp`: `SetWindowRgn`, `UpdateWindow`, `ValidateRect`, `ValidateRgn`, `WindowFromDC`. Stop condition: re-read the sites, then re-census `painting.cpp` - its counts must already match the slice-2 census.

## Cursors

painting slice 1: {"tu":"painting.cpp","slice_start":0,"slice_len":10,"done":true}
painting slice 2: {"tu":"painting.cpp","slice_start":10,"slice_len":10,"done":true}
painting slice 3: {"tu":"painting.cpp","slice_start":20,"slice_len":5,"done":true}

## Closed 12 Nov 2026 - evidence

- Slice 3 landed the last 4 tokens (`SetWindowRgn`, `UpdateWindow`, `ValidateRect`, `ValidateRgn`) and wrote `WindowFromDC`'s reason; the census after the slice reads `CheckReturn-code=15 table-checked=15 table-unchecked=10 returncheck-includes=1 ok` - 7+4+4 = 15, exactly the TU's `checked` row count, so the TU's stop condition is met and the mid-batch `check-FAIL` tag is gone.
- `SetWindowRgn` is the TU's only non-`BoolCheck` `checked` row besides the two `CheckNonZeroHandle< HDC >` rows: its `sdkRetType` and `pubRetType` are both `int`, so the base is `int` and the class is `CheckNonZero< int >` (returncheck.h:187) - `BoolCheck` would key at the wrong type.
- Deviation from this leaf's `## Context`, which predicted `ValidateRect`/`ValidateRgn` as `unchecked` ("answer the previous state"): their pages state status only ("If the function succeeds, the return value is nonzero. If the function fails, the return value is zero." as the whole Return value section), so both are `checked`; the row that actually answers a question here is `GetUpdateRect`, which is `unchecked`.
- All 25 rows of the TU now carry a written block: 15 `the row reads checked` and 10 `the row reads unchecked` (`WindowFromDC` being the slice's `unchecked` row - a device context that belongs to no window answers NULL legitimately, so `CheckNonZeroHandle< HWND >` would report a legal lookup as a failure).
- All 4 of slice 3's `checked` rows have exactly 1 in-tree caller each, all in `windowcontroller.cpp` (`SetWindowRgn` at :594, `UpdateWindow` :599, `ValidateRect` :604, `ValidateRgn` :609); each reads the result as a status through `? true : false` except `SetWindowRgn`, whose int is returned through to its own callers - every audit is recorded in that site's policy comment.
- Standalone compile after the slice: `tmp/_tmp_u2_one_tu.bat ...painting.cpp` -> `TU_EXIT=0`, log = `painting.cpp` only, zero diagnostics at `/W4`.

- Slice 1 landed 7 tokens (`BeginPaint`, `DrawAnimatedRects`, `DrawCaption`, `DrawEdge`, `DrawFocusRect`, `DrawFrameControl`, `DrawState`); census after the slice read `CheckReturn-code=7 table-checked=15 ... check-FAIL`, the expected mid-batch tag.
- Slice 2 landed 4 tokens (`GetWindowDC`, `InvalidateRect`, `PaintDesktop`, `RedrawWindow`); census after the slice reads `CheckReturn-code=11 table-checked=15 table-unchecked=10 returncheck-includes=1 check-FAIL` - 7+4 = 11, so the 4 `checked` rows slice 3 owns (`SetWindowRgn`, `UpdateWindow`, `ValidateRect`, `ValidateRgn`) are the only gap left.
- All 10 of slice 2's sites are in an end state: the 4 `checked` rows carry the token plus a policy comment, and the 6 `unchecked` rows (`GetUpdateRgn`, `GetWindowRgn`, `GetWindowRgnBox`, `GrayString`, `InvalidateRgn`, `LockWindowUpdate`) carry the contract's written reason and no token - so 13 of the TU's 17 written blocks are in place, the other 4 (`EndPaint`, `ExcludeUpdateRgn`, `GetUpdateRect` from slice 1) already there.
- Class bases follow the table's `sdkRetType`: `GetWindowDC` takes `CheckNonZeroHandle< HDC >` (the same class as `BeginPaint`), and `InvalidateRect` / `PaintDesktop` / `RedrawWindow` take `BoolCheck` (returncheck.h:94). No new check class was needed in this TU.
- `RedrawWindow` was the last site of the slice to land: the census said 10 where 11 were expected, and the gap was traced to that site before its token was written. Its audit resolved to `windowcontroller.cpp:589` (`WindowController::Redraw`), which reads the result as a status through `? true : false` and never treats a zero as a legal answer - the audit is recorded in the site's policy comment.
- `GetUpdateRgn`, `GetWindowRgn`, `GetWindowRgnBox`, `LockWindowUpdate` and `InvalidateRgn` are the nonzero-caller `unchecked` rows (1 each); their written reasons sit at the sites, and `GetUpdateRgn` / `GetWindowDC` / `GetWindowRgn` / `GetWindowRgnBox` / `InvalidateRect` / `InvalidateRgn` / `LockWindowUpdate` / `RedrawWindow` all resolve to `windowcontroller.cpp` (lines 547-599) in `tmp/_tmp_u2_paint2_callers.txt`.
- Standalone compile after the slice: `tmp/_tmp_u2_one_tu.bat ...painting.cpp` -> `TU_EXIT=0`, log = `painting.cpp` only, zero diagnostics at `/W4`. Note the run needs `tmp/obj/` to exist - a missing obj dir surfaces as `fatal error C1083`, not as a source error.

