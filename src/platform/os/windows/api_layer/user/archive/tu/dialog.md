# narrative: dialog.cpp, session 13

## dialog.cpp - session 13 (30 Oct 2026)

- **Scope:** the "Dialog functions" section, `user32.h:469-493` - **22 declarations** (`:470-492`, with the `//NOTE:` at
  `:488`), 22 definitions in the header's order, all `static` (U13: 0 instances). The largest section reviewed so far. The
  TU was 171 lines before this session and 192 after (the 20-line section comment now at `:16-35` records the binding
  evidence and the contracts the wrappers pass through).
- **1 real run-time failure fixed - P1's third instance.** The binding that was `qor_pp_useswinapi(user32, GetDlgItemTextT)`
  (`:89` pre-fix; now `:110`) stringized the **member** name, which is not an export. `/EP` pre-fix literal
  `"GetDlgItemTextT"` -> live probe **MISSING**; post-fix `qor_pp_useswinapiAW(user32, GetDlgItemText)` literal
  `"GetDlgItemTextW"` -> **RESOLVED** (ordinal 151; the `A` half 150 is exported too but is not what a `LPTSTR` signature
  wants here). Same class as the two `combobox.cpp` sites. **Latent rather than live:** `GetDlgItemTextT` has no call site
  anywhere in `src`/`examples`/`test` (U15), so unlike `coordtransform.cpp` (session 8) no shipped path was broken.
- **1 convention conversion (P2 7 -> 6).** `MessageBox` -> `qor_pp_useswinapiAW(user32, MessageBox)` (`:145`).
  **No behaviour change**: the pre-fix `/EP` literal was already `"MessageBoxW"`, because the SDK's
  `#define MessageBox MessageBoxW` (`winuser.h:9244`) rewrites the stem before `_ATXT` stringizes it (G1). Converted
  because P2 tracks bindings that work *only* through that macro; G3's false-positive list named `MessageBox` for exactly
  this reason.
- **13 `AW` + 9 bare, and the 9 bare ones are right for a reason the export table alone cannot show.** AW bindings:
  `:40`, `:47`, `:54`, `:61`, `:68`, `:110`, `:131`, `:145`, `:152`, `:159`, `:167`, `:174`, `:188`. Bare bindings:
  `EndDialog` `:75`, `GetDialogBaseUnits` `:82`, `GetDlgCtrlID` `:89`, `GetDlgItem` `:96`, `GetDlgItemInt` `:103`,
  `GetNextDlgGroupItem` `:117`, `GetNextDlgTabItem` `:124`, `MapDialogRect` `:138`, `SetDlgItemInt` `:181` - all nine have a
  **single** SDK spelling (`winuser.h:5147`, `:5293`, `:5287`, `:5154`, `:5170`, `:5271`, `:5279`, `:11573`, `:5161`), no
  `A`/`W` variant in the export table, and all nine probe **RESOLVED** exactly as written. `DefDlgProcW` (A0) and
  `DefDlgProcA` (9F) are exported as **forwarders to `NTDLL.NtdllDialogWndProc_W`/`_A`**, so the binding resolves through
  `user32.dll` while the code lives in `ntdll` - the first forwarders seen in this review apart from the `win32u` route in
  session 7. The trap in this section is `IsDialogMessage`: its bare stem **is** exported (231) but at the same RVA as
  `IsDialogMessageA`, so binding the stem would hand the ANSI half a `LPTSTR`; the `AW` macro at `:131` is what makes the
  binding match the signature.
- **`MessageBoxTimeout` is the review's second SDK-invisible entry point** (after `ConsoleControl`, session 7): zero hits for
  the name across the installed `Windows Kits\10\Include\10.0.26100.0` tree, yet `MessageBoxTimeoutW` (28A) and
  `MessageBoxTimeoutA` (289) are exported and probe **RESOLVED** while the bare stem is **MISSING**. So the `AW` binding at
  `:167` is the only spelling that can bind, and `user32.h:489`'s six-parameter list has **no SDK prototype to check it
  against** - the export table plus the live probe are the entire authority (U16's mirror case: no version guard to worry
  about because there is no declaration at all).
- **Section coverage (U21's shape again - the header mirrors only part of the SDK's dialog block).** The SDK block is
  `winuser.h:4912-5385` (`#ifndef NOCTLMGR`) plus the message-loop half at `:11233-12072`. This section mirrors 22 of it and
  leaves out: `CheckDlgButton` (`:5223`), `CheckRadioButton` (`:5231`), `IsDlgButtonChecked` (`:5240`) - which `user32.h:372-374`
  declares as **non-static members that no TU defines**, three of U4's 97 - and the DPI-behaviour quartet
  `SetDialogControlDpiChangeBehavior` (`:5340`), `GetDialogControlDpiChangeBehavior` (`:5347`), `SetDialogDpiChangeBehavior`
  (`:5363`), `GetDialogDpiChangeBehavior` (`:5370`), all four exported and probe **RESOLVED** but declared nowhere in this
  library. `DlgDirList`/`DlgDirSelectEx` (`:11580` and following) belong to the same SDK family but are declared in the
  header's list-box section (`:546-547`, non-static, defined by no TU - U4 again); their combo-box counterparts are the
  session-6 pair.
