# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 3 Nov 2026 - **session 30 (`scrollbar.cpp`) CLOSED; session 31 `systemshutdown.cpp` is the current TU**
(pre-flags at the end of this file). HEAD carries **sessions 1-29** (`88969d2e` = 27, `ed4eaf8c` = 28, `d18d98ac` = 29);
session 30 is committed at its own close; `user32.h` untouched at 695 lines throughout. Review of `src/platform/os/windows/api_layer/user` (the `User32` wrapper class over `user32.dll`) for
correctness, consistency, safety, UB and conformance to the api_layer conventions. Protocol: **one translation unit per
session** - the whole point is not to lose findings between sessions. Small, unambiguous defects get fixed as they are
found; anything that changes library-wide behaviour or needs a decision goes in the register and is *not* touched.

## Where the record lives - start here, then open only what the step needs

| File | Contents | Open it when |
|---|---|---|
| `status.md` (this file; **stay under 220 lines / 22 KB**) | live state, queue, the three convention decisions, register locator, verification gate, current TU pre-flags | every session, first |
| `registers/<file>.md` - **one register entry per file**, index in `registers/README.md` | verbatim register text: P1-P5 `prescan.md`, P6+U27 `p6-u27.md`, G1-G6 `ground-rules.md`, U1-U26 `u<n>.md`, T1-T5 `tasks.md`, totals `census.md` | only the ids named by the locator below or by the current TU's pre-flags |
| `method.md` | the per-TU review method: the numbered steps one session runs | every session, before it starts |
| `tooling.md` | the `tmp/` instrument index - what each scan prints, when it was added, for which TU | before running an instrument or the gate |
| `gotchas.md` | the caveats those instruments carry and the traps in editing these records (indexed from `tooling.md`) | before acting on any instrument flag |
| `archive/tu/<tu>.md` - **one narrative per TU**, index in `archive/README.md` | that TU's narrative + verdict detail | only the TU you just reviewed |
| `archive/session-log.md` | the session-log table (rows 29-30) + per-session closure notes; older rows in the six `archive/session-log-<range>.md` slices | only while writing the row for the TU you just reviewed |
| `archive/history.md`, `archive/prescan-help.md` | consolidation/split history, consumed `help.cpp` pre-flags | never, unless auditing the record itself |

The rule: **findings, narratives and register text never go in this file.** New finding -> the register file the locator
names; new TU narrative -> `archive/tu/<tu>.md`; new row -> `archive/session-log.md`; new instrument -> `tooling.md`, new
caveat -> `gotchas.md`; folder-wide measures -> append to the **end** of `registers/census.md` (append-only). Hard rule:
**no record file over ~40 KB**, this file under 220 lines / 22 KB - when it is over, compress the **Live state** bullets and
the consumed pre-flags (that detail belongs in `archive/tu/` and the session-log row), never the locator.
Slice by exact line range, hash-verify the rejoin, re-point the indexes, then re-run the locator generator + the splice.

## Live state (3 Nov 2026)
- **31 of 38 TUs reviewed** (sessions 1-30); the 7 left are the Queue table below. **Session 30 (`scrollbar.cpp`) CLOSED
  3 Nov 2026** - 12 wrappers, **0 defects**: 12/12 bare single exports so P1/P2/P6 cannot arise, header/definition/SDK
  parameter names identical 12/12 on a case-sensitive compare, every `Call<>` tight, 0 U16 surprises, and U19 **8 LAYER /
  8 "INDIRECT" / 0 BYPASS / 0 MEMBER** - all 8 INDIRECT hits are the caller's own same-named member *definitions*
  (session 28's false-positive class), and nothing in-tree constructs `ScrollBar`, so the façade chain is latent. One
  cosmetic fix with the user's decision: the TU's 12 `qor_pp_useswinapi(User32, ...)` module tokens normalized to
  `user32` - it was the folder's **only** remaining `User32` spelling site (12 of 428 bindings); `/EP` pre vs post differ
  on **exactly 12 lines**, each only that literal, 0 case-insensitive difference (`tmp/_tmp_s30_neutral_final.ps1`). G4's
  population re-measured (it cited `combobox.cpp`, which no longer spells it). The caller-side mirror `ScrollInfo`
  (`ui/gui/controllers/s_s/scrollbarcontroller.h:33`) is **layout-identical to `SCROLLINFO` on x64 and x86** (28 bytes, 7
  matching offsets), so the `reinterpret_cast` at `scrollbarcontroller.cpp:57`/`:72` is sound - U26's fifth clean mirror,
  with one caveat: `nPos`/`nTrackPos` alone have no default initializer, so a default-constructed struct carries an
  indeterminate `int` into both `GetScrollInfo` and `SetScrollInfo`.
- **Session 29 (`resource.cpp`)** - the folder's only **U11** pair, a real defect (both definitions were free functions, so
  the `static` members at `user32.h:443`-`:444` had no definition anywhere): fixed by qualifying both with `User32::`,
  proved three ways. **U11 CLOSED**; `static` with no definition now **0 folder-wide**; call-paren census **421 tight /
  0 spaced**; every reviewed TU carries a contract block.
