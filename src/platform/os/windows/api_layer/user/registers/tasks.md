# T1-T8 deferred tasks - T2, T6 open, each needing a decision; T1, T3, T4, T5, T7 and T8 are CLOSED

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


- **T4 - the menu half of the context-help-id family is declared and defined nowhere.** CLOSED 8 Nov 2026 (user
  decision: add the two definitions, do not delete the declarations). `user32.h:744` `SetMenuContextHelpId(HMENU, DWORD)`
  and `user32.h:745` `GetMenuContextHelpId(HMENU)` gain `static`, and `help.cpp` gains both definitions ahead of the
  window pair, in the header's order, each bound bare `qor_pp_useswinapi` (single-stem exports 34A / 17C, no A/W sibling
  of any of the four context-help-id names in `tmp/_tmp_user32_exports.txt`, and neither stem is in `user32.h:13-21`'s
  nine `#undef`s, so the bare form is the D13 spelling for both per G1/G2). Both halves of the defect close together:
  U4 declared-but-undefined -> defined, and the U18 non-static shape -> static, matching the three static members the
  section already had. Contract as recorded when the task opened, and honoured by the definitions: the id is a plain
  `DWORD` property of the menu (0 clears it), no handle is acquired, and `GetMenuContextHelpId`'s `DWORD` return makes a
  rejected menu indistinguishable from a menu that legitimately carries no context id - the same reason the window pair
  gets no `CheckReturn` (U1/U2 over the section: 5 call sites, 0 handlers, 0 checks). Measured at the close: `help.cpp`
  standalone at /W4 exit 0, folder 46/46 at /W4 exit 0 clean, `cl /EP` resolves all five binding literals against module
  literal "user32", `_tmp_sigcmp.ps1` over `user32.h:744-748` gives 5/5 decl==def ok and 5/5 Call ok, and the wrong-stem
  scan flags nothing in this TU (its 2 folder-wide hits are the pre-existing `CopyCursorT`/`CopyIcon` and `CreateWindowT`/`CreateWindowEx`
  pairings). Census moves static 538 -> 540 / non-static 95 -> 93 and bindings 539 -> 541 (the `.cpp`-only scan reads
  538 -> 540; the folder-wide count adds the header-inline `wsprintfT` binding at `user32.h:838`); members stay 633 and coverage
  stays **757 of 1,018** unchanged, because the coverage instrument counts a member declaration in `user32.h` and the pair
  were already declared. The shared-header `user32.h` edit was gated as a diff against the standing 114-source all-libraries
  baseline: 114 -> 114 cpp echoes, 165 warnings = 165, 1 `C1083` both sides, exit **2** both sides, set-difference 0 new /
  0 gone. Narrative is the banner at `help.cpp:15-97`; the opening prose survives verbatim in `archive/prescan-help.md`.

Note - the 38-TU review queue that these tasks were tracked against closed in full at session 37 (5 Nov 2026), so its queue arithmetic and its per-TU hand-offs are dropped: the folder is now 46 sources with no next review TU, and every hand-off lives in the register that names it.

- **T5 - the IME/WINNLS block in the unlabelled `user32.h` run is declared and defined nowhere.** CLOSED 8 Nov 2026
  (D23 - the "wrap it and mark it obsolete" option the task named). `ime.cpp` now owns the whole IME/WINNLS block. The
  block's two non-static members `DisableProcessWindowsGhosting` (export BB) and `WINNLSEnableIME` (export 3E6) gain
  `static` and both get definitions bound bare `qor_pp_useswinapi` - both are single-stem exports (`tmp/_tmp_name_resolve_s19b.txt`:
  both RESOLVED, `WINNLSEnableIME`'s `A`/`W` spellings MISSING), so the bare form is the D13 spelling, unlike `SendIMEMessageEx`
  next door which stays `qor_pp_useswinapiAW`. The SDK's two sibling WINNLS entry points that `user32.h` declared nowhere -
  `WINNLSGetIMEHotkey` (export 3E8) and `WINNLSGetEnableStatus` (3E7) - are added as `static` declarations plus bare-bound
  definitions in the header's order. `WINNLSEnableIME` keeps the SDK's obsolete warning as a one-line comment above its
  declaration and **remains on the published interface**; `GetDpiForWindow` stays in `window.cpp`. The block is now `ime.cpp`'s
  five members in header order (DisableProcessWindowsGhosting, SendIMEMessageEx, WINNLSGetIMEHotkey, WINNLSEnableIME,
  WINNLSGetEnableStatus). Both halves of the original defect close together (U4 declared-but-undefined -> defined, U18
  non-static -> static), no ABI is touched, and contracts pass through: `DisableProcessWindowsGhosting` is void/void (fire-and-forget,
  `Library::voidCall`, no failure signal), the three WINNLS members return their SDK types raw with no `CheckReturn` (a hotkey code,
  the enable state, and a BOOL whose fault/legitimate-FALSE are indistinguishable per U7), matching the block's U1/U2 policy.
  Measured at the close: `ime.cpp` standalone `/W4` exit 0 and folder 46/46 `/W4` exit 0 clean; `cl /EP` emits five binding
  literals against module literal "user32" (`"SendIMEMessageExW"`, `"DisableProcessWindowsGhosting"`, `"WINNLSGetIMEHotkey"`,
  `"WINNLSEnableIME"`, `"WINNLSGetEnableStatus"`). The shared-header `user32.h` edit was gated against the 58 external consumers
  that `#include user32.h` outside the folder (the `ui/gui`/`ui/gl` targets): the only diagnostics there are pre-existing missing-include
  `C1083` artifacts on the two commented-out-of-build targets and pre-existing `C4100` unreferenced-parameter warnings in `ui/gui`
  headers, and a scan of that log for the new/changed member names or `user32.h` returns 0 hits. Census moves static 540 -> 544 /
  non-static 93 -> 91 and bindings 540 -> 544 (`tmp/_tmp_t4_census.ps1`); members 633 -> 635 and coverage 757 -> 759 of 1,018
  (`tmp/_tmp_pt_coverage_s27.ps1` - the two new WINNLS members were undeclared before and are real exports, so they add 2; the pair
  were already declared and already covered).

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
