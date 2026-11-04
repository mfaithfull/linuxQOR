# session log table rows 29+ + closure notes (rows 20-22 in `archive/session-log-20-22.md`, rows 23-24 in `archive/session-log-23-24.md`, rows 25-26 in `archive/session-log-25-26.md`, rows 27-28 in `archive/session-log-27-28.md`, close-out notes for sessions 24/25/29/30 in `archive/session-log-notes-24-30.md`)

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

### Rows 27-28 moved out

Rows 27-28 (`processthread.cpp`, `rectangle.cpp`) and their close-out notes are kept verbatim in
`archive/session-log-27-28.md` - moved on 3 Nov 2026 when the file passed 40 KB with row 29. Each row and every note
line was sha256-matched against `tmp/_tmp_s29_preslice_sessionlog.md` before the rewrite.

| 29 | `resource.cpp` | 2 | **1 real defect - the folder's only U11 pair: both definitions were free functions, so the two `static` members at `user32.h:443`-`:444` had no definition anywhere (U4 x2) while the two free functions were never published** (built DLL 439 exports / import library 854 member symbols, neither name present). Fixed in-session by qualifying both with `User32::`; proven three ways - before/after `dumpbin /symbols` (`YA` free -> `SA` static member), the wrong-stem scan's folder rows 3 -> 1, and a consumer-side link probe (`tmp/_tmp_res_ref_s29.cpp`) with **0** unresolved for either member. Bindings, signatures and return policy already correct: 0 P1/P2/P6, 0 divergence x2, both `_T`-suffix shapes right. Also 2 whitespace nits (`int , UINT`, and the folder's **last** `>( pFunc` site -> census 421 tight / 0 spaced) and a 10-line contract block - this was the last reviewed TU without one. **U11 CLOSED**; U4's `static`-declaration direction now **0** folder-wide. Narrative `archive/tu/resource.md` |
| 30 | `scrollbar.cpp` | 12 tokens + 1 contract block (102 -> 156 lines) | **0 defects.** All 12 stems are bare single exports, so P1/P2/P6 cannot arise; signatures clean 12/12 (`_tmp_sigcmp.ps1`, its one `CALL TYPE MISMATCH` being the new `const ::RECT *p` vs `const ::RECT*` **pointer-spacing** artifact); 0 header-vs-definition name divergence, 4 def-vs-SDK name divergences over 3 members (U18, published names); every `Call<>` tight. **One defect class fixed:** all 12 bindings spelled the binding macro's module token `User32` - the folder's last such site (12 of 428 bindings; G4's cited co-offender `combobox.cpp` no longer spells it, so G4's population was stale). Proved behaviour-neutral three ways: `/EP` pre/post differ on **exactly 12 lines**, each only that literal; an x64+x86 probe where `user32`/`User32`/`USER32`/`user32.dll` resolve to one `HMODULE`, a bogus name to NULL, and **0 of 12** entry points differ by module spelling; code-only `/EP` output **SHA-256 identical** across the contract-block insertion. Folder census after the fix: **428 bindings, 428 `user32` tokens, 0 non-lowercase sites.** U16: 12/12 DESKTOP-partition, only 2 version-guarded - `GetScrollInfo`/`SetScrollInfo` under `#if(WINVER >= 0x0400)` (`winuser.h:12026`), the item's **first 0x0400 gate** and its first `NOSCROLL`/`NOWINABLE` guard names. U21: 13 "Scroll" exports, 12 wrapped; the 13th `ScrollChildren` is exported but **undeclared in the SDK**, so not wrappable. U19: 8 LAYER / 8 "INDIRECT" / 0 BYPASS / 0 MEMBER, all 8 INDIRECT being the caller's own same-named member definitions, and nothing in-tree constructs `ScrollBar` - a latent façade chain. U26: the caller mirror `ScrollInfo` is **layout-identical to `SCROLLINFO` on both arches** (28 bytes, all 7 offsets), but `nPos`/`nTrackPos` alone lack a default initializer, so a default-constructed struct carries an indeterminate `int` through `GetScrollInfo`/`SetScrollInfo`. U2: 0 checks, 0 `_Check_return_`, and all 4 `int`-returning members have 0 as a legal value. T7 **35 -> 36** (`scrollbar.cpp:11`). Gate: 38/38, exit 0, `scrollbar.obj` newer than the source. Narrative `archive/tu/scrollbar.md` |
| 31 | `systemshutdown.cpp` | 5 | **1 defect class fixed:** `:37`/`:44` were the folder's only two **`WINAPI`-qualified `User32::` definition heads**, disagreeing with the unmarked declarations at `user32.h:605`/`:606` - **ill-formed for x86** (`C2373` x2) and a **silent no-op for x64** (identical `__cdecl` decoration on both sides of a caller probe, `dumpbin /disasm` byte-identical, `/EP` non-blank lines 154,302 both sides). 5/5 bare single exports (D13-correct), 5/5 `decl==def` including parameter names, **0 U18 traffic**, U16 5/5 DESKTOP with **1** version gate (`LockWorkStation` `_WIN32_WINNT >= 0x0500`; the `ShutdownBlockReason` trio has **no** floor - refutes the 0x0601 pre-flag lead), **0 callers of any class** for all 5 stems (D14 - no U19/U15 row), T7 36 -> 37. Opened a folder-wide measure in `registers/u6.md`: **x86 is unbuildable - 93 of 106 api_layer TUs fail on `kernel32.h:92` `PUNWIND_HISTORY_TABLE`**, the 13 survivors being exactly the `winqor_api_*.cpp` glue TUs. Narrative `archive/tu/systemshutdown.md` |
| 32 | `text.cpp` | 4 | **0 defects, 0 code edits** (contract block only; neutrality proved by `/EP`: **154,296 non-blank lines before and after, +81 comment lines (453,279 -> 453,360 total), 0 non-blank differences**, 4 W literals unchanged, 0 bare and 0 `…WW`). The folder's **only TU where all four bound stems have the SDK A/W macro live at the binding site** (`winuser.h:8269`/`:8331`/`:8470`/`:8494`) - `qor_pp_useswinapiAW` pastes `W` onto a bare name before that name can expand, so P1/P2/P6 are **correct by construction** and the `…WW` exports a double-suffix would need do not exist. Header range corrected to `user32.h:568-:571`; `GetTabbedTextExtent` returns `DWORD` on both sides (**refutes** the `SIZE` lead). **U23 gains a second class**: `CONST LPINT` on the tab-stop parameter is `INT* const`, not the SDK's `CONST INT *` - probe `tmp/_tmp_s32_u23_probe.cpp` compiles, negative control `_neg.cpp` gives `C2664`; plus the `DT_MODIFYSTRING` `_Inout_` +4-characters buffer contract (`winuser.h:8250`, `:8309`) that `LPCTSTR` hides. U16 4/4 DESKTOP, 1/4 version-gated (`DrawTextExT` under `#if(WINVER >= 0x0400)`), new guard name **`NODRAWTEXT`**; 0 U4/U11/U13/U17/U18/U25/U26; U19 0 (the `DT_*` family is **comment-only in-tree**, caller passes only `DT_EDITCONTROL`); U15 0 by D14 (12 consumers in `ui/gui/painting/themedata.cpp`, 2 in `winqor_kernelobj.cpp`). Closed the comment-only token measure: `user32.h` 14 -> **12** (2 were live code), `text.cpp` 0 -> **12**. Carried **U12/U13 + U18** lead: `windowproperties.cpp:173` unqualified `SetLayeredWindowAttributes`, double-gated at `WINVER`/`_WIN32_WINNT >= 0x0600`, `DWORD` vs SDK `BOOL`. **Record repair:** `tmp/_tmp_s32_queue_audit.ps1` re-derived the queue from the session logs - the Queue table had a duplicated `text.cpp` row, no `windowprocedure.cpp` row, and `window.cpp` at 270 when it is **615** lines; 33/38 TUs now reviewed. Gate `tmp/_tmp_cl_all_s32.log`: 38/38 TU echoes, 0 errors, 0 warnings, 0 stale `.obj`. Narrative `archive/tu/text.md` |
| 33..38 | not started (5 TUs left, see "Queue" in `status.md`; next `window.cpp`, **85 wrappers over 615 lines**, census says 50) | | | |

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


## Session 24-30 close-out notes moved out

The close-out notes for sessions 24 (`monitor.cpp`), 25 (`mouse.cpp`), 29 (`resource.cpp`) and 30 (`scrollbar.cpp`),
plus the session 27-28 pointer, are kept verbatim in `archive/session-log-notes-24-30.md` - moved on 4 Nov 2026 when
this file passed the 40 KB ceiling. Moved block `SHA256 
09136B67A3331B040BA9A3CA6E01E4808FFCC9C073047C123A49AA549A064852
`; rejoin hash-verified before the move.

### Session 31 - `systemshutdown.cpp` (4 Nov 2026)

5 wrappers, `user32.h:601`-`:606` under the header's own misspelled label `//System Shutdwown functions`, 51 -> **116
lines** (contract block only). **One defect class, fixed:** `:37` and `:44` read `BOOL WINAPI User32::ShutdownBlockReasonDestroy`
/ `...Query` - the **only two `WINAPI`-qualified `User32::` definition heads in all 38 TUs**, while `user32.h:605`/`:606`
declare those members with no convention token. Removed. Not a style call: MSVC rejects the disagreement when the TU is
compiled **for x86** with `error C2373: 'qor::win::api::User32::ShutdownBlockReasonDestroy': redefinition; different type
modifiers` (same at `:44`), i.e. the TU was **ill-formed** - and on x64, the only arch the tree configures
(`CMakeSettings.json:4` `x64-Debug`, generator `Visual Studio 17 2022`), the modifier is dropped, so those two heads changed
nothing. Proof of neutrality, three ways: a caller probe (`tmp/_tmp_s31_winapi_probe_s31.cpp`) shows the symbol MSVC emits
and the symbol a consumer of the header declaration references are **the same `__cdecl` decoration** on both sides
(`?ShutdownBlockReasonDestroy@User32@api@win@qor@@SAHPEAUHWND__@@@Z`, `tmp/_tmp_s31_winapi_s31.txt`); `dumpbin /disasm` of
the object is **identical before and after** (42,310 lines each, 2 differing lines, both the dump header:
`tmp/_tmp_s31_x64_pre_s31.asm` / `_post_s31.asm`); and the `/EP` diff is **55 lines, of which 51 blank**, the other 4 being
only the two definition heads (`BOOL __stdcall User32::...` -> `BOOL User32::...`) - **non-blank lines 154,302 both sides**,
all 5 binding literals byte-identical.
**Binding (D13):** 5/5 bare single exports, no A/W family - `ExitWindowsEx` 10F, `LockWorkStation` 26C,
`ShutdownBlockReasonCreate` 39C, `ShutdownBlockReasonDestroy` 39D, `ShutdownBlockReasonQuery` 39E; a family walk over the
1,018-name table gives **exactly 1 export per stem** and the `um` headers carry **0 `#define <Stem>` alias sites**, so the
bare `qor_pp_useswinapi` at `:19, :26, :33, :40, :47` is the only spelling that can bind them (`/EP` emits exactly those
5 literals against `L"user32"`); 0 P1, 0 P2, 0 P6.
**Signatures:** 5/5 `decl==def` on return type, parameter types, parameter names and `Call<>` arity
(`_tmp_sigcmp.ps1 -Tu systemshutdown.cpp -First 602 -Last 606`), 0 name divergences of any kind - the first TU with no U18
name traffic at all.
**U16:** 5/5 prototypes are `WINAPI_PARTITION_DESKTOP`, **exactly one version-gated** - `LockWorkStation` under
`#if(_WIN32_WINNT >= 0x0500)` (`winuser.h:14912`); `ExitWindowsEx` (`:3639`, also inside `#ifndef NOUSER`) and the
`ShutdownBlockReason` trio (the `#pragma region Desktop Family` block opened `:15919`, which also defines
`MAX_STR_BLOCKREASON 256`) carry **no version floor**. This **refutes the session-30 U16 lead** ("expect the highest
version floor yet, likely 0x0601") - the trio has none, and no guard name outside the population already recorded appears.
**U19:** 0 callers anywhere - the comment-stripped scan over `src`, `examples` and `test` with all five stems prints
**0 LAYER / 0 INDIRECT / 0 BYPASS / 0 MEMBER**; published surface, so **no U19 or U15 row** (D14).
**U1/U2:** 5/5 return `BOOL`, 0 `Win32ErrorHandler`, 0 `CheckReturn`, and **0 `_Check_return_` on the 5 prototypes**;
`ShutdownBlockReasonDestroy`'s `FALSE` distinguishes `ERROR_SHUTDOWN_IN_PROGRESS` from `ERROR_NOT_FOUND`, so the
session-28 answer-shaped-`BOOL` caution applies. `:11` includes `../returncheck.h` and uses none of it - **T7 36 -> 37**.
**Build note (recorded in `registers/u6.md`, no new register):** `kernel32.h:92` names `PUNWIND_HISTORY_TABLE`, which the
SDK typedefs only under `#if defined(_AMD64_)||defined(_ARM_)||defined(_ARM64_)` (`winnt.h:21939`-`:21963`), so that one
header line makes **93 of the 106 api_layer TUs fail with C2061 on x86** (`tmp/_tmp_s31_x86_apiall_s31.txt`, digest
`tmp/_tmp_s31_x86_digest_s31.ps1`); the 13 survivors are the library glue TUs, and `kerrorhandling.cpp:104` then fails with
C2065 `'TargetGp': undeclared identifier`. The same x86 blind spot is what kept U6's stack corruption invisible until the
x86 probe caught it, and it is what hid this TU's C2373 pair from 30 sessions of x64-only gates.
**Gate:** single TU `cl_exit=0` (`tmp/_tmp_compile_one.ps1`); library `tmp/_tmp_cl_all_s31c.log` **38 echoes / 0
diagnostics**, dated by `.obj` freshness (all 38 newer than the edited source). Census unchanged: 428 bindings, 428
lowercase `user32` module tokens, call-parens 421 tight / 0 spaced, this TU contributes 0 spaced `Call< ` sites (U14 frozen,
D15). Narrative `archive/tu/systemshutdown.md`.
**Pre-flags for the next TU (`text.cpp`, 44 lines / 2,028 B, `user32.h:567`-`:571` under `//Font and Text functions`, 4
members `DrawTextT`, `DrawTextExT`, `GetTabbedTextExtentT`, `TabbedTextOutT`):**
- **All 4 bindings are `qor_pp_useswinapiAW` and all 4 stems are A/W-only with no bare export**: `DrawTextA` DB `00036A30`
  / `DrawTextW` DE `00024390`, `DrawTextExA` DC `00053630` / `DrawTextExW` DD `00023E50`, `GetTabbedTextExtentA` 1CB / W
  1CC, `TabbedTextOutA` 3AB / W 3AC; a word-boundary walk of the export table prints **`DrawText`, `DrawTextEx`,
  `GetTabbedTextExtent`, `TabbedTextOut` all count=0** - `AW` is D13-correct and P6 cannot arise (unlike `CallMsgFilter` /
  `BroadcastSystemMessage`).
- **The bare names exist only as SDK macros or a managed inline**: `#define DrawText DrawTextA` (`winuser.h:8271`,
  `!UNICODE`) plus `#if defined(_M_CEE)` `__inline int DrawText(` at `:8274`-`:8278`, while `DrawTextEx`,
  `GetTabbedTextExtent` and `TabbedTextOut` have **0 SDK declarations**. The binding macro stringizes the pasted name, so
  the macro never expands there - but consumers see the macro, which is a U26 include-order question for these 4 names.
- **`user32.h:571` spells the parameter type `CONST::LPINT`** (no space) where `:570` uses `CONST LPINT`, and `text.cpp:36`
  copies `CONST::LPINT` in the head but `CONST ::LPINT` in its `Call< >` list - a U14(a)-family token-spacing site,
  cosmetic only (U14 itself is frozen by D15).
- **Hygiene:** longest line **175**, 3 comment lines, **no contract block** - add one. Parse signatures with
  `_tmp_sigcmp.ps1 -Tu text.cpp -First 568 -Last 571` (the section head is `:567`; `-First 655` lands in
  `//Authorization functions` and parses nothing).
- **Return policy (U1/U2):** `int`, `int`, `DWORD`, `LONG`; `DrawText` returns the text height and `DrawTextEx` a line
  count or error code, so test the session-27 rule ("only sound where the failure value is not also a legal success value")
  against 0 before any check idea. 0 checks present, and `:11` includes `../returncheck.h` unused - **T7 37 -> 38** if left.
- **Expect U16 traffic:** the four prototypes sit at `winuser.h:8248`, `:8260`, `:8306`, `:8320`, `:8448`, `:8460`, `:8478`,
  `:8487` - region-walk the guards with the depth-counted walk, never a backward scan (`winuser.h:4121`).
