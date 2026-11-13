---
Type: Plan
Title: "menu rows"
Parent: "../plan.md"
Level: 4
Items:
  - Title: menu slice 1
    Status: Atomic
    Content:
  - Title: menu slice 2
    Status: Atomic
    Content:
  - Title: menu slice 3
    Status: Atomic
    Content:
  - Title: menu slice 4
    Status: Atomic
    Content:
---

# menu rows

Slug: `MenuRows` - from title "menu rows" (no drop-words, PascalCased, 8 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **39** `menu.cpp` rows - the largest TU in the library - in 4 slices of at most 10 URLs.

## Resume

Cursor: **terminal** - all 4 items `Atomic`, all 4 closed 12 Nov 2026. All **39** rows of `menu.cpp` carry the verdict triple: `checked 31 / unchecked 8 / void 0`, `tmp/_tmp_u2_tu_state.ps1 menu.cpp` `rows=39 checked=31 unchecked=8 void=0 blank=0 shape_bad=0`; the folder single-pass audit `tmp/_tmp_u2_folder_audit.ps1` `rows=597 filled=506 checked=299 unchecked=199 void=8 sum=506 class-shape violations=0 filled rows with no docQuote=0`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: pointer rows, keyboard rows, strings rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM menu`: 39 rows in slices of 10/10/10/9 in table order. 12 of 39 rows have 0 in-tree callers - the other 27 need the caller count in `notes` before a check lands. `GetMenu`/`GetSubMenu`-shaped handle rows answer a real handle; `IsMenu` answers BOOL - both expected `unchecked` unless the page names a failure value that is not a legal answer.

## menu slice 1

Rows 1-10: `AppendMenu`, `CheckMenuItem`, `CheckMenuRadioItem`, `CreateMenu`, `CreatePopupMenu`, `DeleteMenu`, `DestroyMenu`, `DrawMenuBar`, `EnableMenuItem`, `EndMenu`. One fetch batch of 10, then write those 10 rows.

## menu slice 2

Rows 11-20: `GetMenu`, `GetMenuBarInfo`, `GetMenuCheckMarkDimensions`, `GetMenuDefaultItem`, `GetMenuInfo`, `GetMenuItemCount`, `GetMenuItemID`, `GetMenuItemInfo`, `GetMenuItemRect`, `GetMenuState`. One fetch batch of 10, then write those 10 rows.

## menu slice 3

Rows 21-30: `GetMenuString`, `GetSubMenu`, `GetSystemMenu`, `HiliteMenuItem`, `InsertMenu`, `InsertMenuItem`, `IsMenu`, `LoadMenu`, `LoadMenuIndirect`, `MenuItemFromPoint`. One fetch batch of 10, then write those 10 rows.

## menu slice 4

Rows 31-39: `ModifyMenu`, `RemoveMenu`, `SetMenu`, `SetMenuDefaultItem`, `SetMenuInfo`, `SetMenuItemBitmaps`, `SetMenuItemInfo`, `TrackPopupMenu`, `TrackPopupMenuEx`. One fetch batch of 9, then write those 9 rows.

## Cursors

menu slice 1: {"tu":"menu.cpp","slice_start":0,"slice_len":10,"done":true}
menu slice 2: {"tu":"menu.cpp","slice_start":10,"slice_len":10,"done":true}
menu slice 3: {"tu":"menu.cpp","slice_start":20,"slice_len":10,"done":true}
menu slice 4: {"tu":"menu.cpp","slice_start":30,"slice_len":9,"done":true}

## Closed 12 Nov 2026 - evidence

- Four fetch batches, all `bad=0 no-page=0 VERDICT fail=0`, every row `match-cache` at HTTP 200: `tmp/_tmp_u2_menu_fetch.out.txt` (10), `_s2` (10), `_s3` (10), `_s4` (9). Instrument `tmp/_tmp_u2_menu_fetch.ps1` cloned from the strings one with `want=menu.cpp`, report slug `menu`.
- 39 verdict lines appended to `tmp/_tmp_u2_decisions.txt` (5 fields each) and written by `tmp/_tmp_u2_patch.ps1` (`decisions with no table row = 0`).
- The 404 fallback leg is load-bearing here: 8 rows have no page under the plain slug and were fetched at the suffixed ANSI twin (`appendmenua`, `getmenuiteminfoa`, `insertmenua`, `insertmenuitema`, `loadmenua`, `loadmenuindirecta`, `modifymenua`, `setmenuiteminfoa`), each printed as a `CORRECTED` line.
- The leaf's expected unchecked set did not hold. Measured, only **8** of 39 rows are unchecked - `menu.cpp` is the most-checkable TU in the library at **31** checked rows, because the family is dominated by `Set*`/`Append*`/`Insert*`/`Delete*` actions whose pages reserve zero outright.
- Class shapes used here, several for the first time in the tree: `CheckNonZeroHandle< HMENU >` (4 rows - `CreateMenu`, `CreatePopupMenu`, `LoadMenu`, `LoadMenuIndirect`, all with the published `HMENU` as the base since their `sdkRetType` is blank or `NO-PROTOTYPE`), `TCheckFailureValue< DWORD, ( DWORD )-1 >` (`CheckMenuItem` - the UINT_MAX sentinel, MF_UNCHECKED being a legal zero), `TCheckFailureValue< UINT, ( UINT )-1 >` (4 rows - `GetMenuDefaultItem`, `GetMenuItemID`, `GetMenuState`: zero is a legal item position/identifier/flag-mask answer so the check must key on the documented sentinel), `TCheckFailureValue< int, -1 >` (2 rows - `GetMenuItemCount`, `MenuItemFromPoint`), and `TCheckFailureValue< BOOL, -1 >` (`EnableMenuItem`, the `GetMessage` reading with `MF_ENABLED` a legal zero).
- The arbitration recorded in the reasons: `GetMenu`/`GetSubMenu`/`GetSystemMenu`/`IsMenu`/`HiliteMenuItem` are unchecked because their pages name **no** failure condition - NULL or zero answers a legal question about the caller's own window, item or handle (`GetSystemMenu` answers NULL by design when `bRevert` is TRUE) - while `CreateMenu`/`CreatePopupMenu`/`LoadMenu`/`LoadMenuIndirect` take `CheckNonZeroHandle< HMENU >` because their pages do name a failure condition and route to `GetLastError`.
- `TrackPopupMenu`/`TrackPopupMenuEx` are unchecked on a shape no other TU shows: the page makes zero simultaneously the legal answer for a cancelled menu and the error value under a runtime flag (`TPM_RETURNCMD`), so the two are indistinguishable and no class can branch on the flag.
- 27 of 39 rows carry nonzero in-tree callers (1 each, `GetMenuItemRect` and `HiliteMenuItem` 2), matching this leaf's `## Context`; each carries its caller count in `notes` as the audit-before-landing marker, and the 12 zero-caller rows are booked change-safe.
- No code change is proposed here; only the verdict triple.
