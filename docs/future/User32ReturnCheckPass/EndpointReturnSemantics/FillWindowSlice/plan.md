---
Type: Plan
Title: "Fill the window slice"
Parent: "../plan.md"
Level: 2
Items:
  - Title: window BOOL rows
    Status: Refined
    Content: ./WindowBOOLRows/plan.md
  - Title: window HWND rows
    Status: Refined
    Content: ./WindowHWNDRows/plan.md
  - Title: window other rows
    Status: Refined
    Content: ./WindowOtherRows/plan.md
---

# Fill the window slice

Slug: `FillWindowSlice` - from title "Fill the window slice" (drop-word `the` removed, PascalCased, 15 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **86** table rows of `window.cpp`, the densest TU, cut by documented return family so every item is one fetch batch of at most 10 URLs.

## Resume

Cursor: **terminal** - all 3 items `Refined` with children on disk, all 3 **CLOSED 12 Nov 2026**. All **86** rows of `window.cpp` carry the verdict triple (`checked 50 / unchecked 35 / void 1`, `tmp/_tmp_u2_tu_state.ps1 window.cpp` `rows=86 checked=50 unchecked=35 void=1 blank=0 shape_bad=0`): item 1 **window BOOL rows** 55 rows (`checked 37 / unchecked 18 / void 0`), item 2 **window HWND rows** 17 rows (`checked 5 / unchecked 12 / void 0`), item 3 **window other rows** 14 rows (`checked 8 / unchecked 5 / void 1` - `./WindowOtherRows/plan.md` terminal at 2 closed `Atomic` items over fetch batches of 10/4 URLs). The folder single-pass audit `tmp/_tmp_u2_folder_audit.ps1` reads `rows=597 filled=592 checked=349 unchecked=234 void=9 sum=592 class-shape violations=0 filled rows with no docQuote=0`, the only remaining blanks (5) being the `accelerator.cpp` rows this plan's sibling item 4 owns.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the leaves obey is the parent's `## Decisions` - read it, do not restate it.** Sibling titles in the parent: Fill the small TU region slice, Fill the large TU region slice, Fill the accelerator rows, Audit the verdicts.

Measured by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` (recreate by slug): `window.cpp` = 86 rows, published-type mix **BOOL 55, HWND 17, UINT 4, int 3, WORD 3, HDWP 2, DWORD 1, void 1**; 11 rows are `NO-PROTOTYPE`; 34 rows have 0 in-tree callers. The parent's prose said "53 `BOOL` + 34 `HWND`" - the disk says 55/17, so the disk owns this file. One TU of 86 rows is not one item: it is cut by family (BOOL / HWND / the numeric-and-`void` rest) because the family decides the verdict shape, then cut into slices of 10 so no item exceeds one fetch batch (6 + 2 + 2 = 10 fetch slices).

## window BOOL rows

55 rows, 6 slices of at most 10. This is where the answer-shaped `BOOL` ("FALSE is an answer") rejections concentrate, so most rows end `unchecked` with the page's wording as the reason. Child plan: `./WindowBOOLRows/plan.md` (Level 2 -> 3, 6 `Atomic` items, terminal).

## window HWND rows

17 rows, 2 slices. Here NULL is a documented failure and also a legal answer for the lookup-shaped APIs (`WindowFromPoint`, `GetLastActivePopup`), so the page's exact wording decides `checked` vs `unchecked` per row. Child plan: `./WindowHWNDRows/plan.md` (2 `Atomic` items, terminal).

## window other rows

14 rows (`UINT` 4, `int` 3, `WORD` 3, `HDWP` 2, `DWORD` 1, `void` 1), 2 slices. The `void` row is settled by D51's rule with no fetch. Child plan: `./WindowOtherRows/plan.md` (2 `Atomic` items, terminal).
