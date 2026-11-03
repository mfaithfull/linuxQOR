# session log table rows 27-28 + closure notes (rows 20-22 in `archive/session-log-20-22.md`, rows 23-24 in `archive/session-log-23-24.md`, rows 25-26 in `archive/session-log-25-26.md`)

## Moved out of `status.md` on 1 Nov 2026 (session log, session-17 block, icon pre-flags, notes, residue, consolidation history)

## Session log (append only, never reread)

### Rows 1-8 moved out

Rows 1-8 (`accelerator`, `accessibility`+`timer`, `accessibility` 2b, `brush`, `caret`, `clipboard`, `combobox`, `console`,
`coordtransform`) are kept verbatim in `archive/session-log-1-8.md` - moved on 2 Nov 2026 when this file passed the 40 KB
ceiling. Rows below are renumbered nowhere; the `#` column is the original session number.
| # | TU | Wrappers | Verdict | Action taken |
|---|---|---|---|---|

### Rows 9-16 moved out

`cursor`, `desktop`, `devicecontext`, `devicemanagement`, `dialog`, `errorhandling`, `filledshapes`, `help` - moved
verbatim to `archive/session-log-9-16.md` on 2 Nov 2026 (same 40 KB reason). Session numbers in the `#` column are the
original ones.
| # | TU | Wrappers | Verdict | Action taken |
|---|---|---|---|---|
### Rows 17-19 moved out

Rows 17-19 (`hook`, `icon`, `ime`) are kept verbatim in
`archive/session-log-17-19.md` - moved on 2 Nov 2026 when the file passed 38 KB after row 23.

### Rows 20-22 moved out

Rows 20-22 (`keyboard`, `mdi`, `menu`) are kept verbatim in `archive/session-log-20-22.md` - moved on 2 Nov 2026
when the file passed 40 KB after row 24. Session numbers in the `#` column are the original ones.

### Rows 23-24 moved out

Rows 23-24 (`message.cpp`, `monitor.cpp`) are kept verbatim in `archive/session-log-23-24.md` - moved on 3 Nov 2026
when the file passed 40 KB with row 27. Each row was sha256-matched against
`tmp/_tmp_s27_preslice_sessionlog.md` before the rewrite.


### Rows 25-26 moved out

Rows 25-26 (`mouse.cpp`, `painting.cpp`) are kept verbatim in `archive/session-log-25-26.md` - moved on 3 Nov 2026
when the file passed 40 KB with row 28. Each row was sha256-matched against
`tmp/_tmp_s28_preslice_sessionlog.md` before the rewrite.

