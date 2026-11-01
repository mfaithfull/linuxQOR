# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 1 Nov 2026 (session 19 `ime.cpp` closed - code in `f2f87453`, record work in this pass). Review of
`src/platform/os/windows/api_layer/user` (the `User32` wrapper class over `user32.dll`) for correctness, consistency,
safety, undefined behaviour and conformance to the api_layer conventions. Protocol: **one translation unit per session** -
the whole point is not to lose findings between sessions. Small, unambiguous defects get fixed as they are found; anything
that changes library-wide behaviour or needs a decision goes in the register and is *not* touched.

## Where the record lives - start here, then open only what the step needs

| File | Contents | Open it when |
|---|---|---|
| `status.md` (this file; **stay under 180 lines / 18 KB** - 176 after the session-19 update) | live state, queue, the two convention decisions, register locator, defect census pointer, verification gate, current TU pre-flags | every session, first |
| `registers/<file>.md` - **one register entry per file**, index in `registers/README.md` | verbatim register text: P1-P5 `prescan.md`, P6+U27 `p6-u27.md`, G1-G6 `ground-rules.md`, U1-U26 `u<n>.md`, T1-T4 `tasks.md`, census `census.md` | only the ids named by the locator below or by the current TU's pre-flags |
| `method.md` | the per-TU review method, tooling index, **instrument caveats** | before running an instrument or the gate |
| `archive/tu/<tu>.md` - **one narrative per TU**, index in `archive/README.md` | that TU's narrative + verdict detail | only the TU you just reviewed |
| `archive/session-log.md` | the session-log table (rows 1-19) + closure notes | only while writing the row for the TU you just reviewed |
| `archive/history.md`, `archive/prescan-help.md` | consolidation/split history, consumed `help.cpp` pre-flags | never, unless auditing the record itself |

The rule this split exists to enforce: **findings, narratives and register text never go in this file.** New finding ->
the register file the locator names; new TU narrative -> `archive/tu/<tu>.md`; new row -> `archive/session-log.md`; new
instrument or caveat -> `method.md`. This file outgrew a session's context once and the review started cycling; the same
happened to `registers.md` (1,063 lines) and `session-log-archive.md` (1,394 lines), so both were split on 1 Nov 2026.
Hard rule now: **no record file over ~40 KB**, and the accumulating entries (`u15`, `u16`, `u17`, `u19`, `u20`) are read
for their **head only** (the rule) and appended to at the **end** - never read the whole tally back.


## Live state (1 Nov 2026)

- **20 TUs reviewed** (sessions 1-19): `accelerator`, `accessibility`, `brush`, `caret`, `clipboard`, `combobox`,
  `console`, `timer`, `coordtransform`, `cursor`, `desktop`, `devicecontext`, `devicemanagement`, `dialog`,
  `errorhandling`, `filledshapes`, `help`, `hook`, `icon`, `ime`.
- **Session 19 (`ime.cpp`) CLOSED 1 Nov 2026** - 1 wrapper, **0 binding / correctness / UB / convention defects**; its
  `qor_pp_useswinapiAW` was already D13-correct and is load-bearing (bare `SendIMEMessageEx` is MISSING in the export
  table, and no SDK macro is in scope to rewrite it). Row 19 of `archive/session-log.md`; register writes to **U4**
  (2 members + an instrument blind spot that had under-counted U4 by one), **U15** 1/1, **U16** 0/1 + the opt-in-header
  sub-class, **U17** 1, **U19** 1/1/0/0, **U26** (second mirror-struct instance) and new **T5**. Code in `f2f87453`
  (includes the first `user32.h` edit since session 7 - kept at 695 lines deliberately); gate green **38/38** at `/W4`,
  zero diagnostics (`tmp/_tmp_cl_all_s19c.log`).
- **Session 20 (`keyboard.cpp`) OPEN** - 34 wrappers, the TU holding the folder's largest known defect count: 3 of the
  5 remaining **P1** bindings. Measured pre-flags below; verdict and fixes to be agreed with the user.

