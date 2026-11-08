# Active Context

Last updated: 7 Nov 2026 - **T8 steps 1-6 are done, including the last tranche 6g: 6a `pointer.cpp` (32), 6b `dpi.cpp` (21),
6c `touch.cpp` (14), 6d `displayconfig.cpp` (7), 6e `powermanagement.cpp` + `CancelShutdown` (3), 6f `misc.cpp` (24 names / 23
members) and 6g `legacy.cpp` (6 names / 5 members), so `src/platform/os/windows/api_layer/user` is 45 sources / 632 members /
535 bindings.** **U18 and T8 are CLOSED** (D19 put the last 6 names in scope, D20 gave `wsprintfA`/`wsprintfW` a variadic
wrapper; per-name dispositions in `.../api_layer/user/registers/u18-coverage.md`, measurement pre-flags in
`.../api_layer/user/preflags.md`) and the backlog is empty - the 8 raw gap names all carry written exclusions. The 38-source
review queue closed at session 37 and the seven coverage TUs each carry their own review banner, so there is no next review TU.
Session 37 fixed nothing - the TU is the library's module glue (0 wrappers, 0 bindings) and measured clean - and its
durable findings, including one measured build-system hazard left deliberately unregistered, are in its own review
banner at `winqor_api_user.cpp:4-72`.

**The record protocol turned over twice in two closes: v2 at session 34, v3 after session 36.** Session 36 measured the
TU, passed the compile gate and wrote its banner, then lost the remainder of the session to the record write pass and
reached a compaction without committing. The four measured causes were: retired rules left as imperatives inside the live
procedure file, per-session prose in the one file every close has to edit, a line ceiling the file had already broken, and
an uncommitted close. v3 fixes all four - the close **is** the commit, every artefact is written once and never read back,
`status.md` carries only tables, and pre-flags moved to their own wholesale-replaced `preflags.md`. The superseded
procedure text is verbatim at `src/platform/os/windows/api_layer/user/archive/protocol-v1-2026-11-05.md`.
Per-session narrative belongs nowhere in the memory bank: it is the review banner at the top of each reviewed `.cpp`.

## Current focus: winqor_api_user - review queue, T8, U18 all closed and T1 CLOSED 8 Nov 2026; the backlog is empty

- **T1 CLOSED 8 Nov 2026:** a new `security.cpp` TU defines `GetUserObjectSecurity` (export 1DB) and `SetUserObjectSecurity`
  (export 375), bound bare `qor_pp_useswinapi`, and `user32.h:760-761` gains `static` on both - closing both halves of the
  pair's defect (U4 declared-but-undefined -> defined, U18 non-static -> static). Folder is now 46 sources / 633 members
  (536 static, 97 non-static) / 537 bindings; coverage unchanged at 757 of 1,018 (the pair were already declared, so already
  covered). Gate: `security.cpp` standalone /W4 exit 0, folder 46/46 exit 0, and the shared-header `user32.h` edit diffed 0
  new / 0 gone against the standing 113 -> 114-source all-libraries baseline (exit 2 both sides). Narrative is the banner at
  `security.cpp:9-36`; per-name disposition in `registers/tasks.md` T1.
- Reviewing `src/platform/os/windows/api_layer/user` (class `qor::win::api::User32` over `user32.dll`) for
  correctness, consistency, safety, UB and convention conformance. **Protocol: exactly one translation unit
  per session**; small unambiguous defects fixed in place, everything behavioural recorded as an open item.
- **Session 37 (`winqor_api_user.cpp`) CLOSED 5 Nov 2026** - narrative and all measurements: the banner at
  `winqor_api_user.cpp:4-72`; it is committed, so nothing about it is to be re-derived.
- **The queue is empty - there is no next TU.** The folder has a purpose statement now
  (`src/platform/os/windows/api_layer/user/purpose.md`, 5 Nov 2026: "a full set of wrapper functions for the exports of the
  User32.dll Windows system library"), recorded as policy **D18** in `docs/agent/DECISIONS.md`; it changes the acceptance
  criterion - a wrapper's published signature is the SDK prototype of the export it binds, and an unwrapped export is a
  gap. The work that follows is **T8** (`.../api_layer/user/registers/tasks.md`; measured sites in `.../registers/u18.md`):
  1 return type, 4 parameter types, **102** parameter names (not the 16 the per-TU records added up to), 2 unwrapped
  `*ForPerMonitorDPI` exports, then a **109-name** coverage backlog. Steps 1-5 are in as of 6 Nov 2026 - the names pass
  renamed 101 of the 102 across header and definition together, and step 2's consumer fix only landed when that rename
  made the `UINT_PTR` change fail to compile in `ui/gui/controllers/i_m/menu.cpp`. Steps **6a**-**6g** are all in as of
  7 Nov 2026, so the backlog is **empty** (D19 decided the last 6 names - `GetWindowWord`, `SetWindowWord`, `SetMessageQueue`,
  `DragObject`, `wsprintfA`/`wsprintfW` - IN scope, and 6g wrapped them as 5 members: four ordinary ones in `legacy.cpp` plus
  the folder's one header-inline `wsprintfT` member over `Library::varCall`, D20; scope decided 5 Nov: documented
  exports only, superseded families excluded - the 253 prototype-less exports are out; per-name dispositions in
  `.../registers/u18-coverage.md`). Other leads - open items, the
  T1-T6
  tasks, D17 and what it closed, the unregistered version-string hazard - remain in
  `src/platform/os/windows/api_layer/user/preflags.md`; `status.md` still carries the queue table, the id map and the
  compile gate for any future edit here.

## Decisions in force (pointer only - not restated here)
 - Project wide decisions are recorded in `docs/agent/DECISIONS.md`. The memory bank deliberately does not duplicate their text.
- **Rule adopted 2 Nov 2026 (user):** this file carries the current focus, and pointers **never a per-session narrative**. At each session close, the outgoing "recent changes" entry is either reduced to the two or three
lines a new session needs to start, or moved to `memory-bank/archive/`. Session detail belongs in the project record tree
(item 3), not here.

## Where the record lives (boot order)

1. `src/platform/os/windows/api_layer/user/status.md` - the review boot file: the protocol, the queue of all 45 sources,
   the id-to-file map and the compile gate. With `method.md`, `preflags.md` and the current TU this is the **whole** boot
   read-set (protocol v3): about 30 KB. Whether an earlier session finished is answered by `git log --oneline` and a clean
   `git status` for that folder - never by reading the record deeper.
2. `src/platform/os/windows/api_layer/user/registers/` - one file per open item; open only the file for an id you are
   acting on, and read its entry head. Never the whole tree. `../method.md` holds the procedure, `../tooling.md` the
   instrument index and `../gotchas.md` the caveats that index points at.
3. `src/platform/os/windows/api_layer/user/archive/` - per-TU narratives (`tu/<name>.md`, sessions 1-15 and 21-33) and the
   session-log rows (the only narrative for sessions 16-20). Frozen, authoritative history; **not boot material**. Since
   session 34 the session narrative is the review banner at the top of each reviewed `.cpp`.
4. `memory-bank/archive/` - session records moved out of this file (index: `memory-bank/archive/README.md`; **not boot
   material** - do not read it at session start).

**If previous activities need to be researched they are in the git history**

