# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 10 Nov 2026 - **U4 CLOSED (D26's T9 queue, closed at D34 - census 0 non-static / 0 declared-but-undefined, no code change); T9 is CLOSED (D34 - its last step 9h deleted the 34-member DDE block from `user32.h`, no TU added); 9g is CLOSED (D33): a new `printing.cpp` TU defines the 1 `//Printing functions` member of `user32.h:783` (`PrintWindow`) with 1 x bare `qor_pp_useswinapi` and 0 x `qor_pp_useswinapiAW` (the bare stem **IS** the export and the AW spelling is **unavailable**, not merely wrong: this session's fresh `dumpbin /exports C:\Windows\System32\user32.dll` dump reads the row `PrintWindow` ordinal 2197 hint `2B2` rva `0002F620` and **NO_ROW** for `PrintWindowA`/`PrintWindowW`/`PrintWindowEx`/`PrintWindowExA`/`PrintWindowExW`/`PrintWindowT`, the live probe reads RESOLVED `00007FFF2CBFF620` for the bare stem and MISSING for every other spelling, the SDK puts **no** `#define PrintWindow` switch on the stem over `um\WinUser.h` 10.0.26100.0, and the stem is **not** one of the nine `#undef`s at `user32.h:13`-`:21` - the INVERSE of 9f's `LoadBitmap`, whose AW form was mandatory), and `user32.h:783` gains `static` and nothing else - the declaration already spells the SDK's parameter names `hwnd`/`hdcBlt`/`nFlags`, so no mangled name moves.** 9f is CLOSED (D32), 9e is CLOSED (D31), 9d is CLOSED (D30), 9c is CLOSED (D29), 9b is CLOSED (D28), 9a is CLOSED (D27) and **all eight deferred tasks T1-T8 are CLOSED**; the 38 review-queue TUs stay closed from session 37. Their close detail - and the prior review phase that ended 7 Nov 2026 - lives read-only in `completed_work.md`, never here. **T9 is CLOSED (D34)**: the last step **9h deleted the 34-member `//Dynamic Data Exchange functions` block (was `user32.h:506`-`:540`) from `user32.h`** - 36 lines gone, header **855 -> 819**, no TU and no `CMakeLists.txt` change, the folder still 54 sources / 599 bindings. D34's measured basis: 0 of the 34 declared in `um\WinUser.h`, 27 in `um\ddeml.h` (whose 100 DDE lines carry **0** `const`, so the 34 `) const;` spellings copy no SDK prototype), 7 in neither header, the fresh x64 export dump reads 38 DDE rows (31 of the 34 names hit a row, 3 do not), 0 `User32::` uses of any of the 34 across `src`/`examples`/`test`, and coverage moves **759 -> 724** of 1,018 with the family's 35 exports landing in the no-prototype exclusion bucket (251 -> 286) while the 8 SDK-visible exclusions stay 8.

**Current folder figures (measured 9 Nov 2026 post-9h with `_tmp_static_census.ps1`, then `_tmp_decl_no_def_repo_scan.ps1`, then `_tmp_u18_coverage4.ps1`):** 54 sources / 600 member-declaration lines (600 static, **0 non-static**) / 599 binding lines; coverage **724 of 1,018** (8 raw with written exclusions, 286 with no prototype); 0 defined-but-not-static, 0 defined-without-a-declaration. `user32.h` is **819 lines** - 9h deleted 36. The census, the gate diffs and the prior-close state recorded at the closed closes are in `completed_work.md`.

Reviewing `src/platform/os/windows/api_layer/user` (the `User32` wrappers over `user32.dll`) for correctness,
consistency, safety, UB and conformance to the api_layer conventions. One translation unit per session, and **commit at
close**: a clean `git status` plus this file is all a session needs to resume after a context reset.

**Purpose (authoritative: `purpose.md`, added 5 Nov 2026; policy D18 + same-day scope addendum in
`docs/agent/DECISIONS.md`): the library intends a full set of wrapper functions for the exports of `user32.dll` - scoped to
the exports `winuser.h` declares, superseded families excluded.** A wrapper is judged against the export it binds, and an
unwrapped declared export is a gap (**8 names** remain raw after T8 steps 5 and 6a-6g, listed in
`registers/u18-coverage.md`, and **all 8 carry written exclusions** - 7 superseded twins whose `*Ptr`/`Ex` replacement this
library wraps and 1 instrument artifact with no prototype). The backlog is complete: every one of the 109 names is wrapped
or excluded, so **U18 and T8 are CLOSED** and there is no actionable T8 lead left. The backlog's close record - the 8
exclusions, D19's scope call and D20's variadic wrapper - is in `completed_work.md`.

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
   open `archive/`, `gotchas/sessions-*.md` or the read-only `completed_work.md` only to research a named TU, session or closed task. **This file is 179 lines** - the
   ceiling is structural, not a task at close: the queue holds one fixed row per source (54 rows since `printing.cpp`
   joined on 9 Nov 2026), the protocol and the gate are fixed text, a close adds no prose and moves closed-work prose to `completed_work.md`. Growing this file breaks 4.

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
| `bitmap.cpp` | 136 | banner `:9` | closed (T9 9f, added 9 Nov 2026) |

