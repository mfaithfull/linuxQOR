# T1-T9 deferred tasks - **all nine CLOSED** (T9 CLOSED 9 Nov 2026, steps 9a-9h under D27-D34)

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

- **T9 - U4's remaining declared-but-undefined surface: 86 non-static members declared in `user32.h` and defined in no TU.** CLOSED 9 Nov 2026 - **9h CLOSED (D34), the queue's last step** (D26 opened the queue 8 Nov 2026; D27 landed 9a, D28 landed 9b, D29 landed 9c, D30 landed 9d, D31 landed 9e, D32 landed 9f, D33 landed 9g and D34 landed 9h on 9 Nov 2026). One section per TU per session, the T8 6a-6g shape. Measured 8 Nov 2026, before 9a: `_tmp_static_census.ps1` reads 634 member-declaration lines / 548 `static` / **86 non-static** / 0 defined-but-not-static, and `_tmp_decl_no_def_repo_scan.ps1` reads **52** names declared and never mentioned as `User32::` in `src`/`examples`/`test`; the other 34 are the DDE block, invisible to that scanner because they end `) const;` (34 such lines in the header). **52 are in D18 scope** (declared in `um\WinUser.h`, all 52 name real exports): 9a String functions 34 (`:342-375`) -> `strings.cpp` **CLOSED (D27)**, 9b Raw Input 7 (`:543-549`) -> `rawinput.cpp` **CLOSED (D28)**, 9c System Information 4 (`:753-756`) -> `systeminfo.cpp` **CLOSED (D29)**, 9d Button Control 3 (`:378-380`) -> `buttoncontrol.cpp` **CLOSED (D30)**, 9e Synchronization 2 (`:779-780`) -> `synchronization.cpp` **CLOSED (D31)**, 9f Bitmap 1 (`:776`) -> `bitmap.cpp` **CLOSED (D32)**, 9g Printing 1 (`:783`) -> `printing.cpp` **CLOSED (D33)**. **9h is the DDE block, 34 members (the block was `:506`-`:540` before the edit), CLOSED 9 Nov 2026 (D34) as the DELETE arm** - the block and its section banner are deleted from `user32.h` (855 -> 819 lines, 36 lines gone) and **no TU is added**, so the folder stays 54 sources / 599 bindings. D34's measurements: 0 of the 34 in `um\WinUser.h` (its 4 `Dde|DDE` hits are `SM_CXPADDEDBORDER`/`DDL_HIDDEN`/`SPI_GETKEYBOARDDELAY`/`SPI_SETKEYBOARDDELAY`), 27 of the 34 in `um\ddeml.h` (674 lines, 100 DDE lines, **0** `const`), 7 in neither header, the fresh x64 export dump reads 38 DDE rows with 31 of the 34 names hitting a row and 3 not (`mxDdeCreateStringHandle`, `DdeInitialize`, `DdeQueryString`) and 7 rows undeclared here, 0 `User32::` uses of any of the 34 across `src`/`examples`/`test`, and coverage moves **759 -> 724** of 1,018 with the family's 35 exports landing in the no-prototype exclusion bucket (251 -> 286) while the 8 SDK-visible exclusions stay 8. The 34 `) const;` spellings are folder-invented
against `ddeml.h`'s unqualified spelling and `mxDdeCreateStringHandle` (`:516`) is a mistyped name whose exports are
`DdeCreateStringHandleA/W` (86/87). Each of 9a-9g puts `static` on the declaration with the definition in the new TU,
copies the export's prototype (D18) and binds `qor_pp_useswinapiAW` for the `#undef`'d bare stems
`SystemParametersInfo`/`GetRawInputDeviceInfo`/`LoadBitmap` (G2 + G5). Coverage stays **759 of 1,018** throughout 9a-9g
- those steps close a link gap (`LNK2019`), never a coverage gap; 9h is the step that moves coverage, because it deletes
declarations. Post-9g census (9 Nov 2026, `_tmp_static_census.ps1`): **54** sources / 634 decls / **600 static / 34
non-static** / **599** bindings, 0 defined-but-not-static and 0 defined-without-a-declaration, and
`_tmp_decl_no_def_repo_scan.ps1` reads **0** declared-and-undefined names repo-wide (the 34 DDE `) const;` lines are that
scanner's blind spot, not part of either count). Post-9h: **54** sources / **600** decls / **600 static / 0 non-static** /
**599** bindings, 0 and 0, and the blind spot is gone.
