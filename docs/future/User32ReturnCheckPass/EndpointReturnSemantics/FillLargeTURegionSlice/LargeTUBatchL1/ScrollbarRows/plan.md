---
Type: Plan
Title: "scrollbar rows"
Parent: "../plan.md"
Level: 4
Items:
  - Title: scrollbar slice 1
    Status: Atomic
    Content:
  - Title: scrollbar slice 2
    Status: Atomic
    Content:
---

# scrollbar rows

Slug: `ScrollbarRows` - from title "scrollbar rows" (no drop-words, PascalCased, 13 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **12** `scrollbar.cpp` rows in 2 slices of at most 10 URLs.

## Resume

Cursor: terminal - both items **CLOSED 12 Nov 2026**. Item 1 **scrollbar slice 1** closed first: rows 1-10 decided **checked 5 / unchecked 5** after one fetch batch of 10 URLs (`tmp/_tmp_u2_sbr_probe.txt` - 12 `resolved` rows, every page `has-section`). `GetScrollBarInfo`, `GetScrollRange`, `ScrollDC`, `ScrollWindow` carry the plain BOOL status - "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero." - so they are `checked` `BoolCheck` over `BOOL` / `BOOL`; `ScrollWindowEx` is `checked` `CheckNonZero< int >` because the failure answer is `ERROR` and the installed `wingdi.h` region-flag block defines `ERROR` 0 next to `NULLREGION` 1 / `SIMPLEREGION` 2 / `COMPLEXREGION` 3, so every legal answer is nonzero and the session-27 test passes at 0. The 5 `unchecked` rows answer 0 legitimately: `EnableScrollBar` puts a legal no-change outcome on the same value as an error, `GetScrollInfo` answers "nothing retrieved", `GetScrollPos` names "0 through 100" as a legal range, `SetScrollInfo` documents no failure value at all, `SetScrollPos` answers the previous position. Item 2 **scrollbar slice 2** closed: the 2-URL fetch batch (`tmp/_tmp_u2_sb_ssr.html` http=200 / `tmp/_tmp_u2_sb_ssb.html` http=200, `tmp/_tmp_u2_sb_extract.out.txt` both `Return value` sections 182 chars) reads the same plain BOOL status for both, so `SetScrollRange` and `ShowScrollBar` are `checked` `BoolCheck` with `sdkRetType` BOOL, each with **1** in-tree caller (`scrollbarcontroller.cpp:82`, `:87`). Leaf outcome **checked 7 / unchecked 5 / void 0**; re-census `tmp/_tmp_u2_filled.txt` `scrollbar.cpp rows=12 filled=12 full`, folder-wide `filled = 157` of `597` `full-tus=36 / partial=0 / empty=0`, `tmp/_tmp_u2_patch.out.txt` `decided = 157`, `checked = 95 / unchecked = 58 / void = 4`, `class-shape violations = 0`, `decisions with no table row = 0`, **fail=0**, `tmp/_tmp_u2_docquotes.txt` `150` rows. Terminal: 2 items `Atomic`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: mouse rows, rectangle rows, icon rows, touch rows, windowclass rows, cursor rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM scrollbar`: 12 rows in slices of 10 and 2 in table order. `GetScrollPos`/`GetScrollRange`/`GetScrollInfo`-shaped counts answer 0 legitimately - the root plan names `GetScrollPos` as an expected `unchecked` row.

## scrollbar slice 1

Rows 1-10: `EnableScrollBar`, `GetScrollBarInfo`, `GetScrollInfo`, `GetScrollPos`, `GetScrollRange`, `ScrollDC`, `ScrollWindow`, `ScrollWindowEx`, `SetScrollInfo`, `SetScrollPos`. One fetch batch of 10, then write those 10 rows.

## scrollbar slice 2

Rows 11-12: `SetScrollRange`, `ShowScrollBar`. One fetch batch of 2, then write those 2 rows.
