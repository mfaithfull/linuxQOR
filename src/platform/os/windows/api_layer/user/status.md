# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 3 Nov 2026 - **session 26 (`painting.cpp`) CLOSED**. HEAD `17f39829` (3 Nov, "Refactored method.md")
carries **sessions 1-25**; **only session 26 is uncommitted** (`git status`: modified `painting.cpp`, `status.md`,
`tooling.md`, `gotchas.md`, 4 registers (`census`, `u16`, `u19`, `u26`), `archive/README.md`, `archive/session-log.md` and
`memory-bank/activeContext.md` = 11; new `archive/tu/painting.md`); `user32.h` untouched at 695 lines. Review of
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
| `archive/session-log.md` | the session-log table (rows 23-26) + closure notes; rows 1-22 in the four `archive/session-log-<range>.md` slices | only while writing the row for the TU you just reviewed |
| `archive/history.md`, `archive/prescan-help.md` | consolidation/split history, consumed `help.cpp` pre-flags | never, unless auditing the record itself |

The rule: **findings, narratives and register text never go in this file.** New finding -> the register file the locator
names; new TU narrative -> `archive/tu/<tu>.md`; new row -> `archive/session-log.md`; new instrument -> `tooling.md`, new
caveat -> `gotchas.md`; folder-wide measures -> append to the **end** of `registers/census.md` (append-only). Hard rule:
**no record file over ~40 KB**, this file under 180 lines / 18 KB. When one outgrows its context, slice by exact line range,
hash-verify the rejoin, re-point the indexes, re-run the locator generator + `_tmp_s23_locsplice.ps1` (2 Nov 2026:
`method.md` -> `tooling.md` + `gotchas.md`). Prefer naming a thing over citing its size or line count.

## Live state (3 Nov 2026)

- **27 of 38 TUs reviewed** (sessions 1-26); the 11 left are the Queue table below. **Session 26 (`painting.cpp`) CLOSED
  3 Nov 2026** - 25 wrappers, the whole `//Painting functions` section: **0 binding fixes** (all 25 `/EP` literals resolve;
  `AW` is already the correct D13 spelling on exactly the two A/W families and bare on the other 23 - first all-clean
  binding TU since `monitor.cpp`), 0 signature/return divergences, only **2 whitespace nits** fixed; new register
  material: **U16's first 0x0501 gate** (`GetWindowRgnBox`, 7 guarded members), **U19's 2 new real BYPASS sites**
  (`paintsession.cpp:41`/`:47`, layer-vs-thunk side-by-side), **U26's width-invisible pair** (`DrawState`/`GrayString`
  A/W draw differently with identical opaque arguments, measured x64+x86) and a `-replace 'T$'` case-insensitivity
  instrument bug found and fixed. Row 26 in `registers/census.md`.
