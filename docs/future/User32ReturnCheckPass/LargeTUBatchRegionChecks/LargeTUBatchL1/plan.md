---
Type: Plan
Title: "Large TU batch L1"
Parent: "../plan.md"
Level: 2
Items:
  - Title: mouse sites
    Status: Atomic
    Content:
  - Title: rectangle sites
    Status: Refined
    Content: ./RectangleSites/plan.md
  - Title: scrollbar sites
    Status: Refined
    Content: ./ScrollbarSites/plan.md
  - Title: icon sites
    Status: Refined
    Content: ./IconSites/plan.md
  - Title: touch sites
    Status: Refined
    Content: ./TouchSites/plan.md
  - Title: windowclass sites
    Status: Refined
    Content: ./WindowclassSites/plan.md
  - Title: cursor sites
    Status: Refined
    Content: ./CursorSites/plan.md
---

# Large TU batch L1

Slug: `LargeTUBatchL1` - from title "Large TU batch L1" (no drop-words, PascalCased, 14 chars, ASCII); the verdict side's `LargeTUBatchL1` folder name, so the fetch cut and the edit cut cover identical row sets. Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **90 sites** of these 7 large TUs, so that every leaf is one edit pass of at most 10 sites closing with its TU's re-census.

## Resume

Cursor: **terminal** - all 7 items closed 12 Nov 2026. Item 7 **cursor sites** is **CLOSED 12 Nov 2026** - `./CursorSites/plan.md` terminal at 2 closed `Atomic` items; all **12** `checked` of the **15** `cursor.cpp` sites carry the token (slice 1: 9 - `ClipCursor`, `GetClipCursor`, `GetCursorInfo`, `GetCursorPos`, `GetPhysicalCursorPos` at `BOOL, BoolCheck`, `CreateCursor`, `LoadCursor`, `LoadCursorFromFile` at `HCURSOR, CheckNonZeroHandle< HCURSOR >`, `DestroyCursor` at `BOOL, BoolCheck`; slice 2: 3 - `SetCursorPos`, `SetPhysicalCursorPos`, `SetSystemCursor` at `BOOL, BoolCheck`) and the 3 `unchecked` sites (`GetCursor`, `ShowCursor`, `SetCursor`) each carry a written reason at the site and keep the bare `return Library::Call< ... >`; one `../returncheck.h` include; re-census `cursor.cpp rows=15 fcontext=15 useswinapi=15 (13 bare + 2 AW) LibraryCall=15 Win32ErrorHandler=15 CheckReturn-code=12 table-checked=12 table-unchecked=3 returncheck-includes=1 ok`; the layer-wide tally at this closure is `tus_with_checks=38 code_sites=157 tus_with_returncheck_include=38` (`tmp/_tmp_u2_live_checks.ps1`). Item 6 **windowclass sites** is **CLOSED 12 Nov 2026** - `./WindowclassSites/plan.md` terminal at 2 closed `Atomic` items; all **6** `checked` of the **14** `windowclass.cpp` sites carry the token (5 in slice 1: `GetClassInfo`, `GetClassInfoEx` at `BOOL, BoolCheck`, `GetClassName` at `int, CheckNonZero< int >`, `RegisterClass` and `RegisterClassEx` at `ATOM, CheckNonZero< ATOM >`; 1 in slice 2: `UnregisterClass` at `BOOL, BoolCheck`), the 8 `unchecked` sites keep the bare `return Library::Call< ... >`; one `../returncheck.h` include at :12; re-census `windowclass.cpp rows=14 fcontext=14 useswinapi=14 (2 bare + 12 AW) LibraryCall=14 Win32ErrorHandler=14 CheckReturn-code=6 table-checked=6 table-unchecked=8 returncheck-includes=1 ok` with `tus=1 fail=0` (`tmp/_tmp_u2_wc_census.out.txt`); the leaf records that `GetClassName` was seeded in the table only (`CheckReturn-code=0` at the leaf's start), so its site was landed like the others. Item 5 **touch sites** is **CLOSED 12 Nov 2026** - `./TouchSites/plan.md` terminal at 2 closed `Atomic` items; all **12** `checked` of the **14** `touch.cpp` sites carry `CheckReturn< BOOL, BoolCheck >::TType` (8 in slice 1, 4 in slice 2) and the 2 `unchecked` sites (`IsTouchWindow`, `PackTouchHitTestingProximityEvaluation`) keep the bare `return Library::Call< ... >`; one `../returncheck.h` include at `touch.cpp:107`; re-census `touch.cpp rows=14 fcontext=14 useswinapi=14 (14 bare + 0 AW) LibraryCall=14 Win32ErrorHandler=14 CheckReturn-code=12 table-checked=12 table-unchecked=2 returncheck-includes=1 ok`; the layer-wide tally at this closure is `tus_with_checks=36 code_sites=139 tus_with_returncheck_include=36` (`tmp/_tmp_u2_live_checks.ps1`). Item 4 **icon sites** is **CLOSED 12 Nov 2026** - all **14** sites of `icon.cpp` decided from the table's verdict columns: **13 carry the token** (6 `CheckReturn< HICON, CheckNonZeroHandle< HICON > >::TType` over `CopyIcon`, `CreateIcon`, `CreateIconFromResource`, `CreateIconFromResourceEx`, `CreateIconIndirect`, `LoadIconT`, 5 `CheckReturn< BOOL, BoolCheck >::TType` over `DestroyIcon`, `DrawIcon`, `DrawIconEx`, `GetIconInfo`, `GetIconInfoExT`, 2 `CheckReturn< int, CheckNonZero< int > >::TType` over `LookupIconIdFromDirectory`, `LookupIconIdFromDirectoryEx`) and **1 stayed token-free** (`PrivateExtractIconsT`, the TU's only unchecked row - its page names no failure value); `icon.cpp` gained its one `../returncheck.h` include, `CopyIcon`'s row reported 6 in-tree callers and the site-level read found **3 live sites** (`ui/gui/gdiobjects/cursor.cpp:91` in `Cursor::Clone()`, the `Icon(const Icon&)` copy ctor at `ui/gui/gdiobjects/icon.cpp:18`, `Icon::Clone()` at `icon.cpp:102`, each wrapping the handle in a `PrimitiveHandle`), audited before its token moved; its re-census reads `icon.cpp rows=14 fcontext=14 useswinapi=14 (11 bare + 3 AW) LibraryCall=14 Win32ErrorHandler=14 CheckReturn-code=13 table-checked=13 table-unchecked=1 returncheck-includes=1 ok` at `tus=1 fail=0`, and the layer-wide tally at this closure is `tus_with_checks=35 code_sites=127 tus_with_returncheck_include=35` (`tmp/_tmp_u2_live_checks.ps1`). Item 3 **scrollbar sites** is **CLOSED 12 Nov 2026** - all **12** sites of `scrollbar.cpp` decided from the table's verdict columns: **7 carry the token** (6 `CheckReturn< BOOL, BoolCheck >::TType` over `GetScrollBarInfo`, `GetScrollRange`, `ScrollDC`, `ScrollWindow`, `SetScrollRange`, `ShowScrollBar` - each page line the plain status form with the answer carried by an out-parameter - plus `CheckReturn< int, CheckNonZero< int > >::TType` over `ScrollWindowEx`, sound because the installed `wingdi.h` region-flag block defines `ERROR 0` next to `NULLREGION 1` / `SIMPLEREGION 2` / `COMPLEXREGION 3`, which corrects the session-30 register row that read the same site as "0 is not an error") and **5 stay token-free** with a written reason at the site (`EnableScrollBar` - the page puts a legal no-change outcome on the same value as an error; `GetScrollInfo`, `GetScrollPos`, `SetScrollInfo`, `SetScrollPos` - each hands its answer to its caller and zero is a legal value); the TU gained its one `../returncheck.h` include (0 -> 1), caller audit ran before landing (`tmp/_tmp_u2_scroll_callers.ps1` - exactly 3 of the 7 checked rows have live in-tree callers at `ui/gui/controllers/s_s/scrollbarcontroller.cpp:67` `:82` `:87`, inside `bool` facades, so the landed `BoolCheck` replaces a `FALSE` they used to return with a throw), and its re-census reads `scrollbar.cpp rows=12 fcontext=12 useswinapi=12 (12 bare + 0 AW) LibraryCall=12 Win32ErrorHandler=12 CheckReturn-code=7 table-checked=7 table-unchecked=5 returncheck-includes=1 ok` at `tus=1 fail=0`. Item 2 **rectangle sites** is **CLOSED 12 Nov 2026** - all **11** sites of `rectangle.cpp` decided from the table's verdict columns: **5 carry the token** (`CheckReturn< BOOL, BoolCheck >::TType` over `CopyRect`, `InflateRect`, `OffsetRect`, `SetRect`, `SetRectEmpty` - the only plain-status rows in this TU) and **6 stayed `unchecked` with a written reason at the site** (`EqualRect`, `IntersectRect`, `IsRectEmpty`, `PtInRect`, `SubtractRect`, `UnionRect` - the whole answer-shaped BOOL family this TU's session-28 register entry quantified, each page stating the BOOL as the answer to the caller's question about the RECTs it holds, so zero is a legal documented outcome and the page names no failure value at all); `rectangle.cpp` gained its one `../returncheck.h` include (0 -> 1), all 11 sites carry a policy comment, and its re-census reads `rectangle.cpp rows=11 fcontext=11 useswinapi=11 (11 bare + 0 AW) LibraryCall=11 Win32ErrorHandler=11 CheckReturn-code=5 table-checked=5 table-unchecked=6 returncheck-includes=1 ok` at `tus=1 fail=0`; the register entry is `src/platform/os/windows/api_layer/user/registers/u2.md` and the layer-wide tally at that closure was `tus_with_checks=33 code_sites=107 tus_with_returncheck_include=33` (`tmp/_tmp_u2_live_checks.ps1`; now 34 / 114 / 34 after the scrollbar leaf). Item 1 **mouse sites** is **CLOSED 12 Nov 2026** - all **10** sites of `mouse.cpp` decided from the table's verdict columns: **4 carry the token** (1 `CheckReturn< int, TCheckFailureValue< int, -1 > >::TType` over `GetMouseMovePointsEx`, 2 `CheckReturn< BOOL, BoolCheck >::TType` over `ReleaseCapture`/`SetDoubleClickTime`, 1 `CheckReturn< BOOL, BoolCheck >::TType` over `TrackMouseEvent`), **5 stayed `unchecked`** with the written reason at the site (`DragDetect`, `GetCapture`, `SetCapture`, `GetDoubleClickTime`, `SwapMouseButton`) and `mouse_event` is the TU's `void` row; `mouse.cpp` gained its one `../returncheck.h` include (0 -> 1) and its re-census reads `mouse.cpp rows=10 fcontext=10 useswinapi=10 (10 bare + 0 AW) LibraryCall=10 Win32ErrorHandler=10 CheckReturn-code=4 table-checked=4 table-unchecked=5 returncheck-includes=1 ok` at `tus=1 fail=0`. Terminal: all 7 items closed 12 Nov 2026; this plan is terminal and its **90** sites are all in their end state.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the parent's (`../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, caller audit for nonzero counts, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: Large TU batch L2, Large TU batch L3, Window TU batch.

Measured by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt`: mouse 10 (1 slice), rectangle 11 (2), scrollbar 12 (2), icon 14 (2), touch 14 (2), windowclass 14 (2), cursor 15 (2) = 90 rows / 13 slices. `mouse` at exactly 10 is one leaf; every TU above 10 is not.

## mouse sites

10 sites in `mouse.cpp`: `DragDetect`, `GetCapture`, `GetDoubleClickTime`, `GetMouseMovePointsEx`, `mouse_event`, `ReleaseCapture`, `SetCapture`, `SetDoubleClickTime`, `SwapMouseButton`, `TrackMouseEvent`. Land the `checked` rows only; `TrackMouseEvent`/`GetMouseMovePointsEx` answer FALSE/0 legitimately and stay `unchecked` with the page's wording. Stop condition: re-read the sites, then re-census `mouse.cpp` against its `checked` row count.

## rectangle sites

11 sites in `rectangle.cpp` - over the leaf budget, so it gets its own child plan. Child plan: `./RectangleSites/plan.md` (Level 2 -> 3, 2 `Atomic` items, terminal).

## scrollbar sites

12 sites in `scrollbar.cpp` - over the leaf budget, own child plan. Child plan: `./ScrollbarSites/plan.md` (2 `Atomic` items, terminal).

## icon sites

14 sites in `icon.cpp` - over the leaf budget, own child plan. Child plan: `./IconSites/plan.md` (2 `Atomic` items, terminal). `CopyIcon` carries **6** in-tree callers - its token waits for the site-level read.

## touch sites

14 sites in `touch.cpp` - over the leaf budget, own child plan. Child plan: `./TouchSites/plan.md` (2 `Atomic` items, terminal).

## windowclass sites

14 sites in `windowclass.cpp` - over the leaf budget, own child plan. Child plan: `./WindowclassSites/plan.md` (2 `Atomic` items, terminal). 12 of these rows are `NO-PROTOTYPE`, so their class base comes from the fetched page's documented SDK type.

## cursor sites

15 sites in `cursor.cpp` - over the leaf budget, own child plan. Child plan: `./CursorSites/plan.md` (2 `Atomic` items, terminal).