| 27 | `processthread.cpp` | 5 | **0 binding, signature, return or idiom defects; whitespace-only fixes** (the folder's last TAB line at old `:29`, three stray spaces before `)` at old `:21`/`:46`/`:54`, and the TU's 5 `Call<>` sites normalised to the folder's tight spelling by user decision - U14(a) moved deliberately for the first time) | Whole `user32.h:255 //Process and Thread functions` section (`:256`-`:260`): 5/5 decl/def pairs in header order, 0 U4/U11/U13/U17/U18, **0 hdr-vs-def and 0 def-vs-sdk parameter renames**, returns all agree (`tmp/_tmp_pt_ret_s27.txt`), `_tmp_sigcmp.ps1` 5/5 `decl==def ok` **and** 5/5 call-site `ok` - that instrument had to be fixed first, it read one line so 3 of these 5 wrapped calls printed `CALL NOT PARSED`. Binding: 5 bare single exports, **no A/W family and no `winuser.h` macro**, so the bare `qor_pp_useswinapi` it already had is the only spelling that can bind them - 5/5 `/EP` literals resolve, 0 P1/P2/P5/P6 from this TU (`tmp/_tmp_pt_class_s27.ps1`); no live probe needed (no `AW` site, no name to disprove). | TU 57 -> **122** lines (64-line contract block); `/EP` pre 453,292 -> final 453,357 with the non-blank multiset differing only by the 7 whitespace-touched lines and the 5 binding literals byte-identical. Registers: **U16** 3 of 5 prototypes version-guarded, 5 of 5 DESKTOP-only, second `_WIN32_WINNT >= 0x0501` (`IsWow64Message`) plus the **empty** `#if (_WIN32_WINNT >= 0x0602)`/`#endif` pair at `winuser.h:4121`-`:4122` that a guard scan without a depth counter reports as a real guard; **U2** 3 of the library's 4 `CheckReturn` sites are here + its **inverse case** (`WaitForInputIdle` returns 0 for success, so `CheckNonZero` would invert it); **U19** 1 LAYER / 0 INDIRECT / 0 BYPASS (`ui/gui/view/handlers/messagehandler.cpp:246`). New folder measures: **coverage** 370 of 1,018 exports unwrapped, 114 SDK-declared (`tmp/_tmp_pt_coverage_s27.ps1`), and the **`returncheck.h` include census** 37 include / 2 use / **35 dead** -> registered as **T7**. Folder U14(a) **174 spaced / 254 tight**, census 428 bindings / **1** unresolvable, wrong-stem 3 rows / 0 live. Gate `tmp/_tmp_cl_all_s27.log` 38/38 `/W4` exit 0 after the 08:00:59 edit. Narrative `tu/processthread.md`. |
| 28 | `rectangle.cpp` | 11 | **0 binding, signature, return or idiom defects; spacing-only fixes** - `:20` a space before the argument list's `)`, `:69` a space before a comma (`int , int`, a shape new to the review), and the TU's 11 `Call<>` sites normalised to the folder's tight spelling by user decision, which **closes U14 (policy D15)** because this was the last reviewed TU with spaced sites | Whole `user32.h:446 //Rectangle functions` section (`:447`-`:457`): 11/11 decl/def pairs in header order, 0 U4/U11/U13/U17/U18, **0 hdr-vs-def and 0 def-vs-sdk parameter renames** (the section copies the SDK's names verbatim - the cleanest naming reviewed), returns all `BOOL` and all agree (`tmp/_tmp_rect_ret_s28.ps1`). Binding: **11 bare single exports, no A/W family and no `winuser.h` macro** (`5B`,`10B`,`20C`,`223`,`23E`,`29B`,`2B8`,`360`,`361`,`3A3`,`3C0`), so P2/P6 cannot arise here by construction and no live probe is needed - 11/11 `/EP` literals resolve (`tmp/_tmp_rect_class_s28.ps1`: 0 unresolved, 0 macro, 0 P6-shaped); folder census unchanged 428 bindings / 1 unresolvable, no wrong-stem row. `_tmp_sigcmp.ps1` 11/11 `decl==def ok` but **7 `CALL TYPE MISMATCH` rows carrying 11 mismatched arguments** - every one the **U25** spelling artifact (`CONST::RECT*` in the definition vs `CONST ::RECT*` in the template argument, and one list even mixes `CONST RECT*`), so arity/types were re-checked field-for-field against the SDK dump instead of trusting it. **U2's biggest increment**: the eleven `BOOL`s are three different kinds - answers (`EqualRect`, `IsRectEmpty`, `PtInRect`, where `BoolCheck` would report a legitimate FALSE as an error), documented-outcome zeros (`IntersectRect` writes an all-empty `RECT` **and** returns 0; `SubtractRect`/`UnionRect` likewise), and 5 success-or-failure calls whose FALSE is discarded silently by this layer *and* by `ui/gui/view/drawing/rect.cpp:53`/`:86`/`:93` one level up. **U16 0 of 11** - the first TU with **no** version-guarded prototype at all (the whole block sits in `#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)` at `winuser.h:9717`). **U19** 11 LAYER / 2 "INDIRECT" / 3 BYPASS, and both INDIRECT rows are a **new false-positive class**: same-named member *definitions*, not calls into the DLL (`EditController::SetRect` sends `EM_SETRECT` via `SendMessageT`; `Region::SetRect` calls `GDI32::SetRectRgn`). The 3 BYPASS sites are live in the built `winqor_gl` target (`ui/gl/CMakeLists.txt:5`): `ui/gl/windowimpl.cpp:516`, `:521` (unqualified `OffsetRect` beside `WindowController::AdjustRect*` calls) and `:1756` (`::SetRect`); a 4th grep hit at `:1470` is inside the `/* */` block `:1303`-`:1507` and the scan's comment filter excluded it correctly. New coverage findings: `SetSysColors` is declared at `user32.h:651` and **no TU defines it** (a U4 instance surfaced from this section), `GetWindowWord`/`SetWindowWord` have no member anywhere, and over the 51 export names containing `Rect` 41 are SDK-declared / 39 wrapped (`tmp/_tmp_rect_region_s28.ps1`, `tmp/_tmp_rect_family_s28.ps1` - the region scan's `GetWindowLongA/W` "no member" rows are an instrument artifact, the member is `GetWindowLongT`). | TU 93 -> **174** lines (81-line contract block); `/EP` 453,328 -> 453,409 with the **non-blank multiset identical at 154,338** (22 diff entries = the 11 intended call lines) and all 11 binding literals byte-identical (`tmp/_tmp_rect_neutral_s28.txt`). Folder U14(a) 174/254 -> **163 spaced / 265 tight**, U14 CLOSED with the tally frozen and the residual located (30 spaced in five unreviewed TUs, 133 in reviewed ones). Registers: **U2**, **U16**, **U19**, **U25** appended; **U14 closed**. Gate `tmp/_tmp_cl_all_s28b.log` **38/38 at `/W4`, 0 diagnostics**, log newer than the last edit; TU hygiene 0 TAB / 0 trailing space / 0 non-ASCII, longest line 124. Narrative `tu/rectangle.md`. |
| 29..38 | not started (TU index; 9 left, see "Queue" in `status.md`) | | | |

**Session 17 (`hook.cpp`, 31 Oct 2026) - closed.** Row 17 above plus new items **P6** and **U27** carry the record, and the TU
narrative is the 64-line comment block now at `hook.cpp:16-79`, so this session has no separate write-up (the session-16
pattern). The consumed `hook.cpp` pre-flags are archived verbatim in `session-log-archive.md` under "Consumed pre-scan
notes"; one of them was wrong and is corrected in row 17 (the "2 more U17 instances" flag - both wrappers do carry the `T`
suffix).

