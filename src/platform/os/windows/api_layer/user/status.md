# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 2 Nov 2026 - **session 25 (`mouse.cpp`) CLOSED**. HEAD `3a66dc25` (2 Nov **21:40**, "memory bank reorganised")
carries **sessions 1-24**; **only session 25 is uncommitted** (`git status`: modified `mouse.cpp`, `method.md`, `status.md`,
6 registers (`census`, `prescan`, `tasks`, `u14`, `u16`, `u19`), `archive/README.md`, `archive/session-log.md` and both
`memory-bank/` files; new `archive/tu/mouse.md` + `archive/session-log-20-22.md`); `user32.h` untouched at 695 lines. Review of
`src/platform/os/windows/api_layer/user` (the `User32` wrapper class over `user32.dll`) for correctness, consistency,
safety, undefined behaviour and conformance to the api_layer conventions. Protocol: **one translation unit per session** -
the whole point is not to lose findings between sessions. Small, unambiguous defects get fixed as they are found; anything
that changes library-wide behaviour or needs a decision goes in the register and is *not* touched.

## Where the record lives - start here, then open only what the step needs

| File | Contents | Open it when |
|---|---|---|
| `status.md` (this file; **stay under 180 lines / 18 KB**) | live state, queue, the three convention decisions, register locator, verification gate, current TU pre-flags | every session, first |
| `registers/<file>.md` - **one register entry per file**, index in `registers/README.md` | verbatim register text: P1-P5 `prescan.md`, P6+U27 `p6-u27.md`, G1-G6 `ground-rules.md`, U1-U26 `u<n>.md`, T1-T5 `tasks.md`, totals `census.md` | only the ids named by the locator below or by the current TU's pre-flags |
| `method.md` | the per-TU review method: the numbered steps one session runs | every session, before it starts |
| `tooling.md` | the `tmp/` instrument index - what each scan prints, when it was added, for which TU | before running an instrument or the gate |
| `gotchas.md` | the caveats those instruments carry and the traps in editing these records (indexed from `tooling.md`) | before acting on any instrument flag |
| `archive/tu/<tu>.md` - **one narrative per TU**, index in `archive/README.md` | that TU's narrative + verdict detail | only the TU you just reviewed |
| `archive/session-log.md` | the session-log table (rows 23-25) + closure notes; rows 1-22 in the four `archive/session-log-<range>.md` slices | only while writing the row for the TU you just reviewed |
| `archive/history.md`, `archive/prescan-help.md` | consolidation/split history, consumed `help.cpp` pre-flags | never, unless auditing the record itself |

The rule: **findings, narratives and register text never go in this file.** New finding -> the register file the locator
names; new TU narrative -> `archive/tu/<tu>.md`; new row -> `archive/session-log.md`; new instrument -> `tooling.md`, new
caveat -> `gotchas.md`; folder-wide measures -> append to the **end** of `registers/census.md` (append-only). Hard rule:
**no record file over ~40 KB**, this file under 180 lines / 18 KB. When one outgrows its context, slice by exact line range,
hash-verify the rejoin, re-point the indexes, re-run the locator generator + `_tmp_s23_locsplice.ps1` (2 Nov 2026:
`method.md` -> `tooling.md` + `gotchas.md`). Prefer naming a thing over citing its size or line count.

## Live state (2 Nov 2026)

- **26 of 38 TUs reviewed** (sessions 1-25); the 12 left are the Queue table below. **Session 25 (`mouse.cpp`) CLOSED
  2 Nov 2026** - 10 wrappers, the whole `//Mouse functions` section: **10 P1 binding fixes** (all
  ten `AW` over stems with **no A/W family** - the inverse of the P2 class; bare `qor_pp_useswinapi` is the only spelling
  that can bind them), **P5's second instance** (a binding that named the parameter *type*), and 1 U14(b) deducing site;
  folder unresolvable bindings **11 -> 1**, live wrong-stem sites **1 -> 0**. Proven four ways (`/EP` diff, live
  `GetProcAddress` probe, census, gate). Narrative `archive/tu/mouse.md`, row 25.
