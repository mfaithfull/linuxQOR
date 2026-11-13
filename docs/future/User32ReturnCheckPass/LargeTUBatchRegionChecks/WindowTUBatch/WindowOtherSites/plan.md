---
Type: Plan
Title: "window other sites"
Parent: "../plan.md"
Level: 3
Items:
  - Title: window other slice 1
    Status: Atomic
    Content:
  - Title: window other slice 2
    Status: Atomic
    Content:
---

# window other sites

Slug: `WindowOtherSites` - from title "window other sites" (no drop-words, PascalCased, 16 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **14** `window.cpp` rows that are neither `BOOL` nor `HWND` (`UINT` 4, `int` 3, `WORD` 3, `HDWP` 2, `DWORD` 1, `void` 1), in 2 slices of at most 10 sites.

## Resume

Cursor: **terminal** - both items `Atomic`, both closed 13 Nov 2026. All **14** rows of this slice set carry their site in its end state: `token=8 bare=5 void=1` (`tmp/_tmp_u2_wo_sites.ps1` `tu=window.cpp other-rows=14 token=8 bare=5 void=1`), and the per-TU re-census `tmp/_tmp_u2_site_census_win.ps1` reads `rows=86 bare-checked=0 wrapped-unchecked=0 multi-or-void-wrap=0 VERDICT fail=0` with `window.cpp CheckReturn<` total **50** = the table's 50 `checked` rows across all `window.cpp` slices.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, the caller audit for nonzero counts, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: window BOOL sites, window HWND sites. Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM window-other`. The `void` row (`CascadeWindows`) gets no token - D51's rule settles it. `GetDC`/`ReleaseDC` (4 and 3 in-tree callers) sit in the sibling `devicecontext` TU, not here.

## window other slice 1

10 sites in `window.cpp`: `ArrangeIconicWindows`, `BeginDeferWindowPos`, `CascadeChildWindows`, `CascadeWindows`, `DeferWindowPos`, `GetWindowModuleFileName`, `GetWindowText`, `GetWindowTextLength`, `GetWindowThreadProcessId`, `InternalGetWindowText`. Land only the `checked` rows; `CascadeWindows` takes no token. Stop condition: re-read the sites, then re-census `window.cpp` - its counts must already match the HWND census.

## window other slice 2

4 sites in `window.cpp`: `RealGetWindowClass`, `SwitchToThisWindow`, `TileWindows`, `GetDpiForWindow`. Stop condition: re-read the sites, then re-census `window.cpp` - its `CheckReturn<` total must equal the table's `checked` rows across all 10 `window.cpp` slices.


## Cursors

window other slice 1: {"tu":"window.cpp","slice_start":0,"slice_len":10,"done":true}
window other slice 2: {"tu":"window.cpp","slice_start":10,"slice_len":4,"done":true}

## Closed 13 Nov 2026 - evidence

- Slice 1 (10 sites) landed 5 tokens - `ArrangeIconicWindows` (`CheckReturn< UINT, CheckNonZero< UINT > >`), `BeginDeferWindowPos` (`CheckNonZeroHandle< HDWP >`), `CascadeWindows` (`CheckNonZero< WORD >`), `DeferWindowPos` (`CheckNonZeroHandle< HDWP >`), `GetWindowThreadProcessId` (`CheckNonZero< DWORD >`); the other 5 of the slice (`CascadeChildWindows`, `GetWindowModuleFileName`, `GetWindowText`, `GetWindowTextLength`, `InternalGetWindowText`) are `unchecked` rows and stayed bare. Sites re-read after the pass: `token=5 bare=8 void=1`.
- Slice 2 (4 sites) landed 3 tokens - `RealGetWindowClassT` (`CheckReturn< UINT, CheckNonZero< UINT > >`, def-line 927), `TileWindows` (`CheckReturn< WORD, CheckNonZero< WORD > >`, def-line 1164), `GetDpiForWindow` (`CheckReturn< UINT, CheckNonZero< UINT > >`, def-line 1229); `SwitchToThisWindow` took no token. Sites re-read: `token=8 bare=5 void=1`, matching the table's 8 `checked` / 5 `unchecked` / 1 `void` across the 14 rows.
- Deviation from this leaf's `## Context`, which named `CascadeWindows` as the `void` row: measured, the `void` row is **`SwitchToThisWindow`** (`void User32::SwitchToThisWindow(HWND hwnd, BOOL fUnknown)`, `pubRetType void`, `sdkRetType VOID`, its page's Return value section carrying no value), and `CascadeWindows` is **checked** `CheckNonZero< WORD >` - its page reserves zero ("Type: WORD If the function succeeds, the return value is the number of windows arranged. If the function fails, the return value is zero. To get extended error information, call GetLastError ."). So slice 1 landed 5 tokens rather than 4, and slice 2's void row is the one that takes no token.
- Census-instrument fix recorded here: 4 of the 14 rows are defined under the ANSI twin header (`GetWindowModuleFileNameT` :654, `GetWindowTextT` :695, `GetWindowTextLengthT` :703, `RealGetWindowClassT` :927), so `tmp/_tmp_u2_wo_sites.ps1` needed `[A-Za-z0-9_]*` after the stem in its header regex; without it those 4 printed `NO-DEFINITION` and the tally under-counted.
- Caller audits for the nonzero-count rows in these slices, taken before the tokens moved: `TileWindows` - `windowcontroller.cpp:443` spells it `return User32::TileWindows(WINDOW_HANDLE, how, reinterpret_cast<RECT*>(&rc), static_cast<UINT>(vecChildren.size()), vecChildren.data());`, so the controller passes the count straight through untested and the landed check leaves the value identical, adding only the diagnostic. `GetDpiForWindow` - `windowcontroller.cpp:792` inside `unsigned int WindowController::GetDPI()` spells it `return User32::GetDpiForWindow(WINDOW_HANDLE);`, the same untested pass-through. `RealGetWindowClass` carries 0 in-tree callers (change-safe).
- The 4 unqualified `GetDpiForWindow` sites in `windowimpl.cpp` (`:361`, `:1130`, `:1212`, `:1462`) are not live bypasses: every one sits inside a block comment, the ranges recorded at `dpi.cpp:85-86` (325-408, 530-1301, 530-1301, 1303-1511). The audit instrument's `//` filter does not exclude block comments, which is why they printed as `HIT`s.
- `CheckNonZero< WORD >` and `CheckNonZeroHandle< HDWP >` land at their row's own published type; `RealGetWindowClass`'s row is `sdkRetType NO-PROTOTYPE` (AW binding), so its class base is the published `UINT` from `returncheck.h:187`'s `template< typename TNumeric >`.
- Gates after the pass: per-TU `tmp/_tmp_u2_gate_tus.ps1` on `window.cpp` `exit=TU_EXIT=0 log_lines=1 diags=0 dating=obj-newer ok` `tus=1 fail=0`; folder `tmp/_tmp_u2_gate54.bat` + `tmp/_tmp_u2_gatechk.ps1` `srcs=54 not-in-log=0 stale=0 real-diag-lines=0`.
- Stop condition met: the per-TU re-census `tmp/_tmp_u2_site_census_win.ps1` reads `rows=86 bare-checked=0 wrapped-unchecked=0 multi-or-void-wrap=0 VERDICT fail=0`, and `window.cpp`'s `CheckReturn<` total is **50** = the table's 50 `checked` rows across all 10 `window.cpp` slices (BOOL leaf 37 + HWND leaf 5 + this leaf 8). `returncheck.h` is included once.
