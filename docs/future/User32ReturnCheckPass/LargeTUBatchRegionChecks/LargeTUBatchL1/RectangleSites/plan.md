---
Type: Plan
Title: "rectangle sites"
Parent: "../plan.md"
Level: 3
Items:
  - Title: rectangle slice 1
    Status: Atomic
    Content:
  - Title: rectangle slice 2
    Status: Atomic
    Content:
---

# rectangle sites

Slug: `RectangleSites` - from title "rectangle sites" (no drop-words, PascalCased, 14 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **11** `rectangle.cpp` sites in 2 slices of at most 10 sites.

## Resume

Cursor: **terminal** - both items `Atomic`, both closed 12 Nov 2026. All **11** `rectangle.cpp` sites decided from the table's verdict columns: **5 carry the token** (`CheckReturn< BOOL, BoolCheck >::TType` over `CopyRect`, `InflateRect`, `OffsetRect`, `SetRect`, `SetRectEmpty`) and **6 stayed `unchecked` with a written reason at the site** (`EqualRect`, `IntersectRect`, `IsRectEmpty`, `PtInRect`, `SubtractRect`, `UnionRect`); `rectangle.cpp` gained its one `../returncheck.h` include (0 -> 1), all 11 sites carry a `//Return policy (U2, table row <stem> / rectangle.cpp, 12 Nov 2026)` comment, and the re-census reads `rectangle.cpp rows=11 fcontext=11 useswinapi=11 (11 bare + 0 AW) LibraryCall=11 Win32ErrorHandler=11 CheckReturn-code=5 table-checked=5 table-unchecked=6 returncheck-includes=1 ok` at `tus=1 fail=0`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: mouse sites, scrollbar sites, icon sites, touch sites, windowclass sites, cursor sites.

Slice boundaries fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM rectangle`. This family is the answer-shaped `BOOL` region - expect a high `unchecked` ratio with a written reason per row.

## rectangle slice 1

10 sites in `rectangle.cpp`: `CopyRect`, `EqualRect`, `InflateRect`, `IntersectRect`, `IsRectEmpty`, `OffsetRect`, `PtInRect`, `SetRect`, `SetRectEmpty`, `SubtractRect`. Land only the `checked` rows. Stop condition: re-read the sites, then re-census `rectangle.cpp` against its `checked` row count.

## rectangle slice 2

1 site in `rectangle.cpp`: `UnionRect`. Stop condition: re-read the site, then re-census `rectangle.cpp` - its counts must already match the slice-1 census.

## Closed 12 Nov 2026 - evidence

- Slice 1 (10 sites `CopyRect`..`SubtractRect`) and slice 2 (1 site `UnionRect`) both landed in one edit pass each; the slice-2 census is identical to the slice-1 census, which is this leaf's stop condition for slice 2 (`CheckReturn-code=5 == table-checked=5`, `returncheck-includes=1`, `ok` at `tus=1 fail=0`).
- Class base is the `sdkRetType` `BOOL` on all five token sites, so `BoolCheck` (`returncheck.h:94`) covers them and **no new check class was needed** - the whole TU is `BOOL`-returning (11/11 rows).
- The leaf's Context prediction ("a high `unchecked` ratio") measured 6/11, and every unchecked row is the answer-shaped BOOL this TU's session-28 register entry quantified: `IntersectRect`, `UnionRect` and `SubtractRect` are exactly the three rows that header block names as returning zero for a documented outcome, and `EqualRect` / `IsRectEmpty` / `PtInRect` are the same shape on their own pages ("If the two rectangles are identical...", "If the rectangle is empty...", "If the specified point lies within the rectangle...").
- Caller audit read at the site, not from the table's upper bound: all 11 rows report 1 in-tree caller, and `platform::win::Rect` in `ui/gui/view/drawing/rect.cpp` is that caller. The five checked rows are the five facades that read the BOOL as a success flag (`:37 CopyFrom`, `:47 Inflate`, `:64 Offset`, `:75 Set`, `:80 SetEmpty`, each `return User32::<stem>(...) ? true : false`), so landing the check replaces a FALSE those facades returned with a throw - the behaviour change D51 accepts for a row whose page reserves zero for failure. The three unchecked rows whose facades drop the BOOL outright (`:53 FromIntersection`, `:86 FromSubtraction`, `:93 FromUnion`) are exactly three of the six unchecked rows, so no live caller inspects a zero the check would have thrown on.
- The register entry is `src/platform/os/windows/api_layer/user/registers/u2.md`; the layer-wide tally after this leaf is `tus_with_checks=33 code_sites=107 tus_with_returncheck_include=33` (`tmp/_tmp_u2_live_checks.ps1`), and the verdict side is unchanged at 597/597 filled.