- **Session 28 (`rectangle.cpp`)** - 0 defects; the session in which **D15 closed U14** (frozen at 163 spaced / 265 tight).
  **Sessions 23-27** - `message.cpp` (**P2 CLOSED**), `monitor.cpp` (first all-clean TU), `mouse.cpp` (**10 P1 binding
  fixes**, folder unresolvable 11 -> 1), `painting.cpp` (2 nits), `processthread.cpp` (folder's last TAB; U16's second
  0x0501 gate + the **empty** `#if`/`#endif` at `winuser.h:4121`; **T7**). `archive/tu/` narratives cover sessions 1-15 and
  21-30; sessions 16-20 have their narrative in the row only.

## Queue - 7 TUs left, alphabetical (lines = on-disk physical lines, measured 3 Nov 2026)

| # | TU | lines | # | TU | lines |
|--:|---|--:|--:|---|--:|
| 31 | `systemshutdown.cpp` | 51 | 35 | `windowproperties.cpp` | 51 |
| 32 | `text.cpp` | 44 | 36 | `winqor_api_user.cpp` (glue) | 14 |
| 33 | `windowclass.cpp` | 114 | 37 | `window.cpp` | **615** |
| 34 | `windowprocedure.cpp` | 30 | | | |

919 lines remain, **615 of them in one TU** (`window.cpp`, last in the queue). What is left of the binding census is
`window.cpp:124 CreateWindow` (`tmp/_tmp_p1census_post_s28.txt`: 428 bindings, **1** unresolvable); **P2, P5 and U14 are
closed** - U14's frozen number is **163 spaced / 265 tight** and the spaced sites must not be re-touched (D15).
`window.cpp` (85 bindings, P3 `CreateWindowT`, U18's `void` converters, U14(b)'s last 3 deducing sites at `:118`, `:254`,
`:317`) may not fit one session - decide the split before starting it, never mid-session. Hand-offs it must not lose: the
version-guarded MDI members `CascadeWindows`/`TileWindows` (**U16**), the 17 DPI / `*Info` / WinEvent-hook heads
(`tmp/_tmp_mon_region_s24.txt`), **T6**'s ownerless `//List Box functions` section and **T7**'s 35 dead `returncheck.h`
includes.

## Convention decision
- **D13 (adopted 29 Oct 2026, user decision; `docs/agent/DECISIONS.md`).** Explicit `qor_pp_useswinapiAW(<mod>, Stem)`
  wherever the SDK has an A/W family; bare `qor_pp_useswinapi` only for a single export with no A/W variants. Never rely on
  the SDK's `Stem -> StemW` macro surviving into the binding expression (G1); apply as each TU is reviewed.
- **Return-check / error-handler policy (settled 29 Oct 2026, user decision, session 3) - leave as-is: no
  `Win32ErrorHandler _;`, no `CheckReturn`/`CheckNonZeroHandle` here**, though `api_layer/shell` and `api_layer/kernel` use
  both and G6 makes `CheckReturn` always active. Sessions *record* the sites (U1/U2), change nothing - a `FALSE`/NULL/0
  failure is silent and a swallowed SEH fault is indistinguishable from a legitimate zero (U7).
- **D14 (adopted 2 Nov 2026, user decision; `docs/agent/DECISIONS.md`) - U15 CLOSED.** An exported wrapper with no
  reachable in-tree caller is published surface, **not a finding**: record none, `registers/u15.md` is frozen. Measure
  reachability only where another item needs it (P1/P5 latent-vs-live, U18's `TrackPopupMenu`, U19's LAYER-vs-BYPASS split).
