# session log table rows 23-25 + closure notes (rows 20-22 in `archive/session-log-20-22.md`)

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

| 23 | `message.cpp` | 24 | **0 binding / correctness / UB defects after 4 D13 rewrites; 0 return-type and 0 parameter-type divergences.** The whole `user32.h:289 //Message and Message Queue functions` section (decls `:290-313`, defs `:106`-`:267`, header order): all 24 decl/def pairs parse, 0 U4, 0 U13, 0 U17 (all 12 A/W members carry `T`), every `Call<>` site explicit and tight. Binding: 12 stems are SDK A/W families, 12 bind bare and `tmp/_tmp_msg_class_s23.ps1` proves each of those 12 a **genuine single export** (bare present, no `<stem>A`/`<stem>W`, no `winuser.h` macro). Four of the twelve were bare - `RegisterWindowMessage` `:214`, `SendMessageTimeout` `:235`, `SendMessage` `:242`, `SendNotifyMessage` `:249` - i.e. the **entire remaining P2 list**, and the conversion is **behaviour-neutral**: pre-fix `/EP` already emitted the four `"*W"` literals (G1), post-fix emits the same four by `##W` pasting. | Fixed: those 4 bindings -> `qor_pp_useswinapiAW`, **P2 now CLOSED (0 open)**; folder census **15 -> 11** unresolvable (`tmp/_tmp_p1census_post_s23.txt`) with no `message.cpp` line left, `/EP` over the TU gives 24 literals, all 24 exported. Also fixed: U14(c)'s last zero-argument `()` - `InSendMessageEx` def `:176` + `user32.h:300` -> `(void)`, closing U14(c) (folder 38 defs / 38 decls all `(void)`); and 3 binding-macro comma-space nits `:19`, `:26`, `:68` (folder spacing now 414 spaced / 12 not, all 12 in `scrollbar.cpp`). New 90-line TU comment, file 184 -> 274 lines; `user32.h` edited in place, still **695 lines**. Registers: **P6 second instance** - bare `BroadcastSystemMessage` is exported (ordinal `14`, RVA `00092270` = the same RVA as `BroadcastSystemMessageA`) while `BroadcastSystemMessageW` is a different entry point at `00007240`, so a bare binding there would run the ANSI entry point on wide arguments instead of throwing; the TU's `AW` binding avoids it. **U18**: 0 of 24 return-type divergences via the session-22 instrument pattern extended to `winuser.h`'s own-line return type (`tmp/_tmp_msg_ret_s23.ps1`); the one deliberate signature deviation is `InSendMessageEx`, published with no parameter where the SDK has `_Reserved_ LPVOID lpReserved` (`winuser.h:4253`) - the `Call<DWORD, LPVOID>` list passes `nullptr`, so the call shape matches the export; both pre-flag `CALL TYPE MISMATCH` rows were instrument artifacts. **U16** 4 of 24 guarded (`SetMessageExtraInfo` WINVER 0x0400, `BroadcastSystemMessageEx*` `_WIN32_WINNT >= 0x0501`, `BroadcastSystemMessage*` WINVER 0x0400 + `defined(_WIN32_WINNT)`, `InSendMessageEx` WINVER 0x0500). **U19** 172 LAYER / 12 INDIRECT / 68 BYPASS / 2 MEMBER: all 24 wrappers have a through-layer site (first TU with no zero-site family; `SendMessageT` alone has 147), and all 68 bypass rows are in code no target builds (62 in `retropad`, commented out at `osspecific/windows/CMakeLists.txt:8`; 6 in `ui/gl/helperwindow.cpp`/`platform.cpp`, in no source list); 1 row is a false positive (`framework/event/events/eventsource.h:20` is a member declaration). **U21 fifth instance**: `NOMSG` opens 6x and `NOWINMESSAGES` 6x in `winuser.h` (`tmp/_tmp_msg_regions_s23.ps1`), this section covers only the `:3395`-`:3552`, `:1976`-`:2787` and the unguarded `:3650`-`:3900` neighbourhoods, `SetMessageQueue` (exported, ordinal `351`) is declared **nowhere** in the library, and the SDK's other message-region members are wrapped in four other TUs. **U25**: 0 `CONST` here, plus a fourth spelling - `const MSG*` with no `::` in a section that qualifies `::PBSMINFO`/`::SENDASYNCPROC`. **U26** hand-off picked up and re-measured for both arches: x64 `MSG`/`Message` identical (48 bytes, same offsets), x86 divergent from `lParam` on (`MSG` 28, mirror 40); the layer's signatures are SDK-exact so the repair is a `winqor_gui` header decision - recorded, not fixed. Gate: `tmp/_tmp_cl_all_s23recover.log` 38/38 at `/W4`, exit 0, 0 diagnostics, re-run after the last source edit (the `_s23final`/`_s23pre` copies the interrupted pass left are byte-identical to session 22's log; census note at `registers/census.md:101`). |



