---
Type: Plan
Title: "cursor sites"
Parent: "../plan.md"
Level: 3
Items:
  - Title: cursor slice 1
    Status: Atomic
    Content:
  - Title: cursor slice 2
    Status: Atomic
    Content:
---

# cursor sites

Slug: `CursorSites` - from title "cursor sites" (no drop-words, PascalCased, 11 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **15** `cursor.cpp` sites in 2 slices of at most 10 sites.

## Resume

Cursor: **terminal** - all 2 items `Atomic`, both closed 12 Nov 2026. All **15** `cursor.cpp` sites are in their end state: `checked 12 / unchecked 3`, and `tmp/_tmp_u2_code_census.ps1` reads `cursor.cpp rows=15 fcontext=15 useswinapi=15 (13 bare + 2 AW) LibraryCall=15 Win32ErrorHandler=15 CheckReturn-code=12 table-checked=12 table-unchecked=3 returncheck-includes=1 ok` - the token count equals the `checked` row count, one include, `ok`. Item 2 **cursor slice 2** is **CLOSED 12 Nov 2026**. Item 1 **cursor slice 1** is **CLOSED 12 Nov 2026**. Terminal: both items `Atomic`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: mouse sites, rectangle sites, scrollbar sites, icon sites, touch sites, windowclass sites.

Slice boundaries fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM cursor`. The `HCURSOR` rows decide on documented NULL-as-failure only; `SetCursor`/`ShowCursor` answer the previous cursor state and stay `unchecked` with the reason.

## cursor slice 1

10 sites in `cursor.cpp`: `ClipCursor`, `CreateCursor`, `DestroyCursor`, `GetClipCursor`, `GetCursor`, `GetCursorInfo`, `GetCursorPos`, `GetPhysicalCursorPos`, `LoadCursor`, `LoadCursorFromFile`. Land only the `checked` rows. Stop condition: re-read the sites, then re-census `cursor.cpp` against its `checked` row count.

## cursor slice 2


## Cursors

cursor slice 1: {"tu":"cursor.cpp","slice_start":0,"slice_len":10,"done":true}
cursor slice 2: {"tu":"cursor.cpp","slice_start":10,"slice_len":5,"done":true}

## Closed 12 Nov 2026 - evidence

- Slice 1 landed 9 tokens and left `GetCursor` bare; re-census at the leaf's own stop condition read `CheckReturn-code=9 table-checked=12 ... check-FAIL`, the FAIL being the 3 rows slice 2 owes.
- Slice 2 landed 3 tokens (`SetCursorPos`, `SetPhysicalCursorPos`, `SetSystemCursor` at `CheckReturn< BOOL, BoolCheck >::TType`) and left `ShowCursor` and `SetCursor` bare; final census `CheckReturn-code=12 table-checked=12 table-unchecked=3 returncheck-includes=1 ok`.
- One `../returncheck.h` include for the TU (G6: a landed check is live on every call, since `qor_pp_parameter_checking` / `qor_pp_yes` are defined nowhere and `returncheck.h:19`'s `#if` reads `0 == 0`, making `::TType` the checked wrapper type).
- Caller audit ran before landing (`tmp/_tmp_u2_cursor_callers.ps1`, then T-tolerant `_tmp_u2_cursor_callers_t.ps1`): 10 of the 15 rows report live in-tree callers, all found at `src/platform/os/windows/ui/gui/gdiobjects/cursor.cpp` - the first pattern missed the T-suffixed wrappers, and the re-run caught them.
- `SetCursor`'s only in-tree caller discards the return: `Cursor::SetAsCurrent()` at `ui/gui/gdiobjects/cursor.cpp:185-186` is `User32::SetCursor( reinterpret_cast<HCURSOR>(m_handle.Use()));` in a `void` method, so the previous-cursor handle the page describes is dropped there and no site misreads it - recorded at the site rather than repaired.
- `ShowCursor` is unchecked on the display-counter reading quoted at its site; the misreading is recorded, not repaired, at `ui/gui/gdiobjects/cursor.cpp:180` (`Cursor::Show` returns `User32::ShowCursor(show ? 1 : 0) ? true : false`, which reads the signed count as a flag) - a `winqor_gui` behaviour change the api_layer review records rather than performs, closed by D47.
- Deviation from the leaf's `## Context`, which predicted `SetCursor`/`ShowCursor` as the unchecked pair: measured, the unchecked set is `GetCursor`, `ShowCursor`, `SetCursor` - `SetCursorPos` and `SetPhysicalCursorPos` are `checked` because their pages carry the plain status form with the coordinates as parameters. `HCURSOR` rows decide on documented NULL-as-failure only, and `CheckNonZeroHandle< HCURSOR >` was used for the three handle-returning rows (`CreateCursor`, `LoadCursor`, `LoadCursorFromFile`) whose pages reserve NULL.
- No vocabulary item was needed: every class used here (`BoolCheck`, `CheckNonZeroHandle< HCURSOR >`) is already in `api_layer/returncheck.h`.

5 sites in `cursor.cpp`: `SetCursorPos`, `SetPhysicalCursorPos`, `ShowCursor`, `SetCursor`, `SetSystemCursor`. Stop condition: re-read the sites, then re-census `cursor.cpp` - its counts must already match the slice-1 census.
