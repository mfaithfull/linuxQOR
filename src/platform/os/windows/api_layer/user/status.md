# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 1 Nov 2026 (session 18 `icon.cpp` closed - code in `83d9f019`, record work in this pass). Review of
`src/platform/os/windows/api_layer/user` (the `User32` wrapper class over `user32.dll`) for correctness, consistency,
safety, undefined behaviour and conformance to the api_layer conventions. Protocol: **one translation unit per session** -
the whole point is not to lose findings between sessions. Small, unambiguous defects get fixed as they are found; anything
that changes library-wide behaviour or needs a decision goes in the register and is *not* touched.

## Where the record lives - start here, then open only what the step needs

| File | Contents | Open it when |
|---|---|---|
| `status.md` (this file; **stay under 150 lines / 15 KB**) | live state, queue, the two convention decisions, register locator, defect census, verification gate, current TU pre-flags | every session, first |
| `registers.md` | verbatim register text: P1-P6, G1-G6, U1-U27, T1-T4 | before acting on any id in the locator below |
| `method.md` | the per-TU review method, tooling index, **instrument caveats** | before running an instrument or the gate |
| `session-log-archive.md` | per-TU narratives + the full session-log table + consumed pre-flags | only while writing the row for the TU you just reviewed |

The rule this split exists to enforce: **findings, narratives and register text never go in this file.** New finding ->
`registers.md`; new TU row/narrative -> `session-log-archive.md`; new instrument or caveat -> `method.md`. This file
outgrew a session's context once and the review started cycling, so keep it under 150 lines.

## Live state (1 Nov 2026)

- **19 TUs reviewed** (sessions 1-18): `accelerator`, `accessibility`, `brush`, `caret`, `clipboard`, `combobox`,
  `console`, `timer`, `coordtransform`, `cursor`, `desktop`, `devicecontext`, `devicemanagement`, `dialog`,
  `errorhandling`, `filledshapes`, `help`, `hook`, `icon`.
