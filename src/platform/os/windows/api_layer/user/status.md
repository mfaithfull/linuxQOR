# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 1 Nov 2026 (session 21 `mdi.cpp` closed). **Session 21's code is uncommitted** - the working tree carries
`mdi.cpp` plus this session's record writes, and on top of that the still-uncommitted off-protocol **U6** fix
(`api_layer/library.h`) with its `cursor.cpp`/`method.md` follow-ups. Session 20's code is in `4948838d`. Review of
`src/platform/os/windows/api_layer/user` (the `User32` wrapper class over `user32.dll`) for correctness, consistency,
safety, undefined behaviour and conformance to the api_layer conventions. Protocol: **one translation unit per session** -
the whole point is not to lose findings between sessions. Small, unambiguous defects get fixed as they are found; anything
that changes library-wide behaviour or needs a decision goes in the register and is *not* touched.

## Where the record lives - start here, then open only what the step needs

| File | Contents | Open it when |
|---|---|---|
| `status.md` (this file; **stay under 180 lines / 18 KB** - 177 / 17.9 KB after the session-21 trim) | live state, queue, the two convention decisions, register locator, defect census pointer, verification gate, current TU pre-flags | every session, first |
| `registers/<file>.md` - **one register entry per file**, index in `registers/README.md` | verbatim register text: P1-P5 `prescan.md`, P6+U27 `p6-u27.md`, G1-G6 `ground-rules.md`, U1-U26 `u<n>.md`, T1-T4 `tasks.md`, census `census.md` | only the ids named by the locator below or by the current TU's pre-flags |
| `method.md` | the per-TU review method, tooling index, **instrument caveats** | before running an instrument or the gate |
| `archive/tu/<tu>.md` - **one narrative per TU**, index in `archive/README.md` | that TU's narrative + verdict detail | only the TU you just reviewed |
| `archive/session-log.md` | the session-log table (rows 1-21) + closure notes - **over the ~40 KB ceiling, see the hygiene bullet in "Live state"** | only while writing the row for the TU you just reviewed |
| `archive/history.md`, `archive/prescan-help.md` | consolidation/split history, consumed `help.cpp` pre-flags | never, unless auditing the record itself |

The rule: **findings, narratives and register text never go in this file.** New finding -> the register file the locator
names; new TU narrative -> `archive/tu/<tu>.md`; new row -> `archive/session-log.md`; new instrument or caveat ->
`method.md`. This file, `registers.md` (1,063 lines) and `session-log-archive.md` (1,394) each outgrew a session's context
and were split on 1 Nov 2026. Hard rule: **no record file over ~40 KB**; read tally entries for their **head only**, append at the **end**.

## Live state (1 Nov 2026)

- **22 TUs reviewed** (sessions 1-21): `accelerator`, `accessibility`, `brush`, `caret`, `clipboard`, `combobox`,
  `console`, `timer`, `coordtransform`, `cursor`, `desktop`, `devicecontext`, `devicemanagement`, `dialog`,
  `errorhandling`, `filledshapes`, `help`, `hook`, `icon`, `ime`, `keyboard`, `mdi`.
