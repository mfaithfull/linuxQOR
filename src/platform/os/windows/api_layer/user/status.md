# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 2 Nov 2026 - **session 23 (`message.cpp`) CLOSED**; **U15 CLOSED earlier the same day by user decision: a
policy change, record-only, no code touched** (D14 in `docs/agent/DECISIONS.md`), see "Convention decision" below. HEAD
`a502f6ad` carries session 22; session 23's code + record sit on top (`2bc8e513` is the off-protocol **U6** fix). Review of
`src/platform/os/windows/api_layer/user` (the `User32` wrapper class over `user32.dll`) for correctness, consistency,
safety, undefined behaviour and conformance to the api_layer conventions. Protocol: **one translation unit per session** -
the whole point is not to lose findings between sessions. Small, unambiguous defects get fixed as they are found; anything
that changes library-wide behaviour or needs a decision goes in the register and is *not* touched.

## Where the record lives - start here, then open only what the step needs

| File | Contents | Open it when |
|---|---|---|
| `status.md` (this file; **stay under 180 lines / 18 KB** - **180 lines / 18,429 B after session 23, ceiling 18,432 B: add only by trimming**) | live state, queue, the three convention decisions, register locator, verification gate, current TU pre-flags | every session, first |
| `registers/<file>.md` - **one register entry per file**, index in `registers/README.md` | verbatim register text: P1-P5 `prescan.md`, P6+U27 `p6-u27.md`, G1-G6 `ground-rules.md`, U1-U26 `u<n>.md`, T1-T5 `tasks.md`, totals `census.md` | only the ids named by the locator below or by the current TU's pre-flags |
| `method.md` | the per-TU review method, tooling index, **instrument caveats** | before running an instrument or the gate |
| `archive/tu/<tu>.md` - **one narrative per TU**, index in `archive/README.md` | that TU's narrative + verdict detail | only the TU you just reviewed |
| `archive/session-log.md` | the session-log table (rows 20-23) + closure notes; rows 1-19 in the three `archive/session-log-<range>.md` slices | only while writing the row for the TU you just reviewed |
| `archive/history.md`, `archive/prescan-help.md` | consolidation/split history, consumed `help.cpp` pre-flags | never, unless auditing the record itself |

