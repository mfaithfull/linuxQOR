# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 4 Nov 2026 - **session 32 (`text.cpp`) CLOSED; session 33 `window.cpp` is the current TU** (its
pre-flags are the last block of this file). Review of
`src/platform/os/windows/api_layer/user` (the `User32` wrapper class over `user32.dll`) for
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

## Live state (4 Nov 2026)
- **33 of 38 TUs reviewed**; the 5 left are the Queue table below. **Session 32 (`text.cpp`) CLOSED 4 Nov 2026** - 4
  wrappers (the whole `user32.h:567` *Font and Text functions* section), **0 defects, 0 code edits** (comment block only;
  neutrality proved by `/EP`: 154,296 non-blank lines before and after, +80 comment lines, 0 non-blank differences).
  It is the folder's **only TU where all four bound stems have their SDK A/W macro live at the binding site**
  (`winuser.h:8269`/`:8331`/`:8470`/`:8494`): `qor_pp_useswinapiAW` pastes `W` onto a **bare** name before that name can
  expand, so `/EP` shows `"DrawTextW"`, `"DrawTextExW"`, `"GetTabbedTextExtentW"`, `"TabbedTextOutW"` - 0 bare literals,
  0 of the non-existent `…WW` exports, i.e. P1/P2/P6 **correct by construction** (the G1 case). 4/4 `decl==def`
  (0 U4/U11/U13/U17), **0 U18 traffic**, and **U23 gains a second class**: `CONST LPINT` on a pointer parameter is
  `INT* const` in a member-function parameter list, not the SDK's `CONST INT *` (probe-proved - the whole tab-stop array
  is writable through `GetTabbedTextExtentT`/`TabbedTextOutT`), plus the `DT_MODIFYSTRING` `_Inout_` buffer contract the
  `LPCTSTR` parameter hides. U16: 4/4 DESKTOP, 1/4 version-gated (`DrawTextExT` inside `#if(WINVER >= 0x0400)`), new
  guard name `NODRAWTEXT`. U25 0, U19 0 instances (the `DT_*` family is comment-only in-tree), U15 0 (D14), U26 0 (no
  mirrored struct); all four stems have live out-of-folder consumers -> 0 wrong-stem rows, 0 dead exports.
- **Session 31 (`systemshutdown.cpp`)
  CLOSED 4 Nov 2026** - 5 wrappers, **1 defect class fixed**: `:37`/`:44` were the folder's only two `WINAPI`-qualified
  `User32::` definition heads, disagreeing with the unmarked declarations at `user32.h:605`/`:606`, which makes the TU
  **ill-formed for x86** (`C2373` x2) and a **no-op for x64** (same `__cdecl` decoration on both sides of a caller probe,
  `dumpbin /disasm` byte-identical, `/EP` non-blank lines 154,302 both sides). 5/5 bare single exports (D13-correct), 5/5
  `decl==def` including parameter names, **0 U18 traffic**, U16 5/5 DESKTOP with **1** version gate
  (`LockWorkStation` `_WIN32_WINNT >= 0x0500`; the `ShutdownBlockReason` trio has **no** floor - refutes the 0x0601 lead),
  **0 callers of any class** for all 5 stems (D14 - no U19/U15 row), T7 36 -> 37. Folder-wide measure opened in
  `registers/u6.md`: **x86 is unbuildable - 93 of 106 api_layer TUs fail on `kernel32.h:92` `PUNWIND_HISTORY_TABLE`**, the
  13 survivors being exactly the `winqor_api_*.cpp` glue TUs.
- **Session 30 (`scrollbar.cpp`)** - 12 wrappers, **0 defects**, one cosmetic fix by user decision (the folder's last 12
  `User32` module tokens normalized to `user32`, neutral by a 12-line `/EP` diff); U19 8 LAYER / 8 false-positive INDIRECT;
  U26's fifth clean caller-side mirror (`ScrollInfo` == `SCROLLINFO` layout on both arches).
- **Session 29 (`resource.cpp`)** - the folder's only **U11** pair, a real defect (both definitions were free functions, so
  the `static` members at `user32.h:443`-`:444` had no definition anywhere): fixed by qualifying both with `User32::`,
  proved three ways. **U11 CLOSED**; `static` with no definition now **0 folder-wide**; call-paren census **421 tight /
  0 spaced**; every reviewed TU carries a contract block.
- **Session 28 (`rectangle.cpp`)** - 0 defects; the session in which **D15 closed U14** (frozen at 163 spaced / 265 tight).
  **Sessions 23-27** - `message.cpp` (**P2 CLOSED**), `monitor.cpp` (first all-clean TU), `mouse.cpp` (**10 P1 binding
  fixes**, folder unresolvable 11 -> 1), `painting.cpp` (2 nits), `processthread.cpp` (folder's last TAB; U16's second
  0x0501 gate + the **empty** `#if`/`#endif` at `winuser.h:4121`; **T7**). `archive/tu/` narratives cover sessions 1-15 and
  21-31; sessions 16-20 have their narrative in the row only.

## Queue - 5 TUs left, alphabetical (lines = on-disk physical lines, re-measured 4 Nov 2026)
Repaired 4 Nov 2026 by `tmp/_tmp_s32_queue_audit.ps1` (for each of the 38 `.cpp` files, count the session-log rows that
name it): exactly five TUs have **no** session row. The previous table carried a **duplicated `text.cpp` row**.

| # | TU | lines | # | TU | lines |
|--:|---|--:|--:|---|--:|
| 33 | `window.cpp` | **615** | 35 | `windowproperties.cpp` | 51 |
| 34 | `windowclass.cpp` | 114 | 36 | `windowprocedure.cpp` | 30 |
| 37 | `winqor_api_user.cpp` (glue) | 14 | | | |

