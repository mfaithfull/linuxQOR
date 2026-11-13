---
Type: Plan
Title: "Large TU batch L1"
Parent: "../plan.md"
Level: 3
Items:
  - Title: mouse rows
    Status: Atomic
    Content:
  - Title: rectangle rows
    Status: Refined
    Content: ./RectangleRows/plan.md
  - Title: scrollbar rows
    Status: Refined
    Content: ./ScrollbarRows/plan.md
  - Title: icon rows
    Status: Refined
    Content: ./IconRows/plan.md
  - Title: touch rows
    Status: Refined
    Content: ./TouchRows/plan.md
  - Title: windowclass rows
    Status: Refined
    Content: ./WindowclassRows/plan.md
  - Title: cursor rows
    Status: Refined
    Content: ./CursorRows/plan.md
---

# Large TU batch L1

Slug: `LargeTUBatchL1` - from title "Large TU batch L1" (no drop-words, PascalCased, 14 chars, ASCII); the sibling U1 tree's name, kept identical so the two passes re-census against one another. Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **90** table rows of these 7 large TUs, so that every item is one fetch batch of at most 10 URLs - which means a TU of 11+ rows gets its own child plan.

## Resume

Cursor: item 1 **mouse rows** is **CLOSED 12 Nov 2026** - all **10** rows of `mouse.cpp` carry the verdict triple (`tmp/_tmp_u2_filled.txt`: `mouse.cpp rows=10 filled=10 full`, folder-wide `tus=53 rows=597 filled=134 full-tus=34 / partial=0 / empty=0`; `tmp/_tmp_u2_patch.out.txt`: `decided = 134`, `checked = 83 / unchecked = 47 / void = 4`, `class-shape violations = 0`, `decisions with no table row = 0`, **fail=0**; `tmp/_tmp_u2_docquotes.txt` `127` rows). The page's wording overrode this item's pre-fetch expectation: `GetMouseMovePointsEx` reserves **-1** ("Otherwise, the function returns -1") where the success answer is a point count, so it is `checked` with `TCheckFailureValue< int, -1 >`, and `TrackMouseEvent`'s section is a plain status BOOL ("If the function succeeds, the return value is nonzero . If the function fails, return value is zero") so it is `checked` `BoolCheck` - the page is the authority and the prose was a guess. Mouse outcome: **checked 4 / unchecked 5 / void 1**. Item 3 **scrollbar rows** is **CLOSED 12 Nov 2026** (all **12** `scrollbar.cpp` rows decided: **checked 7 / unchecked 5** - `GetScrollBarInfo`, `GetScrollRange`, `ScrollDC`, `ScrollWindow`, `SetScrollRange`, `ShowScrollBar` are `checked` `BoolCheck` on the plain BOOL status wording over `BOOL` / `BOOL`, `ScrollWindowEx` is `checked` `CheckNonZero< int >` on the `wingdi.h` `ERROR` failure answer, and the 5 `unchecked` rows (`EnableScrollBar`, `GetScrollInfo`, `GetScrollPos`, `SetScrollInfo`, `SetScrollPos`) answer 0 legitimately with the page's wording as the written reason; `tmp/_tmp_u2_filled.txt` `scrollbar.cpp rows=12 filled=12 full`; its child plan `./ScrollbarRows/plan.md` is terminal at 2 `Atomic` items). Item 5 **touch rows** is **CLOSED 12 Nov 2026** (all **14** `touch.cpp` rows decided **checked 12 / unchecked 2 / void 0** across 2 fetch batches of 10 and 4 URLs, every `Return value` section `match-cache` against `tmp/_tmp_u2_probe_cache.txt`; the 12 `checked` rows are `BoolCheck` on the plain status BOOL wording over `BOOL` `sdkRetType`, the 2 `unchecked` rows are `IsTouchWindow` (the BOOL answers the yes/no question the call asks) and `PackTouchHitTestingProximityEvaluation` (the `LRESULT` packs a score and a 0 score is legitimate); `tmp/_tmp_u2_tu_state.ps1 touch.cpp` -> `tu=touch.cpp rows=14 checked=12 unchecked=2 void=0 blank=0 shape_bad=0`; its child plan `./TouchRows/plan.md` is terminal at 2 `Atomic` items). Cursor: item 6 **windowclass rows** (`Refined`) -> descend into `./WindowclassRows/plan.md`. Item 4 **icon rows** is **CLOSED 12 Nov 2026** (all **14** `icon.cpp` rows decided **checked 13 / unchecked 1 / void 0** across 2 fetch batches; its child `./IconRows/plan.md` is terminal at 2 `Atomic` items. Batch 1 - item 1 **icon slice 1** **CLOSED 12 Nov 2026** (all **10** rows decided **checked 10 / unchecked 0 / void 0** on one 10-URL batch (`tmp/_tmp_u2_icon_fetch.out.txt` `fetched=10 of 10 / bad=0 / VERDICT fail=0`, every section `match-cache`): the 5 handle rows `CopyIcon`, `CreateIcon`, `CreateIconFromResource`, `CreateIconFromResourceEx`, `CreateIconIndirect` are `checked` `CheckNonZeroHandle< HICON >` on the pages' reserved NULL, the 5 status rows `DestroyIcon`, `DrawIcon`, `DrawIconEx`, `GetIconInfo`, `GetIconInfoEx` are `checked` `BoolCheck` on the plain BOOL status wording; `GetIconInfoEx` is an AW row read off the corrected `a`-slug page `nf-winuser-geticoninfoexa`). Batch 2 - item 2 **icon slice 2** **CLOSED 12 Nov 2026** (rows 11-14 on `tmp/_tmp_u2_icon_fetch_s2.out.txt` `fetched=4 of 4 / bad=0 / VERDICT fail=0`, every section `match-cache`, `LoadIcon` and `PrivateExtractIcons` fetched off the corrected `a`-slug pages `nf-winuser-loadicona` / `nf-winuser-privateextracticonsa`: `LoadIcon` `checked` `CheckNonZeroHandle< HICON >` on "If the function fails, the return value is NULL" - the class base is the published `HICON` because `sdkRetType` is NO-PROTOTYPE; `LookupIconIdFromDirectory` and `LookupIconIdFromDirectoryEx` `checked` `CheckNonZero< int >` on "If the function fails, the return value is zero" where the answer is an `RT_ICON`/`RT_CURSOR` resource identifier; `PrivateExtractIcons` `unchecked` because the page reserves 0 and 0xFFFFFFFF while 0 is also the legal count when nothing is extracted; `tmp/_tmp_u2_filled.txt` `icon.cpp rows=14 filled=14 full`, folder-wide `filled = 171` of `597` `full-tus=37 / partial=0 / empty=0`; `tmp/_tmp_u2_patch.out.txt` `decided = 171`, `checked = 108 / unchecked = 59 / void = 4`, `class-shape violations = 0`, `decisions with no table row = 0`, **fail=0**; `tmp/_tmp_u2_docquotes.txt` `164` rows). Item 2 **rectangle rows** is **CLOSED 12 Nov 2026** (all **11** `rectangle.cpp` rows decided: **checked 5 / unchecked 6** - the five `CopyRect`, `InflateRect`, `OffsetRect`, `SetRect`, `SetRectEmpty` rows are `checked` `BoolCheck` because the page reserves zero ("If the function succeeds, the return value is nonzero. If the function fails, the return value is zero."), while `EqualRect`, `IntersectRect`, `IsRectEmpty`, `PtInRect`, `SubtractRect`, `UnionRect` end `unchecked` because their zero is the test's or the query's own answer, e.g. "If the resulting rectangle is empty, the return value is zero"; every row `BOOL` / `BOOL` with 1 in-tree caller, so each note carries the count); its child plan `./RectangleRows/plan.md` is terminal at 2 `Atomic` items. Terminal: 1 item `Atomic`, 6 items `Refined` with children on disk. Item 6 **windowclass rows** is **CLOSED 12 Nov 2026**: all **14** `windowclass.cpp` rows decided across 2 fetch batches of 10 and 4 URLs (**checked 6 / unchecked 8 / void 0**) - `BoolCheck` on `GetClassInfo`, `GetClassInfoEx` and `UnregisterClass` (plain status BOOL, zero reserved, extended `GetLastError` info), `CheckNonZero< int >` on `GetClassName` (success returns "the number of characters copied to the buffer"), `CheckNonZero< ATOM >` on `RegisterClass`/`RegisterClassEx` (success returns "a class atom that uniquely identifies the class being registered", `Dimm.h:258 typedef WORD ATOM` since the rows are `NO-PROTOTYPE`); the 8 `unchecked` rows are the value-returning and previous-state family whose zero is a legal answer, the pages saying so outright - "If SetWindowLong has not been called previously, GetWindowLong returns zero for values in the extra window or class memory" and "If the previous value of the specified 32-bit integer is zero, and the function succeeds, the return value is zero, but the function does not clear the last error information. This makes it difficult to determine success or failure." - so no value is reserved for failure there. `tmp/_tmp_u2_tu_state.ps1 windowclass.cpp` re-reads `rows=14 checked=6 unchecked=8 void=0 blank=0 shape_bad=0`; its child plan `./WindowclassRows/plan.md` is terminal at 2 `Atomic` items. Folder-wide after that close: `tmp/_tmp_u2_filled.txt` `tus=53 rows=597 filled=199 full-tus=39 / partial=0 / empty=0`, `tmp/_tmp_u2_patch.out.txt` `filled-verdict = 199`, `checked = 126 / unchecked = 69 / void = 4`, `class-shape violations = 0`, `decisions with no table row = 0`, **fail=0**. Cursor: item 7 **cursor rows** is **CLOSED 12 Nov 2026** - all **15** rows of `cursor.cpp` carry the verdict triple (`tmp/_tmp_u2_tu_state.ps1 cursor.cpp`: `rows=15 checked=12 unchecked=3 void=0 blank=0 shape_bad=0`; folder-wide `tmp/_tmp_u2_filled.txt` `tus=53 rows=597 filled=214 full-tus=40 / partial=0 / empty=0`; `tmp/_tmp_u2_patch.out.txt` `filled-verdict = 214`, `decided = 214`, `checked = 138 / unchecked = 72 / void = 4`, `class-shape violations = 0`, `filled rows with no page = 7`, `decisions with no table row = 0`, **fail=0**; `tmp/_tmp_u2_docquotes.txt` `207` rows). Two fetch batches: slice 1 = 10 URLs (`tmp/_tmp_u2_cursor_fetch.out.txt` `fetched=10 of 10 bad=0 VERDICT fail=0`, 2 `CORRECTED` lines for the AW rows `LoadCursor`/`LoadCursorFromFile` on their `a`-slug pages), slice 2 = 5 URLs (`tmp/_tmp_u2_cursor_fetch_s2.out.txt` `fetched=5 of 5 bad=0 VERDICT fail=0`). The 12 `checked`: 9 `BoolCheck` on plain status BOOL wording (`ClipCursor`, `DestroyCursor`, `GetClipCursor`, `GetCursorInfo`, `GetCursorPos`, `GetPhysicalCursorPos`, `SetCursorPos`, `SetPhysicalCursorPos`, `SetSystemCursor`) and 3 `CheckNonZeroHandle< HCURSOR >` on pages that reserve NULL with extended error info (`CreateCursor`, `LoadCursor`, `LoadCursorFromFile`). The 3 `unchecked` answer NULL or a count legitimately: `GetCursor` ("If there is no cursor, the return value is NULL"), `SetCursor` ("If there was no previous cursor, the return value is NULL"), `ShowCursor` whose `int` is "the new display counter" and the page makes 0 reachable ("If a mouse is installed, the initial display count is 0"). Every row carries its 1 in-tree caller in `notes` for the audit before any code lands. Child plan `./CursorRows/plan.md` is terminal at 2 `Atomic` items. Terminal: all 7 items closed - 1 `Atomic` (mouse) + 6 `Refined` with terminal children.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the grandparent's (`../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: Large TU batch L2, Large TU batch L3.

Measured by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt`: mouse 10 (1 slice), rectangle 11 (2), scrollbar 12 (2), icon 14 (2), touch 14 (2), windowclass 14 (2), cursor 15 (2) = 90 rows / 13 fetch slices. `mouse` at exactly 10 rows is one item; every TU above 10 is not.

## mouse rows

10 rows: `DragDetect`, `GetCapture`, `GetDoubleClickTime`, `GetMouseMovePointsEx`, `mouse_event`, `ReleaseCapture`, `SetCapture`, `SetDoubleClickTime`, `SwapMouseButton`, `TrackMouseEvent`. One fetch batch of exactly 10, then write. `TrackMouseEvent`/`GetMouseMovePointsEx` answer FALSE/0 legitimately - expect `unchecked` rows with the page's wording as the reason.

## rectangle rows

11 rows - over the cap, so it gets its own child plan. Child plan: `./RectangleRows/plan.md` (Level 3 -> 4, 2 `Atomic` items, terminal). This family is the answer-shaped `BOOL` region (`EqualRect`, `IsRectEmpty`, `PtInRect`, `IntersectRect`, `UnionRect`) - expect a high `unchecked` ratio with a written reason per row.

## scrollbar rows

12 rows - over the cap, own child plan. Child plan: `./ScrollbarRows/plan.md` (2 `Atomic` items, terminal). `GetScrollPos`/`GetScrollRange`-shaped counts answer 0 legitimately.

## icon rows

14 rows - over the cap, own child plan. Child plan: `./IconRows/plan.md` (2 `Atomic` items, terminal).

## touch rows

14 rows - over the cap, own child plan. Child plan: `./TouchRows/plan.md` (2 `Atomic` items, terminal).

## windowclass rows

14 rows - over the cap, own child plan. Child plan: `./WindowclassRows/plan.md` (2 `Atomic` items, terminal). `GetClassName` (AW) is already seeded `checked` `CheckReturn< int, CheckNonZero< int > >` from a fetched page - its row still needs the quote stored for the audit.

## cursor rows

15 rows - the largest here, own child plan. Child plan: `./CursorRows/plan.md` (2 `Atomic` items, terminal).
