# Folder-level leads - the review queue is closed; T9 (U4's tranche queue) is the open lead

Written wholesale at the D27 close (9 Nov 2026) and replaced wholesale at every later close. All 38 review-queue sources are `closed` and the ten coverage TUs `pointer.cpp` (6a) through `legacy.cpp` (6g), `security.cpp` (T1), `listbox.cpp` (T6) and `strings.cpp` (T9 9a) are closed with them, so there is **no next review TU** - but **T9 is open**: U4's remaining declared-but-undefined surface, deferred by the user (D26) as a tranche queue in T8's 6a-6g shape, one section / one new TU / one gate diff per session. This file names its next step. **Write-once**: read it once at boot, edit it never, replace it wholesale at the next close.

## Current folder figures (re-measured 9 Nov 2026 after 9a with `_tmp_static_census.ps1`, then `_tmp_decl_no_def_repo_scan.ps1`)

**48 sources / 634 member-declaration lines (582 static, 52 non-static) / 581 binding lines / 1,018 export names / 759 covered / 8 declared-and-unwrapped raw / 251 with no prototype.** `_tmp_static_census.ps1` also prints 0 defined-but-not-static, 0 defined-without-a-declaration and 581 distinct defined member names; the coverage instrument counts 635 members, so quote the instrument with the figure. `user32.h` is 855 lines.

**U4's remaining population is 52 non-static member declarations with no definition** (9a closed its 34; the `static` direction of U4 is 0 - U11's pair went in at session 29). It splits exactly on D18's scope line:

| T9 step | Section (`user32.h`) | Members | New TU |
|---|---|--:|---|
| **9b - the lead** | `//Raw Input functions` `:543-549` | 7 | `rawinput.cpp` |
| 9c | `//System Information functions` `:753-756` | 4 | `systeminfo.cpp` |
| 9d | `//Button Control functions` `:378-380` | 3 | `buttoncontrol.cpp` |
| 9e | `//Synchronization functions` `:779-780` | 2 | `synchronization.cpp` |
| 9f | `//Bitmap functions` `:776` | 1 | `bitmap.cpp` |
| 9g | `//Printing functions` `:783` | 1 | `printing.cpp` |
| 9h - **blocked on a scope decision** | `//Dynamic Data Exchange functions` `:507-540` | 34 | `dde.cpp` or delete |

**State the instrument with the count.** `_tmp_decl_no_def_repo_scan.ps1` reads **18** declared-and-undefined names repo-wide
(`src`/`examples`/`test`); the other 34 are the DDE block, whose declaration lines end `) const;` and whose regex the scanner
misses by design (the header has exactly 34 such lines). `_tmp_static_census.ps1` remains the authority for the published
**52**. U18's coverage instrument keeps its own member count at 635 - quote the instrument with the figure. Census to the end
of 9b-9g: static 582 -> 600, non-static 52 -> 34 (the DDE block only), bindings 581 -> 599 over 48 -> 54 sources.

**What 9b-9g is measured to be:** all 18 are declared in `um\WinUser.h` (in D18 scope) and all 18 name real `user32.dll`
exports - 15 bind an exact bare export name and 3 face an A/W pair only: `GetRawInputDeviceInfo` (hints 1B7/1B8, **9b**),
`SystemParametersInfo` (hints 3A8/3AA, 9c) and `LoadBitmap` (hints 254/255, 9f). Those three stems are `#undef`'d at
`user32.h:16`/`:15`/`:21` and their bare spellings are **not** exported, so `qor_pp_useswinapiAW` is mandatory there (G2 +
G5: a bare binding stringizes a name that does not exist and caches a NULL `pFunc` for the process lifetime). `wvsprintfA`/`wvsprintfW`
(hints 3F8/3F9) are separate exports from the header-inline `wsprintfT` (D20) - the SDK's `#define wvsprintf  wvsprintfW` at
`um\WinUser.h:318` is **live** (the stem is not among the `user32.h` `#undef`s), so keep the A/W spellings and never spell the member `wvsprintf`.

