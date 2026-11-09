# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 9 Nov 2026 - **T9 step 9a is IN (D27): a new `strings.cpp` TU defines all 34 `//String functions` members of `user32.h:342-375` with 34 x bare `qor_pp_useswinapi` bindings and 0 x `qor_pp_useswinapiAW` (the members already carry the A/W variant suffix, so D13's binding literal **is** the exported name), and `user32.h:342-375` gains `static` on all 34 declarations - no type and no parameter name moves, so no mangled name moves.** T2 is CLOSED (D25): `desktop.cpp`'s two `Enum*` callbacks now take the SDK's **W** spellings `DESKTOPENUMPROCW`/`WINSTAENUMPROCW` - at `user32.h:410`/`:413`, at the `desktop.cpp:86`/`:107` definition heads and in the two `Library::Call<>` argument lists at `desktop.cpp:90`/`:111` - because both wrappers bind `qor_pp_useswinapiAW` and the exports they bind (`EnumDesktopsW` `WinUser.h:1480`, `EnumWindowStationsW` `:1610`) spell their callback parameter that way per D18. The edit moves no type and no mangled name (`tmp/_tmp_t2_probe_id.cpp` exit 0 on four `static_assert`s: `is_same<DESKTOPENUMPROC,DESKTOPENUMPROCW>`=1 etc.), and the parameter stays type-checked (`tmp/_tmp_t2_probe_neg.cpp` exit 2 rejecting an A-shaped callback, `tmp/_tmp_t2_probe_farproc.cpp` exit 2 rejecting the `!STRICT` FARPROC shape). U17 took no action (this section was already 9/9 compliant) and the section's 18/18 deadness stays D14's territory. Folder gate 47/47 exit 0 and the 115-source all-libraries logs SHA-256 identical pre/post, so **all eight deferred tasks T1-T8 are now CLOSED.** T6 is CLOSED (D24): a new `listbox.cpp` TU defines the three `//List Box functions` members - `DlgDirList` (exports C1/C4) and `DlgDirSelectEx` (C7/C8) bound `qor_pp_useswinapiAW`, `GetListBoxInfo` (export 174) bound bare `qor_pp_useswinapi` - and `user32.h:552-554` gains `static` on all three, with `DlgDirSelectEx`'s declaration moved to the SDK's parameter names `hwndDlg`/`chCount`/`idListBox` per D18.** T5 is CLOSED: `ime.cpp` now owns the IME/WINNLS block - `DisableProcessWindowsGhosting` and `WINNLSEnableIME` gain `static` with bare `qor_pp_useswinapi` definitions, and the two WINNLS siblings `WINNLSGetIMEHotkey`/`WINNLSGetEnableStatus` are added as `static` declarations plus definitions; `WINNLSEnableIME` keeps a one-line obsolete comment and stays on the interface.** T4 is also CLOSED: `help.cpp` gains `SetMenuContextHelpId` (export 34A) and `GetMenuContextHelpId`
 (export 17C), bound bare `qor_pp_useswinapi`, and `user32.h:744-745` gains `static` on both.** T3 is also CLOSED:
 `powermanagement.cpp` defines `RegisterPowerSettingNotification` (export 2F1) and `UnregisterPowerSettingNotification`
 (export 3CB), bound bare `qor_pp_useswinapi`, and `user32.h:788-789` gains `static` on both. T1 is also CLOSED: a new
 `security.cpp` TU defines `GetUserObjectSecurity` (export 1DB) and `SetUserObjectSecurity` (export 375), bound bare
 `qor_pp_useswinapi`, and `user32.h:760-761` gains `static` on both, and T5 wraps the IME/WINNLS block in `ime.cpp`, and T6 wraps the List Box trio in a new `listbox.cpp`, and T9 step 9a wraps the String section in a new `strings.cpp` - so the folder is 48 sources / 634 member-declaration lines (582
 static, 52 non-static) / 581 binding lines, and T1's, T3's, T4's, T5's, T6's and T9 9a's declared-but-undefined + non-static halves are gone
 (U4 declared-but-undefined -> defined, U18 non-static -> static; `_tmp_static_census.ps1` re-run post-edit reads 634 member-declaration lines / 582 static /
 52 non-static over 48 sources and 0 defined-but-not-static, 0 defined-without-a-declaration - the same instrument read 545 static / 89 non-static on the pre-edit header, and the folder's binding lines move 544 -> 581 over 46 -> 48 sources. Coverage stays **759 of 1,018** - T6's trio were already declared and so already covered;
 T5's close moved it 757 -> 759 by additionally declaring the two WINNLS entry points `WINNLSGetIMEHotkey`/`WINNLSGetEnableStatus` that were declared nowhere, and both are real exports. The T4 shared-header `user32.h` edit was gated as a diff
 against the standing 114-source all-libraries baseline: 114 -> 114 cpp echoes, 165 warnings = 165, 1 `C1083` both sides,
 exit 2 both sides, set-difference 0 new / 0 gone (T1's was diffed the same way against the 113 -> 114 baseline).** The
 prior close stands: **T8 step 6g is in (`legacy.cpp` wraps the 6 held-open legacy names), U18 and T8 are CLOSED**
(D19 decided the 6 names in scope, D20 fixed the variadic wrapper shape; `docs/agent/DECISIONS.md`). The 38 review-queue TUs stay closed from
session 37**; **U25 and U23 are both in on the code lines** (7 Nov 2026: `CONST` normalised to lowercase `const`
folder-wide - **0** `CONST` tokens on code lines, the 24 that remain are comment lines in 10 TUs - and the 5 class-A
const-on-pointer sites rewritten to `const BYTE*` / `const INT*` / `const ::MSGBOXPARAMS*`, which moves 5 mangled
names `QE..` -> `PE..`**; **U16 is CLOSED (D21: the SDK version triple is pinned in `kernel32.h`, floor asserted)**. Reviewing `src/platform/os/windows/api_layer/user` (the `User32` wrappers over `user32.dll`) for correctness,
consistency, safety, UB and conformance to the api_layer conventions. One translation unit per session, and **commit at
close**: a clean `git status` plus this file is all a session needs to resume after a context reset.

**Purpose (authoritative: `purpose.md`, added 5 Nov 2026; policy D18 + same-day scope addendum in
`docs/agent/DECISIONS.md`): the library intends a full set of wrapper functions for the exports of `user32.dll` - scoped to
the exports `winuser.h` declares, superseded families excluded.** A wrapper is judged against the export it binds, and an
unwrapped declared export is a gap (**8 names** remain raw after T8 steps 5 and 6a-6g, listed in
`registers/u18-coverage.md`, and **all 8 carry written exclusions** - 7 superseded twins whose `*Ptr`/`Ex` replacement this
library wraps and 1 instrument artifact with no prototype). **The backlog is complete: every one of the 109 names is wrapped
or excluded, so U18 and T8 are CLOSED and there is no actionable T8 lead left (D19 put the last 6 names in scope, D20 gave
`wsprintfA`/`wsprintfW` a variadic wrapper).**

## Record protocol v3 - a close is a commit (adopted 5 Nov 2026, after session 36)

v2 cut the boot read-set, but session 36 still compacted during its write pass: it measured the TU, wrote its banner and
passed the gate, then spent the rest of the session updating the record and never reached the commit. Four measured
causes, all fixed here - (a) retired v1 rules were still written as imperatives inside the live procedure file
(`method.md`), so a session runs audits it has already been told to stop running; (b) this file carried per-session prose
that already exists in the TU banner, so every close rewrote about half of it (71 of 134 lines at session 36) and then
re-read it to convince itself nothing was lost; (c) the file was over its own 120-line ceiling, which forces a
compress-then-recheck pass on every close; (d) the close was left uncommitted, which is what makes the next session
re-derive everything. **No record content was deleted** - the superseded procedure text is verbatim at
`archive/protocol-v1-2026-11-05.md`.

1. **The close ends with `git commit`, and the session is not closed before it happens.** `git log` / `git show` are the
   resumption record. A later session never re-reads this tree to find out what an earlier one did.
2. **Every artefact is written once and not opened again in the same session.** The commit's diff is the audit; the compile
   gate below is the only gate. No cite audit, no locator verify, no queue audit, no read-back pass.
3. **The TU's in-source review block is the narrative.** Every reviewed TU gets a `//=== api_layer conventions ====`
   banner at the top of its `.cpp` (`windowprocedure.cpp:16-73` is the current model: counts, the instrument slug behind
   each count, the verdict for every id the TU touches, and why anything was recorded rather than fixed). It cannot drift,
   because it sits on the code it describes, and it needs no index entry. `archive/tu/<tu>.md` (sessions 1-15, 21-33)
   stays as frozen history - open one only to research a closed TU.
4. **This file carries no per-session prose.** One row per source file in the queue below; closing a session changes that
   row's `status` cell and nothing else here. No session section, no verdict summary, no hand-off bullets - the banner in
   the source holds all of it. This is what keeps the per-close diff at one or two lines.
5. **Pre-flags for the next TU live in `preflags.md`** - written wholesale by the closing session, read once at boot,
   replaced wholesale at the next close. Never edited in place, never carried forward, never diffed against its previous
   version (git holds that). Measuring the next TU from source beats inheriting numbers.
6. **Retired procedure does not stay in a live file.** Nothing in `method.md` may order a step that is no longer
   performed; superseded procedure moves to `archive/`. Annotating a retired imperative with "superseded" does not stop
   the next session from running it - that is how sessions 34-36 lost their write passes.
7. **Registers hold the rule, never a running tally.** A **new** open item gets `registers/u<n>.md` (`u28.md` is the
   shape); no register is ever appended to, and a closed or frozen one is never opened at session start
   (`registers/census.md`, `u16.md`, `u19.md`, `u26.md`).
8. **Never cite `tmp/` artefacts in durable text.** `tmp/` is deleted at task end (`memory-bank/techContext.md:36-38`), so
   such a citation is dead as it is written; name the instrument's script slug instead.
9. **Boot read-set = `purpose.md` + this file + `method.md` + `preflags.md` + the TU in hand** (about 30 KB of the folder's 91 markdown
   files / 788 KB, measured 5 Nov 2026). Never open an `/EP` capture or `tmp/_tmp_*_pp_*.txt`; never read a whole register;
   open `archive/` or `gotchas/sessions-*.md` only for a named TU or session. **This file is 184 lines** - the
   ceiling is structural, not a task at close: the queue holds one fixed row per source (45 rows since `legacy.cpp`
   joined on 7 Nov 2026), the protocol and the gate are fixed text, and a close adds no prose. Growing this file breaks 4.

## Resuming after a context reset - ask git, not the record

    git -C c:\RAID0\Develop\linuxQOR log --oneline -3 -- src/platform/os/windows/api_layer/user
    git -C c:\RAID0\Develop\linuxQOR status --porcelain -- src/platform/os/windows/api_layer/user
    git -C c:\RAID0\Develop\linuxQOR grep -n "=== api_layer conventions" -- src/platform/os/windows/api_layer/user

An empty second command under a `close session N` head means session N is closed and there is nothing to re-derive. The
third lists every TU that carries its own banner (measured 7 Nov 2026: **13 of 45** - `text.cpp`, `window.cpp`, `windowclass.cpp`,
`windowprocedure.cpp`, `windowproperties.cpp`, `winqor_api_user.cpp` and the seven coverage TUs `pointer.cpp`, `dpi.cpp`,
`touch.cpp`, `displayconfig.cpp`, `powermanagement.cpp`, `misc.cpp`, `legacy.cpp`); every other closed TU is named in the queue column
below. If `git status` is not empty, the unfinished close **is** the state - finish it, do not re-measure it.

## Queue - all 45 sources, alphabetical (one row each; a close edits one `status` cell)

Generated once 5 Nov 2026 by a file scan (line count of the `.cpp`, presence of `//=== api_layer conventions`, presence of
`archive/tu/<name>.md`). Not regenerated - the queue is complete, so nothing can drift except the one cell being closed.
`lines` counts the source as it stands now, including its review banner.

| TU | lines | narrative | status |
|---|--:|---|---|
| `accelerator.cpp` | 59 | `archive/tu/accelerator.md` | closed |
| `accessibility.cpp` | 49 | `archive/tu/accessibility-timer.md` | closed |
| `brush.cpp` | 28 | `archive/tu/brush.md` | closed |
| `caret.cpp` | 77 | `archive/tu/caret.md` | closed |
| `clipboard.cpp` | 177 | `archive/tu/clipboard.md` | closed |
| `combobox.cpp` | 58 | `archive/tu/combobox.md` | closed |
| `console.cpp` | 49 | `archive/tu/console.md` | closed |
| `coordtransform.cpp` | 56 | `archive/tu/coordtransform.md` | closed |
| `cursor.cpp` | 165 | `archive/tu/cursor.md` | closed |
| `desktop.cpp` | 173 | `archive/tu/desktop.md` | closed |
| `devicecontext.cpp` | 114 | `archive/tu/devicecontext.md` | closed |
| `devicemanagement.cpp` | 86 | `archive/tu/devicemanagement.md` | closed |
| `dialog.cpp` | 191 | `archive/tu/dialog.md` | closed |
| `displayconfig.cpp` | 195 | banner `:9` | closed (T8 6d, added 6 Nov 2026) |
| `dpi.cpp` | 271 | banner `:9` | closed (T8 6b, added 6 Nov 2026) |
| `errorhandling.cpp` | 92 | `archive/tu/errorhandling.md` | closed |
| `filledshapes.cpp` | 98 | `archive/tu/filledshapes.md` | closed |
| `help.cpp` | 143 | `archive/session-log*.md` + banner `:15` | closed (T4, 8 Nov 2026) |
| `hook.cpp` | 107 | `archive/session-log*.md` | closed |
| `icon.cpp` | 168 | `archive/session-log*.md` | closed |
| `ime.cpp` | 54 | `archive/session-log*.md` | closed |
| `keyboard.cpp` | 316 | `archive/session-log*.md` | closed |
| `legacy.cpp` | 162 | banner `:12` | closed (T8 6g, added 7 Nov 2026) |
| `listbox.cpp` | 106 | banner `:9` | closed (T6, added 8 Nov 2026) |
| `mdi.cpp` | 111 | `archive/tu/mdi.md` | closed |
| `menu.cpp` | 358 | `archive/tu/menu.md` | closed |
| `message.cpp` | 273 | `archive/tu/message.md` | closed |
| `misc.cpp` | 255 | banner `:12` | closed (T8 6f, added 7 Nov 2026) |
| `monitor.cpp` | 161 | `archive/tu/monitor.md` | closed |
| `mouse.cpp` | 166 | `archive/tu/mouse.md` | closed |
| `painting.cpp` | 260 | `archive/tu/painting.md` | closed |
| `pointer.cpp` | 292 | banner `:4` | closed (T8 6a, added 6 Nov 2026) |
| `powermanagement.cpp` | 139 | banner `:9` | closed (T8 6e, added 7 Nov 2026) |
| `processthread.cpp` | 122 | `archive/tu/processthread.md` | closed |
| `rectangle.cpp` | 173 | `archive/tu/rectangle.md` | closed |
| `resource.cpp` | 40 | `archive/tu/resource.md` | closed |
| `scrollbar.cpp` | 155 | `archive/tu/scrollbar.md` | closed |
| `strings.cpp` | 328 | banner `:13` | closed (T9 9a, added 9 Nov 2026) |
| `systemshutdown.cpp` | 124 | `archive/tu/systemshutdown.md` | closed |
| `text.cpp` | 125 | banner `:16` | closed |
| `timer.cpp` | 32 | `archive/tu/accessibility-timer.md` | closed |
| `touch.cpp` | 249 | banner `:9` | closed (T8 6c, added 6 Nov 2026) |
| `window.cpp` | 705 | banner `:16` | closed |
| `windowclass.cpp` | 160 | banner `:16` | closed |
| `windowprocedure.cpp` | 89 | banner `:16` | closed |
| `windowproperties.cpp` | 113 | banner `:16` | closed |
| `winqor_api_user.cpp` | 83 | banner `:4` | closed |

**38 of 38 review-queue sources closed** in sessions 1-37 - the queue is empty, the folder review is complete (some
sessions covered two TUs; sessions 16-20 exist nowhere outside the session-log rows). `pointer.cpp` (T8 step 6a),
`dpi.cpp` (6b), `touch.cpp` (6c), `displayconfig.cpp` (6d), `powermanagement.cpp` (6e), `misc.cpp` (6f) and `legacy.cpp` (6g) joined on 6-7 Nov
2026 as coverage - **48 sources** (`security.cpp` for T1 and `listbox.cpp` for T6 joined 8 Nov 2026, `strings.cpp` for T9 9a joined 9 Nov 2026) - each narrative is its own banner, and those ten are the only rows not from a review
session. History is write-once and not boot material: `archive/session-log*.md`, `archive/tu/<tu>.md`. Append nothing.

## Open items - id to file (derivable; there is no index to maintain)

- `U<n>` -> `registers/u<n>.md` | `P1`-`P5` -> `registers/prescan.md` | `P6`, `U27` -> `registers/p6-u27.md` |
  `G1`-`G6` -> `registers/ground-rules.md` | `T1`-`T8` -> `registers/tasks.md` | folder totals -> `registers/census.md`.
- Closed / frozen - append nothing: **T1** (8 Nov 2026: `security.cpp` defines the Authorization pair + `static` at
  `user32.h:760-761`), **T3** (8 Nov 2026: `powermanagement.cpp` defines the Power Management pair + `static` at
  `user32.h:788-789`) and **T4** (8 Nov 2026: `help.cpp` defines the menu context-help-id pair + `static` at
  `user32.h:744-745`), **T5** (8 Nov 2026, D23: `ime.cpp` defines the IME/WINNLS pair + `static` and adds the two WINNLS siblings), **T6** (8 Nov 2026, D24: a new `listbox.cpp` defines the List Box trio + `static` at `user32.h:552-554`), **P1, P2, P5** (4 Nov 2026: 428 bindings, 0 unresolvable), **U11** (session 29,
  3 Nov 2026), **U14** (D15), **U15** (D14), **U6**, **U3**, **T7** (5 Nov 2026, D16: the 35 dead `../returncheck.h`
  includes are gone, the scan now reads 2 includes / 0 without use), **T2** (8 Nov 2026, D25: `EnumDesktopsT`/`EnumWindowStationsT`
  take the SDK's `DESKTOPENUMPROCW`/`WINSTAENUMPROCW` at `user32.h:410`/`:413` - type- and symbol-neutral), **U28** (5 Nov 2026, D17: Windows is x64-only, so the
  four `*Ptr` bindings in `windowclass.cpp` are correct for the supported image), **U18** and **T8** (7 Nov 2026, D19 + D20:
  the 109-name backlog is wrapped or excluded, and the 8 raw gap names all carry written exclusions), **U23** (7 Nov 2026:
  the 5 class-A const-on-pointer sites fixed on the code lines - `const BYTE*` / `const INT*` / `const ::MSGBOXPARAMS*`
  over declaration + definition + `Library::Call<>` list = 15 sites in `user32.h`, `keyboard.cpp`, `dialog.cpp`, `text.cpp`;
  the fix is **not** symbol-neutral, all 5 mangled names move `QE..` -> `PE..`; two cross-library follow-ups recorded not
  done, and the negative control now rejects write-through with 2 x `C3892`), **U16** (7 Nov 2026, D21: the SDK version
  triple is pinned in `kernel32.h:22-30` at the values the unpinned build measured - `_WIN32_WINNT`/`WINVER 0x0A00`,
  `NTDDI_VERSION 0x0A000010 = NTDDI_WIN11_GE` - guarded `#ifndef` so an already-pinned consumer cannot re-define them,
  with a floor `#error` at `:33-35`; folder gate 45/45 exit 0 clean and the 113-source all-libraries log byte-identical
  to the pre-pin run. The pin is library-side, so a consumer that bypasses `kernel32.h` still shows U16's divergence -
  recorded, not fixed; the shrink macros U16's sibling cases name stay unpinned).
- Open: U1, U2, U4, U5, U7, U8, U9, U10, U12, U13, U17, U19, U20, U21, U22, U24, U25, U26, U27,
  P3, P4, P6. **One open task: T9** (D26 - U4's remaining 86 declared-but-undefined members as a tranche queue; **9a `strings.cpp` IN (D27)**, next step 9b `rawinput.cpp`. T1-T8 all CLOSED, T2 closed 8 Nov 2026, D25). Open the file the map names for an id you are acting on, never the whole tree.
- Decisions in force (D13-D18, incl. D17 x64-only and D18 purpose = full wrapper set) are in `docs/agent/DECISIONS.md`.

## Gate (identical every session)

`tmp/` does not persist between sessions, so the gate is the **command line**, not a script to be found - recreate the
wrapper by slug if it is missing. Compile the whole folder with:

    cl /nologo /c /std:c++20 /EHsc /GR /utf-8 /W4 /DQOR_WINAPIUSER `
       /Dqor_pp_ver_major=0 /Dqor_pp_ver_minor=5 /Dqor_pp_ver_patch=0 `
       /I c:\RAID0\Develop\linuxQOR /Foc:\RAID0\Develop\linuxQOR\tmp\obj\ <all 48 .cpp>

after `call vcvars64.bat` (`C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat`).
**The `/Fo` path must be attached to the switch** - measured 5 Nov 2026, the spaced form `/Fo c:\...\tmp\obj\` prints
`D9024` + `D9027` and writes every `.obj` into the **current directory** instead, so the run is no longer clean and the
dating rule below cannot be read.
Edited TU standalone at `/W4` exit 0, then the whole library **48/48** exit 0 with no diagnostics. Date the gate by
`tmp/obj/<tu>.obj` being newer than the source - a clean 48-TU log **cannot** be dated by its hash, because clean runs are
byte-identical (this cost a re-run in sessions 31 and 34). `cl /EP` after the last edit must still yield the expected
binding literals. `user32.h` untouched unless the session says otherwise; a library-header edit additionally needs the
116-source all-libraries gate as a **diff** against a pre-fix run (116 since `legacy.cpp` + `security.cpp` + `listbox.cpp` + `strings.cpp`; baseline 1 `C1083` at
`opengl/wgl.cpp:31`, 165 warning lines, exit **2** - the diff is the signal, never the exit code, which must be captured
inside a `.bat` because an inline `& echo %ERRORLEVEL%` expands at parse time and writes a false 0).

## There is no next TU - what a further session in this folder works from

All 38 review-queue TUs are closed, as are the ten coverage TUs `pointer.cpp` (6a) through `legacy.cpp` (6g), `security.cpp` (T1), `listbox.cpp` (T6) and `strings.cpp` (T9 9a), so `preflags.md`
pre-flags the T9 tranche 9b and no review TU: **U18 and T8 are CLOSED** (D19 put the last 6 names in scope, D20 gave
`wsprintfA`/`wsprintfW` a variadic wrapper) and the coverage backlog is empty - the 8 raw gap names all carry written exclusions.
A next session takes **T9 step 9b** (`rawinput.cpp` over `user32.h:543-549`, D26) or picks another open item the map above names (U1-U27 less the frozen ids, P3, P4, P6 -
**T1-T8 all CLOSED with T2 closed 8 Nov 2026 by D25, T9 open with 9a IN by D27**) or moves to
another `api_layer` folder. Architecture is settled - Windows is x64-only (D17) - and the gate above applies to any edit here.
