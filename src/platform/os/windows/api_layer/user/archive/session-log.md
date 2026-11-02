# session log table rows 20-23 + closure notes

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

| 20 | `keyboard.cpp` | 34 | **3 run-time-breaking binding defects fixed** - the folder's last three `*T`-stem bindings: `GetKeyboardLayoutNameT` (def `:82`), `VkKeyScanT` (`:243`), `VkKeyScanExT` (`:250`) used bare `qor_pp_useswinapi`, so the member's own `T` suffix stringized into the requested name (`kernel32.h:50`) and `GetProcAddress` was asked for `"GetKeyboardLayoutNameT"` etc.; the export table has **0 names ending in an uppercase `T`**, so all three cached a NULL `pFunc` and threw on first call (P1, G5). All three are real A/W families (`16D/16E`, `3E1/3E4`, `3E2/3E3`), so the fix is `qor_pp_useswinapiAW` over the bare stem; post-fix `/EP` = 27 bare + 7 `*W` literals (`tmp/_tmp_kb_bindings_s20_post.txt`) and the live probe resolves **all 34** (`tmp/_tmp_kb_probe_s20.txt`). Section shape: `user32.h:70-104` = 34 declarations, all `static`, all defined here in header order (0 U4, 0 U11, 0 U13); sigcmp decl==def 34/34, 0 parameter-name divergences (folder zero holds); **0 U17** - all 7 A/W members carry `T`. **U18 2 divergences recorded, not changed**: `GetKeyboardLayoutList` `UINT` vs SDK `int` (`winuser.h:1313`), `keybd_event` `void*` vs `ULONG_PTR` (`:6071`) - same width, published surface. **U14(b) 2 of the folder's 6 deducing sites fixed** (`GetKeyboardLayout`, `GetKeyboardLayoutList`) -> folder now 424 explicit / 4 deducing; **U14(c)** `GetKBCodePage()` -> `(void)` both sides -> folder 37/1 defs, 37/1 decls, sole survivor `message.cpp InSendMessageEx`. **U21**: 7 of the 34 are SDK *Window* entry points parked in the keyboard section, none in `//Window functions` - recorded, not moved. **U16 16 of 34** version-guarded, 18 Desktop-only. **U15 29 of 34 unreachable**; **U19 5 LAYER / 7 INDIRECT / 11 BYPASS / 2 MEMBER** - the folder's largest bypass count, and **every bypass is in a target that is not built**: `ui/gl/session.cpp` in no source list, `winqor_gl`'s `add_subdirectory(gl)` commented out at `ui/CMakeLists.txt:4`, `retropad`'s commented out at `examples/.../windows/CMakeLists.txt:8`; 1 of the 11 BYPASS rows is a member declaration (`windowcontroller.h:310`), 2 INDIRECT are Linux. **New defect class found by re-running the folder-wide AW checklist: `qor_pp_useswinapiAW` on a stem with no A/W pair** - `mouse.cpp` reads `bare=0 AW=10 single-ok=0`, i.e. **all 10 of its wrappers ask for a `*W` that does not exist** (added to P1/census; its session becomes a whole-TU fix). Post-fix folder census: 428 bindings, 17 script-unresolvable = 11 truly broken (mouse 10 + `window.cpp:124`) + 6 P2 | Fixed: the 3 bindings + 2 `Call<>` template lists + `(void)` (`user32.h:77`, def) + 7 stray inner spaces + a **61-line TU comment** (`:16-76`) covering D13, the KL_NAMELENGTH/cbSize/dead-key contracts, the calling-thread input-queue semantics, U16 and the not-built bypass chain. File 254 -> 314 lines, pure ASCII, 0 tabs, 0 trailing whitespace, CRLF. `user32.h` touched at 1 line, still **695 lines**. Gate: this TU `/W4` exit 0, folder **38/38 exit 0**, 0 diagnostics |
| 21 | `mdi.cpp` | 4 | **0 binding / correctness / UB / convention-surface defects; 1 internal `Call<>` type contradiction fixed** - the "Multi Document Interface functions" section (`user32.h:494`, declarations `:495-498`) is 4 declarations, 4 definitions, in header order, all `static`, all `User32::`-qualified, `Call<>` explicit 4/4 (spaced style); all four bindings were already D13-correct, verified three ways: `cl /EP` emits exactly four literals `"CreateMDIWindowW"`, `"DefFrameProcW"`, `"DefMDIChildProcW"`, `"TranslateMDISysAccel"` (`tmp/_tmp_mdi_pp.txt:453320`, `:453327`, `:453334`, `:453341`); the export table has the three families as A/W-only (`6F`/`70`, `A1`/`A2`, `A3`/`A4`) with **no bare stem export** and `TranslateMDISysAccel` as a genuine single export (`3B9`), so the three `AW` bindings are **load-bearing** - they would resolve today only because the SDK macros `#define CreateMDIWindow CreateMDIWindowW` (`winuser.h:12230`-`:12234`), `DefFrameProc` (`:12147`-`:12151`) and `DefMDIChildProc` (`:12179`-`:12183`) are live inside the binding expression, exactly the reliance G1 forbids; and a live probe (`tmp/_tmp_mdi_probe_s21.txt`) resolves all four names and reports MISSING for the bare stems and the `*T` spellings (the probe had to `LoadLibraryW` first: `GetModuleHandleW(L"user32")` fails with gle=126 in a bare console process - a direct G5 measurement). Signatures **and parameter names** match `winuser.h` field-for-field, so **0 U18**; 0 U17 (all three A/W members carry `T`, census re-run unchanged 525/108/404/11/2); 0 U11, 0 U13; **U16 0 of 4** version-guarded - all four under `#ifndef NOMDI` (`:12074`-`:12261`) + the Desktop partition (`:12092`), `TranslateMDISysAccel` also inside `#ifndef NOMSG` (`:12185`-`:12194`), measured with `tmp/_tmp_mdi_guards_s21.ps1`; **U21 third instance** - the SDK's `NOMDI` block holds **seven** entry points and this section owns four, the other three (`ArrangeIconicWindows` `:12199`/`E`, `CascadeWindows` `:12249`/`24`, `TileWindows` `:12240`/`3AE`) being declared in `//Window functions` (`user32.h:135`, `:139`, `:208`) and defined in `window.cpp:58`, `:86`, `:572` - recorded, not moved; **U15 4 of 4 unreachable**; **U19 1 LAYER / 1 INDIRECT / 0 BYPASS / 0 MEMBER** - the first family of the review with **0 SDK bypass**, consistent with there being no MDI host anywhere in the tree | Fixed: **1 `Call<>` template argument** - `DefFrameProcT`'s third type `INT` -> `UINT`, contradicting both its own `UINT uMsg` (`user32.h:496`, def `:91`) and the SDK (`:12132`, `:12141`); the **only uppercase `INT` token in any `Call<>`/`voidCall<>` list in the folder** (new instrument `tmp/_tmp_int_sites_s21.ps1`: 1 of 435 `Call<>`/`voidCall<>` template lists over raw source text - `tmp/_tmp_callcount_s21.ps1` reconciles that denominator against the census' 428 code-line bindings - now 0) and the only line `tmp/_tmp_sigcmp.ps1` flags in this TU. No ABI effect (same width, same slot, ids far below `int`'s sign bit), so a U14 convention fix, not a run-time one. + a 67-line TU comment (`mdi.cpp:16-82`); file 44 -> 112 lines. **New finding, recorded not fixed (U26 third mirror-struct instance):** the family's only compiled call site (`messagehandler.cpp:241`) passes a `Message*` (`message.h:33-44`) to the export as `::LPMSG`, and `tmp/_tmp_msg_mirror_s21.cpp`/`_tmp_msg_mirror_s21.bat` - the review's first two-architecture layout measurement - show the mirror is byte-identical to `::MSG` on **x64** (48 bytes, same offsets to `pt.y`) and divergent from `lParam` onward on **x86** (`MSG` 28 bytes `lParam@12 time@16 pt@20` vs `Message` 40 bytes `lParam@16 time@24 pt@28`); the same mirror feeds **7 call sites across 6 wrappers** (`messagehandler.cpp:136,:146,:181,:236,:241`, `windowcontroller.cpp:658`, `acceleratortable.cpp:83`), latent while only the `x64-Debug` profile exists, and handed to `message.cpp` (session 23). |
| 22 | `menu.cpp` | 39 | **0 binding / correctness / UB defects after 2 D13 rewrites; 0 return-type divergences, 3 parameter-type divergences.** The whole `user32.h:214 //Menu functions` section (decls `:215-253`, defs `:82`-`:348`, header order): all 39 decl/def pairs parse with **0 signature flags**, 0 U4 (all 39 static and defined here), 0 U13, 0 U17 (all 9 A/W members carry `T`), and every `Call<>` site is explicit. Binding: 9 stems are SDK A/W families and bind through `qor_pp_useswinapiAW`, 30 bind bare - and `tmp/_tmp_menu_class_s22.ps1` proves each of those 30 is a **genuine single export** (bare present, no `<stem>A`/`<stem>W`, and no `<stem> -> <stem>W` macro in `winuser.h`), so the folder's largest bare block is correct rather than suspicious. Two of the nine were bare: `GetMenuString` `:225`, `InsertMenuItem` `:260` - the **P2** pair. **Behaviour-neutral**: pre-fix `/EP` already emitted `"GetMenuStringW"`/`"InsertMenuItemW"` because the SDK macro rewrote the stem before stringizing (G1), and post-fix emits the same two literals by `##W` pasting. | Fixed: those 2 bindings -> `qor_pp_useswinapiAW`; folder census **17 -> 15** unresolvable (`tmp/_tmp_p1census_post_s22.txt`), no `menu.cpp` line left in it, and `/EP` over the TU gives 39 literals, all 39 in the export table. New 66-line TU comment, file 289 -> 355 lines. Registers: **U18** new measurement - the SDK return type sits on its **own** line above the stem in `winuser.h`, so a declaration-line-only instrument cannot see it; new `tmp/_tmp_menu_ret_s22.ps1` reads it and finds **0 of 39** return-type divergences but 3 parameter ones: `TrackPopupMenu`'s `HWND prcRect` vs the SDK's `_Reserved_ CONST RECT*` (unreachable, so latent) and `InsertMenuT`/`ModifyMenuT`'s `void* uIDNewItem` vs `UINT_PTR` (both live; `AppendMenuT` in the same section has it right). All published header surface, so recorded not changed, and the `Call<>` lists at `:254`/`:296` inherit them. **U16** 12 of 39 guarded (9 at `WINVER >= 0x0400`, 3 at `0x0500`) - the largest single-TU count; **U15** 4 of 39 unreachable (`GetMenuCheckMarkDimensions`, `IsMenu`, `LoadMenuT`, `TrackPopupMenu`); **U19** 40 LAYER / 10 INDIRECT / 18 BYPASS rows and 6 MEMBER - 3 BYPASS rows are controller *declarations* the classifier mis-reads (`i_m/menu.h:95`, `windowcontroller.h:226`, `:229`), the 15 real ones all in `retropad.cpp`, whose subdirectory is commented out at `examples/.../windows/CMakeLists.txt:8` - **0 bypass in built code**; **U21** fourth instance (`#ifndef NOMENUS` `winuser.h:7432-8101` = 38 stems, plus `CheckMenuRadioItem` in a second NOMENUS region at `:10509`; `IsMenu`/`GetMenuBarInfo` pair outside it; `ChangeMenuA/W` + `CalculatePopupWindowPosition` declared nowhere; the context-help pair in `//Shell functions` = the **T4** pair, still open, both halves re-measured as real exports ordinals `34A`/`17C`). Gate **38/38** at `/W4`, exit 0, 0 diagnostics. Hygiene ran first: `session-log.md` 45.4 KB -> 30.3 KB via two hash-verified slices. |
| 23 | `message.cpp` | 24 | **0 binding / correctness / UB defects after 4 D13 rewrites; 0 return-type and 0 parameter-type divergences.** The whole `user32.h:289 //Message and Message Queue functions` section (decls `:290-313`, defs `:106`-`:267`, header order): all 24 decl/def pairs parse, 0 U4, 0 U13, 0 U17 (all 12 A/W members carry `T`), every `Call<>` site explicit and tight. Binding: 12 stems are SDK A/W families, 12 bind bare and `tmp/_tmp_msg_class_s23.ps1` proves each of those 12 a **genuine single export** (bare present, no `<stem>A`/`<stem>W`, no `winuser.h` macro). Four of the twelve were bare - `RegisterWindowMessage` `:214`, `SendMessageTimeout` `:235`, `SendMessage` `:242`, `SendNotifyMessage` `:249` - i.e. the **entire remaining P2 list**, and the conversion is **behaviour-neutral**: pre-fix `/EP` already emitted the four `"*W"` literals (G1), post-fix emits the same four by `##W` pasting. | Fixed: those 4 bindings -> `qor_pp_useswinapiAW`, **P2 now CLOSED (0 open)**; folder census **15 -> 11** unresolvable (`tmp/_tmp_p1census_post_s23.txt`) with no `message.cpp` line left, `/EP` over the TU gives 24 literals, all 24 exported. Also fixed: U14(c)'s last zero-argument `()` - `InSendMessageEx` def `:176` + `user32.h:300` -> `(void)`, closing U14(c) (folder 38 defs / 38 decls all `(void)`); and 3 binding-macro comma-space nits `:19`, `:26`, `:68` (folder spacing now 414 spaced / 12 not, all 12 in `scrollbar.cpp`). New 90-line TU comment, file 184 -> 274 lines; `user32.h` edited in place, still **695 lines**. Registers: **P6 second instance** - bare `BroadcastSystemMessage` is exported (ordinal `14`, RVA `00092270` = the same RVA as `BroadcastSystemMessageA`) while `BroadcastSystemMessageW` is a different entry point at `00007240`, so a bare binding there would run the ANSI entry point on wide arguments instead of throwing; the TU's `AW` binding avoids it. **U18**: 0 of 24 return-type divergences via the session-22 instrument pattern extended to `winuser.h`'s own-line return type (`tmp/_tmp_msg_ret_s23.ps1`); the one deliberate signature deviation is `InSendMessageEx`, published with no parameter where the SDK has `_Reserved_ LPVOID lpReserved` (`winuser.h:4253`) - the `Call<DWORD, LPVOID>` list passes `nullptr`, so the call shape matches the export; both pre-flag `CALL TYPE MISMATCH` rows were instrument artifacts. **U16** 4 of 24 guarded (`SetMessageExtraInfo` WINVER 0x0400, `BroadcastSystemMessageEx*` `_WIN32_WINNT >= 0x0501`, `BroadcastSystemMessage*` WINVER 0x0400 + `defined(_WIN32_WINNT)`, `InSendMessageEx` WINVER 0x0500). **U19** 172 LAYER / 12 INDIRECT / 68 BYPASS / 2 MEMBER: all 24 wrappers have a through-layer site (first TU with no zero-site family; `SendMessageT` alone has 147), and all 68 bypass rows are in code no target builds (62 in `retropad`, commented out at `osspecific/windows/CMakeLists.txt:8`; 6 in `ui/gl/helperwindow.cpp`/`platform.cpp`, in no source list); 1 row is a false positive (`framework/event/events/eventsource.h:20` is a member declaration). **U21 fifth instance**: `NOMSG` opens 6x and `NOWINMESSAGES` 6x in `winuser.h` (`tmp/_tmp_msg_regions_s23.ps1`), this section covers only the `:3395`-`:3552`, `:1976`-`:2787` and the unguarded `:3650`-`:3900` neighbourhoods, `SetMessageQueue` (exported, ordinal `351`) is declared **nowhere** in the library, and the SDK's other message-region members are wrapped in four other TUs. **U25**: 0 `CONST` here, plus a fourth spelling - `const MSG*` with no `::` in a section that qualifies `::PBSMINFO`/`::SENDASYNCPROC`. **U26** hand-off picked up and re-measured for both arches: x64 `MSG`/`Message` identical (48 bytes, same offsets), x86 divergent from `lParam` on (`MSG` 28, mirror 40); the layer's signatures are SDK-exact so the repair is a `winqor_gui` header decision - recorded, not fixed. Gate: `tmp/_tmp_cl_all_s23recover.log` 38/38 at `/W4`, exit 0, 0 diagnostics, re-run after the last source edit (the `_s23final`/`_s23pre` copies the interrupted pass left are byte-identical to session 22's log; census note at `registers/census.md:101`). |



| 24..38 | not started (TU index; 14 left, see "Queue" in `status.md`) | | | |

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