- **Session 24 (`monitor.cpp`) CLOSED 2 Nov 2026** - 5 wrappers, **0 fixes** (the first TU with nothing to change); its
  value is measurement: **U16 6/6**, **U19's first bypass in built code**, **U21 sixth instance** + a **U4 hand-off**
  (`GetListBoxInfo`, declared `user32.h:548`, defined nowhere), **U25 fifth spelling**, **U26 fourth instance**. Row 24.
- **Session 23 (`message.cpp`) CLOSED 2 Nov 2026** - 24 wrappers: the whole remaining **P2** list -> explicit `AW`
  (**P2 CLOSED**), U14(c)'s last `()` -> `(void)` (**U14(c) CLOSED**), 3 spacing nits; 0 signature divergences, all 24
  live, 0 bypass in built code, P6 second instance, U21 fifth, U16 4/24, U19 172/12/68/2. Row 23 + `archive/tu/message.md`.
- **Sessions 16-20 (`help`, `hook`, `icon`, `ime`, `keyboard`) CLOSED 31 Oct - 1 Nov 2026** - no `archive/tu/` file for
  these five, their narrative is the row; `archive/tu/` covers sessions 1-15 and 21-25.
- **U6 CLOSED 1 Nov 2026** (off-protocol, not a TU session) - `Call`/`voidCall` now call through a `__stdcall` `fPtr`
  (`library.h:46`, `:68`) matching the `WINAPI` exports behind all **1,638** bindings; commit `2bc8e513`; U7/G5 open.

## Queue - 12 TUs left, alphabetical (lines = on-disk physical lines, measured 2 Nov 2026)

| # | TU | lines | # | TU | lines |
|--:|---|--:|--:|---|--:|
| 26 | `painting.cpp` | 191 | 32 | `systemshutdown.cpp` | 51 |
| 27 | `processthread.cpp` | 57 | 33 | `text.cpp` | 44 |
| 28 | `winqor_api_user.cpp` (glue) | 14 | 34 | `window.cpp` | **615** |
| 29 | `rectangle.cpp` | 93 | 35 | `windowclass.cpp` | 114 |
| 30 | `resource.cpp` | 30 | 36 | `windowprocedure.cpp` | 30 |
| 31 | `scrollbar.cpp` | 102 | 37 | `windowproperties.cpp` | 51 |

1,392 lines remain, **806 of them in two TUs** (`window` 615, `painting` 191). What is left of the binding census is
`window.cpp:124 CreateWindow` (`tmp/_tmp_p1census_post_s25b.txt`) - **P2 and P5 are closed** and `mouse.cpp`'s 10 sites are
fixed. `window.cpp` (615 lines, 85 bindings, P3 `CreateWindowT`, U18's `void` converters, 3 deducing `Call<>` sites) may not
fit one session - decide how to split it before starting it, not mid-session. Hand-offs the queue must not lose:
`window.cpp` (34) takes the version-guarded MDI members `CascadeWindows`/`TileWindows` (**U16**) plus the 17 DPI / `*Info` /
WinEvent-hook heads that share the monitor SDK region (`tmp/_tmp_mon_region_s24.txt`); **T6** in `tasks.md` carries the
ownerless `//List Box functions` section this scan found.

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
proof, see `gotchas.md`; only the TU-name echoes may appear); `cl /EP` re-run after the last edit still yields the expected
binding literals; `user32.h` untouched unless the row says otherwise. The gate also carries the folder-wide **binding
census** (latest `tmp/_tmp_p1census_post_s25b.txt`: 428 bindings, **1** unresolvable - `window.cpp:124`) and the per-stem
bare-vs-AW classifier (`tmp/_tmp_mouse_class_s25.ps1`). **Session 25's authoritative gate is `tmp/_tmp_cl_all_s25c.log`** -
22:18, 4 min after `mouse.cpp`'s last edit (22:14:54), 38 echoes, 0 diagnostics; `_tmp_cl_all_s25b.log` (22:06) predates
that edit, and session 24's `_s24.log` was likewise a pre-edit run. Library-header edits
must additionally pass the **all-libraries gate** `tmp/_tmp_cl_all_apiall_u6.bat` (106 sources), meaningful only as a diff
against a pre-fix run (pre-existing `C1083` at `opengl/wgl.cpp:31` plus 165 warnings on HEAD).

