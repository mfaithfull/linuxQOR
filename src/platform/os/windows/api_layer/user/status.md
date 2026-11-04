# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 4 Nov 2026 - **session 33 (`window.cpp`) CLOSED; session 34's TU is `windowclass.cpp`** (its pre-flags are
the last block of this file). **P1 is now CLOSED folder-wide - 0 unresolvable bindings of 428.** Review of
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
| `gotchas.md` (index) + `gotchas/<kind>.md` | instrument blind spots, evidence integrity, record-editing traps - the trigger block in `gotchas.md` names the file | when a trigger row matches the task at hand |
| `archive/tu/<tu>.md` - **one narrative per TU**, index in `archive/README.md` | that TU's narrative + verdict detail | only the TU you just reviewed |
| `archive/session-log.md` | the session-log table (rows 29-33) + pointers to the closure-note slices (`session-log-notes-24-30.md`, `session-log-notes-31-33.md`); older rows in the six `archive/session-log-<range>.md` slices | only while writing the row for the TU you just reviewed |
| `archive/history.md`, `archive/prescan-help.md` | consolidation/split history, consumed `help.cpp` pre-flags | never, unless auditing the record itself |

The rule: **findings, narratives and register text never go in this file.** New finding -> the register file the locator
names; new TU narrative -> `archive/tu/<tu>.md`; new row -> `archive/session-log.md`; new instrument -> `tooling.md`, new
caveat -> the `gotchas/` file its trigger names (index `gotchas.md`); folder-wide measures -> append to the **end** of `registers/census.md` (append-only). Hard rule:
**no record file over ~40 KB**, this file under 220 lines / 22 KB - when it is over, compress the **Live state** bullets and
the consumed pre-flags (that detail belongs in `archive/tu/` and the session-log row), never the locator.
Slice by exact line range, hash-verify the rejoin, re-point the indexes, then re-run the locator generator + the splice.

## Live state (4 Nov 2026)
- **34 of 38 TUs reviewed**; the 4 left are the Queue table below. **Session 33 (`window.cpp`) CLOSED 4 Nov 2026** - the
  folder's largest TU (615 -> 692 lines, **85 wrappers**) and the last one with defects: **6 code fixes**.
  **(1) P1 CLOSED folder-wide** - `CreateWindowT` bound `qor_pp_useswinapiAW(user32, CreateWindow)` -> `CreateWindowW`,
  which **is not an export and never can be** (`winuser.h:4464`/`:4468` make `CreateWindow`/`CreateWindowW`
  function-like macros to `CreateWindowExA/W`; the binding macro stringizes before any macro expands - G1). Now
  `…AW(user32, CreateWindowEx)` + an explicit `0` `dwExStyle` (`window.cpp:189`-`:200`); folder census **428 bindings,
  1 -> 0 unresolvable**. **(2) U18's founding pair fixed** - `LogicalToPhysicalPoint`/`PhysicalToLogicalPoint`
  `void`+`voidCall` -> `BOOL`/`Call<BOOL, HWND, ::LPPOINT>` (`:505`, `:528`) + `user32.h:188`, `:191` (a
  published-signature change, consumer probe clean). **(3-4) two dropped `const`s** restored (`:593`/`:597`,
  `:649`/`:653`). **(5) U14(b)'s last 3 deducing sites** made explicit -> **428 explicit / 0 deducing**. **(6)** the rest
  clean: 85/85 bindings literal-exported, 0 spaced parens, 0 live P5. U16 **34 of 85** prototypes version-gated (highest
  floor yet `0x0605`, three guard names new to the review); U26 8 caller structs layout-identical on **both arches**;
  U19 71 LAYER / 49 INDIRECT / 49 BYPASS (2 real groups) / 74 MEMBER and the **`TrackPopupMenu` lead discharged** (it
  belongs to `menu.cpp`). Row 33 + `archive/tu/window.md` + `registers/census.md` carry the detail.