**Session 18 (`icon.cpp`, 1 Nov 2026) - closed.** Row 18 above carries the verdict and the fixes; the per-family tallies
are now written into **U15** (12 of 14 unreachable), **U16** (4 of 14 gated + the `ICONINFOEX` type sub-case), **U17**
(0, census unchanged), **U19** (16/0/7/0), **U20** (the `Icon` half measured) and **U26** (escalated to a silent
overrun), all in `registers.md`; the TU narrative is the 54-line comment block at `icon.cpp:16-69`, so there is no
separate write-up (the session-16/17 pattern). Two pre-flags were corrected by measurement, both recorded in row 18: the
"at least 3 version-gated prototypes" count (it is 4 - `CreateIconFromResourceEx` was missed) and the
`PrivateExtractIconEx` coverage note (exported, but `winuser.h` declares them nowhere and the tree never mentions them,
so they are a U4 surface decision rather than a coverage gap one name away). New instruments this session:
`tmp/_tmp_icon_guards.ps1`, `_tmp_icon_type_guards.ps1`, `_tmp_icon_liveness_scan.ps1`, `_tmp_icon_fmt.ps1`,
`_tmp_icon_own_probe.cpp`/`_tmp_run_icon_own_probe.bat`, `_tmp_icon_null_probe.cpp`/`_tmp_run_icon_null_probe.bat`,
`_tmp_iconinfoex_mirror.cpp`/`_tmp_run_iconinfoex_mirror.bat`, `_tmp_iconinfoex_assert.cpp`/
`_tmp_run_iconinfoex_assert.bat`, `_tmp_icon_overload_probe.cpp`/`_tmp_run_icon_overload_probe.bat`,
`_tmp_icon_include_probe.cpp`/`_tmp_run_icon_include_probe.bat` (the last two compile-only - the layout/overload facts
they settle cannot be got at by running anything, because the `Icon` class objects are in unbuilt modules).
Gate re-run after the edits: 38/38 at `/W4`, `tmp/_tmp_cl_all.log`.

**Next session: `mdi.cpp`** (alphabetical from here; **21 TUs done** across sessions 1-20: `accelerator`,
`accessibility`, `brush`, `caret`, `clipboard`, `combobox`, `console`, `timer`, `coordtransform`, `cursor`, `desktop`,
`devicecontext`, `devicemanagement`, `dialog`, `errorhandling`, `filledshapes`, `help`, `hook`, `icon`, `ime`,
`keyboard`). Start that session from the **P1** entry in `registers/prescan.md` only for orientation - the 2 bindings
still open there belong to `window.cpp:124` and `mouse.cpp:82`, not to `mdi.cpp`; `mdi.cpp` is the folder's smallest real
TU (1,896 bytes) and needs its own section-coverage measurement first.

