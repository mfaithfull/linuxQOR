# Defect census, sessions 1-21 (verbatim)

## Defect census (sessions 1-16 snapshot; moved verbatim out of status.md on 1 Nov 2026)

### Defect census, sessions 1-21 (22 TU entries, 180 wrappers)
- **Run-time-breaking defects: 12, all fixed.** 9 bad bindings - `combobox.cpp` x2 (session 6, P1), `coordtransform.cpp`
  x3 (session 8, P5's "wrong stem" class), `dialog.cpp` x1 (session 13, P1), `keyboard.cpp` x3 (session 20, P1:
  `GetKeyboardLayoutNameT`, `VkKeyScanT`, `VkKeyScanExT` asked `GetProcAddress` for the member's own `*T` spelling);
  plus 3 declaration defects that made every
  call site ill-formed - the three WinEvent members missing `static` (session 2b, C2352 x3) and `ConsoleControl` missing
  `static` (session 7, U13, now closed). Sessions 17 (`hook`), 18 (`icon`) and 19 (`ime`) found none: all three TUs'
  bindings were already D13-correct and verified three ways - and `ime.cpp`'s `qor_pp_useswinapiAW` is the case where the
  explicit spelling is the only thing that resolves at all (bare `SendIMEMessageEx` is MISSING, the SDK macro that would
  rewrite it is not in scope).
- **Still open in unreviewed TUs:** the folder-wide binding census re-run after this session
  (`tmp/_tmp_kb_p1census_s20.ps1` -> `tmp/_tmp_kb_p1census_post_s20.txt`) prints **17 unresolvable of 428 bindings**, and
  they split **11 truly broken + 6 P2**: `mouse.cpp` **all 10 wrappers** (`:19,26,33,40,47,54,61,68,75` bind bare single
  exports - `DragDetect`, `GetCapture`, `GetDoubleClickTime`, `GetMouseMovePointsEx`, `mouse_event`, `ReleaseCapture`,
  `SetCapture`, `SetDoubleClickTime`, `SwapMouseButton` - through `qor_pp_useswinapiAW`, whose `W` paste produces a name
  that does not exist, plus `:82`'s `LPTRACKMOUSEEVENTW`; `awchecklist` reads `mouse.cpp: bare=0 AW=10 single-ok=0`) and
  `window.cpp:124` (`CreateWindowW`). The other 6 are the **P2** class - `menu.cpp:159,194` and
  `message.cpp:124,145,152,159` bind A/W-only families bare and work today only because the SDK macro rewrites the stem.
  So `mouse.cpp`'s session is a **whole-TU binding fix** - the largest single-TU run-time defect of
  the review, deferred to that session by user decision on 1 Nov 2026 (no out-of-turn hotfix; reachability measured in
  `registers/prescan.md` P1) - and the `AW`-on-a-single-export shape is invisible to P5 (stem == member). Also open: P2 x6 in total
  (`menu.cpp` x2, `message.cpp` x4), P3 x2 (`CreateWindowT`, `CopyCursorT` naming), U4's ~97 declared-but-undefined
  members including tasks T1/T3/T4/T5, and U18's `window.cpp` `void`-returning point converters.
- **Convention/consistency fixes:** 10 parameter names made to match the SDK (`devicemanagement` 2, `hook` 1, `icon` 7 -
  the folder-wide divergence scan now reads **0 over 426 definitions**), 2 decl/def parameter divergences and 1
  parameter type `LPTSTR` -> `LPCTSTR` (`desktop`), 2 member renames for the `T` suffix (`combobox`, the only published
  renames of the review), 1 module-token fix over 3 sites (`combobox` `User32` -> `user32`, 416/428 folder-wide now),
  3 `::DLGPROC` spellings (`dialog`), 1 implicit template-argument site (`brush`), 2 deducing `Call<>` sites (`desktop`),
  4 `()` -> `(void)` pairs (`clipboard`), 1 stray inner space (`devicecontext`), 4 trailing-space / `)` nits
  (`accessibility` x2, `errorhandling` x2), 2 nits (`console` comma space, section comment), 1 dead `#undef` in each of 2
  TUs (`console`, `help` - the folder is now clean of `#undef` in sources), 1 `::`-spelling split in a `Call<>` argument
  (`icon.cpp:48`) and 1 missing final newline (`icon.cpp`), 7 stray inner spaces before `)` and 2 deducing `Call<>` sites
  and 1 `()` -> `(void)` pair (`keyboard.cpp`, session 20), plus several stray blank lines.
- **Documentation:** every reviewed TU now carries a TU comment block recording the D13 binding rationale, the SDK/SAL
  and ownership contracts, the section-coverage claim and the U-item tallies - the largest being `help.cpp`'s 70 lines,
  then `hook.cpp`'s 64 and `icon.cpp`'s 54. `ime.cpp`'s is 31 lines, which is the proportionate size for a one-wrapper TU:
  its whole value is that it records *why the AW cannot be bare* and *that no SDK prototype is visible at all*.
- **One header line moved (session 19, the only `user32.h` edit since session 7's `static` fixes):** `:637`'s trailing
  `//Obselete do not use` became a line of its own at `:638` with the spelling corrected. The file stays **695 lines**, so
  no `user32.h:NNN` citation anywhere in the record moved - the comment was moved *into* the blank line below the
  declaration, not inserted above it - and the move additionally cures the `_tmp_decl_no_def_scan.ps1` blind spot, which is
  why the declared-but-undefined census below reads 65 rather than 64.
- **TUs with 0 defects of any class** (comment-only sessions): `caret`, `clipboard`, `cursor`, `devicecontext`,
  `filledshapes`, `errorhandling`, `help`, `ime`, plus `coordtransform`/`devicemanagement`/`console`/`hook`/`icon` for the
  binding half - `icon` and `hook` were clean on correctness and only needed SDK parameter names.
- **Folder-wide measures re-run by these sessions** (current values in the registers): 428 bindings over 38 TUs, P5 flags
  4 (1 live); 0 parameter-name divergences over 426 definitions (re-run 1 Nov, sessions 18 and 19); U14 **424 explicit /
  4 deducing**, spacing 250 tight / 178 spaced (re-run 1 Nov session 20 - the 2 `keyboard.cpp` sites are now explicit,
  leaving `mouse.cpp:83` and `window.cpp:118,254,317`); U14c zero-argument lists **37 defs `(void)` / 1 `()`** and **37
  decls `(void)` / 1 `()`**, the survivor `message.cpp InSendMessageEx`; T-suffix census 525 / 108 / 404 / **11** / 2, unchanged across sessions 11-13 and 17-19;
  U15 and U19 per-family tallies accumulate in their own entries; `user32.h` 525 declarations, 428 `static` since
  session 7, 695 lines; declared-but-undefined per `_tmp_decl_no_def_scan.ps1` **491 parsed / 426 defined / 65
  undefined** as of session 19 (was 490/426/64 while `:637` hid behind a trailing comment - see the header note above).
- **Folder-wide measures after session 21 (`mdi.cpp`, 1 Nov 2026).** 22 of 38 TUs reviewed. `mdi.cpp`: 4 wrappers, 4
  bindings, **0 binding defects** - all four already D13-correct, verified by `/EP` literal (`tmp/_tmp_mdi_pp.txt`),
  export table and live probe (`tmp/_tmp_mdi_probe_s21.txt`); **0 published-signature or parameter-name divergences**;
  **1 internal defect fixed** - the `INT` in `DefFrameProcT`'s `Call<>` list (U14), which takes the folder's count of
  uppercase-`INT` template arguments from 1 to **0** (`tmp/_tmp_int_sites_s21.ps1`). Unchanged folder totals: 428
  bindings, **17 unresolvable** (`tmp/_tmp_p1census_s21.txt` - `menu.cpp` 2, `message.cpp` 4, `mouse.cpp` 10,
  `window.cpp` 1); U14(b) 424 explicit / 4 deducing; U14(c) 37 defs `(void)` / 1 `()`; T-suffix census
  525 / 108 / 404 / 11 / 2 (`tmp/_tmp_tsuffix_s21_full.txt`); U4 491 parsed / 426 defined / 65 undefined; `user32.h`
  untouched at 695 lines. New this session: **U26's third mirror-struct instance** with the first x86-vs-x64 layout
  measurement of the review (`tmp/_tmp_msg_mirror_s21.cpp`/`.bat`), and the first family with **0 U19 bypass** (the MDI
  family has no host code anywhere in the tree). Gate: `tmp/_tmp_cl_all_s21c.log`, 38/38 at `/W4`, exit 0, 0 diagnostics.

