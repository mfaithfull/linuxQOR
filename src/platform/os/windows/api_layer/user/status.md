# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 1 Nov 2026 (record split only - no review findings in this pass). Review of
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
`registers.md`; new TU row/narrative -> `session-log-archive.md`; new instrument or caveat -> `method.md`. When this file
outgrew a session's context the review started cycling - keep it under 150 lines.

## Live state (1 Nov 2026)

- **18 TUs reviewed** (sessions 1-17): `accelerator`, `accessibility`, `brush`, `caret`, `clipboard`, `combobox`,
  `console`, `timer`, `coordtransform`, `cursor`, `desktop`, `devicecontext`, `devicemanagement`, `dialog`,
  `errorhandling`, `filledshapes`, `help`, `hook`.
- **Session 18 (`icon.cpp`) is IN FLIGHT - committed but never closed.** Commit `83d9f019` "Ongoing review of
  winqor_api_user in progress" (merged `e17e57a7`, pushed to `origin/main`); working tree clean. Measured in that commit:
  `icon.cpp` 114 -> 169 lines (+63/-8, TU comment block included), `user32.h` +4/-4, `hook.cpp` +66/-2 (session 17).
  The gate ran after those edits and is green: 38/38 at `/W4`, `tmp/_tmp_cl_all.log` at 01/11 00:17. **Still missing: the
  row-18 session-log entry, the `icon` tallies in U15/U16/U17/U19/U20, and a census refresh.** Write them from
  `icon.cpp`'s own comment block plus `tmp/_tmp_icon_final.txt`, `_tmp_icon_liveness.txt`, `_tmp_icon_class.txt`,
  `_tmp_icon_pre18.txt`, `_tmp_icon_own_probe.cpp` (drivers: `_tmp_icon_pre18.bat`, `_tmp_run_icon_own_probe.bat`).
- **First job of the next session: close session 18.** Then open session 19 on `ime.cpp`.

## Queue - 20 TUs left, alphabetical (lines = on-disk physical lines, measured 1 Nov 2026)

| # | TU | lines | # | TU | lines |
|--:|---|--:|--:|---|--:|
| 18 | `icon.cpp` **in flight** | 169 | 28 | `rectangle.cpp` | 93 |
| 19 | `ime.cpp` | 23 | 29 | `resource.cpp` | 30 |
| 20 | `keyboard.cpp` | 254 | 30 | `scrollbar.cpp` | 102 |
| 21 | `mdi.cpp` | 44 | 31 | `systemshutdown.cpp` | 51 |
| 22 | `menu.cpp` | 289 | 32 | `text.cpp` | 44 |
| 23 | `message.cpp` | 184 | 33 | `window.cpp` | **615** |
| 24 | `monitor.cpp` | 51 | 34 | `windowclass.cpp` | 114 |
| 25 | `mouse.cpp` | 86 | 35 | `windowprocedure.cpp` | 30 |
| 26 | `painting.cpp` | 191 | 36 | `windowproperties.cpp` | 51 |
| 27 | `processthread.cpp` | 57 | 37 | `winqor_api_user.cpp` (glue) | 14 |