- **Sessions 19-20 (`ime.cpp`, `keyboard.cpp`) CLOSED 1 Nov 2026** - full narrative in `archive/tu/ime.md` and
  `archive/tu/keyboard.md` and rows 19-20 of `archive/session-log.md`; code in `f2f87453` and `4948838d`, both gate-green
  **38/38** at `/W4`. Headline: `ime.cpp` had 0 defects and its AW binding is load-bearing (no SDK macro in scope to
  rewrite it); `keyboard.cpp` fixed 3 P1 binding defects (the bare macro stringized the member's own `*T` spelling) and
  produced the folder-wide AW checklist, which is what flagged **all 10 `mouse.cpp` wrappers broken** (`prescan.md` P1).
- **Session 21 (`mdi.cpp`) CLOSED 1 Nov 2026** - 4 wrappers, **0 binding / correctness / UB / convention defects**: all
  four bindings already D13-correct (`/EP` + export table + live probe `tmp/_tmp_mdi_probe_s21.txt`, which measured **G5**
  directly - `GetModuleHandleW(L"user32")` fails with gle 126 in a bare console process), and signatures **and parameter
  names** match `winuser.h` field-for-field (0 U18/U17/U11/U13, U16 0 of 4). One internal fix: `DefFrameProcT`'s `Call<>`
  named `INT` where its signature and both SDK prototypes say `UINT` - the folder's only uppercase-`INT` template argument
  (new instrument `tmp/_tmp_int_sites_s21.ps1`: 1 hit of 435 such lists, now 0). Plus a 67-line TU comment; 44 -> 112 lines.
  Registers: **U14** new sub-class (an explicit site can be wrong as well as verbose), U15 4/4, U16 0/4, U17 0, U18 0,
  **U19 1/1/0/0** - the **first family of the review with 0 SDK bypass**, because there is no MDI host anywhere in the tree
  - **U21** third instance (`#ifndef NOMDI`, `winuser.h:12074`-`:12261`, split 4 here + 3 in `//Window functions` ->
  `window.cpp`) and **U26** third mirror: `Message` vs `::MSG` byte-identical on x64, divergent from `lParam` onward on x86
  (`tmp/_tmp_msg_mirror_s21.cpp` + `.bat`, the review's first two-architecture layout measurement), 7 call sites across 6
  wrappers, latent while only `x64-Debug` exists, handed to `message.cpp` (23). Gate green **38/38** at `/W4`, exit 0.
- **Record hygiene, act before session 22 writes anything:** `archive/session-log.md` is **45.4 KB**, over the ~40 KB
  ceiling this file sets. Split the oldest rows (1-8) into `archive/session-log-1-8.md` at the **start** of session 22 by
  the slice-and-rejoin-then-hash procedure of the 1 Nov splits (`archive/history.md`), then re-point both README indexes.
- **U6 CLOSED 1 Nov 2026 (off-protocol item fix, not a TU session).** `Call`/`voidCall` now call through a `__stdcall`
  `fPtr` (`library.h:46`, `:68`), matching `DefProc` (`:16`) and the `WINAPI` exports behind all **1,638** bindings in the
  tree; proved by an x86 disassembly probe (`tmp/_tmp_u6_cc_probe.cpp`: the caller-side `add esp,0Ch` after the indirect
  `call` is gone). Gate: user folder 38/38 at `/W4` exit 0 plus a new **all-libraries** gate over 106 api_layer sources
  with byte-identical pre-/post-fix logs (`method.md` section 1). `registers/u6.md` carries it; U7/G5 stay open.

## Queue - 16 TUs left, alphabetical (lines = on-disk physical lines, measured 1 Nov 2026)

| # | TU | lines | # | TU | lines |
|--:|---|--:|--:|---|--:|
| ~~20~~ | ~~`keyboard.cpp`~~ done | - | 29 | `rectangle.cpp` | 93 |
| ~~21~~ | ~~`mdi.cpp`~~ done | - | 30 | `resource.cpp` | 30 |
| 22 | `menu.cpp` | 289 | 31 | `scrollbar.cpp` | 102 |
| 23 | `message.cpp` | 184 | 32 | `systemshutdown.cpp` | 51 |
| 24 | `monitor.cpp` | 51 | 33 | `text.cpp` | 44 |
| 25 | `mouse.cpp` | 86 | 34 | `window.cpp` | **615** |
| 26 | `painting.cpp` | 191 | 35 | `windowclass.cpp` | 114 |
| 27 | `processthread.cpp` | 57 | 36 | `windowprocedure.cpp` | 30 |
| 28 | `winqor_api_user.cpp` (glue) | 14 | 37 | `windowproperties.cpp` | 51 |

2,002 lines in total, and **1,279 of them are in four TUs**: `window`, `menu`, `painting`, `message` - which hold every
remaining P2 (**`menu.cpp:159,194`, `message.cpp:124,145,152,159`**) plus `window.cpp:124`. The biggest *unflagged*
finding is in `mouse.cpp` (86 lines): **all 10 of its wrappers bind a name that does not exist** - see the P1 entry in
`registers/prescan.md`. `window.cpp` (615 lines, 85 bindings, P3 `CreateWindowT`, U18's `void` converters) may not fit one
session - decide how to split it before starting it, not mid-session. Two hand-offs the queue should not lose: `message.cpp`
(23) inherits the `Message`/`::MSG` mirror question and its seven call sites (**U26**, session 21), and `window.cpp` (34)
inherits the two version-guarded MDI members `CascadeWindows`/`TileWindows` (**U16**, session 21).

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
- **P2** `registers/prescan.md:60` bindings that work today only because the SDK macro rewrites them; convert to explicit AW
- **P3** `registers/prescan.md:66` naming flags from the new T-suffix census (`tmp/_tmp_tsuffix_census.ps1`, session 6). Two members ca...
- **P4** `registers/prescan.md:72` four exports in the table are forwarders, which the binding question cannot see (found in session 7)...
- **P5** `registers/prescan.md:80` the "wrong stem bound" defect class, and the instrument that finds it (`tmp/_tmp_wrong_stem_scan.ps1...
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
- **T5** `registers/tasks.md:80` the IME/WINNLS pair in the unlabelled `user32.h:635-639` block is declared and defined nowhere. Open...

## Defect census

- The sessions-1-21 census (run-time defects fixed, what is still open, convention fixes, folder-wide measures) is kept verbatim at `registers/census.md:3` - refresh it there when a session closes.

### Verification gate (identical every session)
Edited TU compiles standalone at `/W4` exit 0; the whole library compiles **38/38 at `/W4`, exit 0, no diagnostics**;
`cl /EP` re-run after the edit still yields the expected binding literals; `user32.h` untouched unless the row says
otherwise. Latest closure run `tmp/_tmp_cl_all_s21c.log` (exit 0, 38 TU echo lines, 0 hits for `error C|warning C|fatal`);
earlier runs `_tmp_cl_all_s9/s11*/s12/s13/s17/s18/s19b/s19c/s20/s21b.log`, and the U6 fix run `_tmp_cl_all_u6post.log`. Session 20
also adds the **binding census** (`tmp/_tmp_kb_p1census_s20.ps1`) to the gate - run it folder-wide, not just on the edited
TU; it is the only instrument that sees a wrongly-suffixed stem. **Addition from the U6 fix:** any edit to a header
shared across api_layer libraries (today only `api_layer/library.h`) must additionally pass the **all-libraries gate**
`tmp/_tmp_cl_all_apiall_u6.bat` (106 sources via `tmp/_tmp_u6_apiall_sources.rsp`), compared against a pre-fix run -
that gate carries a pre-existing `fatal error C1083` at `opengl/wgl.cpp:31` plus 165 warnings on HEAD, so its exit code
is only meaningful as a diff.

## Pre-flags for the current TU (rolling - keep this under ~40 lines)

**Session 22 `menu.cpp`** (289 lines; 39 wrappers = the whole `user32.h:214`-`:253` "//Menu functions" section, defs
`:16`-`:282` in header order). Measured 1 Nov 2026 into `tmp/_tmp_menu_pre_s22.txt` (`sigcmp` over decls 214-254): all
39 decl/def pairs parse and **0 signature flags** - the `mdi.cpp` uppercase-`INT` class is absent here. Bindings: 39, of
which 7 are `AW` (`:19,138,187,208,215,229,271`), 30 bare; the folder census (`tmp/_tmp_p1census_s21.txt`) puts this TU
at **2 unresolvable** - `:159 GetMenuString`, `:194 InsertMenuItem`, the **P2** class (A/W family bound bare, alive today
only because the SDK macro rewrites the stem), so expect 2 D13 `AW` rewrites, not a `mouse.cpp`-scale fix. All 9 A/W
members carry the `T` suffix, so 0 U17 expected. U14: all 39 `Call`/`voidCall` sites already name their template
arguments (0 deducing here - the folder's 4 survivors are `mouse.cpp:83`, `window.cpp:118,254,317`).

## How this record is split

Both 1 Nov 2026 splits are described in `archive/history.md` ("How the record was split"): `status.md` (1,222 lines)
into four files, then `registers.md` (1,063) into `registers/` and `session-log-archive.md` (1,394) into `archive/`, by
exact line range, verified by re-joining every slice and hashing the result against the original bytes (both `MATCH`).
**Regenerate the locator above with `tmp/_tmp_status_index_gen_s21.ps1` whenever a register entry moves.**
