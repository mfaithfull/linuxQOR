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
| 14..38 | not started (TU index; 24 left, see "Remaining queue") | | | |

**Next session: `errorhandling.cpp`** (alphabetical from here; 14 TUs done across sessions 1-13: `accelerator`,
`accessibility`, `brush`, `caret`, `clipboard`, `combobox`, `console`, `timer`, `coordtransform`, `cursor`, `desktop`,
`devicecontext`, `devicemanagement`, `dialog`). **4 wrappers over 4 declarations**, a 46-line TU with no comment block.
The "Error Handling functions" section (`user32.h:613-617`, declarations at `:614-617`) is fully covered by it, so unlike
most of the folder it is not a U4 block. In `add_library`. **What is deliberately not in the queue: there is no
`dde.cpp`** - the 34-declaration DDE section (`user32.h:501-534`) has no TU anywhere, which is a U4 surface decision, not
a TU session (see the U4 entry). Measured pre-flags for the next session (from this session's scans, so they are facts,
not guesses):
- **Expect 0 P1 and 0 D13 conversions - but prove it.** All four bindings are bare (`:19 FlashWindow`, `:26 FlashWindowEx`,
  `:33 MessageBeep`, `:42 SetLastErrorEx`) and all four are genuine **single exports**: 115, 116, 283, 342 in
  `tmp/_tmp_user32_exports.txt`, with no `A`/`W` spelling of any of them anywhere in the table. Run `cl /EP`,
  `tmp/_tmp_name_resolve_probe.cpp` (add the four spellings) and `_tmp_wrong_stem_scan.ps1` as usual.
- **U18 candidate to settle, not assume.** `SetLastErrorEx` is this TU's only `void` wrapper and uses `Library::voidCall`
  (`:43`); the SDK prototype is `winuser.h:13561`, and U18 exists precisely because a `void` here contradicted an
  unsigned-returning pair elsewhere - read the SDK return type before accepting it.
- **U16 guard stack (measured with `tmp/_tmp_guard_stack_scan.ps1`)**: `FlashWindow` `winuser.h:4683` under
  `WINAPI_PARTITION_DESKTOP`; `FlashWindowEx` `:4699` additionally under `#if(WINVER >= 0x0500)`; `MessageBeep` `:9365`
  additionally under `#ifndef NOMB`; `SetLastErrorEx` `:13561` under DESKTOP only. So `user32.h:614-617` always compiles,
  but at `WINVER < 0x0500` the `FlashWindowEx` prototype vanishes (the wrapper's `Call<>` then has no prototype to pair
  with) and `NOMB` removes `MessageBeep`'s.
- **Section coverage**: `user32.dll` exports three more error-adjacent entry points that `user32.h` declares nowhere -
  `RegisterErrorReportingDialog` (2E6), `SetDebugErrorLevel` (32F) and `DdeGetLastError` (8E, in the DDE block).
  `GetLastError`/`SetLastError` are correctly absent: they are `kernel32` exports (0 hits in the `user32` table).
- **Return-check asymmetry to record**: `MessageBeep` alone carries `CheckReturn<BOOL, BoolCheck>` (`:34`); `FlashWindow`
  returns the *previous* flash state rather than a success flag, so a `BoolCheck` there would be wrong, and
  `FlashWindowEx`/`SetLastErrorEx` have no check at all (U1). Two U14 spacing nits: `:35` `(pFunc, uType )` and
  `:43` `(pFunc, dwErrCode, dwType )`.
- The name-divergence scan should report **0** for every TU from here on: session 12 took the folder to zero (2 of 426
  definitions, both in `devicemanagement.cpp`) and session 13 re-confirmed 0 of 426.
Take the pre-scan P1/P2/P3/P5 entries into account only when their owning TU comes up; do not fix other TUs outside
their own session.

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


Remaining queue (24 TUs, alphabetical; `accelerator.cpp`, `accessibility.cpp`, `brush.cpp`, `caret.cpp`,
`clipboard.cpp`, `combobox.cpp`, `console.cpp`, `timer.cpp`, `coordtransform.cpp`, `cursor.cpp`, `desktop.cpp`,
`devicecontext.cpp`, `devicemanagement.cpp`, `dialog.cpp` done):
errorhandling, filledshapes, help, hook, icon, ime, keyboard, mdi, menu, message, monitor, mouse, painting,
processthread, rectangle, resource, scrollbar, systemshutdown, text, window, windowclass, windowprocedure,
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


## cursor.cpp - session 9 (30 Oct 2026)

Sixteen wrappers, sixteen bindings - the largest TU reviewed and **0 defects of any class** (binding, signature, UB,
convention). It is therefore the TU where the method had to earn its keep: nothing to fix, so everything the earlier
sessions had been asserting by inspection was measured instead. Two instruments changed as a result, one open item (U20)
was opened from the caller side, and the pre-flagged P3 case was settled as correct.