The rule: **findings, narratives and register text never go in this file.** New finding -> the register file the locator
names; new TU narrative -> `archive/tu/<tu>.md`; new row -> `archive/session-log.md`; new instrument or caveat ->
`method.md`; the session's folder-wide measures -> append to the **end** of `registers/census.md` (append-only, older text
there stays verbatim). Hard rule: **no record file over ~40 KB**, and this file under 180 lines / 18 KB. When a record file
outgrows its context, slice it by exact line range, hash-verify the rejoin, then re-point the indexes and re-run
`tmp/_tmp_status_index_gen_s22.ps1` + `tmp/_tmp_s23_locsplice.ps1` (session 23's register edits moved P3/P4/P5/T5).

## Live state (2 Nov 2026)

- **24 of 38 TUs reviewed** (sessions 1-23); the 14 left are the Queue table below. **Session 23 (`message.cpp`) CLOSED
  2 Nov 2026** - 24 wrappers, the whole `//Message and Message Queue functions` section: **4 fixes** = the entire remaining
  **P2** list -> explicit `qor_pp_useswinapiAW`, behaviour-neutral by `/EP`, so **P2 is CLOSED (0 open)**; plus U14(c)'s last
  `()` -> `(void)` (**U14(c) CLOSED**, folder 38 defs / 38 decls) and 3 comma-space nits. 0 return-type and 0 parameter-type
  divergences, **all 24 live through the layer**, **0 bypass in built code**, P6 second instance, U21 fifth, U16 4/24, U19
  172/12/68/2. Narrative `archive/tu/message.md`, row 23.
- **Session 22 (`menu.cpp`) CLOSED 2 Nov 2026** - 39 wrappers, 2 neutral D13 fixes, 0 return-type divergences (first TU where
  that half of step 2 was measurable), 3 parameter divergences recorded not changed, U16 12/39, U21 fourth. Row 22 + `archive/tu/menu.md`.
- **Sessions 16-20 (`help`, `hook`, `icon`, `ime`, `keyboard`) CLOSED 31 Oct - 1 Nov 2026** - these five have **no
  `archive/tu/` file**, their narrative is the row; `archive/tu/` covers sessions 1-15 and 21-23, so the `archive/tu/ime.md` /
  `archive/tu/keyboard.md` some briefs cited never existed.
- **U6 CLOSED 1 Nov 2026 (off-protocol, not a TU session)** - `Call`/`voidCall` now call through a `__stdcall` `fPtr`
  (`library.h:46`, `:68`), matching the `WINAPI` exports behind all **1,638** bindings; commit `2bc8e513`; U7/G5 open.

## Queue - 14 TUs left, alphabetical (lines = on-disk physical lines, measured 1 Nov 2026)

| # | TU | lines | # | TU | lines |
|--:|---|--:|--:|---|--:|
| 24 | `monitor.cpp` | 51 | 31 | `scrollbar.cpp` | 102 |
| 25 | `mouse.cpp` | 86 | 32 | `systemshutdown.cpp` | 51 |
| 26 | `painting.cpp` | 191 | 33 | `text.cpp` | 44 |
| 27 | `processthread.cpp` | 57 | 34 | `window.cpp` | **615** |
| 28 | `winqor_api_user.cpp` (glue) | 14 | 35 | `windowclass.cpp` | 114 |
| 29 | `rectangle.cpp` | 93 | 36 | `windowprocedure.cpp` | 30 |
| 30 | `resource.cpp` | 30 | 37 | `windowproperties.cpp` | 51 |

1,529 lines remain, **806 of them in two TUs** (`window` 615, `painting` 191). What is left of the binding census is
`mouse.cpp` (10 sites `:19`-`:82`) and `window.cpp:124 CreateWindow` (`tmp/_tmp_p1census_post_s23.txt`) - **P2 is closed**.
The biggest *unflagged* finding is `mouse.cpp`: **all 10 of its wrappers bind a name that does not exist** (`prescan.md` P1),
so that session is a whole-TU binding fix. `window.cpp` (615 lines, 85 bindings, P3 `CreateWindowT`, U18's `void` converters)
may not fit one session - decide how to split it before starting it, not mid-session. Hand-offs the queue must not lose:
`window.cpp` (34) takes the version-guarded MDI members `CascadeWindows`/`TileWindows` (**U16**) plus the DPI / `*Info` /
WinEvent-hook members sharing the monitor SDK region (section (b) of `tmp/_tmp_mon_pre2_s24.txt`); `mouse.cpp` (25) takes P1
and the `AW`-on-a-single-export shape.

## Convention decision

- **D13 (adopted 29 Oct 2026, user decision; `docs/agent/DECISIONS.md`).** Explicit `qor_pp_useswinapiAW(<mod>, Stem)`
  wherever the SDK has an A/W family; bare `qor_pp_useswinapi` only for entry points that are a single export with no A/W
  variants. Never rely on the SDK's `Stem -> StemW` macro surviving into the binding expression (G1); apply as each TU is reviewed.
- **Return-check / error-handler policy (settled 29 Oct 2026, user decision, session 3).** **Leave as-is: no
  `Win32ErrorHandler _;` and no `CheckReturn`/`CheckNonZeroHandle` in these wrappers**, even though `api_layer/shell` and
  `api_layer/kernel` use both and G6 makes `CheckReturn` always active. Sessions still *record* the sites that would take
  them (U1/U2) but change no behaviour - so a Win32 `FALSE`/NULL/0 failure is silent here, and a swallowed SEH fault is
  indistinguishable from a legitimate zero (U7).
- **D14 (adopted 2 Nov 2026, user decision; `docs/agent/DECISIONS.md`) - U15 CLOSED.** An exported wrapper with no
  reachable in-tree caller is published surface, **not a finding**: record no U15 instances and append nothing to
  `registers/u15.md` (its recorded instances are frozen history). Measure reachability only where another item needs it
  (a P1/P5 mis-binding being latent or live, U18's `TrackPopupMenu`, U19's LAYER-vs-BYPASS split), and record it there.

## Register locator - id -> which `registers/` file and line holds its text

A locator, not a finding: open the file at the line given before you act on, extend or close an id - never open the whole
`registers/` tree. Generated by `tmp/_tmp_status_index_gen_s22.ps1`; do not hand-edit it (re-run it and re-splice).

- **P1** `registers/prescan.md:10` bindings that cannot resolve at run time (export table + live probe + source line). Six at the first...
- **P2** `registers/prescan.md:60` bindings that work today only because the SDK macro rewrites them; convert to explicit AW
- **P3** `registers/prescan.md:74` naming flags from the new T-suffix census (`tmp/_tmp_tsuffix_census.ps1`, session 6). Two members ca...
- **P4** `registers/prescan.md:80` four exports in the table are forwarders, which the binding question cannot see (found in session 7)...
- **P5** `registers/prescan.md:88` the "wrong stem bound" defect class, and the instrument that finds it (`tmp/_tmp_wrong_stem_scan.ps1...
- **P6** `registers/p6-u27.md:9` (promoted extract) Bare `CallMsgFilter` resolves and runs the ANSI entry point on wide arguments
- **G1** `registers/ground-rules.md:5` the SDK A/W macro rewrites the binding name before it is stringized. `_ATXT(_NAME)` inside
- **G2** `registers/ground-rules.md:12` the nine stems `user32.h:9-17` `#undef`s are the real missing-export risk. All nine resolve to
- **G3** `registers/ground-rules.md:24` the previous audit report is partly wrong. `tmp/_tmp_audit_report.txt` section A ("run-time
- **G4** `registers/ground-rules.md:30` module token case is cosmetic. `_TXT(User32)` stringizes to `L"User32"` and the module lookup is
- **G5** `registers/ground-rules.md:33` the "already loaded image" assumption is not universal. In a bare console process
- **G6** `registers/ground-rules.md:36` return checking is silently always on. `returncheck.h:19` tests
- **U1** `registers/u1.md:3` no call-site error handler anywhere in this library. Policy settled 29 Oct 2026 (user): accepted as-...
- **U2** `registers/u2.md:3` return-check policy is absent. Policy settled 29 Oct 2026 (user): accepted as-is - no `CheckReturn`
- **U3** `registers/u3.md:3` `hook.cpp` build-list status: RESOLVED (user commit `05d3ca2f`, 29 Oct 2026).
- **U4** `registers/u4.md:3` `user32.h` declares about 101 non-static members that no `.cpp` defines. The class is documented
- **U5** `registers/u5.md:3` the binding is cached for the process lifetime including failure. `qor_pp_useswinapi` expands to
- **U6** `registers/u6.md:3` calling-convention mismatch in the call path: RESOLVED 1 Nov 2026 (off-protocol item fix, not a TU s...
- **U7** `registers/u7.md:3` SEH faults are swallowed into a default return. `Call` catches everything and returns `ret _{}`
- **U8** `registers/u8.md:3` `throw("Missing library function.")` (`library.h:60`, `:92`) throws a `const char*`. Not derived
- **U9** `registers/u9.md:3` no `purpose.md` in this folder and no scope/audit note. `api_layer/shell/purpose.md` and
- **U10** `registers/u10.md:3` `user32.h:19-62` defines `CONSOLECONTROL`, `CONSOLEENDTASK`, `CONSOLEWINDOWOWNER`,
- **U11** `registers/u11.md:3` `resource.cpp` defines both Resource members without the `User32::` qualifier. `resource.cpp:16`
- **U12** `registers/u12.md:3` `accessibility.cpp` and `timer.cpp` held each other's wrappers - FIXED 29 Oct 2026 (session 2).
- **U13** `registers/u13.md:3` one member was defined through `User32::` but declared non-static: `ConsoleControl` - FIXED 30 Oct 2...
- **U14** `registers/u14.md:31` re-measure 1 Nov 2026 (session 20, `keyboard.cpp`). (b) `tmp/_tmp_u14_census.ps1` now reports
- **U14** `registers/u14.md:3` `Library::Call`/`voidCall` template-argument style is split two ways. Measured 29 Oct 2026 over all...
- **U14** `registers/u14.md:55` (c) CLOSED 2 Nov 2026 (session 23, `message.cpp`). The last `()` pair went: definition
- **U15** `registers/u15.md:3` wrappers that are exported through `QOR_WINAPIUSER` but have no reachable caller in the tree: CLOSED...
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
- **T3** `registers/tasks.md:29` the "Power Management functions" pair is declared but has no owning TU at all. Opened 30 Oct 2026 (s...
- **T4** `registers/tasks.md:52` the menu half of the context-help-id family is declared and defined nowhere. Opened 31 Oct 2026 (ses...
- **T5** `registers/tasks.md:87` the IME/WINNLS pair in the unlabelled `user32.h:635-639` block is declared and defined nowhere. Open...

## Verification gate (identical every session)
Edited TU compiles standalone at `/W4` exit 0; the whole library compiles **38/38 at `/W4`, exit 0, no diagnostics**
(`tmp/_tmp_cl_all.bat` -> the shared `tmp/_tmp_cl_all.log`, snapshotted under a session name - never copy an old log as
proof, see `method.md`); `cl /EP` re-run after the last edit still yields the expected binding literals; `user32.h` untouched
unless the row says otherwise. The gate also carries the folder-wide **binding census** (latest
`tmp/_tmp_p1census_post_s23.txt`: 428 bindings, 11 unresolvable, none in the reviewed TU) and the per-stem bare-vs-AW
classifier (`tmp/_tmp_msg_class_s23.ps1`). **Session 23's authoritative run is `tmp/_tmp_cl_all_s23recover.log`** - 13:11,
after the last source edit, 38 echoes, 0 diagnostics, exit 0; the `_s23final`/`_s23pre` names the interrupted pass left are
session 22's log byte-for-byte. Library-header edits must additionally pass the **all-libraries gate**
`tmp/_tmp_cl_all_apiall_u6.bat` (106 sources), meaningful only as a diff against a pre-fix run (pre-existing `C1083` at
`opengl/wgl.cpp:31` plus 165 warnings on HEAD).

## Pre-flags for the current TU (rolling - keep this under ~40 lines)

**Consumed:** session 23's pre-flags are superseded by row 23 + `archive/tu/message.md`; both `CALL TYPE MISMATCH` flags were
instrument artifacts (the tool read this library's own prototype as the SDK side; `PostQuitMessage`'s reported types were the
next function's `Call<>` list) and were resolved without acting on them.

**Session 24 `monitor.cpp`** (51 lines; 5 wrappers = the whole `user32.h:587`-`:592` "//Monitor functions" section, decls
`:588`-`:592` all `static`, defs `:16`-`:49` in header order). Measured 2 Nov 2026 into `tmp/_tmp_mon_pre_s24.txt` (via
`tmp/_tmp_sigcmp.ps1 -Tu monitor.cpp -First 588 -Last 592`), `_tmp_mon_pre2_s24.txt`, `_tmp_mon_guards_s24.txt`,
`_tmp_mon_bypass_s24.txt`:
- **Signature:** 5 decl/def pairs parse, **all `decl == def`, 0 flags** (0 U4, 0 U11, 0 U13); SDK returns `BOOL`, `BOOL`,
  `HMONITOR` x3 match the wrappers -> expect 0 U18. Check by hand that `MonitorFromPoint`'s `::POINT pt` is by value (it is)
  and whether U26's mirror question extends to `POINT`/`MONITORINFO`/`MONITORINFOEX` rather than only `MSG`.
- **Binding:** **1 `AW` (`:26 GetMonitorInfo`), 4 bare** (`:19 EnumDisplayMonitors`, `:33 MonitorFromPoint`, `:40
  MonitorFromRect`, `:47 MonitorFromWindow`). `GetMonitorInfo` has **no bare export** (only `18C GetMonitorInfoA` / `18D
  GetMonitorInfoW`), so the `AW` is the only spelling that can resolve (the `mdi.cpp`/`ime.cpp` class); the other four are
  genuine single exports (`FE`, `28E`, `28F`, `290`) -> **expect 0 P2, 0 P6, 0 U17** (`GetMonitorInfoT` is the only family
  member and carries `T`), but re-prove per stem with a clone of `tmp/_tmp_msg_class_s23.ps1`.
- **U16:** **6 of 6 SDK declarations version-guarded** - one `#if(WINVER >= 0x0500)` at `winuser.h:13594` owns the whole
  monitor block, over the Desktop partition (`:13605`, `:13638`); first TU whose entire section shares a single guard, and
  nothing in this build pins `WINVER`.
- **U21 sixth instance, the largest yet:** `winuser.h` has **no `NOMONITOR` macro at all**, and the owning region
  `:13594`-`:14905` holds **45** declaration heads of which only 6 are monitor (the rest: the DPI family, the `*Info` family,
  the WinEvent-hook pair, `GetAltTabInfo`, the `Real*` pair, `BlockInput`, `InheritWindowMonitor` - full list in section (b)
  of `tmp/_tmp_mon_pre2_s24.txt`); attribute each to its section/TU in the session - the DPI/`*Info` half is `window.cpp`'s
  (34) hand-off.
- **U19: 8 LAYER / 0 INDIRECT / 4 BYPASS / 0 MEMBER** over 8 spellings - **all 8 layer sites are in built code**
  (`ui/gui/controllers/i_m/monitor.cpp:56,62,187,228,236,242,248` + `windowcontroller.cpp:679`; `winqor_gui` is in
  `cmakebuild/QOR.sln`), so **all 5 wrappers are live through the layer**, and **1 bypass is in built code** -
  `monitor.cpp:163` calls the `GetMonitorInfoW` thunk in the *same file* that calls `User32::GetMonitorInfoT` at `:56`/`:62`
  (session 20's three-spellings shape); the other 3 are unbuilt (`ui/gl/monitorimpl.cpp:79`, `ui/gl/windowimpl.cpp:435,1902` -
  `winqor_gl` is commented out at `ui/CMakeLists.txt:4`). **First bypass in built code since U19 opened** (sessions 9/21/22
  read 0): record it, do not fix it here.
- **Style:** `:20` is `Library::Call< BOOL, ...>` (space after the bracket) - the folder is split (179 spaced-angle sites), so
  churn not defect; all 5 `Call<>` sites are explicit (U14b).

