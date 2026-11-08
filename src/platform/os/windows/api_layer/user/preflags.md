# Folder-level leads - the review queue is closed; T9 (U4's tranche queue) is the open lead

Written wholesale at the D26 close (8 Nov 2026) and replaced wholesale at every later close. All 38 review-queue sources are `closed` and the nine coverage TUs `pointer.cpp` (6a) through `legacy.cpp` (6g), `security.cpp` (T1) and `listbox.cpp` (T6) are closed with them, so there is **no next review TU** - but **T9 is open**: U4's remaining declared-but-undefined surface, deferred by the user (D26) as a tranche queue in T8's 6a-6g shape, one section / one new TU / one gate diff per session. This file names its next step. **Write-once**: read it once at boot, edit it never, replace it wholesale at the next close.

## Current folder figures (re-measured 8 Nov 2026 with `_tmp_static_census.ps1`, then `_tmp_decl_no_def_repo_scan.ps1`)

**47 sources / 634 member-declaration lines (548 static, 86 non-static) / 547 binding lines / 1,018 export names / 759 covered / 8 declared-and-unwrapped raw / 251 with no prototype.** `_tmp_static_census.ps1` also prints 0 defined-but-not-static, 0 defined-without-a-declaration and 547 distinct defined member names; the coverage instrument counts 635 members, so quote the instrument with the figure. `user32.h` is 855 lines.

**U4's remaining population is 86 non-static member declarations with no definition** (the `static` direction of U4 is 0 - U11's pair went in at session 29). It splits exactly on D18's scope line:

| T9 step | Section (`user32.h`) | Members | New TU |
|---|---|--:|---|
| **9a - the lead** | `//String functions` `:342-375` | 34 | `strings.cpp` |
| 9b | `//Raw Input functions` `:543-549` | 7 | `rawinput.cpp` |
| 9c | `//System Information functions` `:753-756` | 4 | `systeminfo.cpp` |
| 9d | `//Button Control functions` `:378-380` | 3 | `buttoncontrol.cpp` |
| 9e | `//Synchronization functions` `:779-780` | 2 | `synchronization.cpp` |
| 9f | `//Bitmap functions` `:776` | 1 | `bitmap.cpp` |
| 9g | `//Printing functions` `:783` | 1 | `printing.cpp` |
| 9h - **blocked on a scope decision** | `//Dynamic Data Exchange functions` `:507-540` | 34 | `dde.cpp` or delete |

**State the instrument with the count.** `_tmp_decl_no_def_repo_scan.ps1` reads **52** declared-and-undefined names repo-wide
(`src`/`examples`/`test`); the other 34 are the DDE block, whose declaration lines end `) const;` and whose regex the scanner
misses by design (the header has exactly 34 such lines). `_tmp_static_census.ps1` remains the authority for the published
**86**. U18's coverage instrument keeps its own member count at 635 - quote the instrument with the figure.

**What 9a-9g is measured to be:** all 52 are declared in `um\WinUser.h` (in D18 scope) and all 52 name real `user32.dll`
exports - 49 bind an exact bare export name, and 3 face an A/W pair only: `GetRawInputDeviceInfo` (1B7/1B8), `SystemParametersInfo`
(3A8/3AA) and `LoadBitmap` (254/255). Those three stems are `#undef`'d at `user32.h:15`/`:16`/`:21` and their bare spellings
are **not** exported, so `qor_pp_useswinapiAW` is mandatory there (G2 + G5: a bare binding stringizes a name that does not
exist and caches a NULL `pFunc` for the process lifetime). `wvsprintfA`/`wvsprintfW` (3F8/3F9) are separate exports from the
header-inline `wsprintfT` (D20) - the SDK's `#define wvsprintf  wvsprintfW` at `um\WinUser.h:318` is **live** (the stem is not
among the `user32.h` `#undef`s), so keep the A/W spellings and never spell the member `wvsprintf`.

**What 9h owes before it executes:** the DDE block is out of scope twice over - D18's addendum names the **DDE family** as a
superseded exclusion, and 0 of its 34 members are declared in `um\WinUser.h` (they are in `um\ddeml.h`) - yet 34 unusable
non-static members sit on the published class, and D18's direction for U4 is "completion, not deletion". **Wrap-or-delete is a
user decision, not a review action.** Measured against it: `ddeml.h` declares 28 distinct stems, so `FreeDDElParam`/`PackDDElParam`/
`ReuseDDElParam`/`UnpackDDElParam`/`ImpersonateDdeClientWindow` are declared in neither header; the folder's 34 `) const;` are
folder-invented against `ddeml.h`'s unqualified spelling and contradict D18; `mxDdeCreateStringHandle` (`:516`) is a mistyped
name with 0 export rows whose real exports are `DdeCreateStringHandleA/W` (86/87); and `DdeGetQualityOfService` (8F) is exported
and declared nowhere. Repo-wide there are **0 `User32::` mentions of any of the 86**, so no caller constrains the choice.

**What every step owes:** `static` on the declaration together with the definition in the new TU (that closes U4's link gap
and U18's non-static shape); the signature copies the prototype of the export it binds (D18); the binding spelling follows D13.
Coverage stays **759 of 1,018** throughout 9a-9g - all 52 were already declared, so each step closes a `LNK2019` link gap, never
a coverage gap (the arithmetic D24 recorded for T6). Census per step: static 548 -> 582 and non-static 86 -> 54, bindings 547 ->
599 over 47 -> 54 sources, once 9a-9g are in.

**D21's pin still under every session's feet:** `kernel32.h:22-30` pins `_WIN32_WINNT`/`WINVER` `0x0A00` and `NTDDI_VERSION`
`0x0A000010` (`NTDDI_WIN11_GE`) before `<Windows.h>`, guarded `#ifndef`, with a floor `#error` at `:33-35`. Do not lower or
re-`#define` them; an SDK prototype that vanished behind a `WINVER`/`NTDDI` guard is no longer a U16 finding, while the shrink
macros (`NOGDI`, `NOSYSPARAMSINFO`, `NOUSER`, `NOMSG`, `WINAPI_FAMILY`) stay unpinned and remain findings if a consumer sets them.

**Gate for any edit here:** the new TU standalone at `/W4` exit 0, then the whole folder **48/48** (and up as T9 lands TUs) at
`/W4` exit 0 with no diagnostics, `cl /EP` emitting the expected binding literals against the module literal `L"user32"`, and -
because every step edits the shared `user32.h` - the all-libraries run as a **diff** against a pre-fix run (standing baseline:
1 `C1083` at `opengl/wgl.cpp:31`, 165 warning lines, exit **2**; the source count grows with each new TU). The `/Fo` path must
stay attached to its switch. `wsprintfT` remains the folder's only header-inline member and `library.h`'s `varCall` its only
variadic call form.

