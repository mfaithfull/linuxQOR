---
Type: Plan
Title: "touch rows"
Parent: "../plan.md"
Level: 4
Items:
  - Title: touch slice 1
    Status: Atomic
    Content:
  - Title: touch slice 2
    Status: Atomic
    Content:
---

# touch rows

Slug: `TouchRows` - from title "touch rows" (no drop-words, PascalCased, 9 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **14** `touch.cpp` rows in 2 slices of at most 10 URLs.

## Resume

This plan is **terminal** - both items `Atomic` and both **CLOSED 12 Nov 2026**. Item 1 **touch slice 1** is **CLOSED 12 Nov 2026**: rows 1-10 decided **checked 8 / unchecked 2 / void 0** (one batch of exactly 10 URLs; `tmp/_tmp_u2_tu_fetch.ps1 touch.cpp 10 0` -> `tmp/_tmp_u2_touch_fetch.out.txt`, every `Return value` section `match-cache` against `tmp/_tmp_u2_probe_cache.txt`). The 8 `checked` rows are `BoolCheck` on the plain status wording ("If the function succeeds, the return value is nonzero. If the function fails, the return value is zero", and the TRUE/FALSE spellings on `InitializeTouchInjection` / `InjectTouchInput`) over `BOOL` `sdkRetType`, so the class base matches and `BoolCheck` is unparameterised. The 2 `unchecked` rows are answer-shaped: `IsTouchWindow` ("Returns TRUE if the window supports Windows Touch; returns FALSE if the window does not support Windows Touch.") answers the yes/no question the call asks, and `PackTouchHitTestingProximityEvaluation` returns the packed score as an `LRESULT` - "If this function succeeds, it returns the score and adjustedPoint values from TOUCH_HIT_TESTING_PROXIMITY_EVALUATION as an LRESULT." - so a packed score of 0 is a legitimate score. Item 2 **touch slice 2** is **CLOSED 12 Nov 2026**: rows 11-14 decided **checked 4 / unchecked 0 / void 0** (one batch of exactly 4 URLs; `tmp/_tmp_u2_touch_fetch_s2.out.txt` - 4 `FETCH` rows at `http=200`, `fetched=4 of 4`, `bad=0`, `VERDICT fail=0`, every section `match-cache`). All 4 are plain status BOOLs with the same 0-reserving wording, so all 4 take `BoolCheck`; `UnregisterTouchWindow` stays `checked` despite its Remark "The UnregisterTouchWindow function succeeds even if the specified window was not previously registered as being touch-capable." - that Remark moves the not-registered case out of the FALSE, it does not make FALSE a legal answer. Stop condition met: `tmp/_tmp_u2_tu_state.ps1 touch.cpp` -> `tmp/_tmp_u2_touch_state5.txt` `tu=touch.cpp rows=14 checked=12 unchecked=2 void=0 blank=0 shape_bad=0`, i.e. the **14** rows `tmp/_tmp_u2_slice_stems.txt` claims for `ITEM touch`; folder-wide `tmp/_tmp_u2_filled.txt` `tus=53 rows=597 filled=185 full-tus=38 / partial=0 / empty=0` and `tmp/_tmp_u2_patch.out.txt` `decided = 185`, `class-shape violations = 0`, `decisions with no table row = 0`, **fail=0**. No source file was touched - this pass fills table rows only.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: mouse rows, rectangle rows, scrollbar rows, icon rows, windowclass rows, cursor rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM touch`: 14 rows in slices of 10 and 4 in table order. `IsTouchWindow` answers BOOL legitimately - expected `unchecked` with the page's wording.

## touch slice 1

Rows 1-10: `CloseGestureInfoHandle`, `CloseTouchInputHandle`, `GetGestureConfig`, `GetGestureExtraArgs`, `GetGestureInfo`, `GetTouchInputInfo`, `InitializeTouchInjection`, `InjectTouchInput`, `IsTouchWindow`, `PackTouchHitTestingProximityEvaluation`. One fetch batch of 10, then write those 10 rows.

## touch slice 2

Rows 11-14: `RegisterTouchHitTestingWindow`, `RegisterTouchWindow`, `SetGestureConfig`, `UnregisterTouchWindow`. One fetch batch of 4, then write those 4 rows.
