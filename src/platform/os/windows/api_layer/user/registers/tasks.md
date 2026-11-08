# T1-T8 deferred tasks - all eight CLOSED; T2 closed 8 Nov 2026 (D25)

- **T1 - two declared-but-undefined members in the "Authorization functions" section.** CLOSED 8 Nov 2026 (D22).

- **T2 - the whole Window Station / Desktop surface is dead exported API, and its conventions are therefore untestable
  against a caller.** **CLOSED 8 Nov 2026 (D25)** - the resolution is the paragraph at the end of this bullet; the
  description below stays as written. All 18 `desktop.cpp` wrappers have **zero** in-tree callers (searched `src`, `examples`, `test`
  for `User32::<name>` across all 18 - only the definitions match), and a comment-stripped search for direct SDK calls
  (`OpenWindowStation|OpenDesktop|SwitchDesktop|GetUserObjectInformation(W|A)?\(` excluding `User32::`) finds **zero
  bypass sites** - the single hit is the declaration at `user32.h:416`. So this section is 18/18 U15 instances (U15 is
  CLOSED by user decision 2 Nov 2026 - policy D14 - which does **not** close this task: what makes it a user decision is
  that the conventions below are untestable, not that the surface is unused) and
  0/18 U19 sites, which makes several folder decisions (U17 `T`-suffix consistency, P3, and whether `EnumDesktopsT`'s
  callback should be typed as the SDK's `DESKTOPENUMPROC` or as a `LPCWSTR`-taking callback now that the binding is
  always the `W` form) pure taste calls with no call site to inform them. Recorded for the folder-wide pass; nothing in
  this section is exercised by `examples/mainwindow`, so no session can observe a behaviour change here.
  **Resolution (8 Nov 2026, D25):** the callback question is settled in favour of the SDK's **W** spellings. `EnumDesktopsT`
and `EnumWindowStationsT` now take `DESKTOPENUMPROCW` / `WINSTAENUMPROCW` - at `user32.h:410` and `:413`, at the `desktop.cpp:86`
and `:107` definition heads, and in the two `Library::Call<>` argument lists at `desktop.cpp:90` and `:111` - because both
wrappers bind `qor_pp_useswinapiAW` and the prototype parameters of the exports they bind are spelled that way (`WinUser.h:1480`,
`:1610`), which is what D18's "copy the prototype of the export it binds" asks for; the generic names reach the same shape only
through the `#ifdef UNICODE` alias at `WinUser.h:202`/`:201`, whose `#else` arm swaps in the A shape. The edit moves no type and
no mangled name - `tmp/_tmp_t2_probe_id.cpp` compiles at exit 0 behind four `static_assert`s (`is_same<DESKTOPENUMPROC,DESKTOPENUMPROCW>`,
`is_same<WINSTAENUMPROC,WINSTAENUMPROCW>`, both equal to `BOOL(__stdcall*)(LPWSTR,LPARAM)`), and it is not a move to an untyped
parameter: `tmp/_tmp_t2_probe_neg.cpp` exits 2 (an A-shaped callback is still rejected) and `tmp/_tmp_t2_probe_farproc.cpp` exits
2 (none of the four names is `FARPROC` in this `STRICT`/`UNICODE` build). The other two conventions T2 named take no action here:
U17 measured 9/9 compliant in this section (`EnumDesktopWindows` correctly bare) and P3 found no mirror case, so both stay where
they are until the folder-wide pass. The deadness stays D14's territory - re-measured 8 Nov 2026, 0 `User32::<name>` hits for all
18 names across `src`/`examples`/`test` outside `desktop.cpp` and `user32.h`. Gate: `desktop.cpp` standalone `/W4` exit 0, folder
47/47 exit 0 with 0 diagnostics, and the 115-source all-libraries logs SHA-256 identical pre/post (`D286F848...D6EAF`).

- **T3 - the "Power Management functions" pair is declared but has no owning TU at all.** CLOSED 8 Nov 2026.

- **T4 - the menu half of the context-help-id family is declared and defined nowhere.** CLOSED 8 Nov 2026.

Note - the 38-TU review queue that these tasks were tracked against closed in full at session 37 (5 Nov 2026), so its queue arithmetic and its per-TU hand-offs are dropped: the folder is now 46 sources with no next review TU, and every hand-off lives in the register that names it.

- **T5 - the IME/WINNLS block in the unlabelled `user32.h` run is declared and defined nowhere.** CLOSED 8 Nov 2026 (D23).

- **T6 - the whole "//List Box functions" section is declared and defined nowhere - there is no owning TU.** CLOSED
  8 Nov 2026 (D24), opened 2 Nov 2026 by the session-24 (`monitor.cpp`) U21 region scan. `user32.h:545` opens the section with **three**
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
  **Resolution (8 Nov 2026, D24):** the TU was written. A new `listbox.cpp` defines all three - `DlgDirList` and
  `DlgDirSelectEx` bound `qor_pp_useswinapiAW` (their bare stems are not exported and `user32.h:17-18` `#undef`s both, so
  a bare binding is impossible there) and `GetListBoxInfo` bound bare `qor_pp_useswinapi` - and `user32.h:552-554` gains
  `static` on all three, with the `DlgDirSelectEx` declaration moved to the SDK's parameter names `hwndDlg`/`chCount`/`idListBox`
  per D18. U4's and U18's instances for the trio are gone; U17 takes no action (2 of the 11 A/W members that carry no `T`
  suffix, deferred until one folder-wide pass); coverage does not move, because all three were already declared and already
  counted covered. The measured shape (bindings, prototypes and guards, the U7 no-failure-signal contract, the gate) is in
  D24 and in the `listbox.cpp` banner.

- **T7 - the dead `#include "../returncheck.h"` includes across the folder.** CLOSED 5 Nov 2026 (D16).

- **T8 - execute U18: make every wrapper's published signature match the export it binds, and close the named coverage gap.** CLOSED 7 Nov 2026 (D19 + D20).