- **Session 18 (`icon.cpp`) CLOSED 1 Nov 2026** - verdict, the 7 parameter-name fixes and the per-family tallies are row 18
  of `session-log-archive.md` and the `icon` entries under **U15/U16/U17/U19/U20/U26** in `registers.md` (U26 escalated to a
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


## Register locator - id -> where its text lives in `registers.md`

A locator, not a finding: open `registers.md` at the line given before you act on, extend or close an id. Each summary is
the first line of that entry as written at session 18.

- **P1** `registers.md:15` bindings that cannot resolve at run time (export table + live probe + source line). Six at the first...
- **P2** `registers.md:42` bindings that work today only because the SDK macro rewrites them; convert to explicit AW
- **P3** `registers.md:48` naming flags from the new T-suffix census (`tmp/_tmp_tsuffix_census.ps1`, session 6). Two members ca...
- **P4** `registers.md:54` four exports in the table are forwarders, which the binding question cannot see (found in session 7)...
- **P5** `registers.md:62` the "wrong stem bound" defect class, and the instrument that finds it (`tmp/_tmp_wrong_stem_scan.ps1...
- **G1** `registers.md:88` the SDK A/W macro rewrites the binding name before it is stringized. `_ATXT(_NAME)` inside
- **G2** `registers.md:95` the nine stems `user32.h:9-17` `#undef`s are the real missing-export risk. All nine resolve to
- **G3** `registers.md:107` the previous audit report is partly wrong. `tmp/_tmp_audit_report.txt` section A ("run-time
- **G4** `registers.md:113` module token case is cosmetic. `_TXT(User32)` stringizes to `L"User32"` and the module lookup is
- **G5** `registers.md:116` the "already loaded image" assumption is not universal. In a bare console process
- **G6** `registers.md:119` return checking is silently always on. `returncheck.h:19` tests
- **U1** `registers.md:129` no call-site error handler anywhere in this library. Policy settled 29 Oct 2026 (user): accepted as-...
- **U2** `registers.md:137` return-check policy is absent. Policy settled 29 Oct 2026 (user): accepted as-is - no `CheckReturn`
- **U3** `registers.md:143` `hook.cpp` build-list status: RESOLVED (user commit `05d3ca2f`, 29 Oct 2026), review still open.
- **U4** `registers.md:150` `user32.h` declares about 101 non-static members that no `.cpp` defines. The class is documented
- **U5** `registers.md:187` the binding is cached for the process lifetime including failure. `qor_pp_useswinapi` expands to
- **U6** `registers.md:190` calling-convention mismatch in the call path (half fixed, committed). Measured 29 Oct 2026, re-check...
- **U7** `registers.md:200` SEH faults are swallowed into a default return. `Call` catches everything and returns `ret _{}`
- **U8** `registers.md:203` `throw("Missing library function.")` (`library.h:60`, `:92`) throws a `const char*`. Not derived
- **U9** `registers.md:206` no `purpose.md` in this folder and no scope/audit note. `api_layer/shell/purpose.md` and
- **U10** `registers.md:209` `user32.h:19-62` defines `CONSOLECONTROL`, `CONSOLEENDTASK`, `CONSOLEWINDOWOWNER`,
- **U11** `registers.md:227` `resource.cpp` defines both Resource members without the `User32::` qualifier. `resource.cpp:16`
- **U12** `registers.md:237` `accessibility.cpp` and `timer.cpp` held each other's wrappers - FIXED 29 Oct 2026 (session 2).
- **U13** `registers.md:250` one member was defined through `User32::` but declared non-static: `ConsoleControl` - FIXED 30 Oct 2...
- **U14** `registers.md:265` `Library::Call`/`voidCall` template-argument style is split two ways. Measured 29 Oct 2026 over all...
- **U15** `registers.md:292` wrappers that are exported through `QOR_WINAPIUSER` but have no reachable caller in the tree. Opened
- **U16** `registers.md:446` the wrappers pair with SDK declarations that the SDK only emits when a version guard passes, and not...
- **U17** `registers.md:573` A/W wrappers declared without the folder's `T` suffix. Opened 29 Oct 2026 (session 6) when the two c...
- **U18** `registers.md:615` a wrapper pair whose return type contradicts the SDK: `void` where `user32.dll` returns `BOOL`. Open...
- **U19** `registers.md:629` call sites that reach a `user32.dll` entry point through the SDK import thunk instead of through thi...
- **U20** `registers.md:740` the caller-side `Cursor`/`Icon` ownership model is wrong at the sites this layer feeds. Opened 30 Oc...
- **U21** `registers.md:813` the "DC functions" section is not the SDK's DC family, and the SDK's DC family is split across two T...
- **U22** `registers.md:829` the `DeviceContext` release path this layer feeds calls `ReleaseDC` with a NULL `HWND`. Opened 30 Oc...
- **U23** `registers.md:847` `MessageBoxIndirectT`'s parameter puts `const` on the pointer instead of the pointee. Opened 30 Oct...
- **U24** `registers.md:862` the one in-tree `FlashWindow` caller reads its `BOOL` as a success flag. Opened 31 Oct 2026 (session...
- **U25** `registers.md:883` `const` is spelled three ways in `user32.h`, and the `CONST::` spelling is an SDK macro glued to a s...
- **U26** `registers.md:911` `TCHAR` width is an include-order property, not a build setting. Opened 31 Oct 2026 (session 16,
- **T1** `registers.md:954` two declared-but-undefined members in the "Authorization functions" section. `user32.h:658-659` decl...
- **T2** `registers.md:967` the whole Window Station / Desktop surface is dead exported API, and its conventions are therefore u...
- **T3** `registers.md:976` the "Power Management functions" pair is declared but has no owning TU at all. Opened 30 Oct 2026 (s...
- **T4** `registers.md:999` the menu half of the context-help-id family is declared and defined nowhere. Opened 31 Oct 2026 (ses...
- **P6** `registers.md:83` (opened inside session-log row 17; promoted extract below the P section) Bare `CallMsgFilter` resolves and runs the ANSI entry point on wide arguments
- **U27** `registers.md:83` (same extract) STRICT changes the identity of HOOKPROC and every DECLARE_HANDLE type, so 3 of the 4 hook wrappers change mangled name

## Defect census

- The sessions-1-18 census (run-time defects fixed, what is still open, convention fixes, folder-wide measures) is kept verbatim at `registers.md:1028` - refresh it there when a session closes.

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

The 1 Nov 2026 split of the 1,222-line `status.md` into these four files is recorded in `session-log-archive.md` ("How the record was split"); regenerate the locator above with `tmp/_tmp_status_index_gen.ps1` whenever `registers.md` gains or moves an entry.