**Session 19 (`ime.cpp`, 1 Nov 2026) - closed.** Row 19 above carries the verdict; the tallies are written into **U4**
(2 more members + the instrument blind spot that had been under-counting U4 by one), **U15** (1 of 1), **U16** (0 of 1,
plus a sharper sub-class: opt-in headers rather than version-gated ones), **U17** (1 instance, census unchanged), **U19**
(1/1/0/0), **U26** (second mirror-struct instance) and **T5** (new deferred task). The TU narrative is the 31-line comment
block now at `ime.cpp:16-46`, so there is no separate write-up (the session-16/17/18 pattern). **Two pre-flags corrected
by measurement:** the claim that the WINNLS/IMP family is declared in no SDK `um` header (it is - `um\WinNls32.h:65-89`,
with `IMEPROA`/`IMEPROW` at `:37-52`), and the implied "IME is unreachable from this layer" (`um\Windows.h:250` includes
`<imm.h>`, so 64 `Imm*` prototypes sit in every TU's chain). Both corrections are in the **U4** and **U16** entries. The
`user32.h` edit - the first header change of the review since session 7's `static` fixes - was chosen by the user from
four options; the file was deliberately kept at **695 lines** so no `user32.h:NNN` citation anywhere in the record went
stale, which also fixed the `_tmp_decl_no_def_scan.ps1` blind spot and moved the declared-but-undefined count 64 -> 65.
Instruments: all of the `ime` ones were **staged by session 18** on 1 Nov, not written here - `tmp/_tmp_ime_pre.bat` +
`_tmp_ime_pre.txt` (the measured pre-flag run), `_tmp_ime_shape.ps1` (include-block and `Call<` spacing norms + the header
blind-spot count), `_tmp_pp_ime.bat` -> `_tmp_ime_pp.txt` (re-run after this session's edits, `pp_exit=0`),
`_tmp_dump_ime.bat` -> `_tmp_ime_linkermember.txt` (`linkermember:1` publishes `?SendIMEMessageEx@User32@api@win@qor@@SA_J`
and `?GetDpiForWindow@...@SAI` and **neither** undefined member of the block) and `_tmp_ime_probe_build.txt`. Written here:
6 rows appended to `tmp/_tmp_name_resolve_probe.cpp` (bare `IMPGetIME`/`IMPQueryIME`/`IMPSetIME`,
`WINNLSGetEnableStatus`, `CliImmSetHotKey`, `User32InitializeImmEntryTable`) and its re-run
`tmp/_tmp_name_resolve_s19b.txt`, which is what closes the section-coverage claim - every `IMP*` family member resolves
**only** as `A`/`W`. Gate after the edits: **38/38 at `/W4`, exit 0, zero diagnostics**
(`tmp/_tmp_cl_all_s19c.log`).

**Off-protocol item fix (U6, 1 Nov 2026) - closed.** Not a TU session: the user asked for U6 resolved before the queue
moved on, so no session row was added and the TU count stays at **21**. The fix is `library.h:46` and `:68` - `Call` and
`voidCall` now build their `fPtr` as `__stdcall`, matching `DefProc` (`:16`, fixed earlier in user commit `89da3452`) and
the `WINAPI` exports every `qor_pp_useswinapi`/`...AW` binding names. The full measurement, the x86 disassembly evidence
and the 1,638-binding blast radius are in **U6** (`registers/u6.md`), which is now a resolved record; `registers/u7.md`
(the exception-swallow in the same two functions) and `G5` (the NULL-pointer throw) stay open, and the `Call`/`voidCall`
line numbers in those entries shifted by the 13 lines added above `Call` - **re-measured here: throw at
`library.h:52` (`Call`) and `:84` (`voidCall`), swallow-and-default-return at `:54-62` (`Call`), swallow-only at
`:74-80` (`voidCall`)**; `registers/u6.md` carries the full HEAD-vs-now mapping.
`cursor.cpp:34` is the only TU comment in the folder that cited U6 as live risk and was updated to past tense.
Gate: user folder **38/38** at `/W4`, exit 0, 0 diagnostics (`tmp/_tmp_cl_all_u6post.log`), plus the new all-libraries
extended gate over **106** api_layer sources, whose pre-fix and post-fix logs are byte-identical
(`tmp/_tmp_cl_all_apiall_u6_pre.log` / `..._post.log`) - see "Off-protocol item fix (U6)" in `tooling.md` for the
instruments and the caveat that the extended gate already exits non-zero on HEAD because of `opengl/wgl.cpp:31`.

**Next session: `menu.cpp` (session 22) - and split rows 1-8 out of this file first; it is over the ~40 KB ceiling recorded in `status.md`.**


**Consumed pre-scan notes for `icon.cpp`** - kept verbatim below as they stood when session 18 opened (they were in
`status.md` at the time). Everything in them was measured 31 Oct 2026; the two corrections are marked in row 18.

The "Icon functions"
section is `user32.h:426-440`: **14 declarations, all `static`, all 14 defined here** (`icon.cpp:16`..`:107`, in header
order) - a 114-line TU with **no comment block**. `_tmp_sigcmp.ps1 -Tu icon.cpp -First 427 -Last 440` gives **14/14
decl==def ok** and 13/14 `Call ok` (run 31 Oct) - the single `CALL TYPE MISMATCH` at `icon.cpp:48` (`sdk 'const
::ICONINFO*' vs call 'const ICONINFO*'`) is the known `::`-spelling false positive class, see "Instrument caveats".

Measured pre-flags for `icon.cpp` (session 18) - every number below measured 31 Oct 2026, so they are facts, not guesses:
- **11 bare bindings + 3 `AW`, and the `AW` set is exactly the SDK's three Icon A/W families.** The export table
  (`tmp/_tmp_user32_exports.txt`) has `CopyIcon` 59, `CreateIcon` 6B, `CreateIconFromResource` 6C,
  `CreateIconFromResourceEx` 6D, `CreateIconIndirect` 6E, `DestroyIcon` B1, `DrawIcon` D5, `DrawIconEx` D6,
  `GetIconInfo` 160, `GetIconInfoExA` 161/`W` 162, `LoadIconA` 25A/`W` 25B, `LookupIconIdFromDirectory` 26F,
  `LookupIconIdFromDirectoryEx` 270, `PrivateExtractIconsA` 2B5/`W` 2B6. `tmp/_tmp_winuser_aw_macros.txt` has exactly
  three `*Icon*` macros (`LoadIcon`, `GetIconInfoEx`, `PrivateExtractIcons`) - precisely the three `AW` sites at
  `icon.cpp:82`, `:89`, `:110`, so 0 P5 wrong-stem flags and 0 D13 conversions needed. The live probe (rows staged
  31 Oct, run 31 Oct): all **11 bare stems RESOLVED**, and bare `GetIconInfoEx`/`LoadIcon`/`PrivateExtractIcons`
  **MISSING** with their `W` halves RESOLVED - the three `AW` bindings are load-bearing (a bare binding there asks for a
  name `user32.dll` does not export) and the eleven bare ones are the D13 spelling. Still re-run `cl /EP` into its own
  file to confirm the literals, as every session does.
- **U17: 0 instances** - the three A/W wrappers already carry the folder's `T` suffix (`GetIconInfoExT`, `LoadIconT`,
  `PrivateExtractIconsT`), and the census (525/108/404/11/2, re-run 31 Oct) is unchanged.
- **U16: at least 3 of the 14 prototypes are version-gated, and one is the ill-formed-type sub-case.** Guard stacks
  measured 31 Oct with `tmp/_tmp_guard_stack_scan.ps1`: `LookupIconIdFromDirectoryEx` (`winuser.h:10819`) sits under
  `#if(WINVER >= 0x0400)` (`:10815`) and `DrawIconEx` (`:10948`) under `:10883` (also `0x0400`); `GetIconInfoExW`
  (`:11026`) sits under `#if(_WIN32_WINNT >= 0x0600)` (**`:10986`, closed `:11034`**) - and `ICONINFOEXA`/`ICONINFOEXW`
  plus the `PICONINFOEX` typedef are at **`:10987`-`:11014`, inside that same guard**, so `user32.h:436`'s
  `GetIconInfoExT(HICON, ::PICONINFOEX)` does not merely lose its prototype below `_WIN32_WINNT 0x0600`, it becomes
  ill-formed on an unknown type - the `HDEVNOTIFY`/`DEVMODE`/`MSGBOXPARAMSW`/`PFLASHWINFO` sub-case again, and the
  highest version threshold of them. The rest (`LoadIconW` `:10750`, `PrivateExtractIconsW` `:10775`, `CreateIcon`
  `:10793`, `CreateIconIndirect`/`CopyIcon`/`GetIconInfo` `:10965`-`:10982`) sit under nothing but the Desktop partition
  + `NOUSER` (`:64`) - no version test at all. Four names still need their stacks read
  (`CreateIconFromResource`/`Ex`, `DestroyIcon`, `LookupIconIdFromDirectory`, `GetIconInfoExA`).
- **U19: 17 stems -> 16 LAYER / 0 INDIRECT / 7 BYPASS / 0 MEMBER** (31 Oct, corrected instrument). All 16 **LAYER** sites
  are in one **built** file: `ui/gui/gdiobjects/icon.cpp:18`, `:24`, `:30`, `:36`, `:42`, `:48`, `:54`, `:62`, `:73`,
  `:79`, `:84`, `:89`, `:94`, `:99`, `:104`, `:109` (`gui/CMakeLists.txt:49`) - the first family of the review where
  every wrapper has a compiled call site, so the session's liveness work is the one-level-up question, and **U20** is
  already open on the `Icon` ownership model in exactly that file. The 7 **BYPASS** are all in **unbuilt** targets: 5 in
  `ui/gl/windowimpl.cpp:191`, `:1603`, `:1608`, `:1642`, `:1645` (`ui/CMakeLists.txt:4 #add_subdirectory(gl)`) and 2 in
  `examples/.../retropad/controller.cpp:43`, `retropad.cpp:940` (`osspecific/windows/CMakeLists.txt:9`) -> **0 bypass in
  built code**.
- **Section coverage:** `PrivateExtractIconExA` 2B3 / `PrivateExtractIconExW` 2B4 are exported (both RESOLVED in the
  probe) and declared **nowhere** in `user32.h`, one name away from the wrapped `PrivateExtractIcons` - decide whether
  the section should include them. `ArrangeIconicWindows` E (`user32.h:135`), `IsIconic` 237 (`:181`) and `OpenIcon` 29F
  (`:190`) are Icon-adjacent exports declared in the **Window** section instead (a U21-shaped placement note), and
  `InternalGetWindowIcon` 221 is internal. `winuser.h:10681` defines `CopyCursor(pcur)` as a macro over `CopyIcon`, so
  the cursor family silently depends on this TU's `CopyIcon`.
- Style to check against U14/U25 in the session: the file spells all 14 sites **tight** (`Call<HICON, HICON>`), it has no
  TU comment block at all, and `icon.cpp:44`/`:48` use `const ::ICONINFO*` / `const ICONINFO*` - the `::`-spelling split
  in a `Call<>` template argument (the `dialog.cpp` class, cured by spelling the site like the declaration).

Take the pre-scan P1/P2/P3/P5 entries into account only when their owning TU comes up; do not fix other TUs outside
their own session.

**What is deliberately not in the queue: there is no `dde.cpp`** - the 34-declaration DDE
section (`user32.h:501-534`) has no TU anywhere, which is a U4 surface decision, not a TU session (see the U4 entry).

**Consumed pre-scan notes.** The `help.cpp` (session 16) pre-flags that used to sit in this space are superseded in full
by the session-16 row in the table above, the **U26** entry and task **T4**; the pre-flag text itself is kept verbatim in
`session-log-archive.md` under "Consumed pre-scan notes". Sessions 15/16's `filledshapes`/`help` pre-flags were consumed
the same way and are not repeated here.

**Session 22 (`menu.cpp`, 2 Nov 2026) - closed.** Row 22 above carries the verdict and the fixes; the narrative is the
66-line comment block now at `menu.cpp:15`-`:80` (the session-16/17/21 pattern), with the per-family detail in
`archive/tu/menu.md`. Consumed pre-scan notes: the P2 pair `menu.cpp:159`/`:194` (post-block `:225`/`:260`) is struck
from `registers/prescan.md`, and the four P2 survivors named in that entry are all `message.cpp`. No `user32.h` line
moved, so no `user32.h:NNN` citation anywhere in the record shifted. Two record-slices were taken **before** this row was
written (`archive/session-log-1-8.md`, `archive/session-log-9-16.md`, both rejoin-verified `MATCH`); the ceiling is back
under 40 KB and `archive/README.md` points at all three files.

**Session 23 (`message.cpp`, 2 Nov 2026) - closed.** Row 23 above carries the verdict; narrative `archive/tu/message.md`.
This session was **interrupted by a tool failure mid-close-out** and finished by a recovery pass, so every close-out fact was
re-measured rather than trusted: gate re-run **after** the last source edit (`tmp/_tmp_cl_all_s23recover.log` - 38/38 at
`/W4`, exit 0, 0 diagnostics, `tmp/obj/message.obj` rebuilt at 13:11), because the `_tmp_cl_all_s23final.log`/`_s23pre.log`
the interrupted pass produced are byte-identical to session 22's log (SHA256 `7EB28546A9...`, 10:03, i.e. before both source
edits) - `Copy-Item` preserves the source's timestamp, so a copied gate log cannot date its own run (caveat written into
`method.md`). The rows 17-19 slice was **hash-verified row by row** against `tmp/_tmp_s23_preslice_sessionlog.md` (`MATCH`
x3; ids 1-23 present exactly once across the four session-log files), the locator was **regenerated** (46 entries - P3/P4/P5
and T5 had moved inside `prescan.md`/`tasks.md` under this session's register edits, so 6 of `status.md`'s pointers were
stale) and both README indexes rewritten (`archive/README.md` 26 rows, now including `tu/message.md` and
`session-log-17-19.md`; `registers/README.md` 34), `status.md` was trimmed **202 lines / 21.0 KB -> 180 / 18,432 B** (its own
ceiling), the table's placeholder row was corrected `23..38` -> `24..38` (14 left), and two record citations were corrected:
**`archive/tu/ime.md` and `archive/tu/keyboard.md` do not exist** - sessions 16-20 have no TU narrative file, their narrative
is the row. Audit instrument: `tmp/_tmp_s23_closeout.ps1` (six checks: gate log, slice rejoin hashes, record ceilings,
locator pointer resolution, index coverage, TU hygiene/binding counts) + `tmp/_tmp_s23_locsplice.ps1`. One pre-existing nit
recorded, not touched: `user32.h:66-68` (the class head) is tab-indented - 3 tabs in the header, unchanged since before the
review, outside this TU's scope.


