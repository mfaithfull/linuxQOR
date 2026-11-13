---
Type: Plan
Title: "windowclass sites"
Parent: "../plan.md"
Level: 3
Items:
  - Title: windowclass slice 1
    Status: Atomic
    Content:
  - Title: windowclass slice 2
    Status: Atomic
    Content:
---

# windowclass sites

Slug: `WindowclassSites` - from title "windowclass sites" (no drop-words, PascalCased, 16 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **14** `windowclass.cpp` sites in 2 slices of at most 10 sites.

## Resume

Cursor: **terminal** - all 2 items `Atomic`, both closed 12 Nov 2026. All 6 `checked` rows of `windowclass.cpp` carry the token; the final census `tmp/_tmp_u2_code_census.ps1` (TUS file `tmp/_tmp_u2_census_tus.txt` = `windowclass.cpp`, report `tmp/_tmp_u2_wc_census.out.txt`) reads `rows=14 fcontext=14 useswinapi=14 (2 bare + 12 AW) LibraryCall=14 Win32ErrorHandler=14 CheckReturn-code=6 table-checked=6 table-unchecked=8 returncheck-includes=1 ok`, `tus=1 fail=0`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: mouse sites, rectangle sites, scrollbar sites, icon sites, touch sites, cursor sites.

Slice boundaries fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM windowclass`. 12 of 14 rows are `NO-PROTOTYPE`, so their class base comes from the fetched page's documented SDK type, not the table's `NO-PROTOTYPE` column. `GetClassName` is already seeded `checked` - its site is not re-edited.

## windowclass slice 1

10 sites in `windowclass.cpp`: `GetClassInfo`, `GetClassInfoEx`, `GetClassLongPtr`, `GetClassName`, `GetClassWord`, `GetWindowLong`, `RegisterClass`, `RegisterClassEx`, `SetClassLongPtr`, `SetClassWord`. Land only the unseeded `checked` rows. Stop condition: re-read the sites, then re-census `windowclass.cpp` against its `checked` row count.

## windowclass slice 2

4 sites in `windowclass.cpp`: `SetWindowLong`, `SetWindowLongPtr`, `GetWindowLongPtr`, `UnregisterClass`. Stop condition: re-read the sites, then re-census `windowclass.cpp` - its counts must already match the slice-1 census.

## Cursors

windowclass slice 1: {"tu":"windowclass.cpp","slice_start":0,"slice_len":10,"done":true}
windowclass slice 2: {"tu":"windowclass.cpp","slice_start":10,"slice_len":4,"done":true}

## Closed 12 Nov 2026 - evidence

- Slice 1 landed 5 of its 10 sites - the only checked rows in the slice: `GetClassInfo` (`CheckReturn< BOOL, BoolCheck >`), `GetClassInfoEx` (`BOOL, BoolCheck`), `GetClassName` (`int, CheckNonZero< int >`), `RegisterClass` and `RegisterClassEx` (`ATOM, CheckNonZero< ATOM >`). The other 5 rows in the slice are `unchecked` and take no token (`GetClassLongPtr`, `GetClassWord`, `GetWindowLong`, `SetClassLongPtr`, `SetClassWord`).
- Slice 2 landed its 1 checked row, `UnregisterClass` (`BOOL, BoolCheck`); `SetWindowLong`, `SetWindowLongPtr`, `GetWindowLongPtr` are `unchecked` and untouched. Census after slice 2 matches the slice-1 census in every count except `CheckReturn-code` (5 -> 6), as the stop condition required.
- Include rule: one `#include "../returncheck.h"` at :12, added this pass. The TU's own conventions block records that the include was removed under T7 on 5 Nov 2026 because it was unused (`0 CheckReturn/BoolCheck/Win32ErrorHandler`); it is now live, and the census counts 1.
- Deviation from this leaf's `## Context`: it said `GetClassName` was "already seeded `checked` - its site is not re-edited". The census read `CheckReturn-code=0`, i.e. only the *table* row was seeded; the site carried no token, so it was landed like the others.
- Class bases: 12 of 14 rows are `NO-PROTOTYPE` in the table, so the base came from the published type per the landing contract - `GetClassName` -> `int`, `RegisterClass`/`RegisterClassEx` -> `ATOM` (`Dimm.h:258 typedef WORD ATOM`), the `GetClassInfo`/`GetClassInfoEx`/`UnregisterClass` trio -> `BOOL`. `GetClassWord`/`SetClassWord` are the only rows with an SDK prototype (`WORD`), and both are `unchecked`.
- Variable names follow the folder convention already in use (`bResult` BOOL, `iResult` int, `aResult` ATOM - matching `hResult` for handles in `icon.cpp` and `iResult` at `icon.cpp:259`/`:275`).
- All 6 checked rows report 0 in-tree callers, so every site is booked change-safe; each policy comment states the row's docQuote verbatim and the G6 liveness caveat.
- `GetClassWord` and `SetClassWord` report 1 in-tree caller each and stay `unchecked`, so no audit-before-landing marker is owed for them.

