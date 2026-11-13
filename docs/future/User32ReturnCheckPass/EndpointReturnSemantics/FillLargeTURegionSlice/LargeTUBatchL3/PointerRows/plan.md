---
Type: Plan
Title: "pointer rows"
Parent: "../plan.md"
Level: 4
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

# pointer rows

Slug: `PointerRows` - from title "pointer rows" (no drop-words, PascalCased, 11 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **32** `pointer.cpp` rows in 4 slices of at most 10 URLs.

## Resume

Cursor: **terminal** - all 4 items `Atomic`, all 4 closed 12 Nov 2026. All **32** rows of `pointer.cpp` carry the verdict triple: `checked 28 / unchecked 3 / void 1`, `tmp/_tmp_u2_tu_state.ps1 pointer.cpp` `rows=32 checked=28 unchecked=3 void=1 blank=0 shape_bad=0`; the folder single-pass audit `tmp/_tmp_u2_folder_audit.ps1` `rows=597 filled=399 checked=248 unchecked=144 void=7 sum=399 class-shape violations=0 filled rows with no docQuote=0`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: keyboard rows, strings rows, menu rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM pointer`: 32 rows in slices of 10/10/10/2 in table order. All 32 rows here have 0 in-tree callers (change-safe without a caller audit).

## pointer slice 1

Rows 1-10: `CreateSyntheticPointerDevice`, `DestroySyntheticPointerDevice`, `EnableMouseInPointer`, `GetPointerCursorId`, `GetPointerDevice`, `GetPointerDeviceCursors`, `GetPointerDeviceProperties`, `GetPointerDeviceRects`, `GetPointerDevices`, `GetPointerFrameInfo`. One fetch batch of 10, then write those 10 rows.

## pointer slice 2

Rows 11-20: `GetPointerFrameInfoHistory`, `GetPointerFramePenInfo`, `GetPointerFramePenInfoHistory`, `GetPointerFrameTouchInfo`, `GetPointerFrameTouchInfoHistory`, `GetPointerInfo`, `GetPointerInfoHistory`, `GetPointerInputTransform`, `GetPointerPenInfo`, `GetPointerPenInfoHistory`. One fetch batch of 10, then write those 10 rows.

## pointer slice 3

Rows 21-30: `GetPointerTouchInfo`, `GetPointerTouchInfoHistory`, `GetPointerType`, `GetRawPointerDeviceData`, `InjectSyntheticPointerInput`, `IsMouseInPointerEnabled`, `RegisterPointerDeviceNotifications`, `RegisterPointerInputTarget`, `RegisterPointerInputTargetEx`, `SkipPointerFrameMessages`. One fetch batch of 10, then write those 10 rows.

## pointer slice 4

Rows 31-32: `UnregisterPointerInputTarget`, `UnregisterPointerInputTargetEx`. One fetch batch of 2, then write those 2 rows.

## Cursors

pointer slice 1: {"tu":"pointer.cpp","slice_start":0,"slice_len":10,"done":true}
pointer slice 2: {"tu":"pointer.cpp","slice_start":10,"slice_len":10,"done":true}
pointer slice 3: {"tu":"pointer.cpp","slice_start":20,"slice_len":10,"done":true}
pointer slice 4: {"tu":"pointer.cpp","slice_start":30,"slice_len":2,"done":true}

## Closed 12 Nov 2026 - evidence

- Four fetch batches, all at `VERDICT fail=0` with every `Return value` section `match-cache` against `tmp/_tmp_u2_probe_cache.txt`: `tmp/_tmp_u2_ptr_fetch.out.txt` (10), `tmp/_tmp_u2_ptr_fetch_s2.out.txt` (10), `tmp/_tmp_u2_ptr_fetch_s3.out.txt` (10), `tmp/_tmp_u2_ptr_fetch_s4.out.txt` (2) - 32 of 32 fetched at HTTP 200, `bad=0 no-page=0`, zero `CORRECTED` legs (every page lives on the plain `nf-winuser-<stem>` slug).
- Instrument `tmp/_tmp_u2_ptr_fetch.ps1` cloned from `tmp/_tmp_u2_paint_fetch.ps1` with the corrected slice-tag math `[Math]::Floor($skip / $take) + 1`.
- 32 verdict lines appended to `tmp/_tmp_u2_decisions.txt` (the `pointer.cpp` block) and written by `tmp/_tmp_u2_patch.ps1`: `decisions with no table row = 0`, `class-shape violations = 0`, `filled rows with no page = 11` unchanged (no `pointer.cpp` row is pageless).
- All 32 rows report 0 in-tree callers, so every row is booked change-safe and no caller audit is owed - matching this leaf's `## Context` exactly.
- `DestroySyntheticPointerDevice` (pubRetType void, sdkRetType VOID, page Return value "None") is the leaf's `void` row.
- `CreateSyntheticPointerDevice` is the leaf's only non-`BoolCheck` check: a handle answer with a documented NULL failure, so `CheckNonZeroHandle< HSYNTHETICPOINTERDEVICE >` (returncheck.h:137) over the row's blank `sdkRetType`, i.e. the published type - the same class the HDC/HBITMAP/HCURSOR/HDESK rows take, not `HandleCheck` (which keys on `INVALID_HANDLE_VALUE`).
- The three `unchecked` rows: `IsMouseInPointerEnabled` (reports the process's own pointer-mode state - "If EnableMouseInPointer is set, the return value is nonzero. If EnableMouseInPointer is not set, the return value is zero." - so a process that never enabled pointer mode answers zero legitimately), and `RegisterPointerInputTargetEx` / `UnregisterPointerInputTargetEx` whose entire Return value section is the two words "Not supported." - the page names no value for success or failure, so D51 step 2 reads no-check and no check is landed on a value the page never assigns meaning to.
- Deviation from this leaf's `## Context` (which named no expected shape): measured, 28 of 32 rows are `checked` - the TU is dominated by the status-BOOL family whose data all travels in out-parameters, so the answer-shaped minority is 3, not a majority. The three page wordings used are the same three the misc leaf booked checked (`If this function succeeds, it returns TRUE. Otherwise, it returns FALSE.`, `TRUE if the function succeeds; otherwise, FALSE.`, `If the function succeeds, the return value is non-zero. If the function fails, the return value is zero.`), which is what keeps the pair `EnableMouseInPointer` checked / `IsMouseInPointerEnabled` unchecked defensible.
- No code change proposed; verdict triple only. `pointer.cpp` has 0 live `CheckReturn` sites today, so all 28 checks would be the first in this TU - reconciled at the `PointerSites` code-side leaf.