| Wrapper (`user32.h:381-397`, all 16 `static`) | Definition | Binding (`cl /EP` literal) | Export (ordinal, RVA) | SDK prototype (`winuser.h` 10.0.26100) | Verdict |
|---|---|---|---|---|---|
| `ClipCursor(const RECT*) -> BOOL` | `:53` | `user32, ClipCursor` -> `"ClipCursor"` | 4E, 0002EA90 | `:9594 BOOL WINAPI ClipCursor(_In_opt_ CONST RECT*)` | correct |
| `CopyCursorT(HCURSOR) -> HCURSOR` | `:60` | `user32, CopyIcon` -> `"CopyIcon"` | 59, 00027DF0 | `:10976 HICON WINAPI CopyIcon(_In_ HICON)`; `CopyCursor` is the `:10681` macro | correct (see below) |
| `CreateCursor(HINSTANCE,int,int,int,int,const VOID*,const VOID*) -> HCURSOR` | `:67` | `"CreateCursor"` | 60, 0004CF40 | `:10659` | correct |
| `DestroyCursor(HCURSOR) -> BOOL` | `:74` | `"DestroyCursor"` | AF, 00029250 | `:10671` | correct |
| `GetClipCursor(LPRECT) -> BOOL` | `:81` | `"GetClipCursor"` | 134, 0002EE40 | `:9447` | correct |
| `GetCursor(void) -> HCURSOR` | `:88` | `"GetCursor"` | 13F, 0002EE90 | `:9453 HCURSOR WINAPI GetCursor(VOID)` | correct |
| `GetCursorInfo(PCURSORINFO) -> BOOL` | `:95` | `"GetCursorInfo"` | 141, 0002EEA0 | `:14661` | correct |
| `GetCursorPos(LPPOINT) -> BOOL` | `:102` | `"GetCursorPos"` | 142, 0002B230 | `:9426` | correct |

| `GetPhysicalCursorPos(LPPOINT) -> BOOL` | `:109` | `"GetPhysicalCursorPos"` | 193, **0002B230** | `:9439` | correct |
| `LoadCursorT(HINSTANCE,LPCTSTR) -> HCURSOR` | `:116` | `useswinapiAW` -> `"LoadCursorW"` | 259, 00015C30 | `:10619` (macro `:10623`) | correct |
| `LoadCursorFromFileT(LPCTSTR) -> HCURSOR` | `:123` | `useswinapiAW` -> `"LoadCursorFromFileW"` | 258, 00048300 | `:10642` (macro `:10645`) | correct |
| `SetCursorPos(int,int) -> BOOL` | `:130` | `"SetCursorPos"` | 32E, 0002FA50 | `:9398` | correct |
| `SetPhysicalCursorPos(int,int) -> BOOL` | `:137` | `"SetPhysicalCursorPos"` | 354, **0002FA50** | `:9406` | correct |
| `ShowCursor(BOOL) -> int` | `:144` | `"ShowCursor"` | 395, 0002FD90 | `:9386 int WINAPI ShowCursor(_In_ BOOL)` | correct |
| `SetCursor(HCURSOR) -> HCURSOR` | `:151` | `"SetCursor"` | 32C, 0002A790 | `:9420 HCURSOR WINAPI SetCursor(_In_opt_ HCURSOR)` | correct |
| `SetSystemCursor(HCURSOR,DWORD) -> BOOL` | `:158` | `"SetSystemCursor"` | 36A, 0007EB90 | `:10728` | correct |

- **All sixteen resolve, and by two independent routes.** `cl /EP` of the TU prints the sixteen `GetProcAddress` literals
  (`tmp/_tmp_cursor_pp.txt:453241-453346`), and a live probe that resolves them exactly the way `qor_pp_useswinapi` does
  (`GetModuleHandleW(L"user32")` then `GetProcAddress`) resolves **16 of 16 RESOLVED**, plus the A/W alternatives, plus the
  names that should fail (`tmp/_tmp_cursor_probe.cpp`: `CopyCursor` MISSING, `SetCursorContents`/`ShowSystemCursor`/
  `GetCursorFrameInfo` RESOLVED). Note the probe's first line: `GetModuleHandleW(L"user32")` returns NULL in a process that
  has not mapped the DLL - the library never `LoadLibrary`s, so `pFunc` would be cached NULL forever. That is G5/U5
  demonstrated rather than asserted; recorded, not fixed.
- **None of the sixteen stems is in the `user32.h:9-17` `#undef` list** (measured against the header: 9 names -
  `RegisterDeviceNotification`, `WinHelp`, `SystemParametersInfo`, `GetRawInputDeviceInfo`, `DlgDirList`,
  `DlgDirSelectEx`, `DlgDirListComboBox`, `DlgDirSelectComboBoxEx`, `LoadBitmap`). So G2 - the SDK `stem -> stemW` macro
  making a bare-stem binding a live P1 failure - cannot bite in this family, which is why fourteen bare
  `qor_pp_useswinapi` spellings are correct here rather than a defect class.
- **The pre-scan P3 flag (`CopyCursorT` -> `CopyIcon`) is a correct binding, and now for a measured reason.** `CopyCursor`
  is not an export at all: `winuser.h:10681` is `#define CopyCursor(pcur) ((HCURSOR)CopyIcon((HICON)(pcur)))` (inside
  `#ifndef _MAC`), and the live probe says `CopyCursor` MISSING / `CopyIcon` RESOLVED. It is also the mirror case the U17
  census reports for this section (`T` suffix with no A/W family). Left as-is: renaming a member exported through
  `QOR_WINAPIUSER` is a published-surface change and belongs with the folder-wide U17/P3 decision.
