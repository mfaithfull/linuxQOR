# Archived: `winqor_api_user` review - completed-session records

Verbatim slice of the old `memory-bank/activeContext.md` lines 34-414, moved out 2 Nov 2026 so future sessions do
not re-read finished session narratives. **Nothing here is authoritative any more** - the authoritative record of
this review is `src/platform/os/windows/api_layer/user/` (`status.md` boot file, `registers/`, `archive/`). The
decisions these lines describe (D13, D14/U15, the U1/U2 return-check policy) are in force and live in
`status.md` "Convention decision" + `docs/agent/DECISIONS.md`; the counts quoted below are stale by design.

- **Session 23 (`message.cpp`) closed 2 Nov 2026** - 24 wrappers (the whole `user32.h:289 //Message and Message Queue
  functions` section), **4 fixes**: the **entire remaining P2 list** (`RegisterWindowMessage` `:214`, `SendMessageTimeout`
  `:235`, `SendMessage` `:242`, `SendNotifyMessage` `:249`) -> `qor_pp_useswinapiAW`, behaviour-neutral by `cl /EP`, so **P2
  is CLOSED (0 unresolvable-from-this-class left)**; plus U14(c)'s last zero-argument `()` -> `(void)` (`InSendMessageEx`
  def `:176` + `user32.h:300`), which **closes U14(c)** - every binding macro in the folder now spells zero arguments
  `(void)` (38 defs / 38 decls); and 3 comma-space nits in binding macros (`scrollbar.cpp` keeps the folder's last 12).
  **0 return-type and 0 parameter-type divergences** over all 24; U16 4/24 SDK prototypes guarded; U19 172 LAYER / 12
  INDIRECT / 68 BYPASS / 2 MEMBER with **all 24 wrappers live through the layer** (first TU with no zero-site family) and
  **0 bypass in built code**; **P6 second instance** (bare `BroadcastSystemMessage` is exported and is the same entry point
  as `BroadcastSystemMessageA`); U21 fifth instance; U25 fourth spelling; U26 re-measured on x64 (identical) and x86
  (divergent from `lParam` on) - the mirror repair stays a `winqor_gui` decision, recorded not fixed. 90-line TU contract
  block, 184 -> 274 lines; `user32.h` still 695 lines. Gate 38/38 at `/W4`, exit 0.
- **Session 23 was interrupted by a tool failure and recovered the same day** - the code and the register edits were already
  on disk; the recovery pass (`tmp/_tmp_s23_closeout.ps1`, `tmp/_tmp_s23_locsplice.ps1`) re-ran the gate, regenerated and
  re-spliced the register locator, rebuilt both README indexes, verified the rows 17-19 `session-log` slice rejoin (3/3
  hashes MATCH) and trimmed `status.md` from 202 lines / 21.0 KB to **180 lines / 18,432 B - exactly its ceiling**. Four
  durable caveats are now in
  `method.md`: (1) **a copied gate log cannot date its own run** - `Copy-Item` preserves `LastWriteTime`, and that pass left
  `tmp/_tmp_cl_all_s23final.log`/`_s23pre.log` that are byte-identical to session 22's log, so the gate citation in the row,
  the TU narrative and the census had to be re-pointed at `tmp/_tmp_cl_all_s23recover.log`; (2) **re-run `cl /EP` after the
  last edit** - the `_post_s23` capture predated the `user32.h` edit, so the neutral-conversion proof was redone as
  `_final_s23`; (3) PowerShell **`-like` treats a backtick as an escape**, so a pattern containing a literal backtick never
  matches - use `-match`; (4) verify locator pointers with `tmp/_tmp_u15_locator_verify.ps1`, which knows the deliberate
  `p6-u27.md` U27 exception. Two record citations were also corrected: **`archive/tu/ime.md` and `archive/tu/keyboard.md`
  do not exist** - sessions 16-20 have no TU narrative file, their narrative is the session-log row; `archive/tu/` covers
  sessions 1-15 and 21-23.
- **Session 22 (`menu.cpp`) closed 2 Nov 2026**: 39 wrappers (the whole `//Menu functions` section), **2 fixes** - the
  `GetMenuString`/`InsertMenuItem` P2 pair converted to explicit `qor_pp_useswinapiAW`, behaviour-neutral by `cl /EP`;
  the other 30 bare bindings proven correct per stem by a new four-way classifier. **0 return-type divergences** (first
  time that half of the method was measurable - the SDK return type sits on its own line, so a declaration-line
  instrument cannot see it), 3 parameter-type divergences recorded not fixed, **12 of 39** SDK prototypes
  version-guarded (U16's largest single-TU count), U21's fourth instance (plus `ChangeMenuA/W` and
  `CalculatePopupWindowPosition`, SDK menu entries declared nowhere in this library), 4 of 39 wrappers unreachable,
  0 bypass sites in built code. `archive/session-log.md` was sliced twice (rows 1-8, rows 9-16, both rejoin-hash
  verified) **before** its row was written, so it is back under its ~40 KB ceiling.
- **U15 CLOSED 2 Nov 2026 by user decision (policy **D14** in `docs/agent/DECISIONS.md`) - record-only, no code touched.**
  "Exported through `QOR_WINAPIUSER`, no reachable caller in the tree" is **not a finding for this review**: the wrapper is
  defined, linked and callable, so it is published surface, and whether the tree exercises it is a consumer-side
  (`winqor_gui`) question. From session 23 on there is **no U15 tally**: no per-TU count, no session-log column, nothing
  appended to `registers/u15.md`, whose sessions-1-22 instances stay as frozen history. Method step 5 still looks for the
  caller when another item's severity depends on reachability (P1/P5 latent-vs-live, U18's `TrackPopupMenu`, U19's
  LAYER-vs-BYPASS split) and records the fact there. U4 and tasks T1/T3/T4/T5 (declared but never defined - `LNK2019`)
  and T2 (a whole dead section's conventions) are unaffected and stay open.
- **Next: `monitor.cpp`** (session 24, 51 lines, 5 wrappers = the whole `user32.h:587`-`:592` `//Monitor functions` section).
  Pre-flags measured 2 Nov are in `status.md`: all 5 `decl == def`, 1 `AW` + 4 bare bindings already the right shape
  (`GetMonitorInfo` has no bare export), expect 0 P2/P6/U17/U18, U16 6/6 guarded by a single `WINVER >= 0x0500`, U21's sixth
  instance (no `NOMONITOR` macro at all), and U19 **8 LAYER / 4 BYPASS with the first bypass in built code since U19 opened**
  (`ui/gui/controllers/i_m/monitor.cpp:163` calls the `GetMonitorInfoW` thunk next to `User32::GetMonitorInfoT` calls) -
  record it, do not fix it there. After `monitor.cpp` the biggest open items are `window.cpp` (615 lines - decide how to
  split it **before** starting) and `mouse.cpp` (all 10 wrappers bind names that do not exist, P1).
- The running record was **split on 1 Nov 2026**: `status.md` had reached 1,222 lines / 145 KB (open items 709 lines =
  52%) and no session could hold it, which is what caused the repeated context failures and compaction cycles. Boot on
  `src/platform/os/windows/api_layer/user/status.md` - a **~175-line brief** (live state, queue, the two convention
  decisions, a generated locator giving the `registers/<file>:<line>` of every register id, the census pointer, the
  verification gate) - then open **only the slice the locator names**. `registers/` holds the verbatim G1-G6
  (`ground-rules.md`), P1-P5 (`prescan.md`), P6+U27 (`p6-u27.md`), U1-U26 (`u<n>.md`), T1-T4 (`tasks.md`) and the census -
  **one register entry per file**, indexed by `registers/README.md`; `method.md` has the review method, tooling and
  instrument caveats; `archive/` has **one narrative file per TU** plus the full session-log table, indexed by
  `archive/README.md`.
  Findings go in the register slice the locator names, narratives in `archive/tu/<tu>.md`, never in the brief, which must
  stay under 180 lines. The same pass sliced `registers.md` (1,063 lines -> 33 files) and `session-log-archive.md`
  (1,394 lines -> 19 files) by exact line range, verified by re-joining every slice and hashing it against the original
  bytes (both `MATCH`). The first split
  was mechanical and verified (1,209 of 1,222 lines moved verbatim, every id re-located by line number, encoding and line
  endings unchanged), and it caught a live defect in the record itself: **P6 and U27 had no standalone entry at all** -
  only the session-log row-17 cell - so they are now promoted verbatim into a "P6 and U27" section of `registers.md`.
  The paragraph below is the **session-17 snapshot** (it says 17 TUs / next `hook.cpp`); the live state is the brief.
  **Seventeen TUs done** in sessions 1-16 (`accelerator`, `accessibility`, `timer`, `brush`, `caret`,
  `clipboard`, `combobox`, `console`, `coordtransform`, `cursor`, `desktop`, `devicecontext`, `devicemanagement`,
  `dialog`, `errorhandling`, `filledshapes`, `help`); **next is
  `hook.cpp`** (4 wrappers over "Hook functions", `user32.h:315-319`, all `static` and all defined; `CallMsgFilterT`/
  `SetWindowsHookExT` are a **load-bearing** AW pair - unlike `WinHelp`, neither stem is in the `user32.h:9-17` `#undef`
  list, so G1 can silently rewrite a bare binding, and `SetWindowsHookExAW` (38B) is an export the header never mentions;
  `CallNextHookEx`/`UnhookWindowsHookEx` have no A/W family, so bare is correct; U15 4/4 with 0 bypass sites; the
  `SetWindowsHookEx` -> `UnhookWindowsHookEx` handle contract is the section's main item). Session 16 (`help.cpp`) fixed
  the folder's last dead `#undef` (`:14`), added a 70-line TU comment, opened **U26** (`TCHAR` width is an include-order
  property - nothing in the build defines `UNICODE`/`_UNICODE`; the failure mode is a link error on the narrow spelling) and
  task **T4** (`user32.h:642-643` menu context-help pair: declared non-static, defined in no TU, and absent from the built
  import library per `dumpbin /linkermember:1`), and corrected **U2** (`../returncheck.h` included by 37 of 38 TUs, used by
  2). `help.cpp` and `status.md` were uncommitted at HEAD `9ecbf581`.
  Session 13 (`dialog.cpp`, 22 wrappers - the largest section so far) fixed the third **P1** instance: `:89` bound
  `"GetDlgItemTextT"`, which the probe says is MISSING, now `qor_pp_useswinapiAW(user32, GetDlgItemText)`; converted
  `MessageBox` to explicit AW (no behaviour change - the SDK macro already rewrote it); made 3 `Call<...>` rows spell
  `::DLGPROC` like their declarations; and found **`MessageBoxTimeout` has no declaration anywhere in the installed SDK**,
  so its name/parameters rest on the export table + live probe alone (U16's mirror case). New open item **U23**
  (`MessageBoxIndirectT` puts `const` on the pointer, not the pointee - published-signature change, recorded not fixed).
  Session 12
  also took the folder to **0 parameter-name divergences** (new instrument `tmp/_tmp_name_divergence_scan.ps1`); session 13
  re-confirmed 0 of 426. **Instrument caveat added in session 13:** `_tmp_sigcmp.ps1`'s `CALL TYPE MISMATCH ... sdk '::DLGPROC'
  vs call 'DLGPROC'` rows are a string comparison on a *spelling*, not a type error - read a MISMATCH row before acting on it.
  U13 closed in
  session 7; U10's SDK-side half is closed too (the console-internal types collide with nothing in
  SDK 10.0.26100 or 10.0.10240), leaving only the decision about keeping/namespacing them. **T1** = `GetUserObjectSecurity`
  / `SetUserObjectSecurity` are declared (`user32.h:658-659`) with no definition in any TU (link error for any caller) -
  add the definitions or delete the declarations. **T2** = the whole Window Station / Desktop section is 18/18 dead exported
  API (0 callers, 0 bypass sites), so its U17/P3 naming questions have no caller to inform them. **T3** = the "Power
  Management functions" pair (`user32.h:686-687`) is **not** `static` and its entry points are real exports (2F1/3CB, RESOLVED
  by the probe) but **no `.cpp` in the folder defines either, and no file exists that could** - write `powermanagement.cpp`
  (+ an `add_library` line, and settle `static` with them) or delete the declarations.
- Session 1 (29 Oct 2026) reviewed `accelerator.cpp`: 5 wrappers, all signatures and exports correct; fixed
  the three bare-stem A/W bindings to the explicit `qor_pp_useswinapiAW` form (verified by `cl /EP` name
  resolution and a `/W4` compile at exit 0, and the whole library still compiles 38/38 at `/W4` exit 0).
  Nothing else in that TU was changed.
- Session 2 (29 Oct 2026) reviewed `accessibility.cpp` and, because they were transposed, `timer.cpp` too
  (open item U12): `accessibility.cpp` defined the Timer wrappers and `timer.cpp` defined the accessibility
  wrapper. The bodies were swapped back so each file matches its `user32.h` section. All three wrappers
  (`KillTimer`, `SetTimer`, `NotifyWinEvent`) are correct as written - none has an A/W family in `winuser.h` and
  `user32.dll` exports only the bare stems, so bare `qor_pp_useswinapi` is the D13 spelling. Only style fixed
  (`( pFunc` -> `(pFunc` at both sites) plus single-export source comments. U3 and U6 re-measured and corrected
  in status.md: `hook.cpp` is in `add_library` since user commit `05d3ca2f` and compiles at `/W4`, and
  `Library::DefProc` is now `__stdcall` in the uncommitted working tree - the remaining convention gap is the
  default-`__cdecl` `fPtr` inside `Library::Call`/`voidCall`.
- Session 3 (29 Oct 2026) reviewed `brush.cpp` (1 wrapper - the smallest TU): `User32::GetSysColorBrush(int)` is
  correct in every direction - `user32.h:655` static declaration, SDK `winuser.h:9694-9698`, real export
  (ordinal 1C6), no A/W family so bare `qor_pp_useswinapi` is the D13 spelling and `/EP` resolves
  `"GetSysColorBrush"`. Fixed only: `Call< HBRUSH >` -> `Call<HBRUSH, int>` (explicit SDK arg types, norm 382/428)
  and a new TU comment recording the single-export rationale plus the ownership contract (the returned brush is
  system-owned and must never be deleted - which is what the sole live caller `ui/gui/gdiobjects/brush.cpp:111-116`
  relies on via `DontClose()`). The "Brush functions" section is complete - `user32.dll` exports one `*Brush*` name.
- Session 3 also re-measured the header census with `tmp/_tmp_static_census.ps1` / `_tmp_undefined_static.ps1` and
  corrected the record: `user32.h` has 427 `static` and 98 non-static member declarations, 426 names defined through
  `User32::`, so U4 is exactly 97 declared-and-undefined plus **2 `static` ones** (`user32.h:443 CopyImage`,
  `:444 LoadImageT`) - U11 was wrong that `resource.cpp:23` is qualified; both Resource definitions are unqualified
  free functions. U13 (`ConsoleControl` defined at `console.cpp:18` but declared non-static at `user32.h:690`) and
  U14 (the 250/178 `Call<...>` spacing split and the 46 sites that pass no argument types) are now written up.
  Also corrected: the `__stdcall` `Library::DefProc` typedef is **committed** (`89da3452`, HEAD `586f4ad1`), not an
  uncommitted working-tree edit as the session-2 note said - U6's remaining half is still the `__cdecl` `fPtr`.
- Session 4 (29 Oct 2026) reviewed `caret.cpp` (8 wrappers - the "Caret functions" section `user32.h:459-467`) and
  found **nothing wrong**: all eight bindings are bare `qor_pp_useswinapi` on genuine single exports (no A/W family
  in `winuser.h:9456-9506`, no `*A`/`*W` entry in the export table, `cl /EP` resolves all eight literal stems), all
  eight signatures match the SDK types and parameter names, all eight declarations are `static`, and section coverage
  is complete both ways (the DLL exports exactly eight `*Caret*` names). Only a TU comment was added (binding rationale
  + the `CreateCaret` bitmap contract); no code line changed. U14(b) was **re-measured and corrected**: the "46 bare
  sites" figure counted zero-argument `Call< BOOL >(pFunc)` sites and every `voidCall` as deducing - the true count is
  **8 deducing sites** (`desktop.cpp:83,104`, `keyboard.cpp:69,76`, `mouse.cpp:83`, `window.cpp:118,254,317`) over 428
  sites, and `shell`/`kernel` have none at all; the new tool is `tmp/_tmp_u14_census.ps1`. Also recorded: the entire
  caret surface is exported but unreachable in-tree - its only consumers, `platform::win::Caret`
  (`ui/gui/controllers/a_h/caret.cpp`) and `WindowController::HideCaret/ShowCaret`, have no call site anywhere.
- **U15 opened (user decision, session 4):** "exported through `QOR_WINAPIUSER`, no reachable in-tree caller" is now
  one folder-wide item that each session adds instances to, instead of a per-TU note. Seeded with `caret.cpp` 8/8 and
  `hook.cpp` 4/4; `accelerator.cpp` (5/5), `timer.cpp`+`accessibility.cpp` (3/3) and `brush.cpp` (1/1) confirmed live.
  The review method gained step 6 (caller check) to feed it. **Closed 2 Nov 2026 by user decision (policy D14):** the
  observation is no longer recorded at all - see the U15 bullet at the top of this file and
  `src/platform/os/windows/api_layer/user/registers/u15.md`, whose instances are frozen.
- Session 5 (29 Oct 2026) reviewed `clipboard.cpp` (20 wrappers - the "Clipboard functions" section
  `user32.h:106-126`): **nothing wrong in the wrappers**, and the first TU already D13-conformant before it was opened
  (`GetClipboardFormatNameT`/`RegisterClipboardFormatT` bound `qor_pp_useswinapiAW`, the other 18 are single exports,
  `/EP` resolves all 20 to real exports). Fixed: `()` -> `(void)` on `CloseClipboard`/`CountClipboardFormats`/
  `GetOpenClipboardWindow` at both the definitions and the `user32.h` declarations (new sub-item U14(c) - the folder norm
  is `(void)` 33/38, the SDK spells it `VOID`), plus a TU comment with the binding rationale and the clipboard
  affinity/ownership contracts. Findings worth keeping: one `qor_pp_unicode` switch drives both the AW binding and the
  wide `TCHAR` types; `GetPriorityClipboardFormat` reports failure as 0 and "none" as -1, i.e. the opposite polarity to
  the `shell`/`kernel` non-zero idiom; all 20 are U15 instances, while the tree's real clipboard work happens in
  `ui/gl` through a **parallel direct-SDK path** (`platform.cpp:143-158`, `windowimpl.cpp:2395-2410`) that bypasses this
  library, and gui's `ClipboardHandler` is never installed (`m_clipboardOwner` unwritten). **U16 opened**: four of the
  20 SDK declarations sit behind `WINVER` guards that nothing in the build pins - **user decision: defer the pin**, each
  session now lists its version-guarded SDK names against U16 (review-method step 7) and the build-wide
  `WINVER`/`_WIN32_WINNT` choice is taken once the review is done. Library still 38/38 at `/W4` exit 0.
- Session 6 (29 Oct 2026) reviewed `combobox.cpp` (3 wrappers - the "Combo Box functions" section `user32.h:550-553`)
  and found the **first real run-time failures** of the review: `qor_pp_useswinapi(User32, DlgDirListComboBox)` and
  `(User32, DlgDirSelectComboBoxEx)` asked for stems that `user32.dll` does not export (both stems are `#undef`'d at
  `user32.h:15,16`, so the SDK macro cannot rescue them - live probe: both literals MISSING, both `W` variants
  RESOLVED), i.e. those two wrappers could only ever throw. Fixed: both now
  `qor_pp_useswinapiAW(user32, <stem>)` (`cl /EP` resolves `"DlgDirListComboBoxW"`/`"DlgDirSelectComboBoxExW"`),
  the `User32` module token -> `user32` (416/428 folder-wide now), and a TU comment with the binding rationale plus the
  buffer/ownership contracts. **User decision: rename A/W wrappers to the folder's `T` suffix as each TU is reviewed** -
  `user32.h:551-552` became `DlgDirListComboBoxT`/`DlgDirSelectComboBoxExT` (no call sites anywhere in the tree, so the
  rename is behaviour-neutral); the rest go to new **U17**, measured by the new `tmp/_tmp_tsuffix_census.ps1`: of 525
  members, 108 A/W+suffix (norm), 404 single-export bare, 11 A/W without suffix (only 4 implemented), 2 suffix with no
  A/W family. New tool `tmp/_tmp_sdk_guard_scan.ps1` gave U16 its first per-TU tally beyond clipboard:
  `GetComboBoxInfo` (+`COMBOBOXINFO`) is behind `winuser.h:13594 #if(WINVER >= 0x0500)`. `GetComboBoxInfo` is **not** a
  U15 instance (live through `ComboBoxController` -> `WindowFactory::CreateComboBox` -> `examples/.../editbox.cpp:86`);
  the other 2 are, and gui duplicates their work via the `CB_DIR` message. Cross-checking the section surfaced a **new
  pre-scan P1 finding**: `window.cpp:124` binds `qor_pp_useswinapiAW(user32, CreateWindow)` -> `CreateWindowW` MISSING
  (`CreateWindow` is the SDK's macro for `CreateWindowEx`). Library still 38/38 at `/W4`, exit 0, 0 diagnostics.
- Session 7 (30 Oct 2026) reviewed `console.cpp` (1 wrapper - the "Console functions" section `user32.h:689-690`) and
  established that `ConsoleControl` is **the first wrapper in the review with no SDK declaration at all**: the name,
  `CONSOLECONTROL` and the five console-internal structures at `user32.h:19-62` occur in no header of SDK 10.0.26100.0
  or 10.0.10240.0, so the signature rests on the export table (ordinal 1594, hint 55, RVA 00028FD0 - not a forwarder)
  and on the de-facto internal definition, and `desktop.cpp` no longer has a U10 collision to check. Binding correct as
  written (`/EP` -> `"ConsoleControl"`; live probe RESOLVED with `ConsoleControlA/W` MISSING, so bare is the D13
  spelling and no `T` suffix is right; the section is complete - one of one). Fixed: **U13 closed** - `static` added at
  `user32.h:690`, measured with `tmp/_tmp_console_static_probe.cpp` (before 1 x C2352 exit 2, after exit 0), census now
  428 static / zero defined-but-non-static; `,DWORD` -> `, DWORD`; `//Console` -> `//Console functions`; dead
  `#undef WinHelp` at `console.cpp:14` removed; TU comment added. New pre-scan **P4**: four user32 exports are
  forwarders (`DefDlgProcA/W`, `DefWindowProcA/W` -> NTDLL), flagged for `windowprocedure.cpp`'s U6 check, and the
  exports-table grep for forwarders must use "forwarded", not `->`. Recorded as the worst U7 instance so far:
  `Library::Call`'s default return makes a swallowed SEH fault in this wrapper indistinguishable from
  `STATUS_SUCCESS`, and `returncheck.h` has no NTSTATUS-aware check (`>= 0` is success). The wrapper is a U15 instance
  (no caller anywhere; the console surface callers want is kernel32's 135 `*Console*` exports, already wrapped by
  `api_layer/kernel/kconsole.cpp`). Library still 38/38 at `/W4`, exit 0, 0 diagnostics.
- Session 8 (30 Oct 2026) reviewed `coordtransform.cpp` (3 wrappers - "Coordinate Transform functions" `user32.h:377-379`)
  and found the review's worst live defect: **all three wrappers bound `GetDCEx` instead of their own entry point** - one
  binding line copied into three bodies. Each call therefore entered `GetDCEx(HWND, HRGN, DWORD)` with the caller's
  `POINT*` in the region-handle slot and a garbage flags argument, returned an `HDC` truncated to `BOOL`/`int`, never
  transformed a coordinate, and leaked a device context on the paths that succeeded. `GetDCEx` is a real export, the
  module token was right, `cl /EP` produced a valid literal and the TU compiles clean, so **no existing check could see
  it** - the pre-scan and method only ever validate the bound name in isolation. New instrument `tmp/_tmp_wrong_stem_scan.ps1`
  compares each binding's stem with the `User32::` member enclosing it: 428 bindings, 7 flags at the first run, 4 after
  this fix. Fixed the three bindings; `/EP` now yields `"ClientToScreen"`/`"ScreenToClient"`/`"MapWindowPoints"`, all three
  RESOLVED live with `*A`/`*W` MISSING, so the bare stem is the D13 spelling and no `T` suffix is due. **First reviewed TU
  that is not a U15 instance**: the three wrappers are called at `windowcontroller.cpp:643,:648,:653` and reachable from
  `WindowImpl::GetPos` (`windowimpl.cpp:1657`) and `WindowImpl::SetCursorPosWin32` (`:2218`), which is why the defect
  mattered. New pre-scan **P5** (the defect class + the instrument) and a new P1 instance found with it - `mouse.cpp:82`
  `qor_pp_useswinapiAW(user32, LPTRACKMOUSEEVENT)` asks for `LPTRACKMOUSEEVENTW` (MISSING, and so is `TrackMouseEventW`,
  while bare `TrackMouseEvent` is RESOLVED), so that one needs both the stem and the AW-ness fixed in its own session.
  New open item **U18**: `user32.h:188,:191` `LogicalToPhysicalPoint`/`PhysicalToLogicalPoint` are declared and defined
  `void` (`window.cpp:432-437`, `:453-458`) where the SDK returns `BOOL` (`winuser.h:9522-9535`), and both callers in
  `windowcontroller.cpp` are `void` too, so the failure channel is destroyed all the way up - a published-surface decision
  for `window.cpp`'s session. U16 tally: this TU has **0** version-guarded declarations (cleanest yet). No header change,
  no rename, no U14 churn (3/3 explicit template args, 3/3 spaced, file-consistent); library still 38/38 at `/W4`, exit 0.
- Session 8 also opened **U19** (user decision: track api-layer bypass sites and count them per session): call sites that
  reach a `user32.dll` entry point through the SDK import thunk instead of through `User32::`. New instrument
  `tmp/_tmp_bypass_scan.ps1` classifies each occurrence LAYER / INDIRECT / BYPASS after stripping comments. Baseline: the
  coordinate-transform family is clean (3 LAYER, 4 INDIRECT, **0 BYPASS**), while the clipboard family has **9 BYPASS**
  sites, all in `ui/gl/platform.cpp:143-202`. The session also exposed a **method hazard that invalidated two earlier
  records**: raw greps are not evidence of liveness because big parts of the Windows `ui` tree are commented out -
  `ui/gl/windowimpl.cpp` is 1503 comment-only lines of 2637 (57%), and C comments do not nest, so a block opened at
  `:2221` runs to `:2498`. Eight "bypass" hits there (including the `windowimpl.cpp:2395-2410` clipboard sites cited in
  the session-5 clipboard record and in the U15 clipboard entry) are dead code; **both records corrected**, and
  `tmp/_tmp_comment_state.ps1` (character-level comment/string state machine) is now part of the method, as step 5b.
- Session 9 (30 Oct 2026) reviewed `cursor.cpp` (16 wrappers, the "Cursor functions" section): **0 binding / correctness /
  UB / convention defects** - the largest clean TU of the review, and only a TU comment was added. It opened **U20** from
  that TU's call sites: the caller-side `Cursor`/`Icon` ownership model is wrong (`ui/gui/gdiobjects/cursor.h:30` defaults
  `own = true`; `ButtonController` omits the flag for the four `BM_GETIMAGE`/`BM_SETIMAGE` cursor/icon refs at
  `buttoncontroller.cpp:77,82,87,92` while `StaticController` passes `false` for the same situation; `~Cursor()` destroys
  the handle but never clears `m_handle`, so `~PrimitiveHandle` raises a spurious leak warning on every destruction).
  Recorded, not fixed - it is `winqor_gui` surface.
- Session 10 (30 Oct 2026) reviewed `desktop.cpp` (18 wrappers, the "Window Station and Desktop functions" section
  `user32.h:399-417`): **0 binding / correctness / UB defects** - 9 A/W families bound `qor_pp_useswinapiAW`, 9 single
  exports bound bare, all 18 verified by `cl /EP`, the export table and a live `GetProcAddress` probe that also gets
  non-null handles back from `GetProcessWindowStation`/`GetThreadDesktop`. Four consistency defects fixed: `OpenWindowStationT`'s
  parameter `LPTSTR` -> `LPCTSTR` in the declaration (`user32.h:412`) and definition together, `CreateDesktopT`'s definition
  parameter name, and both of the TU's U14(b) deducing sites (`GetThreadDesktop`, `OpenWindowStationT`) - plus a 28-line TU
  comment with the `HDESK`/`HWINSTA` release contract. Consequences: U14(b) is now **6 deducing sites** (was 8) and U15
  gained its largest block, **18/18 desktop wrappers with no in-tree caller and no bypass site**. Two larger items were
  deferred as tasks instead of being fixed: **T1** (`GetUserObjectSecurity`/`SetUserObjectSecurity` declared at
  `user32.h:658-659` and defined nowhere) and **T2** (the dead Window Station / Desktop surface and the naming questions it
  makes untestable). Verified at exit: `desktop.cpp` and the whole library 38/38 at `/W4` exit 0.
- Session 11 (30 Oct 2026) reviewed `devicecontext.cpp` (8 wrappers, the `user32.h:325 //DC functions` section):
  **0 binding / correctness / UB / convention defects.** All 8 pair field-for-field with `winuser.h` and all 8 resolve -
  5 A/W families (`ChangeDisplaySettings`, `ChangeDisplaySettingsEx`, `EnumDisplayDevices`, `EnumDisplaySettings`,
  `EnumDisplaySettingsEx`) bound `qor_pp_useswinapiAW`, 3 single exports bare (`GetDC` 143, `GetDCEx` 144,
  `ReleaseDC` 2FF); `cl /EP` (`tmp/_tmp_pp_dc.bat` -> `tmp/_tmp_dc_pp.txt:453278-453327`) resolves all 8 to the `*W`/bare
  names. The section's distinctive trap is that it mixes **two opposite zero-return conventions** -
  `DISP_CHANGE_SUCCESSFUL == 0` for the two `ChangeDisplaySettings*` wrappers vs `BOOL`/`int` where 0 is failure for the
  other six - so no blanket return check could be applied (U1/U2 instances recorded, not fixed). Fixed in place:
  `(pFunc, hWnd )` -> `(pFunc, hWnd)` and a 36-line TU comment. Two **new open items**: **U21** (the section is not the
  SDK's DC family - 5 of 8 members are display-configuration calls with no `HDC`, while the SDK's DC neighbourhood
  `winuser.h:8560-8614` is split across this TU and `painting.cpp`, and the folder wraps no `CreateDC`/`DeleteDC`/`SaveDC`/
  `RestoreDC`/`GetDeviceCaps` at all) and **U22** (the live `WindowController::GetADC` -> `PerThread::DeviceContextFromHandle`
  -> `MustRelease` -> `~DeviceContext` path calls `User32::ReleaseDC(NULL, hdc)` because `m_hWnd` is never set on that route -
  a DC leak, reachable from the built `examples/.../mainwindow`, recorded not fixed). New U16 sub-case: the display block is
  inside `#ifdef _WINGDI_`/`#ifndef NOGDI`, and the wrappers' own signatures need `wingdi.h` types, so `NOGDI` would make
  `user32.h:326-330` ill-formed rather than merely lose a prototype. **Two review-instrument bugs fixed** in
  `tmp/_tmp_bypass_scan.ps1`: it matched bare stems only (missing every `*T` layer call and every `*W` bypass) and matched
  `::stem(` only at line start - earlier sessions' per-family BYPASS counts are lower bounds and must be re-run; this TU's
  true split is **16 LAYER / 12 BYPASS**, with only 4 LAYER sites reachable at run time and all 12 bypasses in the
  `ui/gl` tree (2 of its files are in no source list at all). Full library still 38/38 at `/W4` exit 0
  (`tmp/_tmp_cl_all_s11b.log`). `tmp/_tmp_sigcmp.ps1` was generalised into the reusable decl<->def + `Call<>` checker
  (`-Tu`/`-First`/`-Last`) and reports this section **8/8 `decl==def ok`, 8/8 `Call ok`** - its first run's "NO DEFINITION"
  for all 8 was an instrument artefact of a `$Src` parameter aliasing the `$src` file array (PowerShell names are
  case-insensitive), not a finding. U15 also gained **`ChangeDisplaySettingsT` with 0 call sites anywhere**. Exit pass
  re-measured every `winuser.h` range cited in the new comment (`tmp/_tmp_aw_spans.ps1` gives the exact A/W family spans)
  and corrected three off-by-one/rounded citations in the TU comment, `status.md` and here; also recorded that
  `_tmp_compile_all.ps1` writes its `cl` log to the fixed path `tmp/_tmp_cl_all.log`, so a redirected stdout only holds the
  `exit=`/`loglines=` summary - the 38/38 proof is 38 TU echo lines + 38 fresh `.obj` files in `tmp/obj`.
- Session 12 (30 Oct 2026) reviewed `devicemanagement.cpp` (2 wrappers, `user32.h:681-683` "Device Management functions"):
  **0 binding / correctness / UB / convention defects, and both binding spellings were already right** - `qor_pp_useswinapiAW`
  `/EP`-resolves to `"RegisterDeviceNotificationW"` (2E5), and the bare binding for `UnregisterDeviceNotification` is correct
  because the live probe shows its `A`/`W` spellings **MISSING** (single export 3C6) - the first case where the export table
  alone could not decide it. G2 is genuinely live for this stem (bare `"RegisterDeviceNotification"` MISSING, `user32.h:9`
  `#undef`) but inert, since the AW macro token-pastes `W` before the SDK macro is consulted. **Fixed: 2 parameter-name
  divergences** (`notificationFilter` -> `NotificationFilter`, `handle` -> `Handle`) - the new
  `tmp/_tmp_name_divergence_scan.ps1` measured those as the **only 2 in 426 definitions folder-wide**, 0 after - plus a
  52-line TU comment. **Instrument bug fixed:** `_tmp_sigcmp.ps1` compared names with case-insensitive PowerShell `-ne` and
  had printed `decl==def ok` for exactly these sites; now `-cne`, and re-running it changed no earlier verdict
  (`devicecontext` 8/8, `desktop` 18/18, `combobox` 3/3). Counts: U13 0, U14(b) 0, U17 1 of 2 (no rename, settled policy -
  sharpened: the A/W choice is invisible here, the filter is a bare `LPVOID` whose trailing `TCHAR` name field is read per
  the bound half), U15 2/2 dead, U19 0 LAYER / 1 BYPASS (`ui/gl/helperwindow.cpp:85` - live code, struct filled correctly,
  but in no build target, and it never unregisters, so enabling `gl` would leak the registration), U16 2/2 with the hardest
  dependency after `NOGDI` (`HDEVNOTIFY` is typedef'd inside the same `WINVER >= 0x0500` + Desktop-partition guards, so a
  lower `WINVER` makes `user32.h:682-683` ill-formed). **New task T3**: the "Power Management functions" pair
  (`user32.h:686-687`) is **not** `static`, its entry points are real exports (2F1/3CB, RESOLVED by the probe) and **no `.cpp`
  in the folder defines either** - the folder has no file for it. Exit: TU standalone `/W4` exit 0, library **38/38** exit 0 (`tmp/_tmp_cl_all_s12.log`). `status.md`
  bookkeeping corrected too: the queue had **omitted `text.cpp`** (25 names under a 26 count; now 13 done + 25 remaining =
  the 38 sources), and a stray `| 2b |` row was moved back into the session log.
- Session 13 (30 Oct 2026) reviewed `dialog.cpp` (22 wrappers, `user32.h:469-493` "Dialog functions" - the largest
  fully-covered section so far): **1 real run-time failure + 1 convention defect**. The P1 instance `qor_pp_useswinapi(user32,
  GetDlgItemTextT)` at `:89` stringized the *member* name (`/EP` literal `"GetDlgItemTextT"`, probe **MISSING**) - now
  `qor_pp_useswinapiAW(user32, GetDlgItemText)` (`"GetDlgItemTextW"`, **RESOLVED**); latent, not live, because that wrapper has
  no call site anywhere. `MessageBox` -> explicit AW (P2 7->6, **no behaviour change**: the pre-fix literal was already
  `"MessageBoxW"` via `winuser.h:9244`, G1). 3 `Call<...>` rows now spell `::DLGPROC` like their declarations. All 22 bindings
  then confirmed by `/EP` + export table + probe: 13 AW (all `W`) + 9 bare, and the 9 bare are right because the SDK declares
  those nine with a single non-A/W spelling. Two findings that the export table alone cannot show: `IsDialogMessage` **is**
  exported bare (231) but at the same RVA as `IsDialogMessageA`, and `DefDlgProcW/A` (A0/9F) are **forwarders to
  `NTDLL.NtdllDialogWndProc_W/_A`**. **`MessageBoxTimeout` is the review's second SDK-invisible entry point** - zero hits across
  the installed include tree, yet `MessageBoxTimeoutW` (28A) / `A` (289) are exported and RESOLVED while the stem is MISSING, so
  `user32.h:489`'s six parameters have no SDK prototype to check against. decl==def 22/22, 0 name divergences, correctness/UB
  pass clean. **New open item U23**: `MessageBoxIndirectT`'s parameter puts `const` on the pointer instead of the pointee
  (published-signature change - recorded, not fixed). Exit: TU standalone `/W4` exit 0, library 38/38 exit 0
  (`tmp/_tmp_cl_all_s13.log`); `dialog.cpp` +26/-5, `user32.h` untouched.
- Session 14 (31 Oct 2026) reviewed `errorhandling.cpp` (4 wrappers, `user32.h:613-617` "Error Handling functions" - fully
  covered by the TU, so not a U4 block): **0 binding / correctness / UB / convention defects**. All four bindings are bare
  `qor_pp_useswinapi` and all four are provably correct as written: `cl /EP` yields `"FlashWindow"`, `"FlashWindowEx"`,
  `"MessageBeep"`, `"SetLastErrorEx"`, the live probe resolves all four and reports **every `*A`/`*W` spelling MISSING**, and
  none of the four stems is one of the nine `user32.h:9-17` `#undef`s - so this is D13's single-export case and `AW` would
  break all four. `SetLastErrorEx`'s `void` matches the SDK's `VOID` (`winuser.h:13558-13563`), so **U18 gains nothing**.
  Fixed only 2 trailing-space nits at the `CheckReturn` and `voidCall` sites; U14 inner spacing left for the folder-wide
  pass (this TU 2 tight / 2 spaced against 250 / 179 folder-wide - no norm either way). U16: `FlashWindowEx` is
  version-guarded (`winuser.h:4687` `WINVER >= 0x0500`) and `PFLASHWINFO` is typedef'd **inside that same guard**, so
  `WINVER < 0x0500` makes `user32.h:615` ill-formed rather than merely prototype-less; `MessageBeep` needs `NOMB` absent.
  Return-check instance: the `MessageBeep` site is 1 of only 4 `CheckReturn` uses in the library (G6 makes it always live),
  and `FlashWindow` must **not** get one - which is what **new open item U24** records: `WindowController::Flash`
  (`ui/gui/controllers/windowcontroller.cpp:715-718`) publishes `FlashWindow`'s previous-active-state `BOOL` as a `bool`
  "did it work" result (gui-side, recorded not fixed; its only in-tree caller is in the unbuilt `winqor_gl`). U15 3 of 4,
  U19 1 LAYER / 0 BYPASS, 0 parameter-name divergences (a third TU at 0 of 426), 0 U13/U4/U11. Exit: TU standalone `/W4`
  exit 0, library 38/38 exit 0 (`tmp/_tmp_compile_all.ps1`); `errorhandling.cpp` 46 -> 92 lines, `user32.h` untouched.
  **Instrument trap found this session:** `pwsh -NoProfile -File script.ps1 -Stems A,B -Roots x,y` does **not** pass arrays -
  `_tmp_bypass_scan.ps1` then dies at `:12` on the bogus `-Roots` while still printing an all-zero totals line. Use
  `pwsh -NoProfile -Command "& script.ps1 -Stems @('A','B')"` and trust the script's default roots.
- Session 15 (31 Oct 2026) reviewed `filledshapes.cpp` (3 wrappers, `user32.h:608-611` "Filled Shapes functions" - fully
  covered, so not a U4 block): **0 binding / correctness / UB / convention defects**. All three bindings are bare
  `qor_pp_useswinapi` and all three are provably correct as written: `cl /EP` yields `"FillRect"`, `"FrameRect"`,
  `"InvertRect"` (`tmp/_tmp_fs_pp.txt:453304`, `:453311`, `:453318`), the export table gives 110/117/226, and the live probe
  resolves all three with **`*A`/`*W` MISSING** - so `AW` would break them, and this is D13's single-export case even though
  the signatures look A/W-shaped. Return types match the SDK (`int`/`int`/`BOOL` at `winuser.h:9726-9747`) so **U18 gains
  nothing**; **U16 gains nothing either** - the first TU whose guard stack has no version macro at all (Desktop partition
  `winuser.h:9717` inside `#ifndef NOUSER` `:64`), and `RECT` is declared in a *broader* partition (`shared/windef.h:154-160`
  under Application/Games/System), the reverse of the `HDEVNOTIFY`/`FLASHWINFO` sub-case. **New open item U25**: the header's
  `CONST::X*` spelling is the SDK's `CONST` macro glued to a scope qualifier (`minwindef.h:153`), 17 lines `CONST::` vs 7
  `const ::` vs 0 `CONST ::`, which makes `_tmp_sigcmp.ps1` print a **false** `CALL TYPE MISMATCH` on 3/3 sites here (and will
  on `rectangle.cpp`/`painting.cpp`/`text.cpp`); the one real smell inside the family is `user32.h:571`'s `CONST::LPINT`
  (const pointer to mutable ints vs the SDK's `CONST INT *`) and belongs with U23. U15 **3 of 3**: the three wrappers' only
  callers are `DeviceContext::FillRect`/`FrameRect`/`InvertRect` (`ui/gui/gdiobjects/devicecontext.cpp:75`, `:80`, `:85`),
  and those three `DeviceContext` methods have no call site anywhere - while the DC they would use *is* live in the built
  `mainwindow` example, so the chain is compiled, exported and simply never invoked; U19 3 LAYER / 3 INDIRECT / 0 BYPASS.
  Caller-side alias checked: those sites pass `reinterpret_cast<const RECT*>(&rc)` from QOR's hand-mirror `Rect`
  (`ui/gui/view/drawing/rect.h:33-38`) - field-for-field identical to `RECT` on this toolchain, so sound in practice, and the
  third instance of the U23/U15 mirror-struct pattern (gui-side, recorded not fixed). Only edit: a 61-line TU comment
  (`:16-76`) plus one stray blank line removed; `filledshapes.cpp` 38 -> 99 lines, `user32.h` untouched. Exit: TU standalone
  `/W4` exit 0, library 38/38 exit 0, 0 `error C`/`warning C` in `tmp/_tmp_cl_all.log`.
- **Next session (16): `help.cpp`** - 3 wrappers (`:18 SetWindowContextHelpId`, `:25 GetWindowContextHelpId`,
  `:32 WinHelp`) over the 5-declaration **`//Shell functions`** section (`user32.h:641-646` - the file name does not match
  the section), in `add_library` at `user/CMakeLists.txt:11`. Pre-measured: `:642-643`
  `SetMenuContextHelpId`/`GetMenuContextHelpId` are **non-static and defined nowhere in the folder** (U4 + U13 pair, and both
  are real exports `34A`/`17C`); the export table has **`WinHelpA` `3EC` / `WinHelpW` `3ED` and no bare `WinHelp`**, so
  `help.cpp:35`'s `qor_pp_useswinapiAW` is not just correct but mandatory; the four context-help-id prototypes are at
  `winuser.h:9125`, `:9132`, `:9138`, `:9145` under `#if(WINVER >= 0x0400)` (`:9105`) + Desktop (`:9110`) - **4 of 5 are U16
  instances** - while `WinHelpA/W` (`:12372`, `:12380`) sit under `#ifndef NOHELP` (`:12270`) + Desktop with no version
  guard, and `#define WinHelp WinHelpW` at `:12386` is the macro `user32.h:10` undefs. The `#undef WinHelp` at `help.cpp:14`
  is therefore redundant (same nit session 7 removed from `console.cpp:14`): `kernel32.h:12 #include <Windows.h>` runs
  before `user32.h:9-17` and include guards stop it being re-defined afterwards. **U15 expected 3 of 3**: the three
  **LAYER** call sites are `windowcontroller.cpp:785`, `:790`, `:795` inside `WindowController::SetContextHelpId` /
  `GetContextHelpId` / **`Help`** (`:783-796`, declared `windowcontroller.h:284-286`), and those three methods have **no
  call site anywhere** in `src`/`examples`/`test`. All help-family names are already in
  `tmp/_tmp_name_resolve_probe.cpp` (added by session 15) and all five **RESOLVED**; full pre-flags for the session are in
  status.md `:138-178`.
- **D13 adopted (user, 29 Oct 2026, `docs/agent/DECISIONS.md`):** explicit `qor_pp_useswinapiAW` wherever the
  SDK has an A/W family, bare `qor_pp_useswinapi` only for single exports. Apply it in each TU as it is
  reviewed. Folder-wide pre-scan in status.md lists P1 (bindings that cannot resolve at run time - **2 still open**:
  `window.cpp:124`, `mouse.cpp:82`; `combobox.cpp:20,27` fixed in session 6, `dialog.cpp:89` in session 13 and
  `keyboard.cpp:82,243,250` in session 20) and P2 (macro-dependent bindings to convert - **6 still open**: `menu.cpp` x2,
  `message.cpp` x4; `dialog.cpp` `MessageBox` converted in session 13). Next TU in the queue: `mdi.cpp` (session 21).
- **Session-20 follow-up decision (user, 1 Nov 2026):** the 10 broken bindings the session-20 closing scan found in
  `mouse.cpp` (all its wrappers use `qor_pp_useswinapiAW` on stems with no A/W pair) get **no out-of-turn hotfix** -
  they are fixed in `mouse.cpp`'s own session, keeping the one-TU-per-session rule. Deferral is safe because the
  enclosing `qor::platform::win::Mouse` controller has no call site in src/examples/test even though its 9 LAYER call
  sites compile into `winqor_gui` (`tmp/_tmp_mouse_bypass_s20.ps1`).

- Corrections to earlier audit assumptions, all measured (see status.md G1-G6): the SDK A/W macro expands
  before `_ATXT()` stringizes, so most bare stems already bind the W export and the `tmp/_tmp_audit_report.txt`
  section-A "runtime failures" list is largely false positives; the nine stems `user32.h:9-17` `#undef`s are
  the ones whose bare spelling really is missing; module-token case (`User32`) is cosmetic; `user32.dll` is
  not mapped in a bare console process; `qor_pp_parameter_checking`/`qor_pp_yes` are defined nowhere, so
  `CheckReturn<...>::TType` is always the checked type.
- **U1/U2 are decided** (user, 29 Oct 2026, session 3): leave `winqor_api_user` as-is - no `Win32ErrorHandler _;`
  and no `CheckReturn`/`CheckNonZeroHandle` are to be added by a review session; each TU session records its
  instances only, with no behaviour change. Still open: what to do about the 97 non-static `user32.h` declarations
  that no `.cpp` defines (U4) and the two `static` ones broken by `resource.cpp` (U11). `hook.cpp` (U3) is in
  `add_library` since user commit `05d3ca2f`; only its review is outstanding.
