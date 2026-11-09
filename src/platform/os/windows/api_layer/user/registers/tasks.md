# T1-T9 deferred tasks - eight CLOSED, T9 open with **9e IN** (next step **9f**)

- **T1 - two declared-but-undefined members in the "Authorization functions" section.** CLOSED 8 Nov 2026 (D22).

- **T2 - the whole Window Station / Desktop surface is dead exported API, and its conventions are therefore untestable
  against a caller.** CLOSED 8 Nov 2026 (D25).

- **T3 - the "Power Management functions" pair is declared but has no owning TU at all.** CLOSED 8 Nov 2026.

- **T4 - the menu half of the context-help-id family is declared and defined nowhere.** CLOSED 8 Nov 2026.

Note - the 38-TU review queue that these tasks were tracked against closed in full at session 37 (5 Nov 2026), so its queue arithmetic and its per-TU hand-offs are dropped: the folder is now 46 sources with no next review TU, and every hand-off lives in the register that names it.

- **T5 - the IME/WINNLS block in the unlabelled `user32.h` run is declared and defined nowhere.** CLOSED 8 Nov 2026 (D23).

- **T6 - the whole "//List Box functions" section is declared and defined nowhere - there is no owning TU.** CLOSED
  8 Nov 2026 (D24).

- **T7 - the dead `#include "../returncheck.h"` includes across the folder.** CLOSED 5 Nov 2026 (D16).

- **T8 - execute U18: make every wrapper's published signature match the export it binds, and close the named coverage gap.** CLOSED 7 Nov 2026 (D19 + D20).

- **T9 - U4's remaining declared-but-undefined surface: 86 non-static members declared in `user32.h` and defined in no TU.** OPEN - **9e IN** (D26 opened the queue 8 Nov 2026; D27 landed 9a, D28 landed 9b, D29 landed 9c, D30 landed 9d and D31 landed 9e on 9 Nov 2026). One section per TU per session, the T8 6a-6g shape. Measured 8 Nov 2026, before 9a: `_tmp_static_census.ps1` reads 634 member-declaration lines / 548 `static` / **86 non-static** / 0 defined-but-not-static, and `_tmp_decl_no_def_repo_scan.ps1` reads **52** names declared and never mentioned as `User32::` in `src`/`examples`/`test`; the other 34 are the DDE block, invisible to that scanner because they end `) const;` (34 such lines in the header). **52 are in D18 scope** (declared in `um\WinUser.h`, all 52 name real exports): 9a String functions 34 (`:342-375`) -> `strings.cpp` **CLOSED (D27)**, 9b Raw Input 7 (`:543-549`) -> `rawinput.cpp` **CLOSED (D28)**, 9c System Information 4 (`:753-756`) -> `systeminfo.cpp` **CLOSED (D29)**, 9d Button Control 3 (`:378-380`) -> `buttoncontrol.cpp` **CLOSED (D30)**, 9e Synchronization 2 (`:779-780`) -> `synchronization.cpp` **IN (D31)**, 9f Bitmap 1 (`:776`) -> `bitmap.cpp` **the lead**, 9g Printing 1 (`:783`) -> `printing.cpp`. **9h is the DDE block, 34 members (`:507-540`), and is blocked on a scope decision** - D18's addendum names the DDE family as a superseded exclusion and 0 of the 34 are declared in `um\WinUser.h` (they are in `um\ddeml.h`), so wrap-or-delete is a user decision, not a review action; its 34 `) const;` are folder-invented against `ddeml.h`'s unqualified spelling and `mxDdeCreateStringHandle` (`:516`) is a mistyped name whose exports are `DdeCreateStringHandleA/W` (86/87). Each step puts `static` on the declaration with the definition in the new TU, copies the export's prototype (D18) and binds `qor_pp_useswinapiAW` for the `#undef`'d bare stems `SystemParametersInfo`/`GetRawInputDeviceInfo`/`LoadBitmap` (G2 + G5). Coverage stays **759 of 1,018** throughout 9a-9g - these close a link gap (`LNK2019`), never a coverage gap. Post-9e census (9 Nov 2026, `_tmp_static_census.ps1`): **52** sources / 634 decls / **598 static / 36 non-static** / **597** bindings, 0 defined-but-not-static and 0 defined-without-a-declaration, and `_tmp_decl_no_def_repo_scan.ps1` reads **2** of the remaining 36 (9f's 1 at `:776`, 9g's 1 at `:783` - the other 34 are the DDE `) const;` lines).
