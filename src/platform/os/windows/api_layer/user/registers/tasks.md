# T1-T8 deferred tasks - T2, T4-T6 open, each needing a decision; T1, T3, T7 and T8 are CLOSED

- **T1 - two declared-but-undefined members in the "Authorization functions" section.** CLOSED 8 Nov 2026 (D22).

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
- **T3 - the "Power Management functions" pair is declared but has no owning TU at all.** CLOSED 8 Nov 2026;


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

Note - the 38-TU review queue that these tasks were tracked against closed in full at session 37 (5 Nov 2026), so its queue arithmetic and its per-TU hand-offs are dropped: the folder is now 46 sources with no next review TU, and every hand-off lives in the register that names it.

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

- **T7 - the dead `#include "../returncheck.h"` includes across the folder.** CLOSED 5 Nov 2026 (D16).

- **T8 - execute U18: make every wrapper's published signature match the export it binds, and close the named coverage gap.** CLOSED 7 Nov 2026 (D19 + D20).
