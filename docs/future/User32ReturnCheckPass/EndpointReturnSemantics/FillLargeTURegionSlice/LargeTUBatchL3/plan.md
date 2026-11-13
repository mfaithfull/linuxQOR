---
Type: Plan
Title: "Large TU batch L3"
Parent: "../plan.md"
Level: 3
Items:
  - Title: pointer rows
    Status: Refined
    Content: ./PointerRows/plan.md
  - Title: keyboard rows
    Status: Refined
    Content: ./KeyboardRows/plan.md
  - Title: strings rows
    Status: Refined
    Content: ./StringsRows/plan.md
  - Title: menu rows
    Status: Refined
    Content: ./MenuRows/plan.md
---

# Large TU batch L3

Slug: `LargeTUBatchL3` - from title "Large TU batch L3" (no drop-words, PascalCased, 14 chars, ASCII); the sibling U1 tree's name. Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **139** table rows of the 4 densest TUs, every item one fetch batch of at most 10 URLs.

## Resume

Cursor: item 4 **menu rows** (`Refined`) -> descend into `./MenuRows/plan.md` - this plan's only remaining frontier. Item 3 **strings rows** is **CLOSED 12 Nov 2026** - `./StringsRows/plan.md` terminal at 4 closed `Atomic` items over 4 fetch batches of 10/10/10/4 URLs, all **34** rows of `strings.cpp` carrying the triple (`checked 10 / unchecked 24 / void 0`, `tmp/_tmp_u2_tu_state.ps1 strings.cpp` `rows=34 checked=10 unchecked=24 void=0 blank=0 shape_bad=0`, `CharToOem`/`OemToChar` family 8 rows `BoolCheck`, `LoadStringA`/`LoadStringW` `CheckNonZero< int >`, all six `IsChar*` unchecked, `wvsprintfA`/`wvsprintfW` unchecked on a runtime-length failure signal flagged to `CheckClassVocabulary`; folder audit `rows=597 filled=467 checked=268 unchecked=191 void=8 sum=467 class-shape violations=0`). Item 2 **keyboard rows** is **CLOSED 12 Nov 2026** - `./KeyboardRows/plan.md` terminal at 4 closed `Atomic` items over 4 fetch batches of 10/10/10/4 URLs, all **34** rows of `keyboard.cpp` carrying the triple (`checked 10 / unchecked 23 / void 1`, `tmp/_tmp_u2_tu_state.ps1 keyboard.cpp` `rows=34 checked=10 unchecked=23 void=1 blank=0 shape_bad=0`, `keybd_event` the void row, `GetKeyboardState` the SAL-annotated prototype whose page wording confirmed `checked` `BoolCheck`, folder audit `rows=597 filled=433 checked=258 unchecked=167 void=8 sum=433 class-shape violations=0`). Item 1 **pointer rows** is **CLOSED 12 Nov 2026** - `./PointerRows/plan.md` terminal at 4 closed `Atomic` items over 4 fetch batches of 10/10/10/2 URLs, all **32** rows of `pointer.cpp` carrying the triple (`checked 28 / unchecked 3 / void 1`, `tmp/_tmp_u2_tu_state.ps1 pointer.cpp` `rows=32 checked=28 unchecked=3 void=1 blank=0 shape_bad=0`; all 32 rows 0 in-tree callers, `DestroySyntheticPointerDevice` the `void` row, `RegisterPointerInputTargetEx`/`UnregisterPointerInputTargetEx` unchecked on a "Not supported." Return value section, `CreateSyntheticPointerDevice` the only non-`BoolCheck` check `CheckNonZeroHandle< HSYNTHETICPOINTERDEVICE >`; folder audit `rows=597 filled=399 checked=248 unchecked=144 void=7 sum=399 class-shape violations=0`). Items 1-3 are closed; item 4 (`menu rows`) is `Refined` with children on disk and untouched. Terminal: all 4 items `Refined` with children on disk.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the grandparent's (`../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: Large TU batch L1, Large TU batch L2.

Measured by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt`: pointer 32 (4 slices), keyboard 34 (4), strings 34 (4), menu 39 (4) = 139 rows / 16 fetch slices. `keyboard`'s `GetKeyboardState` is the library's only SAL-annotated prototype (`_Check_return_` at `winuser.h:5934`) - a hint, never a verdict.

## pointer rows

32 rows in 4 slices of at most 10. Child plan: `./PointerRows/plan.md` (4 `Atomic` items, terminal).

## keyboard rows

34 rows in 4 slices of at most 10. Child plan: `./KeyboardRows/plan.md` (4 `Atomic` items, terminal). Expect `GetKeyState`/`GetKeyboardState`-shaped answer rows to end `unchecked`.

## strings rows

34 rows in 4 slices of at most 10. Child plan: `./StringsRows/plan.md` (4 `Atomic` items, terminal). The `A`/`W` pairs (`CharLowerA`/`CharLowerW`, `OemToCharA`/`OemToCharW`) bind one bare stem each, so the `A` row's page covers the `W` row per D18 - but both rows still need their own verdict written.

## menu rows

39 rows in 4 slices of at most 10 - the largest TU in the library. Child plan: `./MenuRows/plan.md` (4 `Atomic` items, terminal).