- **The type story behind that wrapper, measured rather than assumed:** `HCURSOR` *is* `HICON` in this build.
  `windef.h:88` is `DECLARE_HANDLE(HICON)` and `:131` is `typedef HICON HCURSOR;`; the separate
  `DECLARE_HANDLE(HCURSOR)` at `:133` is the `#else` arm of the same `#ifndef _MAC`, so it is not what this TU gets.
  `/EP` of `cursor.cpp` contains exactly one HCURSOR spelling - `typedef HICON HCURSOR;`
  (`tmp/_tmp_cursor_pp.txt:174186`). So `Library::Call< HCURSOR, HCURSOR >` against the `HICON WINAPI(HICON)` export is
  type-identical and needs no cast; the only remaining hazard in that path is U6's `__cdecl` `fPtr`, shared by every
  wrapper in the folder.
- **Session 8's defect class is closed for this family by construction, and that was tested.** A wrapper in this class
  cannot call the SDK entry point of the same name even by accident: inside `User32`, an unqualified `SetCursorPos(x, y)`
  binds to the member itself. Probe `tmp/_tmp_recursion_probe.cpp` compiles that shape and MSVC gives
  `warning C4717: 'User32::SetCursorPos' : recursive on all control paths, function will cause runtime stack overflow` -
  a warning, not an error, so it would still build and then recurse. All 16 bodies here go through `pFunc`, and
  `tmp/_tmp_wrong_stem_scan.ps1` over the whole folder now flags `cursor.cpp:63` only as the known `CopyIcon` alias: 4
  flags folder-wide after session 8's fix, this one, `mouse.cpp:82` (`TrackMouseEvent` binding
  `winapiAW(user32, LPTRACKMOUSEEVENT)` - a live P1-class find for mouse.cpp's session), and the two `resource.cpp`
  "binding outside a `User32::` definition" lines that reproduce U11 mechanically.
- **Two run-time facts the SDK declarations do not show** (export table, this `user32.dll`): the logical/physical pairs are
  *one implementation exported twice* - `GetCursorPos` 142 and `GetPhysicalCursorPos` 193 both at RVA **0002B230**,
  `SetCursorPos` 32E and `SetPhysicalCursorPos` 354 both at RVA **0002FA50**. Whatever DPI behaviour the
  `WINVER >= 0x0600` declarations draw, it is not observable through this table on this build.

- **Ownership is now written where it belongs.** `CreateCursor`, `LoadCursorFromFileT` and `CopyCursorT` return handles the
  caller must `DestroyCursor`; `LoadCursorT` and `GetCursor` return shared system handles that must not be destroyed;
  `SetSystemCursor` takes no ownership (its SDK declaration carries no ownership SAL). `ClipCursor` is `_In_opt_` (NULL
  clears the clip rect) and `GetCursorInfo` requires `pci->cbSize` from the caller first. `ShowCursor` returns the signed
  display *count*, so 0 or negative is a normal hide - there is no failure channel for a return check to read. All of that
  is now in the TU comment; none of it is stated in the callers, which is **open item U20** (four measured caller-side
  findings: default-`true` ownership in the `Cursor`/`Icon` ctors, `ButtonController` omitting the flag its sibling
  `StaticController` passes, a spurious `PrimitiveHandle` leak warning on every destruction because `~Cursor` never clears
  `m_handle`, and `Cursor::Show` collapsing the count to `bool`). Nothing outside `cursor.cpp` was touched.
- **U19 - the cleanest family of the review: 0 bypass sites in built code.** 17 **LAYER** sites (one per wrapper,
  `LoadCursorT` twice, all in `ui/gui/gdiobjects/cursor.cpp`, built at `gui/CMakeLists.txt:43`), 13 raw **BYPASS** hits, of
  which 9 are not SDK calls at all once the enclosing declaration is read (`BlobBuffer::SetCursor`, `Input::DestroyCursor`,
  the two-argument `Platform::SetCursor`, two member declarations). That is why `_tmp_bypass_scan.ps1` gained a fourth
  class, **MEMBER**. The remaining 4 are live SDK calls, all in one file and all in a target that is **not in the build**:
  `ui/gl/windowimpl.cpp:247` (`SetCursor` + `LoadCursorW`), `:254`, `:272` (`::ClipCursor(nullptr)`), `:2219`
  (`::SetCursorPos`) - `winqor_gl` is commented out at `ui/CMakeLists.txt:4` and has no project anywhere in
  `cmakebuild/QOR.sln` (149 projects scanned). Those four lines are CODE, not comment (`tmp/_tmp_comment_state.ps1`; the
  file is 1503 comment-only lines of 2637).
- **U15 - the first TU with a call site for every wrapper, and the sharpest illustration of the item.** At wrapper level
  16/16 are called; one level up, the twelve `Cursor` methods that make those calls (`Show`, `Clip`, `GetClip`,
  `GetCurrent`, `GetInfo`, `GetPosition`, `GetPhysicalPosition`, `SetPosition`, `SetPhysicalPosition`, `SetAsCurrent`,
  `SetAsSystemCursor`, `Clone`) have no call site anywhere in `src`/`examples`/`test`, so fifteen wrappers are linked and
  never executed. Only `LoadCursorT` is live end to end: `examples/experimental/osspecific/windows/mainwindow/mainwindow.cpp:339`
  (a project that *is* in `QOR.sln`) -> `ui/gui/factories/windowfactory.cpp:90`
  (`windowClass->SetCursor(Cursor(IDC_ARROW))`) -> `Cursor::Cursor(const wchar_t* id)` (`gdiobjects/cursor.cpp:49-53`,
  `m_owning = false` - correct for a shared system cursor) -> `User32::LoadCursorT` -> `LoadCursorW`. That path is also
  what makes the U20 findings observable rather than theoretical.
