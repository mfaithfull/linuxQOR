---
Type: Plan
Title: "pointer sites"
Parent: "../plan.md"
Level: 3
Items:
  - Title: pointer slice 1
    Status: Atomic
    Content:
  - Title: pointer slice 2
    Status: Atomic
    Content:
  - Title: pointer slice 3
    Status: Atomic
    Content:
  - Title: pointer slice 4
    Status: Atomic
    Content:
---

# pointer sites

Slug: `PointerSites` - from title "pointer sites" (no drop-words, PascalCased, 12 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **32** `pointer.cpp` sites in 4 slices of at most 10 sites.

## Resume

Cursor: **terminal** - all 4 items `Atomic`, all 4 closed 12 Nov 2026 (slice 3 landed 8 tokens plus 2 unchecked reason comments; slice 4 landed 1 token plus 1 comment; final census `CheckReturn-code=28` of `table-checked=28` = `ok`, 31 comment blocks = 32 rows minus the void row). Items 1-2 are **CLOSED 12 Nov 2026** (slice 2 landed tokens at all **10** of its sites, all 10 rows `checked / BoolCheck`: 3 landed earlier in the leaf and 7 in this pass, census `CheckReturn-code=19` of `table-checked=28`, structural counts unchanged at `rows=32 fcontext=32 useswinapi=32 LibraryCall=32 Win32ErrorHandler=32` and `returncheck-includes=1`): the TU gained its one `../returncheck.h` include (`pointer.cpp:65`, the only include line for it) and 9 of slice 1's 10 sites landed the token - all 10 rows except `DestroySyntheticPointerDevice`, which is the TU's only `void` row and gets neither token nor reason. `pointer.cpp` re-census `tmp/_tmp_u2_code_census.ps1` (list `tmp/_tmp_u2_census_tus.txt` = `pointer.cpp`) reads `rows=32 fcontext=32 useswinapi=32 (32 bare + 0 AW) LibraryCall=32 Win32ErrorHandler=32 CheckReturn-code=9 table-checked=28 table-unchecked=3 returncheck-includes=1` - the 9/28 gap is the leaf's own mid-state, closed by slices 2-4 (10 + 8 + 1 = 19 tokens still owed).

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: keyboard sites, strings sites, menu sites. All 32 rows here have 0 in-tree callers, so no caller audit gates this TU.

## pointer slice 1

10 sites in `pointer.cpp`: `CreateSyntheticPointerDevice`, `DestroySyntheticPointerDevice`, `EnableMouseInPointer`, `GetPointerCursorId`, `GetPointerDevice`, `GetPointerDeviceCursors`, `GetPointerDeviceProperties`, `GetPointerDeviceRects`, `GetPointerDevices`, `GetPointerFrameInfo`. Land only the `checked` rows. Stop condition: re-read the sites, then re-census `pointer.cpp` against its `checked` row count.

## pointer slice 2

10 sites in `pointer.cpp`: `GetPointerFrameInfoHistory`, `GetPointerFramePenInfo`, `GetPointerFramePenInfoHistory`, `GetPointerFrameTouchInfo`, `GetPointerFrameTouchInfoHistory`, `GetPointerInfo`, `GetPointerInfoHistory`, `GetPointerInputTransform`, `GetPointerPenInfo`, `GetPointerPenInfoHistory`. Stop condition: re-read the sites, then re-census `pointer.cpp` - its counts must already match the slice-1 census.

## pointer slice 3

10 sites in `pointer.cpp`: `GetPointerTouchInfo`, `GetPointerTouchInfoHistory`, `GetPointerType`, `GetRawPointerDeviceData`, `InjectSyntheticPointerInput`, `IsMouseInPointerEnabled`, `RegisterPointerDeviceNotifications`, `RegisterPointerInputTarget`, `RegisterPointerInputTargetEx`, `SkipPointerFrameMessages`. Stop condition: re-read the sites, then re-census `pointer.cpp` - its counts must already match the slice-2 census.

## pointer slice 4

2 sites in `pointer.cpp`: `UnregisterPointerInputTarget`, `UnregisterPointerInputTargetEx`. Stop condition: re-read the sites, then re-census `pointer.cpp` - its counts must already match the slice-3 census.

## Cursors

pointer slice 1: {"tu":"pointer.cpp","slice_start":0,"slice_len":10,"done":true}
pointer slice 2: {"tu":"pointer.cpp","slice_start":10,"slice_len":10,"done":true}
pointer slice 3: {"tu":"pointer.cpp","slice_start":20,"slice_len":10,"done":true}
pointer slice 4: {"tu":"pointer.cpp","slice_start":30,"slice_len":2,"done":true}

## Closed 12 Nov 2026 - evidence

- Landing pass over all 4 slices, 31 sites touched: **28 tokens** at exactly the 28 `checked` rows and **3 reason comments** at the 3 `unchecked` rows; the TU's single `void` row (`DestroySyntheticPointerDevice`) got neither, as the landing contract requires.
- Final per-TU census `tmp/_tmp_u2_code_census.ps1` on `src/platform/os/windows/api_layer/user/pointer.cpp`: `rows=32 fcontext=32 useswinapi=32 (32 bare + 0 AW) LibraryCall=32 Win32ErrorHandler=32 CheckReturn-code=28 table-checked=28 table-unchecked=3 returncheck-includes=1  ok` / `tus=1 fail=0`. Mid-leaf censuses recorded as the slices advanced: 9 (after slice 1), 19 (after slice 2), 27 (after slice 3), 28 (after slice 4) - the structural counts stayed at 32/32/32/32 with `includes=1` throughout, so no site gained or lost its `qor_pp_fcontext`, `Win32ErrorHandler`, `qor_pp_useswinapi` or `Library::Call` line.
- Class vocabulary: every one of the 28 landed checks is `CheckReturn< BOOL, BoolCheck >` - all 28 `checked` rows here carry `sdkRetType BOOL`, so the base is `BOOL` and `BoolCheck` (returncheck.h:94) covers the whole TU. **No new check class** was needed and none was written.
- The 3 `unchecked` rows and their reason: `IsMouseInPointerEnabled` - a question, not a flag, "If EnableMouseInPointer is set, the return value is nonzero. If EnableMouseInPointer is not set, the return value is zero." (deliberately unlike the `EnableMouseInPointer` row, the Set* half, which is checked); `RegisterPointerInputTargetEx` and `UnregisterPointerInputTargetEx` - both pages' Return value section is the two words "Not supported.", so the page names no value for success or failure and D51 step 2 reads no-check; the `Ex` twins stay bare while their non-`Ex` halves take `BoolCheck`.
- Deviation found while landing: the tree's declaration `User32::UnregisterPointerInputTargetEx(HWND hwnd, POINTER_INPUT_TYPE pointerType)` takes **two** parameters, not the three the `RegisterPointerInputTargetEx` site takes - the published signature has no `fObserve` half, so the comment on it names the table row rather than the site's shape. No call-site argument was changed.
- All 32 rows carry 0 in-tree callers, so every landed check is change-safe and no caller audit gated this TU; each comment still carries the `AUDIT:` line with the count as the marker.
- `pointer.cpp` grew from 396 to 504 lines over this leaf (28 tokens x 1 line each plus 31 comment blocks x 4-5 lines).