## Session 24 close-out note (`monitor.cpp`, 2 Nov 2026)

Zero-fix session, so the close-out work was all record hygiene and hand-off. Measured again after the edits: folder
binding census **11 of 428** unchanged (`tmp/_tmp_p1census_pre_s24.txt` == `_post_s24.txt` - a comment-only edit must move
nothing, and re-running the census is how that is shown rather than asserted); wrong-stem scan still 4 rows; T-suffix
census still 525/108/404/11/2; `user32.h` **695 lines**, untouched. TU hygiene after the 98-line insert: 0 tabs, 0
trailing-space lines, 0 non-ASCII bytes, longest line 124, and the three in-file self-citations (`:114`/`:142` definition
range, `:124` the `AW`, `:132` the by-value `POINT`) re-pointed to post-insert line numbers - the insert moved the
definitions, which is the one class of stale citation a comment-only edit can create. `archive/tu/monitor.md` added (50
lines) and indexed; `archive/session-log.md` 193 -> 195 lines / 34,847 B with row 24 and the placeholder corrected
`24..38` -> `25..38` (13 left); registers appended at **EOF only** so no locator line number moved (`u16.md` 186 -> 202,
`u19.md` 177 -> 197, `u21.md` 83 -> 106, `u25.md` 40 -> 54, `u26.md` 98 -> 128, `census.md` 105 -> 125 - all under the
40 KB ceiling, none re-sliced); `status.md` rewritten at **180 lines / under its 18 KB ceiling** with session 25's
`mouse.cpp` pre-flags measured fresh (all 10 stems are single exports - `CB`, `125`, `152`, `18E`, `3F5`, `2FE`, `320`,
`339`, `3A4`, `3B3` - and `qor_pp_useswinapiAW` pastes `##W` (`kernel32.h:52`), so all 10 ask for a `*W` name that does
not exist: session 25 is a whole-TU binding fix, not a review). New instruments: `_tmp_mon_pp_s24.bat`,
`_tmp_mon_class_s24.ps1`, `_tmp_mon_mirror_s24.{cpp,bat}`, `_tmp_mon_probe_s24.cpp`, `_tmp_run_mon_probe_s24.bat`
(`_tmp_mon_pre_s24.ps1`, `_tmp_mon_pre2_s24.ps1`, `_tmp_mon_guards_s24.ps1`, `_tmp_mon_bypass_s24.ps1` dated from the
pre-flag stage). Two caveats learned the hard way and written into `method.md`: (1) an inserted block must not re-open
the namespace line - the first attempt duplicated `namespace qor { namespace win { namespace api {`, which the standalone
compile caught as `C2888` + `C1075` before anything else ran; (2) `run_commands` executes its array **concurrently**, so
chaining a producer and its consumer (`cl /EP` then the classifier that reads its output) in one call races and fails -
chain dependent steps across calls or in one shell command.