- **Session 32 (`text.cpp`) CLOSED 4 Nov 2026** - 4 wrappers (the whole `user32.h:567` *Font and Text functions*
  section), **0 defects, 0 code edits** (comment block only). All 4 stems have their SDK A/W macro live at the binding
  site, so the `…WW` risk is **correct by construction** (the G1 case); **U23 gained a second class** - `CONST LPINT` on a
  pointer parameter is `INT* const`, not the SDK's `CONST INT *` (probe-proved), plus the `DT_MODIFYSTRING` `_Inout_`
  buffer contract hidden by the `LPCTSTR` parameter. U16 4/4 DESKTOP, 1/4 gated (`DrawTextExT`, `WINVER >= 0x0400`), new
  guard name `NODRAWTEXT`; U18/U19/U25/U26 0 instances.
- **Sessions 28-32** (`rectangle`, `resource`, `systemshutdown`, `scrollbar`, `text`) - `resource.cpp` held the folder's
  only **U11** pair (both definitions were free functions, so the `static` members at `user32.h:443`-`:444` had no
  definition anywhere; fixed by qualifying both with `User32::` - **U11 CLOSED**, `static`-with-no-definition now 0
  folder-wide); `systemshutdown.cpp` held the only two `WINAPI`-qualified definition heads (**ill-formed for x86**,
  no-op for x64; fixed); `rectangle`, `scrollbar` and `text` had **0 defects** (D15 closed U14 in `rectangle.cpp`).
  Full detail: rows 28-32 of `archive/session-log.md` + `archive/tu/`. Folder-wide measure from session 31:
  **x86 is unbuildable - 93 of 106 api_layer TUs fail at `kernel32.h:92` (`registers/u6.md`)**.
- **Sessions 23-27** - `message.cpp` (**P2 CLOSED**), `monitor.cpp` (first all-clean TU), `mouse.cpp` (**10 P1 binding
  fixes**, folder unresolvable 11 -> 1), `painting.cpp` (2 nits), `processthread.cpp` (folder's last TAB; U16's second
  0x0501 gate + the **empty** `#if`/`#endif` at `winuser.h:4121`; **T7**). `archive/tu/` narratives cover sessions 1-15 and
  21-33; sessions 16-20 have their narrative in the row only.

## Queue - 4 TUs left, alphabetical (lines = on-disk physical lines, re-measured 4 Nov 2026)
Re-derived by `tmp/_tmp_s32_queue_audit.ps1` (for each of the 38 `.cpp` files, count the session-log rows that name it) -
use it rather than trusting this table; `window.cpp` was row 33 and closed on 4 Nov 2026.

| # | TU | lines | # | TU | lines |
|--:|---|--:|--:|---|--:|
| 34 | `windowclass.cpp` | 114 | 36 | `windowprocedure.cpp` | 30 |
| 35 | `windowproperties.cpp` | 51 | 37 | `winqor_api_user.cpp` (glue) | 14 |

