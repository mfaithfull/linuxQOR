---
Type: Plan
Title: "scrollbar sites"
Parent: "../plan.md"
Level: 3
Items:
  - Title: scrollbar slice 1
    Status: Atomic
    Content:
  - Title: scrollbar slice 2
    Status: Atomic
    Content:
---

# scrollbar sites

Slug: `ScrollbarSites` - from title "scrollbar sites" (no drop-words, PascalCased, 14 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **12** `scrollbar.cpp` sites in 2 slices of at most 10 sites.

## Resume

Cursor: **terminal** - both items `Atomic`, both closed 12 Nov 2026. All **12** sites decided: **7 carry the token** (6 `CheckReturn< BOOL, BoolCheck >::TType` over `GetScrollBarInfo`, `GetScrollRange`, `ScrollDC`, `ScrollWindow`, `SetScrollRange`, `ShowScrollBar` plus `CheckReturn< int, CheckNonZero< int > >::TType` over `ScrollWindowEx`) and **5 stay token-free** (`EnableScrollBar`, `GetScrollInfo`, `GetScrollPos`, `SetScrollInfo`, `SetScrollPos`); per-TU re-census `tmp/_tmp_u2_code_census.ps1 scrollbar.cpp` `rows=12 fcontext=12 useswinapi=12 (12 bare + 0 AW) LibraryCall=12 Win32ErrorHandler=12 CheckReturn-code=7 table-checked=7 table-unchecked=5 returncheck-includes=1 ok`, `tus=1 fail=0`; folder-wide live total 114 of 597 (`tmp/_tmp_u2_live_checks.txt` convention).

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: mouse sites, rectangle sites, icon sites, touch sites, windowclass sites, cursor sites.

Slice boundaries fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM scrollbar`. `GetScrollPos`/`GetScrollRange` counts answer 0 legitimately - expected `unchecked` with the page's wording.

## scrollbar slice 1

10 sites in `scrollbar.cpp`: `EnableScrollBar`, `GetScrollBarInfo`, `GetScrollInfo`, `GetScrollPos`, `GetScrollRange`, `ScrollDC`, `ScrollWindow`, `ScrollWindowEx`, `SetScrollInfo`, `SetScrollPos`. Land only the `checked` rows. Stop condition: re-read the sites, then re-census `scrollbar.cpp` against its `checked` row count.

## scrollbar slice 2

2 sites in `scrollbar.cpp`: `SetScrollRange`, `ShowScrollBar`. Stop condition: re-read the sites, then re-census `scrollbar.cpp` - its counts must already match the slice-1 census.

## Cursors

scrollbar slice 1: {"tu":"scrollbar.cpp","slice_start":0,"slice_len":10,"done":true}
scrollbar slice 2: {"tu":"scrollbar.cpp","slice_start":10,"slice_len":2,"done":true}

## Closed 12 Nov 2026 - evidence

- Slice 1 landed the 5 token sites of its 10 (`GetScrollBarInfo`, `GetScrollRange`, `ScrollDC`, `ScrollWindow`, `ScrollWindowEx`) and kept the 5 unchecked rows token-free; slice 2 landed the remaining 2 (`SetScrollRange`, `ShowScrollBar`), each spelled `CheckReturn< sdkRetType, Check >::TType` over the existing `Library::Call< >`.
- Slice 2 stop condition met: the census after slice 2 reads the same counts as after slice 1 (`CheckReturn-code=7 table-checked=7 returncheck-includes=1 ok`) - the two slice-2 sites were already counted, so no drift.
- Caller audit before landing (`tmp/_tmp_u2_scroll_callers.ps1`): exactly 3 of the 7 checked rows have live in-tree callers - `ui/gui/controllers/s_s/scrollbarcontroller.cpp:67` `GetScrollRange`, `:82` `SetScrollRange`, `:87` `ShowScrollBar`, each `return User32::<stem>(...)` inside a `bool` `ScrollBarController` facade, so landing `BoolCheck` replaces a `FALSE` those facades used to return with a throw - the behaviour change D51 accepts for a row whose page reserves zero for failure. `GetScrollBarInfo`, `ScrollDC`, `ScrollWindow`, `ScrollWindowEx` have 0 in-tree callers (published surface, D14).
- Deviation from this leaf's `## Context`, which predicted `GetScrollPos`/`GetScrollRange` both `unchecked`: measured, only `GetScrollPos` is unchecked ("the return value is the current position of the scroll box ... If the function fails, the return value is zero" - zero is both the failure value and a legal position). `GetScrollRange` is `checked`: its return is `BOOL` status, not the range, because the range comes out through the `lpMinPos`/`lpMaxPos` out-parameters, so its page line is the plain "If the function succeeds, the return value is nonzero. If the function fails, the return value is zero." - the same line the `AddClipboardFormatListener` row takes `BoolCheck` for.
- `ScrollWindowEx` is the TU's only non-`BoolCheck` token: `CheckNonZero< int >`, sound because the installed `wingdi.h` region-flag block defines `ERROR 0` next to `NULLREGION 1` / `SIMPLEREGION 2` / `COMPLEXREGION 3`, so the documented failure value is 0 and every legal answer is nonzero. This corrects the older session-30 register note, which read the same row as "0 is not an error" and barred a check there; the table pass measured the header rather than the prose.
- `EnableScrollBar` is unchecked on the page putting a legal no-change outcome on the same value as an error: "If the arrows are already in the requested state or an error occurs, the return value is zero."
- No gate re-run was owed for this TU: it is a `qor_pp_useswinapi` TU already in the build log and the folder gate was last run at `srcs=54 not-in-log=0 stale=0 real-diag-lines=0`; the per-TU re-census is this leaf's stop condition and reads `ok`.