- **Open items this session contributes to (recorded, not acted on):**
  - U1 - 16 more call sites, 0 with `Win32ErrorHandler _;` (0 of 38 TUs unchanged).
  - U2 - 0 `CheckReturn`, and this TU shows why the item is a behaviour decision rather than a typo: `CheckNonZero` on
    `ShowCursor` would be wrong by construction (a count), `CheckPointer` on the three allocating wrappers would turn a
    legitimate NULL-on-failure into an issue, and `SetCursor` returns NULL legitimately to mean "no previous cursor".
  - U3, U10, U11, U12, U13 - none. U4 - none: all sixteen members are declared `static` (`user32.h:382-397`) and defined.
  - U5 - 16 more process-lifetime-cached bindings; the probe above demonstrates the unmapped-module case.
  - U6 - 16 more `Library::Call` sites whose `fPtr` is default `__cdecl` against `__stdcall` exports.
  - U7 - applies to all sixteen, worst for the four handle-returning wrappers, where the swallowed default return
    (`nullptr`) is the same value the API legitimately returns for "no cursor"/"previous cursor was NULL".
  - U8 - untested; only the `LoadCursorT` path could be exercised without inventing a new caller (through
    `WindowFactory::AddWindowClass`).
  - U14 - (b) 16/16 sites state the full SDK argument list, so nothing to fix; (a) this is the first file with an
    *internal* spacing split - 15 `Call< BOOL, LPRECT >` against 1 `Call<BOOL, const RECT*>` (`cursor.cpp:57`) - left
    alone under the no-churn rule and recorded as the folder-wide pass's per-file deviation; (c) `GetCursor(void)` already
    follows the `(void)` norm.
  - U16 - **3 of 16** pair with guarded SDK declarations: `GetPhysicalCursorPos` (`:9439`) and `SetPhysicalCursorPos`
    (`:9406`) inside `#if(WINVER >= 0x0600)` (`:9435`, `:9402`); `GetCursorInfo` (`:14661`) and the
    `CURSORINFO`/`PCURSORINFO` types in its signature (`:14644-14650`) inside `#if(WINVER >= 0x0500)` (`:13594`) with
    `#ifndef NOWINABLE` (`:13716`) between. The other thirteen sit under nothing but `#ifndef NOUSER` plus a
    `WINAPI_FAMILY_PARTITION` (Desktop; Desktop|Games; Desktop|PC-App for `ClipCursor`). Guard stacks printed by
    `tmp/_tmp_sdk_guard_scan.ps1` are kept in `tmp/_tmp_cursor_sdk.txt`. Reverse half: nothing - the seven cursor exports
    not wrapped here are declared nowhere in `user32.h`, so no consumer-visible declaration outranks the SDK.
  - U17 - the mirror case `CopyCursorT` (`:383`) confirmed as *binding-correct, name-indirect*; census unchanged.
  - U18 - none: no return-type contradiction in this section (`ShowCursor`'s `int` is right; the loss is in the caller,
    which is U20).
  - U19, U20 - see the two entries above; U20 opened by this session.

- **Section coverage.** "Cursor functions" (`user32.h:381-397`) is exactly these sixteen. Not wrapped and not declared
  anywhere in `user32.h`: `GetCursorFrameInfo` (140), `SetCursorContents` (32D), `ShowSystemCursor` (399) - all three
  RESOLVED by the live probe - plus `SetThreadCursorCreationScaling` (36D), `GetPointerCursorId` (194),
  `GetPointerDeviceCursors` (196), `MITGetCursorUpdateHandle` (274). `SetSystemCursor`/`ShowSystemCursor` remain supported
  exports although Microsoft documents both as unsupported from Windows 8 up (a documentation claim, not re-measured).
- **Conventions checked and clean:** licence header, include order with the Windows headers last, `qor_pp_fcontext` at all
  16 sites, lowercase `user32` module token, section comment `//Cursor functions` in house style, no local `#undef`,
  4-space indent, no tabs, definitions in header order, all sixteen declaration/definition pairs present in one TU.
- **Fixed this session:** the TU comment block only (D13 binding rationale with ordinals and RVAs, the
  `CopyCursor`-is-a-macro note, the `HCURSOR`-is-`HICON` note, the shared-RVA finding, per-wrapper handle ownership, the
  `ShowCursor` count-not-failure contract, the guard exposure and the list of unwrapped cursor exports). No header change,
  no rename, no binding change.
- **Verified at exit:** `cursor.cpp` compiles standalone at `/W4` exit 0; the whole library compiles **38/38 at `/W4`,
  exit 0, no diagnostics** (`tmp/_tmp_cl_all.log`, re-run after the edit). No published surface moved, so no caller was
  affected.
