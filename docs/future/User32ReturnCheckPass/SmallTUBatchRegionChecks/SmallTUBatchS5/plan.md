---
Type: Plan
Title: "Small TU batch S5"
Parent: "../plan.md"
Level: 2
Items:
  - Title: displayconfig sites
    Status: Atomic
    Content:
  - Title: systemshutdown sites
    Status: Atomic
    Content:
  - Title: caret sites
    Status: Atomic
    Content:
  - Title: rawinput sites
    Status: Atomic
    Content:
  - Title: devicecontext sites
    Status: Atomic
    Content:
---

# Small TU batch S5

Slug: `SmallTUBatchS5` - from title "Small TU batch S5" (no drop-words, PascalCased, 14 chars, ASCII); the same folder name the verdict-side tree uses, so the code batch and the fetch batch cover identical row sets. Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **36 sites** of these 5 small TUs, one item per TU, each item closing with that TU's re-census.

## Resume

Cursor: terminal - all 5 items `Atomic`. Terminal: all 5 items `Atomic`. **Hard precondition: these rows must already carry verdicts.** Items 1 **displayconfig sites**, 2 **systemshutdown sites**, 3 **caret sites**, 4 **rawinput sites** and 5 **devicecontext sites** are **CLOSED 12 Nov 2026**. Item 3 **caret sites**: 8 of 8 sites decided, 8 tokens landed - 7 `CheckReturn< BOOL, BoolCheck >::TType` over `CreateCaret`, `DestroyCaret`, `GetCaretPos`, `HideCaret`, `SetCaretBlinkTime`, `SetCaretPos`, `ShowCaret` and 1 `CheckReturn< UINT, CheckNonZero< UINT > >::TType` over `GetCaretBlinkTime` (its 0 is the documented failure answer while `INFINITE` is the caret-does-not-blink answer, which a nonzero test passes) - 0 unchecked, and `caret.cpp` gained its one `../returncheck.h` include (0 -> 1). All 8 stems have in-tree callers (1 each, 2 for `HideCaret` / `ShowCaret`), so every call site was read before its token moved (`tmp/_tmp_u2_callers_caret.ps1`: 40 mentions, 22 outside the TU - 8 `user32.h:464-:471` declarations, 10 call sites in `ui/gui/controllers/a_h/caret.cpp` and `ui/gui/controllers/windowcontroller.cpp:663` / `:668,` and the rest comments); census `caret.cpp rows=8 fcontext=8 useswinapi=8 (8 bare + 0 AW) LibraryCall=8 Win32ErrorHandler=8 CheckReturn-code=8 table-checked=8 table-unchecked=0 returncheck-includes=1 ok` and the standalone `/W4` leg at exit 0 / 0 diagnostics / obj newer. Layer-wide after the 3 leaves: **65** `CheckReturn<` code sites over **26** TUs with **26** `../returncheck.h` includes (`tmp/_tmp_u2_live_checks.ps1`). Item 1 **displayconfig sites** is **CLOSED 12 Nov 2026** (7 of 7 sites decided from the table's verdict columns: 7 tokens landed - 5 `CheckReturn< LONG, TSuccessCheck< LONG > >::TType` over `DisplayConfigGetDeviceInfo`, `DisplayConfigSetDeviceInfo`, `GetDisplayConfigBufferSizes`, `QueryDisplayConfig`, `SetDisplayConfig` and 2 `CheckReturn< BOOL, BoolCheck >::TType` over `GetDisplayAutoRotationPreferences`, `SetDisplayAutoRotationPreferences`, 0 unchecked, 0 void - and `displayconfig.cpp` gained its one `../returncheck.h` include (0 -> 1); 0 in-tree callers for all 7 stems, re-measured before the tokens moved; census `displayconfig.cpp rows=7 fcontext=7 useswinapi=7 (7 bare + 0 AW) LibraryCall=7 Win32ErrorHandler=7 CheckReturn-code=7 table-checked=7 table-unchecked=0 returncheck-includes=1 ok` and the standalone `/W4` leg at exit 0 / 0 diagnostics / obj newer). Item 2 **systemshutdown sites** is **CLOSED 12 Nov 2026** (6 rows, 5 in play: 4 tokens landed `CheckReturn< BOOL, BoolCheck >::TType` over `ExitWindowsEx`, `LockWorkStation`, `ShutdownBlockReasonCreate`, `ShutdownBlockReasonQuery` and `CancelShutdown` kept `unchecked` with the written reason at its site - no Learn page for that stem, so no documented failure value to key on; `ShutdownBlockReasonDestroy` is D51's seed, never re-edited, so the TU reads 5 `CheckReturn` sites; the TU keeps its 1 `../returncheck.h` include, 0 gained; 0 in-tree callers for all 6 stems, re-measured with `tmp/_tmp_u2_callers_ss.ps1` before the tokens moved; census `systemshutdown.cpp rows=6 fcontext=6 useswinapi=6 (6 bare + 0 AW) LibraryCall=6 Win32ErrorHandler=6 CheckReturn-code=5 table-checked=5 table-unchecked=1 returncheck-includes=1 ok` at exit 0 / 0 diagnostics / obj newer). Layer-wide after both leaves: **57** `CheckReturn<` code sites over **25** TUs with **25** `../returncheck.h` includes (`tmp/_tmp_u2_live_checks.ps1`). Item 4 **rawinput sites** is **CLOSED 12 Nov 2026** (7 of 7 sites decided from the table's verdict columns: 7 tokens landed - 5 `CheckReturn< UINT, TCheckFailureValue< UINT, ( UINT )-1 > >::TType` over `GetRawInputBuffer`, `GetRawInputData`, `GetRawInputDeviceInfo`, `GetRawInputDeviceList`, `GetRegisteredRawInputDevices`, 1 `CheckReturn< LRESULT, TCheckFailureValue< LRESULT, -1 > >::TType` over `DefRawInputProc` and 1 `CheckReturn< BOOL, BoolCheck >::TType` over `RegisterRawInputDevices`, 0 unchecked, 0 void - and `rawinput.cpp` gained its one `../returncheck.h` include (0 -> 1). The landed class is the **sentinel** test, not a nonzero reflex: each page names `( UINT )-1` (`DefRawInputProc`: `-1`) as the reserved error answer while 0 answers the NULL-capacity query and every device/byte/record count is the answer the caller reads back, so gating anything but the sentinel would report a legal count - the review's own warning against `CheckNonZero` at these 6 numerics stays true and the landed shape is the opposite one. `GetRawInputDeviceInfo` is an AW `NO-PROTOTYPE` row, so its class base is the published `UINT`. 0 in-tree callers for all 7 stems, re-measured before the tokens moved, so 0 call site needed re-reading; census `rawinput.cpp rows=7 fcontext=7 useswinapi=7 (6 bare + 1 AW) LibraryCall=7 Win32ErrorHandler=7 CheckReturn-code=7 table-checked=7 table-unchecked=0 returncheck-includes=1 ok` and the standalone `/W4` leg at exit 0 / 0 diagnostics / obj newer. Item 5 **devicecontext sites** is **CLOSED 12 Nov 2026** (8 of 8 sites decided from the table's verdict columns: 7 tokens landed - 2 `CheckReturn< LONG, TSuccessCheck< LONG > >::TType` over `ChangeDisplaySettings`, `ChangeDisplaySettingsEx` (the pages name `DISP_CHANGE_SUCCESSFUL` as the only success code and list only failure codes after it, so the test reads `value == 0` - the opposite polarity to the nonzero reflex the section's own review warned against), 3 `CheckReturn< BOOL, BoolCheck >::TType` over `EnumDisplayDevices`, `EnumDisplaySettings`, `EnumDisplaySettingsEx` and 2 `CheckReturn< HDC, CheckNonZeroHandle< HDC > >::TType` over `GetDC`, `GetDCEx`; `ReleaseDC` stays `unchecked` with the written reason - the table's verbatim docQuote makes zero the answer to "was the DC released" and the Remarks add that a class or private DC answers zero legitimately, so the session-27 soundness test fails at that value); `devicecontext.cpp` gained its one `../returncheck.h` include (0 -> 1). 0 in-tree callers for the 5 bare/AW stems' bare spelling, but the `*T` wrappers are called in-tree (`ui/gdi/displaydevices.cpp:42,:47,:61,:70` and `ui/gui/controllers/i_m/monitor.cpp:94,:177,:195,:213`) and `GetDC` / `GetDCEx` have 3 and 2 `User32::` call sites (`ui/gui/controllers/i_m/monitor.cpp:146`, `ui/gui/controllers/windowcontroller.cpp:615` / `:627`, `ui/gui/gdiobjects/devicecontext.cpp:44` / `:52`) - all re-read before their tokens moved (`tmp/_tmp_u2_callers_dc.ps1`); census `devicecontext.cpp rows=8 fcontext=8 useswinapi=8 (3 bare + 5 AW) LibraryCall=8 Win32ErrorHandler=8 CheckReturn-code=7 table-checked=7 table-unchecked=1 returncheck-includes=1 ok` and the standalone `/W4` leg at exit 0 / 0 diagnostics / obj newer. Layer-wide after all 5 leaves: **79** `CheckReturn<` code sites over **28** TUs with **28** `../returncheck.h` includes (`tmp/_tmp_u2_live_checks.ps1`).

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the parent's (`../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, verbatim quote in the site comment, one include per TU, the 5 seeds never re-edited, the per-TU re-census stop condition. Sibling titles in the parent: Small TU batch S1, Small TU batch S2, Small TU batch S3, Small TU batch S4.

Row sets per TU are the same rows the verdict-side `SmallTUBatchS5` filled (`tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt`, `ITEM <tu> slice 1`). One seed sits in this batch: `systemshutdown.cpp:134`.

## displayconfig sites

7 sites: `DisplayConfigGetDeviceInfo`, `DisplayConfigSetDeviceInfo`, `GetDisplayAutoRotationPreferences`, `GetDisplayConfigBufferSizes`, `QueryDisplayConfig`, `SetDisplayAutoRotationPreferences`, `SetDisplayConfig` (`displayconfig.cpp`). The `DisplayConfig*` rows return an error code whose failure value is not a sentinel - the class is `TCheckFailureValue< >`-shaped, not `CheckNonZero< >`. Re-census after.

## systemshutdown sites

6 rows, **5 in play**: `ExitWindowsEx`, `CancelShutdown`, `LockWorkStation`, `ShutdownBlockReasonCreate`, `ShutdownBlockReasonQuery`; `ShutdownBlockReasonDestroy` at `systemshutdown.cpp:134` is D51's and already landed (its include exists). The FALSE-is-failure rows here carry no answer meaning - the expected `checked` set. Re-census `systemshutdown.cpp` after.

## caret sites

8 sites: `CreateCaret`, `DestroyCaret`, `GetCaretBlinkTime`, `GetCaretPos`, `HideCaret`, `SetCaretBlinkTime`, `SetCaretPos`, `ShowCaret` (`caret.cpp`). The largest small TU here; 8 sites is still one edit pass. Same procedure, then re-census that TU.

## rawinput sites

7 sites: `DefRawInputProc`, `GetRawInputBuffer`, `GetRawInputData`, `GetRawInputDeviceInfo`, `GetRawInputDeviceList`, `GetRegisteredRawInputDevices`, `RegisterRawInputDevices` (`rawinput.cpp`). Same procedure, then re-census that TU.

## devicecontext sites

8 sites: `ChangeDisplaySettings`, `ChangeDisplaySettingsEx`, `EnumDisplayDevices`, `EnumDisplaySettings`, `EnumDisplaySettingsEx`, `GetDC`, `GetDCEx`, `ReleaseDC` (`devicecontext.cpp`). `GetDC`/`GetDCEx` answer NULL both as failure and as a legal query value - those stay `unchecked` with the reason. Same procedure, then re-census that TU.
