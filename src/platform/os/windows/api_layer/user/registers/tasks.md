# T1-T7 deferred tasks, each needs a decision (verbatim)

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
  - **Resolved 8 Nov 2026 (T1 CLOSED) - user decision: add the two definitions, do not delete the declarations.** They go
    in a NEW `security.cpp` TU (not `desktop.cpp`), each bound bare `qor_pp_useswinapi` (single-stem exports 1DB / 375, no
    A/W sibling), and `user32.h:760-761` gains `static` on both declarations. This closes both halves of the defect for
    the pair: U4 (declared-but-undefined -> now defined) and the U18 non-static shape -> static. Measured: standalone
    `security.cpp` at /W4 exit 0, folder 46/46 at /W4 exit 0 clean, and `cl /EP` resolves exactly the two binding literals
    against module literal "user32". The census moves static 534 -> 536 / non-static 99 -> 97 and bindings 535 -> 537
    (`tmp/_tmp_u18_coverage4.ps1` + `tmp/_tmp_t8_6g_counts.ps1`). Coverage stays **757 of 1,018** unchanged - the coverage
    instrument counts a member declaration in `user32.h`, and the pair were already declared, so they were already covered;
    the shared-header `user32.h` edit was run against the 113-source all-libraries baseline as a diff (113 -> 114 cpp
    echoes, 165 warnings = 165, exit 2 both sides, set-difference 0 new / 0 gone).
- **T2 - the whole Window Station / Desktop surface is dead exported API, and its conventions are therefore untestable
  against a caller.** All 18 `desktop.cpp` wrappers have **zero** in-tree callers (searched `src`, `examples`, `test`
  for `User32::<name>` across all 18 - only the definitions match), and a comment-stripped search for direct SDK calls
  (`OpenWindowStation|OpenDesktop|SwitchDesktop|GetUserObjectInformation(W|A)?\(` excluding `User32::`) finds **zero
  bypass sites** - the single hit is the declaration at `user32.h:416`. So this section is 18/18 U15 instances (U15 is
  CLOSED by user decision 2 Nov 2026 - policy D14 - which does **not** close this task: what makes it a user decision is
  that the conventions below are untestable, not that the surface is unused) and
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
  `winuser.h:9138-9140` and `:9145-9146` (re-measured 2 Nov 2026 with `tmp/_tmp_s22_chelpid.ps1`; session 16 recorded
  `:9135`/`:9142`, which are the lines above each declaration - the window pair they share a block with is `:9125-9127` and
  `:9132-9133`), all four in the same `WINVER >= 0x0400` + Desktop block as the window pair that **is**
  wrapped, so nothing external argues against the wrappers. A caller today is ill-formed (`C2352`: a non-static member
  cannot be qualified through the class name) rather than facing `LNK2019`, which is the only way this pair differs from
  T1/T3. Fix is symmetric: add the two definitions to `help.cpp` (with `static`, matching `:644-645`) or delete
  `user32.h:642-643`. Neither touches an ABI, both move the published surface, so it belongs with the U4/U13 sweep.
  Contract for whoever adds them: the id is a plain `DWORD` property of the menu (0 clears it), no handle is acquired, and
  `GetMenuContextHelpId`'s `DWORD` return makes failure indistinguishable from a legitimate 0 - the same reason the
  window pair gets no `CheckReturn`.

Remaining queue (10 TUs, alphabetical; sessions 1-27 = the 24 listed above plus `monitor`, `mouse`, `painting` and
`processthread` done): `winqor_api_user` (module glue), rectangle, resource, scrollbar, systemshutdown, text, window,
windowclass, windowprocedure, windowproperties. `rectangle.cpp` is next (session 28; 93 lines) and **1,144** lines remain
in the queue, **615 of them in `window.cpp` alone** - measure its pre-flags before deciding whether it fits one session,
never mid-session. What is left of the binding census is **one site, `window.cpp:124 CreateWindow`**: `mouse.cpp`'s ten
went in session 25 (2 Nov 2026), and the user's session-20 ruling that they get **no out-of-turn hotfix** was honoured -
they were fixed in `mouse.cpp`'s own session, where the pre-scan predicted them. Hand-offs the queue must not lose:
`window.cpp` (33) takes the version-guarded MDI members `CascadeWindows`/`TileWindows` (**U16**), the 17 DPI / `*Info` /
WinEvent-hook heads that share the monitor SDK region (`tmp/_tmp_mon_region_s24.txt`), U14's last 3 deducing sites and
P3's `CreateWindowT`; **T6** carries the ownerless list-box section.