- **Session 25 (`mouse.cpp`) CLOSED 2 Nov 2026** - 10 wrappers, the whole `//Mouse functions` section: **10 P1 binding fixes** (all
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

## Queue - 11 TUs left, alphabetical (lines = on-disk physical lines, measured 2 Nov 2026)

| # | TU | lines | # | TU | lines |
|--:|---|--:|--:|---|--:|
| 27 | `processthread.cpp` | 57 | 33 | `window.cpp` | **615** |
| 28 | `rectangle.cpp` | 93 | 34 | `windowclass.cpp` | 114 |
| 29 | `resource.cpp` | 30 | 35 | `windowprocedure.cpp` | 30 |
| 30 | `scrollbar.cpp` | 102 | 36 | `windowproperties.cpp` | 51 |
| 31 | `systemshutdown.cpp` | 51 | 37 | `winqor_api_user.cpp` (glue) | 14 |
| 32 | `text.cpp` | 44 |  |  |  |

1,201 lines remain, **615 of them in one TU** (`window.cpp`). What is left of the binding census is
`window.cpp:124 CreateWindow` (`tmp/_tmp_p1census_post_s25b.txt`) - **P2 and P5 are closed** and `mouse.cpp`'s 10 sites are
fixed. `window.cpp` (615 lines, 85 bindings, P3 `CreateWindowT`, U18's `void` converters, 3 deducing `Call<>` sites) may not
fit one session - decide how to split it before starting it, not mid-session. Hand-offs the queue must not lose:
`window.cpp` (33) takes the version-guarded MDI members `CascadeWindows`/`TileWindows` (**U16**) plus the 17 DPI / `*Info` /
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
census** (latest `tmp/_tmp_p1census_post_s26.txt`: 428 bindings, **1** unresolvable - `window.cpp:124`) and the per-stem
bare-vs-AW classifier (`tmp/_tmp_paint_class_s26.ps1`). **Session 26's authoritative gate is `tmp/_tmp_cl_all_s26.log`** -
run 3 Nov after `painting.cpp`'s last edit (01:03:33), 38 echoes, 0 diagnostics. The older lesson stands: session 24's
`_s24.log` and 25's `_s25b.log` were pre-edit runs and must not be cited. Library-header edits
must additionally pass the **all-libraries gate** `tmp/_tmp_cl_all_apiall_u6.bat` (106 sources), meaningful only as a diff
against a pre-fix run (pre-existing `C1083` at `opengl/wgl.cpp:31` plus 165 warnings on HEAD).

## Pre-flags for the current TU (rolling - keep this under ~40 lines)

**Consumed:** session 26's pre-flags are superseded by its row + `archive/tu/painting.md`. Pre-flags are leads, never
facts - session 24's U21 hand-off ("`window.cpp` (34)") was a guess; measured, it is 6 / 17 / 22.

**Session 27 `processthread.cpp`** (57 lines; 5 wrappers = the whole `user32.h:255`-`:260` "//Process and Thread
functions" section; decls all `static`, defs `:16`-`:55` in header order). Measured 3 Nov 2026 inline (no script yet):
- **Binding: 0 P1 expected.** All 5 bare `qor_pp_useswinapi`, all 5 stems single exports (`AttachThreadInput F`,
  `GetGuiResources 15F`, `IsWow64Message 24F`, `UserHandleGrantAccess 3D8`, `WaitForInputIdle 3E9`), no A/W rows for
  any stem in the dump, no `T` members and no families, so D13/U17 are satisfied as found.
- **Hygiene leads:** `:29` is the only TAB line in the folder's queue (`\t\tCheckReturn`); `:21`, `:46`, `:54` have a
  space before the closing `)` of the argument list; `Call<>` spacing is mixed intra-TU (4 spaced `Call< BOOL` /
  1 tight `Call<DWORD`).
- **U2 live evidence:** 3 `CheckReturn<>` sites (`:20`, `:29`, `:45`) - this is one of the only two TUs that actually
  use `../returncheck.h`. The settled policy says wrappers add none; the question here is whether the existing ones
  stay (session 2's precedent in `errorhandling.cpp` says recorded, not removed).
- **U16 leads (innermost non-partition guard):** `GetGuiResources` `winuser.h:12417` under `#if(WINVER >= 0x0500)`,
  `UserHandleGrantAccess` `:14925` under `#if(_WIN32_WINNT >= 0x0500)`, `IsWow64Message` `:3679` under
  `#if(_WIN32_WINNT >= 0x0501)` (the item's second 0x0501 case), `AttachThreadInput` `:4103` + `WaitForInputIdle`
  `:4128` NOUSER-only.
- **U14:** 5 explicit / 0 deducing; `IsWow64Message(void)` already matches `static BOOL IsWow64Message(void)` -
  U14(c) clean. Verify returns (BOOL/DWORD/BOOL/BOOL/DWORD) against the 5 protos and run the standard scans
  (census, wrong-stem, U19 over the 5 bare stems).

