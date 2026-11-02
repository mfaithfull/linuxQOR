# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 2 Nov 2026 - **session 24 (`monitor.cpp`) CLOSED**; **U15 CLOSED the same day by user decision** (policy
change, record-only, no code touched; D14 in `docs/agent/DECISIONS.md`). HEAD `51bf18c7` carries session 23; **session 24's
code + record are **uncommitted** (15 modified + 1 new per `git status`: 13 under `api_layer/user` - `monitor.cpp`, `method.md`,
`status.md`, 7 registers, 2 `archive/` files, new `archive/tu/monitor.md` - plus both `memory-bank/` files; `user32.h`
untouched at 695 lines). Review of
`src/platform/os/windows/api_layer/user` (the `User32` wrapper class over `user32.dll`) for correctness, consistency,
safety, undefined behaviour and conformance to the api_layer conventions. Protocol: **one translation unit per session** -
the whole point is not to lose findings between sessions. Small, unambiguous defects get fixed as they are found; anything
that changes library-wide behaviour or needs a decision goes in the register and is *not* touched.

## Where the record lives - start here, then open only what the step needs

| File | Contents | Open it when |
|---|---|---|
| `status.md` (this file; **stay under 180 lines / 18 KB** - **180 lines / 17,011 B after session 24's close-out, ceiling 180 lines / 18,432 B**) | live state, queue, the three convention decisions, register locator, verification gate, current TU pre-flags | every session, first |
| `registers/<file>.md` - **one register entry per file**, index in `registers/README.md` | verbatim register text: P1-P5 `prescan.md`, P6+U27 `p6-u27.md`, G1-G6 `ground-rules.md`, U1-U26 `u<n>.md`, T1-T5 `tasks.md`, totals `census.md` | only the ids named by the locator below or by the current TU's pre-flags |
| `method.md` | the per-TU review method, tooling index, **instrument caveats** (**39.1 KiB of the ~40 KiB ceiling - slice it before adding session-25 tooling**) | before running an instrument or the gate |
| `archive/tu/<tu>.md` - **one narrative per TU**, index in `archive/README.md` | that TU's narrative + verdict detail | only the TU you just reviewed |
| `archive/session-log.md` | the session-log table (rows 20-24) + closure notes; rows 1-19 in the three `archive/session-log-<range>.md` slices | only while writing the row for the TU you just reviewed |
| `archive/history.md`, `archive/prescan-help.md` | consolidation/split history, consumed `help.cpp` pre-flags | never, unless auditing the record itself |

The rule: **findings, narratives and register text never go in this file.** New finding -> the register file the locator
names; new TU narrative -> `archive/tu/<tu>.md`; new row -> `archive/session-log.md`; new instrument or caveat ->
`method.md`; folder-wide measures -> append to the **end** of `registers/census.md` (append-only). Hard rule: **no record
file over ~40 KB**, this file under 180 lines / 18 KB. When one outgrows its context, slice by exact line range, hash-verify
the rejoin, re-point the indexes, re-run the locator generator + `_tmp_s23_locsplice.ps1`.

## Live state (2 Nov 2026)

- **25 of 38 TUs reviewed** (sessions 1-24); the 13 left are the Queue table below. **Session 24 (`monitor.cpp`) CLOSED
  2 Nov 2026** - 5 wrappers, the whole `//Monitor functions` section, **0 fixes**: the first TU with nothing to change
  (every D13/U14/U18/U25/U27 check passed as found), so its value is measurement - **U16 6/6** (whole section under one
  `WINVER >= 0x0500` guard), **U19's first bypass in built code**, **U21 sixth instance** (45-head SDK region: 6 heads
  here, 17 in seven other TUs, 22 unwrapped) with a **U4 hand-off** (`GetListBoxInfo`, declared `user32.h:548`, defined
  nowhere), **U25 fifth spelling**, **U26 fourth instance** (`MONITORINFO`/`MONITORINFOEX` mirrors sound on x64 + x86).
  Narrative `archive/tu/monitor.md`, row 24.