- **T5 - the IME/WINNLS pair in the unlabelled `user32.h:635-639` block is declared and defined nowhere.** Opened
  1 Nov 2026 (session 19, `ime.cpp`). `user32.h:635` `void DisableProcessWindowsGhosting(void)` and `user32.h:637`
  `BOOL WINNLSEnableIME(HWND hwnd, BOOL bFlag)` are the block's two non-static members; no `.cpp` in the folder defines
  either, and `dumpbin /linkermember:1` on the built `winqor_api_user.lib` publishes neither. Both entry points are real
  single exports - `WINNLSEnableIME` **RESOLVED** (ordinal 3E6, `A`/`W` MISSING), `DisableProcessWindowsGhosting`
  **RESOLVED** (`tmp/_tmp_name_resolve_s19b.txt`) - and the SDK declares `WINNLSEnableIME` at `um\WinNls32.h:88` in the
  same Desktop block as `WINNLSGetEnableStatus` (`:89`) and `WINNLSGetIMEHotkey` (`:87`), none of which `user32.h` declares.
  Same shape as T1/T3/T4: the fix is symmetric (make the pair `static` + write the TU, or delete `:635`/`:637`), no ABI is
  touched, and both moves change the published surface, so it belongs with the U4/U13 sweep. Contract for whoever wraps
  them: `DisableProcessWindowsGhosting` takes nothing and returns nothing (fire-and-forget, no failure signal);
  `WINNLSEnableIME` returns `BOOL` and the SDK header itself says it is obsolete - `user32.h:638` carries the same
  warning as of session 19 - so the honest options are "wrap it and mark it obsolete" or "do not wrap it at all", not
  silently exposing it through this layer. Note also that `WINNLSEnableIME` is a **bare single export** (no `A`/`W`
  spelling exists), so per D13 it would need the bare `qor_pp_useswinapi`, unlike `SendIMEMessageEx` next door.

- **T6 - the whole "//List Box functions" section is declared and defined nowhere - there is no owning TU.** Opened
  2 Nov 2026 by the session-24 (`monitor.cpp`) U21 region scan. `user32.h:545` opens the section with **three**
  declarations (`:546 DlgDirList`, `:547 DlgDirSelectEx`, `:548 GetListBoxInfo`), **all three non-static**, and **no
  `.cpp` in the folder defines any of them** (measured: 0 `User32::` definitions over all 38 TUs, and there is no
  `listbox.cpp` in the folder at all), so the published class carries three members that fail at link time with
  `LNK2019` - the U4 shape and the T3 shape (a whole section with no owner) at once. `GetListBoxInfo` is one of the 22
  heads in the `#if(WINVER >= 0x0500)` region that `monitor.cpp`'s scan found unwrapped
  (`tmp/_tmp_mon_region_s24.ps1` -> `tmp/_tmp_mon_region_s24.txt`), and it is recorded here rather than only under U21 so
  that the session which eventually acts on it is not chosen by which SDK region happened to be under the microscope.
  All three entry points are real exports: `DlgDirListA`/`W` (ordinals `C1`/`C4`), `DlgDirSelectExA`/`W` (`C7`/`C8`),
  `GetListBoxInfo` (`174`, RVA `0002EFF0`) in `tmp/_tmp_user32_exports.txt`. Two conventions bind whoever wraps them: the
  first two are A/W families **and** two of the nine stems `user32.h:13-14` `#undef`s (G2), so a bare binding is not even
  possible there and D13 requires explicit `qor_pp_useswinapiAW`, while `GetListBoxInfo` is a bare single export, so D13
  requires the bare macro there. Its contract also has no failure signal - it returns the count of client items as
  `DWORD` and 0 on error (U7-shaped: a legitimate 0 and a swallowed SEH fault are indistinguishable). Deciding T6 means
  either writing the TU (a three-wrapper file) or deleting the three declarations; both move the published surface, so
  it belongs with the `U4`/`U13` sweep and the T1/T3/T4/T5 decision set, not with a TU review.


