# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 1 Nov 2026 (session 18 `icon.cpp` closed - code in `83d9f019`, record work in this pass; in the same pass
`registers.md` and `session-log-archive.md` were split into `registers/` and `archive/`). Review of
`src/platform/os/windows/api_layer/user` (the `User32` wrapper class over `user32.dll`) for correctness, consistency,
safety, undefined behaviour and conformance to the api_layer conventions. Protocol: **one translation unit per session** -
the whole point is not to lose findings between sessions. Small, unambiguous defects get fixed as they are found; anything
that changes library-wide behaviour or needs a decision goes in the register and is *not* touched.

## Where the record lives - start here, then open only what the step needs

| File | Contents | Open it when |
|---|---|---|
| `status.md` (this file; **stay under 180 lines / 18 KB** - 177 after the 1 Nov slice) | live state, queue, the two convention decisions, register locator, defect census pointer, verification gate, current TU pre-flags | every session, first |
| `registers/<file>.md` - **one register entry per file**, index in `registers/README.md` | verbatim register text: P1-P5 `prescan.md`, P6+U27 `p6-u27.md`, G1-G6 `ground-rules.md`, U1-U26 `u<n>.md`, T1-T4 `tasks.md`, census `census.md` | only the ids named by the locator below or by the current TU's pre-flags |
| `method.md` | the per-TU review method, tooling index, **instrument caveats** | before running an instrument or the gate |
| `archive/tu/<tu>.md` - **one narrative per TU**, index in `archive/README.md` | that TU's narrative + verdict detail | only the TU you just reviewed |
| `archive/session-log.md` | the session-log table (rows 1-18) + closure notes | only while writing the row for the TU you just reviewed |
| `archive/history.md`, `archive/prescan-help.md` | consolidation/split history, consumed `help.cpp` pre-flags | never, unless auditing the record itself |

The rule this split exists to enforce: **findings, narratives and register text never go in this file.** New finding ->
the register file the locator names; new TU narrative -> `archive/tu/<tu>.md`; new row -> `archive/session-log.md`; new
instrument or caveat -> `method.md`. This file outgrew a session's context once and the review started cycling; the same
happened to `registers.md` (1,063 lines) and `session-log-archive.md` (1,394 lines), so both were split on 1 Nov 2026.
Hard rule now: **no record file over ~40 KB**, and the accumulating entries (`u15`, `u16`, `u17`, `u19`, `u20`) are read
for their **head only** (the rule) and appended to at the **end** - never read the whole tally back.


## Live state (1 Nov 2026)

- **19 TUs reviewed** (sessions 1-18): `accelerator`, `accessibility`, `brush`, `caret`, `clipboard`, `combobox`,
  `console`, `timer`, `coordtransform`, `cursor`, `desktop`, `devicecontext`, `devicemanagement`, `dialog`,
  `errorhandling`, `filledshapes`, `help`, `hook`, `icon`.
- **Session 18 (`icon.cpp`) CLOSED 1 Nov 2026** - verdict, the 7 parameter-name fixes and the per-family tallies are row 18
  of `archive/session-log.md` and the `icon` entries under **U15/U16/U17/U19/U20/U26** (see the locator; U26 escalated to a
  silent `ICONINFOEX` overrun; U15 shows 12 of 14 wrappers unreachable). Code committed; gate green **38/38** at `/W4`.
- **Session 19 (`ime.cpp`) OPEN 1 Nov 2026** - 23 lines, 1 wrapper (`SendIMEMessageEx`, `user32.h:636`, in an unlabelled
  block). Pre-flags measured below; verdict and fixes still to be agreed with the user.

## Queue - 19 TUs left, alphabetical (lines = on-disk physical lines, measured 1 Nov 2026)

| # | TU | lines | # | TU | lines |
|--:|---|--:|--:|---|--:|
| 19 | `ime.cpp` | 23 | 29 | `resource.cpp` | 30 |
| 20 | `keyboard.cpp` | 254 | 30 | `scrollbar.cpp` | 102 |
| 21 | `mdi.cpp` | 44 | 31 | `systemshutdown.cpp` | 51 |
| 22 | `menu.cpp` | 289 | 32 | `text.cpp` | 44 |
| 23 | `message.cpp` | 184 | 33 | `window.cpp` | **615** |
| 24 | `monitor.cpp` | 51 | 34 | `windowclass.cpp` | 114 |
| 25 | `mouse.cpp` | 86 | 35 | `windowprocedure.cpp` | 30 |
| 26 | `painting.cpp` | 191 | 36 | `windowproperties.cpp` | 51 |
| 27 | `processthread.cpp` | 57 | 37 | `winqor_api_user.cpp` (glue) | 14 |
| 28 | `rectangle.cpp` | 93 | | | |