- **D15 (adopted 3 Nov 2026, user decision; `docs/agent/DECISIONS.md`) - U14 CLOSED.** `rectangle.cpp` was the last reviewed
  TU with spaced `Library::Call< T, ... >` sites, so the item ran out of TUs to accumulate on; its tally is frozen at
  **163 spaced / 265 tight, 425 explicit / 3 deducing** and nothing is appended to `registers/u14.md` any more. Sessions stop
  reporting a spaced/tight tally; settling the residue would be a folder-wide mechanical pass, which would be a new
  `registers/tasks.md` entry, not a reopened U14. **Not** closed by it: **U25** (the `const` spellings) and U14(b)'s three
  deducing sites, which travel to `window.cpp`'s session as a hand-off.

## Register locator - id -> which `registers/` file and line holds its text
A locator, not a finding: open the file at the line given before you act on, extend or close an id - never open the whole
`registers/` tree. Generated by `tmp/_tmp_status_index_gen_s22.ps1` (descriptions capped at 60 chars); never hand-edit it.

- **P1** `registers/prescan.md:113` re-measured after session 25 (`mouse.cpp`, 2 Nov 2026): 1 un...
- **P1** `registers/prescan.md:10` bindings that cannot resolve at run time (export table + liv...
- **P2** `registers/prescan.md:60` bindings that work today only because the SDK macro rewrites...
- **P3** `registers/prescan.md:74` naming flags from the new T-suffix census (`tmp/_tmp_tsuffix...
- **P4** `registers/prescan.md:80` four exports in the table are forwarders, which the binding...
- **P5** `registers/prescan.md:123` now has 0 live instances. The scan's `mouse.cpp:82` row (`Tr...
- **P5** `registers/prescan.md:98` re-measured 3 Nov 2026 (session 29, `resource.cpp`): 428 bin...
- **P5** `registers/prescan.md:88` the "wrong stem bound" defect class, and the instrument that...
- **P6** `registers/p6-u27.md:9` (promoted extract) Bare `CallMsgFilter` resolves and runs the ANSI entry point on wide arguments
- **G1** `registers/ground-rules.md:5` the SDK A/W macro rewrites the binding name before it is str...
- **G2** `registers/ground-rules.md:12` the nine stems `user32.h:9-17` `#undef`s are the real missin...
- **G3** `registers/ground-rules.md:24` the previous audit report is partly wrong. `tmp/_tmp_audit_r...
- **G4** `registers/ground-rules.md:30` module token case is cosmetic. `_TXT(User32)` stringizes to...
- **G5** `registers/ground-rules.md:33` the "already loaded image" assumption is not universal. In a...
- **G6** `registers/ground-rules.md:36` return checking is silently always on. `returncheck.h:19` te...
- **U1** `registers/u1.md:3` no call-site error handler anywhere in this library. Policy...
- **U2** `registers/u2.md:8` measured 3 Nov 2026 (session 27, `processthread.cpp`) - and...
- **U2** `registers/u2.md:3` return-check policy is absent. Policy settled 29 Oct 2026 (u...
- **U2** `registers/u2.md:19` measured 3 Nov 2026 (session 28, `rectangle.cpp`) - the answ...
- **U3** `registers/u3.md:3` `hook.cpp` build-list status: RESOLVED (user commit `05d3ca2...
- **U4** `registers/u4.md:3` `user32.h` declares about 101 non-static members that no `.c...
- **U5** `registers/u5.md:3` the binding is cached for the process lifetime including fai...
- **U6** `registers/u6.md:3` calling-convention mismatch in the call path: RESOLVED 1 Nov...
- **U7** `registers/u7.md:3` SEH faults are swallowed into a default return. `Call` catch...
- **U8** `registers/u8.md:3` `throw("Missing library function.")` (`library.h:60`, `:92`)...
- **U9** `registers/u9.md:3` no `purpose.md` in this folder and no scope/audit note. `api...
- **U10** `registers/u10.md:3` `user32.h:19-62` defines `CONSOLECONTROL`, `CONSOLEENDTASK`,...
- **U11** `registers/u11.md:3` `resource.cpp` defines both Resource members without the `Us...
- **U11** `registers/u11.md:14` CLOSED in `resource.cpp`'s own session (session 29, 3 Nov 20...
- **U12** `registers/u12.md:3` `accessibility.cpp` and `timer.cpp` held each other's wrappe...
- **U13** `registers/u13.md:3` one member was defined through `User32::` but declared non-s...
- **U14** `registers/u14.md:87` CLOSED 3 Nov 2026 (session 28, `rectangle.cpp`) by user deci...
- **U14** `registers/u14.md:66` (b) re-measured 2 Nov 2026 (session 25, `mouse.cpp`): 425 ex...
- **U14** `registers/u14.md:74` (a) moved deliberately 3 Nov 2026 (session 27, `processthrea...
- **U14** `registers/u14.md:31` re-measure 1 Nov 2026 (session 20, `keyboard.cpp`). (b) `tmp...
- **U14** `registers/u14.md:55` (c) CLOSED 2 Nov 2026 (session 23, `message.cpp`). The last...
- **U14** `registers/u14.md:3` `Library::Call`/`voidCall` template-argument style is split...
- **U15** `registers/u15.md:3` wrappers that are exported through `QOR_WINAPIUSER` but have...
- **U16** `registers/u16.md:230` measured 3 Nov 2026 (session 27, `processthread.cpp`): 5 pro...
- **U16** `registers/u16.md:243` (`rectangle.cpp`, 3 Nov 2026, session 28): 0 of 11 - the pla...
- **U16** `registers/u16.md:203` (`mouse.cpp`, 2 Nov 2026, session 25): 3 of the 10 prototype...
- **U16** `registers/u16.md:3` the wrappers pair with SDK declarations that the SDK only em...
- **U16** `registers/u16.md:220` (`painting.cpp`, 3 Nov 2026, session 26): 8 of this TU's 27...
- **U17** `registers/u17.md:3` A/W wrappers declared without the folder's `T` suffix. Opene...
- **U18** `registers/u18.md:3` a wrapper pair whose return type contradicts the SDK: `void`...
- **U19** `registers/u19.md:3` call sites that reach a `user32.dll` entry point through the...
- **U19** `registers/u19.md:198` (`mouse.cpp`, 2 Nov 2026, session 25): 9 LAYER / 7 INDIRECT...
- **U19** `registers/u19.md:210` (`painting.cpp`, 3 Nov 2026, session 26): 17 LAYER / 11 INDI...
- **U19** `registers/u19.md:222` measured 3 Nov 2026 (session 27, `processthread.cpp`): 1 LAY...
- **U19** `registers/u19.md:231` (`rectangle.cpp`, 3 Nov 2026, session 28): 11 LAYER / 2 "IND...
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
- **T7** `registers/tasks.md:121` 35 of the 38 `user` sources `#include "../returncheck.h"` an...

## Verification gate (identical every session)
Edited TU compiles standalone at `/W4` exit 0; the whole library compiles **38/38 at `/W4`, exit 0, no diagnostics**
(`tmp/_tmp_cl_all.bat` -> the shared `tmp/_tmp_cl_all.log`, snapshotted under a session name - never copy an old log as
proof, see `gotchas.md`; only the TU-name echoes may appear); `cl /EP` re-run after the last edit still yields the expected
binding literals; `user32.h` untouched unless the row says otherwise. The gate also carries the folder-wide **binding
census** (latest folder pass `tmp/_tmp_wrongstem_s30.txt`: 38 files, 428 `User32::` definitions, 428 bindings scanned,
**1** mismatched member/stem pair - the long-known `cursor.cpp:64 CopyCursorT` binds `CopyIcon`; binding census
`tmp/_tmp_p1census_post_s28.txt`: 428 bindings, **1** unresolvable - `window.cpp:124`) and the per-stem
bare-vs-AW classifier (`tmp/_tmp_scroll_class_s30.ps1`). **Session 30's authoritative gate is `tmp/_tmp_cl_all_s30b.log`** -
run 3 Nov after `scrollbar.cpp`'s last edit (19:23:55): 38 echoes, 38 `.obj` files newer than the source (earliest
19:34:09), single-TU `cl_exit=0`, and the `/EP` neutrality diff `tmp/_tmp_s30_neutral_final.ps1` giving **12 differing
lines, each only the module literal**, 0 case-insensitive difference. **A clean 38-TU log cannot be dated by its hash** -
session 28's, 29's and 30's are byte-identical - so date one by `tmp/obj/<tu>.obj` being newer than the source (session 24's
`_s24.log` and 25's `_s25b.log` were pre-edit runs and must not be cited). Library-header edits must additionally pass the
**all-libraries gate** `tmp/_tmp_cl_all_apiall_u6.bat` (106 sources), meaningful only as a diff against a pre-fix run
(pre-existing `C1083` at `opengl/wgl.cpp:31` plus 165 warnings on HEAD).

## Pre-flags for the current TU (rolling - keep this under ~40 lines)

**Consumed:** session 30's pre-flags are superseded by its row + `archive/tu/scrollbar.md` - all held (12/12 bare single
exports so P1/P2/P6 could not arise; U19's 8 INDIRECT rows were all the same-named-member-definition false-positive class,
**0** real INDIRECT/BYPASS; the `const`/`LPINT` U25 lead; the T7 dead include). Two things the pre-scan did **not** predict and the
session found anyway: the `User32` module-token spelling (12 of the folder's 428 bindings, the folder's last such site) and
the caller-side `ScrollInfo` mirror. Pre-flags are leads, never facts.

