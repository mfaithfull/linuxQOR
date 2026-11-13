---
Type: Plan
Title: "Large TU batch L3"
Parent: "../plan.md"
Level: 2
Items:
  - Title: pointer sites
    Status: Refined
    Content: ./PointerSites/plan.md
  - Title: keyboard sites
    Status: Refined
    Content: ./KeyboardSites/plan.md
  - Title: strings sites
    Status: Refined
    Content: ./StringsSites/plan.md
  - Title: menu sites
    Status: Refined
    Content: ./MenuSites/plan.md
---

# Large TU batch L3

Slug: `LargeTUBatchL3` - from title "Large TU batch L3" (no drop-words, PascalCased, 14 chars, ASCII); the verdict side's folder name, so the fetch cut and the edit cut cover identical row sets. Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **139 sites** of the 4 densest TUs, every leaf one edit pass of at most 10 sites closing with its TU's re-census.

## Resume

Cursor: **terminal** - all 4 items `Refined` with children on disk, all 4 closed 12–13 Nov 2026; the folder's 139 sites are fully in a verdict state. Item 4 **menu sites** is **CLOSED 13 Nov 2026** - `./MenuSites/plan.md` is terminal at all 4 closed `Atomic` slices, and `menu.cpp` landed **31** `CheckReturn< >` tokens at exactly its 31 `checked` rows plus **31** `//Return policy` blocks, leaving its 8 `unchecked` rows bare (`GetMenu`, `GetMenuCheckMarkDimensions`, `GetSubMenu`, `GetSystemMenu`, `HiliteMenuItem`, `IsMenu`, `TrackPopupMenu`, `TrackPopupMenuEx`) and carrying no `void` row; its census reads `lines=663 rows=39 fcontext=39 useswinapi=40 (30 bare + 10 AW; the 40th is a header comment) LibraryCall=39 Win32ErrorHandler=39 policy-blocks=31 CheckReturn-code=31 table-checked=31 table-unchecked=8 returncheck-includes=1 ok` and its standalone gate leg `tmp/_tmp_u2_one_tu.bat menu.cpp` reads `TU_EXIT=0 log_lines=1 diags=0 dating=obj-newer`; no new check class was needed. Item 3 **strings sites** is **CLOSED 12 Nov 2026** - `./StringsSites/plan.md` is terminal at all 4 closed `Atomic` slices, and `strings.cpp` landed **10** `CheckReturn< >` tokens at exactly its 10 `checked` rows plus **34** `//Return policy` blocks (the TU has no `void` row), its census reading `rows=34 fcontext=34 useswinapi=34 (34 bare + 0 AW) LibraryCall=34 Win32ErrorHandler=34 CheckReturn-code=10 table-checked=10 table-unchecked=24 returncheck-includes=1  ok` (`tus=1 fail=0`); no new check class was needed - the TU took `BoolCheck` (returncheck.h:94) on the 8 `CharToOem*` / `OemToChar*` rows and `CheckNonZero< int >` (returncheck.h:187) on `LoadStringA/W`. Item 2 **keyboard sites** is **CLOSED 12 Nov 2026** - `./KeyboardSites/plan.md` is terminal at all 4 closed `Atomic` slices, and `keyboard.cpp` landed **10** `CheckReturn< >` tokens at exactly its 10 `checked` rows plus 33 `//Return policy` blocks (the TU's single `void` row `keybd_event` carries neither), its census reading `rows=34 fcontext=34 useswinapi=34 (27 bare + 7 AW) LibraryCall=34 Win32ErrorHandler=34 CheckReturn-code=10 table-checked=10 table-unchecked=23 returncheck-includes=1  ok` (`tus=1 fail=0`); no new check class was needed. Item 1 **pointer sites** is **CLOSED 12 Nov 2026** - `./PointerSites/plan.md` is terminal at all 4 closed `Atomic` slices, and `pointer.cpp` landed **28** `CheckReturn< BOOL, BoolCheck >` tokens at exactly the TU's 28 `checked` rows, **3** reason comments at its 3 `unchecked` rows, and left its single `void` row (`DestroySyntheticPointerDevice`) bare; its per-TU census `tmp/_tmp_u2_code_census.ps1` reads `rows=32 fcontext=32 useswinapi=32 LibraryCall=32 Win32ErrorHandler=32 CheckReturn-code=28 table-checked=28 table-unchecked=3 returncheck-includes=1  ok` (`tus=1 fail=0`), and no new check class was needed. Terminal: all 4 items `Refined` with children on disk. **Hard precondition: these rows carry verdicts** (`EndpointReturnSemantics` subtree).

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the parent's (`../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, caller audit for nonzero counts, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: Large TU batch L1, Large TU batch L2, Window TU batch.

Measured by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt`: pointer 32 (4 slices), keyboard 34 (4), strings 34 (4), menu 39 (4) = 139 rows / 16 slices. Densest-last, so the folder's check shape is proven on cheaper TUs before these run.

## pointer sites

32 sites in `pointer.cpp`, 4 slices; all 32 rows have 0 in-tree callers. Child plan: `./PointerSites/plan.md` (Level 2 -> 3, 4 `Atomic` items, terminal).

## keyboard sites

34 sites in `keyboard.cpp`, 4 slices. `GetKeyboardState` is the library's only SAL-annotated prototype (`_Check_return_` at `winuser.h:5934`) - a hint, never a verdict. Child plan: `./KeyboardSites/plan.md` (4 `Atomic` items, terminal).

## strings sites

34 sites in `strings.cpp`, 4 slices. The `A`/`W` pairs bind one bare stem each; both rows keep their own verdict. Child plan: `./StringsSites/plan.md` (4 `Atomic` items, terminal).

## menu sites

39 sites in `menu.cpp`, 4 slices - the largest TU in the library, so it is last. Child plan: `./MenuSites/plan.md` (4 `Atomic` items, terminal).