**209 lines remain**, all in small TUs. Still unbooked that these must not lose: `windowproperties.cpp`'s **U12/U13/U18**
pair (unqualified `SetLayeredWindowAttributes` at `:173`, double-gated `WINVER >= 0x0600` / `_WIN32_WINNT >= 0x0600`, and
the folder's first U18 type instance at `:211`) - the last cross-TU-ownership finding open; **T6**'s ownerless
`//List Box functions` section; **T7**'s 35 dead `returncheck.h` includes; and the per-TU wrapper counts the session-32
census note demanded for these four, now measured (`windowclass` 14 bindings, `windowproperties` 5, `windowprocedure` 2,
glue 0, plus `window.cpp`'s 85) - so **428 bindings is the closed folder total over all 38 sources, not a floor**
(`registers/census.md`, session-33 entry: both scans always walked all 38 files).

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
`registers/` tree. Generated by `tmp/_tmp_status_index_gen_s33.ps1` - **one bullet per id** (the entry head, plus a count of
later per-TU instance bullets; descriptions capped at 60 chars); never hand-edit it. The two `README.md` line-count columns
are refreshed by `tmp/_tmp_s33_readme_refresh.ps1`.

- **P1** `registers/prescan.md:10` bindings that cannot resolve at run time (export table + liv... (+2 per-TU instance bullets later in the file)
- **P2** `registers/prescan.md:60` bindings that work today only because the SDK macro rewrites...
- **P3** `registers/prescan.md:74` naming flags from the new T-suffix census (`tmp/_tmp_tsuffix...
- **P4** `registers/prescan.md:80` four exports in the table are forwarders, which the binding...
- **P5** `registers/prescan.md:88` the "wrong stem bound" defect class, and the instrument that... (+3 per-TU instance bullets later in the file)
- **P6** `registers/p6-u27.md:9` (promoted extract) Bare `CallMsgFilter` resolves and runs the ANSI entry point on wide arguments
- **G1** `registers/ground-rules.md:5` the SDK A/W macro rewrites the binding name before it is str...
- **G2** `registers/ground-rules.md:12` the nine stems `user32.h:9-17` `#undef`s are the real missin...
- **G3** `registers/ground-rules.md:24` the previous audit report is partly wrong. `tmp/_tmp_audit_r...
- **G4** `registers/ground-rules.md:30` module token case is cosmetic. `_TXT(User32)` stringizes to... (+1 per-TU instance bullet later in the file)
- **G5** `registers/ground-rules.md:33` the "already loaded image" assumption is not universal. In a...
- **G6** `registers/ground-rules.md:36` return checking is silently always on. `returncheck.h:19` te...
- **U1** `registers/u1.md:3` no call-site error handler anywhere in this library. Policy...
- **U2** `registers/u2.md:3` return-check policy is absent. Policy settled 29 Oct 2026 (u... (+5 per-TU instance bullets later in the file)
- **U3** `registers/u3.md:3` `hook.cpp` build-list status: RESOLVED (user commit `05d3ca2...
- **U4** `registers/u4.md:3` `user32.h` declares about 101 non-static members that no `.c...
- **U5** `registers/u5.md:3` the binding is cached for the process lifetime including fai...
- **U6** `registers/u6.md:3` calling-convention mismatch in the call path: RESOLVED 1 Nov... (+1 per-TU instance bullet later in the file)
- **U7** `registers/u7.md:3` SEH faults are swallowed into a default return. `Call` catch...
- **U8** `registers/u8.md:3` `throw("Missing library function.")` (`library.h:60`, `:92`)...
- **U9** `registers/u9.md:3` no `purpose.md` in this folder and no scope/audit note. `api...
- **U10** `registers/u10.md:3` `user32.h:19-62` defines `CONSOLECONTROL`, `CONSOLEENDTASK`,...
- **U11** `registers/u11.md:3` `resource.cpp` defines both Resource members without the `Us... (+1 per-TU instance bullet later in the file)
- **U12** `registers/u12.md:3` `accessibility.cpp` and `timer.cpp` held each other's wrappe...
- **U13** `registers/u13.md:3` one member was defined through `User32::` but declared non-s...
- **U14** `registers/u14.md:3` `Library::Call`/`voidCall` template-argument style is split... (+6 per-TU instance bullets later in the file)
- **U15** `registers/u15.md:3` wrappers that are exported through `QOR_WINAPIUSER` but have...
- **U16** `registers/u16.md:3` the wrappers pair with SDK declarations that the SDK only em... (+7 per-TU instance bullets later in the file)
- **U17** `registers/u17.md:3` A/W wrappers declared without the folder's `T` suffix. Opene...
- **U18** `registers/u18.md:3` a wrapper pair whose return type contradicts the SDK: `void`... (+2 per-TU instance bullets later in the file)
- **U19** `registers/u19.md:3` call sites that reach a `user32.dll` entry point through the... (+6 per-TU instance bullets later in the file)
- **U20** `registers/u20.md:3` the caller-side `Cursor`/`Icon` ownership model is wrong at...
- **U21** `registers/u21.md:3` the "DC functions" section is not the SDK's DC family, and t... (+1 per-TU instance bullet later in the file)
- **U22** `registers/u22.md:3` the `DeviceContext` release path this layer feeds calls `Rel...
- **U23** `registers/u23.md:3` `MessageBoxIndirectT`'s parameter puts `const` on the pointe...
- **U24** `registers/u24.md:3` the one in-tree `FlashWindow` caller reads its `BOOL` as a s...
- **U25** `registers/u25.md:3` `const` is spelled three ways in `user32.h`, and the `CONST:... (+1 per-TU instance bullet later in the file)
- **U26** `registers/u26.md:3` `TCHAR` width is an include-order property, not a build sett... (+2 per-TU instance bullets later in the file)
- **U27** `registers/p6-u27.md:9` (same extract) STRICT changes the identity of HOOKPROC and every DECLARE_HANDLE type
- **T1** `registers/tasks.md:5` two declared-but-undefined members in the "Authorization fun...
- **T2** `registers/tasks.md:18` the whole Window Station / Desktop surface is dead exported...
- **T3** `registers/tasks.md:29` the "Power Management functions" pair is declared but has no...
- **T4** `registers/tasks.md:52` the menu half of the context-help-id family is declared and...
- **T5** `registers/tasks.md:87` the IME/WINNLS pair in the unlabelled `user32.h:635-639` blo...
- **T6** `registers/tasks.md:102` the whole "//List Box functions" section is declared and def...
- **T7** `registers/tasks.md:121` 35 of the 38 `user` sources `#include "../returncheck.h"` an... (+2 per-TU instance bullets later in the file)

## Verification gate (identical every session)
Edited TU compiles standalone at `/W4` exit 0; the whole library compiles **38/38 at `/W4`, exit 0, no diagnostics**
(`tmp/_tmp_cl_all.bat` -> the shared `tmp/_tmp_cl_all.log`, snapshotted under a session name - never copy an old log as
proof, see `gotchas.md`; only the TU-name echoes may appear); `cl /EP` re-run after the last edit still yields the expected
binding literals; `user32.h` untouched unless the row says otherwise. The gate also carries the folder-wide **binding
census** (latest folder pass `tmp/_tmp_wrongstem_final_s33.txt`: 38 files, 428 `User32::` definitions, 428 bindings
scanned, **2** mismatched member/stem pairs, **both benign** - `cursor.cpp:64 CopyCursorT` binds `CopyIcon` (the
long-known P3 case) and `window.cpp:197 CreateWindowT` binds `CreateWindowEx` (session 33's deliberate P1 fix); binding
census `tmp/_tmp_p1census_final2_s33.txt`: 428 bindings, **0 unresolvable - P1 CLOSED**) and the per-stem bare-vs-AW
classifier (`tmp/_tmp_scroll_class_s30.ps1`, per-TU driver `tmp/_tmp_s33_prebind.ps1`). **Session 33's authoritative gate
is `tmp/_tmp_cl_all_final3_s33.log`** - run 4 Nov **22:25:10**, after `window.cpp`'s last edit (20:11:31): 38 echoes, 0
diagnostics, **0 stale `.obj`** (`tmp/_tmp_s33_gatecheck.ps1`). The earlier `final2` capture (19:43:37) is **not** citable -
`window.cpp` was edited at 20:11:31 afterwards, so that run is pre-edit; every other `tmp/_tmp_cl_all*s33*.log` is too.
**A clean 38-TU log cannot be dated by its hash** -
session 28's, 29's and 30's are byte-identical - so date one by `tmp/obj/<tu>.obj` being newer than the source (session 24's
`_s24.log` and 25's `_s25b.log` were pre-edit runs and must not be cited). Library-header edits must additionally pass the
**all-libraries gate** `tmp/_tmp_cl_all_apiall_u6.bat` (106 sources), meaningful only as a diff against a pre-fix run
(pre-existing `C1083` at `opengl/wgl.cpp:31` plus 165 warnings on HEAD).

## Pre-flags for session 34 (`windowclass.cpp`) - measured 4 Nov 2026

**114 lines, 14 wrappers, no contract block** (3 comment lines, longest line 124, 0 `WINAPI` tokens). Header section
`user32.h:619` `//Window-Class functions`, declarations `:620`-`:633` - **14 declarations, 14 definitions**, so U4/U11/U13
are expected clean here; `qor_pp_useswinapiAW` = **12**, direct `qor_pp_useswinapi` = **2**, `qor_pp_fcontext` = 14,
`Library::Call<` = 14, `Library::voidCall` = **0** (so no U18 `void`-converter class), binding sites `:19`-`:110`.

- **Binding is pre-cleared by fact, but still run the check.** All 12 AW stems have **`<Stem>A` and `<Stem>W` exports
  and NO bare export** (`GetClassInfo`, `GetClassInfoEx`, `GetClassLongPtr`, `GetClassName`, `GetWindowLong`,
  `RegisterClass`, `RegisterClassEx`, `SetClassLongPtr`, `SetWindowLong`, `SetWindowLongPtr`, `GetWindowLongPtr`,
  `UnregisterClass` - measured case-sensitively against `tmp/_tmp_user32_exports.txt`), and the 2 direct stems
  (`GetClassWord`, `SetClassWord`) are **bare single exports with no A/W variants** - i.e. D13 spelling is already
  correct on both sides and P1/P2/P6 cannot arise. Confirm with `cl /EP` + `tmp/_tmp_win_ppclass_s33.ps1` (the
  `/EP`-authority classifier), never from the source text.
- **Watch the `*Ptr` / non-`Ptr` pairs for a U18/UB question rather than a binding one**: `GetWindowLongT` returns `LONG
  and `Get/SetWindowLongPtrT` return `LONG_PTR` (`user32.h:625`, `:631`, `:632`) - the SDK's position is that
  pointer-sized values (GWL_USERDATA etc.) need the `Ptr` form on x64; check what the layer's `LONG` members can truncate
  and whether `windowcontroller.cpp` mixes them. `GetClassLongPtrT`/`SetClassLongPtrT` are `ULONG_PTR`/`LONG_PTR` -
  compare against the SDK return types with `tmp/_tmp_s32_names.ps1` (NOT `_tmp_sigcmp.ps1`, whose const test is broken).
- **U25 traffic in the header**: `user32.h:626`/`:627` spell `CONST WNDCLASS*` / `CONST WNDCLASSEX*` (the `CONST X`
  shape); the TU's own spelling must be read case-sensitively.
- **U16**: expect version floors (`GetClassInfoEx`, the `*Ptr` family) - use a depth-counted guard walk only, and record
  the guard NAMES as well as the floors (session 33 found three names new to the review).
- **U17**: **12 of the 14** members carry the folder `T` suffix and `GetClassWord`/`SetClassWord` correctly do not, so
  the T-suffix census should report 0 deviations here - verify, do not assume.
- **Neighbouring header block to book while here**: `user32.h:635` `void DisableProcessWindowsGhosting(void);` is
  **non-static** and defined in no TU (U4/T-form). It sits directly under this TU's section, so the row for session 34
  is the natural place to re-cite it - do not "fix" it without the U4/U13 sweep decision.
- **Method reminders**: re-run `_tmp_s32_queue_audit.ps1` before booking the row; date the gate by `.obj` freshness
  (`tmp/_tmp_s33_gatecheck.ps1` shape); count `CONST`/`const` with `-CaseSensitive`; write the `archive/tu/` narrative
  only from final measurements (session 33's had to be corrected four times); re-run `tmp/_tmp_s33_cite_audit.ps1` (the
  generic version of the s31 audit - it catches artefacts cited but never written, which happened once this session) and
  `tmp/_tmp_u15_locator_verify.ps1`. **Record ceiling**: `tooling.md` is at **37.6 KB** and `registers/census.md` at
  **35.4 KB** - slice either with `tmp/_tmp_s33_slice_log.ps1` (the hash-verified splice) as soon as it passes 40 KB.

