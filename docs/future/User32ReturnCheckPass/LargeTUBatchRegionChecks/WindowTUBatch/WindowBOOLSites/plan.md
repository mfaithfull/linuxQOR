---
Type: Plan
Title: "window BOOL sites"
Parent: "../plan.md"
Level: 3
Items:
  - Title: window BOOL slice 1
    Status: Atomic
    Content:
  - Title: window BOOL slice 2
    Status: Atomic
    Content:
  - Title: window BOOL slice 3
    Status: Atomic
    Content:
  - Title: window BOOL slice 4
    Status: Atomic
    Content:
  - Title: window BOOL slice 5
    Status: Atomic
    Content:
  - Title: window BOOL slice 6
    Status: Atomic
    Content:
---

# window BOOL sites

Slug: `WindowBOOLSites` - from title "window BOOL sites" (no drop-words, PascalCased, 15 chars, ASCII). Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **55** `window.cpp` rows published `BOOL`, in 6 slices of at most 10 sites.

## Resume

Cursor: **terminal** - all 6 items `Atomic`, all 6 closed 12 Nov 2026. Item 6 **window BOOL slice 6** is **CLOSED 12 Nov 2026** (rows 51-55: `ShowWindow`, `ShowWindowAsync`, `SoundSentry`, `UpdateLayeredWindow`, `UpdateLayeredWindowIndirect`): 3 of 5 sites landed (all 3 `checked` rows - `ShowWindowAsync`, `SoundSentry`, `UpdateLayeredWindow`), the 2 `unchecked` rows left bare (`ShowWindow` - previous-state return, "If the window was previously visible, the return value is nonzero. If the window was previously hidden, the return value is zero.", so zero answers the caller's question; `UpdateLayeredWindowIndirect` - its page has no Return value section at all, probe `no-page` / tag `no-return-value-section`, docQuote `-`, booked on its own probe line rather than from the `UpdateLayeredWindow` twin's wording); `window.cpp` `CheckReturn<` = **37** = the slice-5 census (34) plus this slice's 3, and 37 equals the leaf's full `checked` BOOL-row count exactly, so the leaf's per-TU re-census stop condition is met with no site outstanding; `return bResult;` = 37, `#include ... returncheck.h` = 1, `window.cpp` = 1102 lines; the per-TU gate `tmp/_tmp_u2_gate_tus.ps1` (with `tmp/_tmp_u2_gate_tus.txt` = `window.cpp`) reads `exit=TU_EXIT=0 log_lines=1 diags=0 dating=obj-newer ok`, `tus=1 fail=0`; `dumpbin /symbols` (`tmp/_tmp_u2_dumpbin_window.txt`) confirms all 3 stems as live `External` in the fresh `tmp/obj/window.obj` (`UpdateLayeredWindow` under its full 9-parameter C++ name, distinct from the `UpdateLayeredWindowIndirect` symbol, which stays a bare `Library::Call` return). Caller audit for this slice's 2 nonzero rows, both audited before landing at `ui/gui/controllers/windowcontroller.cpp`: :426 (`ShowWindowAsync`) and :447/:448 (`UpdateLayeredWindow`), each spelled `return User32::Stem(...) ? true : false;` so both already test the result and the landed check leaves the returned value identical, adding only the failure diagnostic; `SoundSentry` has 0 in-tree callers (change-safe). Instrument note for this slice's sweep: the caller audit must be a script file (`tmp/_tmp_u2_wo_audit6.ps1`) with PowerShell-increment `$i = $i + 1` - an inline `i++` is parsed as a command name and spams `CommandNotFoundException`, and `Select-String` has no `-File` parameter, so a sweep written that way matches comment lines and reprints file headers instead of call sites. Correction recorded here: this leaf's cursor previously claimed all 6 items closed while the census read 34; the 3 outstanding sites are this item's, and the census now matches. 9 of 10 sites landed (all 9 `checked` rows - `PhysicalToLogicalPointForPerMonitorDPI`, `RegisterShellHookWindow`, `SetLayeredWindowAttributes`, `SetProcessDefaultLayout`, `SetProcessDPIAware`, `SetWindowPlacement`, `SetWindowPos`, `SetWindowText`, `ShowOwnedPopups`), the 1 `unchecked` row (`SetForegroundWindow`) left bare; `window.cpp` `CheckReturn<` = **34** = the slice-4 census (25) plus this slice's 9, `return bResult;` = 34, `#include ... returncheck.h` = 1, and the per-TU gate `tmp/_tmp_u2_gate_tus.ps1` (with `tmp/_tmp_u2_gate_tus.txt` = `window.cpp`) reads `exit=TU_EXIT=0 diags=0 dating=obj-newer ok`, `tus=1 fail=0`; `dumpbin /symbols` (`tmp/_tmp_u2_dumpbin_window.txt`) confirms all 9 stems as live `External` in the fresh `tmp/obj/window.obj` (`SetWindowText` under its `SetWindowTextT` C++ name). Caller audit for this slice's 5 nonzero rows, all 10 call sites audited before landing at `ui/gui/controllers/windowcontroller.cpp`:353 (`RegisterShellHookWindow`), :363 and :368 (`SetLayeredWindowAttributes`), :382 (`SetWindowPlacement`), :387 / :392 / :397 / :402 / :407 (`SetWindowPos` - the densest site in this TU), :417 (`ShowOwnedPopups`), each spelled `return User32::Stem(...) ? true : false;` so every caller already tests the result and the landed check leaves the returned value identical, adding only the failure diagnostic. Item 4 **window BOOL slice 4** is **CLOSED 12 Nov 2026**: 4 of 10 sites landed (all 4 `checked` rows - `LockSetForegroundWindow`, `LogicalToPhysicalPointForPerMonitorDPI`, `MoveWindow`, `OpenIcon`), the 6 `unchecked` rows (`IsWindow`, `IsWindowUnicode`, `IsWindowVisible`, `IsZoomed`, `LogicalToPhysicalPoint`, `PhysicalToLogicalPoint`) left bare; `window.cpp` `CheckReturn<` = **25** = the slice-3 census (21) plus this slice's 4, `return bResult;` = 25, `#include ... returncheck.h` = 1, and the per-TU gate `tmp/_tmp_u2_gate_tus.ps1` (with `tmp/_tmp_u2_gate_tus.txt` = `window.cpp`) reads `exit=TU_EXIT=0 diags=0 dating=obj-newer ok`, `tus=1 fail=0`; `dumpbin /symbols` (`tmp/_tmp_u2_dumpbin_window.txt`) confirms all 4 stems as live `External` in the fresh `tmp/obj/window.obj`. Caller audit for this slice's 2 nonzero rows, both audited before landing: `MoveWindow` at `ui/gui/controllers/windowcontroller.cpp:320` and `OpenIcon` at `:325`, each spelled `return User32::Stem(...) ? true : false;` so both already test the result and the landed check leaves the returned value identical, adding only the failure diagnostic. Instrument caution recorded here: the ad-hoc caller sweep must run over `src` and `examples` with `-File` and read `$f.FullName` - a sweep that iterates `Get-ChildItem` results and reuses a stale `$L` reprints `window.cpp`'s own bodies under every other TU's name (spurious 40k-line output), and the preprocessed dumps under `tmp/` must be excluded since they echo every `user32.h` declaration. Item 3 **window BOOL slice 3** is **CLOSED 12 Nov 2026**: 5 of 10 sites landed (all 5 `checked` rows - `GetProcessDefaultLayout`, `GetTitleBarInfo`, `GetWindowInfo`, `GetWindowPlacement`, `GetWindowRect`), the 5 `unchecked` rows (`IsChild`, `IsGUIThread`, `IsHungAppWindow`, `IsIconic`, `IsProcessDPIAware`) left bare; `window.cpp` `CheckReturn<` = **21** = the slice-2 census (16) plus this slice's 5, `return bResult;` = 21, `#include ... returncheck.h` = 1, and the per-TU gate `tmp/_tmp_u2_gate_tus.ps1` (with `tmp/_tmp_u2_gate_tus.txt` = `window.cpp`) reads `exit=TU_EXIT=0 diags=0 dating=obj-newer ok`, `tus=1 fail=0`; `dumpbin /symbols` (`tmp/_tmp_u2_dumpbin_window.txt`) confirms all 5 stems as live `External` in the fresh `tmp/obj/window.obj`. Instrument correction recorded here: the gate must be driven through `tmp/_tmp_u2_gate_tus.ps1`, which passes the **full** source path to `tmp/_tmp_u2_one_tu.bat` - calling the bat bare with a bare stem from pwsh gives a spurious `TU_EXIT=2` / `c1xx: fatal error C1083: Cannot open source file`, because the bat compiles `%1` relative to cwd. Item 2 **window BOOL slice 2** is **CLOSED 12 Nov 2026** (7 landed, census 16). Item 1 **window BOOL slice 1** is **CLOSED 12 Nov 2026** (9 landed, census 9). Terminal: all 6 items `Atomic`.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the great-grandparent's (`../../../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, the caller audit for nonzero counts, one include per TU, the leaf budget of 10, the per-TU re-census stop condition. Sibling titles in the parent: window HWND sites, window other sites. Slices fixed by `tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt` under `ITEM window-BOOL`. Expect most rows here `unchecked`: the answer-shaped `BOOL` families (`IsWindow`, `IsChild`, `IsIconic`, `ShowOwnedPopups`, `AnimateWindow`) answer FALSE legitimately.

