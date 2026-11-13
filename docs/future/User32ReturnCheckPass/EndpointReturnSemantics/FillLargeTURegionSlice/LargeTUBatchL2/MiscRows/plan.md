---
Type: Plan
Title: "misc rows"
Parent: "../plan.md"
Level: 4
Items:
  - Title: misc slice 1
    Status: Atomic
    Content:
  - Title: misc slice 2
    Status: Atomic
    Content:
  - Title: misc slice 3
    Status: Atomic
    Content:
---

# misc rows

Slug: `MiscRows` - from title "misc rows" (no drop-words, PascalCased, 8 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **23** `misc.cpp` rows in 3 slices of at most 10 URLs.

## Resume

Cursor: **terminal** - all 3 items `Atomic`, all 3 closed 12 Nov 2026. All **23** rows of `misc.cpp` carry the verdict triple: `checked 13 / unchecked 9 / void 1`, `tmp/_tmp_u2_tu_state.ps1 misc.cpp` `rows=23 checked=13 unchecked=9 void=1 blank=0 shape_bad=0`; the folder audit `tmp/_tmp_u2_folder_audit.ps1` `rows=597 filled=318 checked=195 unchecked=118 void=5 class-shape violations=0` and `tmp/_tmp_u2_patch.ps1` `VERDICT fail=0`, `decisions with no table row = 0`, `filled rows with no page = 11` (up 3 from 8 - `ChangeMenu`, `GetClipboardMetadata`, `SetDebugErrorLevel`).

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: desktop rows, clipboard rows, dpi rows, dialog rows, message rows, painting rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM misc`: 23 rows in slices of 10/10/3 in table order. 23 of 23 rows here have 0 in-tree callers. Several pages here (`GetAutoRotationState`, `GetCIMSSM`, `EvaluateProximityTo*`) are not on the plain `nf-winuser-<stem>` slug - the 404 fallback leg of the slice contract is load-bearing for this TU.

## misc slice 1

Rows 1-10: `CalculatePopupWindowPosition`, `ChangeMenu`, `ChangeWindowMessageFilterEx`, `EvaluateProximityToPolygon`, `EvaluateProximityToRect`, `GetAutoRotationState`, `GetCIMSSM`, `GetClipboardMetadata`, `GetCurrentInputMessageSource`, `GetUnpredictedMessagePos`. One fetch batch of 10, then write those 10 rows.

## misc slice 2

Rows 11-20: `GetWindowDisplayAffinity`, `GetWindowFeedbackSetting`, `InheritWindowMonitor`, `IsImmersiveProcess`, `IsWindowArranged`, `RegisterForTooltipDismissNotification`, `SetAdditionalForegroundBoostProcesses`, `SetCoalescableTimer`, `SetDebugErrorLevel`, `SetProcessRestrictionExemption`. One fetch batch of 10, then write those 10 rows.

## misc slice 3

Rows 21-23: `SetThreadCursorCreationScaling`, `SetWindowDisplayAffinity`, `SetWindowFeedbackSetting`. One fetch batch of 3, then write those 3 rows.

## Cursors

misc slice 1: {"tu":"misc.cpp","slice_start":0,"slice_len":10,"done":true}
misc slice 2: {"tu":"misc.cpp","slice_start":10,"slice_len":10,"done":true}
misc slice 3: {"tu":"misc.cpp","slice_start":20,"slice_len":3,"done":true}

## Closed 12 Nov 2026 - evidence

- Slice 1 (rows 1-10) in one fetch batch: `tmp/_tmp_u2_misc_fetch.out.txt` - 8 `FETCH ... match-cache http=200` lines plus 2 `NO-PAGE` lines (`ChangeMenu`, `GetClipboardMetadata`), `fetched=8 of 10 bad=0 no-page=2`, `VERDICT fail=0`.
- Slice 2 (rows 11-20) in one fetch batch: `tmp/_tmp_u2_misc_fetch_s2.out.txt` - 9 `match-cache` at HTTP 200 plus `SetDebugErrorLevel` booked NO-PAGE, `fetched=9 of 10 bad=0 no-page=1`, `VERDICT fail=0`.
- Slice 3 (rows 21-23) in one fetch batch: `tmp/_tmp_u2_misc_fetch_s3.out.txt` - 3 `match-cache` at HTTP 200, `fetched=3 of 3 bad=0 no-page=0`, `VERDICT fail=0`.
- 23 verdict lines appended to `tmp/_tmp_u2_decisions.txt` and written into the table by `tmp/_tmp_u2_patch.ps1`; `misc.cpp rows=23 checked=13 unchecked=9 void=1 blank=0 shape_bad=0`, arithmetic exact (13+9+1=23).
- All 23 rows carry 0 in-tree callers, so every row here is booked change-safe and no call-site audit is owed.
- `SetDebugErrorLevel` is the batch's only `void` row (`pubRetType void`, `sdkRetType VOID`), booked with an empty `checkClass` and a `-` docQuote; the folder audit's `void` count moves 4 -> 5 on it.
- `SetCoalescableTimer` is the batch's only non-`BoolCheck` class: value answer with a reserved zero and a never-zero timer identifier, so `CheckNonZero< UINT_PTR >` (the row's `sdkRetType` column is blank, its `pubRetType` is `UINT_PTR`, and `returncheck.h:187` is `template< typename TNumeric > class CheckNonZero : public AutoErrorCheck< TNumeric >`).
- Deviation from this leaf's `## Context`: it said several pages here live under a suffixed slug and that the 404 fallback leg was load-bearing. It is not - the probe marked all 20 fetched rows `resolved` on the plain `nf-winuser-<stem>` slug and the batches printed zero `CORRECTED` lines. What was load-bearing instead: the probe's `no-page` rows tagged `no-return-value-section` (3 rows) and the pages' Remarks cross-references.
- `RegisterForTooltipDismissNotification` is `unchecked` on the Remarks its Return value line points at ("TRUE if the window was successfully registered or unregistered; otherwise, FALSE . (See Remarks.)"): the Remarks put FALSE at "FALSE if the window was already registered. The window is treated as registered either way" and at "FALSE if the windows was not currently registered. The window is treated as unregistered either way", so FALSE reports a state already where the caller wants it.
- `IsImmersiveProcess` and `IsWindowArranged` are `unchecked` on the answer-shaped reading (the calls ask a yes/no question about the handle they are passed, and their pages carry only the generic boilerplate / "A nonzero value if the window is arranged; otherwise, zero." with no Remarks naming a failure condition); the `SetProcessRestrictionExemption` row shares the same boilerplate but is a `Set*` whose subject is its parameter, so it is `checked` - that pair is the arbitration recorded in both reasons.
- `GetCIMSSM` is `unchecked` with a flagged vocabulary need: its page reserves `ERROR_INVALID_PARAMETER` (an HRESULT) as the failure value, distinct from `TRUE`, so the site would be checked in principle - but the row's `sdkRetType` and `pubRetType` are both `BOOL` and no class in `returncheck.h` compares a `BOOL` row against an HRESULT sentinel (`BoolCheck` and `CheckNonZero` both read the nonzero HRESULT as success), so booking it checked here would land a check that cannot see the documented failure; the fitting class belongs to the `CheckClassVocabulary` leaf. The page also deprecates the call ("GetCIMSSM may be altered or unavailable in the future. Instead, use GetCurrentInputMessageSource").
- `GetAutoRotationState`, `GetCIMSSM` and the `EvaluateProximityTo*` pair are the pointer-family rows this leaf named; the `EvaluateProximityTo*` pair is `checked` because the page's first line puts the answer in the `TOUCH_HIT_TESTING_PROXIMITY_EVALUATION` out-parameter ("Returns the score of a polygon as the probable touch target ... and an adjusted touch point within the polygon"), so a far-away polygon answers through the out-parameter while `FALSE` is a failed call - the same wording the `RegisterTouchHitTestingWindow` row is checked on.
- No code change is proposed here; only the verdict triple.
