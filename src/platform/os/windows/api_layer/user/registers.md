# winqor_api_user - review registers (P / G / U / T)

Verbatim sections moved out of `status.md` on 1 Nov 2026 so a session can boot on a short brief. This file holds the
authoritative text of every register item; `status.md` carries only a locator index (id + line here + first line of the item).
Nothing here was edited, reworded or renumbered. When you open or extend an item, edit it HERE, then refresh the index in
`status.md` and keep that file under 150 lines.

## Pre-scan of the not-yet-reviewed TUs (folder-wide, ahead of the per-TU sessions)

`tmp/_tmp_aw_checklist.ps1` cross-checks every `qor_pp_useswinapi` / `qor_pp_useswinapiAW` binding in the
folder against `tmp/_tmp_user32_exports.txt`. It is a *hint list* - the per-TU `/EP` check (method step 3)
stays the authority, since the script cannot see which SDK macros are in scope. Results that matter, all in
TUs still to be reviewed:

- **P1 - bindings that cannot resolve at run time (export table + live probe + source line).** Six at the first scan;
  the two in `combobox.cpp` are now fixed and two more have since been found (`window.cpp:124` in session 6,
  `mouse.cpp:82` in session 8), so **five remain in three TUs** (`keyboard.cpp` x3, `window.cpp` x1, `mouse.cpp` x1)
  after session 13 fixed `dialog.cpp:89`. Fix each in the owning TU's session:
  - `keyboard.cpp:82` `qor_pp_useswinapi(user32, GetKeyboardLayoutNameT)`, `:243` `VkKeyScanT`,
    `:250` `VkKeyScanExT` - the wrapper's `T` suffix stringizes into the name, and `*T` is not an export.
  - ~~`dialog.cpp:89` `qor_pp_useswinapi(user32, GetDlgItemTextT)`~~ - **FIXED 30 Oct 2026 (session 13)**: same class.
    `/EP` pre-fix resolves the literal `"GetDlgItemTextT"` (live probe: MISSING), post-fix `"GetDlgItemTextW"` (RESOLVED).
    Latent rather than live - that wrapper has no call site in built code (see the U15 `dialog.cpp` entry).
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
- **P2 - bindings that work today only because the SDK macro rewrites them; convert to explicit AW
  under D13.** **Six remain:** `menu.cpp` `GetMenuString`, `InsertMenuItem`; `message.cpp` `RegisterWindowMessage`,
  `SendMessage`, `SendMessageTimeout`, `SendNotifyMessage`. ~~`dialog.cpp` `MessageBox`~~ - **converted 30 Oct 2026
  (session 13)** at `:124`; **no behaviour change** - the pre-fix `/EP` literal was already `"MessageBoxW"` because the
  SDK's `UNICODE` mapping rewrites the stem before stringizing (G1); the AW form reaches the same literal by `##W`
  pasting (`kernel32.h:52`), which is why the conversion is behaviour-neutral.
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


## P6 and U27 - register entries opened inside session-log row 17 (no standalone entry as at 1 Nov 2026)

Extracted verbatim from the row-17 cell (`status.md` line 118 before the split; the same words remain in the row in
`session-log-archive.md`). Until they are promoted into the sections above, this is their only register text - treat it as
authoritative. **P6** is a live binding trap for hook-family / `*MsgFilter*` work; **U27** is a published-signature item.

