# consumed pre-scan notes for help.cpp (session 16)

## Consumed pre-scan notes for `help.cpp` (session 16) - moved here verbatim from status.md on 31 Oct 2026
superseded in full by the session-16 row of the status.md session-log table, open item U26 and task T4.

Measured pre-flags for `help.cpp` (session 16) - from this session's scans, so they are facts, not guesses:
- **Expect 0 P1 and 0 D13 conversions, and one binding that is already AW and must stay AW.** The export table has
  **`WinHelpA` 3EC / `WinHelpW` 3ED only - there is no bare `WinHelp`** (live probe re-run 31 Oct: `WinHelp` **MISSING**,
  `WinHelpA`/`WinHelpW` **RESOLVED**), so `help.cpp:35`'s `qor_pp_useswinapiAW(user32, WinHelp)` is mandatory, and
  `#define WinHelp WinHelpW` at `winuser.h:12386` is exactly the macro `user32.h:10` exists to kill. The two
  context-help-id wrappers at `:19`/`:28` are bare, and the four help-id names are genuine single exports
  (`SetWindowContextHelpId` 37A, `GetWindowContextHelpId` 1E1, `SetMenuContextHelpId` 34A, `GetMenuContextHelpId` 17C -
  no `A`/`W` spelling of any of them appears in `tmp/_tmp_user32_exports.txt`), all four **RESOLVED** by the probe run
  this session. All five names are already in `tmp/_tmp_name_resolve_probe.cpp` (`WinHelp` and `WinHelpW` from an earlier
  session at `:51-52`, the rest added by session 15), so no probe edit is needed to start. Run `cl /EP`, the probe and
  `_tmp_wrong_stem_scan.ps1` as usual.
- **The section is `//Shell functions` (`user32.h:641`), not "Help functions"** - the file name does not match the section
  it implements, and the section holds menu-help entries too: a U12/U21-shaped placement note.
- **2 U4 + likely 1 U13**: `user32.h:642-643` `SetMenuContextHelpId`/`GetMenuContextHelpId` are **non-static and defined
  nowhere in the folder**, while `:644-646` are `static` and defined. Both undeclared-off members are real exports, so the
  pair is a published-surface decision (add or delete), not a TU fix - collect it under U4/U13 like the rest.
- **U16 gains up to 4 of 5**: the context-help-id prototypes are `winuser.h:9125`, `:9132`, `:9138`, `:9145` under
  `#if(WINVER >= 0x0400)` (`:9105`) + Desktop (`:9110`) inside `#ifndef NOUSER` (`:64`); `WinHelpA`/`WinHelpW` are
  `:12372`/`:12380` under `#ifndef NOHELP` (`:12270`) + Desktop (`:12268`) with **no version guard**, so `WinHelp` adds 0
  and the other four add 1 each. Measured with `tmp/_tmp_sdk_guard_scan.ps1` this session (run over
  `WinHelpA,SetWindowContextHelpId,GetWindowContextHelpId,SetMenuContextHelpId`).
- **A dead `#undef WinHelp` at `help.cpp:14`** - `user32.h:10` already undefs the stem before this TU's own
  `#include "user32.h"`, and nothing after it can re-process `winuser.h` past its `_WINUSER_` guard, so the line is inert.
  Session 7 removed the identical line from `console.cpp:14` for the same reason (status.md `:1363-1364`) - same fix here.
- **Expect 3 of 3 U15, but re-measure with the instrument before writing it.** The three **LAYER** call sites are
  `ui/gui/controllers/windowcontroller.cpp:785`, `:790`, `:795` inside `WindowController::SetContextHelpId`
  (`:783-786`, declared `windowcontroller.h:284`), `GetContextHelpId` (`:788-791`, `:285`) and `Help` (`:793-796`, `:286`)
  - note the third method is **`Help`, not `WinHelp`**, and it folds the `BOOL` into `bool` with `? true : false`. All
  three methods are built (`gui/CMakeLists.txt:27`) and have **no call site anywhere** in `src`/`examples`/`test`
  (measured 31 Oct by scanning those roots for the three method names - only the declarations and definitions match), so
  the chain is compiled, exported and never executed one level up, exactly like most U15 instances. Run
  `_tmp_bypass_scan.ps1` over the three stems + `*A`/`*W` variants in the session to get the U19 split. The two menu-help
  exports have no wrapper, so no call site can exist for them.
