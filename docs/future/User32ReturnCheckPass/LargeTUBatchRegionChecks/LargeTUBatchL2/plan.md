---
Type: Plan
Title: "Large TU batch L2"
Parent: "../plan.md"
Level: 2
Items:
  - Title: desktop sites
    Status: Refined
    Content: ./DesktopSites/plan.md
  - Title: clipboard sites
    Status: Refined
    Content: ./ClipboardSites/plan.md
  - Title: dpi sites
    Status: Refined
    Content: ./DpiSites/plan.md
  - Title: dialog sites
    Status: Refined
    Content: ./DialogSites/plan.md
  - Title: misc sites
    Status: Refined
    Content: ./MiscSites/plan.md
  - Title: message sites
    Status: Refined
    Content: ./MessageSites/plan.md
  - Title: painting sites
    Status: Refined
    Content: ./PaintingSites/plan.md
---

# Large TU batch L2

Slug: `LargeTUBatchL2` - from title "Large TU batch L2" (no drop-words, PascalCased, 14 chars, ASCII); the verdict side's folder name, so the fetch cut and the edit cut cover identical row sets. Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **153 sites** of these 7 large TUs, so that every leaf is one edit pass of at most 10 sites closing with its TU's re-census.

## Resume

Cursor: **terminal** - all 7 items `Refined` with children on disk, all 7 **CLOSED 12 Nov 2026**; all **153** L2 sites are in their end state. Item 7 **painting sites** is **CLOSED 12 Nov 2026** - `./PaintingSites/plan.md` is terminal at 3 closed `Atomic` items and `painting.cpp` re-censuses `rows=25 fcontext=25 useswinapi=25 (23 bare + 2 AW) LibraryCall=25 Win32ErrorHandler=25 CheckReturn-code=15 table-checked=15 table-unchecked=10 returncheck-includes=1 ok` (15 tokens = the TU's 15 `checked` rows, 25 policy blocks = 15 checked + 10 unchecked, standalone compile `TU_EXIT=0` with zero diagnostics at `/W4`, and its `registers/u2.md` tick written in the same session). Item 6 **message sites** is **CLOSED 12 Nov 2026** - `./MessageSites/plan.md` is terminal at 3 closed `Atomic` items and `message.cpp` re-censuses `rows=24 fcontext=24 useswinapi=24 (12 bare + 12 AW) LibraryCall=24 Win32ErrorHandler=24 CheckReturn-code=10 table-checked=10 table-unchecked=13 returncheck-includes=1 ok` (10 tokens, all 13 `unchecked` rows carry the contract's written reason in the site comment and no token, `PostQuitMessage` the `void` row carrying the one-line VOID reading instead; standalone compile `TU_EXIT=0` with a 1-line log, zero diagnostics at `/W4`). Item 5 **misc sites** is **CLOSED 12 Nov 2026** - `./MiscSites/plan.md` is terminal at 3 closed `Atomic` items and `misc.cpp` re-censuses `rows=23 fcontext=23 useswinapi=23 (22 bare + 1 AW) LibraryCall=23 Win32ErrorHandler=23 CheckReturn-code=13 table-checked=13 table-unchecked=9 returncheck-includes=1 ok` (13 tokens, the 9 `unchecked` rows carry the contract's written reason in the site comment and no token, the `void` row `SetDebugErrorLevel` carries neither; standalone compile `TU_EXIT=0`, log = `misc.cpp` only, zero diagnostics at `/W4`; the `misc.cpp` tick in `registers/u2.md` was written in the same session). Item 4 **dialog sites** is **CLOSED 12 Nov 2026** - `./DialogSites/plan.md` is terminal at 3 closed `Atomic` items and `dialog.cpp` re-censuses `rows=22 fcontext=22 useswinapi=22 (9 bare + 13 AW) LibraryCall=22 Win32ErrorHandler=22 CheckReturn-code=13 table-checked=13 table-unchecked=9 returncheck-includes=1 ok` (all 13 `checked` rows carry their token and all 9 `unchecked` rows carry their policy comment; 22 `Return policy (U2` blocks, braces balanced 26/26). Item 1 **desktop sites** is **CLOSED 12 Nov 2026** (`desktop.cpp` re-censuses `CheckReturn-code=15 table-checked=15 ok` over 2 slices). Item 2 **clipboard sites** is **CLOSED 12 Nov 2026** - `./ClipboardSites/plan.md` is terminal at 2 closed `Atomic` items and `clipboard.cpp` re-censuses `rows=20 fcontext=20 useswinapi=20 (18 bare + 2 AW) LibraryCall=20 Win32ErrorHandler=20 CheckReturn-code=10 table-checked=10 table-unchecked=10 returncheck-includes=1 ok` (163 insertions / 10 deletions against the U1 commit, the 10 deletions being the bare returns the pass replaced). Item 3 **dpi sites** is **CLOSED 12 Nov 2026** at `CheckReturn-code=6 table-checked=6 ok`. Terminal: all 7 items `Refined` with children on disk. **Hard precondition: these rows carry verdicts** (`EndpointReturnSemantics` subtree).

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the parent's (`../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, caller audit for nonzero counts, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: Large TU batch L1, Large TU batch L3, Window TU batch.

Measured by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt`: desktop 18 (2 slices), clipboard 20 (2), dpi 21 (3), dialog 22 (3), misc 23 (3), message 24 (3), painting 25 (3) = 153 rows / 19 slices. Every TU here exceeds the leaf budget, so all 7 get a child plan.

## desktop sites

18 sites in `desktop.cpp`, 2 slices. All 18 rows have 0 in-tree callers. Child plan: `./DesktopSites/plan.md` (Level 2 -> 3, 2 `Atomic` items, terminal).

## clipboard sites

20 sites in `clipboard.cpp`, 2 slices. Child plan: `./ClipboardSites/plan.md` (2 `Atomic` items, terminal).

## dpi sites

21 sites in `dpi.cpp`, 3 slices. Child plan: `./DpiSites/plan.md` (Level 2 -> 3, 3 `Atomic` items, terminal). **CLOSED 12 Nov 2026** - all 3 slices landed, the last being `SystemParametersInfoForDpi`, so `dpi.cpp` reads `CheckReturn-code=6 table-checked=6 ok` in `tmp/_tmp_u2_code_census.ps1`.

## dialog sites

22 sites in `dialog.cpp`, 3 slices. `GetDlgCtrlID` (**8** in-tree callers, the tree's heaviest nonzero row) sits in slice 1 - its token waits for the site-level read. Child plan: `./DialogSites/plan.md` (3 `Atomic` items, terminal).

## misc sites

23 sites in `misc.cpp`, 3 slices. Child plan: `./MiscSites/plan.md` (3 `Atomic` items, terminal).

## message sites

24 sites in `message.cpp`, 3 slices. `PostQuitMessage` (3 callers) sits in slice 2. Child plan: `./MessageSites/plan.md` (3 `Atomic` items, terminal).

## painting sites

25 sites in `painting.cpp`, 3 slices - the largest L2 TU, so it is last. Child plan: `./PaintingSites/plan.md` (3 `Atomic` items, terminal).
