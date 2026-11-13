---
Type: Plan
Title: "menu sites"
Parent: "../plan.md"
Level: 3
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

# menu sites

Slug: `MenuSites` - from title "menu sites" (no drop-words, PascalCased, 9 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **39** `menu.cpp` sites - the largest TU in the library - in 4 slices of at most 10 sites.

## Resume

Cursor: **terminal** - all 4 items `Atomic`, all 4 closed 12–13 Nov 2026. All **39** `menu.cpp` sites read a verdict and the token landed at exactly the **31** `checked` rows; `menu.cpp` census `lines=663 rows=39 fcontext=39 useswinapi=40 (30 bare + 10 AW; the 40th is the header comment at line 21, so 39 site bindings) LibraryCall=39 Win32ErrorHandler=39 policy-blocks=31 CheckReturn-code=31 table-checked=31 table-unchecked=8 returncheck-includes=1 ok`; the standalone gate `tmp/_tmp_u2_one_tu.bat menu.cpp` reads `TU_EXIT=0 log_lines=1 diags=0 dating=obj-newer`. Terminal: all 4 items `Atomic`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, caller audit for nonzero counts, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: pointer sites, keyboard sites, strings sites. 27 of 39 rows have nonzero in-tree callers - each `checked` row among them needs the site-level read before its token moves. `GetMenu`/`GetSubMenu` answer a real handle and `IsMenu` answers BOOL - expected `unchecked` unless the page names a failure value that is not a legal answer.

## menu slice 1

10 sites in `menu.cpp`: `AppendMenu`, `CheckMenuItem`, `CheckMenuRadioItem`, `CreateMenu`, `CreatePopupMenu`, `DeleteMenu`, `DestroyMenu`, `DrawMenuBar`, `EnableMenuItem`, `EndMenu`. Land only the `checked` rows. Stop condition: re-read the sites, then re-census `menu.cpp` against its `checked` row count.

## menu slice 2

10 sites in `menu.cpp`: `GetMenu`, `GetMenuBarInfo`, `GetMenuCheckMarkDimensions`, `GetMenuDefaultItem`, `GetMenuInfo`, `GetMenuItemCount`, `GetMenuItemID`, `GetMenuItemInfo`, `GetMenuItemRect`, `GetMenuState`. Stop condition: re-read the sites, then re-census `menu.cpp` - its counts must already match the slice-1 census.

## menu slice 3

10 sites in `menu.cpp`: `GetMenuString`, `GetSubMenu`, `GetSystemMenu`, `HiliteMenuItem`, `InsertMenu`, `InsertMenuItem`, `IsMenu`, `LoadMenu`, `LoadMenuIndirect`, `MenuItemFromPoint`. Stop condition: re-read the sites, then re-census `menu.cpp` - its counts must already match the slice-2 census.

## menu slice 4

9 sites in `menu.cpp`: `ModifyMenu`, `RemoveMenu`, `SetMenu`, `SetMenuDefaultItem`, `SetMenuInfo`, `SetMenuItemBitmaps`, `SetMenuItemInfo`, `TrackPopupMenu`, `TrackPopupMenuEx`. Stop condition: re-read the sites, then re-census `menu.cpp` - its counts must already match the slice-3 census.
## Cursors

menu slice 1: {"tu":"menu.cpp","slice_start":0,"slice_len":10,"done":true}
menu slice 2: {"tu":"menu.cpp","slice_start":10,"slice_len":10,"done":true}
menu slice 3: {"tu":"menu.cpp","slice_start":20,"slice_len":10,"done":true}
menu slice 4: {"tu":"menu.cpp","slice_start":30,"slice_len":9,"done":true}

## Closed 12–13 Nov 2026 - evidence

- Slice 1 (sites 1-10) all 10 `checked`, all 10 landed: census `lines=492 tokens=10 policy=10 include=1`.
- Slice 2 (sites 11-20) 8 landed / 2 left bare (`GetMenu`, `GetMenuCheckMarkDimensions`): census `tokens=18 policy=18`.
- Slice 3 (sites 21-30) 6 landed / 4 left bare (`GetSubMenu`, `GetSystemMenu`, `HiliteMenuItem`, `IsMenu`): census `tokens=24 policy=24`.
- Slice 4 (sites 31-39) 7 landed / 2 left bare (`TrackPopupMenu`, `TrackPopupMenuEx`): census `tokens=31 policy=31`, `lines=663`.
- Token total 31 = the table's 31 `checked` rows exactly; bare sites 8 = the table's 8 `unchecked` rows exactly. No site is in the fourth state (no `void` row in this TU).
- Class bases taken from `sdkRetType` per the landing contract: `BoolCheck` (returncheck.h:94) on 20 rows, `TCheckFailureValue< T, iFailure >` (returncheck.h:213) on 9 rows - `CheckMenuItem`/`GetMenuState`/`GetMenuItemID`/`GetMenuDefaultItem`/`GetMenuCheckMarkDimensions`-style `UINT`/`DWORD` rows keyed at `( UINT )-1` / `( DWORD )-1`, `GetMenuItemCount` and `MenuItemFromPoint` at `int, -1`, `EnableMenuItem` at `BOOL, -1` - `CheckNonZeroHandle< HMENU >` (returncheck.h:137) on `CreateMenu`/`CreatePopupMenu`/`LoadMenu`/`LoadMenuIndirect`, and `CheckNonZero< int >` (returncheck.h:187) on `GetMenuString`. No new check class was written in this leaf.
- Caller audit honoured: 27 of 39 rows have nonzero in-tree callers; every landed site among them carries its table count and the site-level read path in its policy comment (`AUDIT: ... site-level read is menu.cpp:<line>`), e.g. `AppendMenu` 1 caller at `menu.cpp:98`, `GetMenuItemRect` 2 callers at `menu.cpp:143`/`menu.cpp:150`.
- `GetMenu` and `GetSubMenu` stayed `unchecked` on the handle-answer reading (a menu with no submenu answers NULL legitimately), while `CreateMenu`/`CreatePopupMenu`/`LoadMenu`/`LoadMenuIndirect` took `CheckNonZeroHandle< HMENU >` because their pages reserve NULL for a failed creation - the pair the leaf's `## Context` predicted, confirmed rather than deviated.
- `GetMenuCheckMarkDimensions` stayed `unchecked`: its page makes the count itself the answer with no reserved failure value, unlike its `GetMenuDefaultItem` neighbour which reserves `-1` (a valid item identifier is nonzero, so `TCheckFailureValue< UINT, ( UINT )-1 >` passes at every identifier).
- Gate re-run after slice 4: `tmp/_tmp_u2_one_tu.bat` on `menu.cpp` exits `TU_EXIT=0` with an empty log (`log_lines=1`, zero `warning|error C|fatal` diagnostics) and `obj-newer` dating - all 31 landed checks compile live under G6 (`qor_pp_yes` defined nowhere, so `returncheck.h:19`'s `#if` reads `0 == 0` and `::TType` is the checked wrapper type).
- Deviation recorded during the leaf: the earlier `grep`-style census over-counted `CheckReturn` at 32 because the header comment at line 82 names the token in prose ("no Win32ErrorHandler and no CheckReturn here"); the census pattern is `CheckReturn<`, giving 31.

