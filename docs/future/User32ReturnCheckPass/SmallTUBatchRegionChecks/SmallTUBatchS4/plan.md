---
Type: Plan
Title: "Small TU batch S4"
Parent: "../plan.md"
Level: 2
Items:
  - Title: monitor sites
    Status: Atomic
    Content:
  - Title: windowproperties sites
    Status: Atomic
    Content:
  - Title: processthread sites
    Status: Atomic
    Content:
  - Title: systeminfo sites
    Status: Atomic
    Content:
  - Title: ime sites
    Status: Atomic
    Content:
  - Title: help sites
    Status: Atomic
    Content:
  - Title: text sites
    Status: Atomic
    Content:
---

# Small TU batch S4

Slug: `SmallTUBatchS4` - from title "Small TU batch S4" (no drop-words, PascalCased, 14 chars, ASCII); the same folder name the verdict-side tree uses, so the code batch and the fetch batch cover identical row sets. Uniqueness is stored here, not recomputed.

## Goal

Land the token at exactly the `checked` verdicts of the **33 sites** of these 7 small TUs, one item per TU, each item closing with that TU's re-census.

## Resume

Cursor: this plan is **terminal** - all 7 items CLOSED **12 Nov 2026**. Item 1 **monitor sites** is **CLOSED 12 Nov 2026** (5 sites decided: 2 tokens landed `EnumDisplayMonitors`/`GetMonitorInfo` `BoolCheck`, 3 written-reason `unchecked` `MonitorFromPoint`/`MonitorFromRect`/`MonitorFromWindow`; per-TU census `tus=1 fail=0`, standalone `/W4` leg `tus=1 fail=0` at exit 0 / 0 diagnostics / obj newer). Item 2 **windowproperties sites** is **CLOSED 12 Nov 2026** (5 sites decided: 1 token `SetProp` `BoolCheck`, 4 written-reason `unchecked` `EnumProps`/`EnumPropsEx`/`GetProp`/`RemoveProp`; census `windowproperties.cpp rows=5 fcontext=5 useswinapi=5 (0 bare + 5 AW) LibraryCall=5 Win32ErrorHandler=5 CheckReturn-code=1 table-checked=1 table-unchecked=4 returncheck-includes=1 ok`, gate `tus=1 fail=0` at exit 0 / 0 diagnostics / obj newer; the TU's one `../returncheck.h` include went 0 -> 1). Item 3 **processthread sites** is **CLOSED 12 Nov 2026** (5 rows decided: the 3 seeds `AttachThreadInput` `BoolCheck` / `GetGuiResources` `CheckNonZero< DWORD >` / `UserHandleGrantAccess` `BoolCheck` never re-edited, 2 written-reason `unchecked` `IsWow64Message` (answer-shaped BOOL; its 1 in-tree caller re-read at `ui/gui/view/handlers/messagehandler.cpp:246`) and `WaitForInputIdle` (inverted site - the page's table prints 0 for the satisfied wait, so a zero-check would call success a failure); 0 tokens landed, census `processthread.cpp rows=5 fcontext=5 useswinapi=5 (5 bare + 0 AW) LibraryCall=5 Win32ErrorHandler=5 CheckReturn-code=3 table-checked=3 table-unchecked=2 returncheck-includes=1 ok`, gate `tus=1 fail=0` at exit 0 / 0 diagnostics / obj newer; the `../returncheck.h` include was already live there for the seeds). Item 4 **systeminfo sites** is **CLOSED 12 Nov 2026** (4 rows decided: 3 tokens `GetSystemMetrics` `CheckNonZero< int >`, `SetSysColors`/`SystemParametersInfo` `BoolCheck`, 1 written-reason `unchecked` `GetSysColor` (the page says outright that zero is a legal RGB value); the table puts `GetSystemMetrics` on a check where the review's TU paragraph expected unchecked - the table's verdict is the one landed, and the addendum at the TU head records the divergence; census `systeminfo.cpp rows=4 fcontext=4 useswinapi=4 (3 bare + 1 AW) LibraryCall=4 Win32ErrorHandler=4 CheckReturn-code=3 table-checked=3 table-unchecked=1 returncheck-includes=1 ok`, gate `tus=1 fail=0` at exit 0 / 0 diagnostics / obj newer; include 0 -> 1). Item 5 **ime sites** is **CLOSED 12 Nov 2026** (5 rows decided: **0 tokens** - the table reads 4 `unchecked` and 1 `void` here, so this TU carries 0 `CheckReturn` and keeps 0 `../returncheck.h` includes; the 4 written reasons are `SendIMEMessageEx` (answer-shaped - the return is the subfunction's own result code and the page puts the failure detail in the caller's IMESTRUCT `wParam`), `WINNLSGetIMEHotkey`/`WINNLSGetEnableStatus` (no Learn page at all - the probe read no-slug-in-Learn-index, so no documented failure value exists) and `WINNLSEnableIME` (previous-state BOOL - "The previous state of the IME. TRUE if it was enabled before this call, otherwise, FALSE"), and the void row `DisableProcessWindowsGhosting` took neither; the `SendIMEMessageEx` hand-through was re-read at `ui/gui/controllers/windowcontroller.cpp:787` before the verdict was accepted; census `ime.cpp rows=5 fcontext=5 useswinapi=5 (4 bare + 1 AW) LibraryCall=5 Win32ErrorHandler=5 CheckReturn-code=0 table-checked=0 table-unchecked=4 returncheck-includes=0 ok`, gate `tus=1 fail=0` at exit 0 / 0 diagnostics / obj newer; the table agrees with the TU's all-raw review record, so no addendum was needed). Item 6 **help sites** is **CLOSED 12 Nov 2026** (5 rows decided: 3 tokens `SetMenuContextHelpId`/`SetWindowContextHelpId`/`WinHelp` `BoolCheck`, 2 written-reason `unchecked` `GetMenuContextHelpId`/`GetWindowContextHelpId` (answer-shaped DWORD - zero is the documented answer for an object that carries no help context id, and a rejected handle is indistinguishable from it); census `help.cpp rows=5 fcontext=5 useswinapi=5 (4 bare + 1 AW) LibraryCall=5 Win32ErrorHandler=5 CheckReturn-code=3 table-checked=3 table-unchecked=2 returncheck-includes=1 ok`, gate `tus=1 fail=0` at exit 0 / 0 diagnostics / obj newer, include 0 -> 1; the three in-tree mentions were re-read at `ui/gui/controllers/windowcontroller.cpp:797`/`:802`/`:807` before the verdicts were accepted, the menu pair has 0 in-tree callers, and `WinHelp` - the AW row whose prototype the census could not resolve (NO-PROTOTYPE) - keeps the published `BOOL` this wrapper declares as its class base, with the TU's addendum recording the divergence from the batch note). Item 7 **text sites** is **CLOSED 12 Nov 2026** (4 rows decided: **0 tokens** - the table reads 4 `unchecked` here, so `text.cpp` carries 0 `CheckReturn` and keeps 0 `../returncheck.h` includes; the 4 written reasons are `DrawText`/`DrawTextEx` (answer-shaped `int` - the page's return is the extent of the drawn text, so 0 is that extent's degenerate answer and the documented 0 failure value is not reserved; the T spellings' in-tree mentions at `ui/gui/gdiobjects/devicecontext.cpp:831`/`:836` were re-read before the verdicts were accepted and return the `int` untested), `GetTabbedTextExtent` (the packed LOWORD width / HIWORD height, 0 that pair's degenerate value) and `TabbedTextOut` (the same packed pair, 0 legal); census `text.cpp rows=4 fcontext=4 useswinapi=4 (0 bare + 4 AW) LibraryCall=4 Win32ErrorHandler=4 CheckReturn-code=0 table-checked=0 table-unchecked=4 returncheck-includes=0 ok`, gate `tus=1 fail=0` at exit 0 / 0 diagnostics / obj newer; the table agrees with the TU's own all-raw `//Return policy (U1/U2)` paragraph, so the TU's addendum records only that the site-level comments were added and the review's answer was already this one). Terminal: all 7 items `Atomic`, all 7 CLOSED.

## Context

Inherited from the parent (`../plan.md`): D51's decision process gated by the session-27 soundness test; three end states per site; G6 makes every landed check live on every call. **The landing contract these items obey is the parent's (`../plan.md`) `## Decisions`** - token form, `sdkRetType` class base, verbatim quote in the site comment, one include per TU, the 5 seeds never re-edited, the per-TU re-census stop condition. Sibling titles in the parent: Small TU batch S1, Small TU batch S2, Small TU batch S3, Small TU batch S5.

Row sets per TU are the same rows the verdict-side `SmallTUBatchS4` filled (`tmp/_tmp_u2_slice_stems.ps1` -> `tmp/_tmp_u2_slice_stems.txt`, `ITEM <tu> slice 1`). Three seeds sit in `processthread.cpp` - counted in its rows, never re-edited.

## monitor sites

5 sites: `EnumDisplayMonitors`, `GetMonitorInfo`, `MonitorFromPoint`, `MonitorFromRect`, `MonitorFromWindow` (`monitor.cpp`). The `MonitorFrom*` rows answer NULL both as failure and as "no monitor there" - expected `unchecked` with the reason. Re-census after.

## windowproperties sites

5 sites: `EnumProps`, `EnumPropsEx`, `GetProp`, `RemoveProp`, `SetProp` (`windowproperties.cpp`). Same procedure, then re-census that TU.

## processthread sites

5 rows, **2 in play**: `IsWow64Message`, `WaitForInputIdle`; the seeds `AttachThreadInput` (`:87`), `GetGuiResources` (`:97`), `UserHandleGrantAccess` (`:115`) are already landed and the include already exists. `WaitForInputIdle` answers `WAIT_OBJECT_0`/`TIMED_OUT` - expected `unchecked` with the page's wording. Re-census `processthread.cpp` after - its `CheckReturn<` count must already carry the 3 seeds.

## systeminfo sites

4 sites: `GetSysColor`, `GetSystemMetrics`, `SetSysColors`, `SystemParametersInfo` (`systeminfo.cpp`). `GetSystemMetrics` answers 0 legitimately - expected `unchecked` with the reason. Re-census after.

## ime sites

5 sites: `DisableProcessWindowsGhosting`, `SendIMEMessageEx`, `WINNLSGetIMEHotkey`, `WINNLSEnableIME`, `WINNLSGetEnableStatus` (`ime.cpp`). Same procedure, then re-census that TU.

## help sites

5 sites: `SetMenuContextHelpId`, `GetMenuContextHelpId`, `SetWindowContextHelpId`, `GetWindowContextHelpId`, `WinHelp` (`help.cpp`). `WinHelp`'s FALSE is also "user cancelled" - expected `unchecked`. Re-census after.

## text sites

4 sites: `DrawText`, `DrawTextEx`, `GetTabbedTextExtent`, `TabbedTextOut` (`text.cpp`). Same procedure, then re-census that TU.