- **U14 clean on inspection**: `user32.h:642-646` all five declarations spell explicit parameter types with `, ` spacing;
  `help.cpp:22`, `:29`, `:36` pass `HWND`/`DWORD`/`LPCTSTR`/`UINT`/`ULONG_PTR` explicitly. Note `WinHelp`'s `LPCTSTR`
  parameter is `_In_opt_` in the SDK (`winuser.h:12374`, `:12382`), and `hWndMain` is `_In_opt_` too - worth a contracts
  sentence, and `windowcontroller.cpp:795` passes `help.c_str()` so it never exercises the optional case. Two caller-side
  `BOOL` conversions to check there: `:785` returns the wrapper's `BOOL` from a `bool` method **implicitly**, while `:795`
  does it explicitly with `? true : false` - a consistency nit on the gui side, not in this layer.
- The name-divergence scan should report **0** again: session 12 took the folder to zero (2 of 426 definitions, both in
  `devicemanagement.cpp`) and sessions 13, 14 and 15 re-confirmed 0.
Take the pre-scan P1/P2/P3/P5 entries into account only when their owning TU comes up; do not fix other TUs outside
their own session.

### Session 16's confirmation/correction pass over those pre-flags (as recorded in status.md before consolidation)

Measured pre-flags for `help.cpp` (session 16) - kept as the record of what was measured; every bullet below was
confirmed or corrected by session 16 (its record is now the session-log table row 16 in `status.md`, item **U26** and
task **T4**):

- **The five-declaration asymmetry and the `AW` mandate are confirmed by this session.** `user32.h:642-643`
  `SetMenuContextHelpId`/`GetMenuContextHelpId` are **non-static and defined nowhere in the folder** (a U4 pair; both real
  exports, `17C`/`34A`, both already in `tmp/_tmp_name_resolve_probe.cpp` from session 15). `WinHelp` is the folder's
  second-confirmed **AW** binding (`qor_pp_useswinapiAW(user32, WinHelp)`; `WinHelp` MISSING / `WinHelpW` RESOLVED per the
  session-1 probe note at line 306) and the only stem here that is in the `user32.h:9-17` `#undef` list, so the `#undef` at
  `help.cpp:14` is dead by construction: `user32.h` includes only `kernel32.h` at `:7` and does the nine `#undef`s at
  `:9-17`, and no later include can re-process `winuser.h` past its own guard. Callers traced:
  `windowcontroller.cpp:785`, `:790`, `:795`. In `add_library` (`user/CMakeLists.txt:11`).
- **Pre-measured exports and guards, all confirmed with one citation correction.** All five names are real exports, and
  the export table has **`WinHelpA`/`WinHelpW` only** - no bare `WinHelp`, which is why the `AW` form is mandatory. The
  four context-help-id prototypes are at `winuser.h:9122-9127`, `:9129-9133`, `:9135-9140`, `:9142-9146` (the earlier
  `:9125`/`:9132`/`:9138`/`:9145` citations are the name lines of the same four blocks) under `#if(WINVER >= 0x0400)`
  (`:9105`) + Desktop (`:9110`) inside the `#ifndef NOUSER` region; `WinHelpA`/`WinHelpW` are at `:12369-12376` /
  `:12377-12384` under `#ifndef NOHELP` (`:12270`) + Desktop (`:12268`) with **no version guard**, and the
  `#ifdef UNICODE` / `#define WinHelp WinHelpW` at `:12385-12386` (`#else` `WinHelpA` at `:12388`) is the macro
  `user32.h:10` exists to kill. So **U16 turned out 0 of 3, not 4 of 5**: the version guard is real but benign here,
  because none of the types these three wrappers name is version-gated (session-16 measurement).

### The pre-flags as they stood in the session-15 commit (`status.md` at HEAD `9ecbf581`, lines 117-135)