## Session 25 close-out note (`mouse.cpp`, 2 Nov 2026)

The session the pre-scan predicted: `mouse.cpp` was already known to hold 10 broken bindings, so the work was to prove
the diagnosis three ways before touching anything (`/EP` literals, the export table, a live `GetProcAddress` probe) and
then fix 11 lines. Measured after the edits: folder census **11 -> 1 unresolvable of 428**, wrong-stem scan **4 -> 3 rows
and 0 live**, U14(b) **424/4 -> 425/3**, U17's T-suffix census **unchanged** (525/108/404/11+2 - all 10 mouse members are
in the 404 single-export class, which is the evidence that `AW` was a defect, not a style choice), name-divergence scan
**0/0 over 10 definitions**, `user32.h` **695 lines**, untouched. `cl /EP` neutrality: pre-fix 453,321 lines, post3
453,402 - **0 removed, 81 added, every added line blank**; and post2 (22:06:35) vs post3 (22:18:29) = **0/0**, which is how the
last comment-wording tweak was shown to be code-neutral. Gate `tmp/_tmp_cl_all_s25c.log` (22:18:27, 3 min after the last
edit at 22:14:54) - the 22:06 `_s25b.log` predates that edit and is **not** the gate of record, the same trap as session
24's `_s24.log`. Verifier `tmp/_tmp_s25_verify.ps1` -> `tmp/_tmp_s25_verify.txt` re-checks gate order, both `/EP` diffs,
the 10 literals, TU hygiene and the record ceilings in one run.