**What 9b measured to be (9 Nov 2026, `_tmp_t9_9b_measure.ps1`, fresh - do not inherit):** 7 members at `user32.h:543-549`, no
blanks, next section `//List Box functions`. `DefRawInputProc` (A5) `um\WinUser.h:15379`, `GetRawInputBuffer` (1B5) `:15293`,
`GetRawInputData` (1B6) `:15206`, `GetRawInputDeviceInfo` **no export row** - its exports are the pair `GetRawInputDeviceInfoA/W`
(1B7/1B8) behind the `#undef` at `user32.h:16` - `GetRawInputDeviceList` (1B9) `:15371`, `GetRegisteredRawInputDevices` (1BC)
`:15357`, `RegisterRawInputDevices` (2F2) `:15349`. So 9b is 6 bare bindings and 1 `qor_pp_useswinapiAW`.

**What 9h owes before it executes:** the DDE block is out of scope twice over - D18's addendum names the **DDE family** as a
superseded exclusion, and 0 of its 34 members are declared in `um\WinUser.h` (they are in `um\ddeml.h`) - yet 34 unusable
non-static members sit on the published class, and D18's direction for U4 is "completion, not deletion". **Wrap-or-delete is a
user decision, not a review action.** Measured against it: `ddeml.h` declares 28 distinct stems, so `FreeDDElParam`/`PackDDElParam`/
`ReuseDDElParam`/`UnpackDDElParam`/`ImpersonateDdeClientWindow` are declared in neither header; the folder's 34 `) const;` are
folder-invented against `ddeml.h`'s unqualified spelling and contradict D18; `mxDdeCreateStringHandle` (`:516`) is a mistyped
name with 0 export rows whose real exports are `DdeCreateStringHandleA/W` (86/87); and `DdeGetQualityOfService` (8F) is exported
and declared nowhere. Repo-wide there are **0 `User32::` mentions of any of the 52**, so no caller constrains the choice.

**What every step owes:** `static` on the declaration together with the definition in the new TU (that closes U4's link gap
and U18's non-static shape); the signature copies the prototype of the export it binds (D18); the binding spelling follows D13
- and D27 measured the direction that matters here: a member that already carries the variant suffix (`CharLowerA`, not the stem
`CharLower`) binds **bare**, because `qor_pp_useswinapiAW` pastes `Stem ## W` at `kernel32.h:77` and would bind `CharLowerA`
to the export `CharLowerW`; the A/W macro is needed only where the header `#undef`s the bare stem. 9a needed 34 bare / 0 A/W;
9b needs 6 bare / 1 A/W.

Coverage stays **759 of 1,018** throughout 9b-9g - all 18 were already declared, so each step closes a `LNK2019` link gap, never
a coverage gap (the arithmetic D24 recorded for T6).

**D21's pin still under every session's feet:** `kernel32.h:22-30` pins `_WIN32_WINNT`/`WINVER` `0x0A00` and `NTDDI_VERSION`
`0x0A000010` (`NTDDI_WIN11_GE`) before `<Windows.h>`, guarded `#ifndef`, with a floor `#error` at `:33-35`. Do not lower or
re-`#define` them; an SDK prototype that vanished behind a `WINVER`/`NTDDI` guard is no longer a U16 finding, while the shrink
macros (`NOGDI`, `NOSYSPARAMSINFO`, `NOUSER`, `NOMSG`, `WINAPI_FAMILY`) stay unpinned and remain findings if a consumer sets them.

**Gate for any edit here:** the new TU standalone at `/W4` exit 0, then the whole folder **49/49** (and up as T9 lands TUs) at
`/W4` exit 0 with no diagnostics, `cl /EP` emitting the expected binding literals against the module literal `L"user32"`, and -
because every step edits the shared `user32.h` - the all-libraries run as a **diff** against a pre-fix run (standing baseline
measured 9 Nov 2026: 1 `C1083` at `opengl/wgl.cpp:31`, diagnostic kinds `C1083`/`C4100`/`C4995`, 166 diagnostic lines, exit
**2**; 116 rsp tokens and the count grows with each new TU). The `/Fo` path must stay attached to its switch. `wsprintfT`
remains the folder's only header-inline member and `library.h`'s `varCall` its only variadic call form.