**Session 31 `systemshutdown.cpp`** (51 lines, 1,734 B; 5 wrappers declared `user32.h:602`-`:606` under the `:601` label,
which the header itself misspells **`//System Shutdwown functions`** - a `System Shutdown` grep finds nothing, and the typo
is a U-something candidate for the header, not for this TU). Measured 3 Nov with `tmp/_tmp_s31_preflags.ps1` ->
`tmp/_tmp_s31_preflags.txt` (one pass, every section under 40 lines).
- **Bindings: 5/5 bare single exports**, each stem `exact=1 A/W=0` in `tmp/_tmp_user32_exports.txt`, module tokens already
  `user32` - D13's spelling, **P1/P2/P6 cannot arise**, no `T` suffix expected or present.
- **The folder-wide singleton found in the pre-scan:** `:37` and `:44` are the **only 2 `WINAPI`-qualified `User32::`
  definition heads in all 38 TUs** (`BOOL WINAPI User32::ShutdownBlockReasonDestroy` / `...Query`); `:16`, `:23`, `:30`
  spell the same shape bare, and `user32.h:602`-`:606` carries no `WINAPI` on any of the five (the header's only `WINAPI`
  substring is inside the `QOR_WINAPIUSER` export macro at `:66`). A calling-convention token on a `static` member
  definition whose pointer is later cast to `Library::DefProc` is at best redundant - check it against the SDK's own
  spelling and against `qor_pp_module_interface` before deciding whether it is U14-family style or a real convention risk.
