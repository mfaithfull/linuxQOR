---
Type: Plan
Title: "misc sites"
Parent: "../plan.md"
Level: 3
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

# misc sites

Slug: `MiscSites` - from title "misc sites" (no drop-words, PascalCased, 9 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **23** `misc.cpp` sites in 3 slices of at most 10 sites.

## Resume

Cursor: **terminal** - all 3 items `Atomic`, all 3 closed 12 Nov 2026. All **13** `checked` `misc.cpp` sites carry the token; the 9 `unchecked` rows and the 1 `void` row stay bare. Final census `tmp/_tmp_u2_code_census.txt` `misc.cpp rows=23 fcontext=23 useswinapi=23 (22 bare + 1 AW) LibraryCall=23 Win32ErrorHandler=23 CheckReturn-code=13 table-checked=13 table-unchecked=9 returncheck-includes=1 ok`; standalone per-TU compile `tmp/_tmp_u2_one_tu.bat misc.cpp` `TU_EXIT=0`, log = `misc.cpp` only (zero diagnostics at `/W4`).

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: desktop sites, clipboard sites, dpi sites, dialog sites, message sites, painting sites. All 23 rows here have 0 in-tree callers, so no caller audit gates this TU.

## misc slice 1

10 sites in `misc.cpp`: `CalculatePopupWindowPosition`, `ChangeMenu`, `ChangeWindowMessageFilterEx`, `EvaluateProximityToPolygon`, `EvaluateProximityToRect`, `GetAutoRotationState`, `GetCIMSSM`, `GetClipboardMetadata`, `GetCurrentInputMessageSource`, `GetUnpredictedMessagePos`. Land only the `checked` rows. Stop condition: re-read the sites, then re-census `misc.cpp` against its `checked` row count.

## misc slice 2

10 sites in `misc.cpp`: `GetWindowDisplayAffinity`, `GetWindowFeedbackSetting`, `InheritWindowMonitor`, `IsImmersiveProcess`, `IsWindowArranged`, `RegisterForTooltipDismissNotification`, `SetAdditionalForegroundBoostProcesses`, `SetCoalescableTimer`, `SetDebugErrorLevel`, `SetProcessRestrictionExemption`. Stop condition: re-read the sites, then re-census `misc.cpp` - its counts must already match the slice-1 census.

## misc slice 3

3 sites in `misc.cpp`: `SetThreadCursorCreationScaling`, `SetWindowDisplayAffinity`, `SetWindowFeedbackSetting`. Stop condition: re-read the sites, then re-census `misc.cpp` - its counts must already match the slice-2 census.

## Cursors

misc slice 1: {"tu":"misc.cpp","slice_start":0,"slice_len":10,"done":true}
misc slice 2: {"tu":"misc.cpp","slice_start":10,"slice_len":10,"done":true}
misc slice 3: {"tu":"misc.cpp","slice_start":20,"slice_len":3,"done":true}

## Closed 12 Nov 2026 - evidence

- Slice 1 landed 6 tokens (`CalculatePopupWindowPosition`, `ChangeWindowMessageFilterEx`, `EvaluateProximityToPolygon`, `EvaluateProximityToRect`, `GetAutoRotationState`, `GetCurrentInputMessageSource`); census after the slice read `CheckReturn-code=6 table-checked=13 ... check-FAIL`, the expected mid-batch tag.
- Slice 2 landed 5 tokens (`GetWindowDisplayAffinity`, `InheritWindowMonitor`, `SetAdditionalForegroundBoostProcesses`, `SetCoalescableTimer`, `SetProcessRestrictionExemption`); census after the slice read `CheckReturn-code=11 ... check-FAIL`.
- Slice 3 landed 2 tokens (`SetWindowDisplayAffinity`, `SetWindowFeedbackSetting`); census after the slice reads `CheckReturn-code=13 table-checked=13 ... ok` - 6+5+2 = 13, exactly the TU's `checked` row count.
- One include only: `../returncheck.h` added at `misc.cpp:12`, and the census counts it once (`returncheck-includes=1`).
- Class bases follow the table's `sdkRetType`, falling back to `pubRetType` where that column is blank: 12 sites take `BoolCheck` (returncheck.h:94) and `SetCoalescableTimer` takes `CheckNonZero< UINT_PTR >` (returncheck.h:187, `template< typename TNumeric > : public AutoErrorCheck< TNumeric >`) - the row's `sdkRetType` is blank and its `pubRetType` is `UINT_PTR`, so the published type is the base. No new check class was needed in this TU.
- The 9 `unchecked` rows carry no token but each got the written reason in its site comment per the contract's `## Decisions` ("`unchecked` rows get the written reason in the site comment and no token; `void` rows get neither") - all 9 blocks are present alongside the 13 checked policy comments (22 blocks total), each quoting the table's verbatim docQuote; the `void` row (`SetDebugErrorLevel`, `Library::voidCall`) carries neither token nor comment.
- Every landed site keeps its original per-row note above the new `//Return policy (U2, table row <stem> / misc.cpp, 12 Nov 2026)` block - the `dpi.cpp` precedent the pass follows - and quotes the table's verbatim docQuote inside it.
- All 23 rows report 0 in-tree callers (D14 published surface), so no caller audit gated any landing; each policy comment states that explicitly.
- Standalone compile after the last slice: `tmp/_tmp_u2_one_tu.bat misc.cpp` → `TU_EXIT=0`, log `misc.cpp` with zero diagnostics at `/W4`. Note the run needed `tmp/obj/` to exist first - it had been cleaned, and a missing obj dir shows up as `fatal error C1083`, not as a source error.