- **Session 23 (`message.cpp`) CLOSED 2 Nov 2026** - 24 wrappers: the whole remaining **P2** list -> explicit `AW`
  (**P2 CLOSED**), U14(c)'s last `()` -> `(void)` (**U14(c) CLOSED**), 3 spacing nits; 0 signature divergences, all 24
  live, 0 bypass in built code, P6 second instance, U21 fifth, U16 4/24, U19 172/12/68/2. Row 23 + `archive/tu/message.md`.
- **Sessions 16-20 (`help`, `hook`, `icon`, `ime`, `keyboard`) CLOSED 31 Oct - 1 Nov 2026** - no `archive/tu/` file for
  these five, their narrative is the row; `archive/tu/` covers sessions 1-15 and 21-24.
- **U6 CLOSED 1 Nov 2026** (off-protocol, not a TU session) - `Call`/`voidCall` now call through a `__stdcall` `fPtr`
  (`library.h:46`, `:68`) matching the `WINAPI` exports behind all **1,638** bindings; commit `2bc8e513`; U7/G5 open.

## Queue - 13 TUs left, alphabetical (lines = on-disk physical lines, measured 2 Nov 2026)

| # | TU | lines | # | TU | lines |
|--:|---|--:|--:|---|--:|
| 25 | `mouse.cpp` | 86 | 32 | `systemshutdown.cpp` | 51 |
| 26 | `painting.cpp` | 191 | 33 | `text.cpp` | 44 |
| 27 | `processthread.cpp` | 57 | 34 | `window.cpp` | **615** |
| 28 | `winqor_api_user.cpp` (glue) | 14 | 35 | `windowclass.cpp` | 114 |
| 29 | `rectangle.cpp` | 93 | 36 | `windowprocedure.cpp` | 30 |
| 30 | `resource.cpp` | 30 | 37 | `windowproperties.cpp` | 51 |
| 31 | `scrollbar.cpp` | 102 | | | |