Registers were appended **at EOF only**, so no locator pointer moved: `u16.md` 202 -> 218, `u19.md` 197 -> 208,
`u14.md` 65 -> 73, `prescan.md` 106 -> 123 (P1 11 -> 1, P5 0 live), `census.md` 131 -> 151 - all still far under the
40 KB ceiling. `tasks.md` is the one in-place edit (the stale queue paragraph at `:74`-`:83`, 14 TUs -> 12), kept at
exactly 10 lines so `T5` at `:87` and `T6` at `:102` still resolve. `archive/tu/mouse.md` added (78 lines) and indexed.
`archive/session-log.md` passed the ceiling (37,988 B) before this row could go in, so rows **20-22** (`keyboard`, `mdi`,
`menu`) moved verbatim to `archive/session-log-20-22.md` (11 lines / 10,350 B), each row **sha256-verified** against the
snapshot `tmp/_tmp_s25_preslice_sessionlog.md` (`tmp/_tmp_s25_logslice.ps1` prints 3 `MATCH` + `0` unmatched original
lines); the placeholder was corrected `25..38` -> `26..38` (12 left) and the file's title line points at the slice. After
row 25 and this note it is **259 lines / ~35.5 KB** - 4.5 KB under the ceiling. Session 26 measured that headroom first:
row 26 plus this note fit without slicing (**264 lines / 37,515 B**, ~2.4 KB spare), so the predicted second slice was
not needed - rows **23-24** stay the natural candidates and **session 27 must slice before its row goes in**. Session 26's
record bookkeeping: `archive/tu/painting.md` (41 lines) added and indexed, `archive/README.md` rows for both, `tooling.md`
+the 6 `paint` instruments and the `-replace`/`-creplace` caveat, `gotchas.md` +the same caveat, `census.md` row 26,
`u16`/`u19`/`u26` extended.

Three instrument lessons, all now in `method.md`: (1) a PowerShell function called `H` is **not** usable - `H` is the
built-in `Get-History` alias, and the argument binder then tries to parse the row text as an `-Id` integer; the slice
script aborted before writing the source, which is why it must verify before it rewrites; (2) `run_commands` still
executes its array concurrently, so a `Get-ChildItem` of a file another element is rewriting reported the **pre-write**
size - re-measure in a later call, never in the same one; (3) any edit that changes a comment block's **line count**
invalidates every in-file self-citation in that block (`:97`/`:160`/`:100`-`:163`/`:164`/`:125` here), so citations are
re-pointed after the block settles and then re-checked mechanically - the first cleanup pass moved them by 2 lines and
the citation fixer caught it. `_tmp_s25_verify.ps1` counts the comment citations (27 here; the 4 it flags as
out-of-range are `user32.h:555`/`:556`/`:565` and `winuser.h` citations, i.e. pointers into another file, not stale ones
- it does not yet distinguish scope, a known limitation of that check).




## Session 27 close-out note (`processthread.cpp`, 3 Nov 2026)

The quietest session of the review so far: 5 wrappers, 0 binding defects, 0 signature defects, and every source change
was whitespace. That is worth recording as a result, not just as an absence - the TU's five stems are single exports with
no A/W family and no `winuser.h` macro, so the bare bindings it already had are load-bearing and the only question was
whether the layer had drifted anywhere near them (it had not: 0 P1/P2/P5/P6, 0 name divergences, 0 return divergences).
The user's decision on the one open judgement call - normalise all five `Call<>` sites to the folder's **tight** majority
rather than this TU's local spaced majority - is the first time U14(a) has been moved deliberately rather than
opportunistically, so folder U14(a) is now **174 spaced / 254 tight** and `processthread.cpp` 0/5.

Three findings that outlive the TU. (1) **U16's empty guard**: `winuser.h:4121`-`:4122` is an `#if (_WIN32_WINNT >=
0x0602)` with nothing between it and its `#endif`, and it sits between `AttachThreadInput` and `WaitForInputIdle` - a
backward guard scan without a depth counter attributes 0x0602 to `WaitForInputIdle`. Every future U16 pass needs the
depth counter (`_tmp_pt_guards_s27.ps1` has it; `_tmp_sdk_guard_scan.ps1` replays nesting properly). (2) **U2's inverse
case**: `WaitForInputIdle` returns `WAIT_OBJECT_0` (0) for success, so the `CheckNonZero` its neighbour uses would invert
it - a checked-failure site is only correct when the SDK's failure value is not also a success value. (3) **coverage was
measured, not assumed**: the contract block originally claimed "the only two user32 exports with no wrapper anywhere",
which was invented; `_tmp_pt_coverage_s27.ps1` says **370 of 1,018** exports have no `user32.h` member at all (114 of
them declared in `winuser.h`), and of the two names cited, one (`DdeSetUserHandle`) is in fact declared - at `:528`, as
part of U4's 34-member DDE block. The paragraph was rewritten against the measurement before the gate ran.

Two instrument fixes. `_tmp_sigcmp.ps1` matched `Library::Call<...>(pFunc, ...);` on **one** line, so any wrapped call
printed `CALL NOT PARSED` - 3 of this TU's 5 sites, and the same shape already recorded in `painting.cpp`,
`monitor.cpp` and `mouse.cpp`; it now joins lines to the terminating `;`, and all 5 sites here report arity + type `ok`.
New `_tmp_pt_cite_s27.ps1` resolves every `:NNN` in the contract block against the file it belongs to (bare citations
inherit the last file marker in the paragraph, and "(in this TU: ...)" marks a self-citation), asserts the 5 def + 5
binding self-citations land on the right construct, and printed 28 citations / 0 unresolved. That check caught the
`gui/CMakeLists.txt` -> `ui/gui/CMakeLists.txt` path error and, before it, three citations that had drifted by 6 lines
when the coverage paragraph grew - the standing rule that any edit changing a block's line count invalidates its own
citations still holds.