2,323 lines in total, and **1,533 of them (66%) are in five TUs**: `window`, `menu`, `keyboard`, `painting`, `message` -
which also hold every remaining flagged defect: **P1 x5** (`keyboard.cpp` x3, `window.cpp:124`, `mouse.cpp:82`) and
**P2 x6** (`menu.cpp` x2, `message.cpp` x4). `window.cpp` (615 lines, 85 bindings, P3 `CreateWindowT`, U18's `void`
converters) may not fit one session - decide how to split it before starting it, not mid-session.

## Convention decision

- **D13 (adopted 29 Oct 2026, user decision; `docs/agent/DECISIONS.md`).** Explicit `qor_pp_useswinapiAW(<mod>, Stem)`
  wherever the SDK has an A/W family; bare `qor_pp_useswinapi` only for entry points that are a single export with no A/W
  variants (`ShowWindow`, `GetComboBoxInfo`, ...). Never rely on the SDK's `Stem -> StemW` macro surviving into the binding
  expression (G1). Apply in every TU as it is reviewed.
- **Return-check / error-handler policy (settled 29 Oct 2026, user decision, session 3).** **Leave as-is: no
  `Win32ErrorHandler _;` and no `CheckReturn`/`CheckNonZeroHandle` in these wrappers**, even though `api_layer/shell` and
  `api_layer/kernel` use both and G6 makes `CheckReturn` always active. Sessions still *record* the sites that would take
  them (U1/U2) but change no behaviour. So a Win32 `FALSE`/NULL/0 failure is silent here, and a swallowed SEH fault is
  indistinguishable from a legitimate zero (U7).


## Register locator - id -> which `registers/` file and line holds its text

A locator, not a finding: open the file at the line given before you act on, extend or close an id - never open the
whole `registers/` tree. Each summary is the first line of that entry as written at session 18, and the whole locator is
generated by `tmp/_tmp_status_index_gen_s19.ps1`; do not hand-edit it.