1,478 lines remain, **806 of them in two TUs** (`window` 615, `painting` 191). What is left of the binding census is
`mouse.cpp` (10 sites `:19`-`:82`) and `window.cpp:124 CreateWindow` (`tmp/_tmp_p1census_post_s24.txt`) - **P2 is closed**,
and session 24 upgraded `mouse.cpp` from "flagged" to **live and whole-TU**: 10 unresolvable bindings, 10 live call sites.
`window.cpp` (615 lines, 85 bindings, P3 `CreateWindowT`, U18's `void` converters) may not fit one session - decide how to
split it before starting it, not mid-session. Hand-offs the queue must not lose: `window.cpp` (34) takes the version-guarded
MDI members `CascadeWindows`/`TileWindows` (**U16**) plus the 17 DPI / `*Info` / WinEvent-hook heads that share the monitor
SDK region (measured, not guessed: `tmp/_tmp_mon_region_s24.txt`); `mouse.cpp` (25) takes P1; **T6** in `tasks.md` carries
the ownerless `//List Box functions` section this scan found.

## Convention decision

- **D13 (adopted 29 Oct 2026, user decision; `docs/agent/DECISIONS.md`).** Explicit `qor_pp_useswinapiAW(<mod>, Stem)`
  wherever the SDK has an A/W family; bare `qor_pp_useswinapi` only for a single export with no A/W variants. Never rely on
  the SDK's `Stem -> StemW` macro surviving into the binding expression (G1); apply as each TU is reviewed.
- **Return-check / error-handler policy (settled 29 Oct 2026, user decision, session 3) - leave as-is: no
  `Win32ErrorHandler _;`, no `CheckReturn`/`CheckNonZeroHandle` here**, though `api_layer/shell` and `api_layer/kernel` use
  both and G6 makes `CheckReturn` always active. Sessions *record* the sites (U1/U2), change nothing - a `FALSE`/NULL/0
  failure is silent and a swallowed SEH fault is indistinguishable from a legitimate zero (U7).
- **D14 (adopted 2 Nov 2026, user decision; `docs/agent/DECISIONS.md`) - U15 CLOSED.** An exported wrapper with no
  reachable in-tree caller is published surface, **not a finding**: record none, append nothing to `registers/u15.md`
  (frozen). Measure reachability only where another item needs it (P1/P5 latent-vs-live, U18's `TrackPopupMenu`, U19's
  LAYER-vs-BYPASS split) and record it there.

## Register locator - id -> which `registers/` file and line holds its text

A locator, not a finding: open the file at the line given before you act on, extend or close an id - never open the whole
`registers/` tree. Generated by `tmp/_tmp_status_index_gen_s22.ps1` (descriptions capped at 60 chars); never hand-edit it.

- **P1** `registers/prescan.md:10` bindings that cannot resolve at run time (export table + liv...
- **P2** `registers/prescan.md:60` bindings that work today only because the SDK macro rewrites...
- **P3** `registers/prescan.md:74` naming flags from the new T-suffix census (`tmp/_tmp_tsuffix...
- **P4** `registers/prescan.md:80` four exports in the table are forwarders, which the binding...
- **P5** `registers/prescan.md:88` the "wrong stem bound" defect class, and the instrument that...
- **P6** `registers/p6-u27.md:9` (promoted extract) Bare `CallMsgFilter` resolves and runs the ANSI entry point on wide arguments
- **G1** `registers/ground-rules.md:5` the SDK A/W macro rewrites the binding name before it is str...
- **G2** `registers/ground-rules.md:12` the nine stems `user32.h:9-17` `#undef`s are the real missin...
- **G3** `registers/ground-rules.md:24` the previous audit report is partly wrong. `tmp/_tmp_audit_r...
- **G4** `registers/ground-rules.md:30` module token case is cosmetic. `_TXT(User32)` stringizes to...
- **G5** `registers/ground-rules.md:33` the "already loaded image" assumption is not universal. In a...
- **G6** `registers/ground-rules.md:36` return checking is silently always on. `returncheck.h:19` te...
- **U1** `registers/u1.md:3` no call-site error handler anywhere in this library. Policy...
- **U2** `registers/u2.md:3` return-check policy is absent. Policy settled 29 Oct 2026 (u...
- **U3** `registers/u3.md:3` `hook.cpp` build-list status: RESOLVED (user commit `05d3ca2...
- **U4** `registers/u4.md:3` `user32.h` declares about 101 non-static members that no `.c...
- **U5** `registers/u5.md:3` the binding is cached for the process lifetime including fai...
- **U6** `registers/u6.md:3` calling-convention mismatch in the call path: RESOLVED 1 Nov...
- **U7** `registers/u7.md:3` SEH faults are swallowed into a default return. `Call` catch...
- **U8** `registers/u8.md:3` `throw("Missing library function.")` (`library.h:60`, `:92`)...
- **U9** `registers/u9.md:3` no `purpose.md` in this folder and no scope/audit note. `api...
- **U10** `registers/u10.md:3` `user32.h:19-62` defines `CONSOLECONTROL`, `CONSOLEENDTASK`,...
- **U11** `registers/u11.md:3` `resource.cpp` defines both Resource members without the `Us...
- **U12** `registers/u12.md:3` `accessibility.cpp` and `timer.cpp` held each other's wrappe...
- **U13** `registers/u13.md:3` one member was defined through `User32::` but declared non-s...
- **U14** `registers/u14.md:31` re-measure 1 Nov 2026 (session 20, `keyboard.cpp`). (b) `tmp...
- **U14** `registers/u14.md:55` (c) CLOSED 2 Nov 2026 (session 23, `message.cpp`). The last...
- **U14** `registers/u14.md:3` `Library::Call`/`voidCall` template-argument style is split...
- **U15** `registers/u15.md:3` wrappers that are exported through `QOR_WINAPIUSER` but have...
- **U16** `registers/u16.md:3` the wrappers pair with SDK declarations that the SDK only em...
- **U17** `registers/u17.md:3` A/W wrappers declared without the folder's `T` suffix. Opene...
- **U18** `registers/u18.md:3` a wrapper pair whose return type contradicts the SDK: `void`...
- **U19** `registers/u19.md:3` call sites that reach a `user32.dll` entry point through the...
- **U20** `registers/u20.md:3` the caller-side `Cursor`/`Icon` ownership model is wrong at...
- **U21** `registers/u21.md:3` the "DC functions" section is not the SDK's DC family, and t...
- **U22** `registers/u22.md:3` the `DeviceContext` release path this layer feeds calls `Rel...
- **U23** `registers/u23.md:3` `MessageBoxIndirectT`'s parameter puts `const` on the pointe...
- **U24** `registers/u24.md:3` the one in-tree `FlashWindow` caller reads its `BOOL` as a s...
- **U25** `registers/u25.md:3` `const` is spelled three ways in `user32.h`, and the `CONST:...
- **U26** `registers/u26.md:3` `TCHAR` width is an include-order property, not a build sett...
- **U27** `registers/p6-u27.md:9` (same extract) STRICT changes the identity of HOOKPROC and every DECLARE_HANDLE type
- **T1** `registers/tasks.md:5` two declared-but-undefined members in the "Authorization fun...
- **T2** `registers/tasks.md:18` the whole Window Station / Desktop surface is dead exported...
- **T3** `registers/tasks.md:29` the "Power Management functions" pair is declared but has no...
- **T4** `registers/tasks.md:52` the menu half of the context-help-id family is declared and...
- **T5** `registers/tasks.md:87` the IME/WINNLS pair in the unlabelled `user32.h:635-639` blo...
- **T6** `registers/tasks.md:102` the whole "//List Box functions" section is declared and def...

## Verification gate (identical every session)
Edited TU compiles standalone at `/W4` exit 0; the whole library compiles **38/38 at `/W4`, exit 0, no diagnostics**
(`tmp/_tmp_cl_all.bat` -> the shared `tmp/_tmp_cl_all.log`, snapshotted under a session name - never copy an old log as
proof, see `method.md`; only the TU-name echoes may appear); `cl /EP` re-run after the last edit still yields the expected
binding literals; `user32.h` untouched unless the row says otherwise. The gate also carries the folder-wide **binding
census** (latest `tmp/_tmp_p1census_post_s24b.txt`: 428 bindings, 11 unresolvable, none in the reviewed TU) and the per-stem
bare-vs-AW classifier (`tmp/_tmp_mon_class_s24.ps1`). **Session 24's authoritative gate is `tmp/_tmp_cl_all_s24b.log`** -
19:08, 13 min after `monitor.cpp`'s only source edit (18:55), 38 echoes, 0 diagnostics; `_tmp_cl_all_s24.log` (18:22) is
the pre-edit run, and session 23's `_s23final`/`_s23pre` names are session 22's log byte-for-byte. Library-header edits
must additionally pass the **all-libraries gate** `tmp/_tmp_cl_all_apiall_u6.bat` (106 sources), meaningful only as a diff
against a pre-fix run (pre-existing `C1083` at `opengl/wgl.cpp:31` plus 165 warnings on HEAD).

## Pre-flags for the current TU (rolling - keep this under ~40 lines)

**Consumed:** session 23's and session 24's pre-flags are superseded by their rows + `archive/tu/`. Pre-flags are leads,
never facts - session 24's U21 hand-off ("`window.cpp` (34)") was a guess; measured, it is 6 / 17 / 22.

**Session 25 `mouse.cpp`** (86 lines; 10 wrappers = the whole `user32.h:555`-`:565` "//Mouse functions" section, decls
`:556`-`:565` all `static`, defs `:16`-`:79` in header order). Measured 2 Nov 2026 into `tmp/_tmp_mouse_pre_s25.txt`
(`_tmp_sigcmp.ps1 -Tu mouse.cpp -First 555 -Last 565`), `_tmp_mouse_pp_pre_s25.txt`, `_tmp_mouse_guards_s25.txt`,
`_tmp_mouse_bypass_s25.txt`:
- **Binding - a live whole-TU P1 fix, inverse of the P2 class:** all 10 bindings are `qor_pp_useswinapiAW`
  (`:19`, `:26`, `:33`, `:40`, `:47`, `:54`, `:61`, `:68`, `:75`, `:82`) and **no mouse stem has an A/W family**: `/EP`
  asks for `DragDetectW`, `GetCaptureW`, `GetDoubleClickTimeW`, `GetMouseMovePointsExW`, `mouse_eventW`,
  `ReleaseCaptureW`, `SetCaptureW`, `SetDoubleClickTimeW`, `SwapMouseButtonW`, `LPTRACKMOUSEEVENTW` - **0 of the 10 in the
  export table**, which has the bare stems (ordinals: `tmp/_tmp_user32_exports.txt`). Per D13 the fix is **bare on all
  ten**; today `pFunc` caches NULL for the process lifetime (U5) and each call throws (G1/G5). 10 of the folder's 11
  unresolvable (`tmp/_tmp_p1census_post_s24.txt`).
- **P5's second instance:** `:82` binds the **parameter type** `LPTRACKMOUSEEVENT` rather than the member's stem
  (`tmp/_tmp_wrongstem_s24.txt`), so even a bare binding there asks for the wrong name; `TrackMouseEvent` is `3B3`.
- **Signature:** 10 decl/def pairs parse, **all `decl == def`, 0 U4 / U11 / U13**. Verify returns and parameter types
  against the SDK (`winuser.h:1356`, `:2773`, `:3646`, `:4276`, `:4282`, `:6105`, `:6964`, `:6970`, `:6976`, `:8155`) with
  the return-on-its-own-line instrument, plus `mouse_event`'s `VOID` over `voidCall` and `DragDetect`'s by-value `::POINT`
  (sound per session 24's `Point`/`POINT` measurement).
- **U14(b):** `TrackMouseEvent`'s `Library::Call< BOOL >(pFunc, lpEventTrack)` (`:83`) restates no parameter types - one of
  the folder's 4 deducing sites (sigcmp `CALL ARITY MISMATCH ... template args=0 call args=1`).
- **U16: 3 of 10 version-guarded** - `GetMouseMovePointsEx` `#if(WINVER >= 0x0500)` (`winuser.h:1329`), `TrackMouseEvent`
  `#if(_WIN32_WINNT >= 0x0400)` (`:2743`, `:2758`), `DragDetect` `#if(WINVER >= 0x0400)` (`:8107`); `GetCapture`,
  `SetCapture` and `ReleaseCapture` sit in the `DESKTOP | PC_APP` partition block (`:6892`). No tree-wide pin exists.
- **U19: 9 LAYER / 7 INDIRECT / 0 BYPASS / 0 MEMBER** over 14 spellings, and **every through-layer site is in built code**
  (`ui/gui/controllers/i_m/mouse.cpp:39,44,49,54,59,64,69,74` + `windowcontroller.cpp:673`) - which makes this TU's binding
  fix behavioural, not cosmetic. Prove it four ways: `/EP` pre-vs-post (only the requested name may change), a live
  `GetProcAddress` probe of the ten bare stems (clone `tmp/_tmp_mon_probe_s24.cpp`), census **11 -> 1**, gate 38/38.
- **U17:** none expected - no mouse stem is an A/W family, so no `T` suffix is owed. Re-prove with
  `tmp/_tmp_tsuffix_census.ps1`; the folder counts 525 / 108 / 404 / 11 / 2 must not move.