868 lines remain, **615 of them in one TU** (`window.cpp`, last in the queue). What is left of the binding census is
`window.cpp:124 CreateWindow` (`tmp/_tmp_p1census_post_s28.txt`: 428 bindings, **1** unresolvable); **P2, P5 and U14 are
closed** - U14's frozen number is **163 spaced / 265 tight** and the spaced sites must not be re-touched (D15).
`window.cpp` (85 bindings, P3 `CreateWindowT`, U18's `void` converters, U14(b)'s last 3 deducing sites at `:118`, `:254`,
`:317`) may not fit one session - decide the split before starting it, never mid-session. Hand-offs it must not lose: the
version-guarded MDI members `CascadeWindows`/`TileWindows` (**U16**), the 17 DPI / `*Info` / WinEvent-hook heads
(`tmp/_tmp_mon_region_s24.txt`), **T6**'s ownerless `//List Box functions` section and **T7**'s 37 dead `returncheck.h`
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

## Pre-flags for session 33 (`window.cpp`) - measured 4 Nov 2026

**This is the largest TU of the review and the census count for it is wrong.** `window.cpp` is **615 lines** with
**85 wrappers**, not the 50 in `registers/census.md`: `qor_pp_fcontext` = 85, qualified definition heads
(`^    ... User32::Name(`) = 85, binding macros = 85, `Library::Call<` = **82** - so **3 wrapper bodies have no
`Library::Call<` line**; find them first (multi-line `Call<` site, a body that does not call, or a comment-adjacent
head). The file has only **3 comment lines**, i.e. no closure block yet. Fix the census row in the same session.

**Binding is where the risk is (start here).** 10 `qor_pp_useswinapiAW` + **75 direct `qor_pp_useswinapi`** (unlike every
reviewed TU so far - `text.cpp` was 4/4 AW). `tmp/_tmp_s33_prebind.ps1` (re-run it; its saved `.txt` is the pre-fix
all-`NEITHER` run, see `gotchas.md`) classifies against the export table:
- `direct | bare-export` **75/75** - every directly-bound stem is a real user32.dll export, so those 75 are P1-clean
  **by fact**, not by assumption. It includes `358:InternalGetWindowText` (an undocumented export the layer wraps).
- `AWmacro | AW-macro-needed` 6 - `132:CreateWindowEx`, `197:FindWindow`, `204:FindWindowEx`, `337:GetWindowText`,
  `344:GetWindowTextLength`, `533:SetWindowText` bind only because the SDK A/W macro rewrites the name (the G1 case);
  confirm the macro is live at each site (partition + `UNICODE` conditions), which is exactly the P2 test.
- `AWmacro | NEITHER` **1 - `124:CreateWindow`**, and this is almost certainly the folder's **last unresolvable P1
  binding**: user32.dll exports `CreateWindowExW` but **no** `CreateWindow`/`CreateWindowW` (the SDK's `CreateWindow` is
  an inline `winuser.h` function), so `_NAME##W` cannot bind. P1's post-`mouse.cpp` count was "11 -> 1"; check whether
  that 1 is here, and confirm with `dumpbin /exports` + `cl /EP` on the TU rather than from the register.
- `AWmacro | bare-export` 3 - list them; if a bare export also has an A/W macro live, the AW macro is rewriting a name
  that would have bound anyway (P2-adjacent, cosmetic).

Other leads to carry in: **U12/U13/U18** - `windowproperties.cpp:173` unqualified `SetLayeredWindowAttributes`
(`registers/u12.md:18`) is double-gated (`WINVER >= 0x0600` :51-:205 and `_WIN32_WINNT >= 0x0600` :118-:197), so first
establish whether that TU compiles it at all, then compare `DWORD` against the SDK's `BOOL` (the folder's first U18
instance, `windowproperties.cpp:211`, is in the same file). **U17**: the earlier family scan flagged non-`T` names in
this range (`CallWndRetProc`, `GetCaretBidiMode`, `SetCaretBidiMode`, `GetWindowRgnBox`, `TrackPopupMenu`) - re-measure
per file; D10b says record, do not rename. **U19**: `window.cpp` carries the `TrackPopupMenu` BYPASS split lead, but
`_tmp_u19_tlayer_s26.ps1` needs the comment-stripping fix from `gotchas.md` before its flag counts are usable.
**U26**: `TagRECT` (`window.cpp:235` region, `TagRECT:BOOL:HDWP:HPAINTBUFFER:INT:INT:BOOL`) is the mirror question.
**U14(b)**'s three deducing `Call<>` sites and **U27**'s `SetScrollInfo` question are still parked here (D15 froze U14's
tally; the three sites are not part of it). `tmp/_tmp_s33_preflags.ps1` is **broken** (reported 0 heads) - do not run it;
the numbers above came from direct `Select-String` counts, recorded in `tooling.md`.

### Correction to a long-standing instrument (found here)
`_tmp_sigcmp.ps1`'s const test matched only the **first** `CONST`, so it **missed `CONST LPINT` typedef-const spellings
entirely**. `_tmp_s32_names.ps1` fixes it with `(const[ ]+[A-Za-z_][A-Za-z0-9_]*\*+)`. Consequence: any earlier
U18/U25 "0 divergences" claim resting on that test covers only `CONST X*` / `CONST::X*`, **not** `CONST LPINT`. Also
repaired this session: the **Queue table** (duplicated `text.cpp` row, missing `windowprocedure.cpp`, `window.cpp` at
270 when it is **615** lines) - audit instrument `tmp/_tmp_s32_queue_audit.ps1`, which is now the way to re-derive the
queue instead of trusting the table.