## Pre-flags for the current TU (rolling - keep this under ~40 lines)

**Consumed:** session 25's pre-flags are superseded by its row + `archive/tu/mouse.md`. Pre-flags are leads, never facts -
session 24's U21 hand-off ("`window.cpp` (34)") was a guess; measured, it is 6 / 17 / 22.

**Session 26 `painting.cpp`** (191 lines; 25 wrappers = the whole `user32.h:262`-`:287` "//Painting functions" section -
the folder's second-largest section after `window`; decls `:263`-`:287` all `static`, defs `:16`-`:184` in header order,
step 7). Measured 2 Nov 2026 by `tmp/_tmp_paint_prep_s26.ps1` into `_tmp_paint_pre_s26.txt` (sigcmp `-First 262 -Last 287`),
`_tmp_paint_namediv_s26.txt`, `_tmp_paint_guards_s26.txt`, `_tmp_paint_bypass_s26.txt`, `_tmp_paint_bindings_s26.txt`:
- **Binding: 0 P1 expected.** 23 bare + 2 `AW` (`DrawState` `:61`, `GrayString` `:117`), and both are real families
  (`DrawStateA/W` `D9`/`DA`, `GrayStringA/W` `1FD`/`1FE`, bare forms **not** exported - `tmp/_tmp_paint_exports_s26.txt`),
  so D13 and the `T` suffix on those two members are correct, not U17 deviations. Census puts 0 unresolvable here.
- **Signature: 25/25 `decl == def`, 0 U4/U11/U13, 0 parameter-name divergences.** sigcmp reports **8 "CALL TYPE MISMATCH"**
  rows, all spelling not type: `CONST ::RECT*`/`CONST ::PAINTSTRUCT*` in the `Call<>` list vs `CONST::RECT*` in the
  declaration (`:27` both args, `:48`, `:69`, `:125`, `:153`, `:174`) and unqualified `DRAWSTATEPROC`/`GRAYSTRINGPROC` vs
  the declared `::DRAWSTATEPROC`/`::GRAYSTRINGPROC` (`:62`, `:118`) - **U25's sixth + seventh spelling**; `:174` also has a
  stray space before `)`. Check the returns against the 27 SDK protos (`winuser.h:2984`,`3047`,`3077`,`3096`,`8348`,`8361`,
  `8406`,`8420`,`8502`,`8522`,`8563`,`8606`,`8619`,`8626`,`8633`,`8641`,`8649`,`8664`,`8673`,`8682`,`8689`,`8697`,`8704`,
  `8712`,`8720`,`8760`,`9722`).
- **U16: 7 of 25** guarded - `DrawEdge`/`DrawFrameControl`/`DrawCaption`/`DrawAnimatedRects` under one
  `#if(WINVER >= 0x0400)` (`:2923`), `DrawStateA/W` under `:8380`, `PaintDesktop` under `:8518`, `GetWindowRgnBox` under
  `#if(_WIN32_WINNT >= 0x0501)` (`:8668`) - the item's first `0x0501` guard. All 25 are Desktop-partition, none `PC_APP`.
- **U14:** 25 explicit / 0 deducing, **0 spaced / 25 tight** - internally uniform, so leave the spacing (pure churn).
- **U19: 17 LAYER / 11 INDIRECT / 3 BYPASS / 0 MEMBER.** Two BYPASS rows are SDK-thunk calls in **built** `winqor_gui`
  code - `paintsession.cpp:41` `BeginPaint`, `:47` `EndPaint` - the second built-code bypass after monitor.cpp's; the third
  (`windowcontroller.h:235`) is a member declaration, so treat it as a probable false positive.
- **U26 question:** neither `T` member has a `TCHAR` parameter (`user32.h:269`, `:277`) - settle what `A`/`W` actually
  changes for `DrawState`/`GrayString` (the opaque `lpData`/`nCount`?) before treating the suffix as meaningful.