- **U16 lead:** the `ShutdownBlockReason*` prototypes are `winuser.h:15924`/`:15931`/`:15939` - the block above this
  library has reviewed, so expect the **highest version floor yet** (likely `0x0601`); `ExitWindowsEx` is at `:3639`, next
  to `IsWow64Message`'s `0x0501` region, and `LockWorkStation` at `:14916`. Depth-counted walk only
  (clone `tmp/_tmp_scroll_guards_s30.ps1`); never a backward scan (the empty `#if`/`#endif` at `:4121`).
- **U19: 0 layer callers** - every one of the 15 grep hits for the five stems is this TU or `user32.h`
  (`tmp/_tmp_s31_preflags.txt` section 6). Under **D14** that is published surface: expect **no U19 row** unless a hit
  appears in a compiled target.
- **Return policy (U1/U2, record only):** 5/5 return `BOOL`. `ExitWindowsEx`/`LockWorkStation` are success-or-failure; the
  `ShutdownBlockReason*` trio's `FALSE` distinguishes documented outcomes (`ERROR_NOT_FOUND`, `ERROR_SHUTDOWN_IN_PROGRESS`),
  so session 28's answer-shaped-`BOOL` caution applies to all three.
- **T7:** `:11` includes `../returncheck.h` and uses none of it (0 `CheckReturn`/`BoolCheck`/`Win32ErrorHandler`) - a dead
  include, **do not remove**.
- **Hygiene:** 0 TAB, trailing space, non-ASCII; longest line **124** at `:9` (the include-order comment); **3 comment
  lines**, no contract block - add one. Only 5 wrappers and 51 lines, so this is a fast TU: spend the slack on the
  `WINAPI` question and the U16 floor.
