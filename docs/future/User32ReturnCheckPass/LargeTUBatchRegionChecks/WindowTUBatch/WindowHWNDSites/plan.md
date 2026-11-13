---
Type: Plan
Title: "window HWND sites"
Parent: "../plan.md"
Level: 3
Items:
  - Title: window HWND slice 1
    Status: Atomic
    Content:
  - Title: window HWND slice 2
    Status: Atomic
    Content:
---

# window HWND sites

Slug: `WindowHWNDSites` - from title "window HWND sites" (no drop-words, PascalCased, 15 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **17** `window.cpp` rows published `HWND`, in 2 slices of at most 10 sites.

## Resume

Cursor: **terminal** - both items `Atomic`, both closed 12 Nov 2026. All **5** `checked` rows of the 17 published-`HWND` `window.cpp` rows carry the token; the **12** `unchecked` rows stayed bare. The definition-anchored site census `tmp/_tmp_u2_site_census_win.ps1` (new instrument) reads for the 17 rows: `CreateWindowEx cr=1`, `FindWindow cr=1`, `FindWindowEx cr=1`, `GetAncestor cr=1`, `SetParent cr=1`, and `cr=0` for all 12 unchecked - `tu=window.cpp rows=86 bare-checked=8 wrapped-unchecked=0 multi-or-void-wrap=0`, and those 8 bare rows are all published non-`HWND` (`ArrangeIconicWindows` UINT, `BeginDeferWindowPos` HDWP, `CascadeWindows` WORD, `DeferWindowPos` HDWP, `GetDpiForWindow` UINT, `GetWindowThreadProcessId` DWORD, `RealGetWindowClass` UINT, `TileWindows` WORD), i.e. the **window other sites** leaf's rows, none of this leaf's. `window.cpp` `CheckReturn<` = **42** = the leaf's 5 on top of the BOOL leaf's 37, `#include ... returncheck.h` = 1, `window.cpp` = 1156 lines. The per-TU gate `tmp/_tmp_u2_gate_tus.ps1` (with `tmp/_tmp_u2_gate_tus.txt` = `window.cpp`) reads `exit=TU_EXIT=0 log_lines=1 diags=0 dating=obj-newer ok`, `tus=1 fail=0`, and `dumpbin /symbols` (`tmp/_tmp_u2_dumpbin_window.txt`) confirms all 5 stems live `External` in the fresh `tmp/obj/window.obj` - the TU names these five `CreateWindowExT`, `FindWindowT`, `FindWindowExT`, `GetAncestor`, `SetParent` with a trailing `T`, which is why the census had to map table stem -> def stem through that suffix. Terminal: both items `Atomic`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, the caller audit for nonzero counts, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: window BOOL sites, window other sites. Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM window-HWND`. Here NULL is a documented failure and also a legal answer for the lookup shape (`WindowFromPoint`, `GetLastActivePopup`), so the page's exact wording decides `checked` vs `unchecked` per row.

## window HWND slice 1

10 sites in `window.cpp`: `ChildWindowFromPoint`, `ChildWindowFromPointEx`, `CreateWindowEx`, `FindWindow`, `FindWindowEx`, `GetAncestor`, `GetDesktopWindow`, `GetForegroundWindow`, `GetLastActivePopup`, `GetParent`. Land only the `checked` rows. Stop condition: re-read the sites, then re-census `window.cpp` - its counts must already match the BOOL-slice census before this slice.

## window HWND slice 2

7 sites in `window.cpp`: `GetShellWindow`, `GetTopWindow`, `GetWindow`, `RealChildWindowFromPoint`, `SetParent`, `WindowFromPhysicalPoint`, `WindowFromPoint`. Stop condition: re-read the sites, then re-census `window.cpp` - its counts must already match the HWND-slice-1 census.

## Closed 12 Nov 2026 - evidence

- Slice 1 landed 4 of its 10 sites: `CreateWindowEx`, `FindWindow`, `FindWindowEx`, `GetAncestor` - the only `checked` rows in that slice; `ChildWindowFromPoint`, `ChildWindowFromPointEx`, `GetDesktopWindow`, `GetForegroundWindow`, `GetLastActivePopup`, `GetParent` stayed bare on the page's answer-shaped reading (NULL is the documented answer for a top-level window / a window with no parent, and the `WindowFromPoint` family's NULL is a legal "no window here" answer).
- Slice 2 landed 1 of its 7 sites: `SetParent`. `GetShellWindow`, `GetTopWindow`, `GetWindow`, `RealChildWindowFromPoint`, `WindowFromPhysicalPoint`, `WindowFromPoint` stayed bare.
- Class for all 5: `CheckNonZeroHandle< HWND >` (returncheck.h:137, `template< typename THandle > class CheckNonZeroHandle : public AutoErrorCheck< THandle >`, Test fails at 0x00000000 only). Of the shapes the leaf's context named, `HandleCheck` keys on `(HANDLE)(-1)` and `HandleNullCheck` has a fixed `HANDLE` base that mismatches this row's `HWND`, so `CheckNonZeroHandle` is the class that fits the row's type - matching the precedent already booked for the `GetDlgItem` rows.
- `CreateWindowEx`, `FindWindow`, `FindWindowEx` have `sdkRetType` NO-PROTOTYPE, so their class base is the published `HWND`; `GetAncestor` and `SetParent` have `sdkRetType` HWND.
- Caller audit before landing (G6 makes the check live on every call): `GetAncestor` 1 in-tree caller and `SetParent` 1 in-tree caller - `windowcontroller.cpp:375` spells `HWND hwnd = User32::SetParent(WINDOW_HANDLE, ...);` and hands `hwnd` to `perThread->FromHandle` without testing it, so the landing adds the diagnostic alone (`TType` is the plain return type under G6, since `qor_pp_parameter_checking`/`qor_pp_yes` are defined nowhere and `returncheck.h:19`'s `#if` reads `0 == 0`). The three NO-PROTOTYPE rows carry 0 in-tree callers (change-safe).
- Stop condition met per slice: after slice 1 the census read `CheckReturn<` = 41 (37 BOOL + 4), after slice 2 = 42 = 37 BOOL + 5 HWND, and `wrapped-unchecked=0` / `multi-or-void-wrap=0` throughout, so no site was wrapped twice and no `unchecked` row acquired a check.
- Instrument correction recorded here: the naive per-stem body scan (`grep stem then next 25 lines`) is unsound - names overlap (`GetLastActivePopup` counted 20) - and the definition-anchored census needed both fixes: PowerShell is case-insensitive so the table loop variable `$l` clobbered the source array `$L` (that bug alone produced the bogus `bare-checked=44`), and the TU's trailing-`T` definition names (`CreateWindowExT`, `FindWindowT`, `FindWindowExT`, `GetAncestor`, `SetParent`, `GetAltTabInfoT`, `SetWindowTextT`, ...) required mapping table stem -> def stem through the `T` suffix (that gap alone produced the 9 bogus `MISSING-SITE` rows). With both fixed the census reads `bare-checked=8 wrapped-unchecked=0 multi-or-void-wrap=0` and the 8 bare rows are exactly the other-sites leaf's.