- **Instruments this session leaves behind:** `tmp/_tmp_cursor_probe.cpp` + `tmp/_tmp_run_cursor_probe.bat` (the
  `GetModuleHandleW`/`GetProcAddress` probe for this family, including the negative controls),
  `tmp/_tmp_cursor_sdk_scan.ps1` -> `tmp/_tmp_cursor_sdk.txt` (SDK signature + guard stack per cursor declaration),
  `tmp/_tmp_recursion_probe.cpp` + `tmp/_tmp_recursion_run.bat` (proves a wrapper cannot call its own SDK name in this
  class - C4717), `tmp/_tmp_cursor_bypass_s9.txt` (the U19 split) and the **MEMBER** class added to
  `tmp/_tmp_bypass_scan.ps1`, which every later session's U19 count now benefits from.

## desktop.cpp - session 10 (30 Oct 2026)

- **Scope:** 18 wrappers, the "Window Station and Desktop functions" section (`user32.h:399-417`). 18 declarations
  <-> 18 definitions, header order preserved; declaration/definition agreement (return type, parameter types, parameter
  **names**) checked mechanically with `tmp/_tmp_desktop_sigcmp.ps1`, which before the fixes reported exactly the 2
  divergences in items 1-2 below and after them reports **18/18 ok** (its one `HEADER LINE NOT PARSED 398` line is the
  section comment, not a defect). No hidden signature drift beyond those two.
- **Bindings (method steps 3 and 3b) - all 18 correct as written, 0 defects:** 9 SDK A/W families bound
  `qor_pp_useswinapiAW` (CreateDesktop, CreateDesktopEx, OpenDesktop, EnumDesktops, CreateWindowStation,
  OpenWindowStation, EnumWindowStations, GetUserObjectInformation, SetUserObjectInformation) and 9 single exports bound
  bare `qor_pp_useswinapi` (CloseDesktop, CloseWindowStation, GetProcessWindowStation, GetThreadDesktop, OpenInputDesktop,
  SetProcessWindowStation, SetThreadDesktop, SwitchDesktop, EnumDesktopWindows) - already D13-conformant before the
  session. `cl /EP` (`tmp/_tmp_pp_desktop.bat` -> `tmp/_tmp_desktop_pp.txt`) resolves the 18 binding literals; all 18 names are in
  `tmp/_tmp_user32_exports.txt`; the live probe `tmp/_tmp_desktop_probe.cpp` + `tmp/_tmp_run_desktop_probe.bat` resolves
  all 18 through the library's own `GetModuleHandleW`/`GetProcAddress` path and gets non-null handles back from
  `GetProcessWindowStation` and `GetThreadDesktop`, which also proves the section is usable rather than merely loadable.
  `_tmp_wrong_stem_scan.ps1` re-run after the edits: 38 files, 426 `User32::` definitions, 428 bindings, **4 folder-wide
  flags, none in this TU** (unchanged set: `mouse.cpp:82` live P1, `cursor.cpp` `CopyCursorT`->`CopyIcon` = P3,
  2 x `resource.cpp` = U11).
- **Fixed (4 consistency defects, no behaviour change):**
  1. `OpenWindowStationT` parameter type `LPTSTR` -> `LPCTSTR`, in the declaration (`user32.h:412`) and the definition
     in the same pass. The SDK takes `LPCSTR`/`LPCWSTR` and every other `T`-suffix wrapper in this section spells the
     parameter `LPCTSTR`, so the divergence was in this wrapper, not in the convention. The member has no in-tree caller
     (task T2), so no caller can observe the change.
  2. `CreateDesktopT`'s definition parameter `pDevMode` -> `pDevmode`, matching its declaration at `user32.h:402`.
     Cosmetic, but parameter-name divergence is exactly what the `sigcmp` script exists to catch.
  3. U14(b): `GetThreadDesktop` `Call< HDESK >(pFunc, dwThreadId )` -> `Call< HDESK, DWORD >(pFunc, dwThreadId)`
     (stray space before `)` removed in the same edit).
  4. U14(b): `OpenWindowStationT` `Call< HWINSTA >(pFunc, lpszWinSta, fInherit, dwDesiredAccess)` ->
     `Call< HWINSTA, LPCTSTR, BOOL, ACCESS_MASK >(pFunc, ...)`. The folder's worst deducing site - three argument types
     were being inferred from the wrapper's own signature.
  5. Documentation: a 28-line TU comment block (`desktop.cpp:16-45`) in the house style - per-binding D13 rationale with
     the `winuser.h` line ranges, the `HDESK`/`HWINSTA` release contract (released by `CloseDesktop`/`CloseWindowStation`,
     **not** `Kernel32::CloseHandle`, and returned raw, so dropping one leaks the object), the thread/process rules
     (`SetThreadDesktop` fails once the thread has a window or a hook on that desktop; `SwitchDesktop` is per-process), the
     always-`W` consequence for the `DESKTOPENUMPROC`/`WINSTAENUMPROC` callback name, section coverage, and U1's 0/18
     `Win32ErrorHandler` note.
- **Checked and deliberately *not* changed:** `GetUserObjectInformationT` / `SetUserObjectInformationT` keep the folder's
  `T` suffix (correct for A/W wrappers; the U17 list stays a folder-wide decision); `EnumDesktopWindows` has **no** `T`
  suffix because it is a genuine single export with no A/W family - the mirror image of the U17 pattern, correct as it
  stands; `CreateDesktopExT`'s `ulHeapSize`/`pvoid` parameters match the SDK's `CreateDesktopExW`; the section's only
  zero-argument member (`GetProcessWindowStation`) already spelled `(void)` in both declaration and definition, so U14(c)
  contributes nothing here.
