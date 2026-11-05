# winqor_api_user - source review: STATUS (per-session boot brief)

Last updated 5 Nov 2026 - **session 36 (`windowprocedure.cpp`) CLOSED and committed (`f4334e05`); 1 TU left**:
`winqor_api_user.cpp` (the module glue, 14 lines). Reviewing
`src/platform/os/windows/api_layer/user` (the `User32` wrappers over `user32.dll`) for correctness, consistency, safety,
UB and conformance to the api_layer conventions. One translation unit per session, and **commit at close**: a clean
`git status` plus this file is all a session needs to resume after a context reset.

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
9. **Boot read-set = this file + `method.md` + `preflags.md` + the current TU** (about 30 KB of the folder's 91 markdown
   files / 788 KB, measured 5 Nov 2026). Never open an `/EP` capture or `tmp/_tmp_*_pp_*.txt`; never read a whole register;
   open `archive/` or `gotchas/sessions-*.md` only for a named TU or session. **This file stays under 160 lines** - the
   ceiling is structural, not a task at close: the queue holds one fixed row per source (38 rows), the protocol and the
   gate are fixed text, and a close adds no prose. A close that grows this file has broken rule 4.

## Resuming after a context reset - ask git, not the record

    git -C c:\RAID0\Develop\linuxQOR log --oneline -3 -- src/platform/os/windows/api_layer/user
    git -C c:\RAID0\Develop\linuxQOR status --porcelain -- src/platform/os/windows/api_layer/user
    git -C c:\RAID0\Develop\linuxQOR grep -n "=== api_layer conventions" -- src/platform/os/windows/api_layer/user

An empty second command under a `close session N` head means session N is closed and there is nothing to re-derive. The
third lists every TU that carries its own banner (measured 5 Nov 2026: **5 of 38** - `text.cpp`, `window.cpp`,
`windowclass.cpp`, `windowprocedure.cpp`, `windowproperties.cpp`); every other closed TU is named in the queue column
below. If `git status` is not empty, the unfinished close **is** the state - finish it, do not re-measure it.

## Queue - all 38 sources, alphabetical (one row each; a close edits one `status` cell)

Generated once 5 Nov 2026 by a file scan (line count of the `.cpp`, presence of `//=== api_layer conventions`, presence of
`archive/tu/<name>.md`). Not regenerated - the queue is complete, so nothing can drift except the one cell being closed.
`lines` counts the source as it stands now, including its review banner.

| TU | lines | narrative | status |
|---|--:|---|---|
| `accelerator.cpp` | 60 | `archive/tu/accelerator.md` | closed |
| `accessibility.cpp` | 50 | `archive/tu/accessibility-timer.md` | closed |
| `brush.cpp` | 29 | `archive/tu/brush.md` | closed |
| `caret.cpp` | 78 | `archive/tu/caret.md` | closed |
| `clipboard.cpp` | 178 | `archive/tu/clipboard.md` | closed |
| `combobox.cpp` | 59 | `archive/tu/combobox.md` | closed |
| `console.cpp` | 50 | `archive/tu/console.md` | closed |
| `coordtransform.cpp` | 57 | `archive/tu/coordtransform.md` | closed |
| `cursor.cpp` | 166 | `archive/tu/cursor.md` | closed |
| `desktop.cpp` | 174 | `archive/tu/desktop.md` | closed |
| `devicecontext.cpp` | 115 | `archive/tu/devicecontext.md` | closed |
| `devicemanagement.cpp` | 87 | `archive/tu/devicemanagement.md` | closed |
| `dialog.cpp` | 192 | `archive/tu/dialog.md` | closed |
| `errorhandling.cpp` | 92 | `archive/tu/errorhandling.md` | closed |
| `filledshapes.cpp` | 99 | `archive/tu/filledshapes.md` | closed |
| `help.cpp` | 108 | `archive/session-log*.md` | closed |
| `hook.cpp` | 108 | `archive/session-log*.md` | closed |
| `icon.cpp` | 169 | `archive/session-log*.md` | closed |
| `ime.cpp` | 55 | `archive/session-log*.md` | closed |
| `keyboard.cpp` | 316 | `archive/session-log*.md` | closed |
| `mdi.cpp` | 112 | `archive/tu/mdi.md` | closed |
| `menu.cpp` | 355 | `archive/tu/menu.md` | closed |
| `message.cpp` | 274 | `archive/tu/message.md` | closed |
| `monitor.cpp` | 162 | `archive/tu/monitor.md` | closed |
| `mouse.cpp` | 167 | `archive/tu/mouse.md` | closed |
| `painting.cpp` | 261 | `archive/tu/painting.md` | closed |
| `processthread.cpp` | 122 | `archive/tu/processthread.md` | closed |
| `rectangle.cpp` | 174 | `archive/tu/rectangle.md` | closed |
| `resource.cpp` | 41 | `archive/tu/resource.md` | closed |
| `scrollbar.cpp` | 156 | `archive/tu/scrollbar.md` | closed |
| `systemshutdown.cpp` | 102 | `archive/tu/systemshutdown.md` | closed |
| `text.cpp` | 125 | banner `:16` | closed |
| `timer.cpp` | 33 | `archive/tu/accessibility-timer.md` | closed |
| `window.cpp` | 692 | banner `:16` | closed |
| `windowclass.cpp` | 158 | banner `:16` | closed |
| `windowprocedure.cpp` | 90 | banner `:16` | closed |
| `windowproperties.cpp` | 114 | banner `:16` | closed |
| `winqor_api_user.cpp` | 14 | - | **next (session 37)** |