## window BOOL slice 1

10 sites in `window.cpp`: `AdjustWindowRect`, `AdjustWindowRectEx`, `AdjustWindowRectExForDpi`, `AllowSetForegroundWindow`, `AnimateWindow`, `AnyPopup`, `BringWindowToTop`, `ChangeWindowMessageFilter`, `CloseWindow`, `DeregisterShellHookWindow`. Land only the `checked` rows. Stop condition: re-read the sites, then re-census `window.cpp` against its `checked` row count.

## window BOOL slice 2

10 sites in `window.cpp`: `DestroyWindow`, `EndDeferWindowPos`, `EndTask`, `EnumChildWindows`, `EnumThreadWindows`, `EnumWindows`, `GetAltTabInfo`, `GetClientRect`, `GetGUIThreadInfo`, `GetLayeredWindowAttributes`. Stop condition: re-read the sites, then re-census `window.cpp` - its counts must already match the slice-1 census.

## window BOOL slice 3

10 sites in `window.cpp`: `GetProcessDefaultLayout`, `GetTitleBarInfo`, `GetWindowInfo`, `GetWindowPlacement`, `GetWindowRect`, `IsChild`, `IsGUIThread`, `IsHungAppWindow`, `IsIconic`, `IsProcessDPIAware`. Stop condition: re-read the sites, then re-census `window.cpp` - its counts must already match the slice-2 census.