- **Open items this session feeds:** U1 - 18 sites, 0 `Win32ErrorHandler` (settled policy, unchanged); U2 - 0 return
  checks, and this section shows the shape of that decision: 4 handle-returning wrappers would take `CheckNonZeroHandle`,
  8 `BOOL` wrappers `CheckReturn<BOOL>`, and `GetUserObjectInformationT` reports "buffer too small" *only* through its
  `lpnLengthNeeded` out-parameter, which the wrapper neither checks nor forwards; U5 applies to all 18 bindings; U15
  **+18**, the largest dead-surface block of the review (task T2); U19 - 0 LAYER and 0 BYPASS sites; U16 - every SDK
  declaration this section pairs with sits inside `WINAPI_PARTITION_DESKTOP` + `!NODESKTOP` / `!NOWINDOWSTATION`
  (`winuser.h:1391-1661`), so a non-DESKTOP `WINAPI_FAMILY` build loses the whole section while `user32.h:399-417` still
  declares it - recorded, not acted on, per the session-5 deferral of the version-pin decision.
- **Verified at exit:** `desktop.cpp` compiles standalone at `/W4`, exit 0; the whole library compiles **38/38 at `/W4`,
  exit 0** (`tmp/_tmp_compile_all.ps1`, re-run after the edits). U14 census re-run: folder **423 explicit / 6 deducing**
  (was 420/8), spacing 250 tight / 179 spaced; `desktop.cpp` 18 explicit / 0 deducing, all 18 spaced.
- **Instruments this session leaves behind:** `tmp/_tmp_desktop_sigcmp.ps1` (declaration <-> definition comparison
  including parameter names - reusable in every later session), `tmp/_tmp_pp_desktop.bat` (`cl /EP` of one TU),
  `tmp/_tmp_desktop_bindings.ps1` (binding-vs-member listing for one TU), `tmp/_tmp_desktop_probe.cpp` +
  `tmp/_tmp_run_desktop_probe.bat` (live `GetProcAddress` probe with non-null-handle positive controls),
  `tmp/_tmp_desktop_sdk_26100.txt` (the `winuser.h` declarations with their guard stacks) - but note
  `tmp/_tmp_desktop_sdk_10240.txt` came out **0 bytes**, so this session could not compare the desktop declarations
  against the older 10.0.10240 SDK: U16's claim for this section rests on the 26100 headers alone, and the older-SDK
  comparison is still open for whichever session takes up the U16 pin decision.

## devicecontext.cpp - session 11 (30 Oct 2026)

- **Scope:** 8 wrappers, the `//DC functions` section (`user32.h:325-333`). 8 declarations <-> 8 definitions, header order
  preserved. Declaration/definition agreement (return type, parameter types, parameter **names**) plus the `Call<...>`
  arity/type check run mechanically with `tmp/_tmp_sigcmp.ps1` - generalised this session from the session-10 one-off
  script, now `-Tu <file> -First <line> -Last <line>`; it reports **8/8 `decl==def ok` and 8/8 `Call ... ok`**, with no
  name-only divergence either. No signature drift at all in this section, which is why the session's code edit count is 1.
- **Bindings (method steps 3 and 3b) - all 8 correct as written, 0 defects:** 5 SDK A/W families bound
  `qor_pp_useswinapiAW` (ChangeDisplaySettings, ChangeDisplaySettingsEx, EnumDisplayDevices, EnumDisplaySettings,
  EnumDisplaySettingsEx) and 3 single exports bound bare (`GetDC`, `GetDCEx`, `ReleaseDC`) - already D13-conformant.
  `cl /EP` (`tmp/_tmp_pp_dc.bat` -> `tmp/_tmp_dc_pp.txt`) resolves the 8 binding literals at lines 453278-453327 to
  `ChangeDisplaySettingsW`, `ChangeDisplaySettingsExW`, `EnumDisplayDevicesW`, `EnumDisplaySettingsW`,
  `EnumDisplaySettingsExW`, `GetDC`, `GetDCEx`, `ReleaseDC`. Export table (`tmp/_tmp_user32_exports.txt` lines 1542-1545,
  1759-1765, 1829-1830, 2270): the bound forms are ordinals 29, 28, FD, 102, 101, 143, 144, 2FF; the A halves are
  26, 27, FC, FF, 100 and are exported but unreachable through this surface. `_tmp_wrong_stem_scan.ps1` re-run after the
  edits: 38 files, 426 `User32::` definitions, 428 bindings, **4 folder-wide flags, none in this TU** (unchanged set).
  None of this TU's stems is in the `user32.h:9-17` `#undef` list, so the G2/P1 hazard does not apply here.
- **Fixed (1 consistency nit + documentation):**
  1. U14 spacing: `GetDC`'s `Call< HDC, HWND >(pFunc, hWnd )` -> `(pFunc, hWnd)`. This TU's only such site; 8/8 sites
     state the full SDK argument list (0 deducing).
  2. A 36-line TU comment block (`devicecontext.cpp:15-50`) in the house style: per-binding D13 rationale with the
     `winuser.h` line ranges and ordinals, the **two opposite zero-return conventions inside one section**, the
     `_Reserved_`/`_In_opt_`/`_Inout_` parameter contracts (incl. "the caller must set `dmSize`/`dmDriverExtra` first"),
     the `GetDC`/`GetDCEx` -> `ReleaseDC` pairing (never `Kernel32::CloseHandle`) and the `hWnd`/`hDC` match requirement,
     section coverage with the `GetWindowDC`/`WindowFromDC` cross-reference, and the U16 guard picture.
