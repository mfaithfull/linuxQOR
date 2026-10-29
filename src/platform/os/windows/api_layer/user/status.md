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

- **P1 - six bindings that cannot resolve at run time (confirmed by export table + source line).** Fix these
  in the owning TU's session, converting to `qor_pp_useswinapiAW`:
  - `keyboard.cpp:82` `qor_pp_useswinapi(user32, GetKeyboardLayoutNameT)`, `:243` `VkKeyScanT`,
    `:250` `VkKeyScanExT` - the wrapper's `T` suffix stringizes into the name, and `*T` is not an export.
  - `dialog.cpp:89` `qor_pp_useswinapi(user32, GetDlgItemTextT)` - same class.
  - `combobox.cpp:20` `qor_pp_useswinapi(User32, DlgDirListComboBox)`, `:27` `DlgDirSelectComboBoxEx` - these
    two stems are `#undef`'d at `user32.h:15,16`, so the macro cannot rescue them (G2); the literals are not
    exported while the `W` entries are (live probe). `combobox.cpp` also spells the module token `User32`
    (G4, cosmetic).
- **P2 - seven bindings that work today only because the SDK macro rewrites them; convert to explicit AW
  under D13.** `dialog.cpp` `MessageBox`; `menu.cpp` `GetMenuString`, `InsertMenuItem`; `message.cpp`
  `RegisterWindowMessage`, `SendMessage`, `SendMessageTimeout`, `SendNotifyMessage`.
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
| 5..35 | not started | | | |

**Next session: `clipboard.cpp`** (alphabetical from here; `accelerator.cpp`, `accessibility.cpp`, `brush.cpp`,
`caret.cpp`, `timer.cpp` done 29 Oct 2026). Take the pre-scan P1/P2 entries into account only when their owning TU comes up;
do not fix other TUs outside their own session.

| 2b | `accessibility.cpp` (same day, after user commit `89da3452` added the three missing WinEvent wrappers) | +3 | 0 binding defects; **1 call-site defect found: the three declarations lacked `static`, so every call site was `error C2352`** | Fixed: `user32.h:662-664` made `static` (verified by a compile-only caller probe, before = 3 errors, after = exit 0); TU comments generalised to the four WinEvent entry points. Census re-run: `user32.h` now 524 declarations, 427 static, 97 non-static (was 100) - see U4, and new U13 for the one remaining defined-but-non-static member |


Remaining queue (33 TUs, alphabetical; `accelerator.cpp`, `accessibility.cpp`, `brush.cpp`, `caret.cpp`,
`timer.cpp` done):
clipboard, combobox, console, coordtransform, cursor, desktop, devicecontext, devicemanagement, dialog,
errorhandling, filledshapes, help, hook, icon, ime, keyboard, mdi, menu, message, monitor, mouse, painting,
processthread, rectangle, resource, scrollbar, systemshutdown, window, windowclass, windowprocedure,
windowproperties, winqor_api_user (module glue). `hook.cpp` is now in the build (see U3).

## Review method (validated on accelerator.cpp)

1. Read the TU; pair every definition against its declaration in `user32.h` (return type, parameter types, `T` suffix).
2. Compare each wrapper signature with the SDK prototype in `winuser.h` and with the export table in
   `tmp/_tmp_user32_exports.txt` (`dumpbin /exports` of `C:\Windows\System32\user32.dll`, 1018 names).
3. Resolve the binding name the wrapper *actually* asks for - preprocess the TU with `cl /EP` and grep the
   `qor_pp_useswinapi` expansion. Never trust the source spelling; see ground rule G1.
4. Check the call-site idiom against `api_layer/kernel` + `api_layer/shell`: `qor_pp_fcontext;`,
   `Win32ErrorHandler _;`, `qor_pp_useswinapi*`, `Library::Call<RET, Args...>` with explicit template
   arguments, `CheckReturn<...>::TType` where zero/false means failure.
5. Look for an in-tree caller of every wrapper (`src`/`examples`/`test`). Name the consumer chain one level deep -
   which `ui/gui` controller or handler calls it - then stop: whether that controller should itself be called is a
   `winqor_gui` question. Wrappers with no reachable caller are recorded as instances of open item U15.
6. Compile the TU standalone (`tmp/_tmp_compile_one.ps1 <name>.cpp`, `/W4`, must be exit 0).

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
replaced the faulty grep-based U14 count).

Full-library compile after the session-1 edit: 38/38 TUs, `/W4`, exit 0, no diagnostics. Re-run after the session-3
edit: same, 38/38, exit 0, no diagnostics (`tmp/_tmp_cl_all.log`).

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
  `Missing library function.` at run time.
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

- **U13 - one member is defined through `User32::` but declared non-static: `ConsoleControl`.** Written up 29 Oct
  2026 (the session-2b row promised this item but never wrote it). Definition `console.cpp:18`, declaration
  `user32.h:690` `NTSTATUS ConsoleControl(CONSOLECONTROL Command, PVOID ConsoleInformation, DWORD
  ConsoleInformationLength);` - no `static`, unlike its 426 qualified siblings, so a caller would have to construct
  an instance of a class documented as all-static (`docs/agent/windows_code_conventions.md`). It is the only such
  member in the folder (`tmp/_tmp_static_census.ps1`). One-token fix (`static` at the declaration) in `console.cpp`'s
  own session; not touched here. Related: the `CONSOLECONTROL` enum in its signature is QOR-defined at
  `user32.h:19-29` (U10).

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
  (a) was re-measured in the same pass and stands unchanged: 250 tight `Call<BOOL, HWND, UINT_PTR>` against
- **U15 - wrappers that are exported through `QOR_WINAPIUSER` but have no reachable caller in the tree.** Opened
  29 Oct 2026 (session 4, user decision) so that the observation is collected in one place rather than repeated per
  TU: each session adds its instances here and links its session note to this item. A wrapper counts when nothing in
  `src`/`examples`/`test` can reach it, including the case where its only consumer is itself never called (name that
  consumer one level deep and stop - whether the gui side should call it is a `winqor_gui` question, and the `Caret`
  controller is already documented at `docs/ui/windows/30-controllers.md:503-512`). These are **not dead code**: the
  TU is in `add_library`, the class carries `qor_pp_module_interface(QOR_WINAPIUSER)`, so the wrapper is part of the
  published DLL surface. The distinction that matters for the audit is U4's: a U4 member has no definition at all, so
  a caller gets `LNK2019`; a U15 wrapper is fully defined, linked and callable - the tree just never calls it.
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
  Not instances (call sites confirmed by the same grep):
  - `accelerator.cpp` **5/5 live** - `ui/gui/view/handlers/acceleratortable.cpp:57` (`CreateAcceleratorTableT`), `:63`
    (`LoadAcceleratorsT`), `:71` (`DestroyAcceleratorTable`), `:78` (`CopyAcceleratorTableT`), `:83`
    (`TranslateAcceleratorT`).
  - `timer.cpp` + `accessibility.cpp` **3/3 live** - `ui/gui/controllers/windowcontroller.cpp:800,805,810`.
  - `brush.cpp` **1/1 live** - `ui/gui/gdiobjects/brush.cpp:111-116`.

  178 spaced `Call< HBRUSH >`; `caret.cpp` is 8/8 spaced, so nothing in it was retouched.


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