| 24 | `monitor.cpp` | 5 | **0 defects of any class - the cleanest TU of the review; 0 fixes.** The whole `user32.h:587 //Monitor functions` section (decls `:588`-`:592`, defs `:127`-`:155`, header order): 5 decl/def pairs parse, `decl == def` 5/5, 0 U4, 0 U11, 0 U13, 0 U17 (`GetMonitorInfoT` is the only A/W member and carries `T`), all 5 `Call<>` lists explicit and self-consistent (U14b), **0 return-type and 0 parameter-type divergences** against `winuser.h:13607,:13614,:13621,:13684,:13690,:13704`, and the parameter names match the SDK's. Binding: 4 bare (`EnumDisplayMonitors` `FE`, `MonitorFromPoint` `28E`, `MonitorFromRect` `28F`, `MonitorFromWindow` `290` - each a genuine single export, no A/W spelling, no `winuser.h` macro) + 1 `AW` (`:137 GetMonitorInfo`, family `18C`/`18D`, **no bare export**), so the `AW` is load-bearing: bare would `GetProcAddress("GetMonitorInfo")` -> NULL cached for the process lifetime (G5+U5). `tmp/_tmp_mon_class_s24.ps1` per stem; live probe `tmp/_tmp_mon_probe_s24.cpp` -> `_tmp_mon_probe_s24.txt`: 5/5 `RESOLVED`, bare `GetMonitorInfo` + every `*T` + `GetMonitorInfoExW/A` `MISSING`, and `GetModuleHandleW` gle=126 before `LoadLibraryW` (G5). **0 P1, 0 P2, 0 P5, 0 P6**; folder census unchanged 11 of 428 with no `monitor.cpp` row. | Only change: a **111-line TU comment** (`:15`-`:125`) - file 51 -> **162 lines**, `user32.h` untouched at **695**. `/EP` pre-vs-post differ by **111 lines, all blank, 0 removed, 0 non-blank added** (`tmp/_tmp_mon_pp_pre_s24.txt` 453,286 -> `tmp/_tmp_mon_pp_post_s24b.txt` 453,397; the `_post_s24.txt` capture predates the last edit and is not the post state), the cheapest proof that a comment block is behaviour-neutral. Registers: **U16 6 of 6** - the first TU whose *entire* section shares one guard (`#if(WINVER >= 0x0500)` at `winuser.h:13594` over the Desktop partition `:13605`/`:13638`), so a `WINVER` pin drops the whole section at once; no `WINVER`/`_WIN32_WINNT`/`NTDDI_VERSION` anywhere in the tree (0 hits), no pin written. **U21 sixth instance:** no `NOMONITOR` macro exists at all, and the guard owns `:13594`-`:14905` = **45 declaration heads**, 6 monitor - the other 39 (DPI contexts, the `*Info` family, the WinEvent-hook family, `GetAltTabInfo`, the `Real*` pair, `GetAncestor`, `BlockInput`, `InheritWindowMonitor`) sit in other sections - **measured, not guessed: 17 wrapped across seven other TUs (`window.cpp` 8, `accessibility.cpp` 4, one each in `combobox`/`cursor`/`keyboard`/`menu`/`scrollbar`) and 22 with no wrapper anywhere** (`tmp/_tmp_mon_region_s24.ps1` -> `_tmp_mon_region_s24.txt`; head list: section (b) of `tmp/_tmp_mon_pre2_s24.txt`), and one of the 22, `GetListBoxInfo`, is declared in this library at `user32.h:548` and defined in no TU at all - a **U4 hand-off**; section label and contents agree, so the instance is one-directional. **U19 8 LAYER / 0 INDIRECT / 4 BYPASS / 0 MEMBER** and **the first bypass in built code**: `i_m/monitor.cpp:163` calls the `GetMonitorInfoW` thunk in the *same* file that calls `User32::GetMonitorInfoT` at `:56`/`:62` (built at `ui/gui/CMakeLists.txt:25`); the other 3 bypass rows are unbuilt `winqor_gl` (`ui/gl/monitorimpl.cpp:79`, `windowimpl.cpp:435,:1902`). All 5 wrappers live through the layer. **U26 fourth instance - the safe half:** the published signatures name no `TCHAR` type, so the width cannot propagate; `MonitorInfo` mirror == `MONITORINFO` (40 B, same offsets) on **x64 and x86**, `MonitorInfoEx` == `MONITORINFOEXW` (104 B, `szDevice@40`) in the built tree, and the narrow-world mirror would be 72 B against a 104-B write; `Point`/`Rect` == `POINT`/`RECT` on both arches, so the by-value `POINT` (`:141`) is sound; the one x86 divergence is the caller's `MonitorEnumProc` taking `long long` where `MONITORENUMPROC` takes `LPARAM` (8 vs 4) - `winqor_gui` surface, recorded not fixed. **U25**: 0 `CONST`, fifth spelling (same declaration qualifies `::MONITORENUMPROC`/`::LPMONITORINFO`/`::POINT` and leaves `LPCRECT` bare). 0 U15 instances (D14). Gate: TU `/W4` exit 0, folder **38/38 exit 0, 0 diagnostics** (`tmp/_tmp_cl_all_s24b.log`, logged 19:08:42 after the 18:55:06 final edit; the 18:22 `_tmp_cl_all_s24.log` predates that edit and is not the gate of record), `cl /EP` exit 0 (`_tmp_mon_pp_post_s24b_err.txt` = the TU name only) |