- **Checked and deliberately *not* changed:** all five A/W wrappers already carry the `T` suffix, so **U17 contributes
  nothing from this TU** (unlike the next one); `GetDC`/`GetDCEx`/`ReleaseDC` correctly have no suffix (single exports);
  `ChangeDisplaySettingsExT`'s `hwnd` stays a free `HWND` even though the SDK marks it `_Reserved_`
  (`winuser.h:13101`, `:13110`) - tightening it to a forced `NULL` would change an exported signature; `lParam` is
  `LPVOID` matching the SDK. No return checks added anywhere (U2 policy) - and this TU is the strongest argument yet that
  that is a behaviour decision, not an oversight (see U2 below).
- **Open items this session feeds:**
  - U1 - 8 call sites, 0 `Win32ErrorHandler` (settled policy).
  - U2 - 0 return checks, plus the sharpest case study of the review: `ChangeDisplaySettingsT`/`ChangeDisplaySettingsExT`
    return `DISP_CHANGE_*` codes where **zero is success** (`DISP_CHANGE_SUCCESSFUL == 0`, positive = reboot needed,
    negative = failure), while the three `EnumDisplay*` wrappers and `ReleaseDC` use the convention where **zero is
    failure**. One `CheckReturn<>` policy cannot cover this one section, and a `CheckNonZero` over the
    ChangeDisplaySettings pair would flag every success as a failure.
  - U5 (binding cached for the process lifetime) applies to all 8. U7 is worse here than anywhere else in the folder:
    the SEH default `ret _{}` is `0`, which for these two wrappers **is `DISP_CHANGE_SUCCESSFUL`** - a fault inside a
    display-mode change would read as a successful one. Recorded, not fixed.
  - U15 - **+1 dead wrapper**: `ChangeDisplaySettingsT` has **no call site anywhere** in `src`/`examples`/`test`.
    `ChangeDisplaySettingsExT`'s only wrapper-level caller is `DisplayDevices::ChangeSettings`
    (`ui/gdi/displaydevices.cpp:70`), and `DisplayDevices` has no user outside its own header. The other five are called
    from built `ui/gui`/`ui/gdi` code (see U19).
  - U19 - **16 LAYER / 12 BYPASS, the largest split of the review** (full entry under U19). Instrument change: the scan
    now matches `::stem(` anywhere in a line, not only at line start - the first version missed
    `if (::ChangeDisplaySettingsExW(...` (`ui/gl/monitorimpl.cpp:138`) and `const HDC dc = ::GetDC(NULL);` (`:263`),
    giving 10 where the fixed instrument gives 12. Bare-stem runs under-count far worse (8 / 4): see step 5b.
  - U16 - 4 of the 13 SDK declarations this section pairs with are version-guarded (`WINVER >= 0x0500`), and the TU adds
    a **new U16 sub-case**: the whole display block sits inside `_WINGDI_` / `!NOGDI` / `!NOSYSPARAMSINFO` and five
    wrapper signatures name `wingdi.h` types, so a `NOGDI` consumer does not merely lose prototypes, it gets an
    ill-formed `user32.h:326-330`. None of those macros is defined anywhere in `cmake/` or in any `CMakeLists.txt`
    (measured this session).
  - U21 (new) - the section name over-promises, and the SDK's DC neighbourhood is split across `devicecontext.cpp` and
    `painting.cpp`.
  - U22 (new) - the `DeviceContext` release path this layer feeds calls `ReleaseDC` with a NULL `HWND`, so the DC is not
    released. Unlike U20 this one **is on a built, reachable path** (`mainwindow` is a project in
    `cmakebuild/QOR.sln:902`, verified this session).
- **Verified at exit:** `devicecontext.cpp` compiles standalone at `/W4`, exit 0; the whole library compiles **38/38 at
  `/W4`, exit 0**, no diagnostics (`tmp/_tmp_compile_all.ps1` -> `tmp/_tmp_cl_all.log`: 38 TU echo lines, the single
  `error|warning` grep hit being the filename `errorhandling.cpp`; run summary in `tmp/_tmp_cl_all_s11b.log`). U14 census
  re-run: folder **423 explicit / 6 deducing**, spacing 179 spaced / 250 tight - unchanged, since this TU's one fix is a
  spacing nit inside the already-explicit 423.
- **Instruments this session leaves behind:** `tmp/_tmp_sigcmp.ps1` - now the reusable declaration<->definition +
  `Call<>` checker for any section (`-Tu`, `-First`, `-Last`). Two instrument hazards were found and fixed here, and both
  produced false findings before they were caught: (a) the parameter **must not** be named `$Src`, because PowerShell
  names are case-insensitive so `$Src` aliases the `$src` file-content array - the first run reported "NO DEFINITION" for
  all 8 wrappers; (b) `cl /EP` output must go to a per-TU file, since the shared `_tmp_pp_one.bat` writes
  `_tmp_pp_out.txt` and reading it before `cl` finished returned a stale, unrelated TU's preprocessed text. Also
  `tmp/_tmp_dc_bypass.txt` (the under-counted bare-stem run, kept as the worked example) and
  `tmp/_tmp_dc_bypass2.txt` (the corrected suffix-variant run).