Record bookkeeping: rows **23-24** sliced out to `archive/session-log-23-24.md` (10 lines / 8,842 B, each row
sha256-matched against `tmp/_tmp_s27_preslice_sessionlog.md` before the rewrite; the live file was 37,515 B and could not
have taken row 27 + this note), `archive/tu/processthread.md` added (52 lines) and indexed, `census.md` row 27 appended,
`u2`/`u14`/`u16`/`u19` extended **at EOF only** so no locator pointer moved, `tooling.md` + the 10 `pt` instruments and
the two scanner fixes, `gotchas.md` + the coverage-claim and citation-context caveats. Instruments:
`_tmp_pt_{pre,ret,guards,class,lines,edit,cite,coverage,neutral}.ps1`, `_tmp_pt_pp_s27.bat`, `_tmp_s27_pre.ps1`,
`_tmp_s27_logslice.ps1`.

Two measures came out of the record pass rather than the TU. **`returncheck.h` include census** -
`_tmp_rcinclude_scan.ps1`, written to settle whether the include this TU carries is a layer convention: **37 of 38**
`user` sources include it, **2** use any name it declares, so **35 includes are dead**; the same scan over
`api_layer/kernel` is **22 includes / 20 TUs with hits**, so it is a `user` habit. Registered as **T7**
(`registers/tasks.md:121`) by user decision - mechanical, but the go-ahead was what was missing, not the analysis.
Instruments added beyond the plan: `_tmp_pt_coverage_s27.ps1`, `_tmp_rcinclude_scan.ps1`, `_tmp_pt_cite_s27.ps1` (the
citation resolver), `_tmp_rect_pre_s28.ps1` (the next TU's pre-flag sweep).
`_tmp_pt_neutral_s27.ps1` was caught comparing the non-blank multiset over the `_final_` capture against the binding
literals of the `_post_` capture - two different captures of the same TU; the result held here, and the script now uses
one capture for both halves.

**Record incident, recorded because it changed the file.** While tidying this note, an edit statement piped a variable
into `[IO.File]::WriteAllText` after a `[string]::Replace` overload-resolution failure had left that variable `$null`, and
the call wrote a **0-byte** `archive/session-log.md`. It was rebuilt the same session from
`tmp/_tmp_s27_preslice_sessionlog.md` (the snapshot the slice script took) by replaying `_tmp_s27_logslice.ps1`'s exact
transformation, re-inserting row 27 and re-applying this note - `tmp/_tmp_s27_logrestore.ps1` performs the replay and
prints the proof (rows 23-24 sha256-match the slice file; every snapshot line except the two rows, the title and the
placeholder paragraph is reproduced byte-identically). The caveat now in `gotchas.md`: `[string]::Replace(a, b)` binds to
the **char** overload when an argument is one character long, so the exception and the `$null` are easy to hit; never write
a record file from a variable produced by a statement that can throw - guard it (`if($null -eq $t) { throw }`) and prefer
the `editor` tool for record edits.

One reading error of my own, caught by the verifier rather than by eye: `_tmp_s27_verify.ps1`'s first pass reported **6
bindings in a 5-wrapper TU**, because it counted every line mentioning `qor_pp_useswinapi` - including the contract block
that quotes the macro in prose. The count is now taken over code lines only (122 lines = 55 code + 67 comment, 5 defs,
5 bindings, 0 tabs, 5 tight `Call<>`). Same lesson as the U14 census stripping comments before it counts: **any TU measure
must exclude the contract block it is standing inside**, and the verifier output
(`tmp/_tmp_s27_verify.txt`, 9 sections) is what the close-out numbers in row 27 come from.

---

**Session 28 close-out (`rectangle.cpp`, 3 Nov 2026).** Record bookkeeping: rows **25-26** sliced to
`archive/session-log-25-26.md` (10 lines / 5,534 B, each row sha256-matched against
`tmp/_tmp_s28_preslice_sessionlog.md` before the rewrite - the live file was 38,226 B and row 28 is ~4.5 KB), the
placeholder corrected `28..38` -> `29..38` (**9 TUs left**) and the title line now points at all three slices.
`tmp/_tmp_s28_logslice.ps1` prints the proof (rows located at lines 39-40, both matched, 33,452 B after).

Three process faults of my own, all caught before they reached a record, recorded because each one is reproducible:
**(1) a whitespace pass that names its target lines breaks the moment a contract block is inserted above them.** The
first `_tmp_rect_edit_s28.ps1` carried `$targets = @(20, 27, ...)`; after the block those same 11 call sites were at
`:101`-`:170`, so the script would have "fixed" 11 prose lines. It is now self-locating (every `Library::Call` line
from the first `BOOL User32::` on) and asserts that exactly 11 lines moved. **(2) order-dependent steps must not go in
one parallel batch** - a batch that held `git checkout`, that edit script and the contract-block splice ran them
concurrently, so the splice read the file the checkout had just restored. **(3) same class, records:** the register
index generator, the `status.md` locator splice and the locator verifier went in one batch, so the splice read the
*previous* session's index file. Run sequentially: `12:35` splice gave 59 lines (stale), the re-run gave 63 and the
verifier then reported **62/63 pointing right + the single deliberate `U27` exception**.

One instrument limitation found, not fixed: `_tmp_rect_region_s28.ps1` reports `GetWindowLongA`/`GetWindowLongW` as
"no member" because `user32.h` declares the family only as `GetWindowLongT` (`:625`). A coverage scan must resolve
`stem -> stemT` before calling a name unwrapped; `_tmp_rect_family_s28.ps1` does, `_tmp_pt_coverage_s27.ps1` partially
does (`gotchas.md`).
