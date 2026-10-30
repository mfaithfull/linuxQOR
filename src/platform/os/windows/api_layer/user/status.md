# winqor_api_user - source review status

Review of `src/platform/os/windows/api_layer/user` (the `User32` wrapper class over `user32.dll`)
for correctness, consistency, safety, undefined behaviour and conformance to the api_layer conventions.

Protocol: **one translation unit per session** - the whole point is not to lose findings between sessions.
Small, unambiguous defects get fixed as they are found; anything that changes library-wide behaviour or
needs a decision goes in the open-items list below and is *not* touched.

This file carries the session log and the open items. `memory-bank/activeContext.md` carries a pointer here.

## Convention decision

- **D13 (adopted 29 Oct 2026, user decision; recorded in `docs/agent/DECISIONS.md`).** Explicit
  `qor_pp_useswinapiAW(<mod>, Stem)` wherever the SDK has an A/W family; the bare `qor_pp_useswinapi` is
  reserved for entry points that are a single export with no A/W variants (`DestroyAcceleratorTable`,
  `ShowWindow`, `GetComboBoxInfo`, ...). Never rely on the SDK's `Stem -> StemW` macro surviving into the
  binding expression (G1). Apply this in every TU as it is reviewed; `accelerator.cpp` already conforms.
- **Return-check / error-handler policy in this library (settled 29 Oct 2026, user decision, session 3).** **Leave
  it as-is: no `Win32ErrorHandler _;` and no `CheckReturn`/`CheckNonZeroHandle` is to be added to these wrappers**,
  even though `api_layer/shell` and `api_layer/kernel` use both and G6 makes `CheckReturn` always active. Each TU
  session still *records* which call sites would take them (U1/U2 instances) so the decision can be revisited with
  complete data, but no session changes behaviour. Consequence to keep in mind while reviewing: a failure that
  Win32 reports as `FALSE`/NULL/0 is silent in this library, and a swallowed SEH fault is indistinguishable from a
  legitimate zero (U7).

## Pre-scan of the not-yet-reviewed TUs (folder-wide, ahead of the per-TU sessions)

`tmp/_tmp_aw_checklist.ps1` cross-checks every `qor_pp_useswinapi` / `qor_pp_useswinapiAW` binding in the
folder against `tmp/_tmp_user32_exports.txt`. It is a *hint list* - the per-TU `/EP` check (method step 3)
stays the authority, since the script cannot see which SDK macros are in scope. Results that matter, all in
TUs still to be reviewed:

- **P1 - bindings that cannot resolve at run time (export table + live probe + source line).** Six at the first scan;
  the two in `combobox.cpp` are now fixed and two more have since been found (`window.cpp:124` in session 6,
  `mouse.cpp:82` in session 8), so **six remain in four TUs**. Fix each in the owning TU's session:
  - `keyboard.cpp:82` `qor_pp_useswinapi(user32, GetKeyboardLayoutNameT)`, `:243` `VkKeyScanT`,
    `:250` `VkKeyScanExT` - the wrapper's `T` suffix stringizes into the name, and `*T` is not an export.
  - `dialog.cpp:89` `qor_pp_useswinapi(user32, GetDlgItemTextT)` - same class.
  - **found in session 6:** `window.cpp:124` `qor_pp_useswinapiAW(user32, CreateWindow)` (inside
    `User32::CreateWindowT`, `window.cpp:121`) asks for `CreateWindowW`, which does not exist. `CreateWindow` is the
    SDK's macro for `CreateWindowEx`; the table has `CreateWindowExA/W` (`:1626,1627`) and no plain `CreateWindow*`
    entry, and the live probe says `CreateWindowW` MISSING / `CreateWindowExW` RESOLVED. Before "fixing" the binding,
    decide whether `CreateWindowT` should exist at all: `CreateWindowExT` (`window.cpp:129`) already wraps the same
    export, so the honest fix is either bind `CreateWindowEx` with a `0` extended-style argument or drop the member.
  - ~~`combobox.cpp:20` `qor_pp_useswinapi(User32, DlgDirListComboBox)`, `:27` `DlgDirSelectComboBoxEx`~~ - **FIXED
    29 Oct 2026 (session 6)**: both stems are `#undef`'d at `user32.h:15,16`, so the macro cannot rescue them (G2); the
    literals are not exported while the `W` entries are (live probe). Now `qor_pp_useswinapiAW(user32, ...)`. The G4
    `User32` module token went in the same pass: 416 of the folder's 428 bindings now spell `user32`, the remaining 12
    are `scrollbar.cpp`'s, for its own session.
  - **found in session 8 (by the new P5 scan, not by the export-table script):** `mouse.cpp:82`
    `qor_pp_useswinapiAW(user32, LPTRACKMOUSEEVENT)` - the parameter **type** name went into the binding slot, so the AW
    macro asks `GetProcAddress` for `LPTRACKMOUSEEVENTW`. Live probe: that name MISSING, and so is `TrackMouseEventW`,
    while bare `TrackMouseEvent` is RESOLVED (`winuser.h:2773`, inside `_WIN32_WINNT >= 0x0400` - a U16 instance for
    that session too). So `mouse.cpp:79-84` needs *both* halves fixed: the stem and the AW-ness (it is a single export,
    so bare `qor_pp_useswinapi(user32, TrackMouseEvent)` is the D13 spelling). Its `Call< BOOL >` is already listed in
    U14(b). Owned by `mouse.cpp`'s session.
- **P2 - seven bindings that work today only because the SDK macro rewrites them; convert to explicit AW
  under D13.** `dialog.cpp` `MessageBox`; `menu.cpp` `GetMenuString`, `InsertMenuItem`; `message.cpp`
  `RegisterWindowMessage`, `SendMessage`, `SendMessageTimeout`, `SendNotifyMessage`.