- **P1** `registers/prescan.md:10` bindings that cannot resolve at run time (export table + live probe + source line). Six at the first...
- **P2** `registers/prescan.md:37` bindings that work today only because the SDK macro rewrites them; convert to explicit AW
- **P3** `registers/prescan.md:43` naming flags from the new T-suffix census (`tmp/_tmp_tsuffix_census.ps1`, session 6). Two members ca...
- **P4** `registers/prescan.md:49` four exports in the table are forwarders, which the binding question cannot see (found in session 7)...
- **P5** `registers/prescan.md:57` the "wrong stem bound" defect class, and the instrument that finds it (`tmp/_tmp_wrong_stem_scan.ps1...
- **P6** `registers/p6-u27.md:9` (promoted extract) Bare `CallMsgFilter` resolves and runs the ANSI entry point on wide arguments
- **G1** `registers/ground-rules.md:5` the SDK A/W macro rewrites the binding name before it is stringized. `_ATXT(_NAME)` inside
- **G2** `registers/ground-rules.md:12` the nine stems `user32.h:9-17` `#undef`s are the real missing-export risk. All nine resolve to
- **G3** `registers/ground-rules.md:24` the previous audit report is partly wrong. `tmp/_tmp_audit_report.txt` section A ("run-time
- **G4** `registers/ground-rules.md:30` module token case is cosmetic. `_TXT(User32)` stringizes to `L"User32"` and the module lookup is
- **G5** `registers/ground-rules.md:33` the "already loaded image" assumption is not universal. In a bare console process
- **G6** `registers/ground-rules.md:36` return checking is silently always on. `returncheck.h:19` tests
- **U1** `registers/u1.md:3` no call-site error handler anywhere in this library. Policy settled 29 Oct 2026 (user): accepted as-...
- **U2** `registers/u2.md:3` return-check policy is absent. Policy settled 29 Oct 2026 (user): accepted as-is - no `CheckReturn`
- **U3** `registers/u3.md:3` `hook.cpp` build-list status: RESOLVED (user commit `05d3ca2f`, 29 Oct 2026), review still open.
- **U4** `registers/u4.md:3` `user32.h` declares about 101 non-static members that no `.cpp` defines. The class is documented
- **U5** `registers/u5.md:3` the binding is cached for the process lifetime including failure. `qor_pp_useswinapi` expands to
- **U6** `registers/u6.md:3` calling-convention mismatch in the call path (half fixed, committed). Measured 29 Oct 2026, re-check...
- **U7** `registers/u7.md:3` SEH faults are swallowed into a default return. `Call` catches everything and returns `ret _{}`
- **U8** `registers/u8.md:3` `throw("Missing library function.")` (`library.h:60`, `:92`) throws a `const char*`. Not derived
- **U9** `registers/u9.md:3` no `purpose.md` in this folder and no scope/audit note. `api_layer/shell/purpose.md` and
- **U10** `registers/u10.md:3` `user32.h:19-62` defines `CONSOLECONTROL`, `CONSOLEENDTASK`, `CONSOLEWINDOWOWNER`,
- **U11** `registers/u11.md:3` `resource.cpp` defines both Resource members without the `User32::` qualifier. `resource.cpp:16`
- **U12** `registers/u12.md:3` `accessibility.cpp` and `timer.cpp` held each other's wrappers - FIXED 29 Oct 2026 (session 2).
- **U13** `registers/u13.md:3` one member was defined through `User32::` but declared non-static: `ConsoleControl` - FIXED 30 Oct 2...
- **U14** `registers/u14.md:3` `Library::Call`/`voidCall` template-argument style is split two ways. Measured 29 Oct 2026 over all...
- **U15** `registers/u15.md:3` wrappers that are exported through `QOR_WINAPIUSER` but have no reachable caller in the tree. Opened
- **U16** `registers/u16.md:3` the wrappers pair with SDK declarations that the SDK only emits when a version guard passes, and not...
- **U17** `registers/u17.md:3` A/W wrappers declared without the folder's `T` suffix. Opened 29 Oct 2026 (session 6) when the two c...
- **U18** `registers/u18.md:3` a wrapper pair whose return type contradicts the SDK: `void` where `user32.dll` returns `BOOL`. Open...
- **U19** `registers/u19.md:3` call sites that reach a `user32.dll` entry point through the SDK import thunk instead of through thi...
- **U20** `registers/u20.md:3` the caller-side `Cursor`/`Icon` ownership model is wrong at the sites this layer feeds. Opened 30 Oc...
- **U21** `registers/u21.md:3` the "DC functions" section is not the SDK's DC family, and the SDK's DC family is split across two T...
- **U22** `registers/u22.md:3` the `DeviceContext` release path this layer feeds calls `ReleaseDC` with a NULL `HWND`. Opened 30 Oc...
- **U23** `registers/u23.md:3` `MessageBoxIndirectT`'s parameter puts `const` on the pointer instead of the pointee. Opened 30 Oct...
- **U24** `registers/u24.md:3` the one in-tree `FlashWindow` caller reads its `BOOL` as a success flag. Opened 31 Oct 2026 (session...
- **U25** `registers/u25.md:3` `const` is spelled three ways in `user32.h`, and the `CONST::` spelling is an SDK macro glued to a s...
- **U26** `registers/u26.md:3` `TCHAR` width is an include-order property, not a build setting. Opened 31 Oct 2026 (session 16,
- **U27** `registers/p6-u27.md:9` (same extract) STRICT changes the identity of HOOKPROC and every DECLARE_HANDLE type
- **T1** `registers/tasks.md:5` two declared-but-undefined members in the "Authorization functions" section. `user32.h:658-659` decl...
- **T2** `registers/tasks.md:18` the whole Window Station / Desktop surface is dead exported API, and its conventions are therefore u...
- **T3** `registers/tasks.md:27` the "Power Management functions" pair is declared but has no owning TU at all. Opened 30 Oct 2026 (s...
- **T4** `registers/tasks.md:50` the menu half of the context-help-id family is declared and defined nowhere. Opened 31 Oct 2026 (ses...

## Defect census

- The sessions-1-18 census (run-time defects fixed, what is still open, convention fixes, folder-wide measures) is kept verbatim at `registers/census.md:3` - refresh it there when a session closes.

### Verification gate (identical every session)
Edited TU compiles standalone at `/W4` exit 0; the whole library compiles **38/38 at `/W4`, exit 0, no diagnostics**;
`cl /EP` re-run after the edit still yields the expected binding literals; `user32.h` untouched unless the row says
otherwise. Latest closure run `tmp/_tmp_cl_all_s19.log` (43 lines, exit 0, 0 hits for `error C|warning C|fatal`); earlier
runs `_tmp_cl_all_s9/s11*/s12/s13/s17/s18*.log`.

## Pre-flags for the current TU (rolling - keep this under ~40 lines)

**`ime.cpp` (session 19) - measured 1 Nov 2026.** Raw output: `tmp/_tmp_ime_pre.txt` (via `tmp/_tmp_ime_pre.bat`, one
owner per file) + `_tmp_ime_pp.txt`, `_tmp_name_resolve_s19.txt`, `_tmp_decl_no_def_s19.txt`, `_tmp_ime_linkermember.txt`,
`_tmp_fmt_check.ps1`.
- **No "IME functions" section exists in `user32.h`:** `:635-639` is an **unlabelled 4-declaration block** between
  `//Window-Class functions` (`:619`) and `//Shell functions` (`:641`) - `DisableProcessWindowsGhosting` `:635`
  (non-static, defined nowhere), `SendIMEMessageEx` `:636` (static, this TU), `WINNLSEnableIME` `:637` (non-static,
  defined nowhere, trailing `//Obselete do not use`), `GetDpiForWindow` `:639` (static, defined at `window.cpp:608` -
  session 33). This TU owns **1 of 4**; U21-shaped placement, no section comment to match.
- **Binding already D13-correct and load-bearing:** `cl /EP` -> `"SendIMEMessageExW"`; probe bare **MISSING**, `A` 313 /
  `W` 314 **RESOLVED** -> 0 P1, 0 P2. Bare `WINNLSEnableIME` **RESOLVED** (3E6) with `A`/`W` **MISSING** - a genuine
  single export, so bare is the right spelling if it is ever wrapped.
- **The SDK prototypes are not in the layer's include chain at all:** they live in the SDK's **`Ime.h`** (`:35`/`:36`,
  under `#ifndef _IME_` `:16` + Desktop partition `:22`, macro arms `:38`/`:40`), which nothing here includes, and
  `IMESTRUCT` (`Ime.h:46-55`) has **0 occurrences** in the TU's preprocessed output and 0 in `src` - so the wrapper's
  `LPARAM` (documented as a `PIMESTRUCT`) names no type. `WINNLSEnableIME`/`WINNLSGetEnableStatus`/`WINNLSGetIMEHotkey`
  are declared in **no** SDK `um` header (0 hits in `10.0.26100.0\um\*.h`) though all three export (3E6/3E7/3E8, RESOLVED).
- **U16 0 of 1** (`_IME_` + Desktop only, no version guard - and the ill-formed-type sub-case cannot apply, the SDK
  declaration is never visible). **U13/U14/U18 clean:** sigcmp 1/1 `decl==def ok` + 1/1 `Call ok`,
  `Call< LRESULT, HWND, LPARAM >` explicit (spaced), return matches SDK `LRESULT`; 0 name divergence, and the SDK
  parameters are **unnamed** (`IN HWND, IN LPARAM`), so SDK name comparison is impossible (the `help.cpp` case).
- **U4: 2 declared-but-undefined members here** - `:635` and `:637`. `_tmp_decl_no_def_scan.ps1` sees only `:635`
  (490 decls / 426 defined / 64 undefined) because its regex needs `;` at line end and `:637` has a trailing comment -
  measured blind spot, true count >= 65. Both non-static, both single exports -> T4-shaped, candidate **T5**.
  `dumpbin /linkermember:1` publishes `SendIMEMessageEx` (`@@SA_J`, static) and `GetDpiForWindow`; neither undefined member.
- **U15: 1 of 1 unreachable** - sole caller `WindowController::SendIMEMessageEx` (`windowcontroller.cpp:773-775`,
  published `windowcontroller.h:282`) has no call site in `src`/`examples`/`test`. **U19: 1 LAYER / 1 INDIRECT / 0 BYPASS /
  0 MEMBER.** The gui IME path is dead too: `IMEHandler` (`view/handlers/interactive/ime.cpp`, built) sits in `m_IME`
  (`interactive.h:57`), read at `interactive.cpp:41`, **never written** -> never installed, and calls no wrapper. The
  `Imm*` family (imm32) is wrapped nowhere in the tree.
- **U17: 1 of the 11** (`:636` is in the census list; re-run 1 Nov unchanged 525/108/404/11/2). Shape: 23 lines, **no TU
  comment block** (3 comment lines = licence + include-order note), pure ASCII, 0 tabs, 0 trailing whitespace, CRLF, final LF.

## How this record is split

The 1 Nov 2026 split of the 1,222-line `status.md` into four files is recorded in `archive/history.md` ("How the record
was split"). The **second** split in the same pass sliced the two files that then outgrew a session - `registers.md`
(1,063 lines) into `registers/` (33 slices, one per register entry) and `session-log-archive.md` (1,394 lines) into
`archive/` (19 slices, one per TU narrative) - by exact line range, verified by re-joining every slice and hashing the
result against the original bytes (both `MATCH`). Pre-split copies: `tmp/_tmp_registers_presplit_s18.md`,
`tmp/_tmp_archive_presplit_s18.md`; slicer `tmp/_tmp_record_split_s19.ps1`; slice manifest
`tmp/_tmp_split_manifest_s19.csv`. **Regenerate the locator above with `tmp/_tmp_status_index_gen_s19.ps1` whenever a
register file gains or moves an entry** - the hand-maintained pointers had drifted 2 lines from U16 onwards before the
split, which is why the locator is now generated, never typed.