## window BOOL slice 4

10 sites in `window.cpp`: `IsWindow`, `IsWindowUnicode`, `IsWindowVisible`, `IsZoomed`, `LockSetForegroundWindow`, `LogicalToPhysicalPoint`, `LogicalToPhysicalPointForPerMonitorDPI`, `MoveWindow`, `OpenIcon`, `PhysicalToLogicalPoint`. Stop condition: re-read the sites, then re-census `window.cpp` - its counts must already match the slice-3 census.

## window BOOL slice 5

10 sites in `window.cpp`: `PhysicalToLogicalPointForPerMonitorDPI`, `RegisterShellHookWindow`, `SetForegroundWindow`, `SetLayeredWindowAttributes`, `SetProcessDefaultLayout`, `SetProcessDPIAware`, `SetWindowPlacement`, `SetWindowPos`, `SetWindowText`, `ShowOwnedPopups`. `SetWindowPos` carries **5** in-tree callers - audit first. Stop condition: re-read the sites, then re-census `window.cpp` - its counts must already match the slice-4 census.

## window BOOL slice 6

5 sites in `window.cpp`: `ShowWindow`, `ShowWindowAsync`, `SoundSentry`, `UpdateLayeredWindow`, `UpdateLayeredWindowIndirect`. Stop condition: re-read the sites, then re-census `window.cpp` - its counts must already match the slice-5 census, and the TU's `CheckReturn<` total must equal the table's `checked` rows across all 10 `window.cpp` slices.