**Next session: `help.cpp`** (alphabetical from here; 16 TUs done across sessions 1-15: `accelerator`,
`accessibility`, `brush`, `caret`, `clipboard`, `combobox`, `console`, `timer`, `coordtransform`, `cursor`, `desktop`,
`devicecontext`, `devicemanagement`, `dialog`, `errorhandling`, `filledshapes`). **3 wrappers over 5 declarations**, a
39-line TU with no comment block (`:18 SetWindowContextHelpId`, `:25 GetWindowContextHelpId`, `:32 WinHelp`) plus a
redundant `#undef WinHelp` at `:14`. The section is `//Shell functions` (`user32.h:641`), **not** "Help functions" - the
file name does not match the section it implements, worth a U12/U21-style note. `:642-643`
`SetMenuContextHelpId`/`GetMenuContextHelpId` are **non-static and defined nowhere in the folder** (a U4 + U13 pair; both
are real exports, `17C`/`34A`, and both are already in `tmp/_tmp_name_resolve_probe.cpp` from session 15). `WinHelp` is the folder's
second-confirmed **AW** binding (`qor_pp_useswinapiAW(user32, WinHelp)`; `WinHelp` MISSING / `WinHelpW` RESOLVED per the
session-1 probe note at line 306) and the only stem here that is in the `user32.h:9-17` `#undef` list, so the `#undef` at
`help.cpp:14` is dead by construction: `user32.h` includes only `kernel32.h` at `:8` and does the nine `#undef`s at
`:9-17`, and no later include can re-process `winuser.h` past its own guard. Callers to trace:
`windowcontroller.cpp:785`, `:790`, `:795`. In `add_library` (`user/CMakeLists.txt:11`).
Pre-measured so the session can start at the analysis: all five names are real exports (`17C`, `34A`, `3EC`, `3ED` - and
note the export table has **`WinHelpA`/`WinHelpW` only**, no bare `WinHelp`, which is why the `AW` form is mandatory here);
the four context-help-id prototypes are at `winuser.h:9125`, `:9132`, `:9138`, `:9145` under
`#if(WINVER >= 0x0400)` (`:9105`) + Desktop (`:9110`) - so **4 of 5 are U16 instances** - while `WinHelpA`/`WinHelpW` are
at `:12372`/`:12380` under `#ifndef NOHELP` (`:12270`) + Desktop (`:12268`) with **no version guard**, and the
`#define WinHelp WinHelpW` at `:12386` is the macro `user32.h:10` exists to kill.

(The `:9125`/`:9132`/`:9138`/`:9145`, `:12372`/`:12380` and "`kernel32.h` at `:8`" citations in that last block were
superseded by the corrected measurements in the block above; `:8` vs `:7` is the `user32.h` include line as read in the
two sessions - session 16's `:7` is the measured value.)

### Superseded bookkeeping lines kept for the record (session-15 commit `status.md`, lines 115, 229-234)

```
| 16..38 | not started (TU index; 22 left, see "Remaining queue") | | | |

Remaining queue (22 TUs, alphabetical; `accelerator.cpp`, `accessibility.cpp`, `brush.cpp`, `caret.cpp`,
`clipboard.cpp`, `combobox.cpp`, `console.cpp`, `timer.cpp`, `coordtransform.cpp`, `cursor.cpp`, `desktop.cpp`,
`devicecontext.cpp`, `devicemanagement.cpp`, `dialog.cpp`, `errorhandling.cpp`, `filledshapes.cpp` done):
help, hook, icon, ime, keyboard, mdi, menu, message, monitor, mouse, painting,
processthread, rectangle, resource, scrollbar, systemshutdown, text, window, windowclass, windowprocedure,
windowproperties, winqor_api_user (module glue). `hook.cpp` is now in the build (see U3).

This file carries the session log and the open items. `memory-bank/activeContext.md` carries a pointer here.
```

Superseded by session 16 (`help.cpp` reviewed): the queue is now 21 TUs and the table's last row is "17..38".
The last line of the block above is `status.md`'s old one-sentence header pointer, replaced on consolidation by the
three-line header that also names this archive file.