- New pre-scan item **P6** (bare `CallMsgFilter` is exported at 1C at RVA `00088CF0` - **the same RVA as `CallMsgFilterA`** - so a wrong binding there resolves and runs the ANSI entry point on wide arguments instead of throwing) and new open item **U27** (`STRICT` changes the identity of `HOOKPROC` and of every `DECLARE_HANDLE` type, hence the published mangled names: 3 of these 4 wrappers change name between the `STRICT` and `NO_STRICT` arms, measured with `tmp/_tmp_hookproc_probe.cpp`). **U15 4/4 dead**, **U19: 4 stems with 0 sites of any class** - the first section of the review with no caller *and* no SDK-import call anywhere, `HHOOK`/`HOOKPROC` appearing in no file of `src`/`examples`/`test` at all |


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
  **2 defined and already safe** (`RegisterDeviceNotification` `devicemanagement.cpp:76`, `WinHelp` `help.cpp:35`, both
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
- **G6 - return checking is silently always on.** `returncheck.h:19` tests
  `qor_pp_parameter_checking == qor_pp_yes` (the checked typedef `:20`, `#else` `:21`, bare typedef `:22`, `#endif` `:23`;
  the citation read `:40` until session 14 re-measured it); neither macro is defined anywhere in the repository, so the
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
  plain instance members: String functions (`:336-369`), Button control (`:372-374`), Shell/System information (`:642-652`), Authorization
  (`:658-659`), WinEvent hooks (`:662-664`), `LoadBitmap` (`:672`), synchronization (`:675-676`),
  `PrintWindow` (`:679`). Against `:424` static declarations, these have no definition anywhere and no call
  site anywhere in `src`/`examples`/`test` - so calling one is an `LNK2019` at the caller, not a compile
  error. `mxDdeCreateStringHandle` (`:510`) is additionally a mistyped name.
  - **DDE re-measured 30 Oct 2026 (session 13) - the largest single U4 block, and the reason the queue has no
    `dde.cpp`.** The section is `user32.h:501-534`: **34 declarations, all non-static, all 34 `const`-qualified**
    (`(Select-String -Pattern '\)\s*const\s*;').Count` = 34, and every one of them is in this section), and the folder's
    38 sources contain **no file for any of them** - no `dde.cpp` exists, and a repo-wide grep for
    `DdeAbandonTransaction`/`DdeInitialize`/`mxDdeCreateStringHandle` returns the three header lines and nothing else. So
    this is not a TU with defects to review: it is 34 declarations of a class that cannot be called (`const` non-static
    members of a documented "all-static" surface) with no definition and no caller. Their entry points are real - 29 of
    the exports sit in the ordinal range 7E-9E of `tmp/_tmp_user32_exports.txt`, including
    `DdeCreateStringHandleA/W` (86/87) for the mistyped `mxDdeCreateStringHandle`, and `DdeInitializeA/W` (91/92) and
    `DdeQueryStringA/W` (98/99) - and one exported member of the family, `DdeGetQualityOfService` (8F), has **no
    declaration at all**. Fixing this is a surface decision (make the section `static` + write the TU, or delete it), not
    a per-TU review, which is why the review passes over it and why it must not be counted as a reviewed TU.
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
  **Name-level cross-check 30 Oct 2026 (session 13)** with the new `tmp/_tmp_decl_no_def_repo_scan.ps1` (searches
  `src`/`examples`/`test`, not just the folder): of the **490** member-declaration lines its regex parses in `user32.h`,
  **426** names appear somewhere as `User32::` and **64** appear nowhere - the full list in `tmp/_tmp_decl_no_def_s13.txt`.
  That 64 is a **different measure from the 97 above and lower, not a correction of it**: this scanner counts *names* and
  its regex stops at declarations whose parameter list wraps or that end `) const;` (34 such lines in the header, e.g.
  `:519 DdeInitialize`, which is why no `Dde*` name appears in the list), so it under-counts by design. Use the
  `tmp/_tmp_static_census.ps1` figures for the published count and this list only to see *which* names they are.
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
  `keyboard.cpp:69,76`, `mouse.cpp:83`, `window.cpp:118,254,317`. No site over-specifies its types. **Re-measured
  30 Oct 2026 (session 10): `desktop.cpp` is now 18/18 explicit, so the folder stands at 423 explicit against 6 deducing** -
  `keyboard.cpp:69,76`, `mouse.cpp:83`, `window.cpp:118,254,317` - with spacing 250 tight / 179 spaced. The old figure
  counted `Call< UINT >(pFunc, nBuff, lpList)`-style sites correctly but also swept in every zero-argument call and
  every `voidCall`, which is why whole clean files (caret 2, clipboard 7, menu 4, message 7, systemshutdown 1,
  processthread 1, dialog 1, cursor 1) appeared in the list: those files have **0** deducing sites. `caret.cpp`
  itself is 8/8 explicit (verified by hand at `caret.cpp:26,33,40,47,54,61,68,75`).
  Deduction always yields the *wrapper's* declared parameter types, so it is type-correct whenever the wrapper
  matches the SDK - but it stops pinning the intended types and stops self-documenting the signature, which is what
  the explicit form is for (`memory-bank/systemPatterns.md` rule 2: "passes SDK argument types verbatim"). The
  reference libraries carry **no deducing site at all** (same script: `shell` 60/60 explicit, `kernel` 327/327 for
  `Call`/`voidCall`; their `inlineCall` sites are not counted by this script). Fix the 6 remaining sites (`keyboard.cpp` 2,
  `mouse.cpp` 1, `window.cpp` 3) in each owning TU's
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
  - `cursor.cpp` - **15 of 16** (every wrapper except `LoadCursorT`). Re-measured 30 Oct 2026 (session 9) with
    `tmp/_tmp_bypass_scan.ps1`: all sixteen have exactly one **LAYER** call site each - 17 sites, `LoadCursorT` twice -
    in `ui/gui/gdiobjects/cursor.cpp` (built at `gui/CMakeLists.txt:43`), so at wrapper level this is the first fully
    called TU of the review. One level up it collapses: none of the twelve `Cursor` methods that make those calls
    (`Show`, `Clip`, `GetClip`, `GetCurrent`, `GetInfo`, `GetPosition`, `SetPosition`, `SetPhysicalPosition`,
    `GetPhysicalPosition`, `SetAsCurrent`, `SetAsSystemCursor`, `Clone`) has a call site anywhere in
    `src`/`examples`/`test` - `search_codebase` finds only their definitions in that one file. So fifteen wrappers are
    linked, exported through `QOR_WINAPIUSER` and never executed. `LoadCursorT` alone is live end to end:
    `examples/experimental/osspecific/windows/mainwindow/mainwindow.cpp:339` (`factory.AddWindowClass(L"CustomWindow")`,
    a target that is in `cmakebuild/QOR.sln`) -> `ui/gui/factories/windowfactory.cpp:90`
    (`windowClass->SetCursor(Cursor(IDC_ARROW))`) -> `Cursor::Cursor(const wchar_t* id)`
    (`ui/gui/gdiobjects/cursor.cpp:49-53`, `m_owning = false` - correct for a shared system cursor) -> `User32::LoadCursorT`
    -> `LoadCursorW`. That single live path is also what makes the **U20** caller-side findings observable rather than
    theoretical.
  - `devicemanagement.cpp` - **all 2** (`RegisterDeviceNotification`, `UnregisterDeviceNotification`; `user32.h:682-683`).
    The first section where the wrappers are 100% dead **and** the underlying family is nevertheless used by the tree, but
    only through the SDK thunk: the single device-notification caller is `ui/gl/helperwindow.cpp:85`
    `::RegisterDeviceNotificationW` (see U19), which is in no build target. So no call site anywhere can observe these two
    wrappers, and the one place that wants device notifications does not link this layer.
  - `dialog.cpp` - **16 of 22** (measured 30 Oct 2026, session 13, `tmp/_tmp_bypass_scan.ps1` with 39 stems - every
    `*A`/`*W`/`*T` variant of the 22 members; saved run `tmp/_tmp_dialog_bypass2.txt`). **14 LAYER** sites cover only
    **6** of the 22 wrappers: `GetDlgCtrlID` x8 (`ui/gui/controllers/{a_h/animation,a_h/button,a_h/combobox,a_h/edit,`
    `i_m/listbox,i_m/mdiclient,s_s/scrollbar,s_s/static}controller.cpp`, one `GetId()` each), `EndDialog` x2 and
    `DialogBoxIndirectParamT` x1 (`ui/gui/windows/a_h/dialog.cpp:58,65,75`), `MessageBoxT`/`MessageBoxTimeout`/
    `MessageBoxIndirectT` (`ui/gui/windows/i_m/messagebox.cpp:41,46,52`). The other 16 wrappers have **no reference of any
    kind** in `src`/`examples`/`test`. This is the first TU where LAYER sites exist and **none of them can run in a built
    target** - each chain ends one hop up at a commented-out example: the eight controller `GetId()` sites are reached only
    through `TopLevelWindowHandler::SetCommandCallback` (`ui/gui/view/handlers/toplevel.cpp:48`, built at
    `gui/CMakeLists.txt:61`), whose only callers are `examples/.../button/button.cpp:49` and
    `examples/.../editbox/editbox.cpp:61,88` - `#add_subdirectory(button)`/`#add_subdirectory(editbox)` at
    `osspecific/windows/CMakeLists.txt:1,3`; and the gui `Dialog`/`MessageBox` classes are constructed only at
    `examples/.../dialogs/dialogs.cpp:44,57,255,256` - `#add_subdirectory(dialogs)` at the same file `:2`.
    `controllers/i_m/mdiclientcontroller.cpp`, one of the eight, is not even compiled: it is absent from
    `gui/CMakeLists.txt` (which lists `windows/i_m/mdiclient.cpp` at `:21` and the other seven controllers at `:22,28-33`).
    Consumer casts recorded while the trail is open (the U20 cross-reference shape): `ui/gui/windows/a_h/dialog.cpp:75-79`
    passes `reinterpret_cast<LPCDLGTEMPLATE>(t)` from a `DlgTemplateEx*` whose declaration
    (`gui/windows/a_h/dialog.h:39-61`) keeps only `DLGTEMPLATE`'s fixed header and comments out the trailing variable
    fields - sound only because the example hands over a real resource template image (`dialogs.cpp:255`
    `&g_DebugDlgTemplate`); and `messagebox.cpp:52` passes `reinterpret_cast<const LPMSGBOXPARAMS>(&params)` where
    `gui/windows/i_m/messagebox.h:73-85` is field-for-field `MSGBOXPARAMSW` (`unsigned int`/`void*`x2/`const TCHAR*`x3/
    `unsigned long`/`unsigned long long`/fn-ptr/`unsigned long` against `UINT`/`HWND`/`HINSTANCE`/`LPCWSTR`x3/`DWORD`/
    `DWORD_PTR`/`MSGBOXCALLBACK`/`DWORD`) with `cbSize` default-initialised at `:75`, which is the entry condition the SDK
    requires. `ui/gui/windows/a_h/dialog.cpp:33-34` and `messagebox.cpp:33` also `#undef MessageBox` /
    `#undef DialogBoxIndirectParam` before calling the layer - the only in-tree consumers that need the SDK macros out of
    the way.

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
  - `errorhandling.cpp` - **3 of 4** (measured 31 Oct 2026, session 14, `tmp/_tmp_bypass_scan.ps1` invoked as
    `pwsh -NoProfile -Command "& _tmp_bypass_scan.ps1 -Stems @('FlashWindow','FlashWindowEx','MessageBeep','SetLastErrorEx')"`
    over the script's default roots `src`/`examples`/`test`, saved run `tmp/_tmp_errh_bypass.txt`; the `pwsh -File` form of
    that invocation degrades the four stems to a single string and can report an empty scan - see the session-14
    instrument note). `FlashWindow` has exactly **1** reference in the whole tree - `User32::FlashWindow` at
    `ui/gui/controllers/windowcontroller.cpp:717` (`WindowController::Flash`, declared `windowcontroller.h:271`, built at
    `gui/CMakeLists.txt:27`) - and that method's only in-tree caller is `WindowImpl::RequestAttention()`
    (`ui/gl/windowimpl.cpp:1833-1836`), which belongs to `winqor_gl`, commented out of the build at `ui/CMakeLists.txt:4`
    and absent from `cmakebuild/QOR.sln` (session 9's measurement, re-confirmed from the same `CMakeLists.txt` line).
    `FlashWindowEx`, `MessageBeep` and `SetLastErrorEx` have **no reference of any kind** outside `user32.h:615-617` and
    their own definitions. Scan totals: 1 LAYER, 0 INDIRECT, 0 BYPASS, 0 MEMBER - so unlike the dialog and DC families,
    nothing in the tree reaches these four entry points through the SDK import either.
- `filledshapes.cpp` - **all 3** (`FillRect`, `FrameRect`, `InvertRect`, `user32.h:609-611`). Each has exactly one call
  site, all three inside `DeviceContext` (`ui/gui/gdiobjects/devicecontext.cpp:75`, `:80`, `:85`), and those three
  `DeviceContext` methods (`devicecontext.h:232-234`, non-virtual) have **no call site anywhere** in
  `src`/`examples`/`test` - unlike the neighbouring `DeviceContext` methods, which the built `examples/mainwindow` does
  reach (`GetTextExtentPoint32T`/`TextOutT` at `mainwindow.cpp:179`, `:180`, and `BitBlt` at `:247` through the `Blit`
  helper the `OnPaint` handler calls at `:176`). This is
  therefore the rare shape where the wrapper's caller exists, the DC it would run on is live, and still nothing invokes
  it. Scan totals 3 LAYER / 3 INDIRECT / 0 BYPASS / 0 MEMBER (`tmp/_tmp_fs_bypass.txt`), so the family has no SDK-import
  path either.
  - `icon.cpp` - **12 of 14** (measured 1 Nov 2026, session 18; `tmp/_tmp_icon_liveness_scan.ps1` -> `tmp/_tmp_icon_liveness.txt`
    plus a `findstr` sweep of every `Icon` construction site in `src`/`examples`/`test`). Unusual shape: all 14 wrappers
    have exactly one caller, the `qor::platform::win::Icon` class (`ui/gui/gdiobjects/icon.cpp:18`-`:109`, built at
    `gui/CMakeLists.txt:49`) - the first family of the review whose whole liveness hangs on one caller-side class. Two
    wrappers are reached in built code: `LoadIconT` twice (`windowfactory.cpp:89` `Icon(IDI_APPLICATION)` and `:92`
    `Icon(nullptr)`, both through `Icon(const wchar_t*)` at `gdiobjects/icon.cpp:52-56`) and `DestroyIcon` (from `~Icon`
    at `:62` when those two temporaries die). `WindowFactory`'s own ctor calls `AddWindowClass` (`windowfactory.cpp:45`),
    and `AddWindowClass` is called from built example code (`examples/experimental/osspecific/windows/mainwindow/mainwindow.cpp:339`),
    so that pair runs in any built app that creates a window. The other 12 are linked, exported and unreachable:
    `CreateIcon`, `CreateIconFromResource`, `CreateIconFromResourceEx`, `CreateIconIndirect` (their `Icon` ctors have no
    call site), `CopyIcon` (`Icon::Clone()` has no call site, and `SetIcon(Icon(...))` never invokes the copy ctor -
    C++17 guaranteed elision, confirmed in `tmp/obj/wf.obj`: `tmp/_tmp_wf_disasm.txt` references no
    `??0Icon@...AEBVIcon@...` at all, only the `PEB_W` ctor), `DrawIcon`, `DrawIconEx`, `GetIconInfo`, `GetIconInfoExT`,
    `LookupIconIdFromDirectory`,
    `LookupIconIdFromDirectoryEx`, `PrivateExtractIconsT`. The one consumer that would use the draw/query half is
    `RenderingHandler` (`ui/gui/view/handlers/rendered/rendering.cpp:122-137`, the `wmGetIcon`/`wmSetIcon` slots, built at
    `gui/CMakeLists.txt:88`), but its installation slot `m_rendering` (`view/handlers/rendered.h:51`) has **no writer**
    anywhere in the tree - only the read at `rendered.cpp:38` - so the handler is never installed (the same shape as
    clipboard's `m_clipboardOwner` above; `m_nonClient` `:49` and `m_ncRendering` `:50` are writer-less too). Note the
    `Icon` objects the four controller sites build (`buttoncontroller.cpp:87`, `:92`, `staticcontroller.cpp:70`, `:94`)
    use the `(PrimitiveHandle, own)` ctor and so call no wrapper at all - and those four controller methods have no call
    site (U20).


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
  - `cursor.cpp` - **3 guarded** of 16: `GetPhysicalCursorPos` (`winuser.h:9439`) and `SetPhysicalCursorPos` (`:9406`)
    sit inside `#if(WINVER >= 0x0600)` (`:9435`, `:9402`); `GetCursorInfo` (`:14661`) and the `CURSORINFO`/`PCURSORINFO`
    type in its signature (`:14644-14650`) sit inside `#if(WINVER >= 0x0500)` (`:13594`) with `#ifndef NOWINABLE`
    (`:13716`) between. The other thirteen pair with declarations under nothing but `#ifndef NOUSER` plus a
    `WINAPI_FAMILY_PARTITION` (Desktop, or Desktop|Games, or Desktop|PC-App for `ClipCursor`). Measured with
    `tmp/_tmp_sdk_guard_scan.ps1`; the printed guard stacks are kept in `tmp/_tmp_cursor_sdk.txt`. U16's reverse half:
    nothing here - the seven cursor-family exports this folder does not wrap (`GetCursorFrameInfo`, `SetCursorContents`,
    `ShowSystemCursor`, `SetThreadCursorCreationScaling`, `GetPointerCursorId`, `GetPointerDeviceCursors`,
    `MITGetCursorUpdateHandle`) are declared nowhere in `user32.h` at any guard level, so no consumer-visible
    declaration outranks the SDK here.

  - `devicecontext.cpp` - **4 guarded** of the 13 SDK declarations the section pairs with: `EnumDisplaySettingsExA/W`
    (`winuser.h:13148`, `:13156`) and `EnumDisplayDevicesA/W` (`:13174`, `:13182`) sit inside `:13143 #if(WINVER >= 0x0500)`;
    `ChangeDisplaySettingsA/W` (`:13080`, `:13086`), `ChangeDisplaySettingsExA/W` (`:13098`, `:13107`) and
    `EnumDisplaySettingsA/W` (`:13126`, `:13133`) carry no version guard at all, and `GetDC` (`:8569`), `GetDCEx` (`:8575`)
    and `ReleaseDC` (`:8612`) sit under nothing but `#ifndef NOUSER` plus
    `#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)` (`:8443`, `:8601`). Measured with
    `tmp/_tmp_sdk_guard_scan.ps1`. This TU adds a **new U16 sub-case, stronger than a version guard**: the whole display
    block is inside `#ifdef _WINGDI_` / `#ifndef NOGDI` (`:13072-13075`) and `#ifndef NOSYSPARAMSINFO` (`:12433`), and five
    wrapper signatures name `DEVMODE`/`DISPLAY_DEVICE` types from `wingdi.h` - a consumer that defines `NOGDI` (a common
    `Windows.h` shrink knob) does not merely lose an SDK prototype, it makes `user32.h:326-330` ill-formed. None of
    `NOGDI`/`NOSYSPARAMSINFO`/`NOUSER`/`WINAPI_FAMILY` is defined anywhere in `cmake/` or in any `CMakeLists.txt`
    (measured 30 Oct 2026), and no `WINVER` is pinned, so the `sdkddkver.h` 0x0A00 default keeps all 13 visible today.
    U16's reverse half: `GetWindowDC` (`:8606`) and `WindowFromDC` (`:8563`) are declared here at `user32.h:274`, `:287` at
    the same guard level as the SDK, so nothing outranks the SDK; the `WINVER >= 0x0601` display-config pair
    (`:13198`) is not declared by this library at all.
  - `devicemanagement.cpp` - **2 guarded of 2** - the whole section, and the hardest dependency of the review so far after
    `NOGDI`: the SDK's device-notification block sits inside `#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)`
    (`winuser.h:3940`) and `#if(WINVER >= 0x0500)` (`:3942`), and the wrapper signatures name `HDEVNOTIFY`, which is
    typedef'd at `:3943-3944` **inside the same guards**. A consumer pinning `WINVER` below `0x0500` (or a non-Desktop
    `WINAPI_FAMILY`) therefore does not just lose the two SDK prototypes - `user32.h:682-683` becomes ill-formed on an
    unknown return type, so this library's own header stops compiling. Measured with `tmp/_tmp_sdk_guard_scan.ps1`
    (SDK-side scan) plus a repo-wide grep confirming still no `WINVER`/`_WIN32_WINNT`/`NTDDI_VERSION` in `cmake/` or any
    `CMakeLists.txt`. U16's reverse half here: the Power Management pair this header declares at `:686-687` pairs with
    `winuser.h:3993`/`:4002` under `#if (_WIN32_WINNT >= 0x0502)` (`:3979`, closed `:4022`), and the `DEVICE_NOTIFY_ALL_INTERFACE_CLASSES` flag
    (`:3948-3950`, `_WIN32_WINNT >= 0x0501`) is not declared by this library at all - see T3.
  - `dialog.cpp` - **1 of 22 pairs with a version-guarded SDK declaration, and in that one case the guard swallows the
    *type* as well as the prototype.** Measured 30 Oct 2026 with `tmp/_tmp_guard_stack_scan.ps1` (SDK-side guard stack):
    the 12 dialog prototypes sit at `winuser.h:5008-5321` under `#ifndef NOUSER` (`:64`) + `#ifndef NOCTLMGR` (`:4912`) +
    `#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)` (`:5006`) with **no `WINVER` test at all**; `MessageBoxA/W`
    (`:9230`, `:9238`) and `MessageBoxExA/W` (`:9276`, `:9285`) under `#ifndef NOMB` (`:9154`) + Desktop (`:9225`);
    `MessageBoxIndirectA/W` (`:9340`, `:9345`) additionally under `#if(WINVER >= 0x0400)` (`:9297`), and
    `IsDialogMessageA/W` (`:11553`, `:11559`) under `#ifndef NOMSG` (`:11548`) + `NOCTLMGR` (`:11233`) + Desktop (`:11542`).
    `MSGBOXPARAMSW` and `LPMSGBOXPARAMS` are typedef'd at `:9314`/`:9330` **inside** that 0x0400 guard, so a consumer that
    pins `WINVER` below 0x0400 does not merely lose the prototype - `user32.h:487` becomes ill-formed on an unknown type,
    the same sub-case `devicemanagement.cpp` (`HDEVNOTIFY`) and `devicecontext.cpp` (`DEVMODE`) opened. Two more of the 22
    signatures depend on SDK **shrink** macros rather than version ones: `NOCTLMGR` removes `LPCDLGTEMPLATEW` (`:4958`,
    inside `:4912`) which `user32.h:470`/`:473` name, and `NOMSG` removes the `MSG` typedef itself (`:1877-1887`, inside
    `:1869`) which `user32.h:483` names - so `NOCTLMGR`/`NOMB`/`NOMSG` in a consumer build would break this section of
    `user32.h` outright. Still latent: none of the three appears in any `CMakeLists.txt` or in `cmake/` (re-confirmed this
    session, third time for the same class). **U16's reverse half, and the hardest instance of the review:**
    **`MessageBoxTimeout` has no declaration in any SDK header on this machine** - zero hits for the name across every
    header of SDK 10.0.26100 (`um`, `shared`, `winrt`, ...), and the 10.0.10240 install under
    `Program Files (x86)\Windows Kits\10\Include` contains only `ucrt`, no `um`, so 26100 is the only Windows-header set
    available. `user32.h:489`'s signature therefore has **no header authority at all**: it is validated only against the
    export table (`MessageBoxTimeoutA`/`W` present, bare `MessageBoxTimeout` absent) and the live probe (both `A`/`W`
    RESOLVED, bare MISSING). The AW paste (`kernel32.h:52`) is what makes the binding work without any SDK macro - no SDK
    `MessageBoxTimeout -> MessageBoxTimeoutW` mapping exists to rescue a bare binding here.
  - `errorhandling.cpp` - **1 version-guarded of 4, and it is the type-level sub-case again.** Measured 31 Oct 2026 with
    the `-Names` form of the guard-stack scan (`tmp/_tmp_sdk_guard_scan.ps1 FlashWindow,FlashWindowEx,MessageBeep,SetLastErrorEx`):
    `FlashWindow` (`winuser.h:4683`) sits under `#ifndef NOUSER` (`:64`) +
    `#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)` (`:4668`) with **no version test**; `FlashWindowEx` (`:4699`)
    is additionally inside `#if(WINVER >= 0x0500)` (`:4687`, closed `:4709`), and `PFLASHWINFO` - the type
    `user32.h:615`'s parameter has - is typedef'd at `:4694` **inside that same guard**, so a consumer pinning `WINVER`
    below `0x0500` does not merely lose the prototype, `user32.h:615` becomes ill-formed on an unknown type (the
    `HDEVNOTIFY`/`DEVMODE`/`MSGBOXPARAMSW` sub-case again, and the mildest of them since `0x0500` is far below the
    `sdkddkver.h` default). `MessageBeep` (`:9365`) is under `#ifndef NOMB` (`:9154`) + Desktop (`:9360`) and
    `SetLastErrorEx` (`:13561`) under Desktop only (`:13556`) - note that this is **outside** `#ifndef NOUSER`, unlike the
    other three, so `NOUSER` does not shield it and `NOMB` in a consumer build would break `user32.h:616` outright. Still
    latent: no `WINVER`/`_WIN32_WINNT` is pinned in `cmake/` or any `CMakeLists.txt` (4th confirmation of the same class),
    and `NOUSER`/`NOMB` appear nowhere either. No `_WINGDI_`/shrink dependency in this section, and no reverse-half
    instance - all four SDK prototypes exist; the two exported error-adjacent entry points `user32.h` omits
    (`RegisterErrorReportingDialog` 2E6, `SetDebugErrorLevel` 32F) are a section-coverage gap, not a guard one.
  - `icon.cpp` - **4 of the 14** wrapped prototypes sit behind a version guard, and the section reproduces the
    ill-formed-*type* sub-case at the highest threshold measured so far. Measured 1 Nov 2026 with
    `tmp/_tmp_icon_guards.ps1` -> `tmp/_tmp_icon_guards.txt` (17 hits over 16,475 lines; it scans the `*A`/`*W` spellings
    too, which is why 17 rows cover 14 members): `LookupIconIdFromDirectoryEx` (`winuser.h:10819`) under
    `#if(WINVER >= 0x0400)` (`:10815`), `CreateIconFromResourceEx` (`:10840`) under `:10836` and `DrawIconEx` (`:10948`)
    under `:10883` - all `0x0400` - and `GetIconInfoExW` (`:11026`, `GetIconInfoExA` `:11020`) under
    `#if(_WIN32_WINNT >= 0x0600)` (`:10986`, closed `:11034`). The other nine members carry no version test at all, only
    `#ifndef NOUSER` (`:64`) plus a `WINAPI_PARTITION_DESKTOP` block (`:10723`, `:10901`, `:10965`); `DrawIcon` is the
    odd one out, declared at `:8170` inside the messaging block under `:8165`. Type guard, measured with
    `tmp/_tmp_icon_type_guards.ps1` -> `tmp/_tmp_icon_type_guards.txt` (5 hits): `ICONINFOEXA`/`ICONINFOEXW`,
    `PICONINFOEXA`/`PICONINFOEXW` and the `ICONINFOEX`/`PICONINFOEX` typedefs all live inside that same `0x0600` block
    (`:10987`-`:11014`), so below `_WIN32_WINNT 0x0600` `user32.h:436`'s `GetIconInfoExT(HICON, ::PICONINFOEX)` does not
    merely lose its prototype, it names an unknown type (the `HDEVNOTIFY`/`DEVMODE`/`MSGBOXPARAMSW`/`PFLASHWINFO` class,
    and the highest threshold of them). `ICONINFO`/`PICONINFO` (`:10739`) sit ungated apart from Desktop + `NOUSER`, so
    the other 13 signatures survive every version pin. The `#ifdef UNICODE` arm of that typedef pair is a sharper
    hazard than the version guard and is recorded under U26. Still latent: no `WINVER`/`_WIN32_WINNT`/`NTDDI_VERSION` in
    `cmake/` or any `CMakeLists.txt` (5th confirmation of this class), and `NOUSER` appears nowhere either.

- **U17 - A/W wrappers declared without the folder's `T` suffix.** Opened 29 Oct 2026 (session 6) when the two combo
  members were renamed (user decision: rename inside the TU being reviewed, keep the rest together for one
  folder-wide call). Measured with `tmp/_tmp_tsuffix_census.ps1`: of the 525 `user32.h` member declarations, **108**
  face an A/W export pair and carry the suffix (the norm), **404** are single exports with no suffix (correct bare per
  D13), and **11** have an A/W family with no suffix: `MessageBoxTimeout` (`:489`, defined `dialog.cpp:143`),
  `DdeInitialize` (`:519`), `DdeQueryString` (`:525`), `GetRawInputDeviceInfo` (`:540`), `DlgDirList` (`:546`),
  `DlgDirSelectEx` (`:547`), `SendIMEMessageEx` (`:636`, defined `ime.cpp:16`), `WinHelp` (`:646`, defined
  `help.cpp:32`), `SystemParametersInfo` (`:652`), `LoadBitmap` (`:672`), `RegisterDeviceNotification` (`:682`, defined
  `devicemanagement.cpp:73`). **Only 4 of the 11 are implemented** (the ones marked `defined` above); the other 7 are
  U4 declared-and-undefined members, so their spelling is currently academic. The census' mirror case - `T` suffix with
  no A/W family - is 2 members, `CreateWindowT` (`:144`) and `CopyCursorT` (`:383`), see pre-scan P3. Deliberately not
  touched per-TU: renaming is a published-surface change (all four implemented ones are exported through
  `QOR_WINAPIUSER`), and 3 of the 4 already bind correctly through `qor_pp_useswinapiAW`, so today the only cost is that
  the name hides the A/W nature from the reader. Note also that where a member keeps the bare stem **and** the stem is
  `#undef`'d at `user32.h:9-17`, the bare-stem binding is a live failure (G2) - that is exactly the P1 set, which is why
  the U17 decision and the D13 binding fix have to be settled in the same pass per TU.
  **Policy settled again 30 Oct 2026 (session 11, asked ahead of `devicemanagement.cpp`): the remaining 11 stay as they
  are until one folder-wide pass - session 9's defer policy governs, and session 6's two combo renames stay the only
  exception.** The case that prompted the question was the sharpest of the 11: `RegisterDeviceNotification` (`:682`) is the
  only U17 member whose stem is also `#undef`'d (`user32.h:9`), so it looked like the situation the paragraph above warns
  about. It is not one: `devicemanagement.cpp:76` already binds it `qor_pp_useswinapiAW(user32, RegisterDeviceNotification)`,
  so the `#undef` never reaches the binding - the cost of deferring is purely that the name hides the A/W nature.
  Session 12 therefore takes no name action; it records the instance count only.
  **Mechanism corrected 30 Oct 2026 (session 13):** the sentence this bullet had ("the SDK's `UNICODE` mapping rewrites the
  stem before stringizing") explains the AW path wrongly. `kernel32.h:50-55` shows `qor_pp_useswinapiAW( m, n )` expands to
  `qor_pp_useswinapi( m, n##W )` (or `n##A`) - **token pasting**, which suppresses macro expansion of the pasted argument.
  The SDK-macro-rewrites-the-name effect (G1) therefore applies **only to the bare `qor_pp_useswinapi` path**, and the paste
  is also why the AW form works for stems that have no SDK macro at all - proved this session by `dialog.cpp:146`
  `MessageBoxTimeout`, a name declared nowhere in any SDK header yet bound correctly. G2's own wording in the Ground-rules
  section already had the paste right, so no session verdict changes: every AW verdict in this file rests on the `/EP`
  literal, not on the mechanism guess.
  **Census re-measured 30 Oct 2026 (session 13)** with `tmp/_tmp_tsuffix_census.ps1`: **unchanged** - 525 member
  declarations, 108 A/W + `T` (the norm), 404 single-export bare (correct per D13), **11** A/W without the suffix, mirror
  case **2**. `dialog.cpp` contributes 1 of the 11 (`MessageBoxTimeout` `:489`) and adds the strongest argument for
  eventually renaming it: the bare member name hides an A/W family whose **bare stem is not even exported** (live probe:
  `MessageBoxTimeout` MISSING, `MessageBoxTimeoutA`/`W` RESOLVED), so nothing in the signature tells a caller that the
  bare spelling would fail at run time.
  **Session 18 (`icon.cpp`, 1 Nov 2026): 0 instances, census unchanged.** The icon section's three A/W wrappers already
  carry the suffix (`GetIconInfoExT` `:436`, `LoadIconT` `:437`, `PrivateExtractIconsT` `:440`), and
  `tmp/_tmp_tsuffix_census.ps1` re-run 1 Nov (`tmp/_tmp_tsuffix_s18.txt`) gives the same five buckets as sessions
  11-13/17: 525 member declarations, 108 A/W + `T` (the norm), 404 bare single exports, **11** A/W without the suffix,
  2 mirror cases. No name action taken, per the session-11 defer policy.
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
  - **Cursor family (session 9, 30 Oct 2026) - the cleanest picture of the review: 0 bypass sites in built code.** 17
    **LAYER** sites (one per wrapper, `LoadCursorT` twice - all in `ui/gui/gdiobjects/cursor.cpp`, built at
    `gui/CMakeLists.txt:43`) and 13 raw **BYPASS** hits, but 9 of the 13 are not SDK calls at all once the enclosing
    declaration is read: `blobbuffer.h:117,185,187` call `BlobBuffer::SetCursor(size_t)`, declared in the same class at
    `:194`; `input.cpp:380,414` call `Input::DestroyCursor` (`input.h:109`); `input.cpp:440` is the two-argument
    `Platform::SetCursor(Window*, Cursor*)`; `input.h:107` and `gl/window/window.h:136` are member *declarations*. The
    remaining 4 are genuine SDK calls in live code, all in one file: `ui/gl/windowimpl.cpp:247` (`SetCursor` +
    `LoadCursorW`), `:254` (`SetCursor`), `:272` (`::ClipCursor(nullptr)`), `:2219` (`::SetCursorPos`) - and `winqor_gl`
    is **not in the build** (`ui/CMakeLists.txt:4` `#add_subdirectory(gl)`, no `winqor_gl` project anywhere in
    `cmakebuild/QOR.sln`), exactly like everything else on that list (`framework/ui/CMakeLists.txt` has no `gl` entry,
    `ui/CMakeLists.txt:2` comments out `egl`, `osspecific/windows/CMakeLists.txt:9` comments out `retropad`). Those four
    windowimpl lines are CODE, not comment, checked with `tmp/_tmp_comment_state.ps1`. Instrument change this session:
    `_tmp_bypass_scan.ps1` now emits a fourth class, **MEMBER** (`x.stem(...)` / `Class::stem`), which keeps the 26
    same-named member calls out of the bypass count (13 raw - 9 members = 4).
    *Re-measured 30 Oct 2026 with the corrected instrument and the full suffix-variant list (17 stems incl. `*T`/`*A`/`*W`):
    15 LAYER / 15 BYPASS / 28 INDIRECT / 26 MEMBER.* The two BYPASS hits beyond session 9's 13 are
    `examples/.../retropad/controller.cpp:44` and `retropad.cpp:941` (`LoadCursor`), and `retropad` is commented out of
    the build (`osspecific/windows/CMakeLists.txt:9`); the rest are the same member-call sites the note above explains.
    **Session 9's conclusion - 0 SDK bypass in built code - stands.** The coordinate-family entry re-runs clean too
    (3 LAYER / 0 BYPASS with `*T` variants included).
  - **DC / display family (session 11, 30 Oct 2026) - the largest split of the review: 16 LAYER / 12 BYPASS.** Measured
    with the suffix-variant list from step 5b (the bare-stem run gave a wrong 8 / 4, and the pre-fix `::` handling a wrong
    10). All 16 **LAYER** sites are compiled: 4 in `ui/gdi/displaydevices.cpp:42,47,61,70` (built -
    `ui/CMakeLists.txt:3 add_subdirectory(gdi)`), 6 in
    `ui/gui/controllers/i_m/monitor.cpp:94,146,149,177,195,213` and 3 in
    `ui/gui/controllers/windowcontroller.cpp:615,627,638` (both built at `gui/CMakeLists.txt:25`, `:27`), 3 in
    `ui/gui/gdiobjects/devicecontext.cpp:44,52,65` (built at `:44`). Only **4 are reachable at run time** - the
    `WindowController::GetDC()`/`GetADC(rgn,flags)`/`ReleaseDC(dc)` trio, which the built
    `examples/.../mainwindow/mainwindow.cpp:80,84` calls, plus `DeviceContext::Release()` (`gdiobjects/…:65`) through the
    destructor path those sites arm; the other 12 are compiled-but-uncalled, and `DisplayDevices` has no user at all
    outside its own header (so `ChangeDisplaySettingsExT`'s only wrapper-level caller is dead code, while
    `ChangeDisplaySettingsT` has no site anywhere). The 12 **BYPASS** sites: 9 in
    `ui/gl/monitorimpl.cpp:63,107,138,172,231,263,266,284,313` and 1 in `ui/gl/wglcontextimpl.cpp:36` - **neither file is
    in any source list** (grep of every `CMakeLists.txt` for `monitorimpl|wglcontextimpl` returns 0), and the other 2 are
    `ui/gl/windowimpl.cpp:151` (`GetDC`) / `:158` (`ReleaseDC`), which are in `winqor_gl`'s list but
    `ui/CMakeLists.txt:4` comments `gl` out of the build entirely. So no bypass of this family is run-time behaviour
    today - all 12 are source debt. `windowimpl.cpp:151`/`:158` are CODE not comment (`tmp/_tmp_comment_state.ps1`).
    Excluded from the count: 3 **INDIRECT** `WindowController::`/`PaintSession::` hop sites and 2 **MEMBER** `GetDC()`
    calls in the example.
  - **Device-notification family (session 12, 30 Oct 2026) - 0 LAYER / 1 BYPASS.** The only site in `src`/`examples`/`test`
    that touches the family at all is `ui/gl/helperwindow.cpp:85` `::RegisterDeviceNotificationW(...)`, classified BYPASS
    (leading `::`), and it is live code - `tmp/_tmp_comment_state.ps1` reports the whole file 0% commented, unlike most of
    `ui/gl`. The call is written correctly for the SDK (it zeroes the struct and fills `dbcc_size`, `dbcc_devicetype` and
    `dbcc_classguid` at `:79-83` before passing it), but `helperwindow.cpp` is in no source list (`winqor_gl`'s list is
    `winqor_gl.cpp cursorimpl.cpp windowimpl.cpp wgl.cpp`) and `ui/CMakeLists.txt:4` comments `add_subdirectory(gl)` out, so
    this is again source debt rather than run-time behaviour. Note the pairing gap it leaves: the same file never calls
    `UnregisterDeviceNotification`, so if `gl` were enabled the helper-window registration would leak - the one
    behavioural signal this family emits in the tree is a leak on the bypass path, not a layer call.
  - **Dialog family (session 13, 30 Oct 2026) - 14 LAYER / 24 BYPASS, the largest LAYER count of the review, and 13 of the
    14 LAYER sites are in compiled code.** Measured with 39 stems (bare + `*T`/`*A`/`*W`) into `tmp/_tmp_dialog_bypass2.txt`.
    The 14 LAYER sites: `GetDlgCtrlID` x8, one per controller (`ui/gui/controllers/{a_h/animationcontroller.cpp:46,
    a_h/buttoncontroller.cpp:107, a_h/comboboxcontroller.cpp:47, a_h/editcontroller.cpp:47, i_m/listboxcontroller.cpp:47,
    i_m/mdiclientcontroller.cpp:47, s_s/scrollbarcontroller.cpp:47, s_s/staticcontroller.cpp:48}`), each returning it as
    `unsigned short` from `GetId()` - a **caller-side** narrowing of this wrapper's `int` return (`user32.h:477` matches the
    SDK's `int GetDlgCtrlID(HWND)` exactly, `_In_ HWND`, no guard, so the layer is faithful and the truncation lives in
    `winqor_gui`, where a control id above `0xFFFF` would be cut);
    `DialogBoxIndirectParamT` (`ui/gui/windows/a_h/dialog.cpp:75`) and `EndDialog` x2 (`:58`, `:65`); and
    `MessageBoxT`/`MessageBoxTimeout`/`MessageBoxIndirectT` (`ui/gui/windows/i_m/messagebox.cpp:41`, `:46`, `:52`). **Ten of
    the eleven** files are compiled (`gui/CMakeLists.txt`: `:14`, `:18` and the controllers at `:22`, `:28`-`:33`);
    `controllers/i_m/mdiclientcontroller.cpp` is absent from that list, so **7 of the 8** `GetDlgCtrlID` sites are compiled
    (the eighth, `mdiclientcontroller.cpp:47`, is not - the list has `windows/i_m/mdiclient.cpp` at `:21` but never the
    controller). So unlike every family before it, the LAYER sites are in **compiled** code: `ui/gui/view/handlers/
    toplevel.cpp:48` calls `ctrl->GetId()` from `TopLevelWindowHandler::SetCommandCallback`, built at
    `gui/CMakeLists.txt:61`. That setter's only call sites in the tree are however `examples/.../button/button.cpp:49` and
    `.../editbox/editbox.cpp:61,88`, both commented out of the build, so these sites are **compiled but never invoked** -
    compiled-and-reachable, not live; the U15 `dialog.cpp` entry is the authority on that question. The 24
    BYPASS sites are all four `retropad` files (`controller.cpp`, `gotodlg.h`, `mainwindowinstancecontroller.cpp`,
    `retropad.cpp`) calling `EndDialog`/`GetDlgItem`/`GetDlgItemInt`/`SetDlgItemInt` bare, and `retropad` is commented out
    of the build (`osspecific/windows/CMakeLists.txt:9`), so **0 bypass in built code** - the family's only debt is the
    24 example sites. The compensating detail: the three `MessageBox*` wrappers' only callers construct
    `qor::platform::win::MessageBox`, and the only such call sites are `examples/.../button/button.cpp:51` and
    `.../dialogs/dialogs.cpp:44,57`, both commented out of the build (`CMakeLists.txt:1`, `:2`) - so the wrappers are
    compiled and reachable through a class that nothing built instantiates (see the U15 `dialog.cpp` entry).

- **Method hazard found with U19 (applies to every liveness claim in this review):** a grep hit is not evidence, because
  large parts of the Windows `ui` tree are commented out. `ui/gl/windowimpl.cpp` is **1503 comment-only lines of 2637
  (57%)**, and the block comments do not nest - a block opened at `:2221` runs to `:2498`, which is why eight
  `ClientToScreen`/`ScreenToClient`/clipboard hits in that file are all dead. `tmp/_tmp_comment_state.ps1 -Path <file>
  -Lines <n...>` settles it with a character-level state machine (block comments, line comments, string and char
  literals) and prints the commented fraction. Two earlier records in this file relied on unfiltered greps and had to be
  corrected (the clipboard U15 entry and its session-5 note).
  - **Icon family (session 18, 1 Nov 2026) - 16 LAYER / 0 INDIRECT / 7 BYPASS / 0 MEMBER, i.e. 0 bypass in built code.**
    17 stems (bare + `*T`/`*A`/`*W`) through `tmp/_tmp_bypass_scan.ps1`, driven from `tmp/_tmp_icon_pre.bat` into
    `tmp/_tmp_icon_pre_s18.txt`, with the coordinate baseline re-run in the same pass and unchanged (3 LAYER / 0 BYPASS).
    All 16 LAYER sites are the `User32::` calls inside one built file, `ui/gui/gdiobjects/icon.cpp:18`, `:24`, `:30`,
    `:36`, `:42`, `:48`, `:54`, `:62`, `:73`, `:79`, `:84`, `:89`, `:94`, `:99`, `:104`, `:109` (`gui/CMakeLists.txt:49`)
    - 14 wrappers plus the two repeats `LoadIconT` (`:48`, `:54`) and `CopyIcon` (`:18`, `:73`). The 7 BYPASS are all in
    targets that are not built: 5 in `ui/gl/windowimpl.cpp:191`, `:1603`, `:1608`, `:1642`, `:1645`
    (`ui/CMakeLists.txt:4` `#add_subdirectory(gl)`) and 2 in
    `examples/experimental/osspecific/windows/retropad/controller.cpp:43`, `retropad.cpp:940`
    (`osspecific/windows/CMakeLists.txt:9`). This is the first family where every wrapper has a *compiled* call site, so
    the liveness question moves one level up (see the icon U15 entry) - a compiled site is not a reached site.
    Adjacent-name note: `OpenIcon` (`user32.h:190`, Window section) does have a live LAYER caller,
    `WindowController::OpenIcon` at `ui/gui/controllers/windowcontroller.cpp:325`, even though the SDK groups it with the
    icon family - a U21-shaped placement note, not an icon.cpp instance.

- **U20 - the caller-side `Cursor`/`Icon` ownership model is wrong at the sites this layer feeds.** Opened 30 Oct 2026
  (session 9, `cursor.cpp`) after tracing that TU's call sites; **recorded, not fixed** - it is `winqor_gui` /
  `winqor_wincommon` surface, and changing who destroys a handle is a behaviour change in another library (the same
  standing rule as U18/U19). Four measured findings:
  - **Ownership defaults to true.** `Cursor(const PrimitiveHandle& h, bool own = true)` (`ui/gui/gdiobjects/cursor.h:30`)
    and the identical `Icon` ctor (`icon.h:44`) take ownership unless the caller says otherwise, and `~Cursor()`
    (`gdiobjects/cursor.cpp:55-62`) calls `User32::DestroyCursor` on `m_handle.Use()` whenever `m_owning` - with no
    null/invalid guard. `Cursor::Clone()` (`:64-69`) feeds the result of `CopyCursorT` straight into
    `Cursor(handle, true)`, so a failed copy (`Library::Call` returns `nullptr` per U7) still calls
    `DestroyCursor(nullptr)`.
  - **The call sites disagree with each other.** In one file, `ButtonController` passes the ownership flag for bitmaps
    (`buttoncontroller.cpp:46`, `:52` - `false`) and omits it for cursors and icons (`:77` `GetCursor`, `:82` `SetCursor`,
    `:87` `GetIcon`, `:92` `SetIcon`), so those four `new_ref<Cursor>`/`new_ref<Icon>` objects own handles they were not
    given: `BM_GETIMAGE` returns the control's *current* image and `BM_SETIMAGE` the *previous* one, and neither transfers
    ownership to the sender, so the returned ref will `DestroyCursor`/`DestroyIcon` a handle the window (or the caller that
    set it) still uses. The sibling `StaticController` pair gets it right in the same situation
    (`staticcontroller.cpp:58`, `:82` - both `false`).
  - **Every destruction raises a spurious leak warning.** `~Cursor` destroys the handle but never clears `m_handle`, and
    `PrimitiveHandle` has no release/reset API (`common/handles/handle.h:9-47`), so the member's own destructor
    (`handle.cpp:27-33`) sees a non-null `m_h` and raises
    `warning("PrimitiveHandle destructor called with non-null handle. This may indicate a resource leak.")` - for owning
    objects (already destroyed) and non-owning ones (never theirs to destroy) alike. This is the one finding that runs
    today: the review's only live cursor path, `windowfactory.cpp:90` (`windowClass->SetCursor(Cursor(IDC_ARROW))`),
    destroys a non-owning temporary per `AddWindowClass` call.
  - **`Show()` erases the API's contract.** `Cursor::Show(bool)` (`gdiobjects/cursor.cpp:139-142`) returns
    `User32::ShowCursor(show ? 1 : 0) ? true : false`. `ShowCursor` returns the signed display *count*, so the first
    `Show(false)` (count becomes -1) reports failure for a successful hide and a second `Show(true)` reports success while
    changing nothing (Microsoft's documented contract, not re-measured here; the wrapper's `int` return is already
    correct - the loss is in the caller).
  Liveness of the first three: the `ButtonController` quartet has **no call site** anywhere in `src`/`examples`/`test`
  (`search_codebase` finds only the definitions), so the ownership bug is dormant and joins U15; only the warning is on a
  live path. For the audit: the folder's own wrappers make the distinction the callers miss - `LoadCursorT`/`GetCursor`
  return shared system handles and `CreateCursor`/`LoadCursorFromFileT`/`CopyCursorT` return private ones (now stated in
  the `cursor.cpp` TU comment), and `Cursor::Cursor(const wchar_t* id)` correctly sets `m_owning = false`.
  **The `Icon` half was measured rather than inferred on 1 Nov 2026 (session 18)**, because `icon.cpp` is the TU that
  feeds it. Drivers `tmp/_tmp_run_icon_own_probe.bat` and `tmp/_tmp_run_icon_null_probe.bat`, outputs
  `tmp/_tmp_icon_own_probe.txt` and `tmp/_tmp_icon_null_probe.txt` (pure SDK calls, so they measure the contract the
  wrappers rely on, not the QOR layer):
  - **`LoadIcon` hands out a shared handle, and `DestroyIcon` on it is silently tolerated.** `LoadIconW(NULL,
    IDI_APPLICATION)` -> `0x1002B`, and loading it twice returns **the same value**; `DestroyIcon` on it returns **1**
    (success) leaving `GetLastError` 1402 (`ERROR_INVALID_HANDLE`), a second `DestroyIcon` on the same value also returns
    1, and `GetIconInfo` on it still succeeds afterwards. The predefined ids are therefore not handle-table entries at
    all (the value sits in the low word), which is exactly why `windowfactory.cpp:89`'s `~Icon` destroying a shared
    handle has never shown up as a failure: the contract is violated, and the OS absorbs it.
  - **`Icon(nullptr)` at `windowfactory.cpp:92` selects `Icon(const wchar_t* id)`, not the handle ctor** - the
    `PrimitiveHandle(void*)` converting ctor (`common/handles/handle.h:14`) is not `explicit`, but the standard
    conversion `nullptr` -> `const wchar_t*` outranks the user-defined one. Proved with `tmp/_tmp_icon_overload_probe.cpp`
    (declared-only overloads returning distinct tag types + `decltype`; compile-only, both `narrow_compile_exit=0` and
    `wide_compile_exit=0` from `tmp/_tmp_run_icon_overload_probe.bat` into `tmp/_tmp_icon_overload_probe.txt`; the
    constexpr-object form of the same test fails as C2131 "unevaluable pointer value", so do not retry it). Confirmed
    **independently at the binary level**: compiling the consumer (`tmp/_tmp_compile_wf.bat` -> `tmp/obj/wf.obj`, then
    `dumpbin /nologo /disasm` -> `tmp/_tmp_wf_disasm.txt`) shows the two temporaries at `windowfactory.cpp:89` and `:92`
    both calling `??0Icon@win@platform@qor@@QEAA@PEB_W@Z` - `Icon(const wchar_t*)` - at `tmp/_tmp_wf_disasm.txt:614` and
    `:701`, each followed by one `??1Icon@...UEAA@XZ` and a `SetIcon`/`SetSmallIcon` taking `AEBVIcon@...`. The same
    probe run in both worlds shows `IDI_APPLICATION` is a macro whose type follows `UNICODE` at the use site, and
    `tmp/_tmp_icon_include_probe.cpp` + `tmp/_tmp_run_icon_include_probe.bat` (`include_probe_compile_exit=0`) settle
    the real TU: after `windowfactory.h` then `api_layer/user/user32.h`, `UNICODE` **is** defined and
    `decltype(IDI_APPLICATION)` is `LPWSTR`, so line `:89` really does reach `Icon(const wchar_t*)` -> `LoadIconT` (in a
    narrow TU it would silently take the handle ctor and call **no** wrapper at all - another U26-shaped fork).
    So line `:92` asks the OS for an icon with a **null name**: measured `LoadIconW(NULL, nullptr)` -> **NULL** with
    `GetLastError` 1813 (`ERROR_RESOURCE_NAME_NOT_FOUND`), and the owning `~Icon` then calls `DestroyIcon(NULL)` ->
    **0**, `GetLastError` 1402. The "no small icon" intent is served only by a failure path, and the layer deliberately
    reports no error on `HICON` returns (U2), so a caller cannot tell "no icon wanted" from "icon load failed" -
    `m_handle` is null either way.
  - **The stored handle outlives the owner that destroys it.** `WindowClass::SetIcon(const Icon&)`
    (`ui/gui/factories/windowclass.cpp:123-126`) keeps `m_hIcon = icon.GetHandle()` - a raw handle copy, with no
    refcount and no reset (`PrimitiveHandle` has no release API, `handle.h:9-47`) - while the `Icon` argument at
    `windowfactory.cpp:89`/`:92` is a temporary that runs `DestroyIcon` at the end of the full-expression; `WindowClass`
    feeds the stored value to `RegisterClassExW` at `windowclassregistration.cpp:44`/`:49`. The OS tolerance measured
    above is the only reason this is not visible today. The spurious `PrimitiveHandle` leak warning described in the
    fourth bullet above applies identically to both `Icon` temporaries.
  - **Checked and *not* a defect:** the six `Icon` ctors that assign `m_owning` inside the body
    (`ui/gui/gdiobjects/icon.cpp:22-56`) cannot leave a half-built object with an indeterminate `m_owning`, because if
    the wrapper call throws (`Library::Call` throws when the module or name is missing, U7) the ctor never completes, so
    `~Icon` is not run. Recorded so the next session does not re-open it.


- **U21 - the "DC functions" section is not the SDK's DC family, and the SDK's DC family is split across two TUs.**
  Opened 30 Oct 2026 (session 11, `devicecontext.cpp`); **recorded, not fixed** - moving declarations between sections is
  a header-surface reorganisation and the same standing rule as U17/U18 applies. Measured facts:
  - `user32.h:325 //DC functions` declares 8 members, but five of them (`ChangeDisplaySettingsT`,
    `ChangeDisplaySettingsExT`, `EnumDisplayDevicesT`, `EnumDisplaySettingsT`, `EnumDisplaySettingsExT`) are
    display-configuration entry points with no `HDC` in any signature, and only three (`GetDC`, `GetDCEx`, `ReleaseDC`)
    are DC calls.
  - Conversely the SDK declares one DC neighbourhood at `winuser.h:8560-8614` - `WindowFromDC` (`:8563`), `GetDC`
    (`:8569`), `GetDCEx` (`:8575`) in the Desktop-family sub-block `:8443-8580`, then `GetWindowDC` (`:8606`) and
    `ReleaseDC` (`:8612`) in the next (`:8601-8654`) - and this library splits it across two sections and two TUs: three members in `devicecontext.cpp`
    (`:331-333`) and `GetWindowDC`/`WindowFromDC` in the "Painting functions" section (`:274`, `:287`), defined
    `painting.cpp:93`, `:184`. A reader auditing DC-ownership from either TU sees an incomplete family.
  - The folder wraps **no** `CreateDC`/`DeleteDC`, `SaveDC`/`RestoreDC`, `SetDCPenColor`-class or `GetDeviceCaps` member
    in any section (`GetDeviceCaps` is in the `gdi32` layer, not here), so the section name over-promises relative to
    both the SDK block and its own contents.
  Nothing here is a correctness defect - all 8 wrappers bind correctly wherever they are declared.
- **U22 - the `DeviceContext` release path this layer feeds calls `ReleaseDC` with a NULL `HWND`.** Opened 30 Oct 2026
  (session 11) while tracing `devicecontext.cpp`'s callers; **recorded, not fixed** - `winqor_gui` surface (the U20 rule).
  `ui/gui/gdiobjects/devicecontext.cpp:65` is `User32::ReleaseDC((HWND)(m_hWnd.Use()), (HDC)(m_handle.Use()))`, and
  `m_hWnd` is only ever set by the two `FromWindow` factories (`:45`, `:53`) - the default and handle constructors set it
  to `0` (`:16`, `:20`) and `PerThread::DeviceContextFromHandle` (`perthread.cpp:163-175`) uses the handle constructor,
  so a `DeviceContext` obtained that way has `m_hWnd == 0`. `WindowController::GetADC()`
  (`windowcontroller.cpp:612-622`) and `GetADC(rgn, flags)` (`:624-633`) do exactly that - `User32::GetDC` (`:615`)/
  `GetDCEx` (`:627`) then
  `DeviceContextFromHandle` (`:618`) then `dc->MustRelease()` (`:619`) - so `~DeviceContext` (`:24-30`) fires `Release()` with
  `ReleaseDC(NULL, hdc)`. Microsoft's contract requires the `hWnd`/`hDC` pair to match (returns 0 otherwise), so the DC
  is not released; the wrapper is correct, the caller's pairing is not. Same shape in `WindowController::GetDC()`
  (`:550-560` - `User32::GetWindowDC` at `:553`, `DeviceContextFromHandle` at `:556`, `MustRelease()` at `:557`).
  Liveness: all four of those sites are
  compiled, and the only in-tree callers are `examples/experimental/osspecific/windows/mainwindow/mainwindow.cpp:80`,
  `:84` - which **is** built (`CMakeLists.txt:61` -> `examples/CMakeLists.txt:4` -> `.../windows/CMakeLists.txt:4`, and
  `mainwindow` is a project in `cmakebuild/QOR.sln`). `PaintSession` is the one correct
  consumer: its ctor (`paintsession.cpp:38-43`) pairs `BeginPaint` (`:41`) with the `~PaintSession` `EndPaint` (`:47`) on
  the same `PAINTSTRUCT` and never sets `MustRelease`, so the handle map entry is left alone and no `ReleaseDC` runs.
- **U23 - `MessageBoxIndirectT`'s parameter puts `const` on the pointer instead of the pointee.** Opened 30 Oct 2026
  (session 13, `dialog.cpp`); **recorded, not fixed** - it is a published-signature change (`user32.h:487` + the matching
  definition `dialog.cpp:135` + the `Library::Call<>` template argument), i.e. header surface, so it goes in the
  folder-wide pass with U17/U13 rather than being changed under a single TU. `LPMSGBOXPARAMS` is itself a pointer typedef
  (`/EP` under `UNICODE` gives `typedef LPMSGBOXPARAMSW LPMSGBOXPARAMS;` -> `MSGBOXPARAMSW*`), so `const ::LPMSGBOXPARAMS`
  is `MSGBOXPARAMSW* const` - a **const pointer to a mutable struct**, while the SDK is
  `_In_ CONST MSGBOXPARAMSW * lpmbp` (`winuser.h:9343-9346`) - a **pointer to a const struct**. Two consequences: the
  wrapper's contract promises nothing about not writing through the caller's `MSGBOXPARAMS`, and the top-level `const` is
  discarded by the function type anyway, so the declaration states something the compiler ignores. Nothing in the tree is
  harmed today - the only call site, `ui/gui/windows/i_m/messagebox.cpp:52`, passes
  `reinterpret_cast<const LPMSGBOXPARAMS>(&params)` from the mirror struct `qor::platform::win::MsgBoxParams`
  (`messagebox.h:73-85`, field-for-field identical to `MSGBOXPARAMSW`, `cbSize` defaulted at `:75`), and a `const`-qualified
  cast binds to a non-const pointee parameter. One-line fix when the pass runs: `const ::MSGBOXPARAMS*` in the declaration
  and definition, with `Library::Call<int, const ::MSGBOXPARAMS*>`; the SDK defines no `LPCMSGBOXPARAMS` spelling, so the
  folder's LP-typedef convention cannot be kept here.
- **U24 - the one in-tree `FlashWindow` caller reads its `BOOL` as a success flag.** Opened 31 Oct 2026 (session 14,
  `errorhandling.cpp`); **recorded, not fixed** - the defect is in `winqor_gui`/`winqor_gl` (the U20 rule), and the wrapper
  is a faithful pass-through. `user32.h:614` publishes `static BOOL FlashWindow(HWND hWnd, BOOL bInvert)` with no comment,
  and `ui/gui/controllers/windowcontroller.cpp:715-718` is
  `bool WindowController::Flash(bool invert) { return User32::FlashWindow(WINDOW_HANDLE, invert ? 1 : 0) ? true : false; }`
  (declared `windowcontroller.h:271`), i.e. the method's `bool` is presented as "did it work". `FlashWindow`'s documented
  return is not a success flag: when the window **is** the foreground window the value is its **prior active state**, and
  when it **is not** the foreground window the window is not flashed at all and the value reports visibility (Microsoft's
  `FlashWindow` documentation; the SDK header carries nothing to contradict that - `winuser.h:4680-4685` has bare `_In_`
  SAL on both parameters and no return annotation, and no `__declspec(deprecated)` even though the docs deprecate the
  function in favour of `FlashWindowEx`). So for the actual use case - "request attention", where by definition the window
  is not active - the value the caller receives says nothing about whether anything happened. Liveness keeps this off the
  live-defect list: `Flash`'s only in-tree caller is `WindowImpl::RequestAttention()` (`ui/gl/windowimpl.cpp:1833-1836`),
  which **discards** the value, and `winqor_gl` is not in the build (`ui/CMakeLists.txt:4` `#add_subdirectory(gl)`), so
  nothing built today observes the wrong meaning - the exposure is the published `WindowController` surface. Three ways
  out when the gui pass runs, all of them gui-side: make `Flash` `void`, or document the previous-state meaning, or switch
  the controller to `User32::FlashWindowEx` (`FLASHW_ALL | FLASHW_TIMERNOFG`), which does return a success flag and is
  already wrapped at `errorhandling.cpp:70`. **Consequence for this layer:** the absence of a `CheckReturn<BOOL, BoolCheck>`
  on the `FlashWindow` wrapper (`errorhandling.cpp:63-68`) is correct and must not be "tidied up" to match
  `MessageBeep` - under G6 a `BoolCheck` is always live, so it would raise a continuable issue on every successful flash of
  an inactive window.
- **U25 - `const` is spelled three ways in `user32.h`, and the `CONST::` spelling is an SDK macro glued to a scope
  qualifier.** Opened 31 Oct 2026 (session 15, `filledshapes.cpp`); **recorded, not fixed** - it reaches 24 published
  declarations across five sections, so it belongs with the folder-wide U14(a) style pass rather than inside one TU.
  **It is not a defect today:** `CONST` is an SDK object-like macro defined as `const` (`shared/minwindef.h:153`, also
  `ntdef.h:227`, `wincrypt.h:101`, `bcrypt.h:48`, `fontsub.h:22`, `tbs.h:35`), so `CONST::RECT*` at `user32.h:609`
  preprocesses to `const ::RECT*` - proved on this TU, where `/EP` writes the definition as
  `int User32::FillRect(HDC hDC, const::RECT* lprc, HBRUSH hbr)` (`tmp/_tmp_fs_pp.txt:453301`). Same type, same generated
  symbols, no UB and no ODR exposure: the two spellings denote one type, so a caller's mangled reference is unaffected by
  which one the header uses. Measured in `user32.h`: **17** lines `CONST::X*` (`:264`, `:267`, `:270`, `:278`, `:282`,
  `:285`, `:447`, `:448`, `:450`, `:451`, `:453`, `:456`, `:457`, `:571`, `:609`, `:610`, `:611`), **7** lines lowercase
  `const ::X*` (`:139`, `:210`, `:431`, `:487`, `:579`, `:580`, `:581`), **0** lines `CONST ::X*` - so the split follows
  the section, not chance, and the `.cpp` side adds a third variant by spelling the template arguments `CONST ::RECT*`
  (`filledshapes.cpp:83`, `:90`, `:97`). Why it is worth changing anyway: it reads as a namespace-qualified name (`CONST`
  looks like a namespace), it is the only place in the library where an SDK macro leaks into a published signature, and it
  is spelling-sensitive for the review's own tooling - `tmp/_tmp_sigcmp.ps1` compares the declaration's type string with
  the `Library::Call<>` argument string, so it prints a **false** `CALL TYPE MISMATCH` on 3/3 sites of this TU
  (`CONST::RECT*` vs `CONST ::RECT*`). The same false positives will appear in `rectangle.cpp` (11 members),
  `painting.cpp` (`DrawFocusRect`, `EndPaint`, `InvalidateRect`, `RedrawWindow`, `ValidateRect`, `DrawAnimatedRects`) and
  `text.cpp` (`TabbedTextOutT`); the instrument should normalize whitespace instead of the source being bent to satisfy it.
  **One real smell sits inside the family:** `user32.h:571` `CONST::LPINT lpnTabStopPositions` is `const (int*)` - i.e.
  `int *const`, a const pointer to **mutable** ints, since `LPINT` is `typedef int far *LPINT` (`minwindef.h:167`) -
  while the SDK has `CONST INT *lpnTabStopPositions` (`winuser.h:8467`, in the `TabbedTextOutW` block at `:8460`), a
  pointer to **const** ints. That is U23's exact shape (top-level `const` on a pointer typedef), so `text.cpp`'s session
  should pair it with U23 rather than opening a third item; the top-level `const` is discarded by the function type, so
  nothing is broken today. Fix when the folder pass runs: normalize the 17 `CONST::` lines to the lowercase
  `const ::X*` the SDK itself uses in its own modern declarations, and the matching template arguments in the four
  affected `.cpp` files - an ABI-neutral, caller-neutral spelling change, because the denoted type is unchanged.

- **U26 - `TCHAR` width is an include-order property, not a build setting.** Opened 31 Oct 2026 (session 16,
  `help.cpp`); recorded, not fixed. Measured: nothing in `cmake/`, the root `CMakeLists.txt`, `CMakeSettings.json` or
  `cmakebuild/CMakeCache.txt` defines `UNICODE`/`_UNICODE` (0 occurrences), so the only source of the define is
  `kernel32.h:7-10` under `qor_pp_unicode` (`src/configuration/configuration.h:13` = 1). `tmp/_tmp_tchar_probe.cpp` +
  `tmp/_tmp_run_tchar_probe.bat` measure both worlds in one run: with the library's include chain `_UNICODE=1`,
  `sizeof(TCHAR)=2`, `LPCTSTR==LPCWSTR`; without it `_UNICODE=0`, `sizeof(TCHAR)=1`, `LPCTSTR==LPCSTR`. Consequence: a
  consumer that sees `Windows.h` before any QOR header gets the **narrow** spelling of every published `*T` signature,
  while the built import library carries the wide one - `dumpbin /linkermember:1` shows `WinHelp` published with a
  `PEB_W` (`const wchar_t*`) - so the failure mode is a **link error** on a name that does not exist, not silent
  narrow-passing. Checked on the only consumer of the section this was found in: `ui/gui/controllers/windowcontroller.h:28-39`
  pulls in no `Windows.h`, so `winqor_gui` sees the wide spelling and `windowcontroller.cpp:795` compiles. This is the
  same one-switch property that makes the `*T` wrappers safe in `clipboard.cpp` (session 5), now stated as a library-wide
  hazard: it guards the whole `*T` surface (U17's list), and closing it means either a build-wide define (a behaviour
  decision for every consumer) or self-sufficient published headers. Related measurement folded into **U2**:
  `../returncheck.h` is included by 37 of the 38 TUs and actually used by 2 (`devicecontext.cpp` mentions it in a comment
  only; the 2 real users are `errorhandling.cpp:77-83` and `processthread.cpp:20,:29,:45`) - instrument
  `tmp/_tmp_returncheck_usage.ps1`, whose raw count over-reads comment mentions.
  **Escalated by `icon.cpp` (session 18, 1 Nov 2026): in the `GetIconInfoExT` path the include-order width is a silent
  buffer overrun, not a link error.** `winuser.h:11009`-`:11014` defines `ICONINFOEX`/`PICONINFOEX` **twice**, once under
  `#ifdef UNICODE` (`:11009`) and once in its `#else` arm (`:11012`) - measured with `tmp/_tmp_icon_type_guards.ps1`.
  `user32.h:436` takes `::PICONINFOEX`, but `qor_pp_useswinapiAW` token-pastes `GetIconInfoExW` unconditionally, so a TU
  that sees `<windows.h>` before `kernel32.h` (the only place `UNICODE`/`_UNICODE` are defined, `kernel32.h:7-10`)
  compiles the **560-byte A shape** of the parameter type against the **W** entry point, which writes **1,080** bytes: no
  compile error, no link error, just 520 bytes written past the end of the caller's struct. Measured with
  `tmp/_tmp_iconinfoex_mirror.cpp` (SDK types plus a hand copy of the caller-side mirrors) and confirmed against the real
  headers compile-only with `tmp/_tmp_iconinfoex_assert.cpp` + `tmp/_tmp_run_iconinfoex_assert.bat`
  (`assert_compile_exit=0`): in the library's own include order (`user32.h:7` pulls in `kernel32.h` first)
  `sizeof(ICONINFOEX)` = 1080 = `sizeof(ICONINFOEXW)` while `sizeof(ICONINFOEXA)` = 560, and the caller-side mirror
  `ui/gui/gdiobjects/icon.h:27-38` is field-for-field `ICONINFOEXW` (`cbSize` 0, `fIcon` 4, `xHotspot` 8, `yHotspot` 12,
  `hbmMask` 16, `hbmColor` 24, `wResID` 32, `szModName` 34, `szResName` 554) because QOR spells its own `TCHAR` from
  `qor_pp_unicode` (`common/structures.h:57-65`, `src/configuration/configuration.h:13` = 1) rather than from `_UNICODE`.
  `sizeof(ICONINFO)` = 32 = the `IconInfo` mirror at `icon.h:18-25` with the same offsets, so the two `reinterpret_cast`s
  at `ui/gui/gdiobjects/icon.cpp:89`/`:94` are layout-safe **as the tree is configured today**, and the path is
  U15-unreachable as well. This is the first U26 instance whose failure mode is corruption rather than a missing-symbol
  link error, and it is a *type* dependency, not just a name dependency: closing U26 for the `*T` surface does not close
  it unless the published headers stay self-sufficient about which `ICONINFOEX` they mean.


> NOTE: the "Remaining queue" paragraph at the end of the section below is the pre-session-17 snapshot (21 TUs,
> `hook.cpp` next). Kept as recorded; the live queue is the one in `status.md`.

### Deferred tasks (opened by session 10; T3 added by session 12) - each needs a decision, so none was touched

- **T1 - two declared-but-undefined members in the "Authorization functions" section.** `user32.h:658-659` declare
  `GetUserObjectSecurity` and `SetUserObjectSecurity` **without `static`** and **no `.cpp` in the folder defines either**
  (measured: the only hits outside the header are the `desktop.cpp` TU comment), so any caller of either fails at link
  time with `LNK2019` rather than at compile time - the same U4 shape as the other 95 non-static declarations without a
  definition. What sets this pair apart is that their entry points are real, exported and single-stem: `GetUserObjectSecurity`
  is export ordinal 1DB (hint 1979, RVA 00002460) and `SetUserObjectSecurity` ordinal 375 (hint 2384, RVA 0002BBE0) in
  `tmp/_tmp_user32_exports.txt`, and the SDK
  declares both at `winuser.h:1671-1684`, directly after `SetUserObjectInformation`/`GetUserObjectInformation`, which
  `desktop.cpp:153` and `:118` wrap. Two callers-away neighbours: `UserHandleGrantAccess` is the third user-object call
  and *is* defined - but in another TU's section (`user32.h:259` "Process and Thread functions", defined
  `processthread.cpp:41`), a U12-style placement fact for that session. Deciding T1 means either adding the two
  definitions (natural home: `desktop.cpp`, which owns the rest of the user-object family) or deleting the declarations -
  both move the published surface, so it is a user decision and belongs with the `U4`/`U13` sweep, not with a TU review.
- **T2 - the whole Window Station / Desktop surface is dead exported API, and its conventions are therefore untestable
  against a caller.** All 18 `desktop.cpp` wrappers have **zero** in-tree callers (searched `src`, `examples`, `test`
  for `User32::<name>` across all 18 - only the definitions match), and a comment-stripped search for direct SDK calls
  (`OpenWindowStation|OpenDesktop|SwitchDesktop|GetUserObjectInformation(W|A)?\(` excluding `User32::`) finds **zero
  bypass sites** - the single hit is the declaration at `user32.h:416`. So this section is 18/18 U15 instances and
  0/18 U19 sites, which makes several folder decisions (U17 `T`-suffix consistency, P3, and whether `EnumDesktopsT`'s
  callback should be typed as the SDK's `DESKTOPENUMPROC` or as a `LPCWSTR`-taking callback now that the binding is
  always the `W` form) pure taste calls with no call site to inform them. Recorded for the folder-wide pass; nothing in
  this section is exercised by `examples/mainwindow`, so no session can observe a behaviour change here.
- **T3 - the "Power Management functions" pair is declared but has no owning TU at all.** Opened 30 Oct 2026 (session 12)
  while mapping the section that follows "Device Management functions": `user32.h:685` `//Power Management functions`
  declares `RegisterPowerSettingNotification` (`:686`) and `UnregisterPowerSettingNotification` (`:687`) - **neither is
  `static`**, unlike the device pair just above and unlike 427 of the header's 525 members - and **no `.cpp` in the folder
  defines either**; there is no `powermanagement.cpp` (nor any other file) that could hold them: the folder's 38 sources
  contain no match for either name outside the header. So they are U4 members of the plainest kind (a caller gets
  `LNK2019`, not `C2352`), and this is the sharpest U4 shape in the folder because both entry points are real and exported -
  `RegisterPowerSettingNotification` ordinal 2F1 and `UnregisterPowerSettingNotification` ordinal 3CB, both **RESOLVED** by
  the live probe (`tmp/_tmp_name_resolve_probe.cpp`) - and the SDK declares both at `winuser.h:3993`/`:4002`, inside
  `#if (_WIN32_WINNT >= 0x0502)` (`:3979`, closed at `:4022`) within the same `WINVER >= 0x0500` + Desktop-partition block
  (`:3942`, `:3940`) as this session's pair; `HPOWERNOTIFY`, which both signatures name, is typedef'd at `:3985` inside that
  same 0x0502 guard. So the section is 2 declarations, 0 definitions, 0 owning TUs: the folder is missing a file, not just a
  body. Resolving T3 means either writing the two definitions (a new `powermanagement.cpp`, which also needs an
  `add_library` line, and the `static` question should be settled with them, not after) or deleting `user32.h:685-687`; both
  move the published surface, so it belongs with the U4/U13 sweep rather than with a TU review. Handle contract for whoever
  takes it: same shape as this session's pair - the returned `HPOWERNOTIFY` is released only by
  `UnregisterPowerSettingNotification`, and the GUID argument must be a registered power-setting GUID; this library exports
  neither those GUID constants nor the `POWER_BROADCAST` payload types. **Section-coverage note:** the SDK's power-notification
  family in this SDK is four entry points - the two above plus `RegisterSuspendResumeNotification` (`:4009`, ordinal 2F6) and
  `UnregisterSuspendResumeNotification` (`:4017`, ordinal 3CD) - and this header declares **2 of 4**. (`SetWindowPowerManagementNotification`
  does not exist in SDK 10.0.26100 at all: zero hits for `PowerManagement` in `winuser.h`, and it is not in the export table.)


- **T4 - the menu half of the context-help-id family is declared and defined nowhere.** Opened 31 Oct 2026 (session 16)
  from the `//Shell functions` block: `user32.h:642` `SetMenuContextHelpId(HMENU, DWORD)` and `user32.h:643`
  `GetMenuContextHelpId(HMENU)` are declared **without `static`** (the only 2 of that block's 5 members so declared) and
  **no `.cpp` in the folder defines either** - the only hits for both names outside the header are session 16's own TU
  comment in `help.cpp`. Confirmed at the binary level, not just by source search: `dumpbin /linkermember:1` on the built
  `cmakebuild/.../Debug/winqor_api_user.lib` lists exactly three help-family members
  (`?SetWindowContextHelpId@User32@api@win@qor@@SAHPEAUHWND__@@K@Z`,
  `?GetWindowContextHelpId@User32@api@win@qor@@SAKPEAUHWND__@@@Z`,
  `?WinHelp@User32@api@win@qor@@SAHPEAUHWND__@@PEB_WI_K@Z`) and **no menu pair**
  (`tmp/_tmp_help_linkermember.txt`). Both entry points are real single exports - `SetMenuContextHelpId` 34A,
  `GetMenuContextHelpId` 17C, both **RESOLVED** by the live probe - and the SDK declares both at
  `winuser.h:9135-9140` / `:9142-9146`, in the same `WINVER >= 0x0400` + Desktop block as the window pair that **is**
  wrapped, so nothing external argues against the wrappers. A caller today is ill-formed (`C2352`: a non-static member
  cannot be qualified through the class name) rather than facing `LNK2019`, which is the only way this pair differs from
  T1/T3. Fix is symmetric: add the two definitions to `help.cpp` (with `static`, matching `:644-645`) or delete
  `user32.h:642-643`. Neither touches an ABI, both move the published surface, so it belongs with the U4/U13 sweep.
  Contract for whoever adds them: the id is a plain `DWORD` property of the menu (0 clears it), no handle is acquired, and
  `GetMenuContextHelpId`'s `DWORD` return makes failure indistinguishable from a legitimate 0 - the same reason the
  window pair gets no `CheckReturn`.

Remaining queue (21 TUs, alphabetical; `accelerator.cpp`, `accessibility.cpp`, `brush.cpp`, `caret.cpp`,
`clipboard.cpp`, `combobox.cpp`, `console.cpp`, `timer.cpp`, `coordtransform.cpp`, `cursor.cpp`, `desktop.cpp`,
`devicecontext.cpp`, `devicemanagement.cpp`, `dialog.cpp`, `errorhandling.cpp`, `filledshapes.cpp`, `help.cpp` done):
hook, icon, ime, keyboard, mdi, menu, message, monitor, mouse, painting,
processthread, rectangle, resource, scrollbar, systemshutdown, text, window, windowclass, windowprocedure,
windowproperties, winqor_api_user (module glue). `hook.cpp` is in the build (see U3) and is next.

## Defect census (sessions 1-16 snapshot; moved verbatim out of status.md on 1 Nov 2026)

### Defect census, sessions 1-18 (19 TU entries, 141 wrappers)
- **Run-time-breaking defects: 9, all fixed.** 6 bad bindings - `combobox.cpp` x2 (session 6, P1), `coordtransform.cpp`
  x3 (session 8, P5's "wrong stem" class), `dialog.cpp` x1 (session 13, P1); plus 3 declaration defects that made every
  call site ill-formed - the three WinEvent members missing `static` (session 2b, C2352 x3) and `ConsoleControl` missing
  `static` (session 7, U13, now closed). Sessions 17 (`hook`) and 18 (`icon`) found none: both TUs' bindings were already
  D13-correct and verified three ways.
- **Still open in unreviewed TUs:** P1 x5 (`keyboard.cpp` x3, `window.cpp:124`, `mouse.cpp:82`), P2 x6 (`menu.cpp` x2,
  `message.cpp` x4), P3 x2 (`CreateWindowT`, `CopyCursorT` naming), U4's ~97 declared-but-undefined members including
  tasks T1/T3/T4, and U18's `window.cpp` `void`-returning point converters.
- **Convention/consistency fixes:** 10 parameter names made to match the SDK (`devicemanagement` 2, `hook` 1, `icon` 7 -
  the folder-wide divergence scan now reads **0 over 426 definitions**), 2 decl/def parameter divergences and 1
  parameter type `LPTSTR` -> `LPCTSTR` (`desktop`), 2 member renames for the `T` suffix (`combobox`, the only published
  renames of the review), 1 module-token fix over 3 sites (`combobox` `User32` -> `user32`, 416/428 folder-wide now),
  3 `::DLGPROC` spellings (`dialog`), 1 implicit template-argument site (`brush`), 2 deducing `Call<>` sites (`desktop`),
  4 `()` -> `(void)` pairs (`clipboard`), 1 stray inner space (`devicecontext`), 4 trailing-space / `)` nits
  (`accessibility` x2, `errorhandling` x2), 2 nits (`console` comma space, section comment), 1 dead `#undef` in each of 2
  TUs (`console`, `help` - the folder is now clean of `#undef` in sources), 1 `::`-spelling split in a `Call<>` argument
  (`icon.cpp:48`) and 1 missing final newline (`icon.cpp`), plus several stray blank lines.
- **Documentation:** every reviewed TU now carries a TU comment block recording the D13 binding rationale, the SDK/SAL
  and ownership contracts, the section-coverage claim and the U-item tallies - the largest being `help.cpp`'s 70 lines,
  then `hook.cpp`'s 64 and `icon.cpp`'s 54.
- **TUs with 0 defects of any class** (comment-only sessions): `caret`, `clipboard`, `cursor`, `devicecontext`,
  `filledshapes`, `errorhandling`, `help`, plus `coordtransform`/`devicemanagement`/`console`/`hook`/`icon` for the
  binding half - `icon` and `hook` were clean on correctness and only needed SDK parameter names.
- **Folder-wide measures re-run by these sessions** (current values in the registers): 428 bindings over 38 TUs, P5 flags
  4 (1 live); 0 parameter-name divergences over 426 definitions (re-run 1 Nov); U14 422 explicit / 6 deducing, spacing
  250 tight / 178 spaced (re-run 1 Nov, `tmp/_tmp_u14_census_s18.txt` - the pair of numbers moved by one since session
  16's 423/179 as `icon.cpp:48`'s mis-spelled `Call<>` argument became countable in the normal shape); U15 and U19
  per-family tallies accumulate in their own entries; `user32.h` 525 declarations, 428 `static` since session 7.