- **Correctness / UB pass: nothing found.** 22/22 declarations match their definitions and 22/22 `Library::Call<>`
  template-argument lists match the definition's parameter types (`_tmp_sigcmp.ps1`), 0 parameter-name divergences for this TU
  and 0 folder-wide (`_tmp_name_divergence_scan.ps1`). The three wrappers that write through caller pointers match the SDK's
  SAL exactly - `GetDlgItemTextT` forwards `LPTSTR`/`int` under `_Out_writes_(cchMax)` + `_Ret_range_(0, cchMax)` with no
  buffer logic of its own, `GetDlgItemInt` the `_Out_opt_ BOOL*`, `MapDialogRect` the `_Inout_ LPRECT` - and the four
  callback-taking wrappers (`CreateDialog*`, `DialogBox*`) hand the caller's `::DLGPROC` straight to `user32` and never
  synthesize one, so the CALLBACK-convention hazard of the enum-callback wrappers does not arise in this TU.
- **3 sites fixed for spelling consistency.** `Call<..., DLGPROC, ...>` -> `Call<..., ::DLGPROC, ...>` at `:41`, `:48`, `:69`;
  the declarations (`user32.h:470`, `:471`, `:474`) and the definitions already spelled it `::DLGPROC`, and `:62`
  (`DialogBoxIndirectParamT`) already did too, so this TU is now internally uniform - no type change, no surface change.
  **Instrument note for every later session:** before this fix those three were exactly the rows `_tmp_sigcmp.ps1` printed as
  `CALL TYPE MISMATCH ... sdk '::DLGPROC' vs call 'DLGPROC'`. That is a string comparison on a spelling, not a type error -
  the review's only sigcmp false-positive class, so a MISMATCH row must be read before it is acted on. Re-run after the fix:
  22/22 clean.
- **Convention counts:** U13 0 of 22 (all `static`, all defined `User32::`-qualified); U14(b) 0 deducing (22/22 explicit,
  spaced style, 0 spacing nits); U17 **1 of this section's 13 A/W families** (`MessageBoxTimeout` `user32.h:489`, no rename per
  the settled policy); U15 **16 of 22** instances; U19 14 LAYER / 24 BYPASS (see the U19 entry); U16 1 version-guarded pair +
  3 type-level opt-out dependencies (see the U16 entry); U1/U2 22 sites, 0 `Win32ErrorHandler`, 0 return checks (settled
  policy) - and worth recording that **17 of the 22 could not use the `CheckReturn` pattern anyway**: only `EndDialog`,
  `IsDialogMessageT`, `MapDialogRect`, `SetDlgItemInt`, `SetDlgItemTextT` return `BOOL`; the rest return handles, `INT_PTR`
  dialog results, `LRESULT`, `UINT` text lengths, a packed `LONG` or a button id where 0 is both a result and a failure.
- **Verified at exit:** `dialog.cpp` compiles standalone at `/W4` **exit 0** (re-run after every edit, last run 30/10/2026
  23:59+), and the whole library compiled **38/38 exit 0** earlier in the session (`tmp/_tmp_cl_all_s13.log`:
  `exit=0 loglines=43`, 38 `.cpp` echoed, 44 objects in `tmp/obj`); post-edit `/EP` re-run yields the 22 binding literals in
  definition order (13 `*W` + 9 bare) and all 22 are **RESOLVED** in the extended probe; `git diff --numstat` for this session is
  `dialog.cpp` **+26/-5** (20 comment lines + 1 blank added; 5 lines changed - 3 `::DLGPROC` spellings + 2 binding macros) with
  **`user32.h` untouched**. (`devicemanagement.cpp` is still in
  `git status` from session 12 - uncommitted at the time of writing, so a working-tree diff of this folder shows both files.)
- **Instruments this session leaves behind:** `tmp/_tmp_pp_dialog.bat`; `tmp/_tmp_binding_names.ps1` (reads the binding
  literals out of a `/EP` dump in definition order - the only practical way to audit a 453k-line preprocessed file);
  `tmp/_tmp_decl_no_def_scan.ps1` / `tmp/_tmp_decl_no_def_repo_scan.ps1` (the declared-but-never-defined census, 64 names into
  `tmp/_tmp_decl_no_def_s13.txt`, reconciled with U4's 97 figure in the U4 entry); `tmp/_tmp_guard_stack_scan.ps1` (the
  generalised U16 guard-stack scan, `-Pattern <regex>` over `winuser.h`, replacing the hard-coded
  `_tmp_sdk_guard_scan.ps1`); and `tmp/_tmp_name_resolve_probe.cpp` extended with 24 dialog spellings plus the 7 exported
  dialog entry points this header does not declare.

