# Active Context

Last updated: 9 Nov 2026 - **T1-T8 are CLOSED and T9 is OPEN with 9f CLOSED (D32: a new `bitmap.cpp` TU defines the 1 `//Bitmap functions` member of `user32.h:776` (`LoadBitmap`) with 1 `qor_pp_useswinapiAW` binding and 0 bare - the AW form is **mandatory** there because the fresh `dumpbin` dump reads no row for the bare stem and the pair `LoadBitmapA` 254 / `LoadBitmapW` 255, `user32.h:21` `#undef`s the stem, and the SDK declares only the switch at `um\WinUser.h:10598`-`:10602` - and `user32.h:776` gains `static` and nothing else); next step 9g `printing.cpp` over `user32.h:783` (`PrintWindow`, bare binding, export hint 2B2)**; the folder is 53 sources / 634 member-declaration lines (599 static, 35 non-static) / 598 bindings, covered 759 of 1,018.** The entry below is the 6g close (7 Nov 2026) and its figures are as they stood then.
**T8 steps 1-6 are done, including the last tranche 6g: 6a `pointer.cpp` (32), 6b `dpi.cpp` (21),
6c `touch.cpp` (14), 6d `displayconfig.cpp` (7), 6e `powermanagement.cpp` + `CancelShutdown` (3), 6f `misc.cpp` (24 names / 23
members) and 6g `legacy.cpp` (6 names / 5 members), so `src/platform/os/windows/api_layer/user` was 45 sources / 632 members /
535 bindings at that close.** **U18 and T8 are CLOSED** (D19 put the last 6 names in scope, D20 gave `wsprintfA`/`wsprintfW` a variadic
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

## Current focus: winqor_api_user - review queue, T8, U18 all closed, T1-T8 CLOSED and **T9 open with 9f CLOSED (next step 9g)** 9 Nov 2026; the coverage backlog is empty, U4's link gap is not

- **T9 OPEN, 9f CLOSED (D26 opened the queue 8 Nov 2026; D27 landed 9a, D28 landed 9b, D29 landed 9c, D30 landed 9d, D31 landed 9e and D32 landed 9f on 9 Nov 2026):** U4's remaining population is **35 non-static
  member declarations with no definition** (`_tmp_static_census.ps1` post-9f: 634 decls / 599 static / 35 non-static / 0 defined-but-not-static;
  `_tmp_decl_no_def_repo_scan.ps1` reads **1** of them, missing the DDE block's 34 `) const;` lines). **1 remains in D18 scope**
  and it names a real export - step 9g `printing.cpp` (1), next step **9g `printing.cpp` over
  `user32.h:783`**; **9h is the DDE block (34) and is blocked on a wrap-or-delete user decision** (D18 excludes the family by name
  and 0 of its 34 are declared in `um\WinUser.h`). Coverage stays 759 of 1,018 throughout - these close `LNK2019` link gaps,
  never coverage gaps. 9f put `static` on `user32.h:776` and defined the 1 member in `bitmap.cpp` with 1 `qor_pp_useswinapiAW`
  binding and 0 bare (the bare stem is not an export - the pair at hints 254/255 is - the stem is `#undef`'d at `user32.h:21`,
  and the SDK declares only the `#ifdef UNICODE` switch); 9e put `static` on `user32.h:779-780` and defined both in `synchronization.cpp` with 2 bare
  `qor_pp_useswinapi` bindings and 0 `qor_pp_useswinapiAW` (the members already carry the A/W suffix, so D13's binding literal
  is the exported name); 9b put `static` on `user32.h:543-549` and defined all 7 in `rawinput.cpp` with 6 bare
  `qor_pp_useswinapi` bindings and 1 `qor_pp_useswinapiAW` (`GetRawInputDeviceInfo` - bare stem `#undef`'d at `user32.h:16`,
  not exported, exports 1B7/1B8); 9c put `static` on `user32.h:753-756` and defined all 4 in `systeminfo.cpp` with 3 bare
  `qor_pp_useswinapi` bindings and 1 `qor_pp_useswinapiAW` (`SystemParametersInfo` - bare stem `#undef`'d at `user32.h:15`,
  not exported, exports 3A8/3AA); 9d put `static` on `user32.h:378-380` and defined all 3 in `buttoncontrol.cpp` with 3 bare
  `qor_pp_useswinapi` bindings and 0 `qor_pp_useswinapiAW` (none of the three stems is an SDK A/W switch - the switch probe
  reads 0 lines - and the table has no A/W row for any of them, the rows being bare at hints 42/47/234 and all six A/W
  spellings read MISSING in a live probe); the queue and its pre-flags live in `registers/tasks.md` T9 and `preflags.md`.
- **T6 CLOSED 8 Nov 2026 (D24):** a new `listbox.cpp` TU defines the three `//List Box functions` members - `DlgDirList`
  (exports C1/C4) and `DlgDirSelectEx` (C7/C8) bound `qor_pp_useswinapiAW`, `GetListBoxInfo` (export 174) bound bare - and
  `user32.h:552-554` gains `static` on all three, with `DlgDirSelectEx`'s declaration moved to the SDK's parameter names
  `hwndDlg`/`chCount`/`idListBox`. U4's and U18's instances for the trio are gone; coverage is unchanged at **759 of 1,018**
  (all three were already declared). Folder is now 47 sources / 634 member-declaration lines (548 static, 86 non-static) /
  547 bindings. Gate: `listbox.cpp` standalone /W4 exit 0, folder 47/47 exit 0, and the shared-header `user32.h` edit diffed
  0 new / 0 gone against the standing 114-source all-libraries baseline (post run 115 cpp echoes, 165 warnings = 165,
  166 diagnostics both sides, exit 2 both sides, the only line new in the post log being the `listbox.cpp` echo itself).
  Narrative is the banner at `listbox.cpp:9-68`; per-name disposition in `registers/tasks.md` T6.
- **T4 CLOSED 8 Nov 2026:** `help.cpp` gains `SetMenuContextHelpId` (export 34A) and `GetMenuContextHelpId` (export 17C),
  bound bare `qor_pp_useswinapi`, and `user32.h:744-745` gains `static` on both - closing both halves of the pair's defect
  (U4 declared-but-undefined -> defined, U18 non-static -> static). Folder is now 46 sources / 633 members (540 static,
  93 non-static) / 541 bindings; coverage unchanged at 757 of 1,018 (the pair were already declared, so already covered).
  Gate: `help.cpp` standalone /W4 exit 0, folder 46/46 exit 0, and the shared-header `user32.h` edit diffed 0 new / 0 gone
  against the standing 114-source all-libraries baseline (165 warnings = 165, exit 2 both sides). Narrative is the banner at
  `help.cpp:15-97`; per-name disposition in `registers/tasks.md` T4.
- **T3 CLOSED 8 Nov 2026:** `powermanagement.cpp` defines the Power Management pair (`RegisterPowerSettingNotification`
  export 2F1, `UnregisterPowerSettingNotification` export 3CB), bound bare `qor_pp_useswinapi`, + `static` at
  `user32.h:788-789`.
- **T1 CLOSED 8 Nov 2026:** a new `security.cpp` TU defines `GetUserObjectSecurity` (export 1DB) and `SetUserObjectSecurity`
  (export 375), bound bare `qor_pp_useswinapi`, and `user32.h:760-761` gains `static` on both - closing both halves of the
  pair's defect (U4 declared-but-undefined -> defined, U18 non-static -> static). Coverage unchanged at 757 of 1,018 (the
  pair were already declared, so already covered). Gate: `security.cpp` standalone /W4 exit 0, folder 46/46 exit 0, and the
  shared-header `user32.h` edit diffed 0 new / 0 gone against the standing 113 -> 114-source all-libraries baseline (exit
  2 both sides). Narrative is the banner at `security.cpp:9-36`; per-name disposition in `registers/tasks.md` T1.
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
  T2 task, D17 and what it closed, the unregistered version-string hazard - remain in
  `src/platform/os/windows/api_layer/user/preflags.md`; `status.md` still carries the queue table, the id map and the
  compile gate for any future edit here.

## Decisions in force (pointer only - not restated here)
 - Project wide decisions are recorded in `docs/agent/DECISIONS.md`. The memory bank deliberately does not duplicate their text.
- **Rule adopted 2 Nov 2026 (user):** this file carries the current focus, and pointers **never a per-session narrative**. At each session close, the outgoing "recent changes" entry is either reduced to the two or three
lines a new session needs to start, or moved to `memory-bank/archive/`. Session detail belongs in the project record tree
(item 3), not here.

## Where the record lives (boot order)

1. `src/platform/os/windows/api_layer/user/status.md` - the review boot file: the protocol, the queue of all 47 sources,
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