2,492 lines in total, and **1,533 of them (62%) are in five TUs**: `window`, `menu`, `keyboard`, `painting`, `message`.
Those same five hold every remaining flagged defect: **P1 x5** (`keyboard.cpp` x3, `window.cpp:124`, `mouse.cpp:82`) and
**P2 x6** (`menu.cpp` x2, `message.cpp` x4). `window.cpp` (615 lines, 85 bindings of which 75 bare, plus P3
`CreateWindowT` and U18's `void`-returning point converters) is the one TU that may not fit a single session - decide how
to split it before starting it, do not discover it mid-session.

## Convention decision

- **D13 (adopted 29 Oct 2026, user decision; recorded in `docs/agent/DECISIONS.md`).** Explicit
  `qor_pp_useswinapiAW(<mod>, Stem)` wherever the SDK has an A/W family; the bare `qor_pp_useswinapi` is
  reserved for entry points that are a single export with no A/W variants (`DestroyAcceleratorTable`,
  `ShowWindow`, `GetComboBoxInfo`, ...). Never rely on the SDK's `Stem -> StemW` macro surviving into the
  binding expression (G1). Apply this in every TU as it is reviewed; `accelerator.cpp` already conforms.
- **Return-check / error-handler policy in this library (settled 29 Oct 2026, user decision, session 3).** **Leave
  it as-is: no `Win32ErrorHandler _;` and no `CheckReturn`/`CheckNonZeroHandle` is to be added to these wrappers**,
  even though `api_layer/shell` and `api_layer/kernel` use both and G6 makes `CheckReturn` always active. Each TU
  session still *records* which call sites would take them (U1/U2 instances) so the decision can be revisited with
  complete data, but no session changes behaviour. Consequence to keep in mind while reviewing: a failure that
  Win32 reports as `FALSE`/NULL/0 is silent in this library, and a swallowed SEH fault is indistinguishable from a
  legitimate zero (U7).


## Register locator - id -> where its text lives in `registers.md`

A locator, not a finding: open `registers.md` at the line given before you act on, extend or close an id. Each summary is
the first line of that entry as written at session 17.

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
- **U16** `registers.md:426` the wrappers pair with SDK declarations that the SDK only emits when a version guard passes, and not...
- **U17** `registers.md:536` A/W wrappers declared without the folder's `T` suffix. Opened 29 Oct 2026 (session 6) when the two c...
- **U18** `registers.md:573` a wrapper pair whose return type contradicts the SDK: `void` where `user32.dll` returns `BOOL`. Open...
- **U19** `registers.md:587` call sites that reach a `user32.dll` entry point through the SDK import thunk instead of through thi...
- **U20** `registers.md:684` the caller-side `Cursor`/`Icon` ownership model is wrong at the sites this layer feeds. Opened 30 Oc...
- **U21** `registers.md:720` the "DC functions" section is not the SDK's DC family, and the SDK's DC family is split across two T...
- **U22** `registers.md:736` the `DeviceContext` release path this layer feeds calls `ReleaseDC` with a NULL `HWND`. Opened 30 Oc...
- **U23** `registers.md:754` `MessageBoxIndirectT`'s parameter puts `const` on the pointer instead of the pointee. Opened 30 Oct...
- **U24** `registers.md:769` the one in-tree `FlashWindow` caller reads its `BOOL` as a success flag. Opened 31 Oct 2026 (session...
- **U25** `registers.md:790` `const` is spelled three ways in `user32.h`, and the `CONST::` spelling is an SDK macro glued to a s...
- **U26** `registers.md:818` `TCHAR` width is an include-order property, not a build setting. Opened 31 Oct 2026 (session 16,
- **T1** `registers.md:842` two declared-but-undefined members in the "Authorization functions" section. `user32.h:658-659` decl...
- **T2** `registers.md:855` the whole Window Station / Desktop surface is dead exported API, and its conventions are therefore u...
- **T3** `registers.md:864` the "Power Management functions" pair is declared but has no owning TU at all. Opened 30 Oct 2026 (s...
- **T4** `registers.md:887` the menu half of the context-help-id family is declared and defined nowhere. Opened 31 Oct 2026 (ses...
- **P6** `registers.md:77` no standalone entry - see the "P6 and U27" section; bare `CallMsgFilter` resolves and runs the ANSI entry point on wide arguments
- **U27** `registers.md:77` no standalone entry - same section; `STRICT` changes the identity of `HOOKPROC` and every `DECLARE_HANDLE` type, so 3 of the 4 hook wrappers change mangled name

## Defect census

- The sessions-1-16 census (run-time defects fixed, what is still open, convention fixes, folder-wide measures) is kept verbatim at `registers.md:916` - refresh it there when a session closes.

### Verification gate (identical every session)
Edited TU compiles standalone at `/W4` exit 0; the whole library compiles **38/38 at `/W4`, exit 0, no diagnostics**;
`cl /EP` re-run after the edit still yields the expected binding literals; `user32.h` untouched unless the row says
otherwise. Session logs: `_tmp_cl_all_s9.log`, `_tmp_cl_all_s11.log`/`_s11b`, `_tmp_cl_all_s12.log`,
`_tmp_cl_all_s13.log` (`exit=0 loglines=43`, 44 objects in `tmp/obj`), sessions 14-16 via `cmd /c tmp/_tmp_cl_all.bat`
(`cl_exit=0`, 42-43-line log). The single `warning|error` grep hit in those logs is the **filename** `errorhandling.cpp`.


## Pre-flags for the current TU (rolling - keep this under ~40 lines)

None pending. Session 18's `icon.cpp` pre-flags were consumed and moved to `session-log-archive.md` under "Moved out of
`status.md` on 1 Nov 2026"; the raw instrument output is still in `tmp/_tmp_icon_*`. When you open session 19, measure and
write `ime.cpp`'s pre-flags HERE, then move them to the archive when that session closes.

## How this record is split (1 Nov 2026)

`status.md` was 1,222 lines / 145 KB (open items 709 lines = 52%, session-log table 25.6 KB in 92 lines) and every
session had to hold it all, which is what caused the context failures and compaction cycles. Review depth is unchanged:
the record was sliced by section - `tmp/_tmp_status_split.ps1` then `tmp/_tmp_status_index_gen.ps1`, pre-split file kept
at `tmp/_tmp_status_presplit_backup.md`. 1,209 of 1,222 lines moved verbatim (the 13-line gap is this header); all 41
standalone register entries are indexed above; P6 and U27 had no standalone entry (they lived only inside the session-log
row-17 cell), so the split promoted their text verbatim into the "P6 and U27" section of `registers.md`.
