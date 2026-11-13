---
Type: Plan
Title: "touch sites"
Parent: "../plan.md"
Level: 3
Items:
  - Title: touch slice 1
    Status: Atomic
    Content:
  - Title: touch slice 2
    Status: Atomic
    Content:
---

# touch sites

Slug: `TouchSites` - from title "touch sites" (no drop-words, PascalCased, 10 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **14** `touch.cpp` sites in 2 slices of at most 10 sites.

## Resume

Cursor: **terminal** - both items `Atomic`, both closed 12 Nov 2026. All **12** `checked` of the **14** `touch.cpp` sites carry the token; the 2 `unchecked` sites (`IsTouchWindow`, `PackTouchHitTestingProximityEvaluation`) keep the bare `return Library::Call< ... >`. The per-TU re-census `tmp/_tmp_u2_code_census.ps1` (list `tmp/_tmp_u2_census_tus.txt` = `touch.cpp`) reads `touch.cpp rows=14 fcontext=14 useswinapi=14 (14 bare + 0 AW) LibraryCall=14 Win32ErrorHandler=14 CheckReturn-code=12 table-checked=12 table-unchecked=2 returncheck-includes=1 ok`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: mouse sites, rectangle sites, scrollbar sites, icon sites, windowclass sites, cursor sites.

Slice boundaries fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM touch`. All 14 rows here have 0 in-tree callers, so no caller audit gates this TU.

## touch slice 1

10 sites in `touch.cpp`: `CloseGestureInfoHandle`, `CloseTouchInputHandle`, `GetGestureConfig`, `GetGestureExtraArgs`, `GetGestureInfo`, `GetTouchInputInfo`, `InitializeTouchInjection`, `InjectTouchInput`, `IsTouchWindow`, `PackTouchHitTestingProximityEvaluation`. Land only the `checked` rows. Stop condition: re-read the sites, then re-census `touch.cpp` against its `checked` row count.

## touch slice 2

4 sites in `touch.cpp`: `RegisterTouchHitTestingWindow`, `RegisterTouchWindow`, `SetGestureConfig`, `UnregisterTouchWindow`. Stop condition: re-read the sites, then re-census `touch.cpp` - its counts must already match the slice-1 census.

## Closed 12 Nov 2026 - evidence

- Slice 1 (10 sites) landed 8 tokens in `touch.cpp`: `CloseGestureInfoHandle`, `CloseTouchInputHandle`, `GetGestureConfig`, `GetGestureExtraArgs`, `GetGestureInfo`, `GetTouchInputInfo`, `InitializeTouchInjection`, `InjectTouchInput` - each `CheckReturn< BOOL, BoolCheck >::TType bResult = Library::Call< ... >(pFunc, ...); return bResult;`. `IsTouchWindow` and `PackTouchHitTestingProximityEvaluation` kept the bare `return Library::Call< ... >` (the first is the yes/no question the call asks - FALSE is a legal answer, matching the misc leaf's reading of the same row; the second returns the `LRESULT` a `WM_TOUCHHITTESTING` handler hands back, so no check-shaped reading is valid).
- Slice 2 (4 sites) landed all 4 tokens: `RegisterTouchHitTestingWindow`, `RegisterTouchWindow`, `SetGestureConfig`, `UnregisterTouchWindow` - all `CheckReturn< BOOL, BoolCheck >::TType`. Slice 2's stop condition ("its counts must already match the slice-1 census") held: no new shape appeared, every row here is `sdkRetType BOOL` with `BoolCheck`.
- All 14 rows report 0 in-tree callers, so no caller audit gated this TU and every row is booked change-safe.
- One include only: `#include "../returncheck.h"` at `touch.cpp:107`, between `user32.h` and `library.h`.
- `UnregisterTouchWindow`'s comment carries the Remarks line that keeps the row checked rather than unchecked: "The UnregisterTouchWindow function succeeds even if the specified window was not previously registered as being touch-capable." - verified against the fetched page `_tmp_u2_touch_unregistertouchwindow.html` (`QUOTE-FOUND`), so an unregistered window is not reported as a failure and only a bad hwnd fails.
- `RegisterTouchWindow`'s comment carries "RegisterTouchWindow must be called on every window that will be used for touch input" - a FALSE is a registration that did not happen, not an answer.
- `RegisterTouchHitTestingWindow` is the one page here that words the pair `TRUE` / `FALSE` rather than `nonzero` / `zero`; the comment says so and `BoolCheck` covers both spellings.
- Deviation from this leaf's `## Context`: it says 14 rows / 2 slices of at most 10, and the actual split is 10 + 4 with **12** `checked` (not 10) - so slice 1 lands 8 and slice 2 lands 4; the leaf budget of 10 sites was never the binding constraint here.