| `brush.cpp` | 28 | `archive/tu/brush.md` | closed |
| `buttoncontrol.cpp` | 117 | banner `:9` | closed (T9 9d, added 9 Nov 2026) |
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
| `printing.cpp` | 125 | banner `:9` | closed (T9 9g, added 9 Nov 2026) |
| `processthread.cpp` | 122 | `archive/tu/processthread.md` | closed |
| `rawinput.cpp` | 157 | banner `:9` | closed (T9 9b, added 9 Nov 2026) |
| `rectangle.cpp` | 173 | `archive/tu/rectangle.md` | closed |
| `resource.cpp` | 40 | `archive/tu/resource.md` | closed |
| `scrollbar.cpp` | 155 | `archive/tu/scrollbar.md` | closed |
| `security.cpp` | 66 | banner `:9` | closed (T1, added 8 Nov 2026) |
| `strings.cpp` | 328 | banner `:13` | closed (T9 9a, added 9 Nov 2026) |
| `synchronization.cpp` | 133 | banner `:9` | closed (T9 9e, added 9 Nov 2026) |
| `systeminfo.cpp` | 132 | banner `:9` | closed (T9 9c, added 9 Nov 2026) |

| `systemshutdown.cpp` | 124 | `archive/tu/systemshutdown.md` | closed |
| `text.cpp` | 125 | banner `:16` | closed |
| `timer.cpp` | 32 | `archive/tu/accessibility-timer.md` | closed |
| `touch.cpp` | 249 | banner `:9` | closed (T8 6c, added 6 Nov 2026) |
| `window.cpp` | 705 | banner `:16` | closed |
| `windowclass.cpp` | 160 | banner `:16` | closed |
| `windowprocedure.cpp` | 89 | banner `:16` | closed |
| `windowproperties.cpp` | 113 | banner `:16` | closed |
| `winqor_api_user.cpp` | 83 | banner `:4` | closed |

**38 of 38 review-queue sources closed** in sessions 1-37 - the queue is empty, the folder review is complete, and the
folder is **54 sources**: the sixteen coverage rows are the only ones not from a review session, and their step labels
and join dates are in `completed_work.md`. Each coverage TU's narrative is its own banner. History is write-once and
not boot material: `archive/session-log*.md`, `archive/tu/<tu>.md`, `completed_work.md`. Append nothing.

## Open items - id to file (derivable; there is no index to maintain)

- `U<n>` -> `registers/u<n>.md` | `P1`-`P5` -> `registers/prescan.md` | `P6`, `U27` -> `registers/p6-u27.md` |
  `G1`-`G6` -> `registers/ground-rules.md` | `T1`-`T8` -> `registers/tasks.md` | folder totals -> `registers/census.md`.
- Closed / frozen - append nothing, and their close records are not here: the id list with the record each closure left
  lives read-only in `completed_work.md`. A closed or frozen id is never opened at session start (protocol 7).
- Open: U1, U2, U5, U7, U8, U9, U10, U12, U13, U17, U19, U20, U21, U22, U24, U25, U26, U27,
  P3, P4, P6. **No open task in the T queue** (T1-T9 all CLOSED - D34 closed T9's last step 9h by deleting the DDE block). Open the file the map names for an id you are acting on, never the whole tree.
- Decisions in force (D13-D18, incl. D17 x64-only and D18 purpose = full wrapper set) are in `docs/agent/DECISIONS.md`.

## Gate (identical every session)

`tmp/` does not persist between sessions, so the gate is the **command line**, not a script to be found - recreate the
wrapper by slug if it is missing. Compile the whole folder with:

    cl /nologo /c /std:c++20 /EHsc /GR /utf-8 /W4 /DQOR_WINAPIUSER `
       /Dqor_pp_ver_major=0 /Dqor_pp_ver_minor=5 /Dqor_pp_ver_patch=0 `
       /I c:\RAID0\Develop\linuxQOR /Foc:\RAID0\Develop\linuxQOR\tmp\obj\ <all 54 .cpp>

after `call vcvars64.bat` (`C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat`).
**The `/Fo` path must be attached to the switch** - measured 5 Nov 2026, the spaced form `/Fo c:\...\tmp\obj\` prints
`D9024` + `D9027` and writes every `.obj` into the **current directory** instead, so the run is no longer clean and the
dating rule below cannot be read.
Edited TU standalone at `/W4` exit 0, then the whole library **54/54** exit 0 with no diagnostics. Date the gate by
`tmp/obj/<tu>.obj` being newer than the source - a clean 54-TU log **cannot** be dated by its hash, because clean runs are
byte-identical (this cost a re-run in sessions 31 and 34). `cl /EP` after the last edit must still yield the expected
binding literals. `user32.h` untouched unless the session says otherwise; a library-header edit additionally needs the
122-source all-libraries gate as a **diff** against a pre-fix run (122 since `legacy.cpp` + `security.cpp` + `listbox.cpp` + `strings.cpp` + `rawinput.cpp` + `systeminfo.cpp` + `buttoncontrol.cpp` + `synchronization.cpp` + `bitmap.cpp` + `printing.cpp`; baseline 1 `C1083` at
`opengl/wgl.cpp:31`, 165 warning lines, exit **2** - the diff is the signal, never the exit code, which must be captured
inside a `.bat` because an inline `& echo %ERRORLEVEL%` expands at parse time and writes a false 0).

## There is no next TU - what a further session in this folder works from

All 38 review-queue TUs and the sixteen coverage TUs are closed (the TU list and its step labels: `completed_work.md`), so `preflags.md`
names **no** blocked tranche and no review TU. The coverage backlog is empty - every raw gap name carries a written exclusion.
T9 is CLOSED (D34) and its queue is empty, so a next session picks another open item the map above names (U1-U27 less the frozen ids, P3, P4, P6) or moves to
another `api_layer` folder. Architecture is settled - Windows is x64-only (D17) - and the gate above applies to any edit here.
