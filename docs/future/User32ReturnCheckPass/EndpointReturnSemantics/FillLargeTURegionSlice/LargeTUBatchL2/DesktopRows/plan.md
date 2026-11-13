---
Type: Plan
Title: "desktop rows"
Parent: "../plan.md"
Level: 4
Items:
  - Title: desktop slice 1
    Status: Atomic
    Content:
  - Title: desktop slice 2
    Status: Atomic
    Content:
---

# desktop rows

Slug: `DesktopRows` - from title "desktop rows" (no drop-words, PascalCased, 11 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Fill the verdict triple for the **18** `desktop.cpp` rows in 2 slices of at most 10 URLs.

## Resume

This plan is **terminal** - both items `Atomic` and both **CLOSED 12 Nov 2026**. Item 1 **desktop slice 1** is **CLOSED 12 Nov 2026**: rows 1-10 decided **checked 8 / unchecked 2 / void 0** over one fetch batch of 10 URLs at `VERDICT fail=0`. Item 2 **desktop slice 2** is **CLOSED 12 Nov 2026**: rows 11-18 (`GetUserObjectInformation`, `OpenInputDesktop`, `OpenWindowStation`, `SetProcessWindowStation`, `SetThreadDesktop`, `SetUserObjectInformation`, `SwitchDesktop`, `EnumDesktopWindows`) decided **checked 7 / unchecked 1 / void 0** over one fetch batch of 8 URLs, all 8 pages `match-cache`, `VERDICT fail=0`; the only unchecked row is `EnumDesktopWindows`, whose zero is the caller's own callback stop rather than a failure. The TU now reads `tmp/_tmp_u2_tu_state.ps1 desktop.cpp` `rows=18 checked=15 unchecked=3 void=0 blank=0 shape_bad=0` and `tmp/_tmp_u2_filled.txt` `desktop.cpp rows=18 filled=18 full`. Terminal: both items `Atomic`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes a landed check live on every call. **The slice contract the items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - read it before filling. Sibling titles in the parent: clipboard rows, dpi rows, dialog rows, misc rows, message rows, painting rows.

Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM desktop`: 18 rows in slices of 10 and 8 in table order. 18 of 18 rows here have 0 in-tree callers, so these are change-safe without a caller audit.

## desktop slice 1

Rows 1-10: `OpenDesktop`, `CloseDesktop`, `CreateDesktop`, `CreateDesktopEx`, `EnumDesktops`, `CreateWindowStation`, `CloseWindowStation`, `EnumWindowStations`, `GetProcessWindowStation`, `GetThreadDesktop`. One fetch batch of 10, then write those 10 rows.

## desktop slice 2

Rows 11-18: `GetUserObjectInformation`, `OpenInputDesktop`, `OpenWindowStation`, `SetProcessWindowStation`, `SetThreadDesktop`, `SetUserObjectInformation`, `SwitchDesktop`, `EnumDesktopWindows`. One fetch batch of 8, then write those 8 rows.
