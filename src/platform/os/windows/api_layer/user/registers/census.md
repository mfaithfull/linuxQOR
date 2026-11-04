# Defect census, sessions 1-23 (append-only; the sessions 1-16/1-21 text below stays verbatim)

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
  U19's per-family tally accumulates in its own entry; U15's stopped at the 2 Nov 2026 closure (user decision - policy
  D14, `docs/agent/DECISIONS.md`), so nothing is appended to `registers/u15.md`; `user32.h` 525 declarations, 428 `static` since
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

- **Folder-wide measures after session 22 (`menu.cpp`, 2 Nov 2026).** 23 of 38 TUs reviewed. `menu.cpp`: 39 wrappers, 39
  bindings, **2 fixed** (the P2 pair `GetMenuString` `:225`, `InsertMenuItem` `:260` -> `qor_pp_useswinapiAW`,
  behaviour-neutral by `/EP`) and **0 binding defects of the P1/P5 class** - the other 30 bare bindings are proven
  correct by the four-way classification (`tmp/_tmp_menu_class_s22.ps1`), which is the folder's first per-stem proof that
  a large bare block is legal rather than merely unchecked. **0 return-type divergences** (new instrument
  `tmp/_tmp_menu_ret_s22.ps1`), **3 parameter-type divergences recorded not changed** (`TrackPopupMenu` `HWND prcRect`;
  `InsertMenuT`/`ModifyMenuT` `void* uIDNewItem`), 0 U4/U13/U17, 0 signature flags, 6 parameter-name drifts. Folder
  totals: 428 bindings, **15 unresolvable** (was 17 - `message.cpp` 4, `mouse.cpp` 10, `window.cpp` 1); wrong-stem scan 3
  flags, unchanged; U14(b) 424 explicit / 4 deducing; U14(c) 37 defs `(void)` / 1 `()`; T-suffix census unchanged; U4 491
  parsed / 426 defined / 65 undefined; `user32.h` untouched at 695 lines. New register content: U18 (return vs parameter
  types), U16 **12 guarded** (largest single TU), U15 **4 of 39** unreachable, U19 **40/10/18/6** with 0 bypass in built
  code, U21 **fourth instance** (+ `ChangeMenuA/W` and `CalculatePopupWindowPosition` declared nowhere), T4 both halves
  re-measured as real exports. Hygiene: `archive/session-log.md` split twice by hash-verified slices (45,673 -> 30,334
  bytes, 34,136 with its row). Gate: `tmp/_tmp_cl_all_s22final.log`, **38/38** at `/W4`, exit 0, 0 diagnostics, 38 fresh objects.