**37 of 38 sources reviewed** in sessions 1-36 (some sessions covered two TUs; sessions 16-20 exist nowhere outside the
session-log rows). History is write-once and is not boot material: `archive/session-log*.md` (rows plus verdict detail)
and `archive/tu/<tu>.md` (narratives for sessions 1-15, 21-33). Do not append to either.

## Open items - id to file (derivable; there is no index to maintain)

- `U<n>` -> `registers/u<n>.md` | `P1`-`P5` -> `registers/prescan.md` | `P6`, `U27` -> `registers/p6-u27.md` |
  `G1`-`G6` -> `registers/ground-rules.md` | `T1`-`T7` -> `registers/tasks.md` | folder totals -> `registers/census.md`.
- Closed / frozen - append nothing: **P1, P2, P5** (4 Nov 2026: 428 bindings, 0 unresolvable), **U11** (session 29,
  3 Nov 2026), **U14** (D15), **U15** (D14), **U6**, **U3**.
- Open: U1, U2, U4, U5, U7, U8, U9, U10, U12, U13, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25, U26, **U28**,
  P3, P4, P6, T1-T7. Each statement and its measured detail is in the file the map names - open that file, never the tree.
- Decisions in force (D13, the return-check policy, D14, D15) are in `docs/agent/DECISIONS.md` and are not restated here.

## Gate (identical every session)

`tmp/` does not persist between sessions, so the gate is the **command line**, not a script to be found - recreate the
wrapper by slug if it is missing. Compile the whole folder with:

    cl /nologo /c /std:c++20 /EHsc /GR /utf-8 /W4 /DQOR_WINAPIUSER `
       /Dqor_pp_ver_major=0 /Dqor_pp_ver_minor=5 /Dqor_pp_ver_patch=0 `
       /I c:\RAID0\Develop\linuxQOR /Fo c:\RAID0\Develop\linuxQOR\tmp\obj\ <all 38 .cpp>

after `call vcvars64.bat` (`C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat`).
Edited TU standalone at `/W4` exit 0, then the whole library **38/38** exit 0 with no diagnostics. Date the gate by
`tmp/obj/<tu>.obj` being newer than the source - a clean 38-TU log **cannot** be dated by its hash, because clean runs are
byte-identical (this cost a re-run in sessions 31 and 34). `cl /EP` after the last edit must still yield the expected
binding literals. `user32.h` untouched unless the session says otherwise; a library-header edit additionally needs the
106-source all-libraries gate, read as a diff against a pre-fix run.

## Pre-flags for the next TU (session 37) - in `preflags.md`

`preflags.md` is written wholesale at this close, read once at boot, replaced wholesale at the next close. The pre-flag
body that stood here is retired: its heading named `winqor_api_user.cpp` while its bullets measured
`windowprocedure.cpp` (at pre-banner line numbers `:16`, `:23`), because the section was edited in place at the previous
close. Nothing is lost - the measurement survives in the `windowprocedure.cpp` banner, and the superseded text is in git
at `f4334e05`, `status.md:104`-`:134`.


