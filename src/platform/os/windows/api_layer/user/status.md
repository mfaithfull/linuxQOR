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
| 3..36 | not started | | | |

**Next session: `brush.cpp`** (alphabetical from here; `accelerator.cpp`, `accessibility.cpp`, `timer.cpp` done
29 Oct 2026). Take the pre-scan P1/P2 entries into account only when their owning TU comes up; do not fix other
TUs outside their own session.

| 2b | `accessibility.cpp` (same day, after user commit `89da3452` added the three missing WinEvent wrappers) | +3 | 0 binding defects; **1 call-site defect found: the three declarations lacked `static`, so every call site was `error C2352`** | Fixed: `user32.h:662-664` made `static` (verified by a compile-only caller probe, before = 3 errors, after = exit 0); TU comments generalised to the four WinEvent entry points. Census re-run: `user32.h` now 524 declarations, 427 static, 97 non-static (was 100) - see U4, and new U13 for the one remaining defined-but-non-static member |


Remaining queue (35 TUs, alphabetical; `accelerator.cpp`, `accessibility.cpp`, `timer.cpp` done): brush, caret,
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
5. Compile the TU standalone (`tmp/_tmp_compile_one.ps1 <name>.cpp`, `/W4`, must be exit 0).

Tooling in `tmp/` (git-ignored scratch): `_tmp_compile_one.ps1 <name>.cpp` (single-TU `/W4` compile),
`_tmp_compile_all.ps1` (all 38 TUs; baseline 29 Oct 2026 = exit 0, zero warnings),
`_tmp_pp_one.bat <name.cpp>` then grep `DefProc pFunc` in `tmp/_tmp_pp_out.txt` (the resolved binding names),
`_tmp_name_resolve_probe.cpp` + `_tmp_run_probe.bat` (live `GetModuleHandleW`/`GetProcAddress` probe against
the real `user32.dll`), `_tmp_cmake_file_diff.ps1` (disk vs `add_library` file list),
`_tmp_user32_exports.txt` (the export table).

Full-library compile after the session-1 edit: 38/38 TUs, `/W4`, exit 0, no diagnostics.

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

- **U1 - no call-site error handler anywhere in this library.** 0 of 38 TUs include
  `../kernel/errorhandler.h` or construct `Win32ErrorHandler _;`, which every `kernel`/`shell` wrapper does.
  `IssueHandler<T>`'s ctor `Push()`es the handler onto the flyer chain for its dynamic extent
  (`handler.h:37-45`), so an issue raised inside `Library::Call` (`Library::ExceptionFilterFunction` calls
  `serious(...)` at `library.h:41`) or by a `CheckReturn` has no handler in this library and lands on
  whatever the host process has installed.
- **U2 - return-check policy is absent.** Only 4 `CheckReturn` uses exist in the whole library
  (`errorhandling.cpp`, `processthread.cpp`); `shell`/`kernel` apply `CheckReturn<BOOL, BoolCheck>` to every
  `BOOL` and `CheckReturn<HRESULT, LongSuccessCheck>` to every `HRESULT`. With G6 in the way, this is a
  behaviour decision, not a typo.
- **U3 - `hook.cpp` build-list status: RESOLVED (user commit `05d3ca2f`, 29 Oct 2026), review still open.**
  `add_library` now lists all 38 sources - the earlier "37 of 38" finding was against the pre-commit state.
  `hook.cpp` compiles standalone at `/W4` exit 0 (measured 29 Oct 2026), so its four wrappers (`CallMsgFilterT`,
  `CallNextHookEx`, `SetWindowsHookExT`, `UnhookWindowsHookEx` - declared at `user32.h:316-319`) are now linked
  into the DLL. What remains: the TU is unreviewed (it is back in the queue) and still has no call site in
  `src`/`examples`/`test`, so it is a live-but-unused surface rather than a declaration-without-definition.
- **U4 - `user32.h` declares about 101 non-static members that no `.cpp` defines.** The class is documented
  as a stateless all-static API surface (`docs/agent/windows_code_conventions.md`), yet whole sections are
  plain instance members: String functions (`:336-369`), Button control (`:372-374`), DDE (`:505-530`, some
  carrying a `const` qualifier on a stateless class), Shell/System information (`:642-652`), Authorization
  (`:658-659`), WinEvent hooks (`:662-664`), `LoadBitmap` (`:672`), synchronization (`:675-676`),
  `PrintWindow` (`:679`). Against `:424` static declarations, these have no definition anywhere and no call
  site anywhere in `src`/`examples`/`test` - so calling one is an `LNK2019` at the caller, not a compile
  error. `mxDdeCreateStringHandle` (`:510`) is additionally a mistyped name.
- **U5 - the binding is cached for the process lifetime including failure.** `qor_pp_useswinapi` expands to
  a function-local `static const Library::DefProc pFunc = ...`; the first call wins forever. A process that
  has not mapped `user32.dll` yet (G5), or a transiently missing name, is poisoned permanently.
- **U6 - calling-convention mismatch in the call path (half fixed in the working tree, uncommitted).** Measured
  29 Oct 2026: `library.h:16` now reads `typedef int(__stdcall *DefProc)(void);` - an uncommitted working-tree
  edit (`git status`: `M src/platform/os/windows/api_layer/library.h`, SPDX header swap plus this typedef;
  `returncheck.h` is modified the same way). The committed text was `int(*)(void)` (`__cdecl`). The other half
  of the defect is untouched: `Call` builds `typedef ret(*fPtr)(MethodArgs...)` (`library.h:33`) and `voidCall`
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
- **U11 - `resource.cpp:16` defines `CopyImage` without the `User32::` qualifier.** It is a free function in
  `qor::win::api`, while `user32.h:443` declares `User32::CopyImage` as a static member - so the declared
  member has no definition (any caller would hit `LNK2019`) and the free function is dead (no call site
  anywhere in `src`). `resource.cpp:23` `User32::LoadImageT` is qualified correctly. One-token fix
  (`User32::CopyImage`), to be applied in that TU's session. Bindings there are otherwise sound:
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