## devicemanagement.cpp - session 12 (30 Oct 2026)

- **Scope:** the "Device Management functions" section, `user32.h:681-683` - 2 declarations (`RegisterDeviceNotification`
  `:682`, `UnregisterDeviceNotification` `:683`), 2 definitions, both in the header's order. Second-smallest TU of the
  review (30 lines before this session), and the first one whose whole SDK block (`winuser.h:3939-3977`) is covered 2 of 2
  while the block **after** it is not covered at all (T3).
- **0 binding defects - and the first TU where both binding spellings were already right for measurable reasons.**
  `qor_pp_useswinapiAW(user32, RegisterDeviceNotification)` -> `/EP` gives `"RegisterDeviceNotificationW"`; the bare
  `qor_pp_useswinapi(user32, UnregisterDeviceNotification)` is correct because `UnregisterDeviceNotificationA`/`W` are both
  **MISSING** in the live probe. The export table alone cannot show that - it lists only what exists, and the absence of the
  `A`/`W` spellings is what makes bare right, which is why the probe (not the table) is the authority here.
- **G2 live for this stem, harmless in practice.** `RegisterDeviceNotification` is `#undef`'d at `user32.h:9` and bare
  `"RegisterDeviceNotification"` is MISSING (probe), so had the binding been bare it would have thrown
  `"Missing library function."` on every call; it is not bare, and the AW macro token-pastes `W` before the SDK macro could
  ever be consulted, so the `#undef` is inert for this TU. Recorded as the first G2 case where the hazard is confirmed to be
  genuinely live rather than theoretical.
- **2 defects fixed, both parameter naming.** `notificationFilter` -> `NotificationFilter` and `handle` -> `Handle` in the
  definitions (`:73`/`:77`, `:80`/`:84` of the file as it now stands, comment block included), matching both the declarations and the SDK's names.
  **Convention for line references in this file:** `user32.h` line numbers are stable (the header was not touched this session), but
  in-TU line numbers move whenever a session's comment block grows - so cross-references from the U-items to a TU cite the
  number as of that session, and `user32.h` numbers in preference to TU numbers wherever both identify the same thing.
  These were the **only two such
  divergences in the entire folder**: `tmp/_tmp_name_divergence_scan.ps1` compares parameter names case-sensitively across
  every `User32::` definition and measured 2 in 426 definitions before the fix, 0 after.
- **Instrument blind spot corrected.** `_tmp_sigcmp.ps1` had used PowerShell `-ne` for names, which is case-insensitive, so
  it reported `decl==def ok` for exactly these two sites. Switched to `-cne` and re-ran the previously reviewed TUs: no
  earlier verdict changes (`devicecontext.cpp` 8/8, `desktop.cpp` 18/18 including its session-10 `pDevmode` fix,
  `combobox.cpp` 3/3).
- **Convention counts:** U13 0 (both `static`); U14(b) 0 (2/2 explicit `Call<>`); U17 1 instance of 2 (`:682` on the
  11-member list, **no rename** per the settled policy) - sharpened here because the A/W choice is invisible in the
  signature: the filter is a bare `LPVOID` and the bound half decides how the payload's trailing `TCHAR` name field is read;
  U15 2/2 (no caller anywhere); U19 0 LAYER / 1 BYPASS (`ui/gl/helperwindow.cpp:85`, live code, correct struct setup, but
  the file is in no target and `gl` is commented out of `ui/CMakeLists.txt:4`); U16 2/2 with the hardest dependency yet -
  `HDEVNOTIFY` is typedef'd inside the same `WINVER >= 0x0500` + Desktop-partition guards as the prototypes, so a consumer
  pinning a lower `WINVER` gets `user32.h:682-683` **ill-formed**, not merely unprototyped; U1/U2 unchanged (no handler, no
  `CheckReturn`).
- **New deferred task T3** (see the Deferred tasks section): the Power Management pair at `user32.h:686-687` is **not**
  `static`, is declared, and is exported for real (2F1/3CB, both RESOLVED in the probe) - yet it is defined in no TU, in a
  folder that has no file for it. U4's cleanest case.
- **Verified at exit:** `devicemanagement.cpp` compiles standalone at `/W4` exit 0; whole library **38/38** exit 0
  (`tmp/_tmp_cl_all_s12.log`, 38 `.cpp` echoed, 38 `.obj` refreshed); `/EP` re-run post-edit confirms both binding literals
  (`tmp/_tmp_devman_pp.txt:453294`, `:453301`); the name-divergence scan reports 0 for this TU and 0 folder-wide.
- **Instruments this session leaves behind:** `tmp/_tmp_name_divergence_scan.ps1` (folder-wide case-sensitive parameter-name
  comparison - run it with no `-Tu` for the folder sweep, expect 0), the corrected `_tmp_sigcmp.ps1` (`-cne`), the extended
  `tmp/_tmp_name_resolve_probe.cpp` (device-notification + power-setting spellings) and `tmp/_tmp_pp_devman.bat`.
- **Bookkeeping corrected in this file:** the "Remaining queue" line listed 25 names under a "26 TUs" count because
  **`text.cpp` had been omitted** from the queue; it is now listed, and the counts reconcile (13 done + 25 remaining = the
  38 sources in `add_library`). The stray `| 2b |` row that had been stranded below the Deferred-tasks block is back inside
  the session log.

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