## Queue - 18 TUs left, alphabetical (lines = on-disk physical lines, measured 1 Nov 2026)

| # | TU | lines | # | TU | lines |
|--:|---|--:|--:|---|--:|
| 20 | `keyboard.cpp` | 254 | 29 | `rectangle.cpp` | 93 |
| 21 | `mdi.cpp` | 44 | 30 | `resource.cpp` | 30 |
| 22 | `menu.cpp` | 289 | 31 | `scrollbar.cpp` | 102 |
| 23 | `message.cpp` | 184 | 32 | `systemshutdown.cpp` | 51 |
| 24 | `monitor.cpp` | 51 | 33 | `text.cpp` | 44 |
| 25 | `mouse.cpp` | 86 | 34 | `window.cpp` | **615** |
| 26 | `painting.cpp` | 191 | 35 | `windowclass.cpp` | 114 |
| 27 | `processthread.cpp` | 57 | 36 | `windowprocedure.cpp` | 30 |
| 28 | `winqor_api_user.cpp` (glue) | 14 | 37 | `windowproperties.cpp` | 51 |

2,300 lines in total, and **1,533 of them (67%) are in five TUs**: `window`, `menu`, `keyboard`, `painting`, `message` -
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
- **T5** `registers/tasks.md:79` the IME/WINNLS pair in the unlabelled `user32.h:635-639` block is declared and defined nowhere. Open...

## Defect census

- The sessions-1-19 census (run-time defects fixed, what is still open, convention fixes, folder-wide measures) is kept verbatim at `registers/census.md:3` - refresh it there when a session closes.

### Verification gate (identical every session)
Edited TU compiles standalone at `/W4` exit 0; the whole library compiles **38/38 at `/W4`, exit 0, no diagnostics**;
`cl /EP` re-run after the edit still yields the expected binding literals; `user32.h` untouched unless the row says
otherwise. Latest closure run `tmp/_tmp_cl_all_s19c.log` (exit 0, 38 TU echo lines, 0 hits for `error C|warning C|fatal`); earlier
runs `_tmp_cl_all_s9/s11*/s12/s13/s17/s18/s19b.log`.

## Pre-flags for the current TU (rolling - keep this under ~40 lines)

**`keyboard.cpp` (session 20) - measured 1 Nov 2026.** Raw output: `tmp/_tmp_kb_pre.txt`, `_tmp_kb_pre2.txt`,
`_tmp_kb_pp.txt` (`_tmp_pp_kb.bat` read by `_tmp_kb_lits_s20.ps1`), `_tmp_kb_guards_s19.txt`, `_tmp_kb_u18_s20.txt`.
- **Section `user32.h:70-104` `//Keyboard Input functions`: 34 declarations, all `static`, all 34 defined here**
  (`keyboard.cpp:16`..`:247`, header order); 0 non-static, so 0 U4/U13. File shape: 254 lines, **no TU comment block** (3
  comment lines), pure ASCII, 0 tabs, 0 trailing whitespace, CRLF, final LF.
- **The folder's 3 remaining P1 bindings are here.** The `/EP` literals ending in an uppercase `T` are exactly three -
  `GetKeyboardLayoutNameT` (`:82`), `VkKeyScanT` (`:243`), `VkKeyScanExT` (`:250`) - and the export table has **0** names
  ending in uppercase `T` (case-sensitive), so each caches a NULL `pFunc` and throws on first call (G5). All three are
  real A/W families (`GetKeyboardLayoutName` 16D/16E, `VkKeyScan` 3E1/3E4, `VkKeyScanEx` 3E2/3E3), so the D13 fix is
  `qor_pp_useswinapiAW(user32, <stem without the T>)`. P5 **cannot see this class** (stem == member name, `T` included);
  the instrument was extended for `*T` stems on 1 Nov and flags 0 here.
