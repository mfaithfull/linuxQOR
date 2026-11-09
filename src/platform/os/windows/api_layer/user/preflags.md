# Folder-level leads - the review queue is closed; T9 (U4's tranche queue) is the open lead

Written wholesale at the T9 step 9c close (9 Nov 2026) and replaced wholesale at every later close. All 38 review-queue sources are `closed` and the twelve coverage TUs `pointer.cpp` (6a) through `legacy.cpp` (6g), `security.cpp` (T1), `listbox.cpp` (T6), `strings.cpp` (T9 9a), `rawinput.cpp` (T9 9b) and `systeminfo.cpp` (T9 9c) are closed with them, so there is **no next review TU** - but **T9 is open**: U4's remaining declared-but-undefined surface, deferred by the user (D26) as a tranche queue in T8's 6a-6g shape, one section / one new TU / one gate diff per session. This file names its next step. **Write-once**: read it once at boot, edit it never, replace it wholesale at the next close.

## Current folder figures (re-measured 9 Nov 2026 post-9c with `_tmp_static_census.ps1`, then `_tmp_decl_no_def_repo_scan.ps1`)

**50 sources / 634 member-declaration lines (593 static, 41 non-static) / 592 binding lines / 1,018 export names / 759 covered / 8 declared-and-unwrapped raw with written exclusions / 251 with no prototype.** `_tmp_static_census.ps1` also prints 0 defined-but-not-static, 0 defined-without-a-declaration and 592 distinct defined member names; the coverage instrument counts 635 members, so quote the instrument with the figure. `user32.h` is 855 lines - 9c moved `static` onto declarations only, so its length is unchanged.

**U4's remaining population is 41 non-static member declarations with no definition** (9a closed its 34, 9b closed 7 and 9c closed 4; the `static` direction of U4 is 0 - U11's pair went in at session 29). It splits exactly on D18's scope line:

| T9 step | Section (`user32.h`) | Members | New TU |
|---|---|--:|---|
| **9d - the lead** | `//Button Control functions` `:378-380` | 3 | `buttoncontrol.cpp` |
| 9e | `//Synchronization functions` `:779-780` | 2 | `synchronization.cpp` |
| 9f | `//Bitmap functions` `:776` | 1 | `bitmap.cpp` |
| 9g | `//Printing functions` `:783` | 1 | `printing.cpp` |
| 9h - **blocked on a scope decision** | `//Dynamic Data Exchange functions` `:507-540` | 34 | `dde.cpp` or delete |

**State the instrument with the count.** `_tmp_decl_no_def_repo_scan.ps1` reads **7** declared-and-undefined names repo-wide (`src`/`examples`/`test`) - re-run post-9c it prints exactly `user32.h:378 CheckDlgButton`, `:379 CheckRadioButton`, `:380 IsDlgButtonChecked`, `:776 LoadBitmap`, `:779 MsgWaitForMultipleObjects`, `:780 MsgWaitForMultipleObjectsEx`, `:783 PrintWindow`; the other 34 are the DDE block, whose declaration lines end `) const;` and whose regex the scanner misses by design (the header has exactly 34 such lines). `_tmp_static_census.ps1` remains the authority for the published **41**. Census to the end of 9d-9g: static 593 -> 600, non-static 41 -> 34 (the DDE block only), bindings 592 -> 599 over 50 -> 54 sources. Coverage stays **759 of 1,018** throughout - these steps close `LNK2019` link gaps, never coverage gaps.

**What 9d-9g is measured to be:** all 7 are declared in `um\WinUser.h` (in D18 scope) and all 7 name real `user32.dll` exports - 6 bind an exact bare export name and 1 faces an A/W pair only: `LoadBitmap` (export rows measured at the 9b close, hints 254/255, 9f). That stem is `#undef`'d at `user32.h:21` and its bare spelling is **not** exported, so `qor_pp_useswinapiAW` is mandatory there (G2 + G5: a bare binding stringizes a name `user32.dll` never exported, caches a NULL `pFunc` for the process lifetime and throws on every call). The other two A/W-only stems D26 named - `SystemParametersInfo` (hints 3A8/3AA) and `GetRawInputDeviceInfo` (1B7/1B8) - went in at 9c and 9b, so they are no longer in the population.

**9d's measurement is owed fresh - nothing was measured for it this session.** 9c's run measured only 9c's 4 members, and no 9c figure is inherited into 9d. Measure `user32.h:378-380` at boot against `um\WinUser.h` (D21's pin: `kernel32.h:22-30` sets `_WIN32_WINNT` 0x0A00, and the folder defines none of the `NO*` shrink macros), check each published signature against the export's prototype (D18), and audit the return convention against the SDK's own `_Success_`/`_Check_return_` claim before choosing any wrapper type (G6).

Gate counts for the next session: the folder is **50 sources** and the all-libraries rsp is **118 tokens** (117 + `systeminfo.cpp`); the standing baseline is 1 `C1083` at `opengl/wgl.cpp:31`, 165 warning lines, exit **2** on the pre-edit and post-edit runs alike - the diff is the signal, never the exit code, and the exit code must be captured inside a `.bat`.
