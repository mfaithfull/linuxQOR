# session log table rows 1-19 + closure notes

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
| 17 | `hook.cpp` | 4 | **0 binding / correctness / UB / convention defects** - the `//Hook functions` block (`user32.h:315-319`) is 4 declarations, 4 definitions, in header order, all `static`, all `User32::`-qualified, `Call<>` explicit 4/4 (spaced style); all 4 bindings were already D13-correct and are now verified three ways (`cl /EP` literal + export table + live probe): `CallMsgFilterT` -> `"CallMsgFilterW"`, `CallNextHookEx` -> `"CallNextHookEx"`, `SetWindowsHookExT` -> `"SetWindowsHookExW"`, `UnhookWindowsHookEx` -> `"UnhookWindowsHookEx"` (`tmp/_tmp_hook_pp.txt`); the two `AW` bindings are **load-bearing** (live probe: bare `SetWindowsHookEx` **MISSING**, `SetWindowsHookExW`/`A`/**`AW`** all RESOLVED) and the two bare ones are the D13 spelling (no `CallNextHookExA/W` or `UnhookWindowsHookExA/W` in the table); signatures and returns match the SDK exactly (`winuser.h:5401-5403`, `:10384-10388`, `:10399-10400`, `:10405-10409`), so **0 U18**; 0 U11, 0 U13, **0 U17** - the pre-flag "2 more U17 instances" was **wrong**: `CallMsgFilterT`/`SetWindowsHookExT` do carry the `T` suffix and sit in the census' 108-member norm bucket (census re-run unchanged: 525 declarations, 108 A/W+`T`, 404 bare single exports, 11 deviations, 2 mirror cases); **U16 0 of 4** - no version guard anywhere, only `#ifndef NOWH` (`:10322`) / `#ifndef NOMSG` (`:5390`) + the Desktop partition + `NOUSER`, measured with `tmp/_tmp_hook_guards.ps1` (8 hits over 16475 lines; the two extra `CallNextHookEx` rows are the `DefHookProc` macro at `:10416`/`:10419`, not declarations) | Fixed: **1 parameter-name divergence** - `user32.h:318` `hMod` -> `hmod` to match `winuser.h:10379`, with the definition (`hook.cpp:94`) and the `Call<>` argument following; the folder's 3rd such fix after `devicemanagement`'s 2, and the first where the name-divergence scan flagged it in a TU whose parameter names otherwise match the SDK. + a 64-line TU comment (`:16-79`); file 44 -> 108 lines, pure ASCII, 0 tabs, 0 trailing whitespace, CRLF throughout. New pre-scan item **P6** (bare `CallMsgFilter` is exported at 1C at RVA `00088CF0` - **the same RVA as `CallMsgFilterA`** - so a wrong binding there resolves and runs the ANSI entry point on wide arguments instead of throwing) and new open item **U27** (`STRICT` changes the identity of `HOOKPROC` and of every `DECLARE_HANDLE` type, hence the published mangled names: 3 of these 4 wrappers change name between the `STRICT` and `NO_STRICT` arms, measured with `tmp/_tmp_hookproc_probe.cpp`). **U15 4/4 dead**, **U19: 4 stems with 0 sites of any class** - the first section of the review with no caller *and* no SDK-import call anywhere, `HHOOK`/`HOOKPROC` appearing in no file of `src`/`examples`/`test` at all |
| 18 | `icon.cpp` | 14 | **0 binding / correctness / UB / convention defects** - the "Icon functions" section (`user32.h:426-440`) is **14 declarations, 14 definitions**, in header order, all `static`, all `User32::`-qualified, `Call<>` explicit **14/14** (tight style, 0 U14(b) sites); all 14 bindings were already D13-correct - **11 bare + 3 `AW`, and the `AW` set is exactly the SDK's three Icon A/W families** (`LoadIcon`, `GetIconInfoEx`, `PrivateExtractIcons`) - verified three ways: `cl /EP` literals (`tmp/_tmp_icon_pp.txt`), export table (`CopyIcon` 59 .. `PrivateExtractIconsW` 2B6, `tmp/_tmp_user32_exports.txt`), live probe (all 11 bare stems **RESOLVED**; bare `GetIconInfoEx`/`LoadIcon`/`PrivateExtractIcons` **MISSING** with their `W` halves **RESOLVED**, so the three `AW` bindings are load-bearing and the eleven bare ones are the D13 spelling) - **0 P5 wrong-stem flags, 0 D13 conversions**; signatures and returns match `winuser.h`, so 0 U18; 0 U11, 0 U13, **0 U17** (all three A/W wrappers already carry `T`; census re-run 1 Nov unchanged: 525 declarations, 108 A/W+`T`, 404 bare single exports, 11 deviations, 2 mirror cases - `tmp/_tmp_tsuffix_s18.txt`); `Call<>` template arguments vs SDK parameter types 14/14 after one spelling fix; **7 parameter-name divergences from the SDK - the largest batch in any TU of the review** | Fixed: **7 parameter names** made to match `winuser.h` - `user32.h:430` `pbIconBits`->`presbits`, `cbIconBits`->`dwResSize`, `dwVersion`->`dwVer`, `uFlags`->`Flags`; `:436` `hIcon`->`hicon`, `piconinfoex`->`piconinfo`; `:440` `lpszFile`->`szFileName` - with the definitions and the `Call<>` arguments following (`icon.cpp:16`-`:107`); names only, no type or ABI change, so no caller is affected. The folder-wide scan now reads **0 divergent parameter names over 426 definitions** (re-run 1 Nov; the folder's only other such sites, `devicemanagement`'s 2 and `hook`'s 1, were cured in their own sessions). Also cured the `::`-spelling split at `icon.cpp:48` (`const ICONINFO*` -> `const ::ICONINFO*`, matching `:431` and the SDK - this removes the pre-flag's single `CALL TYPE MISMATCH`, which was that known false-positive class), a **missing final newline**, and added a **54-line TU comment** (`:16-69`); file 114 -> 169 lines, pure ASCII, 0 tabs, 0 trailing whitespace, no BOM. `user32.h` touched (+4/-4). **U16 4 of 14** - the pre-flag's "at least 3" under-counted: `CreateIconFromResourceEx` (`winuser.h:10840`) is gated too - and the `ICONINFOEX` typedefs sit inside the same `_WIN32_WINNT 0x0600` block, the ill-formed-type sub-case at the highest threshold measured. **U15 12 of 14 unreachable**: all 14 have exactly one caller, the caller-side `Icon` class (`ui/gui/gdiobjects/icon.cpp`, built), and only `LoadIconT` + `DestroyIcon` are reached in built code. **U19 16 LAYER / 0 INDIRECT / 7 BYPASS / 0 MEMBER** - first family where every wrapper has a compiled call site, and all 7 bypass are in unbuilt targets. **U20 measured, not inferred**: `LoadIcon` returns a shared pseudo-handle (value `0x1002B`) that `DestroyIcon` silently tolerates; `Icon(nullptr)` at `windowfactory.cpp:92` binds `LoadIconW(NULL, nullptr)` -> NULL + `DestroyIcon(NULL)` -> 0; `WindowClass::SetIcon` stores a handle whose `Icon` owner destroys it at the end of the full-expression. **U26 escalated**: `ICONINFOEX` is defined twice (`#ifdef UNICODE` `winuser.h:11009`/`:11012`), so a narrow-TU consumer of `GetIconInfoExT` compiles the 560-byte A struct against the W entry point that writes 1,080 - silent overrun, first U26 instance that is corruption rather than a link error. Section coverage: `PrivateExtractIconExA` 2B3 / `PrivateExtractIconExW` 2B4 are exported but the **SDK declares them nowhere** (0 hits in `winuser.h`) and the tree never mentions them, so adding them is a hand-written-prototype U4 surface decision, not a TU fix - the pre-flag's "one name away from the wrapped `PrivateExtractIcons`" is a table adjacency, not a source one. |
| 19 | `ime.cpp` | 1 | **0 binding / correctness / UB / convention defects** - one wrapper, `SendIMEMessageEx` (`user32.h:636`, defined `ime.cpp:48` pre-edit / `:16` pre-session), declared in an **unlabelled 4-declaration block** `:635-639` with no section comment (U21-shaped; this TU owns 1 of its 4 members). The binding was already D13-correct and is **load-bearing**: `cl /EP` -> `"SendIMEMessageExW"` (single literal, `tmp/_tmp_ime_pp.txt:453241`), live probe bare `SendIMEMessageEx` **MISSING** while `A` 313 / `W` 314 **RESOLVED**, so a bare `qor_pp_useswinapi` would cache a NULL `pFunc` and throw on every call; 0 P1, 0 P5 flags, 0 P2 conversions. decl==def 1/1, `Call< LRESULT, HWND, LPARAM >` explicit (spaced), 0 U14(b), 0 U13, 0 U11, 0 U18 (return is the SDK's `LRESULT`), 0 parameter-name divergences, and the SDK parameters are **unnamed** (`IN HWND, IN LPARAM`) so SDK name comparison is impossible (the `help.cpp` case). **U17 1 instance** (`:636` is 1 of the census' 11 A/W-without-`T`, re-run unchanged 525/108/404/11/2) - no rename, session-11 policy. **U16 0 of 1** - no version guard; instead the sharper fact that `um\Ime.h` and `um\WinNls32.h` have **no includer anywhere in SDK 10.0.26100**, so the prototypes and the `IMESTRUCT`/`IMEPRO` types are invisible regardless of `WINVER`. **U4: 2 more members** (`:635`, `:637` - both non-static, defined nowhere, both single exports) -> new **T5**. **U15 1 of 1 unreachable**, **U19 1 LAYER / 1 INDIRECT / 0 BYPASS / 0 MEMBER** (sole caller `WindowController::SendIMEMessageEx` has no call site; the gui `IMEHandler` slot `m_IME` is read at `interactive.cpp:41` and **written nowhere**). **New documentation gap:** no TU comment block (3 comment lines in 23 lines) | Fixed: a **31-line TU comment** (`ime.cpp:16-46`) recording the binding rationale, the opt-in-header fact, the `PIMESTRUCT` mirror-struct hazard (second instance of U26's class), the section-coverage claim (13 IME-shaped exports, 2 declared; each `IMP*` family resolves only as `A`/`W`) and the dead IME call chain; + `user32.h:637`'s trailing `//Obselete do not use` moved to its own line at `:638` with the spelling corrected - **file still 695 lines**, so no header citation in the record moved, and the move cures the `_tmp_decl_no_def_scan.ps1` blind spot: declared-but-undefined **490/426/64 -> 491/426/65**, `WINNLSEnableIME` now listed at `:637`. File 23 -> 55 lines, pure ASCII, 0 tabs, 0 trailing whitespace. Gate 38/38 at `/W4`, exit 0 (`tmp/_tmp_cl_all_s19c.log`) |
| 20 | `keyboard.cpp` | 34 | **3 run-time-breaking binding defects fixed** - the folder's last three `*T`-stem bindings: `GetKeyboardLayoutNameT` (def `:82`), `VkKeyScanT` (`:243`), `VkKeyScanExT` (`:250`) used bare `qor_pp_useswinapi`, so the member's own `T` suffix stringized into the requested name (`kernel32.h:50`) and `GetProcAddress` was asked for `"GetKeyboardLayoutNameT"` etc.; the export table has **0 names ending in an uppercase `T`**, so all three cached a NULL `pFunc` and threw on first call (P1, G5). All three are real A/W families (`16D/16E`, `3E1/3E4`, `3E2/3E3`), so the fix is `qor_pp_useswinapiAW` over the bare stem; post-fix `/EP` = 27 bare + 7 `*W` literals (`tmp/_tmp_kb_bindings_s20_post.txt`) and the live probe resolves **all 34** (`tmp/_tmp_kb_probe_s20.txt`). Section shape: `user32.h:70-104` = 34 declarations, all `static`, all defined here in header order (0 U4, 0 U11, 0 U13); sigcmp decl==def 34/34, 0 parameter-name divergences (folder zero holds); **0 U17** - all 7 A/W members carry `T`. **U18 2 divergences recorded, not changed**: `GetKeyboardLayoutList` `UINT` vs SDK `int` (`winuser.h:1313`), `keybd_event` `void*` vs `ULONG_PTR` (`:6071`) - same width, published surface. **U14(b) 2 of the folder's 6 deducing sites fixed** (`GetKeyboardLayout`, `GetKeyboardLayoutList`) -> folder now 424 explicit / 4 deducing; **U14(c)** `GetKBCodePage()` -> `(void)` both sides -> folder 37/1 defs, 37/1 decls, sole survivor `message.cpp InSendMessageEx`. **U21**: 7 of the 34 are SDK *Window* entry points parked in the keyboard section, none in `//Window functions` - recorded, not moved. **U16 16 of 34** version-guarded, 18 Desktop-only. **U15 29 of 34 unreachable**; **U19 5 LAYER / 7 INDIRECT / 11 BYPASS / 2 MEMBER** - the folder's largest bypass count, and **every bypass is in a target that is not built**: `ui/gl/session.cpp` in no source list, `winqor_gl`'s `add_subdirectory(gl)` commented out at `ui/CMakeLists.txt:4`, `retropad`'s commented out at `examples/.../windows/CMakeLists.txt:8`; 1 of the 11 BYPASS rows is a member declaration (`windowcontroller.h:310`), 2 INDIRECT are Linux. **New defect class found by re-running the folder-wide AW checklist: `qor_pp_useswinapiAW` on a stem with no A/W pair** - `mouse.cpp` reads `bare=0 AW=10 single-ok=0`, i.e. **all 10 of its wrappers ask for a `*W` that does not exist** (added to P1/census; its session becomes a whole-TU fix). Post-fix folder census: 428 bindings, 17 script-unresolvable = 11 truly broken (mouse 10 + `window.cpp:124`) + 6 P2 | Fixed: the 3 bindings + 2 `Call<>` template lists + `(void)` (`user32.h:77`, def) + 7 stray inner spaces + a **61-line TU comment** (`:16-76`) covering D13, the KL_NAMELENGTH/cbSize/dead-key contracts, the calling-thread input-queue semantics, U16 and the not-built bypass chain. File 254 -> 314 lines, pure ASCII, 0 tabs, 0 trailing whitespace, CRLF. `user32.h` touched at 1 line, still **695 lines**. Gate: this TU `/W4` exit 0, folder **38/38 exit 0**, 0 diagnostics |
| 21 | `mdi.cpp` | 4 | **0 binding / correctness / UB / convention-surface defects; 1 internal `Call<>` type contradiction fixed** - the "Multi Document Interface functions" section (`user32.h:494`, declarations `:495-498`) is 4 declarations, 4 definitions, in header order, all `static`, all `User32::`-qualified, `Call<>` explicit 4/4 (spaced style); all four bindings were already D13-correct, verified three ways: `cl /EP` emits exactly four literals `"CreateMDIWindowW"`, `"DefFrameProcW"`, `"DefMDIChildProcW"`, `"TranslateMDISysAccel"` (`tmp/_tmp_mdi_pp.txt:453320`, `:453327`, `:453334`, `:453341`); the export table has the three families as A/W-only (`6F`/`70`, `A1`/`A2`, `A3`/`A4`) with **no bare stem export** and `TranslateMDISysAccel` as a genuine single export (`3B9`), so the three `AW` bindings are **load-bearing** - they would resolve today only because the SDK macros `#define CreateMDIWindow CreateMDIWindowW` (`winuser.h:12230`-`:12234`), `DefFrameProc` (`:12147`-`:12151`) and `DefMDIChildProc` (`:12179`-`:12183`) are live inside the binding expression, exactly the reliance G1 forbids; and a live probe (`tmp/_tmp_mdi_probe_s21.txt`) resolves all four names and reports MISSING for the bare stems and the `*T` spellings (the probe had to `LoadLibraryW` first: `GetModuleHandleW(L"user32")` fails with gle=126 in a bare console process - a direct G5 measurement). Signatures **and parameter names** match `winuser.h` field-for-field, so **0 U18**; 0 U17 (all three A/W members carry `T`, census re-run unchanged 525/108/404/11/2); 0 U11, 0 U13; **U16 0 of 4** version-guarded - all four under `#ifndef NOMDI` (`:12074`-`:12261`) + the Desktop partition (`:12092`), `TranslateMDISysAccel` also inside `#ifndef NOMSG` (`:12185`-`:12194`), measured with `tmp/_tmp_mdi_guards_s21.ps1`; **U21 third instance** - the SDK's `NOMDI` block holds **seven** entry points and this section owns four, the other three (`ArrangeIconicWindows` `:12199`/`E`, `CascadeWindows` `:12249`/`24`, `TileWindows` `:12240`/`3AE`) being declared in `//Window functions` (`user32.h:135`, `:139`, `:208`) and defined in `window.cpp:58`, `:86`, `:572` - recorded, not moved; **U15 4 of 4 unreachable**; **U19 1 LAYER / 1 INDIRECT / 0 BYPASS / 0 MEMBER** - the first family of the review with **0 SDK bypass**, consistent with there being no MDI host anywhere in the tree | Fixed: **1 `Call<>` template argument** - `DefFrameProcT`'s third type `INT` -> `UINT`, contradicting both its own `UINT uMsg` (`user32.h:496`, def `:91`) and the SDK (`:12132`, `:12141`); the **only uppercase `INT` token in any `Call<>`/`voidCall<>` list in the folder** (new instrument `tmp/_tmp_int_sites_s21.ps1`: 1 of 435 `Call<>`/`voidCall<>` template lists over raw source text - `tmp/_tmp_callcount_s21.ps1` reconciles that denominator against the census' 428 code-line bindings - now 0) and the only line `tmp/_tmp_sigcmp.ps1` flags in this TU. No ABI effect (same width, same slot, ids far below `int`'s sign bit), so a U14 convention fix, not a run-time one. + a 67-line TU comment (`mdi.cpp:16-82`); file 44 -> 112 lines. **New finding, recorded not fixed (U26 third mirror-struct instance):** the family's only compiled call site (`messagehandler.cpp:241`) passes a `Message*` (`message.h:33-44`) to the export as `::LPMSG`, and `tmp/_tmp_msg_mirror_s21.cpp`/`_tmp_msg_mirror_s21.bat` - the review's first two-architecture layout measurement - show the mirror is byte-identical to `::MSG` on **x64** (48 bytes, same offsets to `pt.y`) and divergent from `lParam` onward on **x86** (`MSG` 28 bytes `lParam@12 time@16 pt@20` vs `Message` 40 bytes `lParam@16 time@24 pt@28`); the same mirror feeds **7 call sites across 6 wrappers** (`messagehandler.cpp:136,:146,:181,:236,:241`, `windowcontroller.cpp:658`, `acceleratortable.cpp:83`), latent while only the `x64-Debug` profile exists, and handed to `message.cpp` (session 23). |
| 22 | `menu.cpp` | 39 | **0 binding / correctness / UB defects after 2 D13 rewrites; 0 return-type divergences, 3 parameter-type divergences.** The whole `user32.h:214 //Menu functions` section (decls `:215-253`, defs `:82`-`:348`, header order): all 39 decl/def pairs parse with **0 signature flags**, 0 U4 (all 39 static and defined here), 0 U13, 0 U17 (all 9 A/W members carry `T`), and every `Call<>` site is explicit. Binding: 9 stems are SDK A/W families and bind through `qor_pp_useswinapiAW`, 30 bind bare - and `tmp/_tmp_menu_class_s22.ps1` proves each of those 30 is a **genuine single export** (bare present, no `<stem>A`/`<stem>W`, and no `<stem> -> <stem>W` macro in `winuser.h`), so the folder's largest bare block is correct rather than suspicious. Two of the nine were bare: `GetMenuString` `:225`, `InsertMenuItem` `:260` - the **P2** pair. **Behaviour-neutral**: pre-fix `/EP` already emitted `"GetMenuStringW"`/`"InsertMenuItemW"` because the SDK macro rewrote the stem before stringizing (G1), and post-fix emits the same two literals by `##W` pasting. | Fixed: those 2 bindings -> `qor_pp_useswinapiAW`; folder census **17 -> 15** unresolvable (`tmp/_tmp_p1census_post_s22.txt`), no `menu.cpp` line left in it, and `/EP` over the TU gives 39 literals, all 39 in the export table. New 66-line TU comment, file 289 -> 355 lines. Registers: **U18** new measurement - the SDK return type sits on its **own** line above the stem in `winuser.h`, so a declaration-line-only instrument cannot see it; new `tmp/_tmp_menu_ret_s22.ps1` reads it and finds **0 of 39** return-type divergences but 3 parameter ones: `TrackPopupMenu`'s `HWND prcRect` vs the SDK's `_Reserved_ CONST RECT*` (unreachable, so latent) and `InsertMenuT`/`ModifyMenuT`'s `void* uIDNewItem` vs `UINT_PTR` (both live; `AppendMenuT` in the same section has it right). All published header surface, so recorded not changed, and the `Call<>` lists at `:254`/`:296` inherit them. **U16** 12 of 39 guarded (9 at `WINVER >= 0x0400`, 3 at `0x0500`) - the largest single-TU count; **U15** 4 of 39 unreachable (`GetMenuCheckMarkDimensions`, `IsMenu`, `LoadMenuT`, `TrackPopupMenu`); **U19** 40 LAYER / 10 INDIRECT / 18 BYPASS rows and 6 MEMBER - 3 BYPASS rows are controller *declarations* the classifier mis-reads (`i_m/menu.h:95`, `windowcontroller.h:226`, `:229`), the 15 real ones all in `retropad.cpp`, whose subdirectory is commented out at `examples/.../windows/CMakeLists.txt:8` - **0 bypass in built code**; **U21** fourth instance (`#ifndef NOMENUS` `winuser.h:7432-8101` = 38 stems, plus `CheckMenuRadioItem` in a second NOMENUS region at `:10509`; `IsMenu`/`GetMenuBarInfo` pair outside it; `ChangeMenuA/W` + `CalculatePopupWindowPosition` declared nowhere; the context-help pair in `//Shell functions` = the **T4** pair, still open, both halves re-measured as real exports ordinals `34A`/`17C`). Gate **38/38** at `/W4`, exit 0, 0 diagnostics. Hygiene ran first: `session-log.md` 45.4 KB -> 30.3 KB via two hash-verified slices. |


| 23..38 | not started (TU index; 15 left, see "Queue" in `status.md`) | | | |

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