- **T7 - CLOSED 5 Nov 2026 (D16) - the 35 dead `#include "../returncheck.h"` lines are gone; the 2 real consumers keep
  theirs.** Opened 3 Nov 2026 at the
  session-27 (`processthread.cpp`) close-out, while pre-flagging `rectangle.cpp`, and registered as a **deferred task** by
  user decision the same day: the fix is mechanical, only the go-ahead is missing. Measured with the instrument written
  for it, `tmp/_tmp_rcinclude_scan.ps1` (output `tmp/_tmp_rcinclude_s27.txt`): **37 of 38** sources include the header -
  every source except the glue `winqor_api_user.cpp` - and only **2** contain a code hit for any of the 14 names
  `returncheck.h` declares (`:15 CheckReturn`, `:65 AutoErrorCheck`, `:94 BoolCheck`, `:112 HandleCheck`,
  `:124 HandleNullCheck`, `:137 CheckNonZeroHandle`, `:149 SuccessCheck`, `:161 LongSuccessCheck`, `:174 TSuccessCheck`,
  `:187 CheckNonZero`, `:200 CheckFailureInt`, `:213 TCheckFailureValue`, `:232 TRangeCheck`, `:251 TCheckNotLess`):
  `processthread.cpp:85`, `:94`, `:110` and `errorhandling.cpp:81`. `devicecontext.cpp`'s single match (`:29`) is inside a
  comment, which is why the scan skips `//`-leading lines. So **35 includes are dead**, `rectangle.cpp` among them.
  Removing one cannot break a hit-free TU: `library.h:7-8` includes only `<excpt.h>` and `src/qor/flyers/error/error.h`,
  so nothing pulls `returncheck.h` in transitively, and the header is include-guarded (`QOR_PP_H_..._RETURNCHECK`) so a
  needed include stays a single line.
  The comparison that shows this is a folder habit rather than a layer convention: the same scan over
  `api_layer/kernel` (output `tmp/_tmp_rcinclude_kernel_s27.txt`) gives **22 includes, 20 TUs with hits** - there the
  include is load-bearing. Nothing in the settled U1/U2 return-check policy (29 Oct 2026, "no `CheckReturn` in this
  library, sessions record only") is changed by deleting an unused include, but the two are related: if that policy is
  ever revisited, these 35 lines become wanted again, which is why the deletion waits for a decision instead of being
  applied TU by TU as sessions pass through. Whoever executes it: one line per TU, no ABI and no published-surface
  change, gate = the folder compile **38/38 at `/W4`, exit 0, no diagnostics** plus a re-run of
  `tmp/_tmp_rcinclude_scan.ps1` (expect `includes ../returncheck.h: 2`, `include-without-use: 0`).
  **Executed 5 Nov 2026 under D16: 35 files, exactly one line each (`git diff --numstat` = 27 rows of `0 1` plus 8 rows of
  `1 2` / `2 3` - the extra pairs are the comment sentences that used to say the include was still there); the scan re-run
  prints `includes: 2` / `include-without-use: 0`; the folder compiled 38/38, exit 0, 0 diagnostics, every `.obj` newer
  than its source. The scan's include test had to be made line-anchored - un-anchored it also counts the three TUs whose
  banner *quotes* the include text, which prints a false `5 / 3`. The one-line shift's effect on the in-source `:NNN`
  self-citations is measured and left to its own pass in D16.**

- **[history - T7 closed by D16] T7 count moved 35 -> 36 (3 Nov 2026, session 30, `scrollbar.cpp`).** `scrollbar.cpp:11` includes
  `../returncheck.h` and uses none of it - the TU has 0 `CheckReturn` sites, so the include is dead like the 35 already
  listed. Measured with `tmp/_tmp_s30_closeprep.ps1` (prints every `#include` of the TU) against
  `tmp/_tmp_s30_measure2.txt` (0 `CheckReturn`, 0 `Win32ErrorHandler`). The include is **not** removed: with G6 the
  header is the only thing that would make a future check compile, and removing includes is a folder-wide decision
  (same stance as the other 35 sites). Remaining dead includes after this TU: 36 of the 37 that include it; the one real
  consumer is `processthread.cpp` (3 sites) plus `errorhandling.cpp` (1).

- **[history - T7 closed by D16] T7 re-measured 4 Nov 2026 (session 33, `window.cpp`) - unchanged at 37 includes / 35 dead, and the arithmetic in the
  entry above is restated to settle it.** `tmp/_tmp_rcinclude_scan.ps1` run over the folder after this session prints
  `sources scanned: 38`, `includes ../returncheck.h: 37` (all but the glue `winqor_api_user.cpp`), `TUs with a real code
  hit: 2` (`processthread.cpp` x3 at `:85`,`:94`,`:110`; `errorhandling.cpp` x1 at `:81`) and
  `include-without-use: 35`. So the folder figure has been **35 dead since session 30** and the "35 -> 36" line above
  counts one TU twice (`scrollbar.cpp` was already inside the 35 in the head text) - the two numbers describe the same
  37-include population. `window.cpp:11` is in that population (0 `Win32ErrorHandler`, 0 `CheckReturn` in its 85 bodies,
  `tmp/_tmp_s33_verify.ps1`) and was **not** removed: the standing policy is that the deletion waits for one folder-wide
  go-ahead, not 38 per-TU passes. Gate for whoever executes it stays the same (38/38 at `/W4` + re-run of the scan,
  expect `includes: 2`, `include-without-use: 0`).


- **T8 - execute U18: make every wrapper's published signature match the export it binds, and close the named coverage
  gap.** Opened 5 Nov 2026 by the folder's `purpose.md` (policy **D18**, `docs/agent/DECISIONS.md`); the measured site
  list is the D18 block at the foot of `registers/u18.md`. Tranches, each its own commit, each gated by the `status.md`
  gate (edited TU standalone at `/W4`, then the folder 38/38):
  0. ~~Answer three questions first~~ **Mostly settled 5 Nov 2026 by the user's scope decision (D18 addendum,
     `docs/agent/DECISIONS.md`): the "full set" is the exports `winuser.h` declares, and the superseded families - the DDE
     family and `keybd_event`/`mouse_event` - are explicitly excluded.** The 253 undocumented exports are therefore out of
     scope. One sub-question is still open and blocks only tranche 6's arithmetic, not steps 1-5: does every A/W pair get
     two members or a `W` member plus the `T` alias (**U17**) - after the exclusions exactly two pairs remain in the
     backlog (`ChangeMenuA/W`, `wsprintfA/W`), so it moves the count by one wrapper.
  1. **Published-type fixes, one TU per commit.** `keyboard.cpp`: `GetKeyboardLayoutList` `UINT` -> `int`
     (`winuser.h:1313`) at `user32.h:79` + `keyboard.cpp:133` + Call list `:137`; `keybd_event` `void*` -> `ULONG_PTR`
     (`:6071`) at `user32.h:87` + `:189` + `voidCall` list `:193`. `menu.cpp`: `InsertMenuT`/`ModifyMenuT` `void*` ->
     `UINT_PTR` (`:7658`, `:7667`, `:7704`, `:7713`) at `user32.h:239`, `:245` + `menu.cpp:249`, `:291` + Call lists
     `:253`, `:295`; `TrackPopupMenu`'s last slot `HWND` -> `CONST RECT *` (`:7759`) at `user32.h:252` + `menu.cpp:340` +
     Call list `:344`. Header, definition and `Call<>` list must change in the same edit - a header/Call-list mismatch is
     the U14 class of defect, not a style nit.
  2. **Consumer probe per TU, before the header change lands.** Compile the callers against the edited header the way
     session 33 did (`_tmp_win_consumer_s33.bat` in `tooling.md`): the live sites are
     `ui/gui/controllers/i_m/menu.cpp:173`, `:183` (they pass an item id into the `UINT_PTR` slot, so the caller side needs
     a `UINT_PTR` parameter or a cast), `windowcontroller.cpp:313-315`, `:328-330` and
     `scrollbarcontroller.cpp:50-52`, `:57`, `:72`. Fix the `winqor_gui` side in a separate commit; `TrackPopupMenu` has
     no caller, so it is latent.
  3. **Decide `TrackPopupMenu`'s reserved slot rather than retype it blindly**: the SDK marks it `_Reserved_ CONST RECT *`
     (it must be NULL), and the layer spells it `HWND`, which under `STRICT` can only ever carry NULL. Retype it, or drop
     the parameter and pass NULL - do not leave the `HWND` slot as it is.
  4. ~~**Parameter-name alignment in one folder-wide commit**, not TU by TU~~ **Done 6 Nov 2026.** The "16 recorded
     sites" was wrong: the comment-stripped, case-sensitive decl-vs-`winuser.h` instrument (`tmp/_tmp_t8_sigcmp.ps1`,
     registered in `gotchas/instrument-flags.md`) measured **102 divergent parameter names over 54 members**, and
     `tmp/_tmp_t8_rename.ps1` renamed **101 of them across 15 files** - header declaration and definition together,
     because renaming one side alone breaks the build. `InSendMessageEx` is the one row not applied: the layer declares
     that parameter as bare `void`, so there is no name to change - a type divergence, recorded under U18, not a naming
     one. Post-pass: 0 decl-vs-SDK name divergence over the 421 SDK-matched members (`tmp/_tmp_t8_sigcmp_post4.txt`),
     0 decl-vs-definition divergence folder-wide (`tmp/_tmp_t8_decldef_post.txt`), layer gate 38/38
     (`tmp/_tmp_t8_layer_gate_post.txt`), consumer gate 4/4 (`tmp/_tmp_t8_consumers5.txt`). Six members carry
     parameters the SDK declares **without** a name (`GetMenuInfo`, `SetMenuInfo`, `Get/SetWindowContextHelpId`,
     `SetDoubleClickTime`, `RegisterClassExT`) - nothing to align to, left as they are. `CopyCursorT` is excluded from
     the instrument because `CopyCursor` is a function-like macro (`winuser.h:10681`), so any prototype the parser
     finds is an inactive `#ifdef` arm; the member binds `CopyIconW`, so it was aligned by hand to that prototype's
     `hIcon` (`cursor.cpp:60`, `user32.h:385`). The rename pass is also what exposed that T8 step 2's consumer fix had
     never landed: with `InsertMenuT`/`ModifyMenuT` taking `UINT_PTR`, `ui/gui/controllers/i_m/menu.cpp:173`, `:183`
     failed with C2664. Fixed there with `reinterpret_cast<UINT_PTR>(idNewItem)` at both call sites; the GUI header
     cannot carry `UINT_PTR` (it is parsed before the Windows headers), so the SDK-shaped parameter type on
     `Menu::InsertSubMenu`/`Menu::Modify` stays open.
  5. **Add the coordinate-transform pair U18 was opened over**: `LogicalToPhysicalPointForPerMonitorDPI` and
     `PhysicalToLogicalPointForPerMonitorDPI` (`BOOL (HWND, LPPOINT)`, single exports 26E / 2AC, no A/W family and no
     stem -> stemW macro, so bare `qor_pp_useswinapi` is correct under D13). Declare them beside `user32.h:188`/`:191`
     and define them in `window.cpp` beside the pair session 33 fixed (`:504`, `:527`) - the section question (U12) is
     resolved by placing them with their siblings. The `#if(WINVER >= 0x0603)` guard around the SDK prototypes is live
     (`sdkddkver.h:251` defaults `WINVER` to `0x0A00`; nothing in the build sets it). Re-run `_tmp_wrong_stem_scan.ps1`
     and the `/EP` binding-literal check after this step.
  6. **The backlog: 109 names, dispositioned in `registers/u18-coverage.md`** (measured 5 Nov 2026 with
     `_tmp_u18_coverage4.ps1` + `_tmp_u18_buckets.ps1`; the 117-name figure first quoted for D18 is a raw bucket count that
     the scope decision reduces). Work it **one family per session** in the tranche order that file gives, applying the
     D18 rule from the first line: the wrapper's signature is the `winuser.h` prototype, and the disposition list is
     edited as each family lands (Pointer 32, DPI/awareness 21, Touch/gesture 14, Display config 7, Shutdown/power 3,
     Misc 26, held-open legacy 6 (decided in scope by D19 - tranche 6g) - those sum to the 109). Seven names are already dispositioned **excluded** (superseded
     twins whose `*Ptr`/`Ex` replacement the library wraps: `Get/SetClassLongA/W`, `SetWindowsHookA/W`,
     `UnhookWindowsHook`) and one is an instrument artifact (`SetWindowsHookExAW`, no prototype).
     See that file for the per-name list before starting a tranche. **6a (Pointer, 32) DONE 6 Nov 2026** - new TU
      `pointer.cpp`, so the folder is 39 sources / 559 members / 462 bindings. **6b (DPI / awareness, 21) DONE 6 Nov
      2026** - new TU `dpi.cpp`, so the folder is **40 sources / 580 members / 483 bindings** and the backlog re-measures at
      **54** (14 Touch/gesture + 7 Display config + 3 Shutdown/power + 24 Misc + 6 held-open legacy). **6c (Touch /
      gesture, 14) DONE 6 Nov 2026** - new TU `touch.cpp`, so the folder is **41 sources / 594 members / 497 bindings**
      and the backlog re-measures at **40** (7 Display config + 3 Shutdown/power + 24 Misc + 6 held-open legacy). **6d
      (Display configuration, 7) DONE 6 Nov 2026** - new TU `displayconfig.cpp`, so the folder is **42 sources / 601
      members / 504 bindings** and the backlog re-measures at **33** (3 Shutdown/power + 24 Misc + 6 held-open legacy).
      **6e (Shutdown / power, 3) DONE 7 Nov 2026** - new TU `powermanagement.cpp` plus `CancelShutdown` into `systemshutdown.cpp`,
      so the folder is **43 sources / 604 members / 507 bindings** and the backlog re-measures at **38** raw = **30** net (24 Misc
      + 6 held-open legacy). **6f (Misc, 24 names / 23 members) DONE 7 Nov 2026** - new TU `misc.cpp`, so the folder is **44
      sources / 627 members / 530 bindings** and the backlog re-measures at **14** raw = **6** net (the held-open legacy names;
      7 superseded twins and 1 instrument artifact carry written exclusions). **D19 (7 Nov 2026) answers that user question:
      wrap all 6**, so the 6 held-open legacy names are **in scope** and the target stays **109** (the 103 branch is not taken); the final
      tranche is **6g**, and U18 closes only when 6g wraps them. The decision touches no source/header/CMake - wrapping is 6g's
      job: a new `legacy.cpp` TU + `user32.h` declarations + bindings, gated like 6a-6f (wsprintfA/W need a variadic-wrapper
      shape the folder does not yet have). **6g (held-open legacy, 6 names / 5 members) DONE 7 Nov 2026** - new TU
      `legacy.cpp` (4 definitions) plus the section at `user32.h:821-845`, so the folder is **45 sources / 632 members /
      535 bindings** and the backlog re-measures at **8 raw = 0 net** (7 superseded twins + the `SetWindowsHookExAW`
      artifact, all with written exclusions; coverage 751 -> **757 of 1,018**). `wsprintfA/W` was the hard half and is
      decided by **D20** (`docs/agent/DECISIONS.md`): `library.h` gains `varCall` - variant (V), a `__cdecl` variadic
      `fPtr` (`minwindef.h:129` spells `WINAPIV` as `__cdecl`) forwarding with `std::forward` - and `wsprintfT` is the
      folder's one **header-inline** member, because a member template is instantiated only where its body is visible
      (measured: out-of-line LNK2019/LNK1120, header-inline /W4 exit 0 and 0/3/6 through a 0-, 1- and 2-argument tail).
      Gate: `legacy.cpp` standalone `/W4` exit 0, folder 45/45, all-libraries 112 -> 113 echoes at exit **2** both sides
      with 0 new / 0 gone diagnostics, and `cl /EP` yields 5 binding literals (the four here plus `wsprintfW` from the
      header-inline body). **Steps 1-6a-6g are all in: U18 and T8 are CLOSED 7 Nov 2026** - every one of the 109 backlog
      names is wrapped or carries a written exclusion, so the close condition below is met rather than waited on.
  ~~U18 closes when steps 1-5 are in and every one of the 109 backlog names is either wrapped or carries a written
  exclusion. Do not mark it closed on any partial pass.~~ **Met 7 Nov 2026 at the 6g close - U18 and T8 closed.**

