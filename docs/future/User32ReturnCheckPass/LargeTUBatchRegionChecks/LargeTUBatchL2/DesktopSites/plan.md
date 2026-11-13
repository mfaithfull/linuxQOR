---
Type: Plan
Title: "desktop sites"
Parent: "../plan.md"
Level: 3
Items:
  - Title: desktop slice 1
    Status: Atomic
    Content:
  - Title: desktop slice 2
    Status: Atomic
    Content:
---

# desktop sites

Slug: `DesktopSites` - from title "desktop sites" (no drop-words, PascalCased, 12 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **18** `desktop.cpp` sites in 2 slices of at most 10 sites.

## Resume

Cursor: **terminal** - both items `Atomic`, both closed 12 Nov 2026. All **18** `desktop.cpp` sites read; the **15** `checked` rows carry their token and the **3** `unchecked` rows carry their written reason; one `../returncheck.h` include. Item 1 **desktop slice 1** landed 8 tokens and 2 reasons; its census then read `CheckReturn-code=8 table-checked=15` (the 7-row gap slice 2 owed); after slice 2 the TU's re-census `tmp/_tmp_u2_code_census.ps1` reads `desktop.cpp rows=18 fcontext=18 useswinapi=18 (9 bare + 9 AW) LibraryCall=18 Win32ErrorHandler=18 CheckReturn-code=15 table-checked=15 table-unchecked=3 returncheck-includes=1 ok` - core chain exact and `15==15`, so the leaf's stop condition holds. Terminal: both items `Atomic`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: clipboard sites, dpi sites, dialog sites, misc sites, message sites, painting sites. All 18 rows here have 0 in-tree callers, so no caller audit gates this TU.

## desktop slice 1

10 sites in `desktop.cpp`: `OpenDesktop`, `CloseDesktop`, `CreateDesktop`, `CreateDesktopEx`, `EnumDesktops`, `CreateWindowStation`, `CloseWindowStation`, `EnumWindowStations`, `GetProcessWindowStation`, `GetThreadDesktop`. Land only the `checked` rows. Stop condition: re-read the sites, then re-census `desktop.cpp` against its `checked` row count.

## desktop slice 2

8 sites in `desktop.cpp`: `GetUserObjectInformation`, `OpenInputDesktop`, `OpenWindowStation`, `SetProcessWindowStation`, `SetThreadDesktop`, `SetUserObjectInformation`, `SwitchDesktop`, `EnumDesktopWindows`. Stop condition: re-read the sites, then re-census `desktop.cpp` - its counts must already match the slice-1 census.


## Closed 12 Nov 2026 - evidence

- Slice 1 (10 sites) landed 8 tokens; slice 2 (8 sites) landed 7 tokens and wrote 1 reason - `desktop.cpp` now carries **15** `CheckReturn` tokens against **15** `checked` table rows, one `../returncheck.h` include, and 3 written reasons.
- Class bases all from the row's `sdkRetType`, with the published type where that column is blank or `NO-PROTOTYPE`: `CheckNonZeroHandle< HDESK >` (`OpenDesktop`, `CreateDesktop`, `CreateDesktopEx`, `GetThreadDesktop`, `OpenInputDesktop`), `CheckNonZeroHandle< HWINSTA >` (`CreateWindowStation`, `GetProcessWindowStation`, `OpenWindowStation`), `BoolCheck` (the 7 BOOL status rows). All three classes already exist in `api_layer/returncheck.h` (`CheckNonZeroHandle` at :137, `BoolCheck` at :94) - no vocabulary item added by this leaf.
- The three blank-`sdkRetType` handle rows (`GetProcessWindowStation`, `GetThreadDesktop`, `OpenInputDesktop`) take the published base because the SDK declares those prototypes over separate lines; the three AW rows (`GetUserObjectInformation`, `OpenWindowStation`, `SetUserObjectInformation`) carry `NO-PROTOTYPE` and likewise take the published base - the established convention.
- All 18 rows report 0 in-tree callers, so no G6 caller audit gated this TU; every reason carries that count as the change-safe marker.
- The three `unchecked` rows are the callback-enumeration shape, recorded identically at their sites: `EnumDesktops`, `EnumWindowStations` (slice 1) and `EnumDesktopWindows` (slice 2) - each puts the stop and the error in the caller's own callback ("You must ensure that the callback function sets SetLastError if it fails" / the callback returning FALSE stops the enumeration), so a callback that stops early answers zero legally and keying `BoolCheck` would report a legal caller decision as a failure.
- `SwitchDesktop` is the one row whose page withholds extended error info on some failures ("However, SwitchDesktop only sets the last error for the following cases") - recorded as not making zero a legal answer, so the row stays checked.