- **Folder-wide measures after session 23 (`message.cpp`, 2 Nov 2026).** 24 of 38 TUs reviewed. `message.cpp`: 24 wrappers,
  24 bindings, **4 fixed** - the whole remaining **P2** list converted to `qor_pp_useswinapiAW`
  (`:214 RegisterWindowMessage`, `:235 SendMessageTimeout`, `:242 SendMessage`, `:249 SendNotifyMessage`), proven
  behaviour-neutral by `/EP` before and after; plus 1 U14(c) `()` -> `(void)` pair (`InSendMessageEx`, def `:176` +
  `user32.h:300`, which **closes U14(c)** - re-measured 38 defs `(void)` / 0 `()` and 39 decls `(void)` / 0 `()`
  (`tmp/_tmp_u14c_s24.ps1`, run in session 23 despite the `_s24` name; the 39th declaration `user32.h:635
  DisableProcessWindowsGhosting` is defined in no TU, U4) and 3
  binding-macro comma-space nits (`:19`, `:26`, `:68`). **0 return-type and 0 parameter-type divergences** over all 24
  (`tmp/_tmp_msg_ret_s23.ps1` - the SDK's return type sits on its own line above the stem, so this needed a new
  instrument). Folder totals moved: unresolvable bindings **15 -> 11** of 428 and **P2 is closed** - what is left is the
  `mouse.cpp` cluster (10 sites, `:19-:82`) and `window.cpp:124 CreateWindow` (`tmp/_tmp_p1census_post_s23.txt`);
  binding-macro spacing 414 spaced / **12** not-spaced (all 12 in `scrollbar.cpp`, was 15); U14(b) unchanged 424 explicit /
  4 deducing; T-suffix census unchanged 525 / 108 / 404 / 11 / 2 with **0** deviations from this TU; `user32.h` back to
  **695 lines** (the `InSendMessageEx(void)` edit was in place). New items: **P6's second instance**
  (`BroadcastSystemMessage` bare == the `A` RVA), **U21's fifth instance**, 4 U16-guarded names, U19's first
  all-24-live/0-bypass-in-built-code family, U25's fourth spelling (`const MSG*` unqualified), U26 re-confirmed on both
  arches. Gate: **`tmp/_tmp_cl_all_s23recover.log`** - 38/38 at `/W4`, exit 0, 0 diagnostics, re-run 2 Nov 13:11 **after** the
  last source edit (11:36); `cl /EP` of the TU exits 0. The `_tmp_cl_all_s23final.log` / `_s23pre.log` names written by the
  interrupted pass are byte-identical to `_tmp_cl_all_s22final.log` (SHA256 `7EB28546A9...`, 10:03) and must not be cited as
  session-23 evidence: `Copy-Item` keeps the source's timestamp, so a copied gate log cannot date its own run.

- **Folder-wide measures after session 24 (`monitor.cpp`, 2 Nov 2026).** 25 of 38 TUs reviewed. `monitor.cpp`: 5 wrappers,
  5 bindings, **0 fixes** - the first TU of the review with nothing to change (every D13/U14/U18/U25/U27 check passed as
  found). 111 comment lines added, TU 51 -> **162 lines**; `user32.h` **untouched at 695 lines**. Unchanged folder
  measures, all re-run: unresolvable bindings **11 of 428** (`tmp/_tmp_p1census_pre_s24.txt` ==
  `tmp/_tmp_p1census_post_s24.txt`; the 11 are `mouse.cpp:19,:26,:33,:40,:47,:54,:61,:68,:75,:82` and
  `window.cpp:124 CreateWindow`), wrong-stem scan **4** rows (`cursor.cpp:64`, `mouse.cpp:82`, `resource.cpp:19`,
  `:26`), T-suffix census unchanged **525 / 108 / 404 / 11 / 2** with 0 deviations from this TU, U14(b) unchanged
  424 explicit / 4 deducing (all 5 sites here explicit), binding-macro spacing unchanged 414 spaced / 12 not-spaced,
  U14(c) still closed (this TU's 3 zero-parameter-free signatures raise no `()`), 0 `CONST` added. New register content:
  **U16 6/6** (first TU whose whole section shares one guard), **U19 first BYPASS in built code**
  (`i_m/monitor.cpp:163`, same file as its own layer sites at `:56`/`:62`) with 8/0/4/0 and all five wrappers live,
  **U21 sixth instance** (the 45-head `WINVER >= 0x0500` region: measured split **6 heads here, 17 in seven other TUs,
  22 with no wrapper anywhere** - `tmp/_tmp_mon_region_s24.ps1`; plus a **U4 hand-off**: `GetListBoxInfo` is declared at
  `user32.h:548`, defined in no TU and called nowhere, yet exported `174`), **U25 fifth spelling** (mixed `::`
  qualification inside three lines), **U26 fourth instance** (mirror safe on both arches; `long long`-versus-`LPARAM`
  callback the only x86 divergence). 0 new P1/P2/P5/P6 instances; 0 U15 instances (D14). Hygiene of the TU after the
  edit: 0 tabs, 0 trailing-space lines, 0 non-ASCII bytes, longest line 124 (`mdi.cpp`'s own longest is 204, so the
  folder's ceiling is not a 100-column one). Gate: **`tmp/_tmp_cl_all_s24b.log`** - 38/38 at `/W4`, exit 0, 0 `Cnnnn:`
  diagnostics (the only `error` substring in the log is the file name `errorhandling.cpp`), 38 TU echoes and 38 objects
  rebuilt inside the run, logged 2 Nov **19:08:42**, i.e. after the final `monitor.cpp` edit at **18:55**. The earlier
  `_tmp_cl_all_s24.log` (15:14:47) predates that edit and must not be cited as this session's final gate. `cl /EP` of the
  TU exits 0: post state `tmp/_tmp_mon_pp_post_s24b.txt` (18:57), pre `..._pre_s24.txt` (14:56) - **453,286 -> 453,397
  lines, 111 added, all of them blank, 0 removed, 0 non-blank added**, so the emitted code is provably unchanged.
  `_tmp_mon_pp_final_s24.txt` (18:22) predates the 18:55 edit and is not the post state.

- **Session 25 (`mouse.cpp`) folder-wide measures, 2 Nov 2026.** Binding census (`tmp/_tmp_kb_p1census_s20.ps1`): **428
  bindings, 11 unresolvable -> 1** (`tmp/_tmp_p1census_pre_s25.txt` -> `tmp/_tmp_p1census_post_s25b.txt`); all 10 rows
  that left were this TU's `*W` names and the sole survivor is `window.cpp:124 CreateWindow`. Wrong-stem scan
  (`tmp/_tmp_wrong_stem_scan.ps1`): **4 -> 3 rows, 0 live** (survivors `cursor.cpp:64` = P3, `resource.cpp:19`/`:26` =
  U11). U14(b) **424/4 -> 425/3** with folder spacing unchanged at 178 spaced / 250 tight. U17's T-suffix census
  **unchanged** at 525 declarations parsed / 108 A+W+T norm / 404 single-export bare / 11 + 2 deviations
  (`tmp/_tmp_tsuffix_pre_s25.txt`) - all 10 mouse members are in the 404 class, which is what makes their `AW` spelling a
  defect rather than a style choice. U16 gains 3 guarded prototypes (3 of 10 here, and the first
  `DESKTOP | PC_APP` partition instance this item has measured). U19 9 LAYER / 7 INDIRECT / 0 BYPASS / 0 MEMBER,
  unchanged. Name-divergence scan over this TU (`tmp/_tmp_name_divergence_scan.ps1 -Tu mouse.cpp` ->
  `tmp/_tmp_namediv_pre_s25.txt`): 491 header declarations parsed, 10 definitions, **0 site divergences, 0 parameter-name
  divergences** - the folder's zero-divergence record holds. Gate (`tmp/_tmp_cl_all_s25c.log`): **38/38 echoes at `/W4`,
  exit 0, 0 `Cnnnn:` diagnostics**, logged 2 Nov **22:18:27**, after this TU's final edit at **22:14:54** - the earlier
  `_tmp_cl_all_s25b.log` (22:06:32) predates that last citation fix and is not the final gate. `cl /EP` re-run after the
  last edit: post3 `tmp/_tmp_mouse_pp_post3_s25.txt` **453,402** lines vs pre-fix `..._pre_s25.txt` **453,321** -
  **0 removed, 81 added, every added line blank**, so the comment pass changed no emitted code; post2 (22:06) vs post3 is
  **0 removed / 0 added**. Post-fix classifier: 10 literals, **0 not in the export table**, 0 P6-shaped. TU after the
  edit: **167 lines** (86 before), 0 tabs, 0 trailing-space lines, 0 non-ASCII bytes, longest line 144. `user32.h`
  untouched at 695 lines. Close-out verifier: `tmp/_tmp_s25_verify.ps1` -> `tmp/_tmp_s25_verify.txt`.


- **Session 26 (`painting.cpp`) folder-wide measures, 3 Nov 2026.** 27 of 38 TUs reviewed. `painting.cpp`: 25 wrappers,
  the whole `//Painting functions` section, **2 fixes, both whitespace** (`:153` had unspaced `Call<>` arguments, `:244`
  a space before `)`); 191 -> **261 lines**, `user32.h` untouched at 695. Binding census **428 / 1 unresolvable**
  unchanged (`tmp/_tmp_p1census_pre_s26.txt` == `tmp/_tmp_p1census_post_s26.txt`; survivor `window.cpp:124`).
  Wrong-stem scan **3** rows unchanged (`cursor.cpp:64`, `resource.cpp:19`, `:26`). U14(b) **425/3** unchanged; this TU
  25 explicit / 0 deducing, all 25 tight `Call<`, intra-TU argument spacing now uniform. Per-stem classifier
  (`tmp/_tmp_paint_class_s26.ps1` over `tmp/_tmp_paint_pp_pre_s26.txt`): 25 stems, winuser.h macro only for
  `DrawState`/`GrayString`, all 25 `/EP` literals in the export table, **0 P2, 0 P6**. Name compare
  (`tmp/_tmp_paint_pre_s26.ps1`): 25/25 defs pair, **0 hdr-vs-def divergences**, 2 def-vs-sdk parameter renames
  (`DrawCaption`, `DrawStateT`); return types all agree via `tmp/_tmp_paint_ret_s26.txt` (0 U18). Live probe x64+x86:
  25 spellings resolve at the table RVAs, bare `DrawState`/`GrayString` and both `*T` names MISSING (gle=127).
  Registers: U16 +7 guarded members (first `_WIN32_WINNT >= 0x0501`, `GetWindowRgnBox`), U19 +2 real BYPASS sites
  (`paintsession.cpp:41`/`:47`, first layer-vs-thunk side-by-side in this section), U26 width-invisible pair (A/W
  DrawState/GrayString differ in what they draw with identical arguments - 7797 vs 357 px etc, both arches).
  **Instrument bug found**: PS `-replace 'T$'` is case-INsensitive and ate the trailing `t` of six painting stems
  (BeginPaint, EndPaint, DrawFocusRect, GetUpdateRect, InvalidateRect, ValidateRect), which read as sdk=0 rows until
  switched to `-creplace` - same family as the `-ne` case trap at `tooling.md:39`; `_tmp_sdk_param_name_scan.ps1:238-244`
  had already documented and avoided it. Gate: **`tmp/_tmp_cl_all_s26.log`** - 38/38 echoes at `/W4`, exit 0, 0
  `Cnnnn:` diagnostics, run 3 Nov after the final `painting.cpp` edit at **01:03:33**. `cl /EP` neutrality:
  `tmp/_tmp_paint_pp_pre_s26.txt` 453,426 -> `tmp/_tmp_paint_pp_final_s26.txt` 453,496 lines, non-blank multiset
  differs by exactly the two edited `Call<>` lines and by nothing else, and the 25 binding literals are byte-identical.
  TU hygiene: 0 tabs, 0 trailing-space lines, 0 non-ASCII, longest line 173.
- **Session 27 (`processthread.cpp`) folder-wide measures, 3 Nov 2026.** 28 of 38 TUs reviewed. `processthread.cpp`: 5
  wrappers, the whole `//Process and Thread functions` section, **0 binding/signature/return/idiom defects - every source
  change was whitespace** (one TAB line, three spaces before an argument list's `)`, and the TU's five `Call<>` sites
  normalised to the folder's tight spelling); 57 -> **122 lines** (65-line contract block at `:16`-`:79`), `user32.h`
  untouched at 695. Binding census **428 / 1 unresolvable** unchanged - `tmp/_tmp_p1census_pre_s27.txt` and
  `tmp/_tmp_p1census_post_s27.txt` compare **identical**, survivor still `window.cpp:124`. Wrong-stem scan **3** rows
  unchanged (`cursor.cpp:64`, `resource.cpp:19`, `:26`); no `processthread.cpp` row. U14(b) **425 explicit / 3 deducing**
  unchanged, and all 5 sites here explicit. **U14(a) 178 spaced / 250 tight -> 174 / 254** (`tmp/_tmp_u14_post_s27.txt`;
  this TU 4 spaced + 1 tight -> 0 / 5) - the first time (a) has been moved deliberately rather than opportunistically,
  on the user's decision. Per-stem classifier (`tmp/_tmp_pt_class_s27.ps1` over `tmp/_tmp_pt_pp_pre_s27.txt`): 5 stems,
  **0 winuser.h macros**, all 5 `/EP` literals in the export table, **0 P2, 0 P6**. Name compare
  (`tmp/_tmp_pt_pre_s27.ps1`): 5/5 definitions pair with the header, **0 hdr-vs-def and 0 def-vs-sdk** parameter renames;
  return types all agree via `tmp/_tmp_pt_ret_s27.txt` (0 U18). `_tmp_sigcmp.ps1` reports 5/5 "decl==def ok" **and** 5/5
  call-site "ok" - the first TU apart from `message.cpp` where its call-site half actually parsed anything (it read one
  line, so wrapped calls printed `CALL NOT PARSED`; fixed this session). U16: 3 of 5 prototypes version-guarded, 5 of 5
  DESKTOP-only (`tmp/_tmp_pt_guards_s27.txt`). U19 over the 5 stems plus their A/W/T spellings: **1 LAYER / 0 INDIRECT /
  0 BYPASS / 0 MEMBER** (`tmp/_tmp_pt_bypass_s27.txt`). New coverage instrument `tmp/_tmp_pt_coverage_s27.ps1`: of 1,018
  export names **370** have no `user32.h` member at all - 114 declared in the installed `winuser.h`, 256 not. Live probe
  not needed here (no `AW` site and no name to disprove). TU hygiene: 0 tabs, 0 trailing-space lines, 0 non-ASCII, longest
  line 124. Gate: **`tmp/_tmp_cl_all_s27.log`** - 38/38 echoes at `/W4`, exit 0, 0 `Cnnnn:` diagnostics, run 3 Nov after
  the final `processthread.cpp` edit at **08:00:59**. `cl /EP` neutrality: `tmp/_tmp_pt_pp_pre_s27.txt` 453,292 ->
  `tmp/_tmp_pt_pp_final_s27.txt` 453,357 lines, non-blank multiset identical except exactly the **7** whitespace-touched
  lines, and the 5 binding literals byte-identical (`tmp/_tmp_pt_neutral_s27.txt`).


- **Session 27 addendum (3 Nov 2026) - two folder measures that came from the record pass, not from the TU.**
  (a) **Coverage:** `tmp/_tmp_pt_coverage_s27.ps1` compares the 1,018 export names in `tmp/_tmp_user32_exports.txt`
  against the member declarations parsed from `user32.h`: **370 exports have no member at all**, of which **114 are
  declared in the installed `winuser.h`** (SDK-visible and simply unwrapped - DPI-awareness, pointer-input, touch/pen and
  display-config families) and **256 are not declared there**. No `//Process and Thread functions` member is in either
  class. (b) **`returncheck.h` include census:** `tmp/_tmp_rcinclude_scan.ps1` (`tmp/_tmp_rcinclude_s27.txt`) - **37 of 38**
  sources include `../returncheck.h`, only **2** use any name it declares, so **35 includes are dead**; the same scan over
  `api_layer/kernel` gives **22 includes / 20 TUs with hits** (`tmp/_tmp_rcinclude_kernel_s27.txt`), which is why this is a
  `user`-folder habit and not a layer convention. Registered as **T7** (`registers/tasks.md:121`) by user decision - the
  deletion is mechanical, the go-ahead is what was missing. Folder-wide U14(a) after this session's normalisation:
  **174 spaced / 254 tight** (`tmp/_tmp_u14_final_s27.txt`), 428 bindings / **1** unresolvable, wrong-stem scan **3** rows /
  **0** live. Pre-flags for the next TU (`rectangle.cpp`, 11 wrappers in `user32.h:446`-`:457`, all single exports, **no**
  version guard on any of the 11 prototypes) are in `tmp/_tmp_rect_pre_s28.txt`.

- **Session 28 (`rectangle.cpp`) folder-wide measures, 3 Nov 2026.** 11 of 38 sources touched, 11 wrappers, **0 defects of
  any class** and **0 header lines moved** - the 11 edits are whitespace plus the user-authorised `Call<>` normalisation.
  Binding census (`tmp/_tmp_p1census_post_s28.txt`): 428 bindings, **1** unresolvable (`window.cpp:124 CreateWindow`) -
  unchanged. Wrong-stem scan (`tmp/_tmp_wrongstem_post_s28.txt`): 38 files, 426 definitions, 428 bindings, **3** rows,
  **0** live (the known `cursor.cpp:64` + two `resource.cpp` binding-outside-a-definition rows) - unchanged, and no
  `rectangle.cpp` row. U14 after this TU (`tmp/_tmp_u14_post_s28.txt`): 425 explicit / 3 deducing (`window.cpp:118`,
  `:254`, `:317`), **163 spaced / 265 tight**; 30 of the spaced sites are in unreviewed TUs (`windowclass.cpp` 14,
  `systemshutdown.cpp` 5, `windowproperties.cpp` 5, `text.cpp` 4, `resource.cpp` 2), the other 133 in reviewed TUs whose
  sessions did not normalise. **U14 is CLOSED by this session - see decision D15 and `registers/u14.md`.** New folder
  measures from this TU: the `CONST` spelling census (`tmp/_tmp_rect_const_s28.txt`: 17 / 18 / 10 lines in the sources,
  17 / 0 / 4 in `user32.h`, `/EP` literals 31 / 16 / 49) and the rectangle-family coverage measure
  (`tmp/_tmp_rect_family_s28.txt`: 51 exports containing `Rect`, 41 declared in `winuser.h`, 39 with a member;
  SDK-visible gaps `EvaluateProximityToRect`, `GetPointerDeviceRects`, and from the region scan `GetWindowWord` +
  `SetWindowWord`, which have no member at all; `SetSysColors` is declared at `user32.h:651` and undefined anywhere =
  U4). Gate: `tmp/_tmp_rect_cl_s28.log` single TU `cl_exit=0`, library `tmp/_tmp_cl_all_s28b.log` 38 echoes /
  0 diagnostics (run 11:28, after the last edit at 11:19:45), `/EP` neutrality `tmp/_tmp_rect_neutral_s28.txt`
  (non-blank 154,338 both, only the 11 intended call lines differ, 11/11 binding literals identical). T7 re-counted:
  37 of 38 sources include `../returncheck.h`, only 2 (`errorhandling.cpp`, `processthread.cpp`) use it - **35 dead**.
  Pre-flags for the next TU (`resource.cpp`, 2 wrappers declared at `user32.h:443`-`:444` in the `//Resource functions`
  section at `:442`) are in `tmp/_tmp_res_pre_s29.txt`; the U19 scan already flags both of its bindings as outside a
  `User32::` definition (U11), and `LoadImage` is a textbook **P6/D13 shape** - bare not exported, `A`/`W` exported,
  `#define LoadImage LoadImageW` - where this file already uses `qor_pp_useswinapiAW`.

- **Session 29 (`resource.cpp`) folder-wide measures, 3 Nov 2026.** 30 of 38 sources reviewed. 2 wrappers, **1 real defect
  fixed**: the folder's only U11 pair, which was simultaneously **U4 x2** - `User32::CopyImage`/`User32::LoadImageT`
  declared `static` at `user32.h:443`-`:444` with no definition anywhere - plus 2 whitespace nits. `user32.h` did not
  move; both bindings were already D13-correct. Emitted symbols went `?CopyImage@api@win@qor@@YAPEAXPEAXIHHI@Z` ->
  `?CopyImage@User32@api@win@qor@@SAPEAXPEAXIHHI@Z` (same for `LoadImageT`), and **neither form appeared in the built
  DLL's 439 exports or the import library's 854 member symbols** - the members were uncallable, the free functions
  unpublished. Folder instruments after the fix (`tmp/_tmp_res_scan_final_s29.txt`): binding census **428 bindings, 1
  unresolvable** (`window.cpp:124 CreateWindow`) - unchanged; wrong-stem scan **3 rows -> 1** (only `cursor.cpp:64`
  `CopyCursorT` binding `CopyIcon`, P3); `_tmp_undefined_static.ps1` now **0** `static` declarations with no definition
  folder-wide, so **U4 loses 2 of its ~97**; parameter-name divergence **0 over 428 definitions**; U19 over the 8
  spellings of both stems: **0 LAYER / 0 INDIRECT / 0 BYPASS / 0 MEMBER**; `Call<>` paren census **421 tight / 0 spaced**
  (this TU held the folder's last `>( pFunc` site); bracket census unchanged at **265 tight / 163 spaced** (U14 CLOSED,
  D15 - the TU's 2 spaced `Call< ` sites left alone). `resource.cpp` now carries a 10-line contract block, which closes
  the last gap in the census's "every reviewed TU has a block" claim: `_tmp_res_blockchk_s29.ps1` finds the only remaining
  block-less sources are the 7 unreviewed TUs (`scrollbar` 102, `systemshutdown` 51, `text` 44, `windowclass` 114,
  `windowprocedure` 30, `windowproperties` 51, `window` 615) plus the 14-line glue TU. Gate: `tmp/_tmp_res_cl_s29.log`
  single-TU `cl_exit=0`; library `tmp/_tmp_cl_all_s29b.log` 38 echoes / 0 diagnostics, run 15:00:28 with all 38 TU objects
  newer than the edited 14:58:40 source (its hash is identical to session 28's clean log - see `gotchas.md`); `/EP`
  neutrality `tmp/_tmp_res_neutral_final_s29.txt` (453,265 -> 453,276 lines, non-blank identical at 154,284, only the 4
  intended lines differ each way, `"CopyImage"`/`"LoadImageW"` byte-identical); consumer probe `tmp/_tmp_res_ref_s29.cpp`
  links both members with **0** unresolved for either.

## Session 31 census update (4 Nov 2026, `systemshutdown.cpp`)

`systemshutdown.cpp` reviewed: 5 wrappers, **1 defect fixed** - the folder's only two `WINAPI`-qualified `User32::`
definition heads (`:37` `ShutdownBlockReasonDestroy`, `:44` `ShutdownBlockReasonQuery`) normalized to the bare spelling the
other 3 heads and `user32.h:602`-`:606` use; ill-formed for x86 (`C2373` x2), a no-op for x64 (identical mangled symbol and
`dumpbin /disasm` byte-identical). Folder totals after the fix: **428 bindings, 428 lowercase `user32` module tokens, 0
`WINAPI`-qualified definition heads (was 2), 1 unresolvable binding left** (`window.cpp:124 CreateWindow`). Signatures 5/5
`decl==def` incl. parameter names; U4, U11, U13, U17, U18 all 0 for this TU; U16 5/5 DESKTOP, 1 version-gated
(`LockWorkStation` `_WIN32_WINNT >= 0x0500`); U19/U15 0 sites of any class; U14 untouched (frozen 163 spaced / 265 tight,
D15); **T7 36 -> 37**. New measurement for the record: x86 is not buildable tree-wide - **93 of 106** api_layer TUs fail on
`kernel32.h:92` `PUNWIND_HISTORY_TABLE` (`registers/u6.md` addendum). Gate: `tmp/_tmp_cl_all_s31c.log` 38 echoes /
0 diagnostics, all 38 objects newer than the edited source; single-TU `cl_exit=0`. Block census: `systemshutdown.cpp` now
carries a 50-line contract block (`:16`-`:65`), leaving **5 unreviewed TUs** without one (`text` 44, `windowclass` 114,
`windowprocedure` 30, `windowproperties` 51, `window` 615) plus the glue TU.



## Measured 4 Nov 2026 (session 32) - `window.cpp` wrapper count corrected: 50 -> **85**

The census row for `window.cpp` (and the "50 wrappers / 270 lines" figure carried in several sessions' pre-flags) is
wrong. Direct counts on the file (4 Nov 2026): **615 lines**, `qor_pp_fcontext` = **85**, qualified definition heads
(`^    ... User32::Name(`) = **85**, binding macros = **85** (10 `qor_pp_useswinapiAW` + 75 direct
`qor_pp_useswinapi`), `Library::Call<` = **82** - so **3 wrapper bodies have no `Library::Call<` line** and session 33
must account for them. The file has only **3 comment lines**, i.e. no closure block yet. Consequence for the folder-wide
totals: every per-TU wrapper count for the remaining five TUs (`window.cpp`, `windowclass.cpp`, `windowprocedure.cpp`,
`windowproperties.cpp`, `winqor_api_user.cpp`) must be re-measured with the same three counters before the census is
closed, and the binding-site census (428 at session 30) is a **floor**, not a total, until `window.cpp`'s 85 are added.