- **7 A/W families in the section, 4 already `AW`** (`GetKeyNameText` `:103`, `LoadKeyboardLayout` `:138`,
  `MapVirtualKey` `:145`, `MapVirtualKeyEx` `:152` -> `*W` literals); the other 27 are bare and all single exports.
  Post-fix shape: 7 `AW` + 27 bare, and the folder's P1 count drops to 2 (`window.cpp`, `mouse.cpp`).
- **sigcmp 34/34 `decl==def ok`, 0 parameter-name divergence** (folder zero holds; scan re-run 1 Nov, 491 decls). The
  two `CALL ARITY MISMATCH` rows (`GetKeyboardLayout` def `:65`, `GetKeyboardLayoutList` def `:72`) are U14(b)'s
  deducing sites `Call< HKL >` `:69` / `Call< UINT >` `:76` - 2 of the folder's 6, made explicit this session. The
  `CALL TYPE MISMATCH` at `keybd_event` (def `:128`) is a **sigcmp false positive**: the site is
  `Library::voidCall< BYTE, BYTE, DWORD, void* >` (`:132`), correct; the script compared the *next* function's list.
- **U18: 1 return-type divergence** (`tmp/_tmp_kb_u18_s20.ps1`): `GetKeyboardLayoutList` is `UINT` here (`:79`) against
  the SDK's `int` (`winuser.h:1310-1315`) - same width, no ABI change, and a published return type is a surface
  decision, so record it. Same for the `void* dwExtraInfo` parameter of `keybd_event` against SDK `ULONG_PTR`
  (`winuser.h:6068-6075`), same width on both ABIs. `GetKBCodePage()` (`:77`, def `:58`) is a surviving U14(c) `()` list.
- **U16: 16 of the 34 SDK prototypes sit behind a `WINVER`/`_WIN32_WINNT` guard, the other 18 Desktop-only** - use the
  per-name list in `tmp/_tmp_kb_guards_s19.txt`, not its `guarded names: 34` header (which counts a name with an empty
  guard list as guarded). Extremes: `GetAsyncKeyState` (`_WIN32_WINNT >= 0x0604`), `SendInput` (0x0603).
- **U15: 29 of 34 unreachable** - only 5 LAYER sites: `EnableWindow` `windowcontroller.cpp:72`, `IsWindowEnabled` `:77`,
  `RegisterHotKey` `:82`, `SetFocus` `:839`, `UnregisterHotKey` `:87`. **U19: 5 LAYER / 7 INDIRECT / 10 BYPASS /
  2 MEMBER - the folder's largest bypass count by far**: `ToUnicode` x2 (`ui/gl/session.cpp:154,162`), `GetKeyState` x6
  (`ui/gl/windowimpl.cpp:414-424`), `SetFocus` (`retropad.cpp:842`). Caveat: `windowcontroller.h:310` is a member
  *declaration* not a call, and 2 of the 7 INDIRECT are Linux (`os/linux/ui/x/xinput/device.cpp:104,109`).
- **U21-class placement:** 7 of the 34 are SDK *Window* entry points parked in the keyboard section - `BlockInput`
  `:72`, `EnableWindow` `:73`, `GetActiveWindow` `:74`, `GetFocus` `:76`, `IsWindowEnabled` `:86`, `SetActiveWindow`
  `:94`, `SetFocus` `:95` - once each, none in `//Window functions` (`:128-213`); `GetKBCodePage` is real (`:5918`, 167).
- **U17: 0 instances** - all 7 A/W members carry the suffix (`:80`, `:83`, `:88`, `:89`, `:90`, `:103`, `:104`).

## How this record is split

Both 1 Nov 2026 splits are described in `archive/history.md` ("How the record was split"): `status.md` (1,222 lines)
into four files, then `registers.md` (1,063) into `registers/` and `session-log-archive.md` (1,394) into `archive/`, by
exact line range, verified by re-joining every slice and hashing the result against the original bytes (both `MATCH`).
**Regenerate the locator above with `tmp/_tmp_status_index_gen_s19.ps1` whenever a register entry moves.**