- **P3 - naming flags from the new T-suffix census (`tmp/_tmp_tsuffix_census.ps1`, session 6).** Two members carry the
  folder's `T` suffix where no A/W export pair exists: `user32.h:144 CreateWindowT` (see the P1 entry) and
  `user32.h:383 CopyCursorT`, defined `cursor.cpp:23`, whose binding is `qor_pp_useswinapi(user32, CopyIcon)`
  (`cursor.cpp:26`) - a correct binding of a genuinely single export, but a wrapper name that hides which export it is
  (the SDK's `CopyCursor` is a `CopyIcon` cast macro, not an entry point). Both are decisions for the `window.cpp` and
  `cursor.cpp` sessions, together with the U17 list.
- **P4 - four exports in the table are forwarders, which the binding question cannot see (found in session 7).** While
  checking whether `ConsoleControl` is one, note that `dumpbin /exports` marks exactly four of the 1018 names as
  `(forwarded to NTDLL.xxx)` - `DefDlgProcA/W` (ordinals 1668/1669 -> `NTDLL.NtdllDialogWndProc_A`/`_W`) and
  `DefWindowProcA/W` (1675/1676 -> `NTDLL.NtdllDefWindowProc_A`/`_W`). They still print a real RVA, and the marker text
  is "forwarded to", **not** the `->` form an earlier grep assumed - that false negative is why the forwarder question
  was re-asked with a fresh `dumpbin`. Forwarding is transparent to `GetProcAddress`, so no binding breaks, but the code
  that finally runs lives in `ntdll.dll`: take it into account in `windowprocedure.cpp`'s session, where U6's
  default-`__cdecl` `fPtr` reinterprets an NTDLL `__stdcall` entry point rather than a `user32.dll` one.
- **P5 - the "wrong stem bound" defect class, and the instrument that finds it (`tmp/_tmp_wrong_stem_scan.ps1`, session
  8, 30 Oct 2026).** `coordtransform.cpp` bound `GetDCEx` in all three of its wrappers. Nothing in the pre-scan or in the
  per-TU method could see it: `GetDCEx` is a real export (ordinal 144), the module token was right, `cl /EP` produced a
  valid string literal, the TU compiles, and the call site type-checks because `Library::Call` never sees a prototype. The
  only check that catches it is comparing the stem inside `qor_pp_useswinapi*` against the member that encloses it. New
  script, folder-wide and cheap: 428 bindings over 38 TUs, **7 flags at the first run, 4 after the session-8 fix**, and of
  those 4 only one is a live defect (`mouse.cpp:82`, added to P1); `cursor.cpp:26` is the P3 correct-binding/odd-name
  case and the two `resource.cpp` flags are the script failing to see an enclosing member because both `Resource`
  definitions are unqualified free functions - i.e. the script also reproduces U11 mechanically. Re-run it in every session
  and treat a new flag as a P1-class finding until disproved.
- Everything else in the folder already either uses `qor_pp_useswinapiAW` (e.g. `ime.cpp`, `mouse.cpp`,
  `text.cpp`, `windowprocedure.cpp`, `windowproperties.cpp`, `windowclass.cpp`) or binds a genuine single
  export. `window.cpp` is the largest surface (85 bindings, 75 bare) and will need the most care.

## Session log

| # | TU | Wrappers | Verdict | Action taken |
|---|---|---|---|---|
| 1 | `accelerator.cpp` | 5 | 0 correctness bugs, 3 fragility/consistency defects | Fixed: bare-stem bindings on the three A/W families made explicit `qor_pp_useswinapiAW` (29 Oct 2026); conforms to D13 |
| 2 | `accessibility.cpp` + `timer.cpp` | 1 + 2 | 0 correctness/UB defects; 1 file-organisation defect (the two files held each other's bodies), 2 style nits | Fixed: swapped the bodies so each file matches its `user32.h` section (was U12), `( pFunc … )` -> `(pFunc …)` at both call sites, added single-export comments. All three bindings confirmed correct-as-written by `cl /EP` + export table; no A/W conversion needed |
| 3 | `brush.cpp` | 1 | 0 correctness/UB/binding defects; 1 convention defect (implicit template args), 1 documentation gap | Fixed: `Call< HBRUSH >` -> `Call<HBRUSH, int>` (explicit SDK arg types - folder norm 382/428, reference `shell`+`kernel` 344/387) + a single-export and handle-ownership comment added. Binding correct as written: no A/W family, `cl /EP` resolves to `"GetSysColorBrush"`, which is the export (table index 1958, ordinal 1C6) |
| 4 | `caret.cpp` | 8 | 0 correctness/UB/binding/convention defects - the cleanest TU so far | Fixed: TU comment only (single-export binding rationale under D13 + the `CreateCaret` bitmap contract). All 8 bindings correct as written; `cl /EP` resolves all 8 to the literal stems, all 8 are exports, and they are the only 8 `*Caret*` entries in `user32.dll`, so the section is complete. U14(b)'s "caret 2" was a miscount - caret.cpp has 0 deducing call sites (re-measured, see U14) |
| 5 | `clipboard.cpp` | 20 | 0 correctness/UB/binding/convention defects in the wrappers - second clean TU after `caret.cpp`, and the first already D13-conformant before the session started | Fixed: `()` -> `(void)` at the three zero-argument definitions (`CloseClipboard`, `CountClipboardFormats`, `GetOpenClipboardWindow`) plus their three `user32.h` declarations (the SDK spells `VOID`; folder norm now 36 defs against 2, 37 decls against 2 - see U14(c)) + a TU comment (bare-stem rationale for the 18 single exports, the one-switch A/W argument, the clipboard affinity/ownership contracts, section coverage and deprecation). All 20 bindings verified correct as written by `cl /EP` + export table |
| 6 | `combobox.cpp` | 3 | **2 real run-time failures** - pre-scan P1: both `DlgDir*` bindings asked `GetProcAddress` for stems `user32.dll` does not export, so those wrappers could only ever throw; plus 2 consistency defects (no `T` suffix on an A/W wrapper, `User32` module token) and 1 documentation gap | Fixed: both bindings now `qor_pp_useswinapiAW(user32, ...)` (`cl /EP` resolves `"DlgDirListComboBoxW"`/`"DlgDirSelectComboBoxExW"`, both RESOLVED by live probe); members renamed `DlgDirListComboBoxT`/`DlgDirSelectComboBoxExT` at `user32.h:551-552` + definitions (user decision; new **U17** tracks the 11 remaining A/W wrappers without the suffix); module token `User32` -> `user32`; TU comment added. `GetComboBoxInfo` correct as written (single export, ordinal 13D, no A/W family). Also produced a **new pre-scan P1 finding** in `window.cpp:124` (`CreateWindowW` MISSING) |
| 7 | `console.cpp` | 1 | 0 correctness/binding/UB defects - and the first wrapper in the review with **no SDK prototype to pair against at all**: neither `ConsoleControl` nor `CONSOLECONTROL` nor the five console-internal structures are declared anywhere in SDK 10.0.26100.0 or 10.0.10240.0. 1 declaration defect (**U13** - the only defined member declared non-static) + 3 nits (dead `#undef WinHelp`, `//Console` section comment, missing comma space) | Fixed: `static` added at `user32.h:690` (U13 closed; caller probe `tmp/_tmp_console_static_probe.cpp` - before = 1 x C2352 exit 2, after = exit 0), `,DWORD` -> `, DWORD`, `//Console` -> `//Console functions`, dead `#undef WinHelp` removed, TU comment added. Binding correct as written: `cl /EP` -> `"ConsoleControl"`, which is the only `*Console*` name in the export table (ordinal 1594, hint 55, RVA 00028FD0, not a forwarder); live probe RESOLVED with `ConsoleControlA`/`ConsoleControlW` MISSING, so the bare stem is the D13 spelling and the missing `T` suffix is correct |
| 8 | `coordtransform.cpp` | 3 | **3 real run-time defects** - all three wrappers bound `GetDCEx`, a real export, so every existing check (export table, `cl /EP`, compile) passed while no wrapper called its own entry point; 0 convention defects (`static` already correct at `user32.h:377-379`) | Fixed: bindings `GetDCEx` -> `ClientToScreen`/`ScreenToClient`/`MapWindowPoints` (`cl /EP` resolves the three literals; live probe RESOLVED with `*A`/`*W` all MISSING, so the bare stem is the D13 spelling) + TU comment (binding rationale, `_Inout_` POINT ownership, the "non-zero `MapWindowPoints` return is a delta, not a failure" contract, section coverage). No header change, no rename. New pre-scan **P5** covers the defect class; new **U18** records `window.cpp`'s `void`-returning point converters |
| 9..37 | not started | | | |

**Next session: `cursor.cpp`** (alphabetical from here; 9 TUs done across sessions 1-8: `accelerator`,
`accessibility`, `brush`, `caret`, `clipboard`, `combobox`, `console`, `timer`, `coordtransform`). Pre-flagged against
`cursor.cpp`: pre-scan **P3** - `user32.h:383 CopyCursorT` binds the single `CopyIcon` export at `cursor.cpp:26`, which
is a *correct* binding of a genuinely single export but a wrapper name that hides it, and it is the T-suffix census'
mirror case inside U17. Expect the cursor/DPI point-mapping family to interact with what session 8 found:
`user32.h:390 GetPhysicalCursorPos` and the `LogicalToPhysicalPoint`/`PhysicalToLogicalPoint` pair at `:188,:191`
(now open item **U18**, defined in `window.cpp`). Take the pre-scan P1/P2/P3/P5 entries into account only when their
owning TU comes up; do not fix other TUs outside their own session.

| 2b | `accessibility.cpp` (same day, after user commit `89da3452` added the three missing WinEvent wrappers) | +3 | 0 binding defects; **1 call-site defect found: the three declarations lacked `static`, so every call site was `error C2352`** | Fixed: `user32.h:662-664` made `static` (verified by a compile-only caller probe, before = 3 errors, after = exit 0); TU comments generalised to the four WinEvent entry points. Census re-run: `user32.h` now 524 declarations, 427 static, 97 non-static (was 100) - see U4, and new U13 for the one remaining defined-but-non-static member |


Remaining queue (29 TUs, alphabetical; `accelerator.cpp`, `accessibility.cpp`, `brush.cpp`, `caret.cpp`,
`clipboard.cpp`, `combobox.cpp`, `console.cpp`, `timer.cpp`, `coordtransform.cpp` done):
cursor, desktop, devicecontext, devicemanagement, dialog,
errorhandling, filledshapes, help, hook, icon, ime, keyboard, mdi, menu, message, monitor, mouse, painting,
processthread, rectangle, resource, scrollbar, systemshutdown, window, windowclass, windowprocedure,
windowproperties, winqor_api_user (module glue). `hook.cpp` is now in the build (see U3).

## Review method (validated on accelerator.cpp)

1. Read the TU; pair every definition against its declaration in `user32.h` (return type, parameter types, `T` suffix).
2. Compare each wrapper signature with the SDK prototype in `winuser.h` and with the export table in
   `tmp/_tmp_user32_exports.txt` (`dumpbin /exports` of `C:\Windows\System32\user32.dll`, 1018 names).
3. Resolve the binding name the wrapper *actually* asks for - preprocess the TU with `cl /EP` and grep the
   `qor_pp_useswinapi` expansion. Never trust the source spelling; see ground rule G1.
 3b. Check **binding-vs-member agreement** with `tmp/_tmp_wrong_stem_scan.ps1` (added after session 8, which found
    three wrappers calling `GetDCEx`). Step 3 proves the bound name resolves; it does not prove the wrapper calls
    the entry point it wraps. Run the scan and expect this TU's sites to be clean. A flag is a defect until disproved -
    the only benign patterns so far are a deliberate alias to a different export (`cursor.cpp` `CopyCursorT` ->
    `CopyIcon`, pre-scan P3) and a definition not qualified with `User32::` (`resource.cpp`, open item U11).
4. Check the call-site idiom against `api_layer/kernel` + `api_layer/shell`: `qor_pp_fcontext;`,
   `Win32ErrorHandler _;`, `qor_pp_useswinapi*`, `Library::Call<RET, Args...>` with explicit template
   arguments, `CheckReturn<...>::TType` where zero/false means failure.
5. Look for an in-tree caller of every wrapper (`src`/`examples`/`test`). Name the consumer chain one level deep -
   which `ui/gui` controller or handler calls it - then stop: whether that controller should itself be called is a
   `winqor_gui` question. Wrappers with no reachable caller are recorded as instances of open item U15.
5b. For every stem whose callers are established, classify the call sites with `tmp/_tmp_bypass_scan.ps1`
   (**LAYER** through `User32::` vs **BYPASS** through the SDK import) and record the split against open item U19.
   **Never trust a raw grep for liveness**: comments must be stripped first (`tmp/_tmp_comment_state.ps1` for one file,
   the bypass scan's own stripper for a sweep) - `ui/gl/windowimpl.cpp` is 57% commented-out code and produced eight false
   bypass hits (see U19's method-hazard note).
6. Compile the TU standalone (`tmp/_tmp_compile_one.ps1 <name>.cpp`, `/W4`, must be exit 0).
7. Note which SDK prototypes the wrappers pair with are emitted only behind a version guard (`#if WINVER >= ...`,
   `#if NTDDI_VERSION >= ...`, `_WIN32_WINDOWS`, `_WIN32_IE`) and which SDK-visible entry points relevant to the
   section are absent from the installed SDK at that guard level. Record them against open item U16 as a per-TU list;
   do **not** change any build-wide version macro - the pin is deferred to the end of the review (user decision,
   session 5).

Tooling in `tmp/` (git-ignored scratch): `_tmp_compile_one.ps1 <name>.cpp` (single-TU `/W4` compile),
`_tmp_compile_all.ps1` (all 38 TUs; baseline 29 Oct 2026 = exit 0, zero warnings),
`_tmp_pp_one.bat <name.cpp>` then grep `DefProc pFunc` in `tmp/_tmp_pp_out.txt` (the resolved binding names),
`_tmp_name_resolve_probe.cpp` + `_tmp_run_probe.bat` (live `GetModuleHandleW`/`GetProcAddress` probe against
the real `user32.dll`), `_tmp_cmake_file_diff.ps1` (disk vs `add_library` file list),
`_tmp_user32_exports.txt` (the export table), `_tmp_static_census.ps1` (declaration-vs-definition census: how many
`user32.h` members are `static`, how many are defined through `User32::`, and which defined members are not `static`
- added in session 3), `_tmp_undefined_static.ps1` (the reverse direction: `static` declarations with no definition
in the folder), `_tmp_u14_census.ps1 [-Dir <api_layer folder>]` (bracket-matching census of every `Call`/`voidCall`
site - explicit vs argument-deducing template arguments, plus the spaced/tight `Call<` split; added in session 4, it
replaced the faulty grep-based U14 count), `_tmp_tsuffix_census.ps1 [-Header <header>] [-Exports <exports.txt>] [-ShowAll]` (added session 6, the U17 instrument: parses the 8-space-indented member declarations of a `user32.h`-style header, strips the folder's trailing `T`, looks `<stem>A`/`<stem>W` up in an export table and reports A/W-family-without-suffix vs suffix-without-A/W; current `user32.h` census 525 parsed = 108 norm + 404 single-export + 11 deviations, plus 2 mirror-case flags), `_tmp_sdk_guard_scan.ps1 [-File <sdk header>] [-Names <name>...]` (added session 6, the U16 instrument: replays the `#if`/`#else`/`#endif` nesting of an SDK header and prints the guard stack open at each named declaration; defaults to `winuser.h` 10.0.26100), `_tmp_wrong_stem_scan.ps1 [-Dir <folder>]` (added session 8, the P5 instrument: pairs every `qor_pp_useswinapi`/`qor_pp_useswinapiAW` binding with the `User32::` member that encloses it and flags any stem that is neither that member nor its `T`-stripped base nor that base plus `A`/`W`; also mechanically reproduces U11 by reporting bindings outside any `User32::` definition), `_tmp_bypass_scan.ps1 -Stems <stems...> [-Roots <dirs...>]` (added session 8, the U19 instrument: comment-stripping scan that classifies every occurrence of a stem in `src`/`examples`/`test` as LAYER (`User32::`), INDIRECT (qualified on an intermediate class) or BYPASS (unqualified / `::`), i.e. going straight to the SDK import), `_tmp_comment_state.ps1 -Path <file> [-Lines <n...>]` (added session 8: character-level block/line-comment and string-literal state machine; says whether given lines are CODE or COMMENT for the compiler and prints the commented-line fraction of the file - the guard against treating grep hits in commented-out regions as live).

Full-library compile after the session-1 edit: 38/38 TUs, `/W4`, exit 0, no diagnostics. Re-run after the session-3
edit: same, 38/38, exit 0, no diagnostics (`tmp/_tmp_cl_all.log`). Re-run after the session-8 binding fix: same, 38/38,
exit 0, no diagnostics - the log's one `warning|error` grep hit is the filename `errorhandling.cpp`, not a diagnostic.

## Ground rules established (measured, not assumed)

- **G1 - the SDK A/W macro rewrites the binding name before it is stringized.** `_ATXT(_NAME)` inside
  `qor_pp_useswinapi` (`kernel32.h:50`) is a *nested* macro call, so `_NAME` is macro-expanded first.
  Proof: `qor_pp_useswinapi(user32, CreateAcceleratorTable)` preprocesses to
  `GetProcAddress(..., "CreateAcceleratorTableW")`. So a bare stem binds the **W** entry point whenever the
  SDK still defines the `stem -> stemW` macro, and only binds the literal stem when that macro has been
  `#undef`'d. `UNICODE`/`_UNICODE` is set from `qor_pp_unicode` at `kernel32.h:7-10`, so the QOR switch and
  the SDK switch are coupled through include order (`user32.h:7` includes `kernel32.h` first).
- **G2 - the nine stems `user32.h:9-17` `#undef`s are the real missing-export risk.** All nine resolve to
  the literal stem, and all nine literals are *not* exported while their `W` variants are (live probe:
  `WinHelp` MISSING / `WinHelpW` RESOLVED; same for `SystemParametersInfo`, `GetRawInputDeviceInfo`,
  `DlgDirList`, `DlgDirSelectEx`, `DlgDirListComboBox`, `DlgDirSelectComboBoxEx`, `LoadBitmap`,
  `RegisterDeviceNotification`). Any wrapper that binds one of those bare stems throws
  `Missing library function.` at run time. Measured status of all nine after session 6: **2 fixed in this review**
  (`DlgDirListComboBox`/`DlgDirSelectComboBoxEx` in `combobox.cpp`, now `qor_pp_useswinapiAW` and renamed `...T`);
  **2 defined and already safe** (`RegisterDeviceNotification` `devicemanagement.cpp:19`, `WinHelp` `help.cpp:35`, both
  `qor_pp_useswinapiAW`, which token-pastes `W` before the SDK macro could be consulted); **5 not defined anywhere in the
  folder** (`SystemParametersInfo` `user32.h:652`, `GetRawInputDeviceInfo` `:540`, `DlgDirList` `:546`, `DlgDirSelectEx`
  `:547`, `LoadBitmap` `:672` - U4 members, so nothing binds them today). Check this list before re-flagging G2 in a
  later session.
- **G3 - the previous audit report is partly wrong.** `tmp/_tmp_audit_report.txt` section A ("run-time
  failures") was produced by grepping the source spelling, so it reported a false failure for every name
  whose SDK A/W macro is still in scope - `CreateAcceleratorTable`, `LoadAccelerators`, `MessageBox`,
  `TranslateAccelerator` and friends all bind correctly today. Only the G2 names, plus genuinely absent
  names such as `GetDlgItemTextT` (the `T` stringizes with it), are real. Re-derive that list per TU with
  method step 3 rather than trusting the report.
- **G4 - module token case is cosmetic.** `_TXT(User32)` stringizes to `L"User32"` and the module lookup is
  case-insensitive: `GetComboBoxInfo` resolved against `User32`, `USER32` and `user32` alike. The
  `combobox.cpp` / `scrollbar.cpp` spelling `User32` is a style break, not a bug.
- **G5 - the "already loaded image" assumption is not universal.** In a bare console process
  `GetModuleHandleW(L"user32")` returned NULL (measured), so `qor_pp_useswinapi` caches a NULL `pFunc` in a
  function-local `static const` and every later call throws even after the DLL is mapped. See item U5.
- **G6 - return checking is silently always on.** `returncheck.h:40` tests
  `qor_pp_parameter_checking == qor_pp_yes`; neither macro is defined anywhere in the repository, so the
  `#if` evaluates `0 == 0` and `CheckReturn<...>::TType` is always the checked wrapper type, never the bare
  return. Adopting `CheckReturn` in a TU therefore changes run-time behaviour (a `continuable(...)` issue is
  raised), and the `#if` itself relies on undefined-macro semantics. See item U2.

## Open items (library-wide, deliberately not fixed)

- **U1 - no call-site error handler anywhere in this library.** **Policy settled 29 Oct 2026 (user): accepted as-is,
  not to be fixed TU by TU** - see the "Convention decision" section. Each session records the instance count only.
  0 of 38 TUs include
  `../kernel/errorhandler.h` or construct `Win32ErrorHandler _;`, which every `kernel`/`shell` wrapper does.
  `IssueHandler<T>`'s ctor `Push()`es the handler onto the flyer chain for its dynamic extent
  (`handler.h:37-45`), so an issue raised inside `Library::Call` (`Library::ExceptionFilterFunction` calls
  `serious(...)` at `library.h:41`) or by a `CheckReturn` has no handler in this library and lands on
  whatever the host process has installed.
- **U2 - return-check policy is absent.** **Policy settled 29 Oct 2026 (user): accepted as-is** - no `CheckReturn`
  is to be introduced by a review session; instances are recorded per TU only.
  Only 4 `CheckReturn` uses exist in the whole library
  (`errorhandling.cpp`, `processthread.cpp`); `shell`/`kernel` apply `CheckReturn<BOOL, BoolCheck>` to every
  `BOOL` and `CheckReturn<HRESULT, LongSuccessCheck>` to every `HRESULT`. With G6 in the way, this is a
  behaviour decision, not a typo - and it is now a made behaviour decision.
- **U3 - `hook.cpp` build-list status: RESOLVED (user commit `05d3ca2f`, 29 Oct 2026), review still open.**
  `add_library` now lists all 38 sources - the earlier "37 of 38" finding was against the pre-commit state.
  `hook.cpp` compiles standalone at `/W4` exit 0 (measured 29 Oct 2026), so its four wrappers (`CallMsgFilterT`,
  `CallNextHookEx`, `SetWindowsHookExT`, `UnhookWindowsHookEx` - declared at `user32.h:316-319`) are now linked
  into the DLL. What remains: the TU is unreviewed (it is back in the queue) and still has no call site in
  `src`/`examples`/`test`, so it is a live-but-unused surface rather than a declaration-without-definition
  (re-measured 29 Oct 2026 - those four wrappers are U15 instances).
- **U4 - `user32.h` declares about 101 non-static members that no `.cpp` defines.** The class is documented
  as a stateless all-static API surface (`docs/agent/windows_code_conventions.md`), yet whole sections are
  plain instance members: String functions (`:336-369`), Button control (`:372-374`), DDE (`:505-530`, some
  carrying a `const` qualifier on a stateless class), Shell/System information (`:642-652`), Authorization
  (`:658-659`), WinEvent hooks (`:662-664`), `LoadBitmap` (`:672`), synchronization (`:675-676`),
  `PrintWindow` (`:679`). Against `:424` static declarations, these have no definition anywhere and no call
  site anywhere in `src`/`examples`/`test` - so calling one is an `LNK2019` at the caller, not a compile
  error. `mxDdeCreateStringHandle` (`:510`) is additionally a mistyped name.
  **Full census re-measured 29 Oct 2026** with `tmp/_tmp_static_census.ps1` + `tmp/_tmp_undefined_static.ps1`:
  `user32.h` has 525 member-declaration lines (8-space indent, non-comment), 427 `static` and 98 not (the session-2b
  row said 524/427/97 - one line adrift, immaterial, the regex is quoted in the script); 426 distinct names are
  defined with a `User32::` qualifier in the folder. Therefore the gap is precise, not "about 101": exactly **2
  `static` declarations have no definition** (`:443 CopyImage`, `:444 LoadImageT` - both because `resource.cpp`
  defines the names unqualified, see U11) and exactly **97 non-static declarations have no definition**. Verified
  for the section adjacent to `brush.cpp`: the four System Information members (`:649-652` `GetSysColor`,
  `GetSystemMetrics`, `SetSysColors`, `SystemParametersInfo`) are defined by no TU in this folder and called by
  nothing in `src`/`examples`/`test`, so `brush.cpp`'s one implemented brush wrapper is not half of a
  half-wrapped pair - `user32.dll` exports exactly one `*Brush*` entry point (`GetSysColorBrush`), so the
  "Brush functions" section (`:654-655`) is complete.
- **U5 - the binding is cached for the process lifetime including failure.** `qor_pp_useswinapi` expands to
  a function-local `static const Library::DefProc pFunc = ...`; the first call wins forever. A process that
  has not mapped `user32.dll` yet (G5), or a transiently missing name, is poisoned permanently.
- **U6 - calling-convention mismatch in the call path (half fixed, committed).** Measured 29 Oct 2026, re-checked
  against HEAD `586f4ad1`: `library.h:16` now reads `typedef int(__stdcall *DefProc)(void);` - that fix is
  **committed** in user commit `89da3452` ("winqor_api_user fixes 2/n"), not sitting in the working tree as the
  session-2 note said. The other half of the defect is untouched: `Call` builds `typedef ret(*fPtr)(MethodArgs...)` (`library.h:33`) and `voidCall`
  builds `void(*fPtr)(MethodArgs...)` (`:55`) - both default `__cdecl` - then reinterprets the `__stdcall`
  `DefProc` through them and calls, so the caller side still uses the wrong convention for a `WINAPI`
  (`__stdcall`) entry point. Inert on x64, the only configured platform (`CMakeSettings.json`: one
  `msvc_x64_x64` profile); UB with stack corruption on any Win32/x86 build. Remaining fix: `WINAPI`-qualify
  `fPtr` in `Call`/`voidCall`. Line numbers re-measured against the working tree: throw at `:39` and `:71`,
  swallow-and-default-return at `:41-49` (`Call`) / `:61-67` (`voidCall`).
- **U7 - SEH faults are swallowed into a default return.** `Call` catches everything and returns `ret _{}`
  (`library.h:62-70`), i.e. `0`/`nullptr`, which for most of this library is indistinguishable from a
  legitimate zero (see U2). The `serious(...)` report is the only trace.
- **U8 - `throw("Missing library function.")` (`library.h:60`, `:92`) throws a `const char*`.** Not derived
  from `std::exception`, and it bypasses the QOR issue facility the rest of the project uses. No caller in
  `ui/gui` catches, so a null binding ends in `std::terminate`.
- **U9 - no `purpose.md` in this folder and no scope/audit note.** `api_layer/shell/purpose.md` and
  `api_layer/gdi/purpose.md` document surface, decisions and verification; `user/` has nothing, and
  `user32.h` carries no header comment recording which subset of `user32.dll` it intends to cover.
- **U10 - `user32.h:19-62` defines `CONSOLECONTROL`, `CONSOLEENDTASK`, `CONSOLEWINDOWOWNER`,
  `CONSOLESETFOREGROUND`, `CONSOLE_PROCESS_INFO`, `CONSOLE_CARET_INFO` at global scope** - undocumented
  console-internal types. Check them against the SDK's own console declarations for redefinition or
  collision in any TU that also reaches the console SDK headers (`console.cpp`, `desktop.cpp`).
  **SDK-side check closed 30 Oct 2026 (session 7): no collision anywhere.** The tokens `CONSOLECONTROL`,
  `_CONSOLEENDTASK`, `CONSOLEWINDOWOWNER`, `CONSOLESETFOREGROUND`, `CONSOLE_PROCESS_INFO`,
  `CONSOLE_CARET_INFO` and `ConsoleControl` occur in **no** header of SDK 10.0.26100.0 (whole `Include\` tree grepped -
  `um\wincon.h:46-48` merely pulls in `consoleapi.h`/`consoleapi2.h`/`consoleapi3.h`, and across those three the only
  hit for the pattern set is `SetConsoleMode` at `consoleapi.h:138`), and none in 10.0.10240.0 either. Being a whole-tree
  result, it settles the collision question for `desktop.cpp` as well; what remains is the consequence of the types
  being the **only** definition in the build: (i) they carry no Microsoft authority, so their layout and the enum
  numbering cannot be verified locally (see the `console.cpp` section); (ii) `Reserved1`, `Reserved2`, `Reserved3` are
  unqualified global enumerators in a published header, so any consumer that declares its own global `Reserved1` stops
  compiling; (iii) the structures at `:52-62` declare members through the SDK's empty `IN` macro, so the header only
  parses after the SDK headers have been seen (`/EP` of `console.cpp` shows `IN DWORD dwProcessID;` ->
  ` DWORD dwProcessID;`) - harmless today because `user32.h:7` includes `kernel32.h` first, but it makes the type block
  include-order dependent. The decision on the types themselves (namespace them under `qor::win::api`, spell out the
  `IN` markers, or drop the console-internal surface altogether) is still the user's.
- **U11 - `resource.cpp` defines both Resource members without the `User32::` qualifier.** `resource.cpp:16`
  `CopyImage` and (re-measured 29 Oct 2026 in the `brush.cpp` session) `resource.cpp:23` `LoadImageT` are both free
  functions in `qor::win::api`, while `user32.h:443`/`:444` declare `User32::CopyImage` / `User32::LoadImageT` as
  static members - so both declared members have no definition (any caller would hit `LNK2019`) and both free
  functions are dead (re-verified: no call site for either name anywhere in `src`/`examples`/`test`). The earlier
  note here claimed `:23` was qualified correctly - it is not; the working tree reads `HANDLE LoadImageT(HINSTANCE,
  LPCTSTR, UINT, int, int, UINT)`. Two-token fix (`User32::CopyImage`, `User32::LoadImageT`), to be applied in that
  TU's session. Bindings there are otherwise sound:
  `CopyImage` is a single export with no A/W family (bare is right), `LoadImage` uses AW -> `LoadImageW` (yes).

- **U12 - `accessibility.cpp` and `timer.cpp` held each other's wrappers - FIXED 29 Oct 2026 (session 2).**
  Before the fix, `accessibility.cpp` defined only `KillTimer`/`SetTimer` (`user32.h:667-669` "//Timer
  functions") while `timer.cpp` defined only `NotifyWinEvent` (`user32.h:661-665` "//Active Accessibility
  functions") - a transposition against the folder convention of one file per `user32.h` section (`brush.cpp` =
  "Brush functions", `caret.cpp` = "Caret functions", `monitor.cpp` = "Monitor functions"). The bodies were
  swapped so each file now matches its name and its header section. Zero behaviour change: both TUs were already
  in `add_library`, all three wrappers are live from `ui/gui/controllers/windowcontroller.cpp:800/:805/:810`, and
  no header, export name or `CMakeLists.txt` entry is involved. Verified afterwards: `/EP` shows
  `accessibility.cpp` -> `"NotifyWinEvent"` and `timer.cpp` -> `"KillTimer"`, `"SetTimer"`; both TUs compile
  standalone `/W4` exit 0; full library 38/38 exit 0, no diagnostics. Related but still open: the accessibility
  section's three unimplemented members (`SetWinEventHook`, `UnhookWinEvent`, `IsWinEventHookInstalled`) remain
  declared-and-undefined (U4).

- **U13 - one member was defined through `User32::` but declared non-static: `ConsoleControl` - FIXED 30 Oct 2026
  (session 7).** Written up 29 Oct 2026 (the session-2b row promised this item but never wrote it). Definition
  `console.cpp:18`, declaration `user32.h:690` read `NTSTATUS ConsoleControl(CONSOLECONTROL Command, PVOID
  ConsoleInformation,DWORD ConsoleInformationLength);` - no `static`, unlike its 426 qualified siblings, so a caller had
  to construct an instance of a class documented as all-static (`docs/agent/windows_code_conventions.md`), and the
  natural qualified call was a compile error: measured with `tmp/_tmp_console_static_probe.cpp`, before the fix
  `User32::ConsoleControl(ConsoleSetWindowOwner, &owner, sizeof(owner))` gave **1 x C2352** ("a call of a non-static
  member function requires an object"), exit 2; after adding `static` the same probe compiles `/W4` exit 0. Fixed in the
  owning TU's session as planned: `static` added, plus the missing space after the second comma and the section comment
  `//Console` -> `//Console functions` (every other section comment in the header ends in "functions"). Census re-run
  after the fix: 525 member-declaration-shaped lines, **428** `static` (was 427), 426 distinct defined names, **zero**
  defined-but-non-static members. U4's gap is unchanged in substance - the non-static declaration count moves 98 -> 97
  and all 97 of them are still declared-and-undefined, which is exactly the figure U4 already carried.
  Related and still open: the `CONSOLECONTROL` enum in its signature is QOR-defined at `user32.h:19-29` (U10).

- **U14 - `Library::Call`/`voidCall` template-argument style is split two ways.** Measured 29 Oct 2026 over all 428
  call sites in this folder.
  (a) **Inner spacing**: 250 sites `Call<BOOL, HWND, UINT_PTR>` against 178 sites `Call< HBRUSH >`. No majority, so
  this is *not* churned per TU - the conventions say "match local file style when editing legacy code", and a
  58/42 split is not a norm. Only lines a session already edits get tightened (brush.cpp's single site, 29 Oct
  2026). Settle it with one dedicated folder-wide pass, or pin it in `docs/skills/QORconventions.md`.
  (b) **Explicitness: re-measured 29 Oct 2026 (session 4) - the earlier "46 bare sites" was a miscount.**
  `tmp/_tmp_u14_census.ps1` now matches angle brackets and parentheses and compares TOP-LEVEL template arguments
  against call arguments per site (a site is bare only when arguments are actually being deduced; zero-argument
  calls like `Call< BOOL >(pFunc)` and `voidCall` sites, which take no return type, are complete). Over the 428
  `Call`/`voidCall` sites in this folder: **420 explicit, 8 deducing** - in `desktop.cpp:83,104`,
  `keyboard.cpp:69,76`, `mouse.cpp:83`, `window.cpp:118,254,317`. No site over-specifies its types. The old figure
  counted `Call< UINT >(pFunc, nBuff, lpList)`-style sites correctly but also swept in every zero-argument call and
  every `voidCall`, which is why whole clean files (caret 2, clipboard 7, menu 4, message 7, systemshutdown 1,
  processthread 1, dialog 1, cursor 1) appeared in the list: those files have **0** deducing sites. `caret.cpp`
  itself is 8/8 explicit (verified by hand at `caret.cpp:26,33,40,47,54,61,68,75`).
  Deduction always yields the *wrapper's* declared parameter types, so it is type-correct whenever the wrapper
  matches the SDK - but it stops pinning the intended types and stops self-documenting the signature, which is what
  the explicit form is for (`memory-bank/systemPatterns.md` rule 2: "passes SDK argument types verbatim"). The
  reference libraries carry **no deducing site at all** (same script: `shell` 60/60 explicit, `kernel` 327/327 for
  `Call`/`voidCall`; their `inlineCall` sites are not counted by this script). Fix the 8 sites in each owning TU's
  own session.
  (a) was re-measured in the same pass and stands unchanged: 250 tight `Call<BOOL, HWND, UINT_PTR>` against 178 spaced `Call< HBRUSH >`; `caret.cpp` is 8/8 spaced and `clipboard.cpp` 20/20 spaced, so neither was retouched.
  (c) **Zero-argument parameter lists are split `(void)` / `()`.** Measured 29 Oct 2026 over the 38 zero-argument members: **33 of 38 definitions and 34 of 39 `user32.h` declarations** write `(void)` (the SDK spells it `VOID`), 5 write `()` - `clipboard.cpp CloseClipboard`/`CountClipboardFormats`/`GetOpenClipboardWindow`, `keyboard.cpp GetKBCodePage`, `message.cpp InSendMessageEx`. `(void)` is a clear 87% norm, so this is a safe local fix rather than churn: the three clipboard sites were fixed in session 5 (definitions `clipboard.cpp:52,59,115`, declarations `user32.h:109,110,118`). Post-fix census **36 defs `(void)` / 2 `()`**, **37 decls `(void)` / 2 `()`** - both survivors belong to `keyboard.cpp` and `message.cpp`, for their own sessions. `f()` and `f(void)` declare the same type in C++, so nothing behavioural changed.
- **U15 - wrappers that are exported through `QOR_WINAPIUSER` but have no reachable caller in the tree.** Opened
  29 Oct 2026 (session 4, user decision) so that the observation is collected in one place rather than repeated per
  TU: each session adds its instances here and links its session note to this item. A wrapper counts when nothing in
  `src`/`examples`/`test` can reach it, including the case where its only consumer is itself never called (name that
  consumer one level deep and stop - whether the gui side should call it is a `winqor_gui` question, and the `Caret`
  controller is already documented at `docs/ui/windows/30-controllers.md:503-512`). These are **not dead code**: the
  TU is in `add_library`, the class carries `qor_pp_module_interface(QOR_WINAPIUSER)`, so the wrapper is part of the
  published DLL surface. The distinction that matters for the audit is U4's: a U4 member has no definition at all, so
  a caller gets `LNK2019`; a U15 wrapper is fully defined, linked and callable - the tree just never calls it. **Session 8
  found the first exception in the reviewed set:** all three `coordtransform.cpp` wrappers are called in-tree
  (`windowcontroller.cpp:643` `ClientToScreen`, `:648` `ScreenToClient`, `:653` `MapPoints` -> `MapWindowPoints`) and
  reachable one level up (`windowimpl.cpp:1657` in `WindowImpl::GetPos`, `:2218` in `WindowImpl::SetCursorPosWin32`), so
  the `GetDCEx` mis-binding was a live defect in shipped paths, not a dead-code curiosity. That makes liveness worth
  checking before assuming a wrapper's behaviour cannot be observed.
  Instances so far:
  - `caret.cpp` - **all 8** (`CreateCaret`, `DestroyCaret`, `GetCaretBlinkTime`, `GetCaretPos`, `HideCaret`,
    `SetCaretBlinkTime`, `SetCaretPos`, `ShowCaret`). Consumers: `platform::win::Caret`
    (`ui/gui/controllers/a_h/caret.cpp:35-73`, built at `gui/CMakeLists.txt:23`) and
    `WindowController::HideCaret`/`ShowCaret` (`windowcontroller.cpp:661-669`, declared `windowcontroller.h:259-260`)
    - neither the class nor those two methods has a call site in `src`/`examples`/`test`, so the entire caret TU is
    linked-and-exported but never executed by the built app.
  - `hook.cpp` - **all 4** (`CallMsgFilterT`, `CallNextHookEx`, `SetWindowsHookExT`, `UnhookWindowsHookEx`,
    `user32.h:316-319`): linked since user commit `05d3ca2f` (U3), and re-measured 29 Oct 2026 (session 4) - the only
    occurrences of the four names outside `hook.cpp`/`user32.h` are their own declarations, so no caller exists.
  - `clipboard.cpp` - **all 20** (the whole "Clipboard functions" section, `user32.h:107-126`). Re-measured 29 Oct 2026
    (session 5): no `User32::*Clipboard*` token exists anywhere outside `clipboard.cpp`/`user32.h`. The two candidate
    consumers, named one level deep: gui's `ClipboardHandler`
    (`ui/gui/view/handlers/interactive/clipboard.cpp:32-159`) only dispatches the clipboard *messages*
    (`wmDestroyClipboard`, `wmPaintClipboard`, ...) and calls no clipboard wrapper at all - and its slot
    `m_clipboardOwner` (`interactive.h:60`) has **no writer** in the tree, only the read at `interactive.cpp:45`, so the
    handler is never installed (matches the gui track's 50b finding). And `ui/gl` has a **parallel direct-SDK clipboard
    implementation** - `ui/gl/platform.cpp:143-158`/`:170-202` call `OpenClipboard`, `EmptyClipboard`, `SetClipboardData`,
    `GetClipboardData`, `CloseClipboard` as ordinary imports (**9 live sites**, measured 30 Oct 2026 with
    `tmp/_tmp_bypass_scan.ps1` + `tmp/_tmp_comment_state.ps1`), so five of the twenty wrappers have working in-tree
    duplicates that never go through this library (a `winqor_platform` observation, and the first counted instance of open
    item U19). The earlier version of this entry also cited `windowimpl.cpp:2395-2410` - **corrected 30 Oct 2026
    (session 8)**: that block is inside a `/* ... */` region (the comment opened at `:2221` and does not close until
    `:2498`), so it is not compiled. `windowimpl.cpp` is **57% comment lines** (1503 of 2637), which makes grep-based
    liveness claims in that file unreliable in both directions.
  - `combobox.cpp` - **2 of 3** (`DlgDirListComboBoxT` `user32.h:551`, `DlgDirSelectComboBoxExT` `:552`), re-measured
    29 Oct 2026 (session 6): the only occurrences of either name in the whole tree are the declaration and the
    definition. gui implements the same operation a different way - `ComboBoxController::Dir()`
    (`ui/gui/controllers/a_h/comboboxcontroller.cpp:89-94`) posts `CB_DIR` through `User32::SendMessageT`, which is what
    the SDK's `DlgDirListComboBox` does internally - so the pair is dead wrapper surface with a live duplicate above it.
  - `console.cpp` - **1 of 1** (`ConsoleControl`, `user32.h:690`). Re-measured 30 Oct 2026 (session 7): the only
    occurrences of the name anywhere in the tree are the declaration and the definition, and the console-internal types
    at `user32.h:19-62` are referenced by nothing else either. Nothing in `ui/gui` wraps it - there is no console
    controller at all - so it is exported, linked and unused. Worth noting for the audit: until the U13 fix landed it
    could not even be *called* in the natural qualified form (C2352), so this wrapper has never been callable as
    documented.
  Not instances (call sites confirmed by the same grep):
  - `accelerator.cpp` **5/5 live** - `ui/gui/view/handlers/acceleratortable.cpp:57` (`CreateAcceleratorTableT`), `:63`
    (`LoadAcceleratorsT`), `:71` (`DestroyAcceleratorTable`), `:78` (`CopyAcceleratorTableT`), `:83`
    (`TranslateAcceleratorT`).
  - `timer.cpp` + `accessibility.cpp` **3/3 live** - `ui/gui/controllers/windowcontroller.cpp:800,805,810`.
  - `brush.cpp` **1/1 live** - `ui/gui/gdiobjects/brush.cpp:111-116`.
  - `combobox.cpp` `GetComboBoxInfo` **live** - `ui/gui/controllers/a_h/comboboxcontroller.cpp:114,149,184`
    (`ComboBoxController::GetComboBoxInfo`, `GetEditCtrl`, `GetLBCtrl`), reached through
    `WindowFactory::CreateComboBox` (`ui/gui/factories/windowfactory.cpp:236-239`), which
    `examples/experimental/osspecific/windows/editbox/editbox.cpp:86` calls. Cross-reference for the gui track, checked
    here because the wrapper takes the SDK type: the controller passes its own `ComboBoxInfo`
    (`comboboxcontroller.h:34-43`) through `reinterpret_cast<PCOMBOBOXINFO>` at `:114`, and that struct is field-for-field
    `COMBOBOXINFO` (`unsigned long`/`Rect`x2/`unsigned long`/`void*`x3 against `DWORD`/`RECT`x2/`DWORD`/`HWND`x3, with
    QOR's `Rect` four `long` at `ui/gui/view/drawing/rect.h:33-38`); `cbSize` is set before every call
    (`:113,148,183`), which is the entry condition the SDK requires.

- **U16 - the wrappers pair with SDK declarations that the SDK only emits when a version guard passes, and nothing in
  the build pins those version macros.** Opened 29 Oct 2026 (session 5) from `clipboard.cpp`: `winuser.h:5431`/`:5439`
  gate `GetClipboardSequenceNumber` behind `WINVER >= 0x0500` and `:5567`/`:5587` gate `AddClipboardFormatListener`,
  `RemoveClipboardFormatListener` and `GetUpdatedClipboardFormats` behind `WINVER >= 0x0600`, while
  `GetClipboardMetadata` (`:5487-5492`) is Win10-era and unguarded - yet `user32.h:107-126` declares all four wrappers
  unconditionally. No `WINVER`, `_WIN32_WINNT` or `NTDDI_VERSION` is set anywhere in the build (grep of `cmake/`,
  `CMakeLists.txt`, `CMakeSettings.json`: zero hits), so the prototypes are visible only because `sdkddkver.h:227-229`
  defaults `_WIN32_WINNT` to `0x0A00` and `:246-253` defaults `WINVER` to `0x0A00`. A consumer that pins an older
  `WINVER` loses those SDK prototypes while this library's declarations and exports stay, so the same header compiles
  differently per consumer; `GetClipboardMetadata` is the mirror case (present in a 10.0.26100 SDK, absent from an old
  one). Folder-wide exposure is not measured yet - each session records the guarded names in its own TU (review-method
  step 7). **User decision 29 Oct 2026 (session 5): defer the pin.** Nothing is written to `cmake/flags.cmake` during
  the review; each session just lists the version-guarded SDK declarations its own wrappers pair with, and the
  `WINVER`/`_WIN32_WINNT` pin (or a deliberate decision not to pin) is taken once at the end of the review from the
  collected list. Running tally:
  - `clipboard.cpp` - **4 guarded**: `GetClipboardSequenceNumber` (`WINVER >= 0x0500`), `AddClipboardFormatListener`,
    `RemoveClipboardFormatListener`, `GetUpdatedClipboardFormats` (`WINVER >= 0x0600`). Plus 1 reverse case:
    `GetClipboardMetadata` is unguarded in SDK 10.0.26100 but would be absent from an older SDK, and it is the one
    clipboard entry point this folder does not wrap.
  - `combobox.cpp` - **1 guarded**: `GetComboBoxInfo` (`winuser.h:14785`) and the `COMBOBOXINFO` type it takes
    (`:14771`) both sit inside `winuser.h:13594 #if(WINVER >= 0x0500)`; the `DlgDirListComboBox`/
    `DlgDirSelectComboBoxEx` block at `:11643-11687` has no version guard at all, only the `NOUSER`/`NOCTLMGR`
    opt-outs plus the Desktop-family partition. Measured with `tmp/_tmp_sdk_guard_scan.ps1`.
  - `console.cpp` - **0 guarded, and the first TU whose wrapper has no SDK declaration at all to guard**: `ConsoleControl`
    is declared by no header in either installed SDK (U10), so `_tmp_sdk_guard_scan.ps1` has nothing to replay and this
    TU's U16 exposure is total rather than version-dependent - the signature rests on the export table alone. The one
    SDK type in the signature, `NTSTATUS` (`typedef LONG NTSTATUS`, seen at `/EP` line 320947, from `um\winnt.h`), is
    unguarded; the parameter types come from `user32.h:19-62` rather than from any SDK header.
  - `coordtransform.cpp` - **0 guarded**, and the cleanest U16 picture so far: all three SDK declarations (`ClientToScreen`
    `winuser.h:9508-9513`, `ScreenToClient` `:9515-9520`, `MapWindowPoints` `:9556-9563`) sit in the Desktop-family
    partition under nothing but `#ifndef NOUSER` - no `WINVER`/`_WIN32_WINNT` guard at any level, measured with
    `tmp/_tmp_sdk_guard_scan.ps1`. The same is true of the `GetDCEx` declaration (`:8575`) the TU wrongly used before the
    fix, so version guards never had anything to do with that defect. The reverse half of U16 is what this TU does add to:
    the rest of the SDK's coordinate family is `WINVER >= 0x0600`/`>= 0x0603` guarded (`:9522-9537`, `:9539-9554`,
    `:9571-9576`), and this library declares two of those names unconditionally at `user32.h:188,:191` (see U18) - a
    consumer pinning `WINVER` below `0x0600` gets declarations here that the SDK would not have emitted.

- **U17 - A/W wrappers declared without the folder's `T` suffix.** Opened 29 Oct 2026 (session 6) when the two combo
  members were renamed (user decision: rename inside the TU being reviewed, keep the rest together for one
  folder-wide call). Measured with `tmp/_tmp_tsuffix_census.ps1`: of the 525 `user32.h` member declarations, **108**
  face an A/W export pair and carry the suffix (the norm), **404** are single exports with no suffix (correct bare per
  D13), and **11** have an A/W family with no suffix: `MessageBoxTimeout` (`:489`, defined `dialog.cpp:143`),
  `DdeInitialize` (`:519`), `DdeQueryString` (`:525`), `GetRawInputDeviceInfo` (`:540`), `DlgDirList` (`:546`),
  `DlgDirSelectEx` (`:547`), `SendIMEMessageEx` (`:636`, defined `ime.cpp:16`), `WinHelp` (`:646`, defined
  `help.cpp:32`), `SystemParametersInfo` (`:652`), `LoadBitmap` (`:672`), `RegisterDeviceNotification` (`:682`, defined
  `devicemanagement.cpp:16`). **Only 4 of the 11 are implemented** (the ones marked `defined` above); the other 7 are
  U4 declared-and-undefined members, so their spelling is currently academic. The census' mirror case - `T` suffix with
  no A/W family - is 2 members, `CreateWindowT` (`:144`) and `CopyCursorT` (`:383`), see pre-scan P3. Deliberately not
  touched per-TU: renaming is a published-surface change (all four implemented ones are exported through
  `QOR_WINAPIUSER`), and 3 of the 4 already bind correctly through `qor_pp_useswinapiAW`, so today the only cost is that
  the name hides the A/W nature from the reader. Note also that where a member keeps the bare stem **and** the stem is
  `#undef`'d at `user32.h:9-17`, the bare-stem binding is a live failure (G2) - that is exactly the P1 set, which is why
  the U17 decision and the D13 binding fix have to be settled in the same pass per TU.
- **U18 - a wrapper pair whose return type contradicts the SDK: `void` where `user32.dll` returns `BOOL`.** Opened
  30 Oct 2026 (session 8) while mapping the coordinate-transform family for `coordtransform.cpp`. `user32.h:188`
  `static void LogicalToPhysicalPoint(HWND, ::LPPOINT*)` and `:191` `static void PhysicalToLogicalPoint(HWND, ::LPPOINT*)`
  are defined `window.cpp:432-437` and `:453-458` with `Library::voidCall<HWND, ::LPPOINT>`, but the SDK declares both
  returning `BOOL` (`winuser.h:9522-9528` and `:9530-9535`, guarded `WINVER >= 0x0600`) and the export table has both as
  single exports (ordinals 26D, 2AB). `LogicalToPhysicalPoint` returns FALSE when the point is not inside a physical
  display area, which is a case the DPI-aware caller has to handle - the wrapper discards it, and the only two callers,
  `WindowController::LogicalToPhysicalPoint`/`PhysicalToLogicalPoint` (`windowcontroller.cpp:313-315`, `:328-330`), are
  `void` too, so the failure is unrecoverable all the way up. Not a run-time fault (the `voidCall` still calls through a
  correct `__stdcall` signature and the discarded `BOOL` is harmless in the ABI), so it is a surface decision rather than
  a fix: changing `void` -> `BOOL` alters an exported, called member. Also still open in the same family: the SDK's
  `LogicalToPhysicalPointForPerMonitorDPI`/`PhysicalToLogicalPointForPerMonitorDPI` (`winuser.h:9539-9552`, `WINVER >=
  0x0603`) and `WindowFromPhysicalPoint` are exported and declared at `user32.h:211` but the first two are declared
  nowhere in this library. For `window.cpp`'s session.
- **U19 - call sites that reach a `user32.dll` entry point through the SDK import thunk instead of through this layer.**
  Opened 30 Oct 2026 (session 8, user decision: track it as an item and count it per session as callers are traced). A
  wrapper's whole purpose is to route a call through `GetProcAddress` + the flyer/error machinery; when a Windows-specific
  TU calls the bare stem instead, it links against `user32.lib`, bypasses the interception point, and the wrapper's
  liveness (U15) is being satisfied by a code path that does not use it. Instrument:
  `tmp/_tmp_bypass_scan.ps1 -Stems <stems...> [-Roots ...]`, which classifies each occurrence as **LAYER** (`User32::stem`),
  **INDIRECT** (qualified on an intermediate class such as a `ui/gui` controller, which usually lands on LAYER one hop
  down), or **BYPASS** (unqualified or `::stem`), after stripping comments. **Baseline measured at opening:**
  `ClientToScreen`/`ScreenToClient`/`MapWindowPoints` = 3 LAYER + 4 INDIRECT + **0 BYPASS** (clean family);
  `OpenClipboard`/`CloseClipboard`/`GetClipboardData`/`SetClipboardData`/`EmptyClipboard` = **9 BYPASS**, all in
  `ui/gl/platform.cpp:143-202`, vs 1 LAYER (`ui/gui/gdiobjects/brush.cpp:113` for `GetSysColorBrush`). Counting rule for
  later sessions: for each wrapper whose liveness is established in step 5, report the LAYER/BYPASS split for that stem;
  a BYPASS site is recorded here, never "fixed" here, since converting a `ui/gl` call into a layer call is a
  `winqor_platform`/`winqor_gui` change with link-order consequences (the api layer must then be a link dependency of that
  target).
- **Method hazard found with U19 (applies to every liveness claim in this review):** a grep hit is not evidence, because
  large parts of the Windows `ui` tree are commented out. `ui/gl/windowimpl.cpp` is **1503 comment-only lines of 2637
  (57%)**, and the block comments do not nest - a block opened at `:2221` runs to `:2498`, which is why eight
  `ClientToScreen`/`ScreenToClient`/clipboard hits in that file are all dead. `tmp/_tmp_comment_state.ps1 -Path <file>
  -Lines <n...>` settles it with a character-level state machine (block comments, line comments, string and char
  literals) and prints the commented fraction. Two earlier records in this file relied on unfiltered greps and had to be
  corrected (the clipboard U15 entry and its session-5 note).


## accessibility.cpp + timer.cpp - session 2 (29 Oct 2026)

Reviewed together because the two files were transposed - `accessibility.cpp` held the Timer wrappers and
`timer.cpp` held the accessibility wrapper. The bodies were moved so each file matches the `user32.h` section it
is named after (U12, now fixed). Three wrappers, three bindings; each pairs exactly with the SDK prototype and
with a real export, and all three are live.

| Wrapper | Now defined in | SDK prototype (`winuser.h` 10.0.26100) | Resolved binding (`cl /EP`) | Export |
|---|---|---|---|---|
| `NotifyWinEvent(DWORD, HWND, LONG, LONG) -> void` | `accessibility.cpp` | `:13728 VOID WINAPI NotifyWinEvent(DWORD, HWND, LONG, LONG)` | `"NotifyWinEvent"` | yes (ordinal 295) |
| `KillTimer(HWND, UINT_PTR) -> BOOL` | `timer.cpp` | `:7124 BOOL WINAPI KillTimer(HWND, UINT_PTR)` | `"KillTimer"` | yes (ordinal 251) |
| `SetTimer(HWND, UINT_PTR, UINT, TIMERPROC) -> UINT_PTR` | `timer.cpp` | `:7079 UINT_PTR WINAPI SetTimer(HWND, UINT_PTR, UINT, TIMERPROC)` | `"SetTimer"` | yes (ordinal 372) |

- **All three correct as written - no P1/P2 conversion needed.** `winuser.h` declares each entry point directly;
  there is no A/W family and no `stem -> stemW` macro for any of them, and `user32.dll` exports only the bare
  stems (the export table has `KillTimer`/`SetTimer`/`NotifyWinEvent` with no `*A`/`*W` - the only other
  `*Timer` names are `SetCoalescableTimer` and `RIMOnTimerNotification`). Under D13 the bare
  `qor_pp_useswinapi` is therefore the right spelling, and the `/EP` expansion confirms the literal name reaches
  `GetProcAddress`. Same case as session 1's `DestroyAcceleratorTable`; source comments now record it.
- No `T` suffix on any of the three wrappers - correct, since none has an A/W family.
- Signature match is exact against the SDK for declaration and definition alike (`user32.h:665` `NotifyWinEvent`,
  `:668-669` the Timer pair - all three `static`); `Library::Call`/`voidCall` template arguments match the
  parameter types one-for-one, so the `reinterpret_cast` in the call helper is at least type-correct (the
  calling-convention caveat is U6).
- Live callers: `ui/gui/controllers/windowcontroller.cpp:800` (`NotifyWinEvent`), `:805` (`KillTimer`), `:810`
  (`SetTimer`). The caller's `reinterpret_cast<TIMERPROC>(timerFunc)` is sound on x64 -
  `ui/../common/structures.h:415` defines `TimerProc` as `void (stdcall*)(void*, unsigned int, unsigned long
  long, unsigned long)`, same convention and same argument widths as `TIMERPROC` (`winuser.h:109`).
- Fixed (style only): `( pFunc … )` -> `(pFunc …)` at both deviating call sites. The folder norm is now 415 of
  416 sites; the single remaining outlier is `resource.cpp:27`, for that TU's session.
- `return Library::voidCall<…>(…);` inside a `void` wrapper is legal C++ (returning a void expression) and kept
  as found. Note the sibling inconsistency for whoever settles U1/U2: `api_layer/comctl32` uses the same
  `return voidCall` form, every `api_layer/kernel` TU calls `voidCall` as a bare statement.
- Not touched (already logged): no `Win32ErrorHandler _;` (U1), no `CheckReturn<BOOL, BoolCheck>` on `KillTimer`
  or `CheckNonZero<UINT_PTR>` on `SetTimer` (U2 - both signal failure as plain 0, so a failure today is silent);
  `NotifyWinEvent` returns `void`, so a swallowed SEH fault (U7) is completely unobservable; binding cached for
  process lifetime (U5).
- Coverage note: the "//Active Accessibility functions" section (`user32.h:661-665`) has four members and one is
  implemented (`NotifyWinEvent`); `SetWinEventHook`/`UnhookWinEvent`/`IsWinEventHookInstalled` are still
  declared-and-undefined (U4) and are the obvious content for this TU if that surface is ever wanted.
- Verified: `/EP` after the move shows `accessibility.cpp` -> `"NotifyWinEvent"`, `timer.cpp` -> `"KillTimer"`
  and `"SetTimer"`; both TUs compile standalone at `/W4` exit 0; the whole library compiles 38/38 at `/W4`,
  exit 0, no diagnostics.

## accelerator.cpp - the 29 Oct 2026 finding in full

Declarations `user32.h:419-424`, definitions `accelerator.cpp:23-58`: five wrappers, five bindings, each
pairing exactly with the SDK prototype and with a real export.

| Wrapper | SDK entry | Resolved binding (after fix) | Export |
|---|---|---|---|
| `CopyAcceleratorTableT(HACCEL, LPACCEL, int) -> int` | `CopyAcceleratorTableW` | `CopyAcceleratorTableW` | yes (hint 58) |
| `CreateAcceleratorTableT(LPACCEL, int) -> HACCEL` | `CreateAcceleratorTableW` | `CreateAcceleratorTableW` | yes (5E) |
| `DestroyAcceleratorTable(HACCEL) -> BOOL` | `DestroyAcceleratorTable` | `DestroyAcceleratorTable` | yes (AD) - single export, no A/W |
| `LoadAcceleratorsT(HINSTANCE, LPCTSTR) -> HACCEL` | `LoadAcceleratorsW` | `LoadAcceleratorsW` | yes (253) |
| `TranslateAcceleratorT(HWND, HACCEL, LPMSG) -> int` | `TranslateAcceleratorW` | `TranslateAcceleratorW` | yes (3B8) |

- Fixed (fragility, zero behaviour change): `CreateAcceleratorTable`, `LoadAccelerators` and
  `TranslateAccelerator` were bound with the bare stem, which only works because of G1. A `#undef` of any of
  those stems - the header already carries nine of them - would turn the wrapper into a guaranteed run-time
  throw. They now use `qor_pp_useswinapiAW`, which spells in the source the name `GetProcAddress` is given.
  Verified by re-preprocessing the edited TU (the five names above) and by a `cl /W4` compile at exit 0.
- Live callers exist: `ui/gui/view/handlers/acceleratortable.cpp:57` (`CreateAcceleratorTableT`), `:63`
  (`LoadAcceleratorsT`), `:83` (`TranslateAcceleratorT`) - not dead wrappers.
- Not fixed, deferred to U1/U2: no `Win32ErrorHandler _;`, and no `CheckReturn` on the `BOOL` result of
  `DestroyAcceleratorTable` or on the two `HACCEL` results (`CheckNonZeroHandle<HACCEL>` would be the
  `shell`-policy spelling). `CopyAcceleratorTableT`'s `int` is a count and would stay raw.
- Already consistent: license header, include order (`configuration.h` first, Windows headers last),
  `qor_pp_fcontext` in all five, 4-space indent matching the folder, `}}}//qor::win::api` namespace comment,
  and a `T` suffix exactly where an A/W family exists and absent where it does not.



## brush.cpp - session 3 (29 Oct 2026)

The smallest TU in the folder: one wrapper, one binding, and the whole "Brush functions" section
(`user32.h:654-655`). Nothing in it was wrong.

| Wrapper | SDK entry (`winuser.h` 10.0.26100) | Resolved binding (`cl /EP`) | Export |
|---|---|---|---|
| `GetSysColorBrush(int nIndex) -> HBRUSH` | `:9694-9698` `WINUSERAPI HBRUSH WINAPI GetSysColorBrush(_In_ int nIndex)`, guarded `WINVER >= 0x0400` | `"GetSysColorBrush"` | yes (index 1958, ordinal 1C6) |

- **Correct as written - no P1/P2 conversion needed.** `winuser.h` mentions the stem exactly once (the
  declaration), i.e. there is no `GetSysColorBrush -> GetSysColorBrushW` macro, and `user32.dll` exports only the
  bare stem - the export table contains no `*A`/`*W` form of it. Under D13 the bare `qor_pp_useswinapi` is
  therefore the right spelling, and `/EP` proves the literal name reaches `GetProcAddress` (the same
  no-A/W-family case as session 1's `DestroyAcceleratorTable` and session 2's `KillTimer`/`SetTimer`/
  `NotifyWinEvent`). No `T` suffix on the wrapper - correct. The stem is not among the nine `#undef`s at
  `user32.h:9-17`, so G2 does not bite here.
- Declaration/definition/SDK pairing exact: `static HBRUSH GetSysColorBrush(int nIndex)` at `user32.h:655`,
  `HBRUSH User32::GetSysColorBrush(int nIndex)` at `brush.cpp:22`, parameter names match the SDK (`nIndex`).
  The member is `static` (unlike the 97 U4 members) and is qualified correctly (unlike U11).
- **Fixed (convention):** `Library::Call< HBRUSH >(pFunc, nIndex)` passed only the return type, letting
  `MethodArgs...` deduce. Now `Library::Call<HBRUSH, int>(pFunc, nIndex)`, which states the SDK signature and
  matches the dominant idiom (382 of 428 sites in this folder, 344 of 387 in `shell`+`kernel`). Type-correctness
  was already fine - deduction yields the wrapper's own `int` parameter, which is the SDK type - so this is a
  clarity/convention fix with zero behaviour change. It also tightened the inner spacing on that one line; see
  U14(a) for why the rest of the folder's 178 spaced sites were deliberately left alone.
- **Fixed (documentation):** the TU had no comment. It now records (i) why the bare stem is the correct binding
  for this entry point, and (ii) the ownership contract the wrapper hands to its caller - `GetSysColorBrush`
  returns a **system-cached brush owned by USER32**, which must never be `DeleteObject`'d, so returning the raw
  `HBRUSH` (rather than a checked/owned handle) is deliberate. That contract is load-bearing for the one live
  caller, `ui/gui/gdiobjects/brush.cpp:111-116` (`SysBrush::SysBrush`), which does `m_handle = User32::
  GetSysColorBrush(nIndex); m_handle.DontClose(); m_owner = false;` - the `DontClose()` is exactly what the API
  requires. Cross-reference, not re-verified here: `docs/agent/STATE.MD` part-60c walk reports that the raw-handle
  assignment to `m_handle` re-arms `m_needsClose` (`handle.cpp:84`) so the conversion temporary's `~Handle` can run
  `Close()` -> `Kernel32::CloseHandle` on a GDI system-brush handle. If that is right it is a `ui/gui` defect at
  the call site, not in this wrapper, and it belongs to the `winqor_gui` documentation track.
- Section coverage is **complete**: `user32.dll` exports exactly one `*Brush*` entry point (measured against
  `tmp/_tmp_user32_exports.txt`), so there is no un-wrapped brush surface in this library; the other brush calls
  (`CreateBrushIndirect`, `GetBrushOrgEx`, ...) are `gdi32.dll` and live in `api_layer/gdi`.
- Not touched, and now settled policy rather than an open question (see "Convention decision"): no
  `Win32ErrorHandler _;` (U1); no return check although `GetSysColorBrush`
  returns NULL on failure, so a failure today is a silent NULL handle (U2 - the `shell` policy spelling would be
  `CheckNonZeroHandle<HBRUSH>`; G6 means adopting it would change behaviour). Recorded as this TU's instance count:
  1 call site, 0 handlers, 0 return checks. Also left alone as logged: the binding is cached in a function-local
  `static const` for process lifetime (U5); `Call`'s `fPtr` is default `__cdecl` against a `WINAPI` export, so
  this call site is one more instance of the x86 UB in U6; a swallowed SEH fault returns a default `HBRUSH` (U7);
  `returncheck.h` is included and unused here, as in nearly every TU.
- Verified: standalone `/W4` compile of the edited TU exit 0; `/EP` after the edit still resolves
  `Kernel32::GetModuleHandleW(L"user32")` + `"GetSysColorBrush"` and now shows `Library::Call<HBRUSH, int>`;
  whole library 38/38 at `/W4`, exit 0, no diagnostics.


## caret.cpp - session 4 (29 Oct 2026)

The "//Caret functions" section (`user32.h:459-467`): eight wrappers, eight bindings. The TU matches its name and
its header section exactly - the eight caret names are defined in no other file in the folder, so there is no
U12-style transposition here - and it is the first TU reviewed with **nothing wrong in it**.

| Wrapper | SDK prototype (`winuser.h` 10.0.26100) | Resolved binding (`cl /EP`) | Export |
|---|---|---|---|
| `CreateCaret(HWND, HBITMAP, int, int) -> BOOL` | `:9456-9463` `BOOL WINAPI CreateCaret(_In_ HWND, _In_opt_ HBITMAP, _In_ int, _In_ int)` | `"CreateCaret"` | yes (ordinal 5F) |
| `DestroyCaret(void) -> BOOL` | `:9477-9481` `BOOL WINAPI DestroyCaret(VOID)` | `"DestroyCaret"` | yes (AE) |
| `GetCaretBlinkTime(void) -> UINT` | `:9465-9469` `UINT WINAPI GetCaretBlinkTime(VOID)` | `"GetCaretBlinkTime"` | yes (126) |
| `GetCaretPos(LPPOINT) -> BOOL` | `:9502-9506` `BOOL WINAPI GetCaretPos(_Out_ LPPOINT)` | `"GetCaretPos"` | yes (127) |
| `HideCaret(HWND) -> BOOL` | `:9483-9487` `BOOL WINAPI HideCaret(_In_opt_ HWND)` | `"HideCaret"` | yes (200) |
| `SetCaretBlinkTime(UINT) -> BOOL` | `:9471-9475` `BOOL WINAPI SetCaretBlinkTime(_In_ UINT)` | `"SetCaretBlinkTime"` | yes (321) |
| `SetCaretPos(int, int) -> BOOL` | `:9495-9500` `BOOL WINAPI SetCaretPos(_In_ int, _In_ int)` | `"SetCaretPos"` | yes (322) |
| `ShowCaret(HWND) -> BOOL` | `:9489-9493` `BOOL WINAPI ShowCaret(_In_opt_ HWND)` | `"ShowCaret"` | yes (394) |

- **All eight correct as written; neither pre-scan P1 nor P2 touches this TU.** `winuser.h` mentions each stem
  exactly once (the declaration), i.e. no A/W family and no `stem -> stemW` macro, and the export table holds no
  `*A`/`*W` form of any caret name - so under D13 the bare `qor_pp_useswinapi` is the right spelling and `/EP`
  proves the literal name reaches `GetProcAddress` (as in `DestroyAcceleratorTable`, `KillTimer`/`SetTimer`,
  `NotifyWinEvent`, `GetSysColorBrush`). None of the eight stems is among the nine `#undef`s at `user32.h:9-17`,
  so G2 cannot bite here either. No `T` suffix on any wrapper - correct, since none has an A/W family.
- Declaration/definition pairing exact for all eight (`user32.h:460-467` against `caret.cpp:22,29,36,43,50,57,64,71`);
  return types, parameter types **and parameter names** match the SDK verbatim (`hWnd`, `hBitmap`, `nWidth`,
  `nHeight`, `uMSeconds`, `X`, `Y`, `lpPoint`). All eight are declared `static`, so the section is not part of the
  97 U4 non-static members and none of them is the U13 `ConsoleControl` shape.
- **Coverage complete in both directions**: 8 header declarations -> 8 definitions, and `user32.dll` exports exactly
  8 `*Caret*` names -> all 8 wrapped. The only other caret-named things in the header (`ConsoleSetCaretInfo`
  `user32.h:24`, `CONSOLE_CARET_INFO` `:58-62`) are console-internal types, not `user32` exports - see U10.
- **Fixed (documentation only - no line of code changed):** the TU had no comment. It now records why the bare stem
  is the correct binding for all eight entry points (so no later session "converts" them to `qor_pp_useswinapiAW`,
  which would ask `GetProcAddress` for the non-existent `CreateCaretW`), and the `CreateCaret` bitmap contract:
  `hBitmap` is `_In_opt_` (`NULL` = solid system caret), the bitmap is **not** consumed by the call, stays owned by
  the caller and must outlive the caret.

- Everything else already conformed: license header, the folder's include block identical to `brush.cpp`/
  `accelerator.cpp`, `qor_pp_fcontext;` in all eight, 4-space indent with no tabs and no trailing whitespace,
  `}}}//qor::win::api`, and `../returncheck.h` included but unused (folder-wide, logged under U2).
- **U14 contributes nothing from this TU**: 8/8 sites state the full SDK argument list and 8/8 use the spaced
  `Call< BOOL, HWND >` form, so neither half of U14 applies and nothing was retouched. The old U14(b) claim of
  "caret 2" bare sites was an artefact of the previous counting rule (it read `Call< BOOL >(pFunc)` - a complete
  zero-argument call - as if it deduced); U14(b) is corrected and re-measured with `tmp/_tmp_u14_census.ps1`.
- U1/U2 instance count for this TU: **8 call sites, 0 `Win32ErrorHandler`, 0 return checks** (settled policy).
  Under the `shell`/`kernel` policy the seven `BOOL` results would take `CheckReturn<BOOL, BoolCheck>`.
  **`GetCaretBlinkTime` must stay raw** - `0` is a legitimate blink interval (0 = the caret does not blink), not a
  failure - and equally no `CheckNonZero` arm may be added on `SetCaretBlinkTime`, because `SetCaretBlinkTime(0)` is
  the documented way to switch blinking off. So this TU would have 7 checkable sites and 1 deliberately unchecked.
  U5 (binding cached for process lifetime), U6 (8 more `__cdecl` `fPtr` call sites), U7 (a swallowed SEH fault
  returns `FALSE`/`0`, indistinguishable from a real failure) and U8 all apply unchanged.
- **Liveness - the whole caret surface is exported but never executed in-tree: the 8 wrappers are U15 instances.**
  The eight wrappers are reachable
  only through `platform::win::Caret` (`ui/gui/controllers/a_h/caret.cpp:35-73`, built at `gui/CMakeLists.txt:23`)
  and `WindowController::HideCaret`/`ShowCaret` (`windowcontroller.cpp:661-669`, declared `windowcontroller.h:259-260`)
  - and neither the `Caret` class nor those two methods has a single call site in `src`/`examples`/`test`. Linked,
  exported through `QOR_WINAPIUSER`, zero in-tree consumers: a published surface rather than dead code, but nothing
  in the built app exercises it. That is a `winqor_gui` observation, not a defect in this library; the `Caret`
  controller is already documented on the gui track (`docs/ui/windows/30-controllers.md:503-512`, which separately
  flags its un-narrowed `SetBlinkTime` return and the `umseconds` misspelling).
- Cross-boundary cast checked and **sound**: `Caret::GetPosition` hands `reinterpret_cast<LPPOINT>(&pt)`
  (`a_h/caret.cpp:57`) to `GetCaretPos`, where `pt` is `qor::platform::win::Point` = standard-layout
  `struct { long m_x; long m_y; }` (`ui/gui/view/drawing/point.h:31-36`) - layout-compatible with
  `POINT {LONG x; LONG y;}`, so the write-back is well-defined on the current definition (same class of check as
  session 2's `TIMERPROC` note). Any future member, base or virtual added to `Point` would break it silently.
- Verified: `/EP` after the edit still resolves exactly the eight literal names in the table above; `caret.cpp`
  compiles standalone at `/W4` exit 0; whole library 38/38 at `/W4`, exit 0, no diagnostics
  (`tmp/_tmp_cl_all.log`).

## clipboard.cpp - session 5 (29 Oct 2026)

The "//Clipboard functions" section (`user32.h:106-126`): twenty wrappers, twenty bindings, in exactly the header's
order. Second TU reviewed with nothing wrong in the wrappers (`caret.cpp` was the first) and the first that was already
**fully D13-conformant before the session started** - both A/W families bound with `qor_pp_useswinapiAW` and carrying
the `T` suffix, the other eighteen genuine single exports.

| Wrapper (definition line in the edited file) | SDK prototype (`winuser.h` 10.0.26100) | Resolved binding (`cl /EP`) | Export |
|---|---|---|---|
| `AddClipboardFormatListener(HWND hwnd) -> BOOL` (`:38`) | `:5568-5572` `_In_ HWND` [WINVER>=0x0600] | `"AddClipboardFormatListener"` | yes (1) |
| `ChangeClipboardChain(HWND hWndRemove, HWND hWndNewNext) -> BOOL` (`:45`) | `:5459-5464` `_In_` both | `"ChangeClipboardChain"` | yes (25) |
| `CloseClipboard(void) -> BOOL` (`:52`) | `:5424-5428` `CloseClipboard(VOID)` | `"CloseClipboard"` | yes (4F) |
| `CountClipboardFormats(void) -> int` (`:59`) | `:5510-5514` `(VOID)` | `"CountClipboardFormats"` | yes (5C) |
| `EmptyClipboard(void) -> BOOL` (`:66`) | `:5542-5546` `(VOID)` | `"EmptyClipboard"` | yes (E8) |
| `EnumClipboardFormats(UINT format) -> UINT` (`:73`) | `:5516-5520` `_In_ UINT` | `"EnumClipboardFormats"` | yes (F8) |
| `GetClipboardData(UINT uFormat) -> HANDLE` (`:80`) | `:5473-5477` `_In_ UINT` | `"GetClipboardData"` | yes (136) |
| `GetClipboardFormatNameT(UINT format, LPTSTR lpszFormatName, int cchMaxCount) -> int` (`:87`) | `:5522-5540` A/W pair, `_Out_writes_(cchMaxCount)`, macro `:5537/:5539` | `"GetClipboardFormatNameW"` | yes (138) |
| `GetClipboardOwner(void) -> HWND` (`:94`) | `:5441-5445` `(VOID)` | `"GetClipboardOwner"` | yes (13A) |
| `GetClipboardSequenceNumber(void) -> DWORD` (`:101`) | `:5433-5437` `(VOID)` [WINVER>=0x0500] | `"GetClipboardSequenceNumber"` | yes (13B) |
| `GetClipboardViewer(void) -> HWND` (`:108`) | `:5453-5457` `(VOID)` | `"GetClipboardViewer"` | yes (13C) |
| `GetOpenClipboardWindow(void) -> HWND` (`:115`) | `:5561-5565` `(VOID)` | `"GetOpenClipboardWindow"` | yes (191) |
| `GetPriorityClipboardFormat(UINT* paFormatPriorityList, int cFormats) -> int` (`:122`) | `:5554-5559` `_In_reads_(cFormats)` | `"GetPriorityClipboardFormat"` | yes (1AC) |
| `GetUpdatedClipboardFormats(PUINT lpuiFormats, UINT cFormats, PUINT pcFormatsOut) -> BOOL` (`:129`) | `:5580-5586` `_Out_writes_(cFormats)` [WINVER>=0x0600] | `"GetUpdatedClipboardFormats"` | yes (1D8) |
| `IsClipboardFormatAvailable(UINT format) -> BOOL` (`:136`) | `:5548-5552` `_In_ UINT` | `"IsClipboardFormatAvailable"` | yes (230) |
| `OpenClipboard(HWND hWndNewOwner) -> BOOL` (`:143`) | `:5418-5422` `_In_opt_ HWND` | `"OpenClipboard"` | yes (29C) |
| `RegisterClipboardFormatT(LPCTSTR lpszFormat) -> UINT` (`:150`) | `:5494-5508` A/W pair (`LPCSTR`/`LPCWSTR`), macro `:5505/:5507` | `"RegisterClipboardFormatW"` | yes (2E2) |
| `RemoveClipboardFormatListener(HWND hwnd) -> BOOL` (`:157`) | `:5574-5578` `_In_ HWND` [WINVER>=0x0600] | `"RemoveClipboardFormatListener"` | yes (301) |
| `SetClipboardData(UINT uFormat, HANDLE hMem) -> HANDLE` (`:164`) | `:5466-5471` `_In_ UINT`, `_In_opt_ HANDLE` | `"SetClipboardData"` | yes (328) |
| `SetClipboardViewer(HWND hWndNewViewer) -> HWND` (`:171`) | `:5447-5451` `_In_ HWND` | `"SetClipboardViewer"` | yes (329) |


- **All twenty correct as written; neither pre-scan P1 nor P2 touches this TU.** No clipboard stem is among the nine
  `#undef`s at `user32.h:9-17`, so G2 cannot bite here, and `winuser.h` defines a `stem -> stemW` macro for exactly two
  clipboard names (`:5505/:5507` and `:5537/:5539`) - both of which are already bound explicitly, so G1 is not
  load-bearing anywhere in this file. `/EP` after the edits resolves 18 literal stems plus `GetClipboardFormatNameW`
  and `RegisterClipboardFormatW`; every one is in the `user32.dll` export table with the ordinal shown above.
- **`T` suffix correct by construction**: present on exactly the two A/W families, absent on the other eighteen - the
  inverse of the P1 state in `keyboard.cpp`/`dialog.cpp`, where `T`-suffixed wrappers are bound bare.
- **A/W consistency argument (new, worth keeping):** `qor_pp_unicode` is 1 (`src/configuration/configuration.h:13`),
  so `kernel32.h:7-10` defines `UNICODE`/`_UNICODE`; `qor_pp_useswinapiAW` (`kernel32.h:52`) then picks the `W` entry
  point **and** `TCHAR`/`LPTSTR`/`LPCTSTR` are wide. One switch drives both halves, so a `*T` wrapper cannot end up
  wide-named with narrow parameters. `LPCTSTR` in `RegisterClipboardFormatT` is const (`LPCWSTR` in this build) exactly
  as the SDK's `_In_ LPCWSTR` wants, and `LPTSTR` in `GetClipboardFormatNameT` is non-const as its `_Out_writes_`
  parameter requires.
- Signature pairing exact against the SDK for all twenty - return type, parameter types **and parameter names**
  (`hwnd`, `hWndRemove`, `hWndNewNext`, `uFormat`, `hMem`, `format`, `lpszFormatName`, `cchMaxCount`,
  `paFormatPriorityList`, `cFormats`, `lpuiFormats`, `pcFormatsOut`, `lpszFormat`, `hWndNewOwner`). All twenty are
  declared `static`, so the section is not part of U4's 97 and none has U13's shape; `tmp/_tmp_static_census.ps1`
  re-run after the edits still reports exactly one defined-but-non-static member folder-wide (`ConsoleControl`).
- **Fixed (convention, U14(c)):** `()` -> `(void)` at `clipboard.cpp:52,59,115` and the matching declarations
  `user32.h:109,110,118` - the SDK spells these `(VOID)` and 33 of the folder's 38 zero-argument definitions already
  did. Zero behaviour change.
- **Fixed (documentation):** the TU had no comment. It now records the bare-stem rationale for the eighteen, the
  one-switch A/W argument, the affinity/ownership contracts (`OpenClipboard` takes the clipboard for the calling
  thread's task and must be matched by `CloseClipboard` on that thread; `EmptyClipboard` needs ownership;
  `SetClipboardData` transfers `hMem` to the system only on success; `GetClipboardData` is a borrowed system-owned
  handle) and the coverage/deprecation note.
- **Section complete in both directions:** 20 header declarations -> 20 definitions, none of them defined in another TU
  (no U12 transposition, no U4 gap, no U11 unqualified definition); and 20 of the 21 clipboard entry points
  `winuser.h:5418-5587` declares are wrapped. The one unwrapped SDK entry point is `GetClipboardMetadata`
  (`:5487-5492` + `GETCLIPBMETADATA`). `user32.dll` exports two further clipboard names,
  `CheckProcessForClipboardAccess` and `GetClipboardAccessToken`, that have no public SDK declaration and are therefore
  out of reach of this class by design.
- **U16 opened from this TU:** 4 of the 20 SDK declarations (`GetClipboardSequenceNumber`,
  `AddClipboardFormatListener`, `RemoveClipboardFormatListener`, `GetUpdatedClipboardFormats`) sit behind `WINVER`
  guards, and the build pins no `WINVER`/`_WIN32_WINNT`/`NTDDI_VERSION` anywhere - the prototypes are visible only
  through the SDK default (`sdkddkver.h:227-229`, `:246-253` = `0x0A00`).

- **U1/U2 instance count: 20 call sites, 0 `Win32ErrorHandler`, 0 return checks** (settled policy). Under the
  `shell`/`kernel` policy 11 sites would take a mechanical check (8 `BOOL` -> `CheckReturn<BOOL, BoolCheck>`,
  2 `HANDLE` -> `CheckNonZeroHandle<HANDLE>`, `RegisterClipboardFormatT` -> `CheckNonZero<UINT>`) and 9 must stay raw
  because zero/NULL is a legitimate answer (`CountClipboardFormats`, `EnumClipboardFormats` where 0 ends the
  enumeration, `GetClipboardOwner`, `GetClipboardViewer`, `GetOpenClipboardWindow`, `SetClipboardViewer`,
  `GetClipboardSequenceNumber` where 0 also means "not associated"), while `GetClipboardFormatNameT` is ambiguous by
  contract (0 for a standard format *and* for failure) and `GetPriorityClipboardFormat` is **inverted** - 0 = failure,
  -1 = "none of the listed formats" - so the folder's non-zero idiom would fire on its success path, and
  `IsClipboardFormatAvailable` would raise an issue on every ordinary "not available" answer. This TU is the strongest
  instance of the U2 decision: a mechanical rule would be wrong on a third of the surface.
- **U14 contributes nothing to change:** 20/20 sites state the full SDK argument list (0 deducing) and 20/20 use the
  spaced `Call< BOOL, HWND >` form, so neither half of U14(a)/(b) applies; nothing was retouched on those grounds.
- **U15: all 20 wrappers are instances** (analysis in the U15 list). The notable part is that the tree does do clipboard
  work - through a **parallel direct-SDK path** in `ui/gl` (`platform.cpp:143-158`, `:170-202` - 9 live sites), complete
  with its own `while (!OpenClipboard(...))` retry-on-busy loop, which never reaches `GetProcAddress`. gui's
  `ClipboardHandler` handles the clipboard messages but calls no wrapper, and its slot `m_clipboardOwner` has no writer,
  so it is never installed. *(Corrected 30 Oct 2026, session 8: this entry also cited `windowimpl.cpp:2395-2410` as part
  of that path. It is not - the whole region sits inside a block comment that opens at `windowimpl.cpp:2221` and closes at
  `:2498`, verified with `tmp/_tmp_comment_state.ps1`. The 9 `platform.cpp` sites were re-checked the same way and are all
  CODE. Counted as open item U19's first instance.)*
- U5 (binding cached for the process lifetime), U6 (20 more default-`__cdecl` `fPtr` call sites), U7 (a swallowed SEH
  fault returns `FALSE`/`NULL`/`0`, indistinguishable from a real clipboard failure - and clipboard code is exactly
  where a use-after-`CloseClipboard` fault is plausible) and U8 all apply unchanged. No cross-boundary type work in
  this TU: no QOR struct is reinterpreted as a clipboard SDK struct and `GetUpdatedClipboardFormats`'s buffer is
  caller-owned, so no layout-compatibility check of the session-2/session-4 kind was needed.
- Verified: `/EP` after the edits resolves exactly the twenty names in the table; `clipboard.cpp` compiles standalone
  at `/W4` exit 0; whole library **38/38 at `/W4`, exit 0**, no diagnostics (`tmp/_tmp_cl_all.log`).

## combobox.cpp - session 6 (29 Oct 2026)

The "//Combo Box functions" section (`user32.h:550-553`): three wrappers, three bindings, in header order, and the
first TU in this review that contains **bindings that could never work** - two of the three asked `GetProcAddress` for
a name `user32.dll` does not export, so those two wrappers could only ever throw.

| Wrapper (spelling after this session) | SDK entry (`winuser.h` 10.0.26100) | Binding before -> after (`cl /EP`) | Export |
|---|---|---|---|
| `DlgDirListComboBoxT(HWND, LPTSTR, int, int, UINT) -> int` | `:11643-11665` `DlgDirListComboBoxA/W`, `#define DlgDirListComboBox DlgDirListComboBoxW` at `:11662` (`#undef`'d at `user32.h:15`) | `"DlgDirListComboBox"` MISSING -> `"DlgDirListComboBoxW"` | yes (C3/C2, probe) |
| `DlgDirSelectComboBoxExT(HWND, LPTSTR, int, int) -> BOOL` | `:11667-11687` `DlgDirSelectComboBoxExA/W`, macro at `:11684` (`#undef`'d at `user32.h:16`) | `"DlgDirSelectComboBoxEx"` MISSING -> `"DlgDirSelectComboBoxExW"` | yes (C6/C5, probe) |
| `GetComboBoxInfo(HWND, PCOMBOBOXINFO) -> BOOL` | `:14771-14787` `COMBOBOXINFO` + `WINUSERAPI BOOL WINAPI GetComboBoxInfo(_In_ HWND, _Inout_ PCOMBOBOXINFO)` | `"GetComboBoxInfo"` (unchanged) | yes (13D) |

- **Fixed (correctness).** Both `DlgDir*` bindings are now `qor_pp_useswinapiAW(user32, <stem>)`. The reason the bare
  spelling is fatal *for these two specifically* is that `user32.h:15,16` `#undef` the stems, so G2's macro rescue
  cannot fire and `_ATXT(stem)` stringizes the literal, un-exported name. `qor_pp_useswinapiAW` token-pastes `W`
  before expansion, so the `#undef` is irrelevant to it. Live probe after the fix: all three names RESOLVED.
- **Fixed (naming, user decision).** `user32.h:551-552` and the two definitions now read `DlgDirListComboBoxT` /
  `DlgDirSelectComboBoxExT`, restoring the folder's `T`-suffix norm for A/W wrappers. Zero call sites existed for
  either name anywhere in `src`/`examples`/`test`, so no call-site edits were needed and the rename cannot change
  behaviour in this tree. `GetComboBoxInfo` keeps the bare SDK name (no A/W family; D13 names it as the example).
  The remaining 11 such members are U17, and the mirror case (`T` with no A/W family) is pre-scan P3.
- **Fixed (consistency).** All three bindings' module token `User32` -> `user32` (the `_TXT()` argument of
  `GetModuleHandle`; the run-time lookup is case-insensitive, so this is spelling only - G4). Folder-wide census over
  all 428 bindings: 416 now `user32`, 12 `User32`, all in `scrollbar.cpp`, which keeps them for its own session.
- **Fixed (documentation).** New TU comment: why the third binding is bare, why the pair must be AW, the `_Inout_`
  `lpPathSpec` / `_Out_writes_(cchOut)` `lpString` buffer contracts, the `DDL_*` filter set, the
  `cbSize`-pre-set-and-child-handles-are-control-owned contract of `GetComboBoxInfo`, the `T`-suffix rationale and the
  section-coverage claim.
- **Verified correct as written:** all three return types, parameter types, parameter count and order against the SDK;
  `PCOMBOBOXINFO` is used as the SDK type rather than a QOR stand-in; `qor_pp_fcontext` in all three; 4-space indent,
  file-local includes last, `}}}//qor::win::api`; the whole `//Combo Box functions` section is `static` (unlike the
  adjacent List Box and DDE blocks - U4) and all three members are defined, so this TU is not part of U4's gap.
- **Left alone deliberately:** the tight `Call<...>` spacing, 3/3 explicit template arguments (U14 - no churn); no
  `Win32ErrorHandler _;` and no `CheckReturn` at any of the 3 sites (settled policy, U1/U2 - this TU's tally is 3 call
  sites, 0 handlers, 0 checks), so all three failures are silent today; `returncheck.h` included and unused; the
  binding still caches in a function-local `static const` for process lifetime (U5); `Call`'s `fPtr` is still the
  default-`__cdecl` typedef against `WINAPI` exports - 3 more U6 sites; a swallowed SEH fault returns a default
  `int`/`BOOL` (U7); U8 (no test, no caller) applies, and 2 of the 3 wrappers became U15 instances.
- **Return-contract note for a future return-check decision:** `DlgDirListComboBoxT`'s documented result is a list-box
  index with `-1` on failure, so neither a `CheckReturn` non-zero test nor `!= 0` is valid for it (index 0 is
  legitimate); `DlgDirSelectComboBoxExT` and `GetComboBoxInfo` are plain `BOOL`s where `CheckNonZeroHandle`-style
  checking would be the `shell`-library spelling.
- **Section coverage: complete.** `user32.dll` exports exactly three `*Combo*` entry points (`GetComboBoxInfo` plus the
  two `DlgDir*ComboBox*` families), all wrapped; the combo-box *messages* (`CB_ADDSTRING`, `CB_DIR`, ...) are not
  entry points and are reached through `SendMessageT` from `ui/gui`.
- **Found while cross-checking, not fixed (not this TU): `window.cpp:124`** binds
  `qor_pp_useswinapiAW(user32, CreateWindow)` inside `User32::CreateWindowT` - `CreateWindowW` is MISSING, `CreateWindow`
  being the SDK's macro for `CreateWindowEx`. Now the largest remaining P1 item; see the pre-scan entry.
- Verified: `cl /EP` post-edit resolves `"DlgDirListComboBoxW"`, `"DlgDirSelectComboBoxExW"`, `"GetComboBoxInfo"`
  against `L"user32"`; live probe RESOLVED for all three; `combobox.cpp` compiles standalone at `/W4` exit 0; whole
  library **38/38 at `/W4`, exit 0, 0 diagnostics** (`tmp/_tmp_cl_all.log`) - the header rename broke no other TU.


## console.cpp - session 7 (30 Oct 2026)

The "Console functions" section (`user32.h:689-690`): one wrapper, one binding - and the only TU in the review whose
wrapper has **no SDK declaration at all** to pair against, so method step 2 (SDK prototype comparison) had no target and
the export table became the sole authority for the name.

| Wrapper | Declaration / definition | Resolved binding (`cl /EP`) | Export |
|---|---|---|---|
| `ConsoleControl(CONSOLECONTROL, PVOID, DWORD) -> NTSTATUS` | `user32.h:690` (was non-static - **U13**, now `static`) / `console.cpp:18+` | `"ConsoleControl"` against `L"user32"` | yes - ordinal 1594, hint 55, RVA 00028FD0, real code (not one of the four table forwarders) |

- **Binding correct as written; no P1/P2 conversion needed.** `user32.dll` exports exactly one `*Console*` name (fresh
  `dumpbin /exports`: the `Console` grep returns that single line), so there is no A/W family and no `stem -> stemW`
  macro - live probe: `ConsoleControl` RESOLVED, `ConsoleControlA` MISSING, `ConsoleControlW` MISSING. Under D13 the bare
  `qor_pp_useswinapi` is therefore the right spelling and `/EP` proves the literal name reaches `GetProcAddress`. No `T`
  suffix on the wrapper - correct. The stem is not one of the nine `#undef`s at `user32.h:9-17`, so G2 does not apply,
  and the U17 census classifies it with the 404 correct single-export members.
- **The signature cannot be verified against Microsoft - the headline risk of this TU, not a defect in it.**
  `ConsoleControl`, `CONSOLECONTROL` and the five structures at `user32.h:19-62` appear in **no** header of
  SDK 10.0.26100.0 or 10.0.10240.0 (whole `Include\` tree grepped; `wincon.h` only pulls in
  `consoleapi.h`/`consoleapi2.h`/`consoleapi3.h`, whose sole hit for the pattern set is `SetConsoleMode` at
  `consoleapi.h:138`). Consequences recorded: no collision risk (U10's SDK-side check is now closed), the enum numbering
  (0-7 with `Reserved1/2/3` holes) and the structure layouts rest on the de-facto internal definition rather than on
  anything installed here, and the parameter names (`Command`, `ConsoleInformation`, `ConsoleInformationLength`) are the
  project's own with no authority to check them against. `NTSTATUS` is the one SDK-supplied type in the signature
  (`typedef LONG NTSTATUS`, `/EP` line 320947), so the return really is a 32-bit signed status. A newer console may
  expose control codes past `ConsoleEndTask` (7); nothing installed here can tell us, so the enum is recorded as
  *possibly incomplete* - a documentation gap, not a defect.
- **Fixed (U13, the promised one-token fix).** `static` added at `user32.h:690`. Measured both ways with
  `tmp/_tmp_console_static_probe.cpp` (compile-only caller probe, `ConsoleSetWindowOwner` + `CONSOLEWINDOWOWNER`): before
  = **1 x C2352** "a call of a non-static member function requires an object", exit 2; after = `/W4` exit 0. Census
  re-run after the fix: 525 member-declaration-shaped lines, 428 `static` (was 427), 426 distinct defined names, zero
  defined-but-non-static. `desktop.cpp` no longer needs the U10 collision check - the grep was whole-tree.
- **Fixed (nits).** Missing space `,DWORD` -> `, DWORD` at the declaration; section comment `//Console` ->
  `//Console functions` (every other section comment ends in "functions"); dead `#undef WinHelp` at `console.cpp:14`
  removed - the TU references `WinHelp` nowhere and `user32.h:10` already undefs the stem, so the line was inert in
  every state of this file (`help.cpp:14` carries the same line and keeps it for that TU's session).
- **Fixed (documentation).** New TU comment inside the namespace (the `brush.cpp`/`combobox.cpp` pattern): the
  no-SDK-authority finding, the section-completeness claim, the bare-stem/D13 rationale with the live-probe numbers, the
  caller-owned in/out buffer contract (`ConsoleInformation` must be the structure belonging to the chosen control code,
  `ConsoleInformationLength` its size, nothing allocated or freed by the call), the NTSTATUS success semantics, and the
  console-ownership / access-denied behaviour of the sensitive control codes.
- **Verified correct as written:** return and parameter types against the export's real contract; `Library::Call<NTSTATUS,
  CONSOLECONTROL, PVOID, DWORD>` explicit 1/1 with tight spacing (no U14 churn); `qor_pp_fcontext` present; the include
  block identical to the folder norm (4 QOR includes, then `user32.h`, `../returncheck.h`, `../library.h` last);
  4-space indent; `}}}//qor::win::api`.
- **Left alone deliberately:** no `Win32ErrorHandler _;` and no `CheckReturn` at the single call site (settled policy,
  U1/U2 - this TU's tally is 1 call site, 0 handlers, 0 checks); `../returncheck.h` included and unused; the binding is
  cached in a function-local `static const` for process lifetime (U5); `Call`'s default-`__cdecl` `fPtr` against a
  `WINAPI` export (U6, 1 more site) - and here the callee really is user32's own stub, since `win32u.dll` exports
  `NtUserConsoleControl` and not `ConsoleControl` (probe: MISSING for the latter), so no forwarding is involved.
- **Return-contract note for any future return-check decision (the worst U7 instance so far).** NTSTATUS success is
  `>= 0` (0 = `STATUS_SUCCESS`, positive = informational), so `== 0` is not a valid failure test and `returncheck.h` has
  no NTSTATUS-aware check - only `SuccessCheck`/`TSuccessCheck`, which test `== 0`. Worse, `Library::Call` turns a
  swallowed SEH fault into `NTSTATUS _{}` = **0 = STATUS_SUCCESS**: a fault in this wrapper is not merely silent, it is
  reported as success. If U2 is ever reopened, this is the wrapper that needs a real check class (test `< 0`) and a
  non-zero default.
- **Surface-scope note (feeds U9's missing scope note).** The console surface a caller would actually want is in
  `kernel32.dll` - 135 `*Console*` exports (`AllocConsole`, `GetConsoleWindow`, `ReadConsoleW`, `WriteConsoleW`, ...) -
  and `api_layer/kernel/kconsole.cpp` already wraps it, with the kernel error-handler idiom intact; `user32.dll` has
  exactly one, which is why this section is one wrapper long. Nothing in `src`/`examples`/`test` calls this wrapper
  (U15 instance), and there is no console controller in `ui/gui` that could.
- Verified: `/EP` after the edits still shows `qor_pp_useswinapi` -> `GetModuleHandleW(L"user32")` + `"ConsoleControl"`;
  `console.cpp` compiles standalone at `/W4` exit 0; the caller probe compiles `/W4` exit 0; the whole library compiles
  **38/38 at `/W4`, exit 0, no diagnostics** (`tmp/_tmp_cl_all.log`) - the `static` addition broke no other TU.

---

## coordtransform.cpp - session 8 (30 Oct 2026)

Smallest-but-one TU (3 wrappers, 3 bindings) and the most consequential find of the review so far: **every wrapper in
the file called the wrong entry point.**

| Wrapper (`user32.h`) | Binding as found | Binding after fix | Verdict |
|---|---|---|---|
| `:377 static BOOL ClientToScreen(HWND, ::LPPOINT)` | `qor_pp_useswinapi(user32, GetDCEx)` | `qor_pp_useswinapi(user32, ClientToScreen)` | Fixed - wrong export |
| `:378 static BOOL ScreenToClient(HWND, ::LPPOINT)` | `qor_pp_useswinapi(user32, GetDCEx)` | `qor_pp_useswinapi(user32, ScreenToClient)` | Fixed - wrong export |
| `:379 static int MapWindowPoints(HWND, HWND, ::LPPOINT, UINT)` | `qor_pp_useswinapi(user32, GetDCEx)` | `qor_pp_useswinapi(user32, MapWindowPoints)` | Fixed - wrong export |

- **What the defect actually did.** One binding line, copied into three bodies. `GetDCEx` is
  `HWND GetDCEx(HWND hWnd, HRGN hrgnClip, DWORD flags)` (`winuser.h:8575`), so each call passed the wrapper's `lpPoint`
  as `hrgnClip`, left `flags` as whatever the third argument register held, and returned an `HDC` that `Library::Call`
  truncated to `BOOL`/`int`. Consequences, in order of severity: (a) no coordinate was ever transformed - the caller's
  POINT is untouched; (b) the return value is not the API's, so the `? true : false` collapse in
  `windowcontroller.cpp:643,:648` reported success whenever the HDC's low 32 bits were non-zero; (c) passing a `POINT*`
  where USER expects a region handle makes USER dereference caller memory it does not own, and a successful call acquires
  a device context that nobody ever releases; (d) `ClientToScreen`'s FALSE-on-bad-hWnd channel is gone entirely.
- **Why nothing caught it, and the new instrument.** `GetDCEx` is a genuine export (table index 1830, ordinal 144), the
  module token was already `user32`, `cl /EP` emitted a valid literal, the TU compiles clean at `/W4`, and the call site
  type-checks because `Library::Call` receives a cast function pointer and never sees a prototype. Every check in the
  method up to step 3 therefore passed. The missing check is *binding-vs-member agreement*, now
  `tmp/_tmp_wrong_stem_scan.ps1` (pre-scan P5): 428 bindings over 38 TUs, 7 flags at the first run, 4 after this fix, of
  which one is a live defect (`mouse.cpp:82`, added to P1), one is the known P3 case (`cursor.cpp:26` binds the single
  `CopyIcon` export) and two are the script correctly failing to find an enclosing `User32::` member in `resource.cpp`
  (i.e. it reproduces U11 mechanically).
- **It was not dead code.** Unlike the previous seven TUs, all three wrappers are called in-tree -
  `windowcontroller.cpp:643` (`WindowController::ClientToScreen`), `:648` (`ScreenToClient`), `:653`
  (`WindowController::MapPoints` -> `MapWindowPoints`) - and reachable one level up from `WindowImpl::GetPos`
  (`windowimpl.cpp:1657`) and `WindowImpl::SetCursorPosWin32` (`:2218`). So `GetPos` was returning the untranslated
  client origin and `SetCursorPosWin32` was placing the cursor from unconverted coordinates while leaking a DC per call.
  First non-U15 TU of the review; recorded in the U15 list. (A naive grep suggested `windowimpl.cpp` called the SDK forms
  directly at eight sites - **all eight are inside `/* ... */` blocks** (`:257-268`, `:325-408`, `:530-1301`, `:2042-2210`)
  and are not compiled; C comments do not nest, so the block opened at `:2042` runs to the first `*/` at `:2210`. The two
  live calls at `:1657`/`:2218` go to the private base `WindowController` (`windowimpl.h:38`) and from there through the
  layer, so this family has **0** bypass sites. Measured with `tmp/_tmp_bypass_scan.ps1`; the phenomenon is open item
  **U19** and per-session counting of it is now review-method step 5b.)

- **Binding verification after the fix.** `cl /EP` on the edited TU resolves all three to the literal names with module
  token `L"user32"`: `"ClientToScreen"`, `"ScreenToClient"`, `"MapWindowPoints"`. Live probe against the loaded
  `user32.dll`: all three RESOLVED; `ClientToScreenA/W`, `ScreenToClientA/W`, `MapWindowPointsA/W` all MISSING, so these
  are single exports and the bare `qor_pp_useswinapi` is the D13 spelling (no `AW`, no `T` suffix - the header spellings
  at `:377-379` are correct as written and were not touched). Export table: `ClientToScreen` 1586/4D, `ScreenToClient`
  2282/30C, `MapWindowPoints` 2146/27F; `MapVisualRelativePoints` (2145/27E) sits beside it in the table but is
  undocumented and declared nowhere in this library.
- **SDK pairing.** `winuser.h:9508-9513`, `:9515-9520`, `:9556-9563`; the wrappers' parameter lists match the SDK's
  `_In_ HWND` / `_Inout_ LPPOINT` / `_In_opt_ HWND` x2 / `_Inout_updates_(cPoints) LPPOINT` + `_In_ UINT` exactly, and the
  return types match (`BOOL`, `BOOL`, `int`). **U16: 0 version-guarded declarations** - the cleanest tally so far, only
  `#ifndef NOUSER` plus the Desktop-family partition (measured with `tmp/_tmp_sdk_guard_scan.ps1`). The reverse half of
  U16 (library declarations of SDK names the SDK would withhold) is contributed by the neighbouring `user32.h:188,:191`
  pair, not by this TU.
- **Section coverage.** The header's "Coordinate Transform functions" section is these three. The SDK's wider
  coordinate-conversion neighbourhood (`winuser.h:9522-9554`, `:9565-9576`) lives in other header sections
  (`Window functions`: `WindowFromPoint` `:212`, `WindowFromPhysicalPoint` `:211`, the `Logical/PhysicalTo*` pair
  `:188,:191`; `Cursor functions`: `GetPhysicalCursorPos` `:390`). The two `*Point` converters that *are* defined return
  `void` where the SDK returns `BOOL` - **new open item U18**, for `window.cpp`'s session. The two
  `*ForPerMonitorDPI` variants are not declared anywhere in this library (noted under U18 rather than opened separately).
- **Open items this session contributes to (recorded, not acted on):**
  - U1/U2 - 3 more call sites, `0` with `Win32ErrorHandler _;`, `0` wrapping the result in
    `CheckReturn`/`CheckNonZero`. This TU is the strongest argument yet for reopening them: all three functions have a
    real failure channel and the only two callers already collapse the result with `? true : false`
    (`windowcontroller.cpp:643,:648`), so even with the bindings fixed a failure stays silent - but the value now at least
    means what the SDK says. `MapWindowPoints` is the deliberate counter-example: non-zero is a coordinate delta, so a
    `CheckNonZero` there would be wrong by construction.
  - U3 - none. U4 - none (all three declared, all three defined). U6 - 3 more sites hand a `Library::DefProc`
    (`__stdcall`) to `Library::Call`, whose `fPtr` is default-`__cdecl` (13 known sites now).
  - U7 - applies to all three: a fault inside the callee surfaces as `FALSE`/`0`, indistinguishable from a legitimate
    zero (for `MapWindowPoints` that is exactly the common success case).
  - U8 - untested, but unlike the previous TUs it *is* called in-tree, so a test could observe real behaviour through
    `WindowController::ClientToScreen` rather than only checking that a symbol links.
  - U10, U11 - none. U13 - **none, and this is the first reviewed section whose defined members were all already
    `static`** (`user32.h:377-379`), so no `C2352` caller risk existed here at all.
  - U14 - contributes nothing to change: 3/3 sites state the full SDK argument list explicitly and 3/3 use the spaced
    `Call< BOOL, HWND, ::LPPOINT >` form, which is file-consistent (3/3), so no churn per the no-clobber rule.
  - U17 - nothing to rename: no A/W family, so no `T` suffix is due and none is present.
- **Conventions checked and clean:** licence header, include order, Windows headers last, `qor_pp_fcontext` at all 3
  sites, lowercase `user32` module token, section comment `//Coordinate Transform functions` already in the "ends in
  functions" house style (U14(d) - unchanged), no local `#undef`, 4-space indent, no tabs, definitions in header order.
  Added: the TU comment block (D13 binding rationale, the `_Inout_` caller-owned POINT contract, the "non-zero
  `MapWindowPoints` return is a delta" warning, section coverage, and the U18 pointer).
- **Verified at exit:** `cl /EP` -> the three literals above; `coordtransform.cpp` compiles standalone at `/W4` exit 0;
  the whole library compiles **38/38 at `/W4`, exit 0, no diagnostics** (`tmp/_tmp_cl_all.log`; the single
  `warning|error` grep hit is the filename `errorhandling.cpp`). No header change and no rename, so no published surface
  moved and no caller needed recompiling - the fix changes run-time behaviour only.

