# winqor_api_user - source review status

Review of `src/platform/os/windows/api_layer/user` (the `User32` wrapper class over `user32.dll`)
for correctness, consistency, safety, undefined behaviour and conformance to the api_layer conventions.

Protocol: **one translation unit per session** - the whole point is not to lose findings between sessions.
Small, unambiguous defects get fixed as they are found; anything that changes library-wide behaviour or
needs a decision goes in the open-items list below and is *not* touched.

This file carries the session log, the registers (G/P/U/T) and the consolidated session notes; the verbatim per-TU
narratives of sessions 1-15 and the consumed `help.cpp` pre-flags are in `session-log-archive.md` beside it.
`memory-bank/activeContext.md` carries a pointer here.

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

## Session log

| # | TU | Wrappers | Verdict | Action taken |
|---|---|---|---|---|
| 1 | `accelerator.cpp` | 5 | 0 correctness bugs, 3 fragility/consistency defects | Fixed: bare-stem bindings on the three A/W families made explicit `qor_pp_useswinapiAW` (29 Oct 2026); conforms to D13 |
| 2 | `accessibility.cpp` + `timer.cpp` | 1 + 2 | 0 correctness/UB defects; 1 file-organisation defect (the two files held each other's bodies), 2 style nits | Fixed: swapped the bodies so each file matches its `user32.h` section (was U12), `( pFunc … )` -> `(pFunc …)` at both call sites, added single-export comments. All three bindings confirmed correct-as-written by `cl /EP` + export table; no A/W conversion needed |
| 2b | `accessibility.cpp` (same day, after user commit `89da3452` added the three missing WinEvent wrappers) | +3 | 0 binding defects; **1 call-site defect found: the three declarations lacked `static`, so every call site was `error C2352`** | Fixed: `user32.h:662-664` made `static` (verified by a compile-only caller probe, before = 3 errors, after = exit 0); TU comments generalised to the four WinEvent entry points. Census re-run: `user32.h` now 524 declarations, 427 static, 97 non-static (was 100) - see U4, and new U13 for the one remaining defined-but-non-static member |
| 3 | `brush.cpp` | 1 | 0 correctness/UB/binding defects; 1 convention defect (implicit template args), 1 documentation gap | Fixed: `Call< HBRUSH >` -> `Call<HBRUSH, int>` (explicit SDK arg types - folder norm 382/428, reference `shell`+`kernel` 344/387) + a single-export and handle-ownership comment added. Binding correct as written: no A/W family, `cl /EP` resolves to `"GetSysColorBrush"`, which is the export (table index 1958, ordinal 1C6) |
| 4 | `caret.cpp` | 8 | 0 correctness/UB/binding/convention defects - the cleanest TU so far | Fixed: TU comment only (single-export binding rationale under D13 + the `CreateCaret` bitmap contract). All 8 bindings correct as written; `cl /EP` resolves all 8 to the literal stems, all 8 are exports, and they are the only 8 `*Caret*` entries in `user32.dll`, so the section is complete. U14(b)'s "caret 2" was a miscount - caret.cpp has 0 deducing call sites (re-measured, see U14) |
| 5 | `clipboard.cpp` | 20 | 0 correctness/UB/binding/convention defects in the wrappers - second clean TU after `caret.cpp`, and the first already D13-conformant before the session started | Fixed: `()` -> `(void)` at the three zero-argument definitions (`CloseClipboard`, `CountClipboardFormats`, `GetOpenClipboardWindow`) plus their three `user32.h` declarations (the SDK spells `VOID`; folder norm now 36 defs against 2, 37 decls against 2 - see U14(c)) + a TU comment (bare-stem rationale for the 18 single exports, the one-switch A/W argument, the clipboard affinity/ownership contracts, section coverage and deprecation). All 20 bindings verified correct as written by `cl /EP` + export table |
| 6 | `combobox.cpp` | 3 | **2 real run-time failures** - pre-scan P1: both `DlgDir*` bindings asked `GetProcAddress` for stems `user32.dll` does not export, so those wrappers could only ever throw; plus 2 consistency defects (no `T` suffix on an A/W wrapper, `User32` module token) and 1 documentation gap | Fixed: both bindings now `qor_pp_useswinapiAW(user32, ...)` (`cl /EP` resolves `"DlgDirListComboBoxW"`/`"DlgDirSelectComboBoxExW"`, both RESOLVED by live probe); members renamed `DlgDirListComboBoxT`/`DlgDirSelectComboBoxExT` at `user32.h:551-552` + definitions (user decision; new **U17** tracks the 11 remaining A/W wrappers without the suffix); module token `User32` -> `user32`; TU comment added. `GetComboBoxInfo` correct as written (single export, ordinal 13D, no A/W family). Also produced a **new pre-scan P1 finding** in `window.cpp:124` (`CreateWindowW` MISSING) |
| 7 | `console.cpp` | 1 | 0 correctness/binding/UB defects - and the first wrapper in the review with **no SDK prototype to pair against at all**: neither `ConsoleControl` nor `CONSOLECONTROL` nor the five console-internal structures are declared anywhere in SDK 10.0.26100.0 or 10.0.10240.0. 1 declaration defect (**U13** - the only defined member declared non-static) + 3 nits (dead `#undef WinHelp`, `//Console` section comment, missing comma space) | Fixed: `static` added at `user32.h:690` (U13 closed; caller probe `tmp/_tmp_console_static_probe.cpp` - before = 1 x C2352 exit 2, after = exit 0), `,DWORD` -> `, DWORD`, `//Console` -> `//Console functions`, dead `#undef WinHelp` removed, TU comment added. Binding correct as written: `cl /EP` -> `"ConsoleControl"`, which is the only `*Console*` name in the export table (ordinal 1594, hint 55, RVA 00028FD0, not a forwarder); live probe RESOLVED with `ConsoleControlA`/`ConsoleControlW` MISSING, so the bare stem is the D13 spelling and the missing `T` suffix is correct |
| 8 | `coordtransform.cpp` | 3 | **3 real run-time defects** - all three wrappers bound `GetDCEx`, a real export, so every existing check (export table, `cl /EP`, compile) passed while no wrapper called its own entry point; 0 convention defects (`static` already correct at `user32.h:377-379`) | Fixed: bindings `GetDCEx` -> `ClientToScreen`/`ScreenToClient`/`MapWindowPoints` (`cl /EP` resolves the three literals; live probe RESOLVED with `*A`/`*W` all MISSING, so the bare stem is the D13 spelling) + TU comment (binding rationale, `_Inout_` POINT ownership, the "non-zero `MapWindowPoints` return is a delta, not a failure" contract, section coverage). No header change, no rename. New pre-scan **P5** covers the defect class; new **U18** records `window.cpp`'s `void`-returning point converters |
| 9 | `cursor.cpp` | 16 | **0 binding / correctness / UB / convention defects** - largest clean TU of the review; all 16 signatures pair field-for-field with the `winuser.h` prototypes and all 16 bindings resolve (14 bare single exports + 2 A/W switches). The two claims a reader would take on trust were measured instead: `CopyCursorT` -> `CopyIcon` is the only correct binding of a name the SDK spells as a macro, and `HCURSOR` *is* `HICON` in this build (`/EP` gives exactly one `typedef HICON HCURSOR;`) | Fixed: TU comment block only (binding rationale + ordinals/RVAs, the `CopyCursor`-is-a-macro note, per-wrapper handle ownership, the `ShowCursor` count-not-failure contract, the unwrapped cursor exports). No header change, no rename (P3/U17 decision: deferred to the folder-wide pass). New **U20** opened from the caller side |
| 10 | `desktop.cpp` | 18 | 0 binding / correctness / UB defects - all 18 bindings resolve (9 A/W families bound `qor_pp_useswinapiAW`, 9 single exports bound bare; `cl /EP` + export table + live probe), 0 P1/P5 flags; 4 consistency defects (2 U14(b) deducing sites, 2 parameter decl/def divergences), 1 documentation gap | Fixed: `OpenWindowStationT`'s parameter `LPTSTR` -> `LPCTSTR` in **both** the declaration `user32.h:412` and the definition (the only decl/def parameter-type divergence in the section), `CreateDesktopT`'s definition parameter `pDevMode` -> `pDevmode` to match its declaration, both U14(b) sites made explicit (`GetThreadDesktop`, `OpenWindowStationT`) + a 28-line TU comment (binding rationale, `HDESK`/`HWINSTA` ownership, thread/desktop contracts, callback widening, section coverage). **18/18 wrappers have no in-tree caller** - the largest U15 block of the review. Two items deferred as tasks **T1** and **T2** (see below) |
| 11 | `devicecontext.cpp` | 8 | **0 binding / correctness / UB / convention defects** - all 8 signatures pair field-for-field with the `winuser.h` prototypes and all 8 bindings resolve (5 A/W families bound `qor_pp_useswinapiAW` -> the `*W` export, 3 single exports bound bare: `GetDC` 143, `GetDCEx` 144, `ReleaseDC` 2FF); 0 P1/P5 flags, 0 U13/U14(b)/U17/U18/U20 instances; 1 style nit | Fixed: `(pFunc, hWnd )` -> `(pFunc, hWnd)` + a 36-line TU comment (binding rationale and ordinals, the **two opposite zero-return conventions** in this one section, the `_Reserved_`/`_In_opt_`/`_Inout_` parameter contracts, `GetDC`/`GetDCEx`/`ReleaseDC` pairing, section coverage, U16 guards including the hard `NOGDI`/`wingdi.h` type dependency). New **U21**: the SDK's DC neighbourhood (`winuser.h:8560-8614`) is split across this TU and `painting.cpp` (`GetWindowDC`, `WindowFromDC`), while 5 of this TU's 8 members are display-configuration entry points with no `HDC` at all. **Largest U19 split of the review - 16 LAYER / 12 BYPASS** - only 4 of the 16 LAYER sites are reachable at run time, and all 12 BYPASS sites are in the disabled `ui/gl` tree. Also found two **scan-instrument blind spots** that had been under-counting families in earlier sessions (bare-stem matching and line-start-only `::` matching) - see step 5b and the U19 entry |
| 12 | `devicemanagement.cpp` | 2 | DONE 30 Oct (session 12) - 0 binding / correctness / UB / convention defects; **2 naming defects, and they are the only 2 in the whole folder** | "Device Management functions" (`user32.h:681-683`): 2 declarations, 2 definitions, header order, both `static`, both `User32::`-qualified (U13 clean), both `Call<>` sites fully explicit (U14: 2/2 explicit, spaced style, no spacing nit). **Both bindings were already D13-correct**: `qor_pp_useswinapiAW(user32, RegisterDeviceNotification)` -> `/EP` resolves `"RegisterDeviceNotificationW"` (2E5, RESOLVED), and the bare `qor_pp_useswinapi(user32, UnregisterDeviceNotification)` is the right spelling because the probe shows `UnregisterDeviceNotificationA`/`W` both **MISSING** - a genuine single export (3C6). This stem is one of the nine `user32.h:9-17` `#undef`'s **and** bare `"RegisterDeviceNotification"` is MISSING (G2 confirmed live for this stem), but the AW token-paste means the `#undef` never reaches the binding, so the pre-flagged question was naming-only. **Fixed (2 sites, both naming):** definition parameters renamed to their declaration/SDK names - `notificationFilter` -> `NotificationFilter` (`:73` signature + `:77` call arg), `handle` -> `Handle` (`:80` + `:84`; line numbers of the file as it now stands, with the TU comment block in place). Measured outlier: the new instrument `tmp/_tmp_name_divergence_scan.ps1` reports **2 divergent parameter names across 426 definitions folder-wide - both in this TU**. TU comment block added. **U17 1/2** (no rename, per the session-11 policy) with a sharpened argument: the A/W choice here is invisible in the signature, since the filter is a bare `LPVOID` whose trailing `TCHAR` name field is read per the bound half. **U15 2/2** - the first section where every wrapper is dead *and* the family is still used in the tree, through a bypass. **U16 2/2** - both prototypes sit under `#if(WINVER >= 0x0500)` (`winuser.h:3942`) + `WINAPI_PARTITION_DESKTOP` (`:3940`), and `HDEVNOTIFY` is itself typedef'd at `:3943` under the same guards, so a lower `WINVER` makes `user32.h:682-683` **ill-formed** rather than merely absent (the hardest U16 dependency so far after `NOGDI`). **U19: 0 LAYER / 1 BYPASS** - `ui/gl/helperwindow.cpp:85` `::RegisterDeviceNotificationW` is live code (file 0% commented) that fills `dbcc_size`/`dbcc_devicetype`/`dbcc_classguid` first (`:79-83`), but the file is in no source list (`winqor_gl` = `winqor_gl.cpp cursorimpl.cpp windowimpl.cpp wgl.cpp`) and `ui/CMakeLists.txt:4` comments `add_subdirectory(gl)` out -> source debt, not run-time behaviour. **New deferred task T3**: the adjacent "Power Management functions" pair (`user32.h:686-687`; exports 2F1/3CB, both RESOLVED in the probe) is defined in no TU and has **no owning file in the folder at all**. 22 open items unchanged (U1-U22) |
| 13 | `dialog.cpp` | 22 | **1 real run-time failure + 1 convention defect**; 1 published-signature defect and 1 cosmetic inconsistency recorded, not fixed | Fixed: `:89` `qor_pp_useswinapi(user32, GetDlgItemTextT)` -> `qor_pp_useswinapiAW(user32, GetDlgItemText)` (`/EP` post-fix `"GetDlgItemTextW"`, probe RESOLVED - pre-fix asked for a name the DLL does not export) and `:124` `MessageBox` -> explicit AW (no behaviour change, G1). All 22 bindings then confirmed by `/EP` + export table + live probe; the 9 bare spellings are correct because the SDK declares those nine entry points with a single non-A/W spelling. decl==def 22/22; 0 parameter-name divergences (folder now 0 of 426). New **U23** from `MessageBoxIndirectT`'s `const ::LPMSGBOXPARAMS` |
| 14 | `errorhandling.cpp` | 4 | **0 binding / correctness / UB / convention defects** - the "Error Handling functions" section is fully covered by this TU (4 declarations, 4 definitions: 0 U4, 0 U11, 0 U13); all four bindings bare and all four correct as written (`cl /EP` -> `"FlashWindow"`, `"FlashWindowEx"`, `"MessageBeep"`, `"SetLastErrorEx"`, all four **RESOLVED** by live probe with every `*A`/`*W` spelling **MISSING**, so the bare stem is the D13 spelling and `AW` would break all four); `SetLastErrorEx`'s `void` wrapper matches the SDK's `VOID` (`winuser.h:13558-13563`), so **no new U18 instance**; 2 trailing-space nits at the call sites | Fixed: TU comment block (46 lines, `:16-61`) + `(pFunc, uType )` -> `(pFunc, uType)` at the `CheckReturn` site and `(pFunc, dwErrCode, dwType )` -> `(pFunc, dwErrCode, dwType)` at the `voidCall` site. New **U24** from the one in-tree caller, which reads `FlashWindow`'s previous-active-state `BOOL` as a success flag |
| 15 | `filledshapes.cpp` | 3 | **0 binding / correctness / UB / convention defects** - the "Filled Shapes functions" section is fully covered by this TU (3 declarations, 3 definitions: 0 U4, 0 U11, 0 U13); all three bindings bare and all three correct as written (`cl /EP` -> `"FillRect"`, `"FrameRect"`, `"InvertRect"`, all three **RESOLVED** by live probe with every `*A`/`*W` spelling **MISSING**, so the bare stem is the D13 spelling and `AW` would break all three); return types match the SDK exactly (`int`/`int`/`BOOL` at `winuser.h:9726-9747`), so **no new U18 instance**; U16 gets **0 of 3** - the three prototypes sit under nothing but the Desktop partition + `NOUSER`, no version guard | Fixed: TU comment block (61 lines, `:16-76`) + one blank line removed between the comment and the first definition. New **U25** from this TU's `CONST::RECT*` spelling (17 header lines use `CONST::`, 7 use `const ::`); `sigcmp` prints a **false** `CALL TYPE MISMATCH` on all three sites here - instrument note below |
| 16 | `help.cpp` | 3 (over 5 declarations) | **0 binding / correctness / UB / convention defects** - the `//Shell functions` block (`user32.h:641-646`) is **5 declarations, 3 definitions**: the 3 here are in header order, `static`, `User32`-qualified, `Call<>` explicit 3/3, and all 3 bindings were already D13-correct (2 bare single exports - `SetWindowContextHelpId` 37A, `GetWindowContextHelpId` 1E1, no `A`/`W` spelling of either exists - plus the folder's second confirmed **AW**: the table has **no bare `WinHelp`**, only `WinHelpA` 3EC / `WinHelpW` 3ED, probe `WinHelp` **MISSING** / `WinHelpW` **RESOLVED**); 0 U11, 0 U13, 0 U14(b), 0 U17, 0 U18, 0 U25; **U16 0 of 3** (the types are version-independent, unlike `FLASHWINFO`/`HDEVNOTIFY`); 0 parameter-name divergences (folder zero holds for a fifth TU); the four SDK help-id prototypes have **unnamed parameters**, so parameter names cannot be SDK-compared here at all | Fixed: the inert `#undef WinHelp` at `:14` removed - measured as the **only** `#undef` directive in any of the 38 `.cpp` files, so the folder is now clean of them - + a 70-line TU comment (`:16-85`) + 2 stray blank lines; file 39 -> 108 lines, pure ASCII; `user32.h` untouched. **2 U4 members** (`:642-643` `SetMenuContextHelpId`/`GetMenuContextHelpId` - declared non-static, defined in no TU, and **absent from the built import library** per `dumpbin /linkermember:1`, which lists exactly the 3 help-family members) -> task **T4**. New **U26** (TCHAR width is an include-order property, not a build setting) + a measured correction to **U2** (`../returncheck.h` is included by 37 of 38 TUs and used by 2). **U15 3/3**, **U19 3 LAYER / 0 INDIRECT / 0 BYPASS / 0 MEMBER** - cleanest split of the review |
| 17..38 | not started (TU index; 21 left, see "Remaining queue") | | | |

**Next session: `hook.cpp`** (alphabetical from here; 17 TUs done across sessions 1-16: `accelerator`,
`accessibility`, `brush`, `caret`, `clipboard`, `combobox`, `console`, `timer`, `coordtransform`, `cursor`, `desktop`,
`devicecontext`, `devicemanagement`, `dialog`, `errorhandling`, `filledshapes`, `help`). The "Hook functions" section is
`user32.h:315-319`: **4 declarations, all `static`, all 4 defined here** (`hook.cpp:16`, `:23`, `:30`, `:37`) - a 44-line
TU with no comment block. `_tmp_sigcmp.ps1 -Tu hook.cpp -First 316 -Last 319` already gives **4/4 decl==def ok and 4/4
Call ok** (run 31 Oct). `hook.cpp` is in `add_library` and compiles standalone at `/W4` (U3, resolved by user commit
`05d3ca2f`), but its review is still open.

Measured pre-flags for `hook.cpp` (session 17) - from session 16's scans, so they are facts, not guesses:
- **2 `AW` bindings and 2 bare ones, and here the `AW` pair is load-bearing rather than cosmetic.** The export table
  (`tmp/_tmp_user32_exports.txt:1532-1535`, `:2405-2407`, `:2452`) has `CallMsgFilter` **1C**, `CallMsgFilterA` 1D
  (RVA `00088CF0`), `CallMsgFilterW` 1E (`00019740`), `CallNextHookEx` 1F, `SetWindowsHookExA` 38A,
  **`SetWindowsHookExAW` 38B** - an exported name this header never mentions - `SetWindowsHookExW` 38C and
  `UnhookWindowsHookEx` 3BF. Unlike `WinHelp`, **neither `CallMsgFilter` nor `SetWindowsHookEx` is in the nine
  `user32.h:9-17` `#undef`s** (that list is `RegisterDeviceNotification`, `WinHelp`, `SystemParametersInfo`,
  `GetRawInputDeviceInfo`, `DlgDirList`, `DlgDirSelectEx`, `DlgDirListComboBox`, `DlgDirSelectComboBoxEx`, `LoadBitmap` -
  re-read from the header 31 Oct), and `tmp/_tmp_winuser_aw_macros.txt` has all four SDK macros
  (`CallMsgFilter -> CallMsgFilterW` / `-> CallMsgFilterA`, `SetWindowsHookEx -> SetWindowsHookExW` / `-> A`), so G1's
  nested-expansion route is **live** here: a bare `qor_pp_useswinapi(user32, CallMsgFilter)` would still produce
  `"CallMsgFilterW"`. Run `cl /EP` to confirm which literal each of the four bindings yields, and note the trap: bare
  `CallMsgFilter` is a real export at 1C aliasing the **A** entry point, so a wrong choice would resolve at run time
  instead of throwing.
- **2 more U17 instances** (`CallMsgFilterT`, `SetWindowsHookExT`: A/W family, `T` suffix, no `A`/`W` in the name) for
  the folder-wide pass; `CallNextHookEx`/`UnhookWindowsHookEx` have no A/W family at all, so their bare spelling is the
  D13 form.
- **U15 4/4 - every wrapper dead, and no bypass either.** `_tmp_bypass_scan.ps1` over the four stems (31 Oct,
  `tmp/_tmp_hook_pre.bat`): `stems: 4  through-layer (User32::): 0  intermediate-qualified: 0  SDK bypass: 0
  member-call (not SDK): 0` - the first section of the review with no caller and no direct SDK use anywhere.
- **U16 guard scan still to run**: `tmp/_tmp_hook_guards.ps1` + `tmp/_tmp_hook_guards.bat` are staged for it. The direct
  `-File` form printed `hits: 0` on 31 Oct because a comma list arrives as one literal string (the array pitfall in the
  Tooling section) - use the wrapper, which passes a real array.
- Contracts to work out in the session: `SetWindowsHookEx` **acquires** an `HHOOK` that only `UnhookWindowsHookEx`
  releases, so this is the first section where the folder's "an acquired handle is returned for the caller to release"
  rule clearly applies; `HOOKPROC` is a callback the caller supplies (compare the session-10 callback-widening note);
  `dwThreadId`/`hMod` thread-affinity rules decide whether a `WH_GETMESSAGE`-style hook is per-thread or global, which
  changes what `CallNextHookEx` must be passed.

Take the pre-scan P1/P2/P3/P5 entries into account only when their owning TU comes up; do not fix other TUs outside
their own session.

**What is deliberately not in the queue: there is no `dde.cpp`** - the 34-declaration DDE
section (`user32.h:501-534`) has no TU anywhere, which is a U4 surface decision, not a TU session (see the U4 entry).

**Consumed pre-scan notes.** The `help.cpp` (session 16) pre-flags that used to sit in this space are superseded in full
by the session-16 row in the table above, the **U26** entry and task **T4**; the pre-flag text itself is kept verbatim in
`session-log-archive.md` under "Consumed pre-scan notes". Sessions 15/16's `filledshapes`/`help` pre-flags were consumed
the same way and are not repeated here.

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
   **Feed the scan every suffix variant of the stem** (`stem`, `stemA`, `stemW`, `stemT`): it matches `stem\s*\(`, so a
   run with bare stems alone misses every QOR layer call (which uses the `T`-suffixed member) and every bypass that
   calls the `W` export directly. Measured 30 Oct 2026 on the display family: bare stems gave 8 LAYER / 4 BYPASS, the
   full variant list with the pre-fix scan gave 16 LAYER / 10 BYPASS, and with both instrument fixes in place the same
   family gives **16 LAYER / 12 BYPASS** - the bare run hid all four `ui/gdi/displaydevices.cpp` and four
   `i_m/monitor.cpp` layer sites and six of the `*W` bypass sites. Also record, for each bypass file, whether it is
   in a CMake target at all: a bypass site in a file no target compiles is source debt, not run-time behaviour.
   **Second instrument fix this session (30 Oct 2026):** `::stem(` must be matched anywhere in the line. The previous
   pattern required either `^\s*::stem(` or a non-word, non-`:` character immediately before the stem, so a mid-line
   `if (::ChangeDisplaySettingsExW(...)` or `const HDC dc = ::GetDC(NULL);` fell through every branch - two sites in
   `ui/gl/monitorimpl.cpp` were invisible (`:138`, `:263`). Earlier sessions' per-family BYPASS counts were taken with the
   unfixed pattern and are therefore **lower bounds**; re-run the scan with the full variant list before quoting any of
   them (the cursor-family and coordinate-family entries under U19 both need that re-run).
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
replaced the faulty grep-based U14 count), `_tmp_tsuffix_census.ps1 [-Header <header>] [-Exports <exports.txt>] [-ShowAll]` (added session 6, the U17 instrument: parses the 8-space-indented member declarations of a `user32.h`-style header, strips the folder's trailing `T`, looks `<stem>A`/`<stem>W` up in an export table and reports A/W-family-without-suffix vs suffix-without-A/W; current `user32.h` census 525 parsed = 108 norm + 404 single-export + 11 deviations, plus 2 mirror-case flags), `_tmp_sdk_guard_scan.ps1 [-File <sdk header>] [-Names <name>...]` (added session 6, the U16 instrument: replays the `#if`/`#else`/`#endif` nesting of an SDK header and prints the guard stack open at each named declaration; defaults to `winuser.h` 10.0.26100), `_tmp_wrong_stem_scan.ps1 [-Dir <folder>]` (added session 8, the P5 instrument: pairs every `qor_pp_useswinapi`/`qor_pp_useswinapiAW` binding with the `User32::` member that encloses it and flags any stem that is neither that member nor its `T`-stripped base nor that base plus `A`/`W`; also mechanically reproduces U11 by reporting bindings outside any `User32::` definition), `_tmp_bypass_scan.ps1 -Stems <stems...> [-Roots <dirs...>]` (added session 8, the U19 instrument: comment-stripping scan that classifies every occurrence of a stem in `src`/`examples`/`test` as LAYER (`User32::`), INDIRECT (qualified on an intermediate class), MEMBER (`x.stem(...)` / `Class::stem` - a same-named
member, not an SDK call; added session 9 after the cursor sweep showed 9 of 13 raw hits were members) or BYPASS
(unqualified / `::`, i.e. going straight to the SDK import)), `_tmp_comment_state.ps1 -Path <file> [-Lines <n...>]` (added session 8: character-level block/line-comment and string-literal state machine; says whether given lines are CODE or COMMENT for the compiler and prints the commented-line fraction of the file - the guard against treating grep hits in commented-out regions as live). Session 11 added `_tmp_pp_dc.bat` (the same `/EP`
run written for one TU, with its own `_tmp_dc_pp.txt` / `_tmp_dc_pp_err.txt` outputs - the shared `_tmp_pp_one.bat` writes
`_tmp_pp_out.txt`, which is easy to read stale: a copy issued before `cl` finished produced a whole session's worth of
desktop.cpp lines under a devicecontext name), `_tmp_sigcmp.ps1` (generalised from the session-10 one-off: any section,
`-Tu <file> -First <line> -Last <line>`; checks return type, parameter types, parameter names and `Call<>` arity/types),
`_tmp_dc_bypass.txt` / `_tmp_dc_bypass2.txt` / `_tmp_dc_bypass3.txt` (the first, bare-stem run is
kept as the worked example of the suffix blind spot in step 5b; the third is the post-both-fixes run, 16 LAYER / 12 BYPASS),
`_tmp_aw_spans.ps1` (prints the exact `winuser.h` line span - `WINUSERAPI` through `#endif // !UNICODE` - of each A/W
family for a stem list; used to re-check the `devicecontext.cpp` comment's family ranges) and
`_tmp_cl_all_s11.log` / `_tmp_cl_all_s11b.log`.
**Session 12 added** `_tmp_name_divergence_scan.ps1 [-Tu <file>]` (the parameter-name consistency instrument: pairs every
`User32::` definition in the folder with its `user32.h` declaration and compares the parameter names **case-sensitively**;
baseline at writing = 2 divergent names in 426 definitions, both in `devicemanagement.cpp`, and 0 after that TU's fix - it
now expects 0 for every remaining TU), `_tmp_pp_devman.bat` (the per-TU `/EP` run for this TU, outputs
`_tmp_devman_pp.txt`/`_tmp_devman_pp_err.txt`) and six new probe rows in `_tmp_name_resolve_probe.cpp` for the
device-notification and power-setting spellings. **Instrument fix it also made:** `_tmp_sigcmp.ps1` compared parameter
names with PowerShell `-ne`, which is **case-insensitive**, so case-only divergence (`NotificationFilter` vs
`notificationFilter`, `Handle` vs `handle`) compared equal and the script printed `decl==def ok` - the comparison is now
`-cne`. Re-running the corrected script over the previously reviewed TUs changed no earlier verdict (`devicecontext.cpp`
8/8 still clean, `desktop.cpp`'s fixed pair still clean). Two PowerShell gotchas the new scan hit first, both recorded here
so future instruments avoid them: testing for the closing `)` on the captured *inside-parens* text can never succeed
(the capture excludes it), so the signature-continuation loop swallowed whole function bodies; and
`$x = if($c){ @(..) } else { @(..) }` **unrolls a one-element array to a scalar**, so `$x[0]` then indexed a character of
the name - assign inside the branch instead.
**Compile-log gotcha (measured 30 Oct 2026):** `_tmp_compile_all.ps1` writes the `cl` output to the fixed path
`tmp/_tmp_cl_all.log` regardless of the caller, so redirecting the script's own stdout to a per-session file captures only
its summary line (`exit=N loglines=M`) plus its internal diagnostic grep - it is not the `cl` log. The summary's `exit=0` is
already sufficient for the 38/38 claim (one `cl` invocation over all 38 sources fails as a whole if any TU fails), and the
belt-and-braces check is in `tmp/_tmp_cl_all.log`: 38 `\.cpp$` echo lines and 38 `.obj` files in `tmp/obj` newer than the
run - both confirmed for the session-11 run.

Full-library compile after the session-1 edit: 38/38 TUs, `/W4`, exit 0, no diagnostics. Re-run after the session-3
edit: same, 38/38, exit 0, no diagnostics (`tmp/_tmp_cl_all.log`). Re-run after the session-8 binding fix: same, 38/38,
exit 0, no diagnostics - the log's one `warning|error` grep hit is the filename `errorhandling.cpp`, not a diagnostic.
Re-run after the session-9 comment-only edit: same, 38/38, exit 0, no diagnostics (`tmp/_tmp_cl_all_s9.log`, `exit=0
loglines=43`, same single grep hit). Re-run after the session-11 edit (one space + a comment block): same, 38/38, exit 0,
no diagnostics (`tmp/_tmp_cl_all_s11.log`, `exit=0 loglines=43`, same single grep hit). Final session-11 re-run after the
last citation fixes: `exit=0 loglines=43`, and the `cl` log `tmp/_tmp_cl_all.log` shows **38 TU echo lines with 38 fresh
`.obj` files** in `tmp/obj` - the belt-and-braces 38/38 confirmation described above.

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

## Consolidated session notes (sessions 1-16) - the narratives moved out

**Where each kind of fact lives now.** The session-log table at the top of this file is the per-TU verdict-and-action
record. The ground rules (G1-G6), open items (U1-U26), pre-scan items (P1-P5) and deferred tasks (T1-T4) above are the
authority for every cross-cutting finding, count and decision, and they were left intact by this consolidation. The
**full per-TU narrative write-ups of sessions 2-15 are archived verbatim in `session-log-archive.md`** (1,214 lines /
122 KB, moved out of this file on 31 Oct 2026 when it passed 237 KB) together with the `help.cpp` pre-flags of sessions 15
and 16 and the `accelerator.cpp` write-up; nothing was deleted, and every load-bearing fact from those write-ups is also
carried by the registers above. Session 16 (`help.cpp`) has no narrative of its own - its record is the table row, U26, T4
and the section "Session 16 residue" below.

### Defect census, sessions 1-16 (17 TU entries, 123 wrappers)
- **Run-time-breaking defects: 9, all fixed.** 6 bad bindings - `combobox.cpp` x2 (session 6, P1), `coordtransform.cpp`
  x3 (session 8, P5's "wrong stem" class), `dialog.cpp` x1 (session 13, P1); plus 3 declaration defects that made every
  call site ill-formed - the three WinEvent members missing `static` (session 2b, C2352 x3) and `ConsoleControl` missing
  `static` (session 7, U13, now closed).
- **Still open in unreviewed TUs:** P1 x5 (`keyboard.cpp` x3, `window.cpp:124`, `mouse.cpp:82`), P2 x6 (`menu.cpp` x2,
  `message.cpp` x4), P3 x2 (`CreateWindowT`, `CopyCursorT` naming), U4's ~97 declared-but-undefined members including
  tasks T1/T3/T4, and U18's `window.cpp` `void`-returning point converters.
- **Convention/consistency fixes:** 2 parameter names (`devicemanagement`), 2 decl/def parameter divergences and 1
  parameter type `LPTSTR` -> `LPCTSTR` (`desktop`), 2 member renames for the `T` suffix (`combobox`, the only published
  renames of the review), 1 module-token fix over 3 sites (`combobox` `User32` -> `user32`, 416/428 folder-wide now),
  3 `::DLGPROC` spellings (`dialog`), 1 implicit template-argument site (`brush`), 2 deducing `Call<>` sites (`desktop`),
  4 `()` -> `(void)` pairs (`clipboard`), 1 stray inner space (`devicecontext`), 4 trailing-space / `)` nits
  (`accessibility` x2, `errorhandling` x2), 2 nits (`console` comma space, section comment), 1 dead `#undef` in each of 2
  TUs (`console`, `help` - the folder is now clean of `#undef` in sources), plus several stray blank lines.
- **Documentation:** every reviewed TU now carries a TU comment block recording the D13 binding rationale, the SDK/SAL
  and ownership contracts, the section-coverage claim and the U-item tallies - the largest being `help.cpp`'s 70 lines.
- **TUs with 0 defects of any class** (comment-only sessions): `caret`, `clipboard`, `cursor`, `devicecontext`,
  `filledshapes`, `errorhandling`, `help`, plus `coordtransform`/`devicemanagement`/`console` for the binding half.
- **Folder-wide measures re-run by these sessions** (current values in the registers): 428 bindings over 38 TUs, P5 flags
  4 (1 live); 0 parameter-name divergences over 426 definitions since session 12; U14 423 explicit / 6 deducing, spacing
  250 tight / 179 spaced; U15 and U19 per-family tallies accumulate in their own entries; `user32.h` 525 declarations,
  428 `static` since session 7.

### Verification gate (identical every session)
Edited TU compiles standalone at `/W4` exit 0; the whole library compiles **38/38 at `/W4`, exit 0, no diagnostics**;
`cl /EP` re-run after the edit still yields the expected binding literals; `user32.h` untouched unless the row says
otherwise. Session logs: `_tmp_cl_all_s9.log`, `_tmp_cl_all_s11.log`/`_s11b`, `_tmp_cl_all_s12.log`,
`_tmp_cl_all_s13.log` (`exit=0 loglines=43`, 44 objects in `tmp/obj`), sessions 14-16 via `cmd /c tmp/_tmp_cl_all.bat`
(`cl_exit=0`, 42-43-line log). The single `warning|error` grep hit in those logs is the **filename** `errorhandling.cpp`.

### Tooling added after the "Review method" index (sessions 13-16)
`_tmp_pp_dialog.bat`, `_tmp_pp_errh.bat`, `_tmp_pp_fs.bat`, `_tmp_pp_help.bat` (per-TU `cl /EP`, each writing its own
`_tmp_*_pp.txt`); `_tmp_binding_names.ps1` (reads binding literals out of a `/EP` dump in definition order - the only
practical way to audit a 453k-line preprocessed file); `_tmp_decl_no_def_scan.ps1` / `_tmp_decl_no_def_repo_scan.ps1`
(declared-but-never-defined census, 64 names in `_tmp_decl_no_def_s13.txt`, reconciled with U4's 97);
`_tmp_guard_stack_scan.ps1` (generalised U16 guard-stack scan, `-Pattern <regex>` over `winuser.h`, replacing the
hard-coded `_tmp_sdk_guard_scan.ps1`); `_tmp_fs_bypass.txt`, `_tmp_errh_bypass.txt`, `_tmp_help_bypass.txt`;
`_tmp_dump_help.bat` -> `_tmp_help_linkermember.txt` (first `dumpbin /linkermember:1` use - the tool for confirming a U4
"declared but undefined" claim at the binary level); `_tmp_tchar_probe.cpp` + `_tmp_run_tchar_probe.bat`;
`_tmp_ascii_check.ps1` (non-ASCII byte counter); `_tmp_returncheck_usage.ps1` (feeds U2/U26);
`_tmp_hook_pre.bat`, `_tmp_hook_guards.ps1`/`.bat` (staged for session 17); `_tmp_name_resolve_probe.cpp` extended by
+24 dialog spellings (+7 exported-but-undeclared), +16 error-family, +12 filled-shape and +5 help spellings;
`_tmp_status_HEAD_backup.md` (this file as of the session-15 commit, kept beside the archive).

### Instrument caveats - read a flag before acting on it
- `pwsh -NoProfile -File script.ps1 -Stems A,B -Roots x,y` **does not deliver arrays** - each comma list arrives as one
  literal string. `_tmp_bypass_scan.ps1` then reports `stems: 1` and can die at `:12` with a misleading
  `Get-ChildItem: A parameter cannot be found ...` **while still printing a totals line of 0 BYPASS from an empty file
  list**; `_tmp_sdk_guard_scan.ps1` printed `hits: 0` for four real names (a silent false negative). Correct forms: use
  the script's default roots and pass one stem, or `pwsh -Command "& script.ps1 -Stems @('A','B')"`, or a wrapper script.
  A totals line printed next to an error line is not a clean scan.
- `_tmp_sigcmp.ps1` compares **spellings as strings**, so these rows are false positives: `voidCall` rows print
  `(no Library::Call - voidCall or inlineCall?)` by design; `sdk '::DLGPROC' vs call 'DLGPROC'` (cured by spelling the
  call sites `::DLGPROC`); `CONST::X*` vs `CONST ::X*` (3/3 sites in `filledshapes.cpp`, and it will do the same in
  `rectangle.cpp`, `painting.cpp`, `text.cpp` - U25; normalize the instrument, not the source).
- `_tmp_returncheck_usage.ps1` counts `CheckReturn<` in **comments**, so its folder count over-reads (3 shown, 2 real);
  same over-count class as the `Win32ErrorHandler` matches (5, all comments).
- `_tmp_compile_all.ps1`'s `pwsh -File` wrapper returned exit 1 in session 15 while the `.bat` it writes reported
  `cl_exit=0` with a clean log - run `cmd /c tmp/_tmp_cl_all.bat` and read the log, don't trust that wrapper's exit code.
- `cl /EP` output must go to a **per-TU file**: the shared `_tmp_pp_one.bat` writes `_tmp_pp_out.txt`, and reading it
  before `cl` finished once produced a whole session's worth of another TU's text.
- Counting en/em dashes by piping `git show HEAD:<path>` through PowerShell reports **0** (cp1252 re-decode) - measure with
  `[System.IO.File]::ReadAllText` on the working file and compare code-point groups instead; do not "normalize" encoding.
  The original note with the memory-bank dash counts is in the archive's `filledshapes.cpp` section. Measured 31 Oct: this
  file has **3** non-ASCII characters (all U+2026), the archive **4**; `.cpp` sources in this folder stay pure ASCII.

### Session 16 residue (facts not carried by the registers)
- `dumpbin /linkermember:1` on the built `winqor_api_user.lib` proves the help section publishes exactly three members, so
  the `user32.h:642-643` menu pair is absent from the binary as well as from the sources (T4).
- Help coverage: the SDK's help-id neighbourhood is 4 entry points and this header declares all 4 (2 wrapped, 2 not); the
  help command vocabulary (`HELP_CONTEXT` .. `HELP_TCARD_OTHER_CALLER`, `winuser.h:12333-12355`) and
  `HELPWININFO`/`HELPWININFOW` have **no QOR-side spelling anywhere**, which is why the only caller passes a bare
  `unsigned int` for `uCommand`; and the successor API Microsoft points to, `HtmlHelp`, is declared in **no** api_layer
  header at all, even though `comctl32.cpp:324` wraps `MenuHelp` - recorded under U21/U12 placement, not a TU fix.
- U16 measured **0 of 3** for this TU although the version guard is real (`winuser.h:9105` `WINVER >= 0x0400` + Desktop
  for the four help-id prototypes; `:12270` `#ifndef NOHELP` + Desktop for `WinHelpA/W`): the difference from
  `FLASHWINFO` (session 14) and `HDEVNOTIFY` (session 12) is that **no type these wrappers name is version-gated**, so a
  lower `WINVER` leaves the header well-formed. U16 counts *type* dependencies, not macro adjacency.
- Liveness: the three wrappers' only sites are `ui/gui/controllers/windowcontroller.cpp:785`, `:790`, `:795`
  (`SetContextHelpId`, `GetContextHelpId`, `Help` - declared `windowcontroller.h:284-286`), and those three methods have
  no call site in `src`/`examples`/`test`; the gui's own `WM_HELP` path (`ui/gui/view/handlers/interactive/help.cpp:46`)
  reads the id out of the `HELPINFO` payload, so it never goes through the wrapper. No U24 instance here - all three SDK
  returns are genuine success flags or plain values, unlike `FlashWindow`'s previous-state `BOOL`.
- **Two numbers a later session should re-run**: session 16's guard-scan count and its `git diff --stat` line were taken
  while the agent-side shell validator was rejecting most `run_commands` inputs, so treat those two as unconfirmed; every
  other number here was measured with a working shell.
- Bookkeeping at the time of writing: `help.cpp` (session 16) and this file were **uncommitted**; HEAD is `9ecbf581`
  (session 15). Session 12's `devicemanagement.cpp` was likewise uncommitted when its row was written, so a working-tree
  diff of this folder can show more than one file.

### How this file was consolidated (31 Oct 2026, before session 17)
Before: 2,151 lines / 237 KB (session-15 commit) plus session 16's uncommitted additions. After: **1,201 lines / 141 KB**
(`[System.IO.File]::ReadAllLines`, trailing blank lines included; this section is the last thing in the file).
The register sections (convention, pre-scan, session log, deferred tasks, method, ground rules, open items) were kept
verbatim; session 16's additions were folded back in (table row 16, task **T4**, item **U26**, the queue and the
next-session block, all of which a `git checkout` had momentarily reverted); and the per-TU narrative was moved verbatim
to `session-log-archive.md` (1,214 lines) and replaced by the census above.
**Completeness check, run mechanically:** every non-blank line of the session-15 commit (2,049 lines) was tested for
presence in `status.md` + `session-log-archive.md`. First pass: 23 not found - the superseded `help.cpp` pre-flags (with
session 16's confirmation/correction pass over them) and the superseded queue/"not started" bookkeeping rows; all 23 were
then archived verbatim in that file's last sections. Re-run: **0 lines lost**. A fact now appears once: either in a TU's
table row, in the U/G/P/T register entry it feeds, or in this consolidated section.