| 25 | `mouse.cpp` | 10 | **10 run-time-breaking binding defects + 1 wrong-stem binding + 1 deducing call site fixed** - all ten bindings were `qor_pp_useswinapiAW` over stems with **no A/W family at all**: `user32.dll` exports only the bare names (`CB`,`125`,`152`,`18E`,`3F5`,`2FE`,`320`,`339`,`3A4`,`3B3`) and `winuser.h` defines **no `#define <stem>` macro** for any of them, so `##W` (`kernel32.h:52`) asked `GetProcAddress` for `DragDetectW` ... `LPTRACKMOUSEEVENTW` - 10 names the DLL does not have, each caching a NULL `pFunc` for the process lifetime (U5) and throwing from `library.h:50`-`:53` on every call (P1; the **inverse** of the P2 class). Pre-fix `/EP` emits exactly those 10 literals (`tmp/_tmp_mouse_pp_pre_s25.txt`, pp 453254-453317); the live probe says all 10 **MISSING**, all 10 bare stems **RESOLVED** at the table RVAs, and `TrackMouseEventW`/`A`/`T` + `DragDetectA` **MISSING** (`tmp/_tmp_mouse_probe_s25.cpp` -> `_s25.txt`; the probe needs `LoadLibraryW` - `GetModuleHandleW` gle=126, G5). Fix: bare `qor_pp_useswinapi` at `:100`,`:107`,`:114`,`:121`,`:128`,`:135`,`:142`,`:149`,`:156`,`:163` - for these names the **only** spelling that can ever bind, so D13's other half, not a style call. **P5's second instance** went in the same pass: the old `:82` bound the parameter **type** `LPTRACKMOUSEEVENT`, so folder wrong-stem rows **4 -> 3, 0 live** (`cursor.cpp:64` = P3, `resource.cpp:19`/`:26` = U11). **U14(b)**: the `TrackMouseEvent` site is now `Call< BOOL, LPTRACKMOUSEEVENT >` (`:164`) -> folder **425 explicit / 3 deducing**, survivors `window.cpp:118`,`:254`,`:317`; folder spacing unchanged 178 spaced / 250 tight. Section shape: `user32.h:555`-`:565` = 10 declarations, all `static`, all defined here in header order (now `:97`-`:164`), 0 U4 / 0 U11 / 0 U13; **0 signature and 0 parameter-name divergences** over 10/10 (`tmp/_tmp_namediv_pre_s25.txt`) - the folder's zero-divergence record holds. **U16 3 of 10** prototypes guarded, and `winuser.h`'s `#endif` **comments in that stretch are crossed** (`:2756` closes the `#ifndef NOTRACKMOUSEEVENT` at `:2744`, not the version switch) - read the guard stack, not the comments; also this item's **first `DESKTOP \| PC_APP`** partition instance (`:6892`, `GetCapture`/`SetCapture`/`ReleaseCapture`). **U17 0** (no family, so no `T` owed; folder census 525/108/404/11+2 unchanged), **U25 0**, **U26 0** (no `TCHAR` parameter and no `TCHAR` field in `TRACKMOUSEEVENT`/`MOUSEMOVEPOINT`), **U19 9 LAYER / 7 INDIRECT / 0 BYPASS / 0 MEMBER** unchanged since session 20 - but all 9 LAYER sites are in built `winqor_gui` code and every one used to throw, so the LAYER column was nominal. Contracts: 6 failure-indicating, 3 state-not-status (`SwapMouseButton`, `GetCapture`, `DragDetect`), `mouse_event` returns nothing; two caller `cbSize`s and `GetMouseMovePointsEx`'s `GMMP_USE_*_POINTS` are unvalidatable at this layer. | Fixed: the 10 bindings + 1 wrong stem + 1 `Call<>` list (**11 lines, no other line touched**) + an 81-line TU comment block; TU 86 -> **167 lines**, 0 tabs, 0 trailing-space lines, pure ASCII, longest 144, CRLF. `user32.h` untouched, still **695 lines**. Folder binding census **11 -> 1 unresolvable of 428** (`tmp/_tmp_p1census_pre_s25.txt`, `..._post_s25b.txt`). Gate `tmp/_tmp_cl_all_s25c.log`: 38/38 at `/W4`, exit 0, 0 diagnostics, logged 22:18:27 **after** the final edit at 22:14:54 (`_s25b.log` 22:06:32 predates it); `/EP` post3 453,402 vs pre 453,321 = **0 removed, 81 added, every added line blank**. Narrative `archive/tu/mouse.md`, row 25. |
| 26..38 | not started (TU index; 12 left, see "Queue" in `status.md`) | | | |

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
(`tmp/_tmp_cl_all_apiall_u6_pre.log` / `..._post.log`) - see "Off-protocol item fix (U6)" in `method.md` §1 for the
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
row 25 and this note it is **259 lines / ~35.5 KB** - 4.5 KB under the ceiling, so **session 26 must slice again** (rows
23-24 are the natural candidates) before its row goes in.

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

