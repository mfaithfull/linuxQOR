---
Type: Plan
Title: "rectangle rows"
Parent: "../plan.md"
Level: 4
Items:
  - Title: rectangle slice 1
    Status: Atomic
    Content:
  - Title: rectangle slice 2
    Status: Atomic
    Content:
---

# rectangle rows

Slug: `RectangleRows` - from title "rectangle rows" (no drop-words, PascalCased, 13 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **11** `rectangle.cpp` rows in 2 slices of at most 10 URLs, so no item exceeds one fetch batch.

## Resume

Cursor: terminal - both items `Atomic`, both **CLOSED 12 Nov 2026**. Item 1 **rectangle slice 1** filled rows 1-10 and item 2 **rectangle slice 2** filled row 11 (`UnionRect`), so all **11** `rectangle.cpp` rows carry the verdict triple and `tmp/_tmp_u2_filled.txt` reads `rectangle.cpp rows=11 filled=11 full`. Slice 1: **5 `checked` `BoolCheck`** (`CopyRect`, `InflateRect`, `OffsetRect`, `SetRect`, `SetRectEmpty` - the page reserves zero with "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero.") and **5 `unchecked`** (`EqualRect`, `IntersectRect`, `IsRectEmpty`, `PtInRect`, `SubtractRect` - there the zero is the test's own answer, e.g. "If the two rectangles are not identical, the return value is zero" / "If the resulting rectangle is empty, the return value is zero", so the session-27 test fails at that value). Slice 2: `UnionRect` **unchecked** - "If the specified structure does not contain a nonempty rectangle, the return value is zero" is the query's empty result, not a reserved failure. TU outcome **checked 5 / unchecked 6**; every row is `BOOL` / `BOOL` with **1** in-tree caller, so each note carries the caller count for the later code batch. Measured at this write: `tmp/_tmp_u2_patch.out.txt` `decided = 145`, `checked = 88 / unchecked = 53 / void = 4`, `class-shape violations = 0`, `decisions with no table row = 0`, **fail=0**; folder-wide `tus=53 rows=597 filled=145 full-tus=35 / partial=0 / empty=0`; `tmp/_tmp_u2_docquotes.txt` `138` rows.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: mouse rows, scrollbar rows, icon rows, touch rows, windowclass rows, cursor rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM rectangle`: 11 rows in slices of 10 and 1 in table order. This family is the answer-shaped `BOOL` region - `EqualRect`, `IsRectEmpty`, `PtInRect`, `IntersectRect`, `UnionRect`, `SubtractRect` answer FALSE or a real value legitimately, so expect those rows `unchecked` with the page's wording as the reason.

## rectangle slice 1

Rows 1-10: `CopyRect`, `EqualRect`, `InflateRect`, `IntersectRect`, `IsRectEmpty`, `OffsetRect`, `PtInRect`, `SetRect`, `SetRectEmpty`, `SubtractRect`. One fetch batch of 10, then write those 10 rows.

## rectangle slice 2

Row 11: `UnionRect`. One fetch, then write one row.
